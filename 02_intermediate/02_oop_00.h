
#ifndef OOP_00_H
#define OOP_00_H
/*面向对象编程*/
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
    virtual double getArea()
    {
        return length * breadth;
    }
};

#endif