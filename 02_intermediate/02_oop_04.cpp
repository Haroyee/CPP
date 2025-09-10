#include <iostream>

/*拷贝构造函数*/
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
    R(const R &obj)
    { // 深拷贝函数
        ptr = new int;
        *ptr = *obj.ptr;
        std::cout << "执行R的深拷贝函数" << std::endl;
    }
    void setPtr(int p)
    {
        *ptr = p;
    }
    int *getPtr()
    {
        return ptr;
    }
};

int main()
{
    R r1 = R(5);
    R r2 = r1;
    std::cout << *(r2.getPtr()) << std::endl;
    return 0;
}