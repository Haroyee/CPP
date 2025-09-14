#include <iostream>
#include <memory>
/*weak_ptr*/
class B; // 向前声明
class A
{
public:
    std::shared_ptr<B> b_ptr;
    ~A() { std::cout << "A destroyed" << std::endl; }
};

class B
{
public:
    std::weak_ptr<A> a_ptr; // 使用 weak_ptr 打破循环引用
    ~B() { std::cout << "B destroyed" << std::endl; }
};

int main()
{
    std::shared_ptr<A> a = std::make_shared<A>();
    std::shared_ptr<B> b = std::make_shared<B>();
    a->b_ptr = b;
    b->a_ptr = a;

    std::weak_ptr<A> c = std::make_shared<A>();
    /*判断是否过期*/
    std::cout << c.expired() << std::endl;
    /*返回所指向share_ptr,资源过期返回空的share_ptr*/
    auto d = c.lock();
    /*返回引用次数*/
    std::cout << c.use_count() << std::endl;

    return 0;
}