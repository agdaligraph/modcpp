#ifndef org_modcpp_base_IterableIterator
#define org_modcpp_base_IterableIterator

#include <iterator>

namespace org::modcpp::base {

  template<typename IterableT>
  class IterableIterator {
   private:
    typedef IterableT *IterableTPointer;

   public:
    std::bidirectional_iterator_tag iterator_category;
    IterableIterator(IterableT &iterable) : iterablePtr(&iterable) {}
    IterableIterator(const IterableTPointer iterablePtr) : iterablePtr(iterablePtr) {}

   public:
    IterableIterator &operator++() {
      iterablePtr = iterablePtr->next;
      return *this;
    }

    IterableIterator &operator--() {
      iterablePtr = iterablePtr->prev;
      return *this;
    }

    bool operator!=(const IterableIterator<IterableT> &rhs) const {
      return iterablePtr != rhs.iterablePtr;
    }

    bool operator==(const IterableIterator<IterableT> &rhs) const {
      return iterablePtr == rhs.iterablePtr;
    }

    IterableT &operator*() { return *iterablePtr; }

   private:
    IterableTPointer iterablePtr;
  };

}

#endif
