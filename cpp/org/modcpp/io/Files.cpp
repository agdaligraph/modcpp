#include "org/modcpp/io/Files.h"

#include <cassert>
#include <cerrno>
#include <sys/stat.h>
#include <unistd.h>

#include "org/modcpp/string/String.h"
#include "org/modcpp/string/StringBuilder.h"

namespace org::modcpp::io {

  using org::modcpp::string::String;
  using org::modcpp::string::StringBuilder;

  bool Files::ensureDirectory(const String &directory, const String basePath) {
    StringBuilder builder(directory.length() + basePath.length() + 2);
    builder.put(basePath).put('/');

    String::Iterator left = directory.begin();
    String::Iterator right;
    while (left != directory.end()) {
      right = directory.findFirst(left + 1, "/");
      builder.putInterval(left, right);
      builder.getCaret()[0] = '\0';
      int result = mkdir(builder.begin(), 00777);
      if (result && errno != EEXIST) {
        return false;
      }
      left = right;
    }
    return true;
  }

  bool Files::exists(const String fileName) {
    struct stat buffer;
    String fileNameTerminated = String("") + fileName + String("", 1);   
    return (stat(fileNameTerminated.begin(), &buffer) == 0); 
  }

} // namespace
