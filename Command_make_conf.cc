#include "Command.hh"
#include "DefaultPath.hh"
#include "Replacer.hh"
#include "Target.hh"

#include <fstream>
#include <iostream>
#include <vector>

static bool is_number(std::string_view sv) {
  for (char c : sv) {
    if (c >= '0' || c <= '9')
      continue;
    else
      return false;
  }
  return true;
}

static struct Command_make_conf : Command {
  Command_make_conf() { register_command("make_conf", this); }

  virtual const char *usage() override {
    return
        R"ss(    -DEBUG- make_conf <target> [template_file]
)ss";
  }
  virtual int execute(std::vector<std::string> args) override {
    if (args.size() <= 1) {
      throw std::runtime_error("not args");
    }

    std::string path = current_cache_dir;
    path.push_back('/');
    path.append("autosub.target");

    std::unique_ptr<autosub::Target> tg;
    {
      std::ifstream ifs(path, std::ios::binary);
      std::vector<std::unique_ptr<autosub::Target>> targets;
      while (ifs) {
        auto ptr = autosub::TargetFactory::make_from_stream(ifs);
        if (ptr) {
          targets.push_back(std::move(ptr));
        }
      }

      std::string select_target = args.at(1);
      if (is_number(select_target)) {
        int id = std::stoi(select_target);
        if (id < 0 || id >= targets.size())
          throw std::runtime_error("target out of length");
        tg = std::move(targets[id]);
      }
      targets.clear();
    }
    if (!tg)
      throw std::runtime_error("no target");

    std::string template_file;
    if (args.size() >= 3) {
      template_file = args.at(2);
    } else {
      template_file = current_etc_dir;
      template_file.push_back('/');
      template_file.append(tg->type);
      template_file.append(".conf");
    }
    std::ifstream ifs(template_file, std::ios::binary);
    if (!ifs)
      throw std::runtime_error("no templte file");
    Replacer rep;
    tg->set_replacer(rep);
    while (ifs) {
      std::string line;
      std::getline(ifs, line);
      rep << line;
      rep.put('\n');
    }
    std::cout << rep.str();
    return 0;
  }
} single_instance;
