#include <iostream>
#include <iomanip> //需要添加的头文件
/*格式化输出*/
/*
 std::setw(int n)：设置下一个输出项的字段宽度。

 std::setprecision(int n)：设置浮点数的精度。

 std::fixed / std::scientific：设置浮点数的输出格式（定点/科学计数法）。

 std::boolalpha：将布尔值输出为 true/false 而非 1/0。

*/
int main()
{
    double f = 9854.153684846484;
    std::cout << "科学计数法：" << std::setw(20) << std::setprecision(5) << std::scientific << f << std::endl;
    std::cout << "定点：" << std::setw(20) << std::setprecision(5) << std::fixed << f << std::endl;

    return 0;
}
