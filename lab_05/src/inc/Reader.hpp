#pragma once

#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <sstream>
#include <string>

class Reader {
public:
  static std::string read(const std::string &filename) {
    std::ifstream file(filename);
    if (!file) {
      throw std::runtime_error("Не удалось открыть файл");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    return buffer.str();
  }
};