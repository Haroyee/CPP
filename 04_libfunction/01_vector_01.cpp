#include <iostream>
#include <vector>
#include <string>
/*函数库-vector*/
class R
{
private:
    std::string str;

public:
    R(const std::string &s) : str(s) {}

    std::string &getStr()
    {
        return str;
    }
};

int main()
{
    std::vector<R> vr;
    /*添加新元素*/
    // 尾部插入元素
    // 调用移动函数
    vr.push_back(R("0"));
    // 调用构造函数
    vr.emplace_back("1");
    // 任意位置插入元素
    vr.insert(vr.begin() + 2, R("2"));

    /*删除元素*/
    // 删除尾部元素
    vr.pop_back();
    // 删除指定位置元素
    vr.erase(vr.end());

    /*访问元素*/
    // 不检查边界
    std::cout << "第一个元素:" << vr[0].getStr() << std::endl;
    // 检查边界
    std::cout << "第二个元素:" << vr.at(1).getStr() << std::endl;
    // 首位元素
    std::cout << "首位元素:" << vr.front().getStr() << std::endl;
    // 尾部元素
    std::cout << "尾部元素:" << vr.back().getStr() << std::endl;

    /*遍历*/
    // 迭代器遍历
    std::cout << "使用迭代器: ";
    for (auto i = vr.begin(); i != vr.end(); i++)
        std::cout << (*i).getStr() << " ";
    std::cout << std::endl;
    // 范围for循环(c++11)
    std::cout << "范围for循环: ";
    for (R i : vr)
        std::cout << i.getStr() << " ";
    std::cout << std::endl;

    return 0;
}