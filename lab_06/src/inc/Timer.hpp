#pragma once

#include <chrono>
#include <functional>
#include <iostream>
#include <utility>

template <class> struct ExeTime;

// Execution time decorator
template <class R, class... Args> struct ExeTime<R(Args...)> {
public:
  ExeTime(std::function<R(Args...)> func) : f_(func) {}

  R operator()(Args... args) {
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::chrono::duration<double> elasped;

    start = std::chrono::system_clock::now();
    R result = f_(args...);
    end = std::chrono::system_clock::now();
    elasped = end - start;
    std::cout << elasped.count() << " seconds" << std::endl;

    return result;
  }

private:
  std::function<R(Args...)> f_;
};

template <class R, class... Args>
ExeTime<R(Args...)> make_decorator(R (*f)(Args...)) {
  return ExeTime<R(Args...)>(std::function<R(Args...)>(f));
}

typedef std::chrono::high_resolution_clock::time_point TimeVar;

#define duration(a)                                                            \
  std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

template <typename F, typename... Args>
double funcTime(F func, Args &&...args) {
  TimeVar t1 = timeNow();
  func(std::forward<Args>(args)...);
  return duration(timeNow() - t1);
}