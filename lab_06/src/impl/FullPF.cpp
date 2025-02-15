#include "FullPF.hpp"

#include <algorithm>
#include <limits>
#include <ranges>

namespace {
using namespace std;

void permutations(vector<vector<int>> &res, vector<int> arr, int idx) {

  if (idx == arr.size()) {
    res.push_back(arr);
    return;
  }

  for (int i = idx; i < arr.size(); i++) {
    swap(arr[idx], arr[i]);

    permutations(res, arr, idx + 1);

    swap(arr[idx], arr[i]);
  }
}

vector<vector<int>> permute(vector<int> &arr) {
  vector<vector<int>> res;

  permutations(res, arr, 0);
  return res;
}
} // namespace

std::pair<double, std::vector<int>>
FullCombinationPathFinder::find(const Matrix &mat) {
  std::vector<int> places(mat.size());
  for (const int &i : std::views::iota(0ull, mat.size()))
    places[i] = i;

  std::vector<std::vector<int>> placesCombinations = permute(places);

  double minDist = std::numeric_limits<double>::infinity();
  std::vector<int> bestWay;

  for (const auto &combination : placesCombinations) {
    double curDist = 0;
    for (const int &j : std::views::iota(0ull, mat.size() - 1)) {
      int startCity = combination[j];
      int endCity = combination[j + 1];
      curDist += mat[startCity][endCity];
    }

    curDist += mat[combination[mat.size() - 1]][combination[0]];

    if (curDist < minDist) {
      minDist = curDist;
      bestWay = combination;
      bestWay.push_back(bestWay[0]);
    }
  }

  return {minDist, bestWay};
}