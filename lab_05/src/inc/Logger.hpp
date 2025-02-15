#pragma once

#include "Task.hpp"
#include <mutex>
#include <string>

class Logger {
private:
  std::mutex blocker;

  std::string formatTime(const std::chrono::microseconds &duration);

public:
  void log(const std::string &text);
  void logTask(const Task &task);
  void logTime(const std::string &text,
               const std::chrono::microseconds &duration);
};