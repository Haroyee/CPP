#include <iostream>
#include <string>
/*类模版*/
template <class T1, class T2>

class R
{
protected:
    T1 data1;
    T2 data2;

public:
    R(T1 d1, T2 d2)
    {
        this->data1 = d1;
        this->data2 = d2;
    }
    void setData1(T1 d1)
    {
        this->data1 = d1;
    }
    void setData2(T2 d2)
    {
        this->data2 = d2;
    }
    T1 getData1()
    {
        return this->data1;
    }
    T2 getData2()
    {
        return this->data2;
    }
};
template <> // 类模版具体化
class R<int, std::string>
{
protected:
    int data1;
    std::string data2;

public:
    R(int d1, std::string d2)
    {
        this->data1 = d1;
        this->data2 = d2;
    }
    void setData1(int d1)
    {
        this->data1 = d1;
    }
    void setData2(std::string d2)
    {
        this->data2 = d2;
    }
    int getData1()
    {
        return this->data1;
    }
    std::string getData2()
    {
        return this->data2;
    }
};

class R1 : public R<int, std::string> // 类模版具体化继承
{
public:
    using R::R;
};
template <class T1, class T2>
class R2 : public R<T1, T2> // 类模版继承
{
public:
    using R<T1, T2>::R;
};
template <class T> // 模版类继承模版参数给出的基类
class R3 : public T
{
public:
    R3(int a, std::string b) : T(a, b) {}
};
int main()
{
    R<int, std::string> r = R<int, std::string>(1, "15684");
    R3<R1> r3 = R3<R1>(2, "58");
    R3<R<int, std::string>> r4 = R3<R<int, std::string>>(3, "58445");
    return 0;
}