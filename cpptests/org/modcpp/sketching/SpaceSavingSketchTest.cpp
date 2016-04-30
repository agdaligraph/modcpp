#include "org/modcpp/sketching/SpaceSavingSketch.h"

#include "org/modcpp/string/String.h"

using org::modcpp::sketching::SpaceSavingSketch;
using org::modcpp::string::String;

void test_constructor() {
  SpaceSavingSketch<String, uint32_t> sketch1(4);
  SpaceSavingSketch<String, uint64_t> sketch2(12);
}

void test_ensureBucket() {
  SpaceSavingSketch<String, uint32_t> sketch(2);
  sketch.ensureBucket(sketch.usedBuckets.front(), 200);
  assert(sketch.usedBuckets.size() == 1);
  assert(sketch.freeBuckets.size() == 3);
  assert(sketch.usedBuckets.front().count == 200);

  sketch.ensureBucket(sketch.usedBuckets.front(), 200);
  assert(sketch.usedBuckets.size() == 1);
  assert(sketch.freeBuckets.size() == 3);

  sketch.ensureBucket(sketch.usedBuckets.front(), 300);
  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.freeBuckets.size() == 2);

  sketch.ensureBucket(sketch.usedBuckets.front(), 400);
  assert(sketch.usedBuckets.size() == 3);
  assert(sketch.freeBuckets.size() == 1);

  sketch.ensureBucket(sketch.usedBuckets.front(), 500);
  assert(sketch.usedBuckets.size() == 4);
  assert(sketch.freeBuckets.size() == 0);

  sketch.ensureBucket(sketch.usedBuckets.front(), 300);
  assert(sketch.usedBuckets.size() == 4);
  assert(sketch.freeBuckets.size() == 0);  
}

void test_attachToBucket() {
  SpaceSavingSketch<String, uint32_t> sketch(2);
  sketch.items.emplace_back(SpaceSavingSketch<String, uint32_t>::KeyT("key"));

  sketch.ensureBucket(sketch.usedBuckets.front(), 200);
  assert(sketch.items.size() == 1);
  assert(sketch.usedBuckets.front().count == 200);
  sketch.attachToBucket(sketch.items[0], sketch.usedBuckets.front());

  sketch.items.emplace_back(SpaceSavingSketch<String, uint32_t>::KeyT("key2"));
  sketch.attachToBucket(sketch.items[1], sketch.usedBuckets.front());
}

void test_detachFromBucket() {
  SpaceSavingSketch<String, uint32_t> sketch(2);
  sketch.items.emplace_back(SpaceSavingSketch<String, uint32_t>::KeyT("key"));
  sketch.ensureBucket(sketch.usedBuckets.front(), 200);
  sketch.attachToBucket(sketch.items[0], sketch.usedBuckets.front());

  assert(sketch.usedBuckets.size() == 1);
  assert(sketch.freeBuckets.size() == 3);
  sketch.detachFromBucket(sketch.items[0]);
  assert(sketch.usedBuckets.size() == 0);
  assert(sketch.freeBuckets.size() == 4);
}

void test_updateGet() {
  SpaceSavingSketch<String, uint32_t> sketch(2);
  sketch.update("key1", 100);
  sketch.update("key1", 100);
  assert(sketch.usedBuckets.size() == 1);
  assert(sketch.usedBuckets.front().count == 200);
  assert(sketch.get("key1") == 200);

  sketch.update("key2", 100);
  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 100);

  sketch.update("key3", 100);
  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 100);
  assert(sketch.get("key3") == 100);
  assert(sketch.get("key5") == 0);

  sketch.update("key4", 100);
  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.items.size() == 4);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 100);
  assert(sketch.get("key3") == 100);
  assert(sketch.get("key4") == 100);
  assert(sketch.get("key5") == 100); // Guessed

  sketch.update("key2", 100);
  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.items.size() == 4);
  assert(sketch.items.capacity() == 4);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 200);
  assert(sketch.get("key3") == 100);
  assert(sketch.get("key4") == 100);

  sketch.update("key3", 200);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 200);
  assert(sketch.get("key3") == 300);
  assert(sketch.get("key4") == 100);
  assert(sketch.get("key5") == 100); // Guessed

  sketch.update("key5", 99);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 200);
  assert(sketch.get("key3") == 300);
  assert(sketch.get("key4") == 199);
  assert(sketch.get("key5") == 199);
  assert(sketch.usedBuckets.size() == 3);
  assert(sketch.freeBuckets.size() == 1);

  sketch.update("key5", 2);
  assert(sketch.usedBuckets.size() == 3);
  assert(sketch.get("key3") == 300);
  assert(sketch.get("key1") == 200);
  assert(sketch.get("key2") == 200);
  assert(sketch.get("key5") == 201);
  assert(sketch.get("key7") == 200);

  sketch.update("key1", 1);
  sketch.update("key2", 1);
  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.freeBuckets.size() == 2);

  sketch.update("key1", 99);
  sketch.update("key2", 99);
  sketch.update("key5", 99);
  assert(sketch.usedBuckets.size() == 1);

  sketch.update("key1", 1);
  sketch.update("key2", 2);
  sketch.update("key3", 3);
  sketch.update("key5", 7);
  assert(sketch.get("key1") == 301);
  assert(sketch.get("key2") == 302);
  assert(sketch.get("key3") == 303);
  assert(sketch.get("key4") == 301); // Guessed
  assert(sketch.get("key5") == 307);
}

