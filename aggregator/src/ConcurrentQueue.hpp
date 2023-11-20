#pragma once
#include <queue>
#include <mutex>

namespace aggregator {

template<typename T>
class ConcurrentQueue {
private:
    std::queue<T> _internal;
    std::mutex _mutex;
    std::condition_variable _cond;

public:
    ConcurrentQueue() {
        _internal = std::queue<T>();
    }

    void push(T value) {
        std::unique_lock<std::mutex> lock(_mutex);

        _internal.push(value);
        
        _cond.notify_one();
    }

    const T& front() {
        std::unique_lock<std::mutex> lock(_mutex);

        _cond.wait(lock, [this]() { return !_internal.empty(); });

        return _internal.front(); 
    }

    void pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        _internal.pop();
    }

};

}
