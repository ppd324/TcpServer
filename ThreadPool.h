//
// Created by 裴沛东 on 2021/12/29.
//

#ifndef TCPSERVER_THREADPOOL_H
#define TCPSERVER_THREADPOOL_H
#include <thread>
#include <condition_variable>
#include <vector>
#include <queue>
#include <mutex>
#include <functional>
class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasksQueue;
    std::mutex taskMutex;
    std::condition_variable cv;
    bool stop;
public:
    explicit ThreadPool(int size = std::thread::hardware_concurrency()*5);
    ~ThreadPool();
    void addTask(std::function<void()> task);


};


#endif //TCPSERVER_THREADPOOL_H
