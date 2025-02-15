#pragma once // Александр Сергеевич, не бейте

#include <curl/curl.h>
#include <string>

using namespace std;

class CurlWrapper {
public:
  CurlWrapper();
  ~CurlWrapper();

  string get_html(const string &url);

private:
  CURL *instance;
};
#include <sqlite3.h>
