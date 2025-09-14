#include "Command.hh"
#include "Target.hh"
#include "autosub.hh"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>

namespace autosub {
extract_type get_extract_type(std::string ext) {
  if (ext == "url") {
    return extract_type::URL;
  }
  return extract_type::UNK;
}
std::unique_ptr<Target> extract(extract_type t, std::string data) {
  switch (t) {
  case extract_type::URL: {
    return TargetFactory::make_from_url(data);
    break;
  }
  default:
    throw std::runtime_error("unspported extract type");
  }
  return nullptr;
}
} // namespace autosub

static struct Command_extract : Command {
  Command_extract() { register_command("extract", this); }
  virtual const char *usage() override {
    return
        R"ss(    -DEBUG- extract <extract_type> <input> <output>
)ss";
  }
  virtual int execute(std::vector<std::string> args) override {
    if (args.size() < 4) {
      throw std::runtime_error("no args");
    }
    const std::string &ext = args.at(1);
    const std::string &input = args.at(2);
    const std::string &output = args.at(3);

    std::unique_ptr<std::ifstream> fis;
    std::istream *is;
    std::string data;
    if (input.size() == 1 && input[0] == '-') {
      is = &std::cin;
    } else {
      fis = std::make_unique<std::ifstream>(input, std::ios::binary);
      is = fis.get();
    }

    std::stringstream iss;
    while (*is) {
      std::string line;
      std::getline(*is, line);
      if (line.empty())
        continue;
      auto t = autosub::get_extract_type(ext);
      auto ptr = autosub::extract(t, line);
      if (ptr) {
        ptr->save(iss);
      }
    }

    if (output.size() == 1 && output[0] == '-') {
      std::cout << iss.str();
    } else {
      std::ofstream ofs(output, std::ios::binary);
      ofs << iss.str();
    }
    return 0;
  }

} single_instance;
