#include <iostream>
#include <memory>
class R
{
public:
    int num;
    R(int n)
    {
        this->num = n;
    }
};
int main()
{
    /*方式一(推荐)c++11*/
    std::unique_ptr<int> up11(new int(10));
    std::unique_ptr<R> up12(new R(10));
    /*方式二c++14*/
    std::unique_ptr<int> up21 = std::make_unique<int>(10);
    std::unique_ptr<R> up22 = std::make_unique<R>(10);
    /*方式三*/
    int *p = new int(10);
    R *r = new R(10);
    std::unique_ptr<int> up31(p);
    std::unique_ptr<R> up32(R);
    return 0;
}