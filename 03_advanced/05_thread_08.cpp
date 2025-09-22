#include <iostream>
#include <thread>
#include <future>
/*异步编程*/
int add(int x, int y)
{
    std::cout << "task - add start!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "task - add end!" << std::endl;
    return x + y;
}
void add_prom(std::promise<int> &&sum, int x, int y)
{
    std::cout << "task - add_prom start!" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    sum.set_value(x + y);
    std::cout << "task - add_prom end!" << std::endl;
}
int main()
{
    /*std::launch::async协议的async异步函数*/
    std::cout << "std::launch::async协议" << std::endl;
    std::future<int> fu1 = std::async(std::launch::async, add, 10, 20);
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    fu1.wait();                          // 获取状态是否阻塞？
    std::cout << fu1.get() << std::endl; // 获取结果

    /*std::launch::deferred协议的async传递promise值的异步函数*/
    std::promise<int> prom;
    std::future<int> fu_prom = prom.get_future();
    std::cout << "std::launch::deferred协议" << std::endl;
    std::future<void> fu2 = std::async(std::launch::deferred, add_prom, std::move(prom), 10, 30);
    /*由于执行移动语义，此时prom已经为空*/
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    fu2.wait_for(std::chrono::milliseconds(500)); // 若阻塞等待500ms
    fu2.get();
    std::cout << fu_prom.get() << std::endl;

    /*std::share_future*/
    std::future<int> fu3 = std::async(std::launch::async, add, 10, 20);
    std::shared_future<int> share_fu = fu3.share();
    std::cout << share_fu.get() << std::endl;
    std::cout << share_fu.get() << std::endl;
    std::cout << share_fu.get() << std::endl;

    /*std::packaged_task的使用*/
    std::packaged_task<int(int, int)> p_task(add);
    std::future<int> fu4 = p_task.get_future();

    std::thread async_t(std::move(p_task), 10, 20);
    async_t.join();
    std::cout << fu4.get() << std::endl;

    return 0;
}