#ifndef org_modcpp_sketching_SpaceSavingSketch
#define org_modcpp_sketching_SpaceSavingSketch

#include <algorithm>
#include <limits>
#include <type_traits>
#include <vector>

#include "org/modcpp/base/IterableIterator.h"
#include "org/modcpp/hashing/KarpRabinHasher.h"
#include "org/modcpp/sets/IterableSet.h"
#include "org/modcpp/sets/LookupSet.h"

namespace org::modcpp::sketching {
  using org::modcpp::sets::LookupSet;
  using org::modcpp::sets::IterableSet;
  using org::modcpp::base::IterableIterator;

  template<typename DomainT, typename RangeT>
  class SpaceSavingSketch {
   public:
    static_assert(std::is_arithmetic<RangeT>::value, "RangeT must be an arithmetic type");
    struct Item;
    class Bucket;
    typedef uint32_t SizeType;
    typedef Bucket *BucketPointer;
    typedef Item *ItemPointer;
    typedef typename Item::KeyT KeyT;
    static const SizeType NullIndex = std::numeric_limits<SizeType>::max();

   public:
    SpaceSavingSketch(SizeType logCounters);

   public:
    void update(const DomainT coord, const RangeT update);
    RangeT get(const DomainT coord) const;

    template<typename ListItemType>
    void getTopK(std::vector<ListItemType> &list, const SizeType k) const;

   testvisible:
    Bucket &detachFromBucket(Item &item);
    Bucket &ensureBucket(Bucket &start, RangeT count);
    void attachToBucket(Item &item, Bucket &bucket);

   testvisible:
    SizeType numCounters;
    LookupSet<Item> coordToItem;
    std::vector<Item> items;

    std::vector<Bucket> buckets;
    IterableSet<Bucket> freeBuckets;
    IterableSet<Bucket> usedBuckets;
  };

  template<typename DomainT, typename RangeT>
  class SpaceSavingSketch<DomainT, RangeT>::Bucket {
   public:
    Bucket() {}
    RangeT count;
    SizeType firstInBucket;

    BucketPointer prev;
    BucketPointer next;

   private:
    Bucket(BucketPointer prev, BucketPointer next)
        : count(std::numeric_limits<RangeT>::max()), prev(prev), next(next) {}
    friend class IterableSet<Bucket>;
  };

  template<typename DomainT, typename RangeT>
  struct SpaceSavingSketch<DomainT, RangeT>::Item {
    class KeyT {
     public:
      KeyT() {}
      KeyT(const DomainT &coord) : coord(coord) {}

      SizeType getHash() {
        static org::modcpp::hashing::KarpRabinHasher coordHasher;
        return coordHasher.hash(coord);
      }

      DomainT getCoordinate() const { return coord; }

      bool operator==(const KeyT &rhs) { return coord == rhs.coord; }

     private:
      DomainT coord;
    };

    Item() {}
    Item(KeyT &&key) : key(std::move(key)), maxOvershoot(0) {}

    KeyT key;
    SizeType bucket;
    SizeType prevInBucket;
    SizeType nextInBucket;
    RangeT maxOvershoot;

   private:
    friend class LookupSet<Item>;
    ItemPointer nextHashable;
  };

  template<typename DomainT, typename RangeT>
  SpaceSavingSketch<DomainT, RangeT>::SpaceSavingSketch(SizeType logCounters)
      : numCounters(1 << logCounters),
        coordToItem(logCounters + 1), // Hash table has twice the size of the items
        buckets(numCounters) {
    items.reserve(numCounters);
    for (auto &bucket : buckets) {
      freeBuckets.insertFront(bucket);
    }
  }

  template<typename DomainT, typename RangeT>
  void SpaceSavingSketch<DomainT, RangeT>::update(DomainT coord, RangeT delta) {
    assert(delta > 0 && "SpaceSavingSketch allows only positive updates.");

    KeyT key(coord);
    auto maybeItem = coordToItem.get(key);
    ItemPointer itemPtr;
    RangeT newCount;
    BucketPointer startBucketPtr;
    if (maybeItem.isPresent()) {
      itemPtr = &maybeItem.get();
      newCount = buckets[itemPtr->bucket].count + delta;
      startBucketPtr = &detachFromBucket(*itemPtr);
    } else if (items.size() < numCounters) {
      items.emplace_back(std::move(key));
      itemPtr = &items.back();
      coordToItem.insert(*itemPtr);
      newCount = delta;
      startBucketPtr = &usedBuckets.front();
    } else {
      Bucket &smallestBucket = usedBuckets.front();
      itemPtr = &items[smallestBucket.firstInBucket];
      newCount = smallestBucket.count + delta;
      itemPtr->maxOvershoot = smallestBucket.count;
      startBucketPtr = &detachFromBucket(*itemPtr);
      coordToItem.remove(*itemPtr);
      itemPtr->key = key;
      coordToItem.insert(*itemPtr);
    }
    Bucket &newBucket = ensureBucket(*startBucketPtr, newCount);
    attachToBucket(*itemPtr, newBucket);
  }

