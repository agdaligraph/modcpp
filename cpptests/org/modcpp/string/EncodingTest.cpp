#include "org/modcpp/string/Encoding.h"

#include <assert.h>
#include <string.h>

#include "org/modcpp/base/base.h"
#include "org/modcpp/string/String.h"

using org::modcpp::string::Encoding;
using org::modcpp::string::String;

void test_fromIso8859_9() {
  assert(Encoding::fromIso8859_9("") == "");
  assert(Encoding::fromIso8859_9("a\xfe\xe7\xfd \xe2m\xe2 imi\xfe") == "aşçı âmâ imiş");
  assert(Encoding::fromIso8859_9("\xc2\xc7\xce\xd0\xd6\xdb\xdc\xdd\xde") == "ÂÇÎĞÖÛÜİŞ");
  assert(Encoding::fromIso8859_9("1m\xb3") == "1m³");
  assert(Encoding::fromIso8859_9("\xabHaha\xbb") == "«Haha»");
  assert(Encoding::fromIso8859_9("\xc9nl\xe9me") == "Énléme");
}

void test_anglifyChar() {
  assert('s' == Encoding::anglifyChar(*((utf8Char *) "ş\0\0")));
  assert('o' == Encoding::anglifyChar(*((utf8Char *) "ö\0\0")));
  assert('c' == Encoding::anglifyChar(*((utf8Char *) "Ç\0\0")));
  assert('i' == Encoding::anglifyChar(*((utf8Char *) "ı\0\0")));
  assert('a' == Encoding::anglifyChar(*((utf8Char *) "â\0\0")));
  assert('u' == Encoding::anglifyChar(*((utf8Char *) "ü\0\0")));
  assert('g' == Encoding::anglifyChar(*((utf8Char *) "ğ\0\0")));
  assert('u' == Encoding::anglifyChar(*((utf8Char *) "Û\0\0")));
  assert('i' == Encoding::anglifyChar(*((utf8Char *) "Î\0\0")));
  assert('a' == Encoding::anglifyChar(*((utf8Char *) "Â\0\0")));
  assert('a' == Encoding::anglifyChar(*((utf8Char *) "â\0\0")));

  assert('a' == Encoding::anglifyChar(*((utf8Char *) "a\0\0")));
  assert('z' == Encoding::anglifyChar(*((utf8Char *) "z\0\0")));
  assert('?' == Encoding::anglifyChar(*((utf8Char *) "?\0\0")));
}

void test_removeFirstChar() {
  String text = "ÂÇÎĞÖÛÜİŞ";
  assert(*((utf8Char *) "Â\0\0") == Encoding::removeFirstChar(text));
  assert(*((utf8Char *) "Ç\0\0") == Encoding::removeFirstChar(text));
  assert(*((utf8Char *) "Î\0\0") == Encoding::removeFirstChar(text));
  assert(*((utf8Char *) "Ğ\0\0") == Encoding::removeFirstChar(text));
  assert(*((utf8Char *) "Ö\0\0") == Encoding::removeFirstChar(text));
  assert(*((utf8Char *) "Û\0\0") == Encoding::removeFirstChar(text));

  String interleaved = "ağalı";
  assert(*((utf8Char *) "a\0\0\0") == Encoding::removeFirstChar(interleaved));
  assert(*((utf8Char *) "ğ\0\0") == Encoding::removeFirstChar(interleaved));
  assert(*((utf8Char *) "a\0\0") == Encoding::removeFirstChar(interleaved));
  assert(*((utf8Char *) "l\0\0") == Encoding::removeFirstChar(interleaved));
  assert(*((utf8Char *) "ı\0\0") == Encoding::removeFirstChar(interleaved));
  assert(interleaved.length() == 0);
}

int main() {
  test_fromIso8859_9();
  test_anglifyChar();
  test_removeFirstChar();
  return 0;
}
