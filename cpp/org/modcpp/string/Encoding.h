#ifndef org_modcpp_string_Encoding
#define org_modcpp_string_Encoding

#include "org/modcpp/base/base.h"

#include "org/modcpp/string/String.h"

namespace org::modcpp::string {

  class Encoding {
   public:
    static char anglifyChar(utf8Char utf8Char); // May go to a lower level class.
    static bool isAlphabetic(const String text);
    static bool isTextual(const String text);
    static String fromIso8859_9(const String text);

   public:
    /**
     * TODO(saglam): Handle length 3 and 4 utf-8 encoded code points as well.
     */
    static inline utf8Char removeFirstChar(String &text) {
      assert(text.length() > 0);
      auto it = text.begin();
      if (*it > 0) {
        text.trimLeft(1);
        return (*it) & 127;
      } else {
        text.trimLeft(2);
        return *((unsigned short *) it) & 65535;
      }
    }

   private:
    Encoding();

  };

}

#endif
