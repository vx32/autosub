#pragma once

#include <map>
#include <sstream>
#include <string>

struct Replacer {
  void add(std::string name, std::string value) { m_map.insert({name, value}); }
  Replacer &operator<<(std::string str);
  void put(char x) { m_ss << x; }
  std::string str() { return m_ss.str(); }

private:
  std::stringstream m_ss;
  std::map<std::string, std::string> m_map;
};
