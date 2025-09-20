#include <iostream>
#include <string>
/*类的单例模式*/

/*经典单例模式*/
class ClassicSingleton
{
private:
    static ClassicSingleton *instance;
    ClassicSingleton() {}; // 构造函数私有化，防止外部实例化

    ClassicSingleton(const ClassicSingleton &R) = delete;            // 禁用拷贝函数
    ClassicSingleton &operator=(const ClassicSingleton &R) = delete; // 禁用拷贝赋值函数
public:
    static ClassicSingleton *getInstence()
    {
        /*下方被框选的代码删除即为饿汉式单例模式*/
        /*-----------------------*/
        if (instance == nullptr)
        {
            instance = new ClassicSingleton;
        }
        /*----------------------*/
        return instance;
    }
    ~ClassicSingleton()
    {
        if (!instance)
            delete instance;
    }

    static void log(const std::string &str) // 静态方法
    {
        std::cout << "log : " << str << std::endl;
    }
    static void destroy()
    {
        if (instance != nullptr)
        {
            delete instance;
            instance = nullptr;
        }
        std::cout << "this class has destroyed" << std::endl;
    }
    void show()
    {
        std::cout << "this just a show!" << std::endl;
    }
};
/*懒汉式-初始化方式*/
ClassicSingleton *ClassicSingleton::instance = nullptr;
/*饿汉式-初始化方式*/
/*
Classic_Singleton *Classic_Singleton::instance = new Classic_Singleton();
*/

/*Meyers' Singleton(推荐)*/
class MeyerSingleton
{
private:
    MeyerSingleton() {};
    MeyerSingleton(const MeyerSingleton &m) = delete;
    MeyerSingleton &operator=(const MeyerSingleton &m) = delete;

public:
    static MeyerSingleton *getInstance()
    {
        static MeyerSingleton instance;
        return &instance;
    }
    // 不提供销毁函数，因为静态资源由系统自动管理
    void show()
    {
        std::cout << "this just a show!" << std::endl;
    }
};

int main()
{

    ClassicSingleton::log("hello world!");
    ClassicSingleton::getInstence()->show();
    ClassicSingleton::destroy();
    return 0;
}