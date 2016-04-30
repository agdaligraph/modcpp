#include "org/modcpp/hashing/KarpRabinHasher.h"

#include <cstdio>

#include "org/modcpp/sketching/SpaceSavingSketch.h"
#include "org/modcpp/sketching/CountSketch.h"

using org::modcpp::hashing::KarpRabinHasher;
using org::modcpp::string::String;
using org::modcpp::sketching::SpaceSavingSketch;
using org::modcpp::sketching::CountSketch;

static const uint32_t bits = 10;
static constexpr uint32_t mask = (1 << bits) - 1;

uint32_t counts[1 << bits];

int main() {
  KarpRabinHasher kr;
  SpaceSavingSketch<String, uint32_t> sk(10);
  CountSketch<String, int32_t> cs(6, 15);

  char buffer[512];
  FILE *f = fopen("input.txt", "r");
  while (fscanf(f, "%s", buffer) > 0) {
    uint32_t value = kr.hash(String(buffer)) & mask;
    sk.update(String::copyOf(buffer), 1);
    cs.update(String::copyOf(buffer), 1);
    ++counts[value];
  }
  fclose(f);
  uint64_t l22 = 0;
  uint64_t l11 = 0;
  uint32_t max = 0;
  int i = 0;
  for (uint32_t count : counts) {
    l22 += count * count;
    l11 += count;
    max = count > max ? count : max;
  }
  printf("\n%llu %llu %llu %d\n", 1 << bits, l11, l22, max);
  return 0;
}
