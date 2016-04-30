#ifndef org_modcpp_string_String
#define org_modcpp_string_String

#include <cstdlib>
#include <cstring>
#include <ostream>
#include <utility>

#include "org/modcpp/base/base.h"

namespace org::modcpp::string {

  class String {
   private:
    friend class StringBuilder;
    struct Data;

   public:
    typedef const char *Iterator;
    typedef uint32_t SizeType;

   public:
    String() : data(0), start(""), len(0) {}

    String(const char *text) : data(0), start(text), len(std::strlen(text)) {
      assert(text && "Warning: unintentional integral value to char * conversion");
    }

    String(const char *text, SizeType length) : data(0), start(text), len(length) {}

    String(const String &rhs) : data(rhs.data), start(rhs.start), len(rhs.len) {
      Data::adopt(data);
    }

    ~String() {
      Data::disown(data);
    }

    static String copyOf(cstring text, SizeType length);
    String &operator=(const String &rhs);
    String operator+(const String &rhs);
    String &trimLeft(const char *trimChars = " \t");
    String &trimRight(const char *trimChars = " \t");
    String &trim(const char *trimchars = " \t");
    Iterator findFirst(String pattern) const;
    Iterator findFirst(Iterator startPos, String pattern) const;

   public:
    bool isEmpty() const { return len == 0; }
    uint length() const { return len; }
    Iterator begin() const { return start; }
    Iterator end() const { return start + len; }

    String substring(SizeType beginIndex, SizeType endIndex) const {
      assert(beginIndex <= len && endIndex <= len && endIndex >= beginIndex);
      Data::adopt(data);
      return String(data, start + beginIndex, endIndex - beginIndex);
    }

    static String copyOf(cstring text) {
      return String::copyOf(text, std::strlen(text));
    };

    static String valueOf(int i) {
      char buffer[25];
      int length = sprintf(buffer, "%d", i);
      return String::copyOf(buffer, length);
    }

    [[beta]]
    static String fromChar(char c) {
      return String::copyOf(&c, 1);
    }

    /**
     * TODO(saglam): Remove ostream
     */
    friend std::ostream& operator<<(std::ostream &out, const String &string) {
      return out.write(string.start, string.len);
    }

    String operator+(const int rhs) {
      return operator+(String::valueOf(rhs));
    }

    bool operator==(const String &rhs) const {
      return len == rhs.len && std::memcmp(start, rhs.start, len) == 0;
    }

    bool operator!=(const String &rhs) const {
      return !(*this == rhs);
    }

    String &trimLeft(SizeType delta) {
      assert(delta <= len);
      start += delta;
      len -= delta;
      return *this;
    }

    String &trimRight(SizeType delta) {
      assert(delta <= len);
      len -= delta;
      return *this;
    }

#ifndef NDEBUG
   public:
    const String serialize();
#endif

   private:
    Data *data;
    Iterator start;
    SizeType len;

   private:
    String(Data *data, Iterator start, SizeType length)
        : data(data), start(start), len(length) {}

    struct Data {
      SizeType size;
      uint refCount;

      const char *getText() const {
        return (const char *) this + sizeof(Data);
      }

      static void adopt(Data *data) {
        if (data) {
          ++(((Data *) data)->refCount);
        }
      }

      static void disown(Data *data) {
        if (data && --(((Data *) data)->refCount) == 0) {
          free(data);
        }
      }

      static bool isOwner(Data *data) {
        return data && ((Data *) data)->refCount == 1;
      }

      static Data *createWithCapacity(SizeType capacity) {
        uint size = sizeof(Data) + capacity;
        Data *data = (Data *) std::malloc(size);
        assert(data && "Data::createWithCapacity()");
        data->size = size;
        data->refCount = 1;
        return data;
      }

      static Data *resize(void *data, SizeType newSize) {
        Data *newData = (Data *) realloc(data, newSize);
        assert(newData && "Data::resize()");
        newData->size = newSize;
        return newData;
      }
    }; // struct Data
  }; // class String

  String operator+(const char *lhs, const String &rhs);

}

namespace std {
  /**
   * TODO(saglam): Implement an additive hash or look up Patrascu's hash.
   * TODO(saglam): Remove this
   */
  template <>
  struct hash<org::modcpp::string::String>
      : public unary_function<org::modcpp::string::String, size_t> {
    size_t operator()(const org::modcpp::string::String& value) const {
      return value.length();
    }
  };

} // namespace

#endif
