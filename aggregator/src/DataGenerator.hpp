#pragma once
#include <chrono>
#include <memory>
#include <random>
#include <thread>

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


    std::thread generate() {
        std::thread generationThread(&DataGenerator::generateForRandomTime, this);
        return generationThread;
    }

private:
    void generateForRandomTime() {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::uniform_int_distribution<> distribution(0, 10000);

        std::chrono::milliseconds duration(distribution(gen));

        auto startTime = std::chrono::system_clock::now();

        while (std::chrono::system_clock::now() < startTime + duration) {
            generateNext();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    void generateNext() {
        _output.push(_vectorGenerator->create()); 
    }

};
}
