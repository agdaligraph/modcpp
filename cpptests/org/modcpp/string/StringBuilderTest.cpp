#include "org/modcpp/string/StringBuilder.h"

#include <assert.h>
#include <string.h>
#include <iostream>

#include "org/modcpp/string/String.h"
#include "org/modcpp/logging/Console.h"

using org::modcpp::string::String;
using org::modcpp::string::StringBuilder;
using std::cout;
using std::endl;

void test_putAt() {
  StringBuilder builder(100);
  assert(builder.put(builder.begin(), "abc\0").build() == "abc");

  StringBuilder b2(4);
  assert(b2.put(b2.begin(), "abcd").build() == "abcd");

  StringBuilder b3(1);
  assert(b3.put(b3.begin(), "a").build() == "a");
}

void test_put() {
  StringBuilder builder(10);
  builder.put('a').put('m').put('a');
  assert(builder.build()== "ama");

  StringBuilder b2(10);
  assert(b2.put("zczbz").put('?').replace('z', 'a').build() == "acaba?");
}

void test_iterate() {
 StringBuilder builder(1000);
  for (auto &ref : builder) {
    ref = ' ';
  }
  builder.setCaret(builder.end());
  assert(builder.build().trimLeft().length() == 0);

  StringBuilder builder2(1000);
  for (auto &ref : builder2) {
    ref = 'a';
  }
  builder2.setCaret(builder2.end());
  assert(builder2.build().trim().length() == 1000);
}

void test_replace() {
  StringBuilder builder(6);
  assert(builder.put(builder.begin(), "aaabbb").replace('a', 'b').build() == "bbbbbb");

  StringBuilder b2(100);
  assert(b2.put("aaabbb").replace('a', '\t').build().trim() == "bbb");

  StringBuilder b3(100);
  assert(b3.put("aaabbb").replace('b', ' ').build().trim() == "aaa");

  StringBuilder b4(100000);
  for (auto &ref : b4) {
    ref = 'a';
  }
  StringBuilder b5(100000);
  for (auto &ref : b5) {
    ref = '\0';
  }
  assert(b4.setCaret(b4.end()).build() == b5.setCaret(b5.end()).replace('\0', 'a').build());

  StringBuilder b6(7);
  assert(b6.put("abababa")
      .replace('a', 'z')
      .replace('z', 'b')
      .build()
      .trimLeft("b").length() == 0);
}

int main() {
  test_putAt();
  test_put();
  test_iterate();
  test_replace();
  return 0;
}
