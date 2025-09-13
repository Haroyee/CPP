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
int main()
{
    R<int, std::string> r = R<int, std::string>(1, "15684");
    return 0;
}