#include <gtest/gtest.h>

#include "DataProcessor.hpp"

TEST(aggregator, DataProcessor) {
	// arrange
	std::atomic_bool cancellationToken = true;
	aggregator::ConcurrentQueue<std::vector<int>> inputQueue;
	aggregator::ConcurrentQueue<int> outputQueue;
	aggregator::DataProcessor dataProcessor(inputQueue, outputQueue);

	int averageSums = 0;
	for (int i = 0; i < 100; i++) {
		int sum = 0;
		std::vector<int> vector(10);
		for (int j = 0; j < 10; j++) {
			int value = i + 10 + j;
			vector[j] = value;
			sum += value;
		}
		averageSums += sum / 10;
		inputQueue.push(vector);
	}

	// Act
	auto processingThread = dataProcessor.process();
	dataProcessor.stop();
	processingThread.join();

	// Assert
	int result = 0;
	while (!outputQueue.empty()) {
		int average = 0;
		outputQueue.front(average, cancellationToken);
		result += average;
		outputQueue.pop();
	}

	EXPECT_EQ(averageSums, result);

}