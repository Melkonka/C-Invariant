#ifndef DEFAULT_FAIL_H
#define DEFAULT_FAIL_H

#include <exception>
#include <stdexcept>

//TODO: gsl like configurable, macro based failing


namespace inv {

  struct inv_fail : public std::runtime_error
  {
    explicit inv_fail (const char* message) : std::runtime_error (message) {}
  };

  template<typename... Invariants>
  class default_fail 
  {
  public:

    template<typename T>
    void check (T value)
    {
      if ((Invariants::check (value) && ...))
        throw inv_fail ("Invariant check failed");
    }
  };
};

#endif