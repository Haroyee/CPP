#include <iostream>

/*拷贝构造函数与移动构造函数*/
class R
{
protected:
    int *ptr;

public:
    R(int p)
    {
        ptr = new int;
        *ptr = p;
    }

    R &operator=(const R &obj) // 深拷贝赋值函数
    {
        if (this != &obj)
        {               // 处理自我赋值
            delete ptr; // 释放当前资源
            ptr = new int;
            *ptr = *obj.ptr;
            std::cout << "执行R的深拷贝赋值函数" << std::endl;
        }
        return *this;
    }

    R(const R &obj)
    { // 深拷贝函数
        ptr = new int;
        *ptr = *obj.ptr;
        std::cout << "执行R的深拷贝函数" << std::endl;
    }

    R(R &&obj) noexcept
    {
        // delete ptr;问题：新对象的ptr可能未初始化，删除未初始化的指针是未定义行为
        ptr = obj.ptr;
        obj.ptr = nullptr;
        std::cout << "执行R的移动函数" << std::endl;
    }
    R &operator=(R &&obj) noexcept
    {
        delete ptr;
        ptr = obj.ptr;
        obj.ptr = nullptr;
        std::cout << "执行R的移动赋值函数" << std::endl;
        return *this;
    }
    void setPtr(int p)
    {
        *ptr = p;
    }
    int *getPtr()
    {
        return ptr;
    }
    ~R()
    {
        delete ptr;
        std::cout << "执行R的析构函数" << std::endl;
    }
};
R func()
{
    R r = R(8);
    return r;
}

int main()
{
    R r1 = R(5);
    R r2 = std::move(r1);
    R r3 = func();
    std::cout << *(r2.getPtr()) << std::endl;
    return 0;
}