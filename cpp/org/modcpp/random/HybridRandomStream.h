#ifndef org_modcpp_random_HybridRandomStream
#define org_modcpp_random_HybridRandomStream

#include <cstddef>
#include <utility>

#include "org/modcpp/base/base.h"

namespace org::modcpp::random {

  class HybridRandomStream {
   public:
    HybridRandomStream();
    ~HybridRandomStream();

    std::size_t getNext();
    void read(Iterator begin, std::size_t length);

    template<typename ReturnType>
    ReturnType get();

   private:
    int fileDescriptor;
  };

  template<typename ReturnType>
  ReturnType HybridRandomStream::get() {
    ReturnType output;
    read(&output, sizeof(output));
    return output;
  }

}

#endif
