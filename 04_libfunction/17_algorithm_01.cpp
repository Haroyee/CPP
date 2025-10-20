#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
    std::vector<int> nums = {1, 8, 8, 5, 4, 7, 6, 545, 74, 58, 62, 4, 5, 4, 7, 5, 4, 7, 4, 55, 98, 48, 95, 36, 54};

    /*非修改性序列操作（仅查询 / 遍历，不改变元素）*/
    // 1.for_each
    std::for_each(nums.begin(), nums.end(), [](int i)
                  { std::cout << i << " "; });
    std::cout << std::endl;
    // 2.count
    int counts = std::count(nums.begin(), nums.end(), 4);
    std::cout << "counts = " << counts << std::endl;
    // 统计范围中满足一元谓词 pred 的元素个数（比 count 更灵活，支持自定义条件）。
    counts = std::count_if(nums.begin(), nums.end(), [](int i)
                           { return i % 2 == 0; });
    std::cout << "counts = " << counts << std::endl;

    // 3.find 查找范围中第一个等于 value 的元素，返回其迭代器（未找到返回 end）。
    auto it = std::find(nums.begin(), nums.end(), 4);
    if (it != nums.end())
    {
        // 计算索引（距离begin的偏移量）
        int index = std::distance(nums.begin(), it);
        std::cout << "第一个" << 4 << "在索引" << index << "处" << std::endl; // 输出：索引2
    }
    else
    {
        std::cout << "未找到" << 4 << std::endl;
    }
    // find_if 找范围中第一个满足一元谓词 pred 的元素（支持自定义查找条件）。
    it = std::find_if(nums.begin(), nums.end(), [](int i)
                      { return i > 10; });
    if (it != nums.end())
    {
        // 计算索引（距离begin的偏移量）
        int index = std::distance(nums.begin(), it);
        std::cout << "第一个>10的数在索引" << index << "处" << std::endl; // 输出：索引2
    }
    else
    {
        std::cout << "未找到" << std::endl;
    }
    // find_end 在主范围中查找子序列最后一次出现的位置（返回子序列第一个元素的迭代器）。
    std::vector<int> subnums = {4, 7};
    it = std::find_end(nums.begin(), nums.end(), subnums.begin(), subnums.end());
    if (it != nums.end())
    {
        // 计算索引（距离begin的偏移量）
        int index = std::distance(nums.begin(), it);
        std::cout << "subnums最后出现在索引" << index << "处" << std::endl; // 输出：索引2
    }
    else
    {
        std::cout << "未找到" << std::endl;
    }
    // 4.search 在主范围中查找子序列第一次出现的位置（与 find_end 相反）
    it = std::search(nums.begin(), nums.end(), subnums.begin(), subnums.end());
    if (it != nums.end())
    {
        // 计算索引（距离begin的偏移量）
        int index = std::distance(nums.begin(), it);
        std::cout << "subnums最先出现在索引" << index << "处" << std::endl; // 输出：索引2
    }
    else
    {
        std::cout << "未找到" << std::endl;
    }
    // 5.equal 判断两个范围是否所有元素都相等（可自定义比较函数）
    bool eqFlag = std::equal(nums.begin(), nums.end(), subnums.begin());
    std::cout << (eqFlag ? "true" : "false") << std::endl;
    eqFlag = std::equal(nums.begin(), nums.end(), subnums.begin(), [](int a, int b)
                        { return a - 1 == b; }); // 自定义比较函数
    std::cout << (eqFlag ? "true" : "false") << std::endl;
    // all_of / any_of / none_of
    /*
    all_of：检查所有元素是否满足谓词；
    any_of：检查任一元素是否满足谓词；
    none_of：检查没有元素满足谓词。
    */
    std::cout << std::all_of(nums.begin(), nums.end(), [](int i)
                             { return i % 2 == 0; })
              << std::endl;
    std::cout << std::any_of(nums.begin(), nums.end(), [](int i)
                             { return i % 2 == 0; })
              << std::endl;
    std::cout << std::none_of(nums.begin(), nums.end(), [](int i)
                              { return i % 2 == 0; })
              << std::endl;

    return 0;
}