#ifndef DEFAULT_FAIL_H
#define DEFAULT_FAIL_H

#include <exception>
#include <stdexcept>

namespace inv {

  struct inv_fail : public std::runtime_error
  {
    explicit inv_fail (const char* message) : std::runtime_error (message) {}
  };

  class default_fail 
  {
  public:

    static constexpr bool enabled = true;

    static void assert ()
    {
      throw inv_fail ("Invariant check failed");
    }
  };
};

#endif