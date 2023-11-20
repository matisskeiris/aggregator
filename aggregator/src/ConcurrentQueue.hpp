#pragma once
#include <queue>
#include <mutex>

namespace aggregator {

/// <summary>
/// Thread safe queue implementation.
///     Exposes default queue endpoints, push, pop, front, empty.
/// </summary>
/// <typeparam name="T"></typeparam>
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

    /// <summary>
    /// Return value that is at front of the queue. If value is not set, waits until value is pushed.
    /// </summary>
    /// <param name="result">Returns reference to front value</param>
    /// <param name="cancellationToken">Reference to cancellation token. Set value to true and notify_all using confition_variable reference</param>
    /// <returns>true if value was found, false if it was cancelled and no value was present at time of cancellation</returns>
    bool front(T& result, std::atomic<bool>& cancellationToken) {
        std::unique_lock<std::mutex> lock(_mutex);

        // Either queue not empty or cancelled
        _cond.wait(lock, [this, &cancellationToken]() { return !_internal.empty() || cancellationToken; });
        
        if (_internal.empty()) return false;

        result = _internal.front();
        return true;
    }

    bool empty() {
        std::unique_lock<std::mutex> lock(_mutex);
        return _internal.empty();
    }

    void pop() {
        std::unique_lock<std::mutex> lock(_mutex);
        _internal.pop();
    }
};

}
