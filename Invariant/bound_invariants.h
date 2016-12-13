#ifndef BOUND_INVARIANTS_H
#define BOUND_INVARIANTS_H

#include <type_traits>
#include <limits>


namespace inv {

  class lower_bounded_tag {};
  class upper_bounded_tag {};
  class bounded_tag {};

  template<typename T>
  constexpr T inf = std::numeric_limits<T>::max ();

  template<typename T, T LowerBound, T UpperBound>
  class bound_invariant 
  {
    static_assert (!std::is_floating_point_v<T>, "Floating point is not supported yet, come back in a few month!");
    static_assert (LowerBound < UpperBound || LowerBound == inf<T>, "Lower bound must be smaller than upper bound!");
  public:
    static constexpr T lower_bound = LowerBound;
    static constexpr T upper_bound = UpperBound;
    using bound_type = std::conditional_t<lower_bound != inf<T> && upper_bound != inf<T>,
                                          bounded_tag,
                                          std::conditional_t<lower_bound != inf<T>, 
                                                             lower_bounded_tag, 
                                                             upper_bounded_tag>>;

    static bool check (T value) 
    {
      return check_impl (value, bound_type{});
    }

  private:
    static bool check_impl (T value, bounded_tag)
    {
      return value >= lower_bound && value <= upper_bound;
    }

    static bool check_impl (T value, upper_bounded_tag)
    {
      return value <= upper_bound;
    }

    static bool check_impl (T value, lower_bounded_tag)
    {
      return value >= lower_bound;
    }
  };
};

#endif