  template<typename DomainT, typename RangeT>
  RangeT SpaceSavingSketch<DomainT, RangeT>::get(const DomainT coord) const {
    auto maybeItem = coordToItem.get(KeyT(coord));
    return maybeItem.isPresent()
        ? buckets[maybeItem.get().bucket].count
        : items.size() < numCounters
            ? 0
            : usedBuckets.front().count;
  }

  template<typename DomainT, typename RangeT>
  typename SpaceSavingSketch<DomainT, RangeT>::Bucket &
      SpaceSavingSketch<DomainT, RangeT>::detachFromBucket(Item &item) {
    Bucket &bucket = buckets[item.bucket];
    SizeType nextItem = item.nextInBucket;
    SizeType prevItem = item.prevInBucket;
    if (prevItem == NullIndex) {
      if (nextItem == NullIndex) {
        BucketPointer next = bucket.next;
        usedBuckets.remove(bucket);
        freeBuckets.insertFront(bucket);
        return *next;
      } else {
        items[nextItem].prevInBucket = NullIndex;
        bucket.firstInBucket = nextItem;
      }
    } else if (nextItem == NullIndex) {
      items[prevItem].nextInBucket = NullIndex;
    } else {
      items[prevItem].nextInBucket = nextItem;
      items[nextItem].prevInBucket = prevItem;
    }
    return bucket;
  }

  template<typename DomainT, typename RangeT>
  typename SpaceSavingSketch<DomainT, RangeT>::Bucket &
      SpaceSavingSketch<DomainT, RangeT>::ensureBucket(Bucket &start, RangeT count) {
    auto &foundBucket = *std::find_if(IterableIterator<Bucket>(start), usedBuckets.end(),
        [&count] (const Bucket &bucket) {
      return bucket.count >= count;
    });

    if (foundBucket.count == count) {
      return foundBucket;
    }
    Bucket &newBucket = freeBuckets.removeFront();
    newBucket.count = count;
    newBucket.firstInBucket = NullIndex;
    usedBuckets.insertBefore(newBucket, foundBucket);
    return newBucket;
  }

  template<typename DomainT, typename RangeT>
  void SpaceSavingSketch<DomainT, RangeT>::attachToBucket(Item &item, Bucket &bucket) {
    SizeType itemIdx = &item - items.data();
    SizeType bucketIdx = &bucket - buckets.data();
    item.bucket = bucketIdx;
    item.nextInBucket = bucket.firstInBucket;
    item.prevInBucket = NullIndex;
    if (item.nextInBucket != NullIndex) {
      items[item.nextInBucket].prevInBucket = itemIdx;
    }
    bucket.firstInBucket = itemIdx;
  }

  template<typename DomainT, typename RangeT>
  template<typename ListItemType>
  void SpaceSavingSketch<DomainT, RangeT>::getTopK(
      std::vector<ListItemType> &list, const SizeType k) const {
    list.resize(std::min((std::size_t) k, items.size()));
    auto listBegin = list.begin();
    auto listEnd = list.end();
    for (auto &begin = --usedBuckets.end(), &end = --usedBuckets.begin(); begin != end; --begin) {
      for (SizeType itemIdx = (*begin).firstInBucket; itemIdx != NullIndex && listBegin != listEnd;
          itemIdx = items[itemIdx].nextInBucket) {
        (*listBegin).coord = items[itemIdx].key.getCoordinate();
        (*listBegin).count = buckets[items[itemIdx].bucket].count;
        (*listBegin).maxOvershoot = items[itemIdx].maxOvershoot;
        ++listBegin;
      }
      if (listBegin == listEnd) {
        break;
      }
    }
  }

}

#endif
