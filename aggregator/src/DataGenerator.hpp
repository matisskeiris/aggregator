#pragma once
#include <memory>

#include "ConcurrentQueue.hpp"
#include "VectorGenerator.hpp"

namespace aggregator {
class DataGenerator {
private:
    std::unique_ptr<VectorGenerator> _vectorGenerator;
    ConcurrentQueue<std::vector<int>>& _output;

public:
    DataGenerator(std::unique_ptr<VectorGenerator> vectorGenerator,
                  ConcurrentQueue<std::vector<int>>& output) 
        : _vectorGenerator(std::move(vectorGenerator)), 
        _output(output) {}

    void generateNext() {
        _output.push(_vectorGenerator->create()); 
    }
};
}
