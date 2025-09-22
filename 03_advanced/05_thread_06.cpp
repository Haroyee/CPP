#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <fstream>
#include <string>
#include <atomic>
/*条件变量 (Condition Variable)-以生产者消费者问题为例*/
std::ofstream outfile("05_thread_06_data.txt");
std::queue<int> buffer;
std::queue<std::string> strBuffer;
std::mutex m;
std::condition_variable cv_producer, cv_consumer;
std::atomic<bool> finished(false);
const int max_size = 20;
std::atomic<int> au(0);

void producer(int count)
{

    ++au;
    for (int i = 0; i < count; ++i)
    {

        std::unique_lock<std::mutex> lg(m);
        cv_producer.wait(lg, []()
                         { return buffer.size() < max_size; }); // 当buffer不满时且被唤醒时执行以下代码
        /*生产商品*/
        buffer.push(i);
        /*----------写文件----------*/
        std::string line;
        line.reserve(sizeof('生') * 18);
        line.append("producer : 生产了");
        line.append(std::to_string(i));
        line.append("号商品\n");
        strBuffer.push(line);
        if (outfile.is_open() && i % 20 == 0)
        {

            while (!strBuffer.empty())
            {
                outfile << strBuffer.front();
                strBuffer.pop();
            }
        }
        /*------------------------------*/

        lg.unlock();
        cv_consumer.notify_one(); // 唤醒一个消费者
    }

    if (!--au)
    {
        finished = true;
        std::cout << "生产者生成完成！" << std::endl;
    }

    cv_consumer.notify_all();
}
void consumer(int id)
{
    int i = 0;

    while (true)
    {

        std::unique_lock<std::mutex> lg(m);

        cv_consumer.wait(lg, []
                         { return !buffer.empty() || finished; }); // 当buffer不为空或者生产任务已完成且被唤醒时执行以下代码
        if (buffer.empty() && finished)
            break;
        /*处理商品*/
        if (!buffer.empty())
        {
            ++i;
            std::string line;
            line.reserve(sizeof('生') * 18);
            line.append("consumer ");
            line.append(std::to_string(id));
            line.append(" : 消费了");
            line.append(std::to_string(buffer.front()));
            line.append("号商品\n");
            strBuffer.push(line);
            buffer.pop();
            if (outfile.is_open() && i % 20 == 0)
            {
                while (!strBuffer.empty())
                {
                    outfile << strBuffer.front();
                    strBuffer.pop();
                }
            }
        }
        lg.unlock();
        cv_producer.notify_one(); // 通知所有生产者
        // std::this_thread::sleep_for(std::chrono::milliseconds(50));//休眠50ms
    }
    std::cout << "consumer " << id << " : 消费完成！\n"
              << std::endl;
}
int main()
{
    auto start = std::chrono::system_clock::now();

    std::thread t1(producer, 100);
    std::thread t6(producer, 100);

    std::thread t2(consumer, 0);
    std::thread t3(consumer, 1);
    std::thread t4(consumer, 2);
    std::thread t5(consumer, 3);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    t6.join();

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    /*处理多余缓冲区*/
    if (!strBuffer.empty())
    {
        while (!strBuffer.empty())
        {
            outfile << strBuffer.front();
            strBuffer.pop();
        }
    }
    std::cout << "总耗时: " << elapsed_seconds.count() << "s\n";
    outfile << "总耗时: " << elapsed_seconds.count() << "s\n";
    outfile.close();
    return 0;
}