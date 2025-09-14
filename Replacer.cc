#include "Replacer.hh"

Replacer &Replacer::operator<<(std::string str) {
  for (int i = 0; i < str.size(); i++) {
    if (str[i] == '%' && i + 3 < str.size() && str[i + 1] == '{') {
      int j = i + 2;
      for (; j < str.size(); j++) {
        if (str[j] == '}')
          break;
      }
      if (j < str.size()) {
        std::string name = str.substr(i + 2, j - (i + 2));
        auto it = m_map.find(name);
        if (it != m_map.end()) {
          m_ss << it->second;
          i = j;
          continue;
        }
      }
    }
    m_ss << str[i];
  }
  return *this;
}
