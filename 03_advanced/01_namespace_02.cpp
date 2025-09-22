#include <iostream>
/*嵌套的命名空间*/
namespace nspParents
{
    void helloWorld()
    {
        std::cout << "hello world!" << std::endl;
        return;
    }
    namespace nspChild
    {
        void helloWorld()
        {
            std::cout << "hello everybody!" << std::endl;
            return;
        }
    }
}
int main()
{

    nspParents::helloWorld();
    namespace nsp = nspParents::nspChild; // 调用子命名空间
    nsp::helloWorld();
    return 0;
}