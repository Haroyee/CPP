#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
/*getline函数的使用*/

int main()
{
    std::vector<int> numbers;
    std::string line;
    int num = 0;

    while (std::getline(std::cin, line))
    { // 在输入EOF前循环执行，即ctrl+z后+enter
        std::istringstream iss(line);
        while (iss >> num)
            numbers.push_back(num);
    }
    int i = 0;
    for (int n : numbers)
    {
        ++i;
        std::cout << std::setw(5) << n << " ";
        if (i % 10 == 0)
            std::cout << std::endl;
    }
    return 0;
}