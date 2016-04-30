#include "org/modcpp/hashing/KarpRabinHasher.h"

using org::modcpp::hashing::KarpRabinHasher;
using org::modcpp::string::String;

void test_constructor() {
  KarpRabinHasher kr;
}

void test_hash() {
  KarpRabinHasher kr;
  String str1 = "testString";
  uint32_t hashValue = kr.hash(str1);
  
  String str2 = "test";
  assert(kr.hash(str2 + "String") == hashValue);
}

int main() {
  test_constructor();
  test_hash();
  return 0;
}
