#include "org/modcpp/logging/Console.h"

using org::modcpp::logging::Console;

void test_error() {
  Console::error("TEST ERROR");
  org::modcpp::logging::Console::error("TEST ERROR");
}

int main() {
  test_error();
  return 0;
}
