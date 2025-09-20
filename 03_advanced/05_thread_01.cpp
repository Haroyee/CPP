#include <iostream>
#include <thread>
#include <string>
/*创建线程*/
void show(const std::string &str)
{
    std::cout << str << std::endl;
}
void selfadd(int &x)
{
    x++;
}
int main()
{
    std::thread t1 = std::thread(show, "hello world!");
    std::thread t2(show, "hello everybody!");
    int num = 10;
    std::thread t3(selfadd, std::ref(num));

    t1.join();   // 待线程完成,阻塞当前线程，直到目标线程执行完毕,不使用否则主线程执行完毕可能提前结束该线程
    t2.detach(); // 分离线程，让线程在后台独立运行（无法再控制），主线程结束该线程不受主线程影响继续运行
    /*输出结果可能是*/
    /*
    1:
    hello world!hello everybody!
    2
    hello everybody!hello world!
    3:
    hello world!
    hello everybody!
    4:
    hello everybody!
    hello world!
    */

    return 0;
}