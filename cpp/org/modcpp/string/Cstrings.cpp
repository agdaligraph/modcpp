#include "org/modcpp/string/Cstrings.h"

namespace org::modcpp::string {

  bool Cstrings::isEqual(cstring lhs, cstring rhs) {
    return strcmp(lhs, rhs) == 0;
  }

  bool Cstrings::isNullOrEmpty(cstring str) {
    return str == 0 || *str == 0;
  }

  bool Cstrings::isWhiteSpace(cstring str) {
   return isNullOrEmpty(trimLeft(str));
  }

  int Cstrings::length(cstring str) {
    return strlen(str);
  }

  bool Cstrings::startsWith(cstring str, cstring prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
  }

  bool Cstrings::isPrefixEqual(cstring lhs, cstring rhs,
      int length) {
    return strncmp(lhs, rhs, length) == 0;
  }

  bool Cstrings::contains(cstring str, cstring pattern) {
    return strstr(str, pattern);
  }

  cstringIterator Cstrings::findFirst(cstring str, cstring pattern) {
    return strstr(str, pattern);
  }

  static inline void prepareLookupTable(char *table, const char * const setChars) {
    memset(table, 0, 256);
    for (const char *ptr = setChars; *ptr; ++ptr) {
      assert(*ptr > 0 && "String::prepareLookupTable() Only ASCII chars are allowed");
      table[*((uchar *) ptr)] = true;
    }
  }

  char *Cstrings::trimRight(char *str) {
    auto right = str + strlen(str) - 1;
    for (; right >= str && isspace(*right); --right) {}
    *(++right) = 0;
    return str;
  }

  char *Cstrings::trimRight(char *str, cstring trimChars) {
    char isTrimChar[256];
    prepareLookupTable(isTrimChar, trimChars);

    auto right = str + strlen(str) - 1;
    for (; right >= str && isTrimChar[*((uchar *) right)]; --right) {}
    *(++right) = 0;
    return str;
  }

  const char *Cstrings::trimLeft(const char *str) {
    while (*str && isspace(*str)) ++str;
    return str;
  }

  char *Cstrings::trim(char *str) {
    return trimRight((char *) trimLeft(str));
  }

  char *Cstrings::trimRightAt(char * const str, char * const pos) {
    assert(pos >= str);
    *pos = '\0';
    return str;
  }

  char *Cstrings::trim(char * const str, cstring trimChars) {
    char isTrimChar[256];
    prepareLookupTable(isTrimChar, trimChars);

    uchar *front = (uchar *) str;
    while (*front && isTrimChar[*front]) ++front;

    uchar *back = (uchar *) str + strlen(str) - 1;
    while(back >= front && isTrimChar[*back]) --back;
    *(++back) = 0;

    return (char *) front;
  }

  std::vector<char *> Cstrings::splitInto(char *str, char splitChar) {
    std::vector<char *> parts;
    parts.push_back(str);
    for (char *ptr = str; *ptr; ++ptr) {
      if (*ptr == splitChar) {
        *ptr = 0;
        parts.push_back(ptr + 1);
      }
    }
    return parts;
  }

  std::vector<char *> Cstrings::splitIntoKeepBlocks(char *str, char splitChar, cstring blockSep) {
    assert(strlen(blockSep) == 2);
    int blockDepth = 0;
    std::vector<char *> parts;
    parts.push_back(str);
    for (char *ptr = str; *ptr; ++ptr) {
      if (*ptr == *blockSep) {
        ++blockDepth;
      } else if (*ptr == blockSep[1]) {
        --blockDepth;
      } else if (blockDepth == 0 && *ptr == splitChar) {
        *ptr = 0;
        parts.push_back(ptr + 1);
      }
    }
    return parts;
  }

  char *Cstrings::replace(char *str, char a, char b) {
    assert(a);
    for (char *ptr = str; *ptr; ++ptr) {
      if (*ptr == a) *ptr = b;
    }
    return str;
  }

  const char *Cstrings::getFileName(cstring str) {
    const char *back = str + strlen(str) - 1;
    while(back >= str && *back != '/') --back;
    return ++back;
  }

  char *Cstrings::removeTrailingSlash(char *str) {
    char *lastChar = str + strlen(str) - 1;
    if (lastChar > str && *lastChar == '/') {
      *lastChar = '\0';
    }
    return str;
  }

  char *Cstrings::addTrailingSlash(char *str) {
    char *lastChar = str + strlen(str) - 1;
    if (*lastChar != '/') {
      *++lastChar = '/';
      *++lastChar = '\0';
    }
    return str;
  }

}
