#include <iostream>
#include <algorithm>
#include <vector>
/*修改性序列操作（修改元素值或位置）*/
void print(std::vector<int> &num)
{
    std::for_each(num.begin(), num.end(), [](int i)
                  { std::cout << i << " "; });
    std::cout << std::endl;
}

int main()
{
    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    /*1.copy*/
    // copy
    std::vector<int> dest1(nums.size()); // 预分配
    std::copy(nums.begin(), nums.end(), dest1.begin());
    print(dest1);

    std::vector<int> dest2;
    std::copy(nums.begin(), nums.end(), std::back_inserter(dest2)); // 空容器，需用back_inserter自动插入
    print(dest2);
    // copy_if
    std::vector<int> dest3;
    std::copy_if(nums.begin(), nums.end(), std::back_inserter(dest3), [](int i)
                 { return i % 2 == 0; });
    print(dest3);

    /*2.move将范围 [first, last) 中的元素移动到目标范围（源元素会变为 “可析构状态”，避免复制开销，适合大型对象）。
     */
    std::vector<int> dest4;
    std::move(nums.begin(), nums.end(), std::back_inserter(dest4));
    print(dest4);

    /*3. swap_ranges 交换两个范围 [first1, last1) 和 [first2, first2 + (last1-first1)) 中的元素（两个范围需等长）。*/
    std::swap_ranges(dest4.begin(), dest4.begin() + 3, dest4.begin() + 3);
    print(dest4);
    /*4. transform 对范围元素应用函数（或二元操作），结果存入目标范围（支持 “一元转换” 或 “二元合并”）。*/
    // 一元
    std::vector<int> dest5;
    std::transform(nums.begin(), nums.end(), std::back_inserter(dest5), [](int i)
                   { return i * 2; });
    print(dest5);
    // 二元
    std::vector<int> dest6;
    std::transform(nums.begin(), nums.end(), dest5.begin(), std::back_inserter(dest6), [](int x, int y)
                   { return x + y; });
    print(dest6);
    /* 5. replace 将范围中等于 old_val 的元素替换为 new_val（原地修改）。*/
    std::replace(dest5.begin(), dest5.begin() + 3, 2, 0); // 2替换成0
    print(dest5);
    // replace_if
    std::replace_if(dest5.begin(), dest5.begin() + 3, [](int i)
                    { return i != 0; }, 0);
    print(dest5);

    /*6. fill*/
    std::fill(dest5.begin() + 3, dest5.end(), 0);
    print(dest5);
    /*9. generate 用函数 gen 的返回值填充范围（适合生成有规律的序列，如随机数、自增序列）。*/
    std::vector<int> dest7(10);
    std::generate(dest7.begin(), dest7.end(), []()
                  { return rand() % 10; });
    print(dest7);
    return 0;
}