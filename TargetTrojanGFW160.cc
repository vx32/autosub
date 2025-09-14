#include "Replacer.hh"
#include "StringUtils.hh"
#include "Target.hh"

#include <istream>
#include <ostream>

namespace autosub {

struct TargetTrojanGFW160 : Target {
  std::string host;
  short port;
  std::string password;
  std::string sni;
  virtual void save(std::ostream &os) override {
    Target::save(os);
    os << "\t" << escape_string(host) << '\n';
    os << "\t" << escape_string(std::to_string(port)) << '\n';
    os << "\t" << escape_string(password) << '\n';
    os << "\t" << escape_string(sni) << '\n';
  }
  virtual void load(std::istream &is) override {
    Target::load(is);
    host = load_line_t_str_n(is);
    std::string port_s = load_line_t_str_n(is);
    port = std::stoi(port_s);
    password = load_line_t_str_n(is);
    sni = load_line_t_str_n(is);
    type = "TrojanGFW160";
  }
  virtual void set_replacer(Replacer &rep) override {
    Target::set_replacer(rep);
    rep.add("host", host);
    rep.add("port", std::to_string(port));
    rep.add("password", password);
    rep.add("sni", sni);
  }
  ~TargetTrojanGFW160() {}
};

struct TargetSupportTrojanGFW160 : TargetSupport {
  TargetSupportTrojanGFW160() {
    TargetSupport::register_url("trojan://", this);
    TargetSupport::register_type("TrojanGFW160", this);
  }
  virtual std::unique_ptr<Target> make_from_url(std::string_view url) override {
    if (!shift_tag(url, "trojan://")) {
      return nullptr;
    }
    std::unique_ptr<TargetTrojanGFW160> p =
        std::make_unique<TargetTrojanGFW160>();
    TargetTrojanGFW160 &t = *p;
    t.type = "TrojanGFW160";
    t.password = shift_until(url, '@');
    t.host = shift_until(url, ':');
    t.port = std::stoi(std::string{shift_until(url, '?')});
    if (!shift_tag(url, "peer=")) {
      return nullptr;
    }
    t.name = unescape_url(shift_until(url, '&'));
    if (!shift_tag(url, "sni=")) {
      return nullptr;
    }
    t.sni = shift_until(url, '#');
    t.info = unescape_url(url);
    t.address = t.host + ":" + std::to_string(t.port);
    return p;
  }
  virtual std::unique_ptr<Target> make_from_stream(std::istream &is) override {
    std::unique_ptr<TargetTrojanGFW160> p =
        std::make_unique<TargetTrojanGFW160>();
    p->load(is);
    return p;
  }
} single_instance;

} // namespace autosub
