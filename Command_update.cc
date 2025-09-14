#include "Command.hh"
#include "DefaultPath.hh"
#include "Target.hh"
#include "autosub.hh"

#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>

void update_one(const std::string &line, std::ostream &os) {
  std::string_view sv{line};

  auto src_pos = sv.find('-');
  std::string_view src = sv.substr(0, src_pos);
  sv.remove_prefix(src_pos + 1);

  auto code_pos = sv.find('-');
  std::string_view code = sv.substr(0, code_pos);
  sv.remove_prefix(code_pos + 1);
  auto addr_pos = sv.find(' ');
  std::string_view addr = sv.substr(0, addr_pos);
  sv.remove_prefix(addr_pos + 1);
  std::string_view url = sv;

  std::cout << "Updating: " << src << "-" << code << "-" << addr << "  " << url
            << std::endl;
  std::string urls(url);
  std::string rdata = autosub::fetch(urls);

  auto tcode = autosub::get_decode_type(std::string{code});
  std::string data = autosub::decode(tcode, rdata);
  rdata.clear();

  std::vector<std::unique_ptr<autosub::Target>> v;
  std::istringstream iss{data};
  auto et = autosub::get_extract_type(std::string{addr});
  while (iss) {
    std::string line;
    std::getline(iss, line);
    auto p = autosub::extract(et, line);
    if (p)
      p->save(os);
  }
}

static struct Command_update : Command {
  Command_update() { register_command("update", this); }
  virtual const char *usage() override {
    return
        R"ss(    update [feeds.conf]
)ss";
  }
  virtual int execute(std::vector<std::string> args) override {
    std::string feedpath;
    if (args.size() >= 2) {
      feedpath = args.at(1);
    } else {
      feedpath = current_etc_dir;
      feedpath.push_back('/');
      feedpath.append("feeds.conf");
    }

    std::ifstream ifs(feedpath, std::ios::binary);
    if (!ifs) {
      throw std::runtime_error("can not read feeds.conf");
    }

    std::ostringstream oss;
    std::string line;
    while (ifs) {
      std::getline(ifs, line);
      if (line.empty())
        continue;
      if (line.at(0) == '#')
        continue;
      update_one(line, oss);
    }

    std::string path = current_cache_dir;
    path.push_back('/');
    path.append("autosub.target");

    std::ofstream ofs(path, std::ios::binary);
    ofs << oss.str();

    return 0;
  }
} single_instance;
