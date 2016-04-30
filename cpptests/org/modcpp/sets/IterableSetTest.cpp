#include "org/modcpp/sets/IterableSet.h"

#include "org/modcpp/string/String.h"

using org::modcpp::base::IterableIterator;
using org::modcpp::sets::IterableSet;
using org::modcpp::string::String;

class IntIterable {
 public:
  IntIterable(int value) : value(value) {}

 public:
  int getValue() const { return value; }

 private:
  const int value;

 private:
  friend class IterableIterator<IntIterable>;
  friend class IterableIterator<const IntIterable>;
  friend class IterableSet<IntIterable>;
  IntIterable(IntIterable *prev, IntIterable *next) : value(0), prev(prev), next(next) {}
  IntIterable *prev;
  IntIterable *next;
};

void test_size() {
  IterableSet<IntIterable> list;

  assert(list.isEmpty());
  assert(list.size() == 0);

  list.insertFront(*new IntIterable(2));

  assert(!list.isEmpty());
  assert(list.size() == 1);

  list.insertFront(*new IntIterable(2));
  assert(list.size() == 2);

  list.apply([] (IntIterable &intIterable) {
    assert(intIterable.getValue() == 2);
    delete &intIterable;
  });
}

void test_iteration() {
  IterableSet<IntIterable> list;

  list.insertFront(*new IntIterable(6));
  list.insertFront(*new IntIterable(5));
  list.insertFront(*new IntIterable(4));
  list.insertFront(*new IntIterable(3));
  list.insertFront(*new IntIterable(2));
  list.insertFront(*new IntIterable(1));

  assert(list.size() == 6);

  int j = 1;
  for (auto element : list) {
    assert(element.getValue() == j++);
  }

  j = 1;
  const IterableSet<IntIterable> &constList = list;
  for (auto &itl : constList) {
    assert(itl.getValue() == j++);
  }

  list.apply([] (IntIterable &intIterable) {
    delete &intIterable;
  });
}


void test_remove() {
  IterableSet<IntIterable> list;

  IntIterable iterables[5] = {0, 1, 2, 3, 4};

  list.insertFront(iterables[3]);
  list.insertFront(iterables[2]);
  list.insertFront(iterables[1]);
  list.remove(iterables[2]);

  assert(list.size() == 2);

  list.insertFront(iterables[2]);

  assert(list.size() == 3);

  list.remove(iterables[1]);
  list.insertFront(iterables[1]);
  list.insertFront(iterables[0]);

  assert(list.size() == 4);
  int j = 0;
  for (auto itl : list) {
    assert(itl.getValue() == j++);
  }
}

void test_removeFront() {
  IterableSet<IntIterable> list;

  IntIterable iterables[5] = {0, 1, 2, 3, 4};
  for (auto &iterable : iterables) {
    list.insertFront(iterable);
  }

  auto &ref1 = list.removeFront();
  assert(ref1.getValue() == 4);
  assert(list.size() == 4);
  auto &ref2 = list.removeFront();
  assert(ref2.getValue() == 3);
  assert(list.size() == 3);
  auto &ref3 = list.removeFront();
  assert(ref3.getValue() == 2);
  assert(list.size() == 2);
  auto &ref4 = list.removeFront();
  assert(ref4.getValue() == 1);
  assert(list.size() == 1);
  auto &ref5 = list.removeFront();
  assert(ref5.getValue() == 0);
  assert(list.size() == 0);
  assert(list.isEmpty());
}

void test_insertBefore() {
  IterableSet<IntIterable> list;

  IntIterable odds[5] = {9, 7, 5, 3, 1};
  for (auto &odd : odds) {
    list.insertFront(odd);
  }

  IntIterable evens[5] = {0, 2, 4, 6, 8};
  list.insertBefore(evens[0], odds[4]);
  list.insertBefore(evens[1], odds[3]);
  list.insertBefore(evens[2], odds[2]);
  list.insertBefore(evens[3], odds[1]);
  list.insertBefore(evens[4], odds[0]);

  assert(list.size() == 10);
  int j = 0;
  for (auto &iterable : list) {
    assert(iterable.getValue() == j++);
  }

  list.removeFront();
  list.remove(odds[4]);
  assert(list.size() == 8);

  list.insertFront(evens[0]);
  list.insertBefore(odds[4], evens[1]);
  assert(list.size() == 10);
  j = 0;
  for (auto &iterable : list) {
    assert(iterable.getValue() == j++);
  }

  IntIterable one(1);
  IterableSet<IntIterable> list2;
  list2.insertBefore(one, list2.front());
  assert(list2.front().getValue() == 1);
}

namespace test {
  namespace inner {
    class StringIterable {
     public:
      StringIterable(String value) : value(value) {}

     public:
      String getValue() const {
        return value;
      }

     private:
      String value;

     private:
      friend class IterableIterator<StringIterable>;
      friend class IterableIterator<const StringIterable>;
      friend class IterableSet<StringIterable>;
      StringIterable(StringIterable *prev, StringIterable *next)
          : value(""), prev(prev), next(next) {}
      StringIterable *prev;
      StringIterable *next;
    };
  } // namespace

  void test_iteration() {
    IterableSet<inner::StringIterable> list;

    list.insertFront(*new inner::StringIterable("ali"));
    list.insertFront(*new inner::StringIterable("veli"));

    const IterableSet<inner::StringIterable> &constList = list;
    String result = "";
    for (const auto &itl : constList) {
      result = result + itl.getValue();
    }
    assert(result == "veliali");

    list.apply([] (inner::StringIterable &iterable) {
      delete &iterable;
    });
  }
} // namespace


void test_reverseIterate() {
  IterableSet<IntIterable> list;

  IntIterable digits[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (auto &digit : digits) {
    list.insertFront(digit);
  }

  int j = 0;
  for (auto &begin = --list.end(), &end = --list.begin(); begin != end; --begin) {
    assert((*begin).getValue() == j++);
  }
}

int main () {
  test_size();
  test_iteration();
  test_remove();
  test_removeFront();
  test_insertBefore();
  test::test_iteration();
  test_reverseIterate();
  return 0;
}
