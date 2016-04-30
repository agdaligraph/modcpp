#include "org/modcpp/string/Encoding.h"

#include "org/modcpp/string/String.h"
#include "org/modcpp/string/StringBuilder.h"

namespace org::modcpp::string {

  static bool IsTextual[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  };

  /*static bool IsAlpabetic[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
  };*/

  static unsigned short Iso8859_9ToUtf8[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 43970, 0, 0, 0, 0,
    0, 0, 45762, 46018, 0, 0, 0, 0, 0, 0, 0, 48066, 0, 0, 0, 0,
    0, 0, 33475, 0, 0, 0, 0, 34755, 0, 35267, 0, 0, 0, 0, 36547, 0,
    40644, 0, 0, 0, 0, 0, 38595, 0, 0, 0, 0, 39875, 40131, 45252, 40645, 0,
    0, 0, 41667, 0, 0, 0, 0, 42947, 0, 43459, 0, 0, 0, 0, 44739, 0,
    40900, 0, 0, 0, 0, 0, 46787, 0, 0, 0, 0, 48067, 48323, 45508, 40901, 0,
  };

  String Encoding::fromIso8859_9(const String text) {
    if (text.isEmpty()) {
      return "";
    }

    int length = text.length();
    for (const char c : text) {
      if (c <= 0)
        ++length;
    }

    StringBuilder builder(length + 1);
    auto it = builder.begin();
    for (const char c : text) {
      uchar uc = *((uchar *) &c);
      if (uc > 127) {
        *((unsigned short *) it) = Iso8859_9ToUtf8[uc - 128];
        it += 2;
      } else {
        *it = c;
        ++it;
      }
    }
    builder.setCaret(it);
    return builder.build();
  }

  bool Encoding::isAlphabetic(const String text) {
    for (auto c : text) {
      if (c <= 0 && !isalpha(c))
        return false;
    }
    return true;
  }

  bool Encoding::isTextual(const String text) {
    for (auto *it = text.begin(); it != text.end();) {
      if (*it < 0) {
        it += 2;
      } else {
        if (!IsTextual[*(uchar *) it])
          return false;
        ++it;
      }
    }
    return true;
  }

  static constexpr utf8Char utf8Literal(const char * const str) {
    return (((utf8Char) str[1] << 8) + ((utf8Char) str[0] & 255)) & 65535;
  }

  char Encoding::anglifyChar(utf8Char utf8char) {
    switch (utf8char) {
      case utf8Literal("Ş"):
      case utf8Literal("ş"):
        return 's';
      case 45252:
      case 45508:
      case 44739:
      case 36547:
        return 'i';
      case 40131:
      case 48323:
      case 39875:
      case 48067:
        return 'u';
      case utf8Literal("Ö"):
      case utf8Literal("ö"):
        return 'o';
      case utf8Literal("Ğ"):
      case utf8Literal("ğ"):
        return 'g';
      case utf8Literal("Â"):
      case utf8Literal("â"):
        return 'a';
      case utf8Literal("Ç"):
      case utf8Literal("ç"):
        return 'c';
      default:
        return (char) utf8char;
    }
  }

}
