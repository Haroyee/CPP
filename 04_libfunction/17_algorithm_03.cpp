#include <iostream>
#include <algorithm>
#include <vector>
/*排序及相关操作（针对有序 / 无序范围）*/
void print(std::vector<int> &num)
{
    std::for_each(num.begin(), num.end(), [](int i)
                  { std::cout << i << " "; });
    std::cout << std::endl;
}
int main()
{
    std::vector<int> nums(30);
    std::generate(nums.begin(), nums.end(), []()
                  { return rand() % 100; });
    /*1.sort-不稳定排序*/
    std::vector<int> sort_nums;
    std::copy(nums.begin(), nums.end(), std::back_inserter(sort_nums));
    std::sort(sort_nums.begin(), sort_nums.end()); // 默认升序
    print(sort_nums);

    std::sort(sort_nums.begin(), sort_nums.end(), [](int a, int b)
              { return a > b; }); // 自定义降序
    print(sort_nums);
    /*2.table_sort-不稳定排序*/
    std::vector<int> stable_sort_nums;
    std::copy(nums.begin(), nums.end(), std::back_inserter(stable_sort_nums));
    std::stable_sort(stable_sort_nums.begin(), stable_sort_nums.end());
    print(stable_sort_nums);

    /*3.partial_sort
    对范围进行**部分排序**—— 使前 n 个元素成为范围中 “最小的 n 个元素” 且已升序排列，
    其余元素无序（比全量排序更高效，适合只需前 n 个有序元素的场*/
    std::vector<int> partial_sort_nums;
    std::copy(nums.begin(), nums.end(), std::back_inserter(partial_sort_nums));
    std::partial_sort(partial_sort_nums.begin(), partial_sort_nums.begin() + 15, partial_sort_nums.end()); // 前15为最小且有序
    print(partial_sort_nums);
    /*
    4. nth_element(快排的一部分)
    使范围中 “第 n 个元素”（索引从 0 开始）处于 “正确的排序位置”—— 所有小于它的元素在左边，
    大于它的元素在右边（左右元素无需有序），适合快速找 “第 k 小 / 大元素” 或中
    */
    std::vector<int> nth_element_nums;
    std::copy(nums.begin(), nums.end(), std::back_inserter(nth_element_nums));
    std::nth_element(nth_element_nums.begin(), nth_element_nums.begin() + 15, nth_element_nums.end());
    print(nth_element_nums);
    /*
    5.二分查找函数（需基于已排序范围）
    */
    // a. lower_bound 查找范围中 “第一个 ≥ value” 的元素，返回其迭代器（未找到返回 last）。
    auto it_lower = std::lower_bound(stable_sort_nums.begin(), stable_sort_nums.end(), 30);
    std::cout << *it_lower << std::endl;
    // b.upper_bound 查找范围中 “第一个> value” 的元素，返回其迭代器（未找到返回 last）。
    auto it_upper = std::upper_bound(stable_sort_nums.begin(), stable_sort_nums.end(), 30);
    std::cout << *it_upper << std::endl;
    // c.binary_search 检查范围中是否存在 value，返回 bool（本质是基于 lower_bound 实现，效率 O(log n)）。
    bool flag = std::binary_search(stable_sort_nums.begin(), stable_sort_nums.end(), 30);
    if (flag)
        std::cout << "30有" << (it_upper - it_lower) << "个" << std::endl;
    else
        std::cout << "30 不存在！" << std::endl;
    /*6. merge 合并两个**已排序范围**为一个新的有序范围（默认升序，保持稳定性），适合将两个有序数组 /vector 合并为一个有序容器。
     */
    std::vector<int> merge_nums;
    merge_nums.reserve(sort_nums.size() + stable_sort_nums.size());
    std::sort(sort_nums.begin(), sort_nums.end()); // 默认升序
    std::merge(sort_nums.begin(), sort_nums.end(), stable_sort_nums.begin(), stable_sort_nums.end(), std::back_inserter(merge_nums));
    print(merge_nums);

    return 0;
}