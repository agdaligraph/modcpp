#include "org/modcpp/string/StringBuilder.h"

#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>

#include "org/modcpp/logging/Console.h"
#include "org/modcpp/string/String.h"

using org::modcpp::string::String;
using org::modcpp::string::StringBuilder;
using std::cout;
using std::endl;

#define M 200000

int main() {

  for (int i = 0; i < 100000; ++i) {
    int rn =  (rand() % M) + M - 1; // Uniform between 0 and 2M-2 included.
    StringBuilder builder(rn);
    memset(builder.begin(), ' ', rn);
    builder.replace(' ', 'a');
  }
  cout << "a\n";
  return 0;
}
