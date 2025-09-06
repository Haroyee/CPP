#include <iostream>
using namespace std;

int returnNump(int *p)
{
    return *p;
}

int returnNumf(int &p)
{
    return p;
}

int main()
{
    int *p;
    int a = 100;
    p = &a;
    cout << "*p:" << returnNump(p) << "\n"
         << "a via ref:" << returnNumf(*p);

    return 0;
}