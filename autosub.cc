#include "autosub.hh"

#include "Command.hh"
#include "DefaultPath.hh"

#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

std::string default_etc_dir = "/etc/autosub";
std::string default_cache_dir = "/tmp";
std::string current_etc_dir = default_etc_dir;
std::string current_cache_dir = default_cache_dir;

int run(int argc, char *argv[]) {
  if (argc <= 1) {
    std::cout << "Usage: autosub <command> ... \nAll commands:\n";
    std::cout << Command::all_usage();
    return 0;
  }
  std::vector<std::string> args;
  int i = 1;
  for (; i < argc; i++) {
    std::string_view sv{argv[i]};
    if (sv.size() >= 2) {
      if (sv[0] == '-' && sv[1] == 'D') {
        sv.remove_prefix(2);
        current_etc_dir = sv;
        continue;
      } else if (sv[0] == '-' && sv[1] == 'C') {
        sv.remove_prefix(2);
        current_cache_dir = sv;
        continue;
      }
      break;
    }
    break;
  }
  for (; i < argc; i++) {
    args.emplace_back(argv[i]);
  }

  Command *cmd = Command::get_command(args[0]);
  if (cmd) {
    return cmd->execute(args);
  } else {
    throw std::runtime_error("no command");
    return 1;
  }
}

int main(int argc, char *argv[]) {
  try {
    return run(argc, argv);
  } catch (std::exception &e) {
    std::cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}
