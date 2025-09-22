#include <iostream>
#include <sstream>
/*字符串流的处理*/
/*
std::istringstream：字符串输入流，用于从字符串读取数据。

std::ostringstream：字符串输出流，用于将数据格式化成字符串。

std::stringstream：兼具输入和输出功能。
*/
int main()
{

    /*数字转字符串*/

    std::ostringstream oss;
    oss << 12 << 85 << "45" << std::endl;
    std::cout << oss.str() << std::endl;

    /*字符串解析*/
    std::istringstream iss("12 85 jack 0");
    int a;
    int b;
    std::string str;
    bool flag;
    iss >> a >> b >> str >> flag;
    std::cout << a << " " << b << " " << str << " " << flag << std::endl;

    return 0;
}