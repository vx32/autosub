#include "Command.hh"

#include <map>
#include <sstream>

std::map<std::string, Command *> g_all_commands;

bool Command::register_command(std::string name, Command *cmd) {
  auto p = g_all_commands.insert({name, cmd});
  return p.second;
}
Command *Command::get_command(std::string name) {
  auto it = g_all_commands.find(name);
  if (it != g_all_commands.end()) {
    return it->second;
  }
  return nullptr;
}
std::string Command::all_usage() {
  std::stringstream iss;
  for (auto it = g_all_commands.begin(); it != g_all_commands.end(); it++) {
    iss << it->second->usage();
  }
  return iss.str();
}
