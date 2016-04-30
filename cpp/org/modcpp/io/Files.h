#ifndef org_modcpp_io_Files
#define org_modcpp_io_Files

#include "org/modcpp/string/String.h"

using org::modcpp::string::String;

namespace org::modcpp::io {

  class Files {
   private:
    Files() {};

   public:
    static bool ensureDirectory(const String &directory, const String basePath = "");
    static bool exists(const String fileName);
  };

}

#endif
