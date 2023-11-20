#pragma once
#include "ConcurrentQueue.hpp"

namespace aggregator {
class DataAggregator {
private:
    ConcurrentQueue<int>& _input;
    int _currentSum = 0;
    int _size = 0;

public:
    DataAggregator(ConcurrentQueue<int>& input) 
        : _input(input) {}

    void aggregateNext() {
        _currentSum += _input.front();
        _size ++;
        _input.pop();
    }

    int result() {
        return _currentSum / _size;
    }
};
}
