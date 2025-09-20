#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
/*原子操作*/

int main()
{
    /*初始化*/
    std::atomic<int> atomic_int(0);
    std::cout << atomic_int << std::endl;

    /*存贮值*/
    atomic_int.store(15);
    std::cout << atomic_int << std::endl;
    atomic_int.store(15, std::memory_order_relaxed);
    std::cout << atomic_int << std::endl;

    /*加载值*/
    int load = atomic_int.load();
    std::cout << "load =" << load << std::endl;
    load = atomic_int.load(std::memory_order_relaxed);
    std::cout << "load =" << load << std::endl;

    /*交换值*/
    int exchange = atomic_int.exchange(100);
    std::cout << "exchange = " << exchange << std::endl;
    exchange = atomic_int.exchange(15, std::memory_order_relaxed);
    std::cout << "exchange = " << exchange << std::endl;

    /*比较交换*/
    // 如果atomic_int等于exchange，则将其设置为desired，否则将exchange设置为atomic_int的当前值
    int desired = 100;
    std::cout << "atomic_int = " << atomic_int << " exchange = " << exchange << std::endl;
    atomic_int.compare_exchange_weak(exchange, desired, std::memory_order_relaxed);
    std::cout << "atomic_int = " << atomic_int << " exchange = " << exchange << std::endl;
    // 强版本的比较交换，不会虚假失败
    atomic_int.compare_exchange_strong(exchange, desired, std::memory_order_relaxed);
    std::cout << "atomic_int = " << atomic_int << " exchange = " << exchange << std::endl;
    /*读-修改-写操作*/
    // 加法并返回旧值
    exchange = atomic_int.fetch_add(5, std::memory_order_relaxed);
    // 减法并返回旧值
    exchange = atomic_int.fetch_sub(5, std::memory_order_relaxed);
    // 按位与并返回旧值
    exchange = atomic_int.fetch_and(0x0F, std::memory_order_relaxed);
    // 按位或并返回旧值
    exchange = atomic_int.fetch_or(0xF0, std::memory_order_relaxed);
    // 按位异或并返回旧值
    exchange = atomic_int.fetch_xor(0xFF, std::memory_order_relaxed);

    return 0;
}