#include "org/modcpp/string/String.h"

#include <cassert>
#include <cstring>
#include <iostream>

#include "org/modcpp/logging/Console.h"

using org::modcpp::string::String;
using std::cout;
using std::endl;

void test_substring() {
  String str = "mississippi";
  assert(str.substring(0, 0) == "");
  assert(str.substring(0, 1) == "m");
  assert(str.substring(0, 11) == str);
}

void test_trimLeft() {
  String a = "\t \t test";
  a.trimLeft();
  assert(a == "test");

  String b = " abba";
  b.trimLeft("baba ");
  assert(b == "");

  String c = "";
  assert(c.trimLeft() == "");

  String d = "  ";
  assert(d.trimLeft() == "");
}

void test_trimRight() {
  String a = "ads";
  assert(a.trimRight("abcds") == "");

  String b = "asdf\t \t ";
  assert(b.trimRight() == "asdf");

  String c = "";
  assert(c.trimRight() == "");

  String d = "  ";
  assert(d.trimRight() == "");
}

void test_trim() {
  String a = "\"ads\",";
  assert(a.trim(",\"") == "ads");

  String b = " \t asdf\t \t ";
  assert(b.trim() == "asdf");

  String c = "";
  assert(c.trim() == "");

  String d = "  ";
  assert(d.trim() == "");
}

void test_operatorPlus() {
  String a = "a";
  String b = "b";
  String abba = "abba";

  assert(a + b == "ab");
  assert(a + (a + b) == a + a + b);
  assert(b + a != a + b);
  assert(a + b + b + a == abba);
}

void test_operatorEqual() {
  char buffer[512];
  const char *str = "muffin";

  strcpy(buffer, str);
  assert(String(buffer) == String(str));

  assert(String(buffer) != String("kuffer"));
}

void test_doubleRelease() {
  String aa = "aa";
  String bb = "bb";
  String aabb = aa + bb; // Non-tenant string
  String ab = aabb.substring(1, 3);
  String abb = aabb.substring(1, 4);

  assert(ab == "ab");
  assert(abb == "abb");

  ab = aabb;
  abb = aabb;

  assert(ab == abb && abb == aabb && aabb == "aabb");
}

void test_findFirst() {
  String text = ".....a.....a...";
  assert(text.findFirst(text.begin(), "a") == text.begin() + 5);
  assert(text.findFirst("a.") == text.begin() + 5);
}

int main() {
  test_substring();
  test_trimLeft();
  test_trimRight();
  test_trim();
  test_operatorPlus();
  test_operatorEqual();
  test_doubleRelease();
  test_findFirst();
  return 0;
}
