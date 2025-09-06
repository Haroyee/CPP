#include <iostream>
#include <vector>
#include <sstream>
/*创建命名空间*/
namespace minToMax
{ // 小到大
    void bubbleSort(std::vector<int> *numbers)
    {
        int len = (*numbers).size();
        for (int i = 0; i < len - 1; i++)
        {

            bool flag = false;
            for (int j = 0; j < len - i - 1; j++)
            {
                if ((*numbers)[j] > (*numbers)[j + 1])
                {
                    std::swap((*numbers)[j], (*numbers)[j + 1]);
                }
                else
                    flag = true;
            }
            if (!flag)
                break;
        }
    }
}

namespace maxToMin
{ // 大到小
    void bubbleSort(std::vector<int> *numbers)
    {
        int len = (*numbers).size();
        for (int i = 0; i < len - 1; i++)
        {

            bool flag = false;
            for (int j = 0; j < len - i - 1; j++)
            {
                if ((*numbers)[j] < (*numbers)[j + 1])
                {
                    std::swap((*numbers)[j], (*numbers)[j + 1]);
                }
                else
                    flag = true;
            }
            if (!flag)
                break;
        }
    }
}

int main()
{
    std::vector<int> numbers;

    std::cout << "请输入数组以空格相隔:" << std::endl;
    std::string line;
    std::getline(std::cin, line);

    // 使用字符串流解析数字
    std::istringstream iss(line);
    int num;
    while (iss >> num)
    {
        numbers.push_back(num);
    }
    std::vector<int> numbers1 = numbers;
    std::vector<int> numbers2 = numbers;
    minToMax::bubbleSort(&numbers1);
    maxToMin::bubbleSort(&numbers2);
    for (int n : numbers1)
        std::cout << n << " ";
    std::cout << std::endl;
    for (int m : numbers2)
        std::cout << m << " ";
    std::cout << std::endl;

    return 0;
}