#include "Command.hh"
#include "DefaultPath.hh"
#include "Target.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

static struct Command_list : Command {
  Command_list() { register_command("list", this); }
  virtual const char *usage() override {
    return
        R"ss(    list
)ss";
  }
  virtual int execute(std::vector<std::string> args) override {
    std::string path = current_cache_dir;
    path.push_back('/');
    path.append("autosub.target");

    std::ifstream ifs(path, std::ios::binary);
    if (!ifs) {
      throw std::runtime_error("no target. (May update first)");
    }
    std::vector<std::unique_ptr<autosub::Target>> targets;
    while (ifs) {
      auto ptr = autosub::TargetFactory::make_from_stream(ifs);
      if (ptr) {
        targets.push_back(std::move(ptr));
      }
    }
    int i = 0;
    for (auto &ptr : targets) {
      std::cout << "    ";
      std::cout << "[" << i << "] Name: " << ptr->name << " Type: " << ptr->type
                << " Info: " << ptr->info << "\n";
      i++;
    }
    return 0;
  }
} single_instance;
