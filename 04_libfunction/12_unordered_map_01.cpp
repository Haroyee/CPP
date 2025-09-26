#include <iostream>
#include <unordered_map>
/*unordered_map-哈希表*/

class R
{
public:
    R() {};
    ~R() = default;
    R(const R &r) = default;
    R &operator=(const R &r) = default;
};

int main()
{
    /*声明*/
    std::unordered_map<int, R> umap;

    /*插入*/
    // 使用 insert() 成员函数,insert 方法在键不存在时插入，返回一个 pair<iterator, bool>，其中 bool 表示插入是否成功。
    //  使用insert插入pair
    umap.insert(std::pair<int, R>(0, R()));

    // 使用make_pair
    umap.insert(std::make_pair(1, R()));

    // 使用初始化列表方式插入（C++11）
    auto result = umap.insert({2, R()});

    // 检查插入是否成功,返回一个 pair<iterator, bool>，其中 bool 表示插入是否成功。
    if (result.second)
    {
        std::cout << "Insertion of Bob succeeded.\n";
    }

    // 使用下标操作符插入（如果键不存在则插入，存在则更新）
    umap[3] = R();

    // 使用emplace直接构造元素
    umap.emplace(4, R()); // 等同于 umap.insert({4, R()})，但可能更高效

    /*查询*/
    // 使用下标操作符 [] 不推荐,访问不存在的键（危险！会插入新元素）
    R r0 = umap[0];
    // 使用 at() 成员函数,如果键不存在，抛出 std::out_of_range 异常。
    R r1 = umap.at(1);

    /*删除*/
    // 通过键删除。返回删除的元素个数（0 或 1）
    size_t count = umap.erase(2);
    // 通过迭代器删除。返回被删除元素之后元素的迭代器
    auto it = umap.find(2);
    if (it != umap.end())
    {
        umap.erase(it);
    }

    /*检查元素是否存在*/
    // 方法 1: 使用 count()。返回匹配键的数量（0 或 1）
    if (umap.count(2) < 0)
    {
        std::cout << "2 not exists.\n";
    }
    // 方法 2: 使用 find()

    /*哈希表特定操作*/
    // 获取哈希表长
    size_t hash_size = umap.bucket_count();
    // 获取特定键所在的编号
    size_t hash_a = umap.bucket(2);
    // 获取特定编号的元素数量
    size_t hash_size_ = umap.bucket_size(hash_a);
    // 获取负载因子：元素数量 / 表长
    float load_factor = umap.load_factor();
    std::cout << "Current load factor: " << load_factor << std::endl;
    // 获取最大负载因子
    float max_lf = umap.max_load_factor();
    std::cout << "Max load factor: " << max_lf << std::endl;
    // 设置最大负载因子。超过此值，容器会自动增加桶数量（rehash）
    umap.max_load_factor(0.8f);
    // 手动重新哈希，设置一个指定的最小表长
    umap.rehash(100); // 确保表长至少为 100

    return 0;
}