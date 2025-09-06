#include <iostream>
#include <cmath>
#include "02_oop_00.h"
/*继承与多态*/

class RectangleA : public Rectangle
{ // 共有继承

protected: // 受保护成员
    double area;

public:
    void setArea()
    {
        area = getLength() * getBreadth();
    }

    double getArea() override
    {
        return area; // 类内部protected成可以访问
    }

    Rectangle operator+(Rectangle &r)
    {
        // 创建一个新矩形，其面积是两个矩形面积之和
        double totalArea = this->getArea() + r.getArea();

        // 保持长宽比例与当前矩形相同
        double ratio = getLength() / getBreadth(); // length与breadth属于私有成员，无法访问
        double newBreadth = std::sqrt(totalArea / ratio);
        double newLength = totalArea / newBreadth;

        return Rectangle(newLength, newBreadth);
    }
};

class RectangleB : public RectangleA
{

public:
    void func()
    {
        area = 20; // 父类被继承的protected成员
    }
};