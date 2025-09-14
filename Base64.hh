#pragma once

#include <string>
#include <string_view>

struct Base64 {
  static std::string encode(std::string_view raw);
  static std::string decode(std::string_view b64);
};
