#pragma once

#include <memory>
#include <ostream>
#include <string>

struct Replacer;

namespace autosub {
struct Target {
  std::string type;
  std::string name;
  std::string info;
  std::string address;
  virtual void save(std::ostream &os);
  virtual void load(std::istream &is);
  virtual void set_replacer(Replacer &rep);
  virtual ~Target();
};

struct TargetFactory {
  static std::unique_ptr<Target> make_from_url(std::string_view url);
  static std::unique_ptr<Target> make_from_stream(std::istream &is);
};

struct TargetSupport {
private:
  friend class TargetFactory;
  virtual std::unique_ptr<Target> make_from_url(std::string_view url);
  virtual std::unique_ptr<Target> make_from_stream(std::istream &is);

protected:
  static bool register_url(std::string header, TargetSupport *sup);
  static bool register_type(std::string type, TargetSupport *sup);
};
} // namespace autosub
