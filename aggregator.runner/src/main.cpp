#include <aggregator/ConcurrentQueue.hpp>
#include "aggregator/RandomLenghtVectorGenerator.hpp"
#include "aggregator/DataGenerator.hpp"
#include "aggregator/DataProcessor.hpp"
#include "aggregator/DataAggregator.hpp"

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


