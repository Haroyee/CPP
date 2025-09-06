#include <iostream>
#include "02_oop_00.h"

int main()
{
    Rectangle R = Rectangle(8, 6);

    std::cout << "this Rectangle's length is " << R.getLength() << std::endl;
    std::cout << "this Rectangle's breadth is " << R.getBreadth() << std::endl;
    std::cout << "this Rectangle's area is " << R.getArea() << std::endl;
    return 0;
}