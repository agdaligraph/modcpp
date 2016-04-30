#include "org/modcpp/sets/LookupSet.h"

#include "org/modcpp/string/String.h"

using org::modcpp::sets::LookupSet;
using org::modcpp::string::String;

class PoorlyHashable {
 public:
  class KeyT {
    String id;

   public:
    KeyT(String id) : id(id) {}

   public:
    bool operator==(const KeyT &rhs) { return id == rhs.id; } 
    uint32_t getHash() { return 1; }
    String getKeyId() { return id; }
  }; 

 public:
  PoorlyHashable(String key, int value) : value(value), key(key) {}

 public:
  int getValue() { return value; }
  String getKeyId() { return key.getKeyId(); }

 private:
  int value;
  KeyT key;
  
 private:
  friend class org::modcpp::sets::LookupSet<PoorlyHashable>;
  PoorlyHashable *nextHashable;
};

void test_constructor() {
  LookupSet<PoorlyHashable> set1(4);
  LookupSet<PoorlyHashable> set2(10);

  assert(set1.size() == 0);
  assert(set1.capacity() == 16);
  assert(set1.empty());
  assert(set2.capacity() == 1024);
}


void test_insert() {
  LookupSet<PoorlyHashable> set(10);
  PoorlyHashable numbers[] = {{"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5}};

  for (auto &number : numbers) {
    set.insert(number);
  }

  assert(set.size() == 5);
}

void test_insertGet() {
  LookupSet<PoorlyHashable> set(10);
  PoorlyHashable numbers[] = {
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"ten", 10},
  };

  for (auto &number : numbers) {
    set.insert(number);
  }

  assert(set.size() == 10);
  auto maybeValue = set.get(String("seven"));
  assert(maybeValue.isPresent());
  assert(maybeValue.get().getValue() == 7);

  maybeValue = set.get(String("ten"));
  assert(maybeValue.isPresent());

  auto maybeValue2 = maybeValue;
  assert(maybeValue2.isPresent());
  assert(maybeValue2.get().getValue() == 10);

  maybeValue2 = set.get(String("hasan"));
  assert(!maybeValue2.isPresent());

  maybeValue2 = set.get(String("nine"));
  assert(maybeValue2.get().getValue() == 9);

  maybeValue = set.get(String("eleven"));
  assert(!maybeValue.isPresent());

  for (auto &number : numbers) {
    auto maybeValue = set.get(number.getKeyId());
    assert(maybeValue.isPresent());
    assert(maybeValue.get().getValue() == number.getValue());
    assert(&maybeValue.get() == &number);
  }
}

void test_insertRemoveGet() {
  LookupSet<PoorlyHashable> set(10);
  PoorlyHashable numbers[] = {
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"ten", 10},
  };

  for (auto &number : numbers) {
    set.insert(number);
  }

  auto maybeValue = set.remove(String("ten"));
  assert(maybeValue.isPresent());
  assert(maybeValue.get().getValue() == 10);
  assert(set.size() == 9);

  auto maybeValue2 = set.remove(String("thousand"));
  assert(!maybeValue2.isPresent());
  assert(set.size() == 9);

  maybeValue = set.remove(String("four"));
  assert(maybeValue.isPresent());
  assert(maybeValue.get().getValue() == 4);
  assert(set.size() == 8);

  maybeValue = set.remove(String("four"));
  assert(!maybeValue.isPresent());
  assert(set.size() == 8);

  PoorlyHashable newFour("four", 40);
  set.insert(newFour);
  maybeValue2 = set.get(String("four"));
  assert(maybeValue2.isPresent());
  assert(maybeValue2.get().getValue() == 40);
  assert(set.size() == 9);
}

void test_removeItem() {
  LookupSet<PoorlyHashable> set(10);
  PoorlyHashable numbers[] = {
    {"one", 1}, {"two", 2}, {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}, {"ten", 10},
  };

  for (auto &number : numbers) {
    set.insert(number);
  }

  for (auto &number : numbers) {
    set.remove(number);
  }

  assert(set.empty());
}

int main() {
  test_constructor();
  test_insert();
  test_insertGet();
  test_insertRemoveGet();
  test_removeItem();
  return 0;
}
