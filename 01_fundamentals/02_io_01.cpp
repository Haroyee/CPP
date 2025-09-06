#include <iostream>
/*基本输入输出*/
int main()
{

    struct human
    {
        int age;
        std::string name;
        bool sex;
    } student;
    std::cout << "请输入你的年龄名字和性别(1/0)" << std::endl;
    std::cin >> student.age >> student.name >> student.sex;
    std::cout << "你的名字是" << student.name << ",今年" << student.age << "是个" << (student.sex ? "男孩" : "女孩") << std::endl;

    return 0;
}