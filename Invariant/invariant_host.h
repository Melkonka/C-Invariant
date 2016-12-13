#ifndef INVARIANT_HOST_H
#define INVARIANT_HOST_H

#include <type_traits>
#include <tuple>

namespace inv {

//FailPolicy must define:
//1.: a static function assert() - that's we fail, it can call an assert, terminate, throw an exception, debugbreak etc.
//2.: constexpr static bool enabled. If it's false, the invariants checks doesn't happen. (for example, in release builds the checks may not be desired)
// 
//Invariants is a Template parameter pack, which has to define a static function check (PrimitiveType).
//An Invariant's responsibility is to check the invariant, while FailPolicy handle the errors.
template<typename PrimitiveType, typename FailPolicy, typename... Invariants>
class invariant_host
{
  static_assert (!std::is_same_v<PrimitiveType, bool> &&
                (std::is_integral_v<PrimitiveType> || std::is_floating_point_v<PrimitiveType>), 
                 "Template parameter 1 must be a non-bool primitive type");
  static_assert (sizeof...(Invariants) == 1, "Multiple invariants are not supported yet");
public:
  using type = invariant_host<PrimitiveType, FailPolicy, Invariants...>;

  //Info: Currently only 1 invariant is supported for simplicity, this is a temporary solution
  using invariant_1 = std::tuple_element_t<0, std::tuple<Invariants...>>;

  //using invariants = type_list<Invariants...>;

  static constexpr int invariant_size = sizeof...(Invariants);

  invariant_host (PrimitiveType value) : value (value) {checker::check (value);}

  invariant_host (const type& other) = default;
  type& operator= (const type& other) = default;

  template<typename T>
  type& operator= (const T& other) 
  {
    value = other;
    check (value);
  } 

   operator PrimitiveType () const {return value;}

   type& operator++() 
   {
     ++value;
     check (value);
     return this;
   }

   type& operator--()
   {
     --value;
     check (value);
     return *this;
   }

   type operator++(int)
   {
     type tmp (*this);
     operator++ ();
     return tmp;
   }

   type operator--(int)
   {
     PrimitiveType tmp = value;
     operator-- ();
     return tmp;
   }

   type operator+ (PrimitiveType other)
   {
     return type (value + other);
   }

   type& operator+= (PrimitiveType other)
   {
     value += other;
     check (value);
     return *this;
   }

   type operator- (PrimitiveType other)
   {
     return type (value - other);
   }

   type& operator-= (PrimitiveType other)
   {
     value -= other;
     check (value);
     return *this;
   }

   type operator* (PrimitiveType other)
   {
     return type(value * other);
   }

   type& operator*= (PrimitiveType other)
   {
     value *= other;
     check (value);
     return *this;
   }

   type operator/ (PrimitiveType other)
   {
     return type (value / other);
   }

   type& operator/= (PrimitiveType other)
   {
     value /= other;
     check (value);
     return *this;
   }

   friend bool operator< (type lhs, PrimitiveType rhs) noexcept { return lhs.value < rhs; }
   friend bool operator> (type lhs, PrimitiveType rhs) noexcept { return rhs < lhs; }
   friend bool operator<=(type lhs, PrimitiveType rhs) noexcept { return !(lhs > rhs); }
   friend bool operator>=(type lhs, PrimitiveType rhs) noexcept { return !(lhs < rhs); }
   friend bool operator==(type lhs, PrimitiveType rhs) noexcept { return lhs.value == rhs; }
   friend bool operator!=(type lhs, PrimitiveType rhs) noexcept { return !(lhs == rhs); }

   friend bool operator< (PrimitiveType lhs, type rhs) noexcept { return lhs < rhs.value; }
   friend bool operator> (PrimitiveType lhs, type rhs) noexcept { return rhs < lhs; }
   friend bool operator<=(PrimitiveType lhs, type rhs) noexcept { return !(lhs > rhs); }
   friend bool operator>=(PrimitiveType lhs, type rhs) noexcept { return !(lhs < rhs); }
   friend bool operator==(PrimitiveType lhs, type rhs) noexcept { return lhs == rhs.value; }
   friend bool operator!=(PrimitiveType lhs, type rhs) noexcept { return !(lhs == rhs); }

   PrimitiveType get () const noexcept { return value; }

private:
  PrimitiveType value;

  std::enable_if_t<FailPolicy::enabled>
  check () const
  {
    if (!(Invariants::check(value) && ...))
      FailPolicy::assert ();
  }

  std::enable_if_t<!FailPolicy::enabled>
  check () const noexcept {}
};


}

#endif