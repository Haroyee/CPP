#include <deque>
#include <iostream>
#include <vector>
/*双端队列*/

int main()
{
    /*初始化*/
    std::deque<int> dq1;                   // 空deque
    std::deque<std::string> dq2;           // 字符串deque
    std::deque<double> dq3(10);            // 包含10个默认初始化的double
    std::deque<int> dq4(5, 42);            // 包含5个值为42的int
    std::deque<int> dq5 = {1, 2, 3, 4, 5}; // 初始化列表 (C++11)
    /*插入元素*/
    std::deque<int> dq_int;
    // 头部操作
    dq_int.push_front(0);    // 头部添加
    dq_int.emplace_front(1); // 头部构造添加
    // 尾部
    dq_int.push_back(2);    // 头部添加
    dq_int.emplace_back(4); // 头部构造添加
    // 指定位置插入
    //  在指定位置插入单个元素
    auto it = dq_int.begin() + 1;
    dq_int.insert(it, 20); // 在it位置之前插入20

    // 在指定位置插入多个相同元素
    it = dq_int.begin() + 2;
    dq_int.insert(it, 3, 25); // 插入3个25

    // 在指定位置插入范围
    std::vector<int> vec = {35, 36, 37};
    it = dq_int.end() - 1;
    dq_int.insert(it, vec.begin(), vec.end()); // 将vec元素插入dq_int

    // 在指定位置原地构造元素 (C++11)
    it = dq_int.begin() + 1;
    dq_int.emplace(it, 15);

    /*删除元素*/
    dq_int.pop_front(); // 删除头
    dq_int.pop_back();  // 删除尾
    // 删除指定位置的元素
    it = dq_int.begin() + 2;
    it = dq_int.erase(it); // 删除it位置元素，it现在指向原来it位置的元素

    // 删除范围内的元素
    auto first = dq_int.begin() + 1;
    auto last = dq_int.begin() + 4;
    dq_int.erase(first, last); // 删除1-4位置的元素
    /*访问元素*/
    // 使用下标操作符（不检查边界）
    std::cout << "q_int[2] = " << dq_int[2] << std::endl; // 30

    // 使用at()方法（带边界检查）
    std::cout << "dq_int.at(3) = " << dq_int.at(3) << std::endl; // 40
    try
    {
        std::cout << dq_int.at(10) << std::endl; // 抛出std::out_of_range异常
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Out of range: " << e.what() << std::endl;
    }
    /*遍历*/
    for (auto it = dq_int.begin(); it != dq_int.end(); ++it) // 迭代器
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    for (auto &it : dq_int) // 引用范围for循环
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;
    return 0;
}