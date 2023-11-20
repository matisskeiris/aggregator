#include <gtest/gtest.h>

#include "DataGenerator.hpp"

class MockGenerator : public aggregator::VectorGenerator {
private: 
	int _index = 0;
	int& _sum;
public:
	MockGenerator(int& sum) : _sum(sum) {}

	std::vector<int> create() override
	{
		std::vector<int> vector(10);
		for (int i = 0; i < 10; i ++) {
			int value = _index + i + 10;
			vector[i] = value;
			_sum += value;
		}
		return vector;
	}
};

TEST(aggregator, DataGenerator) {
	// Arrange
	std::atomic_bool cancellationToken = true;
	int sum = 0;
	aggregator::ConcurrentQueue<std::vector<int>> queue;
	aggregator::DataGenerator dataGenerator(std::make_unique<MockGenerator>(sum), queue);

	// Act
	auto generationThread = dataGenerator.generate();

	generationThread.join();

	// Assert
	int result = 0;
	while (!queue.empty()) {
		std::vector<int> vector;
		queue.front(vector, cancellationToken);
		for (int i = 0; i < vector.size(); i++) {
			result += vector[i];
		}
		queue.pop();
	}

	EXPECT_EQ(sum, result);

}