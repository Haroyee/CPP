#include <iostream>
#include <thread>
#include <mutex>
/*互斥锁-mutex*/

std::mutex m; // 声明互斥锁
void selfAdd(int *a)
{
    m.lock(); // 上锁
    /*临界区*/
    (*a)++;
    m.unlock(); // 解锁
}
void func(int *a)
{
    for (int i = 0; i < 100000; i++)
        selfAdd(a);
}

int main()
{
    int *num = new int(0);
    std::thread t1(func, num);
    std::thread t2(func, num);
    t1.join();
    t2.join();
    std::cout << *num << std::endl;

    return 0;
}