#include "Base64.hh"
#include "Command.hh"
#include "autosub.hh"

#include <fstream>
#include <iostream>
#include <stdexcept>

namespace autosub {
decode_type get_decode_type(std::string code) {
  decode_type t = decode_type::UNK;
  if (code == "base64") {
    t = decode_type::BASE64;
  }
  return t;
}
std::string decode(decode_type t, std::string data) {
  switch (t) {
  case decode_type::BASE64:
    return Base64::decode(data);
  default:
    throw std::runtime_error("unspported decode");
  }
}
} // namespace autosub

static struct Command_decode : Command {
  Command_decode() { register_command("decode", this); }
  virtual const char *usage() override {
    return
        R"ss(    -DEBUG- decode <code_type> <input> <output>
)ss";
  }
  virtual int execute(std::vector<std::string> args) override {
    if (args.size() < 4) {
      throw std::runtime_error("no args");
    }
    const std::string &code = args.at(1);
    const std::string &input = args.at(2);
    const std::string &output = args.at(3);

    std::string data;
    if (input.size() == 1 && input[0] == '-') {
      while (std::cin) {
        std::cin >> data;
      }
    } else {
      std::ifstream is(input, std::ios::binary);
      while (is) {
        is >> data;
      }
    }

    auto t = autosub::get_decode_type(code);
    std::string addr = autosub::decode(t, data);

    if (output.size() == 1 && output[0] == '-') {
      std::cout.write(addr.c_str(), addr.size());
    } else {
      std::ofstream ofs(output, std::ios::binary);
      ofs.write(addr.c_str(), addr.size());
    }
    return 0;
  }
} single_instance;
