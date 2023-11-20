#include "ConcurrentQueue.hpp"
#include "RandomSizeAndValueVectorGenerator.hpp"
#include "DataGenerator.hpp"
#include "DataProcessor.hpp"
#include "DataAggregator.hpp"
#include <iostream>

int main() {
    aggregator::ConcurrentQueue<std::vector<int>> integerVectors;
    aggregator::ConcurrentQueue<int> processedAverages;
    
    aggregator::DataGenerator dataGenerator(std::make_unique<aggregator::RandomSizeAndValueVectorGenerator>(), integerVectors);
    aggregator::DataProcessor dataProcessor(integerVectors, processedAverages);
    aggregator::DataAggregator dataAggregator(processedAverages);

    auto integerVectorSize = 5; //std::rand();

    auto generationThread = dataGenerator.generate();
    auto processorThread = dataProcessor.process();
    auto aggregatorThread = dataAggregator.aggregate();

    generationThread.join();
    dataProcessor.stop();

    processorThread.join();
    dataAggregator.stop();

    aggregatorThread.join();

    std::cout << "Aggregated result: " << dataAggregator.result() << std::endl;

    return 0;
}


