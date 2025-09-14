#pragma once

#include <string>
#include <vector>

struct Command {
  virtual int execute(std::vector<std::string> args) { return -1; }
  virtual const char *usage() { return ""; };

  static std::string all_usage();
  static Command *get_command(std::string name);

protected:
  static bool register_command(std::string name, Command *cmd);
};
