#include <iostream>
#include <memory>
/*独享智能指针-unique_ptr*/
class R
{
public:
    R()
    {
        std::cout << "调用R的构造函数" << std::endl;
    }
    ~R()
    {
        std::cout << "调用R的析构函数" << std::endl;
    }
    R(R &&obj) noexcept
    {
        std::cout << "调用R的移动函数" << std::endl;
    }
    R &operator=(R &&obj) noexcept
    {
        std::cout << "调用R的移动赋值函数" << std::endl;
        return *this;
    }
};

std::unique_ptr<R> func()
{
    std::unique_ptr<R> pp(new R());
    return pp;
}
/*智能指针*/
int main()
{
    std::unique_ptr<R> up1(new R());
    // std::unique_ptr<int> up2 = up1;            // 非法操作，无法拷贝
    std::unique_ptr<R> up3 = std::move(up1); // 有效操作，可移动
    /*--------------------------------------------------------*/
    std::unique_ptr<R> up4 = std::unique_ptr<R>(new R());
    // 有效操作，此操作无需拷贝函数，‘unique_ptr<int>(new int(10))’为临时的右值
    /*---------------------------------------------------------*/
    up4 = func(); // 有效操作，此操作调用了移动函数

    return 0;
}