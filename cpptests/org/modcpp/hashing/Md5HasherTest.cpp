#include "org/modcpp/hashing/Md5Hasher.h"

using namespace org::modcpp::hashing;

void test_constructor() {
  Md5Hasher hasher("The quick brown fox jumps over the lazy dog");
  assert(String("9e107d9d372bb6826bd81d3542a419d6") == hasher.hash());

  Md5Hasher hasher2("The quick brown fox jumps over the lazy dog.");
  assert(String("e4d909c290d0fb1ca068ffaddf22cbd0") == hasher2.hash());

  Md5Hasher hasher3("org::modcpp::hashing::Md5Hasher");
  assert(String("ce733591a45e98709bc05fdbd98f1454") == hasher3.hash());
}

void test_hash() {
  Md5Hasher md5;

  md5.put("org::modcpp");
  md5.put("::");
  md5.put("hashing::Md5Hasher");

  assert(String("ce733591a45e98709bc05fdbd98f1454") == md5.hash());

  Md5Hasher hasher;
  hasher.put("The quick brown fox");
  hasher.put(" jumps over the lazy dog");

  assert(String("9e107d9d372bb6826bd81d3542a419d6") == hasher.hash());
}

void test_put() {
  Md5Hasher md5;

  md5.put("org::modcpp");
  md5.put("::");
  md5.put(String("hashing::Md5Hasher"));

  assert(String("ce733591a45e98709bc05fdbd98f1454") == md5.hash());
}

int main() {
  test_constructor();
  test_hash();
  test_put();
  return 0;
}
