#ifndef org_modcpp_hashing_KarpRabinHasher
#define org_modcpp_hashing_KarpRabinHasher

#include "org/modcpp/string/String.h"

namespace org {
namespace modcpp {
namespace hashing {
  using org::modcpp::string::String;
  
  class KarpRabinHasher {
   public:
    typedef uint32_t FieldType;

   public:
    KarpRabinHasher(/* InputStream randomStream */);
    
    FieldType hash(const String &input) const;
  
   private:
    FieldType random;
  };

}}} // namespace

#endif
