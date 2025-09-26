#include <iostream>
#include <future>
#include <thread>
#include <memory>

class A
{
private:
    int a;
    int *b;

public:
    int get() const
    {
        return a;
    }
};

class B
{
private:
    B *b;
    A *a;

public:
    int get() const
    {
        return;
    }
};

int main()
{
    int a;
    int b;
    a = b = 4;

    return 0;
}