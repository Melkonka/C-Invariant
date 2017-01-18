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
  constexpr T inf_to_min (T value) { return value == inf<T> ? std::numeric_limits<T>::min () : value; }

  template<typename T, T LowerBound, T UpperBound>
  class bound_invariant 
  {
  public:
    
    static constexpr T lower_bound = inf_to_min (LowerBound);
    static constexpr T upper_bound = UpperBound;

    static_assert (!std::is_floating_point_v<T>, "Floating point is not supported yet!");
    static_assert (lower_bound < upper_bound, "Lower bound must be smaller than upper bound!");

    using bound_type = std::conditional_t<lower_bound != std::numeric_limits<T>::min () && upper_bound != std::numeric_limits<T>::max (),
                                          bounded_tag,
                                          std::conditional_t<lower_bound != std::numeric_limits<T>::min (), 
                                                             lower_bounded_tag, 
                                                             upper_bounded_tag>>;

    static bool check (T value) noexcept  { return check_impl (value, bound_type{}); }

  private:
    static bool check_impl (T value, bounded_tag) noexcept       { return value >= lower_bound && value <= upper_bound; }
    static bool check_impl (T value, upper_bounded_tag) noexcept { return value <= upper_bound; }
    static bool check_impl (T value, lower_bounded_tag) noexcept { return value >= lower_bound; }

    
  };
};

#endif