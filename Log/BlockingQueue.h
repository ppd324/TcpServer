//
// Created by ppd on 2022/1/9.
// 阻塞队列
//

#ifndef TCPSERVER_BLOCKINGQUEUE_H
#define TCPSERVER_BLOCKINGQUEUE_H
#include <deque>
#include <mutex>
#include <thread>
#include <cassert>
#include <condition_variable>

template<typename T>
class BlockingQueue {
public:
    explicit BlockingQueue(size_t MAXCAPACITY = 1000);
    ~BlockingQueue();
    void close();
    void clear();
    bool empty();
    bool full();
    T front();
    size_t size();
    size_t capacity() const;
    void push(T &val);
    void push(T val);
    bool pop(T &val);
    void flush();
private:
    std::deque<T> _que;
    size_t _capacity;
    std::mutex _mutex;
    bool _isClose;
    std::condition_variable _conConsumer;
    std::condition_variable _conProducer;
};

template <typename T>
BlockingQueue<T>::BlockingQueue(size_t MAXCAPACITY):_capacity(MAXCAPACITY) {
    assert(_capacity > 0);
    _isClose = false;
}
template <typename T>
void BlockingQueue<T>::clear() {
    std::lock_guard<std::mutex> lock(_mutex);
    _que.clear();
}

template <typename T>
size_t BlockingQueue<T>::capacity() const {
    return _capacity;
}
template <typename T>
bool BlockingQueue<T>::empty() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _que.empty();
}
template <typename T>
bool BlockingQueue<T>::full() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _que.size() >= _capacity;
}
template <typename T>
T BlockingQueue<T>::front() {
    std::lock_guard<std::mutex> lock(_mutex);
    return _que.front();
}
template <typename T>
void BlockingQueue<T>::push(T &val) {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_que.size() >= _capacity) {
        _conProducer.wait(lock);
        if(_isClose) return;
    }
    _que.push_back(val);
    _conConsumer.notify_one();

}

template <typename T>
void BlockingQueue<T>::push(T val) {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_que.size() >= _capacity) {
        _conProducer.wait(lock);
        if(_isClose) return;
    }
    _que.push_back(val);
    _conConsumer.notify_one();

}
template <typename T>
bool BlockingQueue<T>::pop(T &val) {
    std::unique_lock<std::mutex> lock(_mutex);
    while(_que.empty()) {
        _conConsumer.wait(lock);
        if(_isClose) return false;
    }
    val = std::move(_que.front());
    _que.pop_front();
    _conProducer.notify_one();
    return true;

}
template <typename T>
void BlockingQueue<T>::flush() {
    _conConsumer.notify_one();
}
template <typename T>
void BlockingQueue<T>::close() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _isClose = true;
        _que.clear();
    }
    _conConsumer.notify_all();
    _conProducer.notify_all();

}
template <typename T>
BlockingQueue<T>::~BlockingQueue<T>() {
    close();
}
#endif //TCPSERVER_BLOCKINGQUEUE_H
