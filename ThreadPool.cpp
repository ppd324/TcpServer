//
// Created by 裴沛东 on 2021/12/29.
//

#include "ThreadPool.h"
#include "util.h"

ThreadPool::ThreadPool(int size):stop(false) {
    {
        LOG_INFO("threadpool initialized,thread number is %d",size);
    }
    for(int i=0;i<size;++i) {
        threads.emplace_back([this]() {
            while(true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(taskMutex);
                    cv.wait(lock,[this](){
                       return stop || !tasksQueue.empty();
                    });
                    if(stop && tasksQueue.empty()) return;
                    task = std::move(tasksQueue.front());
                    tasksQueue.pop();
                }
                task();
            }

        });
    }


}
ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(taskMutex);
        stop = true;
    }
    cv.notify_all();
    for(auto &thread:threads) {
        if(thread.joinable()) {
            thread.join();
        }
    }

}
void ThreadPool::addTask(const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lock(taskMutex);
        if(stop)
            throw std::runtime_error("ThreadPoll already stop, can't add task any more");
        tasksQueue.push(task);
    }
    cv.notify_one();

}