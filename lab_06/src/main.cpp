#include "AntPF.hpp"
#include "FullPF.hpp"
#include "Timer.hpp"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

void readMatrix(BasePathFinder::Matrix &matrix, const std::string &path) {
  std::ifstream file(path);
  std::string line;

  while (std::getline(file, line)) {
    std::stringstream ss(line);
    double value;
    std::vector<double> row;

    while (ss >> value) {
      row.push_back(value);
    }

    matrix.push_back(row);
  }

  file.close();
}

double GetCpuTime(BasePathFinder &finder,
                  const BasePathFinder::Matrix &matrix) {
  return funcTime([&finder, &matrix]() { finder.find(matrix); });
}

void measuring() {
  int n = 10;
  std::vector<std::pair<std::string, std::pair<double, double>>> data = {
      {"../data/mat3.txt", {0, 0}},
      {"../data/mat4.txt", {0, 0}},
      {"../data/mat5.txt", {0, 0}},
      {"../data/mat10.txt", {0, 0}},
  };

  for (int j = 0; j < data.size(); j++) {
    BasePathFinder::Matrix matrix;
    readMatrix(matrix, data[j].first);

    for (int i = 0; i < n; i++) {
      auto finder = FullCombinationPathFinder();

      auto antFinder = AntPathFinder();
      antFinder.setAlpha(0.8);
      antFinder.setKEvaporation(0.5);
      antFinder.setE(1.0);
      antFinder.setDays(20);

      data[j].second.first += GetCpuTime(finder, matrix);
      data[j].second.second += GetCpuTime(antFinder, matrix);
    }
    data[j].second.first /= n;
    data[j].second.second /= n;

    std::cout << data[j].first << " " << data[j].second.first << " "
              << data[j].second.second << std::endl;
  }
}

void parametrization() {
  std::vector<double> alpha_arr;
  for (int num = 1; num < 10; ++num)
    alpha_arr.push_back(num / 10.0);

  std::vector<double> k_eva_arr;
  for (int num = 1; num < 9; ++num)
    k_eva_arr.push_back(num / 10.0);

  std::vector<int> days_arr = {10, 20, 30};
  std::vector<double> e_arr = {0.5, 1, 2};

  int size = 10;

  BasePathFinder::Matrix matrix;
  readMatrix(matrix, "../data/mat10_1.txt");
  // readMatrix(matrix, "../data/mat10_2.txt");
  // readMatrix(matrix, "../data/mat10_3.txt");

  std::ofstream file("../outData/parametrization_class1.csv");
  // std::ofstream file("../outData/parametrization_class2.csv");
  // std::ofstream file("../outData/parametrization_class3.csv");

  auto finder = FullCombinationPathFinder();
  auto optimal = finder.find(matrix);

  int count = 0;
  int count_all = alpha_arr.size() * k_eva_arr.size();

  std::cout << std::endl;

  for (const auto &alpha : alpha_arr) {
    for (const auto &k_eva : k_eva_arr) {
      count++;

      for (const auto &days : days_arr) {
        for (const auto &e : e_arr) {
          auto antFinder = AntPathFinder();
          antFinder.setAlpha(alpha);
          antFinder.setKEvaporation(k_eva);
          antFinder.setE(e);
          antFinder.setDays(days);

          auto res = antFinder.find(matrix);

          std::string sep, ender;
          sep = ", ";
          ender = "\n";

          std::string str = std::to_string(alpha) + sep +
                            std::to_string(k_eva) + sep + std::to_string(days) +
                            sep + std::to_string(e) + sep +
                            std::to_string(optimal.first) + sep +
                            std::to_string(res.first - optimal.first) + ender;

          file << str;
        }

        std::cout << "Progress: " << std::fixed << std::setprecision(1)
                  << (static_cast<double>(count) / count_all) * 100 << "%"
                  << '\r';
      }
    }
  }

  file.close();
}

int main() {
#if 1
  BasePathFinder::Matrix matrix;

  readMatrix(matrix, "../data/mat5.txt");

  auto finder = FullCombinationPathFinder();

  auto result = finder.find(matrix);
  std::cout << "Full Combination: Minimum Distance: " << result.first
            << "\nBest Way: ";
  for (int city : result.second) {
    std::cout << city << " ";
  }
  std::cout << std::endl;

  auto antFinder = AntPathFinder();

  antFinder.setAlpha(0.8);
  antFinder.setKEvaporation(0.5);
  antFinder.setE(1.0);
  antFinder.setDays(20);

  result = antFinder.find(matrix);
  std::cout << "Ant Algorithm: Minimum Length: " << result.first
            << "\nBest Way : ";
  for (int city : result.second) {
    std::cout << city << " ";
  }
  std::cout << std::endl;

#else
  // measuring();

  // parametrization();
#endif

  return 0;
}