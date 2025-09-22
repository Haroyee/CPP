#include <iostream>
#include <thread>
#include <mutex>
/*lock_guard与unique_lock*/
std::mutex m;
std::timed_mutex tm; // 允许时间操作的互斥锁

void func_lock_guard(int *a)
{
    std::lock_guard<std::mutex> lock(m); // 初始化调用构造函数加锁
    // std::lock_guard<std::mutex> lock(m,std::adopt_lock);//不加锁
    /*临界区*/
    (*a)++;
    std::cout << "执行func_lock_guard\n";
    /*离开作用于lock调用析构函数解锁*/
}

void func_unique_lock(int *a)
{
    std::unique_lock<std::mutex> lock(m);
    // std::unique_lock<std::mutex> lock(m,std::defer_lock);//不加锁
    (*a)++;
    std::cout << "执行func_unique_lock\n";
}
void func_unique_lock_time(int *a)
{
    std::unique_lock<std::timed_mutex> lock(tm);
    (*a)++;
    lock.unlock();

    lock.lock();
    (*a)++;
    lock.unlock();

    lock.try_lock();
    (*a)++;
    lock.unlock();

    lock.try_lock_for(std::chrono::seconds(2)); // 在指定时间内获取到锁则返回true，反之flase
    (*a)++;
    lock.unlock();
    std::cout << "执行func_unique_lock_time\n";
}

int main()
{
    int *num = new int(0);
    std::thread t1(func_lock_guard, num);
    std::thread t2(func_unique_lock, num);
    std::thread t3(func_unique_lock_time, num);
    t1.join();
    t2.join();
    t3.join();

    return 0;
}