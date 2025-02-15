#include "Logger.hpp"
#include <format>
#include <iostream>

std::string Logger::formatTime(const std::chrono::microseconds &duration) {
  return std::format("\t[{:<8}microseconds.]", duration.count());
}

void Logger::log(const std::string &text) {
  std::lock_guard<std::mutex> lock(blocker);
  std::cout << text << std::endl;
}

void Logger::logTask(const Task &task) {
  std::chrono::microseconds s;

  for (const auto &time : task.times)
    s += time;

  log(std::format("Task {} logging at stage {}; total time: {}", task.id,
                  task.stage, formatTime(s)));
}

void Logger::logTime(const std::string &text,
                     const std::chrono::microseconds &duration) {
  log(std::format("{} in {}", text, formatTime(duration)));
}