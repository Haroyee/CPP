#include <iostream>
/*函数重载*/
namespace addFunc
{
    int add(int a, int b)
    {
        return a + b;
    }
    int add(int a, int b, int c)
    {
        return a + b + c;
    }
    char add(char a, char b)
    {
        int r = a + b;
        if (r > 127)
            throw std::out_of_range("超过ASCII码范围");
        else
            return r;
    }
}

int main()
{

    int a = 5;
    int b = 7;
    int c = 2;
    char d = '0';
    char f = '0';
    std::cout << "a+b=" << addFunc::add(a, b) << std::endl;
    std::cout << "a+b+c=" << addFunc::add(a, b, c) << std::endl;
    std::cout << "d+f=" << addFunc::add(d, f) << std::endl;
}