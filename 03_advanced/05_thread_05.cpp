#include <iostream>
#include <mutex>
#include <thread>
/*线程安全的类的单例实现*/
/*常规双检锁实现*/
class ThreadSafeSingleton
{
private:
    static ThreadSafeSingleton *instance;
    static std::mutex mtx;

    ThreadSafeSingleton()
    {
        std::cout << "ThreadSafeSingleton initialized" << std::endl;
    }

    ThreadSafeSingleton(const ThreadSafeSingleton &) = delete;
    ThreadSafeSingleton &operator=(const ThreadSafeSingleton &) = delete;

public:
    static ThreadSafeSingleton *getInstance()
    {
        // 第一次检查：避免每次调用都加锁
        if (instance == nullptr)
        {
            std::lock_guard<std::mutex> lock(mtx);
            // 第二次检查：确保只有一个线程创建实例
            if (instance == nullptr)
            {
                instance = new ThreadSafeSingleton();
            }
        }
        return instance;
    }

    void doSomething()
    {
        std::cout << "Doing something with ThreadSafeSingleton" << std::endl;
    }

    static void destroy()
    {
        std::lock_guard<std::mutex> lock(mtx);
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }
};
// 初始化静态成员变量
ThreadSafeSingleton *ThreadSafeSingleton::instance = nullptr;
std::mutex ThreadSafeSingleton::mtx;

/*std::call_once实现*/
class CallOnceSingleton
{
private:
    static CallOnceSingleton *instance;
    static std::once_flag flag;
    CallOnceSingleton()
    {
        std::cout << "CallOnceSingleton initialized" << std::endl;
    };
    CallOnceSingleton(const CallOnceSingleton &c) = delete;
    CallOnceSingleton &operator=(const CallOnceSingleton &c) = delete;

    static void createInstance()
    {
        instance = new CallOnceSingleton();
    }

public:
    static CallOnceSingleton *getInstance()
    {
        std::call_once(flag, createInstance);
        return instance;
    }
    void doSomething()
    {
        std::cout << "Doing something with CallOnceSingleton" << std::endl;
    }
    static void destroy()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
    }
};
CallOnceSingleton *CallOnceSingleton::instance = nullptr;
std::once_flag CallOnceSingleton::flag;
int main()
{
    std::thread t1(ThreadSafeSingleton::getInstance);
    std::thread t2(CallOnceSingleton::getInstance);
    t1.join();
    t2.join();

    return 0;
}