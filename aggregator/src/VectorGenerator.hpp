#pragma once
#include <vector>

namespace aggregator {
class VectorGenerator {
public:
  virtual std::vector<int> create() = 0;
};
}
