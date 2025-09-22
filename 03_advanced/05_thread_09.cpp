#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <atomic>
#include <memory>
#include <chrono>
#include <unordered_map>
#include <type_traits>

namespace ThreadPool
{

    // 任务优先级枚举
    enum class Priority
    {
        High,
        Normal,
        Low
    };

    // 线程状态枚举
    enum class ThreadState
    {
        Idle,
        Running,
        Stopped
    };

    // 线程信息结构体
    struct ThreadInfo
    {
        std::thread::id id;
        ThreadState state;
        std::string currentTask;
    };

    class ThreadPool
    {
    private:
        // 任务包装类
        struct TaskWrapper
        {
            // 类型擦除基类
            struct Base
            {
                virtual ~Base() = default;
                virtual void execute() = 0;
            };

            // 模板派生类
            template <typename F>
            struct Impl : Base
            {
                F f;
                Impl(F &&f_) : f(std::move(f_)) {}
                void execute() override { f(); }
            };

            std::unique_ptr<Base> impl;
            Priority priority;
            std::string name;

            TaskWrapper() = default;

            template <typename F>
            TaskWrapper(F &&f, Priority p, std::string n)
                : impl(new Impl<F>(std::move(f))), priority(p), name(std::move(n)) {}

            TaskWrapper(TaskWrapper &&other) noexcept
                : impl(std::move(other.impl)), priority(other.priority), name(std::move(other.name)) {}

            TaskWrapper &operator=(TaskWrapper &&other) noexcept
            {
                if (this != &other)
                {
                    impl = std::move(other.impl);
                    priority = other.priority;
                    name = std::move(other.name);
                }
                return *this;
            }

            // 删除拷贝构造函数和拷贝赋值运算符
            TaskWrapper(const TaskWrapper &) = delete;
            TaskWrapper &operator=(const TaskWrapper &) = delete;

            void operator()()
            {
                if (impl)
                    impl->execute();
            }

            // 优先级比较
            bool operator<(const TaskWrapper &other) const
            {
                return static_cast<int>(priority) < static_cast<int>(other.priority);
            }
        };

        // 线程池成员变量
        std::vector<std::thread> workers;
        std::priority_queue<TaskWrapper> tasks;
        std::unordered_map<std::thread::id, ThreadInfo> threadInfos;

        mutable std::mutex queueMutex;
        std::condition_variable condition;
        std::condition_variable completionCondition;

        std::atomic<bool> stop;
        std::atomic<size_t> activeTasks;
        std::atomic<size_t> totalTasksProcessed;

        size_t minThreads;
        size_t maxThreads;
        std::chrono::seconds idleThreadTimeout;

    public:
        // 构造函数
        ThreadPool(size_t minThreads = std::thread::hardware_concurrency(),
                   size_t maxThreads = std::thread::hardware_concurrency() * 2,
                   std::chrono::seconds idleTimeout = std::chrono::seconds(30))
            : stop(false), activeTasks(0), totalTasksProcessed(0),
              minThreads(minThreads), maxThreads(maxThreads), idleThreadTimeout(idleTimeout)
        {

            if (minThreads == 0)
                minThreads = 1;
            if (maxThreads < minThreads)
                maxThreads = minThreads;

            for (size_t i = 0; i < minThreads; ++i)
            {
                addWorker();
            }

            // 启动线程管理线程
            std::thread manager([this]
                                { manageThreads(); });
            manager.detach();
        }

        // 禁止拷贝
        ThreadPool(const ThreadPool &) = delete;
        ThreadPool &operator=(const ThreadPool &) = delete;

        // 析构函数
        ~ThreadPool()
        {
            shutdown();
        }

        // 提交任务
        template <typename F, typename... Args>
        auto submit(Priority priority, std::string taskName, F &&f, Args &&...args)
            -> std::future<typename std::invoke_result_t<F, Args...>>
        {

            using return_type = typename std::invoke_result_t<F, Args...>;

            auto task = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            std::future<return_type> res = task->get_future();

            {
                std::unique_lock<std::mutex> lock(queueMutex);

                if (stop)
                {
                    throw std::runtime_error("提交任务到已停止的线程池");
                }

                tasks.emplace(
                    [task]()
                    { (*task)(); },
                    priority,
                    std::move(taskName));
            }

            condition.notify_one();
            return res;
        }

        // 快捷提交方法
        template <typename F, typename... Args>
        auto submit(F &&f, Args &&...args)
            -> std::future<typename std::invoke_result_t<F, Args...>>
        {
            return submit(Priority::Normal, "Unnamed", std::forward<F>(f), std::forward<Args>(args)...);
        }

        template <typename F, typename... Args>
        auto submitHighPriority(std::string taskName, F &&f, Args &&...args)
            -> std::future<typename std::invoke_result_t<F, Args...>>
        {
            return submit(Priority::High, std::move(taskName), std::forward<F>(f), std::forward<Args>(args)...);
        }

        template <typename F, typename... Args>
        auto submitLowPriority(std::string taskName, F &&f, Args &&...args)
            -> std::future<typename std::invoke_result_t<F, Args...>>
        {
            return submit(Priority::Low, std::move(taskName), std::forward<F>(f), std::forward<Args>(args)...);
        }

        // 等待所有任务完成
        void waitForCompletion()
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            completionCondition.wait(lock, [this]()
                                     { return tasks.empty() && activeTasks == 0; });
        }

