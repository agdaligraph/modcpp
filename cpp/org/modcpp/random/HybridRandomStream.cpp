#include "org/modcpp/random/HybridRandomStream.h"

#include <fcntl.h>
#include <unistd.h>

namespace org::modcpp::random {

  HybridRandomStream::HybridRandomStream()
      : fileDescriptor(::open("/dev/random", O_RDONLY, 0)) {}

  HybridRandomStream::~HybridRandomStream() {
    ::close(fileDescriptor);
  }

  std::size_t HybridRandomStream::getNext() {
    std::size_t value;
    ::read(fileDescriptor, &value, sizeof(value));
    return value;
  }

  void HybridRandomStream::read(Iterator begin, std::size_t length) {
    ::read(fileDescriptor, begin, length);
  }

}
