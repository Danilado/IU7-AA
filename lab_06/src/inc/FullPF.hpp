#pragma once

#include "BasePF.hpp"

class FullCombinationPathFinder : public BasePathFinder {
public:
  virtual ~FullCombinationPathFinder() = default;

  virtual std::pair<double, std::vector<int>> find(const Matrix &mat) override;
};