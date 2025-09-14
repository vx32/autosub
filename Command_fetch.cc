#include "Command.hh"
#include "autosub.hh"

#include <curl/curl.h>
#include <curl/easy.h>

#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>

struct EasyCurl {
  char *memory = nullptr;
  size_t size = 0;
  CURL *curl = nullptr;
  EasyCurl() {
    curl = curl_easy_init();
    if (!curl) {
      throw std::runtime_error("curl init");
    }
    memory = (char *)malloc(1);
    memory[0] = 0;
  }
  ~EasyCurl() {
    curl_easy_cleanup(curl);
    if (memory) {
      free(memory);
    }
  }
};
static size_t WriteMemoryCallback(char *contents, size_t size, size_t nmemb,
                                  void *void_ez) {
  EasyCurl &ec = *(EasyCurl *)(void_ez);
  size_t realsize = size * nmemb;
  if (ec.size + realsize + 1 > 1024 * 1024 * 2) {
    throw std::runtime_error("curl: large");
  }
  char *ptr = (char *)realloc(ec.memory, ec.size + realsize + 1);
  if (!ptr) {
    throw std::runtime_error("curl: realloc");
  }
  ec.memory = ptr;
  memcpy(ec.memory + ec.size, contents, realsize);
  ec.size += realsize;
  ec.memory[ec.size] = 0;
  return realsize;
}

std::string autosub::fetch(const std::string &url) {
  EasyCurl ec;
  curl_easy_setopt(ec.curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(ec.curl, CURLOPT_FOLLOWLOCATION, 1L);
  curl_easy_setopt(ec.curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
  curl_easy_setopt(ec.curl, CURLOPT_WRITEDATA, (void *)&ec);
  curl_easy_setopt(ec.curl, CURLOPT_USERAGENT, "autosub-agent/0.1");

  CURLcode res = curl_easy_perform(ec.curl);
  if (res != CURLcode::CURLE_OK) {
    throw std::runtime_error(std::string("curl perform: ") +
                             curl_easy_strerror(res));
  }
  long response_code;
  res = curl_easy_getinfo(ec.curl, CURLINFO_RESPONSE_CODE, &response_code);
  if (res != CURLcode::CURLE_OK) {
    throw std::runtime_error(std::string("curl getinfo response code: ") +
                             curl_easy_strerror(res));
  }
  if (response_code != 200) {
    throw std::runtime_error(std::string("response code: ") +
                             std::to_string(response_code));
  }
  return {ec.memory, ec.size};
}

static struct Command_fetch : Command {
  Command_fetch() { register_command("fetch", this); }
  virtual const char *usage() override {
    return
        R"ss(    -DEBUG- fetch <url> <output>
)ss";
  };
  virtual int execute(std::vector<std::string> args) override {
    if (args.size() < 3) {
      throw std::runtime_error("no args");
    }
    const std::string &url = args.at(1);
    const std::string &output = args.at(2);

    std::string data = autosub::fetch(url);

    if (output.size() == 1 && output[0] == '-') {
      std::cout << data;
    } else {
      std::ofstream ofs(output, std::ios::binary);
      if (ofs)
        ofs << data;
      else
        throw std::runtime_error("can not write file.");
    }
    return 0;
  }

  virtual ~Command_fetch() {}
} single_instance;
