#include "org/modcpp/base/Optional.h"

#include <cassert>
#include <utility>

using org::modcpp::base::Optional;

class CopySensitive {
 public:
  CopySensitive(int value) : pointer(new int[1]) { *pointer = value; ++instances; }
  CopySensitive(CopySensitive &&rhs) {
    pointer = rhs.pointer;
    rhs.pointer = nullptr;
    ++instances;
  }

  CopySensitive(const CopySensitive &rhs) {
    assert(false && "Copy constructor invoked.");
  }

  ~CopySensitive() { --instances; delete pointer; }

  int getValue() {
    return *pointer;
  }
  
  static int instances;

 private:
  int *pointer;
};


int CopySensitive::instances = 0;

void test_podConstructor() {
  Optional<int> intOpt1 = 4;
  Optional<int> intOpt2 = Optional<int>::absent();
  Optional<int> intOpt3 = 4;
  Optional<int> intOpt4;

  assert(intOpt1.isPresent());
  assert(!intOpt2.isPresent());
  assert(intOpt3.isPresent());
  assert(!intOpt4.isPresent());
}

void test_refConstructor() {
  int value = 5;
  Optional<int &> refOpt = value;
  Optional<int &> absent = Optional<int &>::absent();
  Optional<int &> absent2;

  assert(refOpt.isPresent());
  assert(refOpt.get() == 5);
  assert(!absent.isPresent());
  assert(!absent2.isPresent());

  ++refOpt.get();

  assert(value == 6);

  Optional<float &> refOpt2;
  assert(!refOpt2.isPresent());
}

void test_classConstructor() {
  {
    Optional<CopySensitive> opt1(CopySensitive(123));
    Optional<CopySensitive> absent1;
    Optional<CopySensitive> absent2;

    assert(opt1.isPresent());
    CopySensitive moved = opt1.get();
    assert(moved.getValue() == 123);

    assert(!absent1.isPresent());
    assert(!absent2.isPresent());
  }

  assert(CopySensitive::instances == 0);
}

void test_assign() {
  {
    Optional<CopySensitive> opt1(CopySensitive(100));
    Optional<CopySensitive> opt2 = std::move(opt1);

    assert(opt2.isPresent());
    assert(opt2.get().getValue() == 100);
  }

  assert(CopySensitive::instances == 0);  
}

int main() {
  test_podConstructor();
  test_refConstructor();
  test_classConstructor();
  test_assign();
  return 0;
}
