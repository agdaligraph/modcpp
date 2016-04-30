#ifndef org_modcpp_base
#define org_modcpp_base

#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#define lengthof(X) (sizeof(X) / sizeof(X[0]))

#include "org/modcpp/base/range.h"

namespace org::modcpp::base {

  namespace types {
    typedef unsigned int utf8Char;

    typedef const char * const cstring;

    typedef const char * cstringIterator;

    typedef char * const mutableCstring;

    typedef char * mutableCstringIterator;

    typedef void * Iterator;

    typedef unsigned char uchar;

    typedef unsigned char byte;

    typedef unsigned char * byteIterator;

    typedef uint32_t uint32;
  }

}

using namespace org::modcpp::base::types;
using org::modcpp::base::range;

#endif
