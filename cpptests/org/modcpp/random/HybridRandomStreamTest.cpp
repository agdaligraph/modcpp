#include "org/modcpp/random/HybridRandomStream.h"

#include <iostream>
#include <iostream>
#include <unordered_map>
#include <string>

using std::cout;
using std::endl;
using org::modcpp::random::HybridRandomStream;

void test_constructor() {
  HybridRandomStream prng;
  assert(prng.get<uint32_t>());
}

int main() {
  test_constructor();
  return 0;
}
