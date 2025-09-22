#include <iostream>
#include <memory>
/*自定义删除器*/
class R
{
private:
    int *data;

public:
    R(int d)
    {
        data = new int(d);
        std::cout << "调用R的构造函数" << std::endl;
    }
    ~R()
    {
        delete this->data;
        std::cout << "调用R的析构函数" << std::endl;
    }
    R(const R &obj)
    {
        this->data = new int;
        *(this->data) = *(obj.data);
        std::cout << "调用R的拷贝函数" << std::endl;
    }
    R &operator=(const R &obj)
    {
        if (this != &obj)
        {
            delete this->data;
            this->data = new int;
            *(this->data) = *(obj.data);
            std::cout << "调用R的拷贝赋值函数" << std::endl;
        }
        return *this;
    }
    R(R &&obj) noexcept
    {
        this->data = obj.data;
        obj.data = nullptr;
        std::cout << "调用R的移动函数" << std::endl;
    }
    R &operator=(R &&obj) noexcept
    {
        delete this->data;
        this->data = obj.data;
        obj.data = nullptr;
        std::cout << "调用R的移动赋值函数" << std::endl;
        return *this;
    }
};

void deleter(R *r)
{
    std::cout << "调用自定义删除器" << std::endl;
    delete r;
}
int main()
{
    /*unique_ptr*/
    std::unique_ptr<R, decltype(deleter) *> up(new R(10), deleter);
    std::unique_ptr<R, void (*)(R *)> up1(new R(10), deleter); // 函数指针
    /*shared_ptr*/
    // std::shared_ptr<R> sp = std::make_shared<R>(10, deleter);//错误用法
    std::shared_ptr<R> sp1(new R(10), deleter);

    return 0;
}