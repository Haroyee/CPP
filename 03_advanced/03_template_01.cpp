#include <iostream>
/*模版-auto关键字*/
int add(int a, int b, int c)
{
    return a + b + c;
}
int main()
{
    auto a = 1;
    auto b = "a";
    auto c = {1, 2, 3, 4, 5};
    std::cout << "a's type is " << typeid(a).name() << std::endl;
    std::cout << "b's type is " << typeid(b).name() << std::endl;
    std::cout << "c's type is " << typeid(c).name() << std::endl;

    /*自动推导函数指针*/
    auto pf = add;
    std::cout << "pf(1,2,3) = " << pf(1, 2, 3) << std::endl;
    return 0;
}