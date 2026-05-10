#include "Command.hh"

#include <map>
#include <memory>
#include <sstream>

std::unique_ptr<std::map<std::string, Command *>> g_all_commands = nullptr;
static void init_check() {
  if (!g_all_commands) {
    g_all_commands = std::make_unique<std::map<std::string, Command *>>();
  }
}

bool Command::register_command(std::string name, Command *cmd) {
  init_check();
  auto p = g_all_commands->insert({name, cmd});
  return p.second;
}
Command *Command::get_command(std::string name) {
  init_check();
  auto it = g_all_commands->find(name);
  if (it != g_all_commands->end()) {
    return it->second;
  }
  return nullptr;
}
std::string Command::all_usage() {
  init_check();
  std::stringstream iss;
  for (auto it = g_all_commands->begin(); it != g_all_commands->end(); it++) {
    iss << it->second->usage();
  }
  return iss.str();
}
