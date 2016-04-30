#ifndef org_modcpp_string_Cstrings
#define org_modcpp_string_Cstrings

#include <vector>

#include "org/modcpp/base/base.h"

namespace org::modcpp::string {

  class Cstrings {
   public:
    static constexpr cstring EMPTY_CSTRING = "";

    static bool isEqual(cstring lhs, cstring rhs);
    static bool isNullOrEmpty(cstring);
    static bool isWhiteSpace(cstring);

    static int length(cstring);

    static bool startsWith(cstring str, const cstring prefix);
    static bool isPrefixEqual(cstring lhs, cstring rhs, int length);
    static bool contains(cstring text, cstring pattern);
    static const char *findFirst(cstring text, cstring pattern);

    static char *trimRight(char *);
    static char *trimRight(char *, cstring);
    static const char *trimLeft(const char *);
    static char *trim(char *);
    static char *trimRightAt(char * const, char * const);
    static char *trim(char *, const char * const);

    static std::vector<char *> splitInto(char *str, char splitChar);
    static std::vector<char *> splitIntoKeepBlocks(char *str, char splitChar, const char *blockSep);

    static char *replace(char *str, char a, char b);

    static const char *getFileName(const char *str);
    static char *removeTrailingSlash(char *str);
    static char *addTrailingSlash(char *str);
  };

}

#endif
