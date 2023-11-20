#pragma once
#include <vector>

#include "ConcurrentQueue.hpp"

namespace aggregator {
class DataProcessor {
private:
    ConcurrentQueue<std::vector<int>>& _input;
    ConcurrentQueue<int>& _output;

public:
    DataProcessor(ConcurrentQueue<std::vector<int>>& input, ConcurrentQueue<int>& output) 
        _output(output) {}

    void processNext() {
        auto& vector = _input.front();

        int result = 0;
        for (int i = 0 ; i < vector.size() ; i ++) {
            result += vector[i];
        }

        _output.push(result / vector.size());
        _input.pop();
    }
};
}
