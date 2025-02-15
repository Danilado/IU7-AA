#pragma once

#include <vector>

class BasePathFinder {
public:
  using Matrix = std::vector<std::vector<double>>;

  virtual ~BasePathFinder() = default;

  virtual std::pair<double, std::vector<int>> find(const Matrix &mat) = 0;
};
