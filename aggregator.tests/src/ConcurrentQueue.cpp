#include <gtest/gtest.h>

#include "ConcurrentQueue.hpp"

void addToQueue(std::vector<int> vector, aggregator::ConcurrentQueue<int>& queue) {
    for (int i = 0; i < vector.size(); i++) {
        queue.push(vector[i]);
    }
}

void removeFromQueue(aggregator::ConcurrentQueue<int>& queue, int& result, std::atomic_bool& cancellationToken)
{
    int value = 0;
    while (queue.front(value, cancellationToken)) {
        result += value;
        queue.pop();
    }
}

TEST(Aggregator, ConcurrentQueue) {
    // Arrange
    auto size = 1000;
    std::vector<int> vector(size);
    std::atomic_bool cancellationToken = false;

    int result = 0;

    aggregator::ConcurrentQueue<int> queue;

    auto sum = 0;
    for (int i = 0; i < size; i++) {
        vector[i] = i;
        sum += i;
    }

    // Act
    std::thread t1(addToQueue, std::ref(vector), std::ref(queue));
    std::thread t2(addToQueue, std::ref(vector), std::ref(queue));
    std::thread t3(removeFromQueue, std::ref(queue), std::ref(result), std::ref(cancellationToken));

    t1.join();
    t2.join();
    cancellationToken = true;
    queue.condition_variable().notify_all();
    t3.join();

    // Assert
    EXPECT_EQ(result, sum * 2);
}
