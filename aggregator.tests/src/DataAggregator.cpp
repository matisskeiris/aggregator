#include <gtest/gtest.h>

#include "DataAggregator.hpp"

TEST(aggregator, DataAggregator) {
	// Arrange
	std::atomic_bool cancellationToken = true;
	aggregator::ConcurrentQueue<int> inputQueue;
	aggregator::DataAggregator dataAggregator(inputQueue);

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
		inputQueue.push(sum / 10);
	}

	// Act
	auto aggregationThread = dataAggregator.aggregate();
	dataAggregator.stop();
	aggregationThread.join();

	// Assert
	EXPECT_EQ(averageSums / 100, dataAggregator.result());

}