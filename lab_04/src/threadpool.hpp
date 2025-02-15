#pragma once

#include <functional>
#include <queue>
#include <thread>
#include <vector>

using namespace std;

class ThreadPool {
public:
  ThreadPool(size_t threads) : threadcount(threads) {
    if (threadcount == 0)
      threadcount = 1;
    if (threadcount > thread::hardware_concurrency())
      threadcount = thread::hardware_concurrency();
  }

  void push_to_queue(function<void(void)> f);
  void run();

private:
  void dispatch(function<void(void)> f);

  queue<function<void(void)>> q;
  vector<thread> threads;
  size_t threadcount;
};