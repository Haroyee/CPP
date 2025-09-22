#include <iostream>

/*析构函数*/
class Shape
{
private:
    double width;
    double height;

public:
    Shape(double w, double h)
    {
        width = w;
        height = h;
    }
    void setWidth(double w)
    {
        width = w;
    }
    void setHeight(double h)
    {
        height = h;
    }
    double getWidth()
    {
        return width;
    }
    double getHeight()
    {
        return height;
    }

    virtual double getArea() = 0; // 纯虚函数
    virtual ~Shape()              // 虚析构函数
    {
        std::cout << "Shape析构函数" << std::endl;
    }
};

class Rectangle : public Shape
{

public:
    using Shape::Shape;
    double getArea()
    {
        return getWidth() * getHeight();
    }
    ~Rectangle() override
    { // 重载析构函数
        std::cout << "Rectangle析构函数" << std::endl;
    }
};

int main()
{

    Rectangle r = Rectangle(5, 5);
    std::cout << "this Rectangle's area is " << r.getArea() << std::endl;
    return 0;
}