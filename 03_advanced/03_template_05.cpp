#include <iostream>
#include <memory>
/*实现双向链表-list*/
template <class T>
class List
{
private:
    struct Node // 链表节点结构体
    {
        std::weak_ptr<Node> prior;
        std::shared_ptr<Node> next;
        T data;
        Node(const T &d) : data(d), next(nullptr) {}
    };
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    size_t length;

public:
    List() : head(nullptr), tail(nullptr), length(0) {}
    ~List() = default;
    List(const List &) = delete;
    List &operator=(const List &) = delete;
    List(List &&other) noexcept : head(std::move(other.head)), tail(std::move(other.tail)), length(other.length)
    {
        other.length = 0;
    }
    List &operator=(List &&other) noexcept
    {
        if (this != &other)
        {
            head = std::move(other.head);
            tail = std::move(other.tail);
            length = other.length;
            other.length = 0;
        }
        return *this;
    }
    bool empty() const
    {
        return head == nullptr;
    }

    size_t size() const
    {
        return length;
    }

    T &front()
    {
        if (empty())
            throw std::runtime_error("list is empty");
        return head->data;
    }

    const T &front() const
    {
        if (empty())
            throw std::runtime_error("list is empty");
        return head->data;
    }

    T &back()
    {
        if (empty())
            throw std::runtime_error("list is empty");
        return tail->data;
    }

    const T &back() const
    {
        if (empty())
            throw std::runtime_error("list is empty");
        return tail->data;
    }

    void push_front(const T &d)
    {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(d);

        if (empty())
            tail = head = newNode;
        else
        {
            newNode->next = head;
            head->prior = newNode;
            head = newNode;
        }
        ++length;
    }
    void push_back(const T &d)
    {
        std::shared_ptr<Node> newNode = std::make_shared<Node>(d);

        if (empty())
            tail = head = newNode;
        else
        {
            newNode->prior = tail;
            tail->next = newNode;
            tail = newNode;
        }
        ++length;
    }
    void insert(size_t pos, const T &d)
    {
        if (pos > length)
            throw std::out_of_range("Position out of range");
        if (pos == 0)
            push_front(d);
        else if (pos == length)
            push_back(d);

        else
        {
            auto p = head;
            for (size_t i = 0; i < pos; ++i)
                p = p->next;
            std::shared_ptr<Node> newNode = std::make_shared<Node>(d);
            newNode->next = p;
            newNode->prior = p->prior;
            p->prior.lock()->next = newNode;
            p->prior = newNode;

            ++length;
        }
    }
    void pop_front()
    {
        if (empty())
            throw std::runtime_error("list is empty.");

        if (length == 1)
            tail = head = nullptr;
        else
        {
            head = head->next;
            head->prior.reset();
        }
        length--;
    }
    void pop_back()
    {
        if (empty())
            throw std::runtime_error("list is empty.");

        if (length == 1)
            head = tail = nullptr;
        else
        {
            tail = tail->prior.lock();
            tail->next = nullptr;
        }
        length--;
    }
    void erase(size_t pos)
    {
        if (!(pos < length))
            throw std::out_of_range("Position out of range");
        if (pos == 0)
            pop_front();
        else if (pos == length - 1)
            pop_back();
        else
        {
            auto p = head;
            for (size_t i = 0; i < pos; ++i)
                p = p->next;
            p->prior.lock()->next = p->next;
            p->next->prior = p->prior;
            --length;
        }
    }
    void clear()
    {
        while (!empty())
            pop_front();
    }
    void print() const
    {
        auto current = head;
        std::cout << "链表内容: ";
        while (current)
        {
            std::cout << current->data;
            if (current->next)
            {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << " -> nullptr" << std::endl;
    }
};

int main()
{
    List<int> ls;
    for (int i = 0; i < 10; ++i)
    {
        ls.push_front(i);
    }
    ls.print();
    ls.pop_front();
    ls.print();
    return 0;
}