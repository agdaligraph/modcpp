#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <assert.h>

#include "org/modcpp/string/String.h"
#include "org/modcpp/logging/Console.h"

using org::modcpp::string::String;
using std::cout;
using std::endl;

#define TRIM trimLeft

#define M 200000

int main() {
  char buffer[2 * M];

  for (int i = 0; i < 100000; ++i) {
    int rn =  (rand() % M) + M - 1; // Uniform between 0 and 2M-2 included.
    memset(buffer, ' ', rn);
    buffer[rn] = 0;
    String a(buffer, rn);
    a.TRIM();
    assert(a.length() == 0);
  }
  return 0;
}
