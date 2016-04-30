#include "org/modcpp/string/StringBuilder.h"

#include <cstdarg>

namespace org::modcpp::string {

  /**
   * @brief append the printf style formatted string to the builder
   *
   * TODO(saglam): expand / realloc the buffer automotically here
   */
  StringBuilder &StringBuilder::putFormatted(const char *formatString, ...) {
    return *this;
  }

} // namespace
