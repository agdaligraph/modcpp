#ifndef org_modcpp_sets_LookupSet
#define org_modcpp_sets_LookupSet

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <utility>

#include "org/modcpp/base/Optional.h"

namespace org::modcpp::sets {

  using org::modcpp::base::Optional;

  template<typename HashableT>
  class LookupSet {
   public:
    typedef uint32_t SizeType;
    typedef HashableT *Iterator;
    typedef typename HashableT::KeyT KeyT;

   public:
    LookupSet(SizeType logCapacity);
    ~LookupSet() { free(array); }
    LookupSet(const LookupSet &) = delete;
    LookupSet(LookupSet &&) = delete;

   public:
    bool empty() const { return numItems == 0; }
    SizeType size() const { return numItems; }
    SizeType capacity() const { return arrayLength; }
    Optional<HashableT &> get(KeyT) const;
    Optional<HashableT &> remove(KeyT);
    Optional<HashableT &> remove(HashableT &);
    void insert(HashableT &);

   private:
    SizeType numItems;
    SizeType arrayLength;
    SizeType arrayMask;
    Iterator *array;
  };

  template<typename HashableT>
  LookupSet<HashableT>::LookupSet(SizeType logCapacity) : numItems(0),
      arrayLength(1 << logCapacity), arrayMask((1 << logCapacity) - 1),
      array((Iterator *) std::malloc(arrayLength * sizeof(Iterator))) {
    assert(logCapacity <= 8 * sizeof(SizeType) - 1 && "Log capacity too high");
    assert(array && "sets::LookupSet: allocation failed.");
    memset(array, 0, arrayLength * sizeof(Iterator));
  }

  template<typename HashableT>
  void LookupSet<HashableT>::insert(HashableT &item) {
    SizeType arrayIndex = item.key.getHash() & arrayMask;
    item.nextHashable = array[arrayIndex];
    array[arrayIndex] = &item;
    ++numItems;
    assert(numItems < (arrayLength << 2) && "LookupSet is overloaded.");
  }

  template<typename HashableT>
  Optional<HashableT &> LookupSet<HashableT>::get(KeyT key) const {
    SizeType arrayIndex = key.getHash() & arrayMask;

    for (Iterator itemIt = array[arrayIndex]; itemIt != nullptr; itemIt = itemIt->nextHashable) {
      if (itemIt->key == key) {
        return Optional<HashableT &>(*itemIt);
      }
    }
    return Optional<HashableT &>::absent();
  }

  template<typename HashableT>
  Optional<HashableT &> LookupSet<HashableT>::remove(KeyT key) {
    SizeType arrayIndex = key.getHash() & arrayMask;

    for (Iterator itemIt = array[arrayIndex], prevIt = nullptr; itemIt != nullptr;) {
      if (itemIt->key == key) {
        if (prevIt == nullptr) {
          array[arrayIndex] = itemIt->nextHashable;
        } else {
          prevIt->nextHashable = itemIt->nextHashable;
        }
        --numItems;
        return Optional<HashableT &>(*itemIt);
      }
      prevIt = itemIt;
      itemIt = itemIt->nextHashable;
    }
    return Optional<HashableT &>::absent();
  }

  template<typename HashableT>
  Optional<HashableT &> LookupSet<HashableT>::remove(HashableT &item) {
    auto maybeItem = remove(item.key);
    assert(maybeItem.isPresent() && (&maybeItem.get() == &item));
    return maybeItem;
  }

}

#endif
