#pragma once

#include "DBManager.hpp"
#include "Logger.hpp"
#include "Task.hpp"
#include <atomic>
#include <queue>
#include <tbb/concurrent_queue.h>
#include <tbb/concurrent_vector.h>
#include <thread>

class TaskManager {
private:
  tbb::concurrent_queue<Task> stage1Queue;
  tbb::concurrent_queue<Task> stage2Queue;
  tbb::concurrent_queue<Task> stage3Queue;
  tbb::concurrent_vector<Task> finished;
  std::atomic<int> taskCounter{0};

  std::atomic<bool> running = false;

  Logger logger;
  DatabaseManager db;

  void logTask(const Task &task, const std::string &message,
               const std::chrono::microseconds &time);

  void readFileStage();
  void processDataStage();
  void writeToDBStage();

public:
  TaskManager();
  void run();
  void stop();
  void push(Task t);
  const tbb::concurrent_vector<Task> &getFinished() { return finished; }
};