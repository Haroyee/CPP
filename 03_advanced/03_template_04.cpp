#include <iostream>
#include <random>
#include <ctime>
/*类模版-数组*/
template <class T>
class vector
{
private:
    T *data;
    int size;

public:
    vector()
    {
        data = nullptr;
        size = 0;
    }
    vector operator[](int i)
    {
        return *(this->data + i);
    }
    bool is_empty()
    {
        if (this->size)
            return false;
        else
            return true;
    }
    void push_back(T d)
    {
        T *temp = new T[size + 1];
        for (int i = 0; i < size; ++i)
        {
            *(temp + i) = *(this->data + i);
        }
        *(temp + size) = d;
        delete[] data;
        data = temp;
        ++this->size;
    }
    void pop_back()
    {
        if (this->is_empty())
        {
            std::cerr << "this vector is empty." << std::endl;
            return;
        }
        T *temp = new T[size - 1];
        for (int i = 0; i < size - 1; ++i)
        {
            *(temp + i) = *(this->data + i);
        }
        delete[] data;
        data = temp;
        --this->size;
    }
    T front()
    { // 访问第一个元素
        if (this->is_empty())
        {
            std::cerr << "this vector is empty." << std::endl;
            return NULL;
        }
        return *(this->data);
    }
    T back()
    {
        if (this->is_empty())
        {
            std::cerr << "this vector is empty." << std::endl;
            return NULL;
        }
        return *(this->data + size - 1);
    }
    int getSize()
    {
        return this->size;
    }
    void insert(int pos, T d)
    {
        if (pos < 0 || pos >= this->size)
        {
            std::cerr << "This coordinate has already exceeded the range of the vector." << std::endl;
            return;
        }
        T *temp = new T[this->size + 1];
        for (int i = 0, j = 0; i < this->size + 1; ++i)
        {
            if (pos == i)
            {
                *(temp + i) = d;
            }
            else
            {
                *(temp + i) = *(this->data + j);
                ++j;
            }
        }
        delete[] data;
        data = temp;
        ++this->size;
    }
    void erase(int pos)
    {
        if (pos < 0 || pos >= this->size)
        {
            std::cerr << "This coordinate has already exceeded the range of the vector." << std::endl;
            return;
        }
        T *temp = new T[this->size - 1];
        for (int i = 0, j = 0; i < size - 1; i++)
        {
            if (pos == i)
            {
                ++j;
            }
            *(temp + i) = *(this->data + j);
            ++j;
        }
        delete[] data;
        data = temp;
        --this->size;
    }
    void clear()
    {
        delete[] data;
        data = nullptr;
        this.szie = 0;
    }
    void show()
    {
        if (this->is_empty())
        {
            std::cerr << "this vector is empty." << std::endl;
            return;
        }

        for (int i = 0; i < size; ++i)
        {
            std::cout << *(this->data + i) << " ";
            if ((i + 1) % 20 == 0)
                std::cout << std::endl;
        }

        std::cout << std::endl;
    }
    ~vector()
    {
        delete[] data;
        data = nullptr;
        std::clog << "The destructor of the vector is called." << std::endl;
    }
};
int main()
{
    vector<int> numbers = vector<int>();
    std::srand(std::time(0));
    std::random_device rd;                       // 用于获取真随机数种子
    std::mt19937 gen(rd());                      // 使用 Mersenne Twister 算法
    std::uniform_int_distribution<> dis(1, 100); // 均匀整数分布，范围1-100

    for (int i = 0; i < 100; ++i)
        numbers.push_back(dis(gen));
    numbers.show();
    numbers.front();
    numbers.back();
    numbers.insert(20, 10000);
    numbers.show();
    numbers.erase(20);
    numbers.show();
    return 0;
}