#include <iostream>
#include <cmath>
#include "02_oop_00.h"
/*继承与多态*/

class RectangleA : public Rectangle
{ // 共有继承

protected: // 受保护成员
    double area;

public:
    using Rectangle::Rectangle; // 依据父类生成所有构造函数(无法继承构造函数与析构函数)

    /*
    等效于：
    RectangleA(double len,double bre):Rectangle(len,bre){}
    RectangleA():Rectangle(){}
    */

    void setArea()
    {
        area = getLength() * getBreadth();
    }

    double getArea() override
    {
        return area; // 类内部protected成可以访问
    }

    RectangleA operator+(RectangleA &r)
    {
        // 创建一个新矩形，其面积是两个矩形面积之和
        double totalArea = this->getArea() + r.getArea();

        // 保持长宽比例与当前矩形相同
        double ratio = getLength() / getBreadth(); // length与breadth属于私有成员，无法访问
        double newBreadth = std::sqrt(totalArea / ratio);
        double newLength = totalArea / newBreadth;

        return RectangleA(newLength, newBreadth);
    }
};

class RectangleB : public RectangleA
{

public:
    using RectangleA::RectangleA;
    void func()
    {
        area = 20; // 父类被继承的protected成员，子类内部可以访问，但外部无法访问
    }
};

int main()
{

    RectangleA Ra = RectangleA(8.0, 9.0);
    RectangleB Rb = RectangleB(8.0, 9.0);
    std::cout << "Ra's lengyh = " << Ra.getLength() << std::endl;
    std::cout << "Ra's breadth = " << Ra.getBreadth() << std::endl;
    Ra.setBreadth(12);
    Ra.setArea();
    std::cout << "Ra's area = " << Ra.getArea() << std::endl;
    std::cout << "Rb's lengyh = " << Rb.getLength() << std::endl;
    std::cout << "Rb's breadth = " << Rb.getBreadth() << std::endl;
    Rb.func();
    std::cout << "Rb's fake area = " << Rb.getArea() << std::endl;
    Rb.setArea();
    std::cout << "Rb's really area = " << Rb.getArea() << std::endl;
    return 0;
}