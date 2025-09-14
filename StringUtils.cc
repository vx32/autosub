#include "StringUtils.hh"

int hex_to_int(char hex) {
  if (hex >= '0' && hex <= '9') {
    return hex - '0';
  } else if (hex >= 'a' && hex <= 'f') {
    return hex - 'a' + 10;
  } else if (hex >= 'A' && hex <= 'F') {
    return hex - 'A' + 10;
  }
  return -1;
}

std::string unescape_url(std::string_view input) {
  size_t length = input.size();
  std::string r;
  for (size_t i = 0; i < length;) {
    if (input[i] == '%' && i < length - 2) {
      int high = hex_to_int(input[i + 1]) * 16;
      int low = hex_to_int(input[i + 2]);
      if (high >= 0 && low >= 0) {
        r.push_back((char)(high + low));
        i += 3;
      }
    } else {
      r.push_back(input[i]);
      i += 1;
    }
  }
  return r;
}

std::string_view shift_until(std::string_view &v, char x, bool eat) {
  auto pos = v.find(x);
  if (pos == std::string_view::npos)
    return {};
  auto r = v.substr(0, pos);
  v.remove_prefix(pos);
  if (eat)
    v.remove_prefix(1);
  return r;
}
bool shift_tag(std::string_view &v, std::string_view s) {
  if (v.compare(0, s.size(), s) != 0) {
    return false;
  }
  v.remove_prefix(s.size());
  return true;
}
std::string escape_string(std::string_view s) {
  std::string r;
  r.reserve(s.size() + 2);
  for (auto c : s) {
    if (c == '\n') {
      r.push_back('\\');
      r.push_back('n');
    } else if (c == '\\') {
      r.push_back('\\');
      r.push_back('\\');
    } else if (c == '{') {
      r.push_back('\\');
      r.push_back('F');
    } else if (c == '}') {
      r.push_back('\\');
      r.push_back('B');
    }
    r.push_back(c);
  }
  return r;
}
std::string unescape_string(std::string_view s) {
  std::string r;
  r.reserve(s.size());
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '\\' && i + 1 < s.size()) {
      if (s[i + 1] == 'n')
        r.push_back('\n');
      else if (s[i + 1] == '\\')
        r.push_back('\\');
      else if (s[i + 1] == 'F')
        r.push_back('{');
      else if (s[i + 1] == 'B')
        r.push_back('}');
      else {
      }
      i++;
    } else {
      r.push_back(s[i]);
    }
  }
  return r;
}
std::string load_line_t_str_n(std::istream &is) {
  std::string s;
  std::getline(is, s);
  std::string r = unescape_string(s);
  if (r.size() >= 1 && r[0] == '\t')
    return r.substr(1);
  else
    return {};
}
