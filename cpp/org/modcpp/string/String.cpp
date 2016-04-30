#include "org/modcpp/string/String.h"

namespace org::modcpp::string {

  static inline uint getAllocationSlack(uint length) {
    return (length >> 1) + 16;
  }

  String operator+(const char *lhs, const String &rhs) {
    return String(lhs) + rhs;
  }

  String &String::operator=(const String &rhs) {
    if (this != &rhs) {
      Data::disown(data);
      data = rhs.data;
      start = rhs.start;
      len = rhs.len;
      Data::adopt(data);
    }
    return *this;
  }

  static inline void prepareLookupTable(char *table, const char * const setChars) {
    memset(table, 0, 256);
    for (const char *ptr = setChars; *ptr; ++ptr) {
      assert(*ptr > 0 && "String::prepareLookupTable() Only ASCII chars are allowed");
      table[*((uchar *) ptr)] = true;
    }
  }

  String &String::trimLeft(const char *trimChars) {
    char isTrimChar[256];
    prepareLookupTable(isTrimChar, trimChars);

    auto left = start;
    for (const auto end = start + len;
        left < end && isTrimChar[*((uchar *) left)]; ++left) {}

    len -= left - start;
    start = left;
    return *this;
  }

  String &String::trimRight(const char *trimChars) {
    char isTrimChar[256];
    prepareLookupTable(isTrimChar, trimChars);

    auto right = start + len - 1;
    for (; right >= start && isTrimChar[*((uchar *) right)]; --right) {}

    len = right - start + 1;
    return *this;
  }

  String &String::trim(const char *trimChars) {
    char isTrimChar[256];
    prepareLookupTable(isTrimChar, trimChars);

    auto left = start;
    for (const auto end = start + len;
        left < end && isTrimChar[*((uchar *) left)]; ++left) {}
    auto right = start + len - 1;
    for (; right >= left && isTrimChar[*((uchar *) right)]; --right) {}

    start = left;
    len = right - left + 1;
    return *this;
  }

  String String::operator+(const String &rhs) {
    String product;
    product.len = len + rhs.len;
    uint aimedSlack = getAllocationSlack(product.len);

    if (Data::isOwner(data)) {
      uint offset = start - (const char *) data;
      if (data->size < offset + product.len) {
        product.data = Data::resize(data, offset + product.len + aimedSlack);

        if (data != product.data) {
          start = (const char *) product.data + offset;
          data = product.data;
        }
      } else {
        product.data = data;
      }
      product.start = start;
      std::memcpy((char *) start + len, rhs.start, rhs.len);
      Data::adopt(data);
    } else {
      product.data = Data::createWithCapacity(product.len + aimedSlack);
      product.start = product.data->getText();
      std::memcpy((char *) product.start, start, len);
      std::memcpy((char *) product.start + len, rhs.start, rhs.len);
    }

    return product;
  }

  String String::copyOf(cstring text, uint length) {
    Data *data = Data::createWithCapacity(length + getAllocationSlack(length));
    std::memcpy((char *) data->getText(), text, length);
    return String(data, data->getText(), length);
  }

  String::Iterator String::findFirst(Iterator startPos, String pattern) const {
    assert(start <= startPos && startPos <= end());
    Iterator first = (Iterator) memmem(startPos, len - (startPos - start),
        pattern.start, pattern.len);
    return first ? first : end();
  }

  String::Iterator String::findFirst(String pattern) const {
    Iterator first = (Iterator) memmem(start, len, pattern.start, pattern.len);
    return first ? first : end();
  }

#ifndef NDEBUG

#define CC(x) (x).length(),(x).begin()

  const String String::serialize() {
    char buffer[512];
    int length;
    if (data) {
      length = sprintf(buffer, "{%.*s, len=%u, cap=%u, refs=%u}",
          CC(*this), len, (uint) (data->size - sizeof(Data)),
          data->refCount);
    } else {
      length = sprintf(buffer, "{%.*s, len=%u, tenant}", CC(*this), len);
    }
    return String::copyOf(buffer, length);
  }

#endif

} // namespace
