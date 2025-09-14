#include "Command.hh"
#include "DefaultPath.hh"
#include "Replacer.hh"
#include "Target.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>
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

static struct Command_push : Command {
  Command_push() { register_command("push", this); }
  virtual const char *usage() override {
    return
        R"ss(    push <target>
)ss";
  }
  virtual int execute(std::vector<std::string> args) override {
    if (args.size() <= 1) {
      throw std::runtime_error("not args");
    }

    std::string targetpath = current_cache_dir;
    targetpath.push_back('/');
    targetpath.append("autosub.target");

    std::ifstream ifs(targetpath, std::ios::binary);
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

    std::unique_ptr<autosub::Target> tg;
    std::string select_target = args.at(1);
    if (is_number(select_target)) {
      int id = std::stoi(select_target);
      if (id < 0 || id >= targets.size())
        throw std::runtime_error("target out of length");
      tg = std::move(targets[id]);
    } else {
      throw std::runtime_error("not select target");
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

    std::ifstream itf(template_file, std::ios::binary);
    if (!itf)
      throw std::runtime_error("no templte file");
    Replacer rep;
    tg->set_replacer(rep);
    while (itf) {
      std::string line;
      std::getline(itf, line);
      rep << line;
      rep.put('\n');
    }

    std::string pushconfpath;
    pushconfpath = current_etc_dir;
    pushconfpath.push_back('/');
    pushconfpath.append("push.conf");

    std::ifstream ipf(pushconfpath, std::ios::binary);
    if (!ipf) {
      throw std::runtime_error("no push conf file");
    }
    std::string output_file;
    ipf >> output_file;

    std::ofstream of(output_file, std::ios::binary);
    of << rep.str();
    return 0;
  }
} single_instance;
