#include "org/modcpp/logging/Console.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace org::modcpp::logging {

  void Console::error(const char *fmt, ...) {
    va_list args;
    printf("%s ", "\x1B[31m[ERROR]\x1B[0m");
    va_start(args,fmt);
    vprintf(fmt, args);
    puts("");
    va_end(args);
  }

  void Console::warning(const char *fmt, ...) {
    va_list args;
    printf("%s ", "\x1B[33m[WARNING]\x1B[0m");
    va_start(args,fmt);
    vprintf(fmt, args);
    puts("");
    va_end(args);
  }

  void Console::info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("%s ", "\x1B[32m[INFO]\x1B[0m");
    vprintf(fmt, args);
    puts("");
    va_end(args);
  }

  void Console::fatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("%s ", "\x1B[33m[FATAL]\x1B[0m");
    vprintf(fmt, args);
    puts("");
    va_end(args);
  }

}
