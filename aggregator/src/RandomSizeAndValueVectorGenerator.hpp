#pragma once
#include <algorithm>
#include <vector>

#include "VectorGenerator.hpp"

namespace aggregator {
class RandomSizeAndValueVectorGenerator : public VectorGenerator {
public:
  std::vector<int> create() {
    auto size = (std::rand() % 255) + 1;
    std::vector<int> result(size);

    std::generate(result.begin(), result.end(), rand);

    return result;
  }
};
}
