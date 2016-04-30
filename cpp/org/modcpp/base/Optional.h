#ifndef org_modcpp_base_Optional
#define org_modcpp_base_Optional

#include <cassert>
#include <new>
#include <utility>

namespace org::modcpp::base {

  template<typename Type> class Optional;
  template<typename Type> class Optional<Type &>;

  template<typename Type>
  class Optional<Type &> {
   private:
    typedef Type *TypePointer;

   public:
    Optional(Type &value) : value(&value) {}
    Optional() : value(nullptr) {}
    static Optional<Type &> absent() { return Optional(); }

   public:
    bool isPresent() const { return value; }
    Type &get() const { return *value; }

   private:
    TypePointer value;
  };

  template<typename Type>
  class Optional {
   public:
    Optional(const Type &value) : present(true), value(value) {}
    Optional(Type &&rvalue) : present(true), value(std::move(rvalue)) {}
    Optional() : present(false) {}
    ~Optional();
    Optional(Optional<Type> &&rhs);
    Optional(const Optional<Type> &rhs);
    static Optional<Type> absent() { return Optional(); }

   public:
    bool isPresent() const { return present; }

    Type get() {
      assert(present && "The value is not present or has been moved out.");
      Type returnValue = std::move(value);
      value.~Type();
      present = false;
      return returnValue;
    }

    Type orElse(const Type& other) {
      return present ? get() : other;
    }

   private:
    bool present;
    union { Type value; };
  };

  template<typename Type>
  Optional<Type>::~Optional() {
    if (present) {
      value.~Type();
    }
  }

  template<typename Type>
  Optional<Type>::Optional(Optional<Type> &&rhs) : present(rhs.present) {
    if (present) {
      ::new (&value) Type(std::move(rhs.value));
    }
  }

  template<typename Type>
  Optional<Type>::Optional(const Optional<Type> &rhs) : present(rhs.present) {
    if (present) {
      ::new (&value) Type(rhs.value);
    }
  }

} // namespace

#endif
