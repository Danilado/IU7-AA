#include "curlwrap.hpp"
#include <format>

namespace {
size_t concatenate(void *data, size_t size, size_t nmemb, void *buf) {
  ((std::string *)buf)->append((char *)data, size * nmemb);
  return size * nmemb;
}
} // namespace

CurlWrapper::CurlWrapper() { instance = curl_easy_init(); }

CurlWrapper::~CurlWrapper() { curl_easy_cleanup(instance); }

string CurlWrapper::get_html(const string &url) {
  if (!instance)
    throw "curl failed to init";

  string buf;

  curl_easy_setopt(instance, CURLOPT_URL, url.c_str());
  curl_easy_setopt(instance, CURLOPT_ENCODING, "");
  curl_easy_setopt(instance, CURLOPT_WRITEFUNCTION, concatenate);
  curl_easy_setopt(instance, CURLOPT_WRITEDATA, &buf);

  CURLcode rc = curl_easy_perform(instance);

  if (rc != CURLE_OK) {
    throw format("curl failed: {}", curl_easy_strerror(rc));
  }

  return buf;
}
