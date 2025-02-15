#include "Task.hpp"
#include "TaskManager.hpp"

#include <thread>

namespace {
const std::vector<std::string> paths = {
    "./data/0.html",
    "./data/1.html",
    "./data/2.html",
    "./data/3.html",
};

constexpr long long run_count = 1024;
} // namespace

int main(void) {
  std::setlocale(LC_ALL, "RUSSIAN");
  TaskManager tm;

  std::thread t(&TaskManager::run, &tm);

  for (size_t i = 1; i < run_count + 1; ++i) {
    tm.push(Task(i, paths[i % paths.size()]));
  }

  tm.stop();
  t.join();

  std::cout << "PROCESSING FINISHED..." << std::endl;
  std::cout << "STATS:" << std::endl;

  auto tasks = tm.getFinished();

  long long total_time, total_q1, total_q2, total_q3, total_s1, total_s2,
      total_s3;

  total_q1 = total_q2 = total_q3 = total_s1 = total_s2 = total_s3 = 0;

  for (const auto &task : tasks) {
    total_q1 += task.times[0].count();
    total_s1 += task.times[1].count();
    total_q2 += task.times[2].count();
    total_s2 += task.times[3].count();
    total_q3 += task.times[4].count();
    total_s3 += task.times[5].count();
  }

  total_time = total_q1 + total_q2 + total_q3 + total_s1 + total_s2 + total_s3;

  std::cout << "Average task lifetime: " << total_time / run_count
            << "\tmicroseconds." << std::endl;
  std::cout << "Average queue 1 wait:  " << total_q1 / run_count
            << "\tmicroseconds." << std::endl;
  std::cout << "Average step 1 time:   " << total_s1 / run_count
            << "\tmicroseconds." << std::endl;
  std::cout << "Average queue 2 wait:  " << total_q2 / run_count
            << "\tmicroseconds." << std::endl;
  std::cout << "Average step 2 time:   " << total_s2 / run_count
            << "\tmicroseconds." << std::endl;
  std::cout << "Average queue 3 wait:  " << total_q3 / run_count
            << "\tmicroseconds." << std::endl;
  std::cout << "Average step 3 time:   " << total_s3 / run_count
            << "\tmicroseconds." << std::endl;

  return 0;
}