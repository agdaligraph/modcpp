#ifndef org_modcpp_hashing_KWiseHasher
#define org_modcpp_hashing_KWiseHasher

#include <array>
#include <type_traits>

#include "org/modcpp/base/base.h"
#include "org/modcpp/hashing/Md5Hasher.h"
#include "org/modcpp/random/HybridRandomStream.h"
#include "org/modcpp/string/String.h"

namespace org {
namespace modcpp {
namespace hashing {
  using org::modcpp::string::String;

  template<typename DomainT, typename RangeT, int StrengthK>
  class KWiseHasher {
    typedef typename std::conditional<std::is_arithmetic<DomainT>::value,
        typename std::conditional<(sizeof(DomainT) > sizeof(RangeT)), DomainT, RangeT>::type,
        typename std::conditional<(sizeof(RangeT) > sizeof(uint)), RangeT, uint>::type>::type
            CoeffT;

   public:
    KWiseHasher();
    ~KWiseHasher() = default;

    RangeT hash(const DomainT input);

   private:
    std::array<CoeffT, StrengthK - 1> coeffs;
    CoeffT constCoeff;

    template<typename DomainU = DomainT>
    typename std::enable_if<std::is_same<DomainU, String>::value, CoeffT>::type 
        domainTypeToCoeffType(const DomainU input) {
      Md5Hasher md5Hasher;
      md5Hasher.put((const unsigned char*) coeffs.data(), StrengthK * sizeof(CoeffT));
      return md5Hasher.hash<CoeffT>(md5Hasher.hash());
    }
    
    template<typename DomainU = DomainT>
    typename std::enable_if<std::is_arithmetic<DomainU>::value, CoeffT>::type
        domainTypeToCoeffType(const DomainU input) {
      return input;
    }
  };

  template<typename DomainT, typename RangeT, int StrengthK>
  KWiseHasher<DomainT, RangeT, StrengthK>::KWiseHasher() {
    // TODO(saglam): Add random source as an InputStream paramater
    org::modcpp::random::HybridRandomStream randomness;
    randomness.read(coeffs.data(), (StrengthK - 1) * sizeof(CoeffT));
    randomness.read(&constCoeff, sizeof(constCoeff));
  }

  template<typename DomainT, typename RangeT, int StrengthK>
  RangeT KWiseHasher<DomainT, RangeT, StrengthK>::hash(const DomainT input) {
    static_assert(sizeof(RangeT) <= sizeof(CoeffT),
        "KWiseHasher: RangeT should be larger than CoeffT.");

    CoeffT sum = 0;
    CoeffT x = domainTypeToCoeffType(input);

    for (const CoeffT coeff : coeffs) {
      sum = (sum + coeff) * x;
    }
    sum += constCoeff;
    return *reinterpret_cast<RangeT*>(&sum);
  }

}}} // namespace

#endif
