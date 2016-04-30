#include "org/modcpp/hashing/KWiseHasher.h"

#include <iostream>

#include "org/modcpp/string/String.h"

using org::modcpp::hashing::KWiseHasher;
using org::modcpp::string::String;

void test_constructor() {
  {
    KWiseHasher<int, int, 3> intHasher;
    std::clog << intHasher.hash(100);

    KWiseHasher<double, double, 1000> doubleHasher;
    std::clog << doubleHasher.hash(45.0);

    KWiseHasher<long double, long double, 100000> longDoubleHasher;
    std::clog << longDoubleHasher.hash(19203.98);

    KWiseHasher<float, float, 100000> floatHasher;
    std::clog << floatHasher.hash(234.0f);

    KWiseHasher<char, char, 10000> charHasher;
    std::clog << charHasher.hash('a');
  }
  { // Across arithmetic types
    KWiseHasher<double, int, 100> doubleToInt;
    std::clog << doubleToInt.hash(101.000);

    KWiseHasher<long double, char, 100000> longDoubleToCharHasher;
    std::clog << longDoubleToCharHasher.hash(435.89);
  }
  { // String to arithmetic types
    KWiseHasher<String, int, 100000> stringToInt;
    std::clog << stringToInt.hash("</xml>");
    
    KWiseHasher<String, unsigned long long int, 100000> stringToLongInt;
    std::clog << stringToLongInt.hash("98ABC54J");
    
    KWiseHasher<String, double, 100000> stringToDouble;
    std::clog << stringToDouble.hash("{'key' : 'value'}");

    KWiseHasher<String, float, 100000> stringToFloat;
    std::clog << stringToFloat.hash("aaaaaaaa");
  }
}

void test_hash() {
}

int main() {
  test_constructor();
  test_hash();
  return 0;
}
