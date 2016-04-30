#include "org/modcpp/base/IterableIterator.h"

#include <cassert>

using org::modcpp::base::IterableIterator;

class IntIterable {
 public:
  IntIterable(int value) : value(value) {}

 public:
  int getValue() const { return value; }
  void setPrev(IntIterable *prev) { this->prev = prev; }
  void setNext(IntIterable *next) { this->next = next; }

 private:
  const int value;

 private:
  friend class IterableIterator<IntIterable>;
  IntIterable *prev;
  IntIterable *next;
};

void test_increment() {
  IntIterable intIbls[3] = {1, 2, 3};
  intIbls[0].setNext(&intIbls[1]);
  intIbls[1].setNext(&intIbls[2]);
  IterableIterator<IntIterable> intItr(intIbls[0]);
  
  assert((*intItr).getValue() == 1);
  ++intItr;
  assert((*intItr).getValue() == 2);
  ++intItr;
  assert((*intItr).getValue() == 3);
}

void test_decrement() {
  IntIterable intIbls[3] = {11, 12, 13};
  intIbls[2].setPrev(&intIbls[1]);
  intIbls[1].setPrev(&intIbls[0]);
  IterableIterator<IntIterable> intItr(intIbls[2]);
  
  assert((*intItr).getValue() == 13);
  --intItr;
  assert((*intItr).getValue() == 12);
  --intItr;
  assert((*intItr).getValue() == 11);  
}

void test_equal() {
  IntIterable intIbls[4] = {10, 10, 20, 40};
  IterableIterator<IntIterable> it1(intIbls[0]);
  IterableIterator<IntIterable> it2(intIbls[1]);
  IterableIterator<IntIterable> it3(intIbls[0]);

  assert(it1 != it2);
  assert(!(it1 == it2));
  assert(it1 == it3);
  assert(!(it1 != it3));
}

int main() {
  test_increment();
  test_decrement();
  test_equal();
  return 0;
}
