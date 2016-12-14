
#include "TestFramework\utility.h"
#include "tests.h"
#include "invariants.h"

using namespace inv;
using namespace inv_test;

namespace {
  void constructor_test()
  {
    positive_i (0);
    EXPECT_NO_THROW (positive_i (1));
    EXPECT_NO_THROW (positive_i (inf<int>));
    EXPECT_NO_THROW (positive_i (100));

    EXPECT_THROW (positive_i (-inf<int>));
    EXPECT_THROW (positive_i (-1));
    EXPECT_THROW (positive_i (-100));
  }

  void copy_constructor_test ()
  {
    positive_i pos (0);
    EXPECT_NO_THROW (positive_i (pos));
  }

  void assignment_test ()
  {
    positive_i pos (0);
    positive_i pos2 (inf<int>);
    EXPECT_NO_THROW (pos = pos2);
    EXPECT_NO_THROW (pos = 0);
    EXPECT_NO_THROW (pos = 100);
    EXPECT_NO_THROW (pos = inf<int>);

    EXPECT_THROW (pos = -1);
    EXPECT_THROW (pos = -inf<int>);
  }

  void decrement_increment_test () 
  {
    negative_l neg (-1);
    EXPECT_NO_THROW (++neg);
    EXPECT_THROW (++neg);
    EXPECT_THROW (++neg);
    check (neg == 2);

    EXPECT_THROW (--neg);
    EXPECT_NO_THROW (neg--);
    EXPECT_NO_THROW (neg--);
    check (neg == -1);

    EXPECT_NO_THROW (neg++);
    EXPECT_THROW (neg++);
    check (neg == 1);

    EXPECT_NO_THROW (neg--);
    EXPECT_NO_THROW (neg--);
    check (neg == -1);
  }

  void arithmetic_operator_test () 
  { 
    //none of these operators are overloaded (so basically it tests implicit conversion)
    //these tests are here because if these are not working it's a serious problem
    bounded<short, 0, 100> bounded_0_100 (10);

    EXPECT_NO_THROW (check (bounded_0_100 + bounded_0_100 == 20));
    EXPECT_NO_THROW (check (bounded_0_100 + 10 == 20));
    EXPECT_NO_THROW (check (10 + bounded_0_100 == 20));
    EXPECT_NO_THROW (check (bounded_0_100 + 1000 == 1010));
    EXPECT_NO_THROW (check (1000 + bounded_0_100 == 1010));
    
    EXPECT_NO_THROW (check (bounded_0_100 - bounded_0_100 == 0));
    EXPECT_NO_THROW (check (bounded_0_100 - 10 == 0));
    EXPECT_NO_THROW (check (10 - bounded_0_100 == 0));
    EXPECT_NO_THROW (check (bounded_0_100 - 1000 == -990));
    EXPECT_NO_THROW (check (1000 - bounded_0_100 == 990));

    EXPECT_NO_THROW (check (bounded_0_100 * bounded_0_100 == 100));
    EXPECT_NO_THROW (check (bounded_0_100 * 10 == 100));
    EXPECT_NO_THROW (check (10 * bounded_0_100 == 100));
    EXPECT_NO_THROW (check (bounded_0_100 * 1000 == 10000));
    EXPECT_NO_THROW (check (1000 * bounded_0_100 == 10000));

    EXPECT_NO_THROW (check (bounded_0_100 / bounded_0_100 == 1));
    EXPECT_NO_THROW (check (bounded_0_100 / 10 == 1));
    EXPECT_NO_THROW (check (10 / bounded_0_100 == 1));
  }

  void compound_assignment_operator_test ()
  {
    bounded<short, 10, 100> bounded_10_100 (50);

    EXPECT_NO_THROW (bounded_10_100 += 50);
    EXPECT_THROW (bounded_10_100 += 100);
    check (bounded_10_100 == 200);

    EXPECT_NO_THROW (bounded_10_100 -= 190);
    EXPECT_THROW (bounded_10_100 -= 5);
    check (bounded_10_100 == 5);

    EXPECT_NO_THROW (bounded_10_100 *= 10);
    EXPECT_THROW (bounded_10_100 *= 3);
    check (bounded_10_100 == 150);

    EXPECT_NO_THROW (bounded_10_100 /= 10);
    EXPECT_THROW (bounded_10_100 /= 3);
    check (bounded_10_100 == 5);
  }
}

namespace inv_test {



  void invariant_host_tests ()
  {
    constructor_test ();
    copy_constructor_test ();
    assignment_test();
    decrement_increment_test ();
    arithmetic_operator_test ();
    compound_assignment_operator_test ();
  }
};                  