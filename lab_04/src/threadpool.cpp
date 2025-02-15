#include "threadpool.hpp"
#include <iostream>
#include <thread>

void ThreadPool::push_to_queue(function<void(void)> f) { q.push(f); }

void ThreadPool::run() {
  while (!q.empty()) {
    // cout << threadcount << '\r';
    if (threadcount > 0) {
      dispatch(std::move(q.front()));
      q.pop();
    }
  }

  for (auto &t : threads)
    t.join();

  threads.clear();
}

void ThreadPool::dispatch(function<void(void)> f) {
  threadcount--;
  threads.emplace_back([this, f]() {
    f();
    threadcount++;
  });
}
