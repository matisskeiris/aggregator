#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <queue>
#include <vector>
#include <mutex>

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

class VectorGenerator {
public:
  virtual std::vector<int> create() = 0;
};

class RandomSizeAndValueVectorGenerator : public VectorGenerator {
public:
  std::vector<int> create() {
    auto size = std::rand() % 255;
    std::vector<int> result(size);

    std::generate(result.begin(), result.end(), rand);

    return result;
  }
};

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
        std::cout << "Generating" << std::endl;
        _output.push(_vectorGenerator->create()); 
    }
};

class DataProcessor {
private:
    ConcurrentQueue<std::vector<int>>& _input;
    ConcurrentQueue<int>& _output;

public:
    DataProcessor(ConcurrentQueue<std::vector<int>>& input, ConcurrentQueue<int>& output) 
        : _input(input),
        _output(output) {}

    void processNext() {
        std::cout << "Processing" << std::endl;
        auto& vector = _input.front();

        int result = 0;
        for (int i = 0 ; i < vector.size() ; i ++) {
            result += vector[i];
        }

        _output.push(result / vector.size());
        _input.pop();
    }
};

class DataAggregator {
private:
    ConcurrentQueue<int>& _input;
    int _currentSum = 0;
    int _size = 0;

public:
    DataAggregator(ConcurrentQueue<int>& input) 
        : _input(input) {}

    void aggregateNext() {
        std::cout << "Aggregating" << std::endl;
        _currentSum += _input.front();
        _size ++;
        _input.pop();
        std::cout << result() << std::endl;
    }

    int result() {
        return _currentSum / _size;
    }
};

void generateVectors(int size, DataGenerator& dataGenerator) {
    for (int i = 0 ; i < size ; i ++) {
        dataGenerator.generateNext();
    }
}

void processData(int size, DataProcessor& dataProcessor) {
    for (int i = 0 ; i < size ; i ++) {
        dataProcessor.processNext();
    }
}

void aggregateData(int size, DataAggregator& dataAggregator) {
    for (int i = 0 ; i < size ; i ++) {
        dataAggregator.aggregateNext();
    }
}

int main() {
    ConcurrentQueue<std::vector<int>> integerVectors;
    ConcurrentQueue<int> processedAverages;
    
    DataGenerator dataGenerator(std::make_unique<RandomSizeAndValueVectorGenerator>(), integerVectors);
    DataProcessor dataProcessor(integerVectors, processedAverages);
    DataAggregator dataAggregator(processedAverages);

    auto integerVectorSize = 5; //std::rand();

    std::thread generationThread(generateVectors, integerVectorSize, std::ref(dataGenerator));
    std::thread processorThread(processData, integerVectorSize, std::ref(dataProcessor));
    std::thread aggregatorThread(aggregateData, integerVectorSize, std::ref(dataAggregator));

    generationThread.join();
    processorThread.join();
    aggregatorThread.join();

    return 0;
}