void test_updateGet2() {
  SpaceSavingSketch<String, uint32_t> sketch(1);
  sketch.update("1", 1);
  sketch.update("2", 2);

  assert(sketch.usedBuckets.size() == 2);
  assert(sketch.freeBuckets.size() == 0);
  assert(sketch.items.capacity() == 2);

  for (int i = 3; i < 500; ++i) {
    sketch.update(String::valueOf(i), 2);
    assert(sketch.usedBuckets.size() == 2);
    assert(sketch.items.size() == 2);
    assert(sketch.get(String::valueOf(i - 1)) == (i - 1));
    assert(sketch.get(String::valueOf(i)) == i);
    assert(sketch.get(String::valueOf(i - 2)) == i - 1);
  }
}

void test_updateGet3() {
  SpaceSavingSketch<String, uint32_t> sketch(10);
  for (int i = 1; i <= 1024; ++i) {
    sketch.update(String::valueOf(i), i);
    assert(i == 1024 || (sketch.get(String::valueOf(i + 1)) == 0));
  }
  for (int i = 1; i <= 1024; ++i) {
    assert(sketch.get(String::valueOf(i)) == i);
  }
  sketch.update("1025", 1025);
  assert(sketch.get("1025") == 1026);
}

void test_updateGet4() {
  SpaceSavingSketch<String, uint32_t> sketch(4);
  for (int i = 1; i <= 10; ++i) {
    sketch.update(String::valueOf(i), i);
  }
  sketch.update("1", 9);
  assert(sketch.usedBuckets.size() == 9);
  assert(sketch.get("1") == 10);
}

struct ListType {
  String coord;
  uint32_t count;
  uint32_t maxOvershoot;
};

void test_getTopK() {
  SpaceSavingSketch<String, uint32_t> sketch(2);
  std::vector<ListType> list;
  sketch.update("key1", 1);
  sketch.update("key2", 2);
  sketch.update("key3", 3);
  sketch.update("key4", 4);

  sketch.getTopK(list, 4);
  assert(list[0].coord == "key4");
  assert(list[0].maxOvershoot == 0);
  assert(list[1].coord == "key3");
  assert(list[1].maxOvershoot == 0);
  assert(list[2].coord == "key2");
  assert(list[2].maxOvershoot == 0);
  assert(list[3].coord == "key1");
  assert(list[3].maxOvershoot == 0);

  sketch.update("key5", 100);
  sketch.update("key6", 101);
  sketch.getTopK(list, 4);
  assert(list[0].coord == "key6");
  assert(list[0].maxOvershoot == 2);
  assert(list[0].count == 103);
  assert(list[1].coord == "key5");
  assert(list[1].maxOvershoot == 1);
  assert(list[1].count == 101);
  assert(list[2].coord == "key4");
  assert(list[2].maxOvershoot == 0);
  assert(list[2].count == 4);
  assert(list[3].coord == "key3");
  assert(list[3].maxOvershoot == 0);
  assert(list[3].count == 3);

  sketch.getTopK(list, 2);
  assert(list.size() == 2);

  sketch.getTopK(list, 0);
  assert(list.empty());
}

int main() {
  test_constructor();
  test_ensureBucket();
  test_attachToBucket();
  test_detachFromBucket();
  test_updateGet();
  test_updateGet2();
  test_updateGet3();
  test_updateGet4();
  test_getTopK();
  return 0;
}
