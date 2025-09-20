#include <iostream>
#include <thread>
#include <string>
#include <functional> // 用于std::bin
#include <memory>
/*线程创建关于类的成员函数*/
class R
{
public:
    R() {}
    void fun(const std::string &str)
    {
        std::cout << "执行R的成员函数fun:" << str << std::endl;
    }
    void startThread()
    {
        std::thread t(&R::fun, this, "内部执行");
        t.join();
    }
};
int main()
{

    std::shared_ptr r = std::make_shared<R>();
    /*方式一*/
    r->startThread();
    /*方式二*/
    std::thread t1(&R::fun, r, "外部执行方式二");
    t1.join();
    /*方式三*/
    std::_Bind bindFun = std::bind(&R::fun, r, "外部执行方式三");
    std::thread t2(bindFun);
    t2.join();

    return 0;
}