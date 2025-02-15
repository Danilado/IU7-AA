#include "curlwrap.hpp"
#include "threadpool.hpp"
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <vector>

using namespace std;

namespace {
const vector<string> links = {
    "http://chiefs.kz/28-blyuda_iz_kabachkov__vkusnye_i_prostye_recepty_s_foto",
    "http://chiefs.kz/27-kulinarnye_recepty_xolodnye_deserty",
    "http://chiefs.kz/26-spicy_morroccan_prawns_with_cherry_tomatoes",
    "http://chiefs.kz/25-lepeshki_k_shashlyku_s_zelenyu_i_tvorogom",
};
}

int main(void) {
  cout.setf(ios::unitbuf);
  int threadcount = 0;
  cin >> threadcount;

  ThreadPool tp(threadcount);

  int entries;
  cin >> entries;

  cout << "Running " << entries << " pulls on " << threadcount << " threads"
       << endl;

  filesystem::create_directory("./out");

  std::chrono::duration<double, std::milli> avg;

  const long tests = 5;

  for (const auto &_ : views::iota(0, tests)) {
    cout << "Iteration " << _ << " // " << double(_) / tests * 100 << "%\r";
    for (const auto &i : views::iota(0, entries)) {
      tp.push_to_queue(std::move([i]() {
        CurlWrapper curl;
        try {
          string buf = curl.get_html(links[i % links.size()]);

          ofstream f(format("./out/{}.html", i));
          f << buf;
        } catch (const std::string &e) {
          cout << i << "Failed with error " << e << endl;
        }
      }));
    }

    auto start = std::chrono::high_resolution_clock::now();

    tp.run();

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duration = end - start;
    avg += duration / double(tests);
  }

  cout << "For " << threadcount << " threads: " << avg << " milliseconds";

  return 0;
}