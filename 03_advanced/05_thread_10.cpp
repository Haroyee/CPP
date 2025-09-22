#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <memory>
#include <atomic>
#include <type_traits>
#include <any>
#include <unordered_map>
#include <cstring>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

// 异常类
class ThreadPoolException : public std::runtime_error
{
public:
    explicit ThreadPoolException(const std::string &msg) : std::runtime_error(msg) {}
};

// 网络消息结构
struct NetworkMessage
{
    int type;
    std::vector<uint8_t> data;
};

// 类型擦除的任务包装器
class TaskWrapper
{
public:
    virtual ~TaskWrapper() = default;
    virtual void execute() = 0;
};

// 线程池主类
class AdvancedThreadPool
{
public:
    explicit AdvancedThreadPool(size_t num_threads, int port = 8080)
        : stop_(false), port_(port), running_tasks_(0)
    {

// 初始化Winsock (Windows only)
#ifdef _WIN32
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            throw ThreadPoolException("WSAStartup failed");
        }
#endif

        // 创建工作线程
        for (size_t i = 0; i < num_threads; ++i)
        {
            workers_.emplace_back([this]
                                  {
                for(;;) {
                    std::unique_ptr<TaskWrapper> task;
                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex_);
                        this->condition_.wait(lock, [this] {
                            return this->stop_ || !this->tasks_.empty();
                        });
                        
                        if(this->stop_ && this->tasks_.empty()) {
                            return;
                        }
                        
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    
                    {
                        std::lock_guard<std::mutex> lock(this->running_mutex_);
                        running_tasks_++;
                    }
                    
                    task->execute();
                    
                    {
                        std::lock_guard<std::mutex> lock(this->running_mutex_);
                        running_tasks_--;
                        completion_condition_.notify_all();
                    }
                } });
        }

        // 启动网络服务线程
        network_thread_ = std::thread(&AdvancedThreadPool::networkServer, this, port_);
    }

    template <typename Func, typename... Args>
    auto enqueue(Func &&func, Args &&...args) -> std::future<typename std::invoke_result_t<Func, Args...>>
    {
        using return_type = typename std::invoke_result_t<Func, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            [func = std::forward<Func>(func), args = std::make_tuple(std::forward<Args>(args)...)]() mutable
            {
                return std::apply(func, std::move(args));
            });

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            if (stop_)
            {
                throw ThreadPoolException("enqueue on stopped ThreadPool");
            }

            // 直接使用TaskWrapperImpl，不需要额外的Task类
            tasks_.emplace(new TaskWrapperImpl<std::packaged_task<return_type()>>([task]()
                                                                                  { (*task)(); }));
        }

        condition_.notify_one();
        return res;
    }

    // 等待所有任务完成
    void waitAll()
    {
        std::unique_lock<std::mutex> lock(running_mutex_);
        completion_condition_.wait(lock, [this]()
                                   { return tasks_.empty() && running_tasks_ == 0; });
    }

    // 获取运行中任务数量
    size_t runningTasks() const
    {
        std::lock_guard<std::mutex> lock(running_mutex_);
        return running_tasks_;
    }

    // 获取等待中任务数量
    size_t pendingTasks() const
    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return tasks_.size();
    }

    // 停止线程池
    void stop()
    {
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            stop_ = true;
        }

        condition_.notify_all();
        for (std::thread &worker : workers_)
        {
            if (worker.joinable())
            {
                worker.join();
            }
        }

        // 停止网络服务
        stop_network_ = true;
        if (network_thread_.joinable())
        {
            network_thread_.join();
        }

// 清理Winsock (Windows only)
#ifdef _WIN32
        WSACleanup();
#endif
    }

    // 注册网络消息处理函数
    template <typename Func>
    void registerNetworkHandler(int message_type, Func &&handler)
    {
        std::lock_guard<std::mutex> lock(handler_mutex_);
        network_handlers_[message_type] = std::function<void(const std::vector<uint8_t> &)>(handler);
    }

    // 发送网络消息到指定地址
    bool sendNetworkMessage(const NetworkMessage &message, const std::string &ip, int port)
    {
#ifdef _WIN32
        SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sock == INVALID_SOCKET)
        {
            return false;
        }
#else
        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
        {
            return false;
        }
