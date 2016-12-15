
#include "TestFramework\utility.h"
#include "tests.h"
#include "bound_invariants.h"

namespace {
  using namespace inv_test;

  void upper_bound_invariant_test()
  {
    using upper_bound_255 = inv::bound_invariant<int, inv::inf<int>, 255>;

    check (upper_bound_255::check (0));
    check (upper_bound_255::check (-inv::inf<int>));
    check (upper_bound_255::check (255));

    check_fail (upper_bound_255::check (256));
    check_fail (upper_bound_255::check (inv::inf<int>));
  }

  void lower_bound_invariant_test()
  {
    using lower_bound_0 = inv::bound_invariant<int, 0, inv::inf<int>>;
    check (lower_bound_0::check (0));
    check (lower_bound_0::check (255));
    check (lower_bound_0::check (256));
    check (lower_bound_0::check (inv::inf<int>));

    check_fail (lower_bound_0::check (-1));
    check_fail (lower_bound_0::check (-inv::inf<int>));
  }

  void bound_invariant_test()
  {
    using bounded_0_255 = inv::bound_invariant<int, 0, 255>;
    check (bounded_0_255::check (0));
    check (bounded_0_255::check (255));

    check_fail (bounded_0_255::check (-1));
    check_fail (bounded_0_255::check (-inv::inf<int>));
    check_fail (bounded_0_255::check (256));
    check_fail (bounded_0_255::check (inv::inf<int>));
  }
}

namespace inv_test {

  void bound_invariant_tests()
  {
    upper_bound_invariant_test ();
    lower_bound_invariant_test ();
    bound_invariant_test ();
  }
};                  