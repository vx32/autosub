#pragma one

#include <memory>
#include <string>

namespace autosub {

std::string fetch(const std::string &url);

enum class decode_type { UNK, NONE, BASE64 };
decode_type get_decode_type(std::string code);
std::string decode(decode_type t, std::string data);

struct Target;
enum class extract_type { UNK, URL };
extract_type get_extract_type(std::string ext);
std::unique_ptr<Target> extract(extract_type t, std::string data);

} // namespace autosub
