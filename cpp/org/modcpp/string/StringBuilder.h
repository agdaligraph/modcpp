#ifndef org_modcpp_string_StringBuilder
#define org_modcpp_string_StringBuilder

#include <algorithm>

#include "org/modcpp/base/base.h"
#include "org/modcpp/string/String.h"

namespace org::modcpp::string {

  class StringBuilder {
   public:
    typedef char *Iterator;
    typedef const char *ConstIterator;

   public:
    explicit StringBuilder(uint capacity)
        : data(String::Data::createWithCapacity(capacity)) {
      start = (Iterator) data->getText();
      caret = start;
    }

    ~StringBuilder() {
      String::Data::disown(data);
    }

   public:
    StringBuilder &putFormatted(const char *formatString, ...);

   public:
    inline Iterator begin() const {
      assert(data);
      return start;
    }

    inline Iterator end() const {
      assert(data);
      return (Iterator) data + data->size;
    }

    inline StringBuilder &setCaret(Iterator it) {
      caret = it;
      return *this;
    }

    inline Iterator getCaret() {
      return caret;
    }

    inline StringBuilder &put(Iterator it, const String &text) {
      auto textEnd = it + text.length();
      assert(data && begin() <= it && textEnd <= end());
      memcpy(it, text.begin(), text.length());
      if (textEnd > caret) {
        caret = textEnd;
      }
      return *this;
    }

    inline StringBuilder &put(const String &text) {
      assert(data && caret + text.length() <= end());
      memcpy(caret, text.begin(), text.length());
      caret += text.length();
      return *this;
    }

    inline StringBuilder &putInterval(ConstIterator begin, ConstIterator end) {
      assert(begin <= end);
      uint length = end - begin;
      memcpy(caret, begin, length);
      caret += length;
      return *this;
    }

    inline StringBuilder &put(char c) {
      assert(caret < end());
      *(caret++) = c;
      return *this;
    }

    inline StringBuilder &replace(char before, char after) {
      assert(data);
      std::replace(begin(), end(), before, after);
      return *this;
    }

    /**
     * @return a {@link String} which owns the data.
     */
    String build() {
      assert(data && "StringBuilder is single serve");
      assert(caret != start && "StringBuilder is empty. Did you mean to set the caret?");
      String::Data *returnedData = data;
      data = nullptr;
      return String(returnedData, start, caret - start);
    }

    /**
     * @return a {@link String} view to the builder. The returned {@link String} is a tenant.
     */
    inline String asString() {
      return String(start, caret - start);
    }

   private:
    String::Data *data;
    Iterator start;
    Iterator caret;

   private:
    StringBuilder(const StringBuilder &) = delete;
    StringBuilder &operator=(const StringBuilder &rhs) = delete;
  };

} // namespace

#endif