#endif

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

        // 先发送消息类型
        int type = htonl(message.type);
        if (sendto(sock, (const char *)&type, sizeof(type), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return false;
        }

        // 发送数据长度和数据
        uint32_t data_len = htonl(message.data.size());
        if (sendto(sock, (const char *)&data_len, sizeof(data_len), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return false;
        }

        if (!message.data.empty() &&
            sendto(sock, (const char *)message.data.data(), message.data.size(), 0,
                   (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
#ifdef _WIN32
            closesocket(sock);
#else
            close(sock);
#endif
            return false;
        }

#ifdef _WIN32
        closesocket(sock);
#else
        close(sock);
#endif
        return true;
    }

    ~AdvancedThreadPool()
    {
        if (!stop_)
        {
            stop();
        }
    }

private:
    // 用于包装std::packaged_task的TaskWrapper实现
    template <typename T>
    class TaskWrapperImpl : public TaskWrapper
    {
    public:
        explicit TaskWrapperImpl(T &&func) : func_(std::move(func)) {}
        void execute() override { func_(); }

    private:
        T func_;
    };

    // 网络服务器函数
    void networkServer(int port)
    {
#ifdef _WIN32
        SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd == INVALID_SOCKET)
        {
            std::cerr << "Failed to create socket: " << WSAGetLastError() << std::endl;
            return;
        }
#else
        int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0)
        {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }
#endif

        struct sockaddr_in server_addr;
        memset(&server_addr, 0, sizeof(server_addr));
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY;
        server_addr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            std::cerr << "Bind failed" << std::endl;
#ifdef _WIN32
            closesocket(sockfd);
#else
            close(sockfd);
#endif
            return;
        }

// 设置超时以便定期检查stop_network_
#ifdef _WIN32
        DWORD tv = 1000; // 1秒
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
#else
        struct timeval tv;
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
#endif

        while (!stop_network_)
        {
            NetworkMessage message;
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);

            // 接收消息类型
            int type;
#ifdef _WIN32
            int n = recvfrom(sockfd, (char *)&type, sizeof(type), 0,
                             (struct sockaddr *)&client_addr, &client_len);
#else
            int n = recvfrom(sockfd, &type, sizeof(type), 0,
                             (struct sockaddr *)&client_addr, &client_len);
#endif

            if (n < 0)
            {
#ifdef _WIN32
                if (WSAGetLastError() == WSAETIMEDOUT)
                {
                    // 超时，继续循环检查stop_network_
                    continue;
                }
                std::cerr << "Error receiving type: " << WSAGetLastError() << std::endl;
#else
                if (errno == EWOULDBLOCK || errno == EAGAIN)
                {
                    // 超时，继续循环检查stop_network_
                    continue;
                }
                std::cerr << "Error receiving type" << std::endl;
#endif
                continue;
            }
            message.type = ntohl(type);

            // 接收数据长度
            uint32_t data_len;
#ifdef _WIN32
            n = recvfrom(sockfd, (char *)&data_len, sizeof(data_len), 0,
                         (struct sockaddr *)&client_addr, &client_len);
#else
            n = recvfrom(sockfd, &data_len, sizeof(data_len), 0,
                         (struct sockaddr *)&client_addr, &client_len);
#endif
            if (n < 0)
            {
                std::cerr << "Error receiving data length" << std::endl;
                continue;
            }
            data_len = ntohl(data_len);

            // 接收数据
            if (data_len > 0)
            {
                message.data.resize(data_len);
#ifdef _WIN32
                n = recvfrom(sockfd, (char *)message.data.data(), data_len, 0,
                             (struct sockaddr *)&client_addr, &client_len);
#else
                n = recvfrom(sockfd, message.data.data(), data_len, 0,
                             (struct sockaddr *)&client_addr, &client_len);
#endif
                if (n < 0)
                {
                    std::cerr << "Error receiving data" << std::endl;
                    continue;
                }
            }

            // 处理消息
            std::function<void(const std::vector<uint8_t> &)> handler;
            {
                std::lock_guard<std::mutex> lock(handler_mutex_);
                auto it = network_handlers_.find(message.type);
                if (it != network_handlers_.end())
                {
                    handler = it->second;
                }
            }

            if (handler)
            {
                // 将消息处理任务加入线程池
                this->enqueue([handler, message]()
                              { handler(message.data); });
            }
        }

#ifdef _WIN32
        closesocket(sockfd);
#else
        close(sockfd);
#endif
    }

    std::vector<std::thread> workers_;
    std::thread network_thread_;
    std::queue<std::unique_ptr<TaskWrapper>> tasks_;

    mutable std::mutex queue_mutex_;
    mutable std::mutex running_mutex_;
    mutable std::mutex handler_mutex_;

    std::condition_variable condition_;
    std::condition_variable completion_condition_;

    std::atomic<bool> stop_;
    std::atomic<bool> stop_network_{false};
    std::atomic<size_t> running_tasks_;

    int port_;
    std::unordered_map<int, std::function<void(const std::vector<uint8_t> &)>> network_handlers_;
};