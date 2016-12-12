#ifndef BOUND_INVARIANTS_H
#define BOUND_INVARIANTS_H

#include <type_traits>
#include <limits>

//TODO: Somehow eliminate upper/lower bound stuff, these just makes things more complicated

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
  public:
    static constexpr T lower_bound = LowerBound;
    static constexpr T upper_bound = UpperBound;
    using bound_type = std::conditional_t<lower_bound != inf<T> && upper_bound != inf<T>,
                                          bounded_tag,
                                          std::conditional_t<lower_bound != inf<T>, 
                                                             lower_bounded_tag, 
                                                             upper_bounded_tag>>;

    std::enable_if<std::is_same_v<bounded_tag, bound_type>, bool>
    check (T value)
    {
      return value >= lower_bound && value <= upper_bound;
    }

    std::enable_if<std::is_same_v<upper_bounded_tag, bound_type>, bool>
    check (T value)
    {
      return value <= upper_bound;
    }

    std::enable_if<std::is_same_v<lower_bounded_tag, bound_type>, bool>
    check (T value)
    {
      return value >= lower_bound;
    }
  };
};

#endif