#pragma once
#include "ConcurrentQueue.hpp"
#include <atomic>
#include <condition_variable>

#include <iostream>

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
        _stopped = true;
        _inputCond.notify_all();
    }

    std::thread aggregate() {
        std::thread aggregationThread(&DataAggregator::aggregateUntilStopped, this);
        return aggregationThread;
    }

    int result() {
        std::cout << "Size " << _size << std::endl;
        return _currentSum / _size;
    }

private:
    void aggregateNext() {
        int value = 0;
        if (!_input.front(value, _stopped)) {
            return;
        }
        _currentSum += value;
        _size ++;
        _input.pop();

        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }

    void aggregateUntilStopped() {
        while (!_stopped || !_input.empty()) {
            aggregateNext();
        }
    }

};
}
