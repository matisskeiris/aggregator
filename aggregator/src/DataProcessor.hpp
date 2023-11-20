#pragma once
#include <atomic>
#include <condition_variable>
#include <vector>

#include "ConcurrentQueue.hpp"

namespace aggregator {
class DataProcessor {
private:
    ConcurrentQueue<std::vector<int>>& _input;
    ConcurrentQueue<int>& _output;

    std::atomic<bool> _stopped;
    std::condition_variable& _input_cond;

public:
    DataProcessor(ConcurrentQueue<std::vector<int>>& input, ConcurrentQueue<int>& output) 
        : _input(input), _output(output), _stopped(false), _input_cond(input.condition_variable()) {}

    void stop() {
        _stopped = true;
        _input_cond.notify_all();
    }

    std::thread process() {
        std::thread processingThread(&DataProcessor::processUntilStopped, this);
        return processingThread;
    }

private:
    void processNext() {
        auto& vector = _input.front(_stopped);

        int result = 0;
        for (int i = 0 ; i < vector.size() ; i ++) {
            result += vector[i];
        }

        _output.push(result / vector.size());
        _input.pop();
    }

    void processUntilStopped() {
        while (!_stopped) {
            processNext();
        }
    }
};
}
