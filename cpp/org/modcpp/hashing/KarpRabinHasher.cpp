#include "org/modcpp/hashing/KarpRabinHasher.h"

#include "org/modcpp/string/String.h"
#include "org/modcpp/random/HybridRandomStream.h"

namespace org::modcpp::hashing {

  using org::modcpp::string::String;
  using org::modcpp::random::HybridRandomStream;

  static const KarpRabinHasher::FieldType prime = 4294967291;

  KarpRabinHasher::KarpRabinHasher() {
    HybridRandomStream randomSource;
    do {
      random = randomSource.get<FieldType>();
    } while (random >= prime);
    printf("%d\n", random);
    random = 7;
  }

  KarpRabinHasher::FieldType KarpRabinHasher::hash(const String &input) const {
    FieldType output = 0;
    for (char c : input) {
      output = ((output + *((unsigned char*) &c)) * random) % prime;
    }
    return output;
  }

} // namespace
