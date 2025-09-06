#include <iostream>
#include <cmath>
/*运算符的重载*/
class Rectangle
{
private:
    double length;
    double breadth;

public:
    /*全参构造*/
    Rectangle(double len, double bre)
    {
        length = len;
        breadth = bre;
    }
    /*无参构造(默认有)*/
    Rectangle()
    {
        length = 0;
        breadth = 0;
    }
    void setLength(double len)
    {
        length = len;
    }
    void setBreadth(double bre)
    {
        breadth = bre;
    }
    double getLength()
    {
        return length;
    }
    double getBreadth()
    {
        return breadth;
    }
    double getArea()
    {
        return length * breadth;
    }
    /*重载运算符*/
    Rectangle operator+(Rectangle &r)
    {
        // 创建一个新矩形，其面积是两个矩形面积之和
        double totalArea = this->getArea() + r.getArea();

        // 保持长宽比例与当前矩形相同
        double ratio = length / breadth;
        double newBreadth = std::sqrt(totalArea / ratio);
        double newLength = totalArea / newBreadth;

        return Rectangle(newLength, newBreadth);
    }
};

int main()
{
    Rectangle R1 = Rectangle(8, 6);
    Rectangle R2 = Rectangle(7, 2);
    /*执行重载后的操作*/
    Rectangle R3 = R1 + R2;
    std::cout << "this Rectangle's length is " << R3.getLength() << std::endl;
    std::cout << "this Rectangle's breadth is " << R3.getBreadth() << std::endl;
    return 0;
}