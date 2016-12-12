#ifndef INVARIANTS_H
#define INVARIANTS_H

#include "invariant_host.h"
#include "bound_invariants.h"
#include "default_fail.h"

namespace inv {

  template<typename T>
  using positive = invariant_host<T, default_fail, bound_invariant<T, 0, inf<T>>>;

  using positive_s = invariant_host<short, default_fail, bound_invariant<short, 0, inf<short>>>;
  using positive_i = invariant_host<int, default_fail, bound_invariant<int, 0, inf<int>>>;
  using positive_l = invariant_host<long, default_fail, bound_invariant<long, 0, inf<long>>>;

  template<typename T>
  using negative = invariant_host<T, default_fail, bound_invariant<T, inf<T>, 0>>;

  using negative_s = invariant_host<short, default_fail, bound_invariant<short, inf<short>, 0>>;
  using negative_i = invariant_host<int, default_fail, bound_invariant<int, inf<int>, 0>>;
  using negative_l = invariant_host<long, default_fail, bound_invariant<long, inf<long>, 0>>;

  template<typename T, T LowerBound, T UpperBound>
  using bounded = invariant_host<T, default_fail, bound_invariant<T, LowerBound, UpperBound>>;

  template<short LowerBound, short UpperBound>
  using bounded_s = invariant_host<short, default_fail, bound_invariant<short, LowerBound, UpperBound>>;

  template<int LowerBound, int UpperBound>
  using bounded_i = invariant_host<int, default_fail, bound_invariant<int, LowerBound, UpperBound>>;

  template<long LowerBound, long UpperBound>
  using bounded_l = invariant_host<long, default_fail, bound_invariant<long, LowerBound, UpperBound>>;
};

#endif