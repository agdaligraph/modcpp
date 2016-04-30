#ifndef org_modcpp_base_IteratorRange
#define org_modcpp_base_IteratorRange

namespace org::modcpp::base {

  template<typename IteratorT>
  class IteratorRange {
   public:
    constexpr IteratorRange(const IteratorT first, const IteratorT last)
        : first(first), last(last) {}

    IteratorT begin() const { return first; }
    IteratorT end() const { return last; }

   private:
    const IteratorT first;
    const IteratorT last;
  };

}

#endif
