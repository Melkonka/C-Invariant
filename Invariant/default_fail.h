#ifndef DEFAULT_FAIL_H
#define DEFAULT_FAIL_H

#include <exception>
#include <stdexcept>
#include <type_traits>

namespace inv {

  struct inv_fail : public std::runtime_error
  {
    explicit inv_fail (const char* message) : std::runtime_error (message) {}
  };

  class default_fail 
  {
  public:

    using enabled = std::true_type;

    static void trigger_assert ()
    {
      throw inv_fail ("Invariant check failed");
    }
  };
};

#endif