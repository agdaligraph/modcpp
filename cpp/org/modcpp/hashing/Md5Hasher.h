#ifndef org_modcpp_hashing_Md5Hasher
#define org_modcpp_hashing_Md5Hasher

#include "org/modcpp/base/base.h"
#include "org/modcpp/string/Cstrings.h"
#include "org/modcpp/string/String.h"

namespace org::modcpp::hashing {

  using org::modcpp::string::String;
  using org::modcpp::string::Cstrings;

  class Md5Hasher {
   public:
    typedef unsigned int SizeType;

   public:
    Md5Hasher();
    explicit Md5Hasher(cstring text);
    explicit Md5Hasher(const String& text);

   public:
    void put(const unsigned char *buffer, SizeType length);
    inline void put(cstring buffer, SizeType length);
    inline void put(const String& text);
    String hash();
    String hash(const String& text);
    template<typename RangeT> RangeT hash(const String& text);

   private:
    const static int blockSize = 64;
    void init();
    void finalize();
    void transform(const uint8_t block[blockSize]);
    static void decode(uint32_t output[], const uint8_t input[], SizeType len);
    static void encode(uint8_t output[], const uint32_t input[], SizeType len);

   private:
    bool finalized;
    uint8_t buffer[blockSize]; // bytes that didn't fit in last 64 byte chunk
    uint32_t count[2];  // 64bit counter for number of bits (lo, hi)
    uint32_t state[4];  // digest so far
    uint8_t digest[16]; // the result
  };

  void Md5Hasher::put(const String& input) {
    put(input.begin(), input.length());
  }

  void Md5Hasher::put(cstring input, SizeType size) {
    put((const unsigned char*) input, size);
  }

  template<typename RangeT>
  RangeT Md5Hasher::hash(const org::modcpp::string::String& text) {
    put(text.begin(), text.length());
    finalize();
    return std::move(*(const RangeT*) digest);
  }

}

#endif
