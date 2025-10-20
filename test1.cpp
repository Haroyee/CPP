#include <iostream>
#include <future>
#include <thread>
#include <memory>
#include <unordered_map>
#include <condition_variable>
int func(int a)
{
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "执行任务！" << std::endl;
    }
    return a;
}

int main()
{

    using funci = int (*)();
    funci f = []()
    { return func(4); };
    std::thread t(f);

    std::thread t1([&]()
                   { std::this_thread::sleep_for(std::chrono::seconds(3)); 
                f = [](){return func(6);} ;
            std::cout<<"替换完毕！"<<std::endl; });

    t.join();
    t1.join();

    return 0;
}