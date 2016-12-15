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

  template<typename T>
  constexpr T negate_if_inf (T value) { return value == inf<T> ? -inf<T> : value; }

  template<typename T, T LowerBound, T UpperBound>
  class bound_invariant 
  {
  public:
    static constexpr T lower_bound = negate_if_inf<T> (LowerBound);
    static constexpr T upper_bound = UpperBound;

    static_assert (!std::is_floating_point_v<T>, "Floating point is not supported yet!");
    static_assert (lower_bound < upper_bound, "Lower bound must be smaller than upper bound!");

    using bound_type = std::conditional_t<lower_bound != -inf<T> && upper_bound != inf<T>,
                                          bounded_tag,
                                          std::conditional_t<lower_bound != -inf<T>, 
                                                             lower_bounded_tag, 
                                                             upper_bounded_tag>>;

    static bool check (T value)  { return check_impl (value, bound_type{}); }

  private:
    static bool check_impl (T value, bounded_tag)       { return value >= lower_bound && value <= upper_bound; }
    static bool check_impl (T value, upper_bounded_tag) { return value <= upper_bound; }
    static bool check_impl (T value, lower_bounded_tag) { return value >= lower_bound; }
  };
};

#endif