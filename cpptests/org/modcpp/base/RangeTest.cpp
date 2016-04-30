#include "org/modcpp/base/Range.h"

#include "org/modcpp/base/base.h"

using org::modcpp::base::Range;

void test_constructor() {
  int count = 0;
  for (char c : Range::inclusive('a', 'z')) {
    count += c - 'a';
  }
  assert(count == 26 * (26 - 1) / 2);
}

int main() {
  test_constructor();
  return 0;
}
