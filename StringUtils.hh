#pragma once

#include <istream>
#include <string>
#include <string_view>

int hex_to_int(char hex);
std::string unescape_url(std::string_view input);
std::string_view shift_until(std::string_view &v, char x, bool eat = true);
bool shift_tag(std::string_view &v, std::string_view s);
std::string escape_string(std::string_view s);
std::string unescape_string(std::string_view s);

// \tstr\n
std::string load_line_t_str_n(std::istream &is);
