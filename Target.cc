#include "Target.hh"
#include "Replacer.hh"
#include "StringUtils.hh"

#include <istream>
#include <map>
#include <ostream>

#include <iostream>

namespace autosub {
void Target::save(std::ostream &os) {
  os << "Target " << type << '\n';
  os << "\t" << escape_string(name) << '\n';
  os << "\t" << escape_string(info) << '\n';
  os << "\t" << escape_string(address) << '\n';
}
void Target::load(std::istream &is) {
  name = load_line_t_str_n(is);
  info = load_line_t_str_n(is);
  address = load_line_t_str_n(is);
}
void Target::set_replacer(Replacer &rep) {
  rep.add("type", type);
  rep.add("name", name);
  rep.add("info", info);
  rep.add("address", address);
}
Target::~Target() {}

std::unique_ptr<Target> TargetSupport::make_from_url(std::string_view url) {
  return nullptr;
}
std::unique_ptr<Target> TargetSupport::make_from_stream(std::istream &is) {
  return nullptr;
}

std::map<std::string, TargetSupport *> g_all_support_url;
std::map<std::string, TargetSupport *> g_all_support_tp;
bool TargetSupport::register_url(std::string header, TargetSupport *sup) {
  auto p = g_all_support_url.insert({header, sup});
  return p.second;
}
bool TargetSupport::register_type(std::string type, TargetSupport *sup) {
  auto p = g_all_support_tp.insert({type, sup});
  return p.second;
}

std::unique_ptr<Target> TargetFactory::make_from_url(std::string_view url) {
  auto pos = url.find(':');
  if (pos + 2 >= url.size())
    return nullptr;
  if (url[pos + 1] != '/' || url[pos + 2] != '/')
    return nullptr;
  std::string_view header = url.substr(0, pos + 3);
  auto it = g_all_support_url.find(std::string{header});
  if (it == g_all_support_url.end())
    return nullptr;
  return it->second->make_from_url(url);
}

std::unique_ptr<Target> TargetFactory::make_from_stream(std::istream &is) {
  std::string_view target_tag = "Target ";
  std::string target_line;
  std::getline(is, target_line);
  std::string_view sv(target_line);
  if (!shift_tag(sv, target_tag))
    return nullptr;
  if (sv.empty())
    return nullptr;
  auto it = g_all_support_tp.find(std::string{sv});
  if (it == g_all_support_tp.end()) {
    return nullptr;
  }
  return it->second->make_from_stream(is);
}
} // namespace autosub
