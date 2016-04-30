#include "org/modcpp/string/Cstrings.h"

using org::modcpp::string::Cstrings;

void test_contains() {
  assert(Cstrings::contains("ab", "b"));
  assert(Cstrings::contains("ab", "a"));
  assert(Cstrings::contains("ab", ""));
  assert(!Cstrings::contains("ab", "c"));
  assert(!Cstrings::contains("ab", "abc"));
}

void test_isEqual() {
  assert(Cstrings::isEqual("ab", "ab"));
  assert(!Cstrings::isEqual("ba", ""));
}

void test_trimRight() {
  char str[100];

  assert(Cstrings::isEqual("", Cstrings::trimRight(strcpy(str, ""))));
  assert(Cstrings::isEqual("", Cstrings::trimRight(strcpy(str, "   \t"))));
  assert(Cstrings::isEqual("a", Cstrings::trimRight(strcpy(str, "a\t\t\t\t"))));
  assert(Cstrings::isEqual("ab", Cstrings::trimRight(strcpy(str, "ab "))));
}

void test_trimLeft() {
  assert(Cstrings::isEqual("", Cstrings::trimLeft("")));
  assert(Cstrings::isEqual("", Cstrings::trimLeft(" ")));
  assert(Cstrings::isEqual("", Cstrings::trimLeft(" \t\t")));
  assert(Cstrings::isEqual("a", Cstrings::trimLeft("\t\t\t\t \ta")));
  assert(Cstrings::isEqual("ab", Cstrings::trimLeft(" ab")));
}

void test_trim() {
  char str[100];

  assert(Cstrings::isEqual("", Cstrings::trim(strcpy(str, ""))));
  assert(Cstrings::isEqual("", Cstrings::trim(strcpy(str, " "))));
  assert(Cstrings::isEqual("", Cstrings::trim(strcpy(str, " \t\t"))));
  assert(Cstrings::isEqual("a", Cstrings::trim(strcpy(str, "\t\t\t\t \ta \t"))));
  assert(Cstrings::isEqual("ab", Cstrings::trim(strcpy(str, " ab"))));
}

void test_findFirst() {
  cstring str1 = "//";
  cstring str2 = "Mongo/projects/cpp";
  assert(str1 == Cstrings::findFirst(str1, "/"));
  assert(str2 + 14 == Cstrings::findFirst(str2, "/cpp"));
}

void test_splitInto() {
  char buffer[128];
  std::vector<char *> parts;

  parts = Cstrings::splitInto(strcpy(buffer, "hasan, mahmut, armut"), ',');

  assert(Cstrings::isEqual("hasan", parts[0]));
  assert(Cstrings::isEqual(" mahmut", parts[1]));
  assert(Cstrings::isEqual(" armut", parts[2]));

  parts = Cstrings::splitInto(strcpy(buffer, ""), ',');

  assert(parts.size() == 1);
  assert(Cstrings::isEqual("", parts[0]));

  parts = Cstrings::splitInto(strcpy(buffer, ""), ',');
  assert(parts.size() == 1);
  assert(Cstrings::isEqual("", parts[0]));

  parts = Cstrings::splitInto(strcpy(buffer, ","), ',');
  assert(parts.size() == 2);
  assert(Cstrings::isEqual("", parts[0]));
  assert(Cstrings::isEqual("", parts[1]));

  parts = Cstrings::splitInto(strcpy(buffer, ", ,"), ',');
  assert(parts.size() == 3);
  assert(Cstrings::isEqual("", parts[0]));
  assert(Cstrings::isEqual(" ", parts[1]));
  assert(Cstrings::isEqual("", parts[2]));
}

void test_splitIntoKeepBlocks() {
  char buffer[128];
  std::vector<char *> parts;

  parts = Cstrings::splitIntoKeepBlocks(strcpy(buffer, "hasan, (ali, mehmet), murat"), ',', "()");
  assert(parts.size() == 3);
  assert(Cstrings::isEqual("hasan", parts[0]));
  assert(Cstrings::isEqual(" (ali, mehmet)", parts[1]));
  assert(Cstrings::isEqual(" murat", parts[2]));

  parts = Cstrings::splitIntoKeepBlocks(strcpy(buffer, "(,),"), ',', "()");
  assert(parts.size() == 2);
  assert(Cstrings::isEqual("(,)", parts[0]));
  assert(Cstrings::isEqual("", parts[1]));

  parts = Cstrings::splitIntoKeepBlocks(strcpy(buffer, ",(,,,,,,,"), ',', "()");
  assert(parts.size() == 2);
  assert(Cstrings::isEqual("", parts[0]));
  assert(Cstrings::isEqual("(,,,,,,,", parts[1]));

  parts = Cstrings::splitIntoKeepBlocks(strcpy(buffer, ",(,,,,,,,),,(,,)(,),(,,)"), ',', "()");
  assert(parts.size() == 5);
  assert(Cstrings::isEqual("", parts[0]));
  assert(Cstrings::isEqual("(,,,,,,,)", parts[1]));
  assert(Cstrings::isEqual("", parts[2]));
  assert(Cstrings::isEqual("(,,)(,)", parts[3]));
  assert(Cstrings::isEqual("(,,)", parts[4]));
}

void test_replace() {
  char buffer[128];
  assert(Cstrings::isEqual("hoson", Cstrings::replace(strcpy(buffer, "hasan"), 'a', 'o')));
  assert(Cstrings::isEqual("h", Cstrings::replace(strcpy(buffer, "hasan"), 'a', 0)));
}

namespace inner::inner {
  using org::modcpp::string::Cstrings;

  void test_nested() {
    assert(Cstrings::isEqual("test", "test"));
  }
}

int main() {
  test_contains();
  test_isEqual();
  test_trimRight();
  test_trimLeft();
  test_trim();
  test_findFirst();
  test_splitInto();
  test_splitIntoKeepBlocks();
  test_replace();
  inner::inner::test_nested();
  return 0;
}
