#ifndef TEST_UTILITY_H
#define TEST_UTILITY_H

#include <assert.h>

namespace inv_test {

#define EXPECT_THROW(function) \
    {                                   \
      bool exception_thrown = false; \
      try                           \
      {                             \
        (function);                   \
      }                             \
      catch (...)                   \
      {                             \
        exception_thrown = true;    \
      }                             \
      assert(exception_thrown && "exception should be thrown by\n"); \
    }

#define EXPECT_NO_THROW(function) \
  {                                   \
    bool exception_thrown = false; \
      try                           \
    {                             \
      (function);                   \
    }                             \
      catch (...)                   \
    {                             \
      exception_thrown = true;    \
    }                             \
    assert (!exception_thrown && "exception should be thrown by\n"); \
}

  inline void check (bool b) {assert (b);}
  inline void check_fail (bool b) {assert (!b);}
};                        

#endif