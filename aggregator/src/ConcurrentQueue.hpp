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

    std::condition_variable& condition_variable() { return _cond; }

    void push(T value) {
        std::unique_lock<std::mutex> lock(_mutex);

        _internal.push(value);

        _cond.notify_one();
    }

    const T& front(std::atomic<bool>& cancellationToken) {
        std::unique_lock<std::mutex> lock(_mutex);

        // Either queue not empty or cancelled
        _cond.wait(lock, [this, &cancellationToken]() { return !_internal.empty() || cancellationToken; });

        return _internal.front();
    }

    void pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        _internal.pop();
    }
};

}
