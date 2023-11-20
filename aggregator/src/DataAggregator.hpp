#pragma once
#include "ConcurrentQueue.hpp"
#include <atomic>
#include <condition_variable>

namespace aggregator {
class DataAggregator {
private:
    ConcurrentQueue<int>& _input;
    int _currentSum = 0;
    int _size = 0;

    std::atomic<bool> _stopped;
    std::condition_variable& _inputCond;

public:
    DataAggregator(ConcurrentQueue<int>& input) 
        : _input(input), _stopped(false), _inputCond(input.condition_variable()) {}

    void stop() {
        _stopped = false;
        _inputCond.notify_all();
    }

    std::thread aggregate() {
        std::thread aggregationThread(&DataAggregator::aggregateUntilStopped, this);
        return aggregationThread;
    }

    int result() {
        return _currentSum / _size;
    }

private:
    void aggregateNext() {
        _currentSum += _input.front(_stopped);
        _size ++;
        _input.pop();
    }

    void aggregateUntilStopped() {
        while (!_stopped) {
            aggregateNext();
        }
    }

};
}
