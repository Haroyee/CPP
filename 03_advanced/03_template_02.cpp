#include <iostream>

/*函数模版*/

template <typename T> // 声明数据类型

void Swap(T &a, T &b) // 单搁数据类型函数模版
{
    T temp = a;
    a = b;
    b = temp;
}

template <typename T, typename Y> // 多个数据类型函数模版
void Message(T number, Y message)
{
    std::cout << number << ":" << message << std::endl;
}
/*类成员函数模版*/
/*为类的成员函数创建模版时不能是析构函数和虚函数*/
template <typename C>
class tempClass
{

protected:
    C data;

public:
    tempClass(C d)
    {
        this->data = d;
    }
    void setData(C d)
    {
        this->data = d;
    }
    C getData()
    {
        return this->data;
    }
};
/*函数模版覆写*/
template <typename T>
void Swap(tempClass<T> &a, tempClass<T> &b)
{

    T temp = a.getData();
    a.setData(b.getData());
    b.setData(temp);
}
/*函数模版具体化*/
template <typename T>
void Swap(int *a, int *b)
{
    int *temp;
    temp = a;
    a = b;
    b = temp;
}
int main()
{
    int number1 = 12;
    int number2 = 18;
    Swap(number1, number2);
    std::cout << "number1 = " << number1 << std::endl;
    std::cout << "number2 = " << number2 << std::endl;
    tempClass Ta = tempClass("kami");
    std::cout << "tempClass = " << Ta.getData() << std::endl;
    Message(15, "太棒了！！！");
    int *p1, *p2;
    int a[2] = {3, 8};
    int b[3] = {4, 5, 9};
    p1 = &a[0];
    p2 = &b[0];
    Swap(p1, p2);
    for (int i = 0; i < 2; i++)
    {
        std::cout << *(p1 + i) << std::endl;
    }

    return 0;
}