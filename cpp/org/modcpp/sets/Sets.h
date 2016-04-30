#ifndef org_modcpp_sets_Sets
#define org_modcpp_sets_Sets

#include <initializer_list>

namespace org::modcpp::sets {

  class Sets {
   public:
    template <typename T>
    static constexpr bool isIn(const T val, const std::initializer_list<T>& list) {
      for (const auto& item : list) {
        if (val == item)
         return true;
      }
      return false;
    }

    template <typename T>
    static inline const T &firstNonEmpty(const T& first, const T& second) {
      return first.empty() ? second : first;
    }

   private:
    Sets();
  };

}

#endif