        // 优雅关闭
        void shutdown()
        {
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                stop = true;
            }

            condition.notify_all();

            for (std::thread &worker : workers)
            {
                if (worker.joinable())
                {
                    worker.join();
                }
            }

            workers.clear();
        }

        // 获取线程池状态
        size_t getPendingTasks() const
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            return tasks.size();
        }

        size_t getActiveTasks() const
        {
            return activeTasks;
        }

        size_t getTotalTasksProcessed() const
        {
            return totalTasksProcessed;
        }

        size_t getThreadCount() const
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            return workers.size();
        }

        std::vector<ThreadInfo> getThreadInfo() const
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            std::vector<ThreadInfo> info;
            for (const auto &pair : threadInfos)
            {
                info.push_back(pair.second);
            }
            return info;
        }

    private:
        // 添加工作线程
        void addWorker()
        {
            workers.emplace_back([this]
                                 {
                // 注册线程信息
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    threadInfos[std::this_thread::get_id()] = {
                        std::this_thread::get_id(),
                        ThreadState::Idle,
                        "None"
                    };
                }
                
                while (true) {
                    std::unique_ptr<TaskWrapper> taskPtr;
                    
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        
                        // 等待任务或停止信号
                        condition.wait(lock, [this] {
                            return stop || !tasks.empty();
                        });
                        
                        if (stop && tasks.empty()) {
                            break;
                        }
                        
                        // 获取任务 - 创建新对象而不是移动
                        taskPtr = std::make_unique<TaskWrapper>(std::move(const_cast<TaskWrapper&>(tasks.top())));
                        tasks.pop();
                        
                        // 更新线程状态
                        threadInfos[std::this_thread::get_id()].state = ThreadState::Running;
                        threadInfos[std::this_thread::get_id()].currentTask = taskPtr->name;
                        
                        activeTasks++;
                    }
                    
                    // 执行任务
                    try {
                        (*taskPtr)();
                    } catch (const std::exception& e) {
                        std::cerr << "任务执行异常: " << e.what() << std::endl;
                    } catch (...) {
                        std::cerr << "未知任务执行异常" << std::endl;
                    }
                    
                    // 更新状态
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        activeTasks--;
                        totalTasksProcessed++;
                        threadInfos[std::this_thread::get_id()].state = ThreadState::Idle;
                        threadInfos[std::this_thread::get_id()].currentTask = "None";
                        
                        if (activeTasks == 0 && tasks.empty()) {
                            completionCondition.notify_all();
                        }
                    }
                    
                    // 释放任务指针
                    taskPtr.reset();
                }
                
                // 移除线程信息
                {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    threadInfos.erase(std::this_thread::get_id());
                } });
        }

        // 线程管理
        void manageThreads()
        {
            while (!stop)
            {
                std::this_thread::sleep_for(std::chrono::seconds(5));

                std::unique_lock<std::mutex> lock(queueMutex);

                size_t currentThreads = workers.size();
                size_t pendingTasks = tasks.size();

                // 根据负载调整线程数
                if (pendingTasks > currentThreads * 2 && currentThreads < maxThreads)
                {
                    // 增加线程
                    size_t threadsToAdd = std::min(pendingTasks / 2, maxThreads - currentThreads);
                    for (size_t i = 0; i < threadsToAdd; ++i)
                    {
                        addWorker();
                    }
                }
                else if (pendingTasks < currentThreads / 2 && currentThreads > minThreads)
                {
                    // 减少空闲线程
                    condition.notify_all(); // 唤醒所有线程以检查停止条件
                }

                // 清理已停止的线程
                auto it = workers.begin();
                while (it != workers.end())
                {
                    if (!it->joinable())
                    {
                        it = workers.erase(it);
                    }
                    else
                    {
                        ++it;
                    }
                }
            }
        }
    };

} // namespace ThreadPool

// 使用示例
int main()
{
    ThreadPool::ThreadPool pool(2, 8); // 最小2个线程，最大4个线程

    // 提交一些任务
    auto future1 = pool.submitHighPriority("重要计算", []
                                           {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "高优先级任务完成" << std::endl;
        return 42; });

    auto future2 = pool.submitLowPriority("后台任务", []
                                          {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "低优先级任务完成" << std::endl;
        return "结果"; });

    for (int i = 0; i < 10000; ++i)
    {
        pool.submit([i]
                    {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            std::cout << "普通任务 " << i << " 完成" << std::endl; });
    }

    // 等待特定任务完成
    auto result1 = future1.get();
    auto result2 = future2.get();

    std::cout << "任务1结果: " << result1 << std::endl;
    std::cout << "任务2结果: " << result2 << std::endl;

    // 等待所有任务完成
    pool.waitForCompletion();

    std::cout << "活跃任务数: " << pool.getActiveTasks() << std::endl;
    std::cout << "待处理任务: " << pool.getPendingTasks() << std::endl;
    std::cout << "总处理任务: " << pool.getTotalTasksProcessed() << std::endl;

    // 获取线程信息
    auto threadInfo = pool.getThreadInfo();
    for (const auto &info : threadInfo)
    {
        std::cout << "线程 " << info.id << " 状态: "
                  << static_cast<int>(info.state)
                  << " 任务: " << info.currentTask << std::endl;
    }

    // 析构函数会自动关闭线程池
    return 0;
}