
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
    positive_i pos (2);
    EXPECT_NO_THROW (positive_i (pos));
    EXPECT_THROW    (negative_i (pos));

    bounded_i<0, 50> bounded_0_50 (50);
    bounded_i<0, 100> bounded_0_100 (bounded_0_50); //Expect no check
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

    bounded_i<0, 100> bounded_0_100 (50);
    bounded_i<0, 50>  bounded_0_50 (20);
    bounded_0_100 = bounded_0_50; //Expect no check
  }

  void decrement_increment_test () 
  {
    negative_l neg (-1);
    EXPECT_NO_THROW (++neg);
    EXPECT_THROW    (++neg);
    EXPECT_THROW    (++neg);
    check           (neg == 2);

    EXPECT_THROW    (--neg);
    EXPECT_NO_THROW (neg--);
    EXPECT_NO_THROW (neg--);
    check           (neg == -1);

    EXPECT_NO_THROW (neg++);
    EXPECT_THROW    (neg++);
    check           (neg == 1);

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
    {
      lower_bounded_l<10> lower_bounded_l_10 (50);

      EXPECT_NO_THROW (lower_bounded_l_10 -= 40);
      EXPECT_THROW    (lower_bounded_l_10 -= 10);
      check           (lower_bounded_l_10 == 0);

      EXPECT_THROW    (lower_bounded_l_10 += 5);
      EXPECT_NO_THROW (lower_bounded_l_10 += 45);
      check           (lower_bounded_l_10 == 50);

      EXPECT_NO_THROW (lower_bounded_l_10 /= 5);
      EXPECT_THROW    (lower_bounded_l_10 /= 2);
      check           (lower_bounded_l_10 == 5);

      EXPECT_THROW    (lower_bounded_l_10 *= 1);
      EXPECT_NO_THROW (lower_bounded_l_10 *= 10);
      check           (lower_bounded_l_10 == 50);     
    }

    {
      upper_bounded<short, 100> upper_bounded_100 (50);

      EXPECT_NO_THROW (upper_bounded_100 += 50);
      EXPECT_THROW    (upper_bounded_100 += 100);
      check           (upper_bounded_100 == 200);

      EXPECT_THROW    (upper_bounded_100 -= 50);
      EXPECT_NO_THROW (upper_bounded_100 -= 100);
      check           (upper_bounded_100 == 50);

      EXPECT_NO_THROW (upper_bounded_100 *= 2);
      EXPECT_THROW    (upper_bounded_100 *= 3);
      check           (upper_bounded_100 == 300);

      EXPECT_THROW    (upper_bounded_100 /= 2);
      EXPECT_NO_THROW (upper_bounded_100 /= 3);
      check           (upper_bounded_100 == 50);
    }

    {
      bounded<short, 10, 100> bounded_10_100 (50);

      EXPECT_NO_THROW (bounded_10_100 += 50);
      EXPECT_THROW    (bounded_10_100 += 100);
      check           (bounded_10_100 == 200);

      EXPECT_NO_THROW (bounded_10_100 -= 190);
      EXPECT_THROW    (bounded_10_100 -= 5);
      check           (bounded_10_100 == 5);

      EXPECT_NO_THROW (bounded_10_100 *= 10);
      EXPECT_THROW    (bounded_10_100 *= 3);
      check           (bounded_10_100 == 150);

      EXPECT_NO_THROW (bounded_10_100 /= 10);
      EXPECT_THROW    (bounded_10_100 /= 3);
      check           (bounded_10_100 == 5);
    }
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