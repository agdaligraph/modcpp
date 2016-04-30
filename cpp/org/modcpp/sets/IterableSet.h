#ifndef org_modcpp_sets_IterableSet
#define org_modcpp_sets_IterableSet

#include <cassert>
#include <functional>

#include "org/modcpp/base/IterableIterator.h"

namespace org::modcpp::sets {

  template<typename IterableT>
  class IterableSet {
   public:
    typedef uint32_t SizeType;
    typedef IterableT *IterableTPointer;
    typedef org::modcpp::base::IterableIterator<IterableT> Iterator;
    typedef org::modcpp::base::IterableIterator<const IterableT> ConstIterator;

   public:
    IterableSet() : sentinel(&sentinel, &sentinel), numItems(0) {}

   public:
    Iterator begin() { return Iterator(sentinel.next); }
    Iterator end() { return Iterator(&sentinel); }
    ConstIterator begin() const { return ConstIterator(sentinel.next); }
    ConstIterator end() const { return ConstIterator(&sentinel); }

    bool isEmpty() const { return numItems == 0; }
    SizeType size() const { return numItems; }
    IterableT &front() const { return *(sentinel.next); }
    IterableT &back() const { return *(sentinel.prev); }
    void apply(std::function<void (IterableT &)>) const;

    void insertFront(IterableT &);
    void insertBefore(IterableT &, IterableT &before);
    IterableT &removeFront();
    const IterableT &remove(const IterableT &);

   private:
    IterableT sentinel;
    SizeType numItems;
  };

  template<typename IterableT>
  void IterableSet<IterableT>::insertFront(IterableT &iterable) {
    iterable.next = sentinel.next;
    iterable.prev = &sentinel;
    sentinel.next->prev = &iterable;
    sentinel.next = &iterable;
    ++numItems;
  }

  template<typename IterableT>
  void IterableSet<IterableT>::insertBefore(IterableT &iterable, IterableT &before) {
    iterable.prev = before.prev;
    iterable.next = &before;
    before.prev->next = &iterable;
    before.prev = &iterable;
    ++numItems;
  }

  template<typename IterableT>
  IterableT &IterableSet<IterableT>::removeFront() {
    assert(numItems > 0);
    IterableTPointer iterablePtr = sentinel.next;
    sentinel.next = iterablePtr->next;
    sentinel.next->prev = &sentinel;
    --numItems;
    return *iterablePtr;
  }

  template<typename IterableT>
  const IterableT &IterableSet<IterableT>::remove(const IterableT &iterable) {
    assert(numItems > 0);
    iterable.prev->next = iterable.next;
    iterable.next->prev = iterable.prev;
    --numItems;
    return iterable;
  }

  template<typename IterableT>
  void IterableSet<IterableT>::apply(std::function<void (IterableT &)> lambda) const {
    for (IterableTPointer iterablePtr = sentinel.next; iterablePtr != &sentinel;) {
      auto next = iterablePtr->next;
      lambda(*iterablePtr);
      iterablePtr = next;
    }
  }

}

#endif
