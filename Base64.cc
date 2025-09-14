#include "Base64.hh"

char bmap[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
               'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
               'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
               'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
               '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/'};
std::string Base64::encode(std::string_view raw) {
  char counts = 0;
  char buffer[3];
  std::string bc;
  bc.resize(raw.size() * 4 / 3 + 4);
  int i = 0, c = 0;

  for (i = 0; i < raw.size(); i++) {
    buffer[counts++] = raw[i];
    if (counts == 3) {
      bc[c++] = bmap[buffer[0] >> 2];
      bc[c++] = bmap[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
      bc[c++] = bmap[((buffer[1] & 0x0f) << 2) + (buffer[2] >> 6)];
      bc[c++] = bmap[buffer[2] & 0x3f];
      counts = 0;
    }
  }

  if (counts > 0) {
    bc[c++] = bmap[buffer[0] >> 2];
    if (counts == 1) {
      bc[c++] = bmap[(buffer[0] & 0x03) << 4];
      bc[c++] = '=';
    } else {
      bc[c++] = bmap[((buffer[0] & 0x03) << 4) + (buffer[1] >> 4)];
      bc[c++] = bmap[(buffer[1] & 0x0f) << 2];
    }
    bc[c++] = '=';
  }
  bc.resize(c);
  return bc;
}
std::string Base64::decode(std::string_view b64) {

  char counts = 0;
  char buffer[4];
  std::string ra;
  ra.resize(b64.size() * 3 / 4);
  int i = 0, p = 0;

  for (i = 0; i < b64.size(); i++) {
    char k;
    for (k = 0; k < 64 && bmap[k] != b64[i]; k++)
      ;
    buffer[counts++] = k;
    if (counts == 4) {
      ra[p++] = (buffer[0] << 2) + (buffer[1] >> 4);
      if (buffer[2] != 64)
        ra[p++] = (buffer[1] << 4) + (buffer[2] >> 2);
      if (buffer[3] != 64)
        ra[p++] = (buffer[2] << 6) + buffer[3];
      counts = 0;
    }
  }

  ra.resize(p);
  return ra;
}
