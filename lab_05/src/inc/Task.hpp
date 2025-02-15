#pragma once

#include <chrono>
#include <string>
#include <vector>

class Task {
public:
  int id;
  int stage;
  std::chrono::time_point<std::chrono::high_resolution_clock> timestamp;
  std::vector<std::chrono::microseconds> times;

  std::string filename;
  std::string data;

  std::string title;
  std::string image;
  std::string ingredients;
  std::vector<std::string> steps;

public:
  Task() {}
  Task(int id, const std::string &filename) : id(id), filename(filename) {
    timestamp = std::chrono::high_resolution_clock::now();
  }

  std::chrono::microseconds nextStage() {
    std::chrono::time_point<std::chrono::high_resolution_clock> t2 =
        std::chrono::high_resolution_clock::now();

    times.push_back(
        std::chrono::duration_cast<std::chrono::microseconds>(t2 - timestamp));

    stage++;

    timestamp = std::chrono::high_resolution_clock::now();

    return times.back();
  }
};
