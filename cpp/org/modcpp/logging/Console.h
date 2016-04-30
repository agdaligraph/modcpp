#ifndef org_modcpp_logging_Console
#define org_modcpp_logging_Console

namespace org::modcpp::logging {

  class Console {
   public:
    static void error(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
    static void warning(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
    static void info(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
    static void fatal(const char *fmt, ...) __attribute__((format(printf, 1, 2)));
    static int test();

   private:
    Console() {};
  };

}

#endif
