#include "org/modcpp/sets/Sets.h"

#include <cassert>

using org::modcpp::sets::Sets;

void test_static() {
  static_assert(Sets::isIn(1, {1, 2, 3, 4, 5}));
  static_assert(not Sets::isIn(1, {2, 3, 4}));
}

void test_runtime() {
  int a = 9;
  assert(Sets::isIn(8, {a, a + 1, a - 1}));
}

int main() {
  test_static();
  test_runtime();
  return 0;
}
