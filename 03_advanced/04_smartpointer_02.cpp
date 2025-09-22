#include <iostream>
#include <memory>
#include <vector>
/*共享智能指针-shared_ptr*/

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

int main()
{

    /*方式一*/
    std::shared_ptr<int> sp0(new int(10));
    /*方式二(推荐)*/
    std::shared_ptr<int> sp1 = std::make_shared<int>(10);
    /*方式三*/
    int *nums = new int(10);
    std::shared_ptr<int> sp2(nums);
    /*可拷贝*/
    std::shared_ptr<int> sp3 = sp2;
    /*返回计数*/
    std::cout << sp0.use_count() << std::endl;
    std::cout << sp2.use_count() << std::endl;
    std::cout << sp3.use_count() << std::endl;
    /*检查唯一性*/
    std::cout << sp0.unique() << std::endl;
    std::cout << sp2.unique() << std::endl;

    return 0;
}