#ifndef INVARIANT_HOST_H
#define INVARIANT_HOST_H

#include <type_traits>
#include <tuple>

namespace inv {
//TODO LIST:
//- create operator+(invariant) and check if we surely keep the invariant without recheck
//Do this with every operator
//- Write a lightweight typelist to make optimizations easier
//- instead of using type to optimize away checks, call a metafunction if the other's invariants are more strict (e.g bounded<0, 1> is garantueed to be bounded<0, 255>);
//- bitwise operators

//FailPolicy must define a static function check (PrimitiveType).
//Invariants is a Template parameter pack (Invariants), which also has to define a static function check (PrimitiveType). //TODO: better naming
//An Invariant's responsibility is to check the invariant, while FailPolicy handle the errors.
//If FailPolicy doesn't have a check function it's basically equivalent to simple primitive types without any check. (e.g.: release build)
template<typename PrimitiveType, template<typename...> class FailPolicy, typename... Invariants>
class invariant_host
{
  static_assert (!std::is_same_v<PrimitiveType, bool> &&
                (std::is_integral_v<PrimitiveType> || std::is_floating_point_v<PrimitiveType>), 
                 "Template parameter 1 must be a non-bool primitive type");
  static_assert (sizeof...(Invariants) == 1, "Multiple invariants are not supported yet");
public:
  using checker = FailPolicy<Invariants...>;
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
    checker::check (value);
  } 

   operator PrimitiveType () const {return value;}

   type& operator++() 
   {
     ++value;
     checker::check (value);
     return this;
   }

   type& operator--()
   {
     --value;
     checker::check (value);
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
     checker::check (value);
     return *this;
   }

   type operator- (PrimitiveType other)
   {
     return type (value - other);
   }

   type& operator-= (PrimitiveType other)
   {
     value -= other;
     checker::check (value);
     return *this;
   }

   type operator* (PrimitiveType other)
   {
     return type(value * other);
   }

   type& operator*= (PrimitiveType other)
   {
     value *= other;
     checker::check (value);
     return *this;
   }

   type operator/ (PrimitiveType other)
   {
     return type (value / other);
   }

   type& operator/= (PrimitiveType other)
   {
     value /= other;
     checker::check (value);
     return *this;
   }

   friend bool operator< (type lhs, PrimitiveType rhs) { return lhs.value < rhs; }
   friend bool operator> (type lhs, PrimitiveType rhs) { return rhs < lhs; }
   friend bool operator<=(type lhs, PrimitiveType rhs) { return !(lhs > rhs); }
   friend bool operator>=(type lhs, PrimitiveType rhs) { return !(lhs < rhs); }
   friend bool operator==(type lhs, PrimitiveType rhs) { return lhs.value == rhs; }
   friend bool operator!=(type lhs, PrimitiveType rhs) { return !(lhs == rhs); }

   friend bool operator< (PrimitiveType lhs, type rhs) { return lhs < rhs.value; }
   friend bool operator> (PrimitiveType lhs, type rhs) { return rhs < lhs; }
   friend bool operator<=(PrimitiveType lhs, type rhs) { return !(lhs > rhs); }
   friend bool operator>=(PrimitiveType lhs, type rhs) { return !(lhs < rhs); }
   friend bool operator==(PrimitiveType lhs, type rhs) { return lhs == rhs.value; }
   friend bool operator!=(PrimitiveType lhs, type rhs) { return !(lhs == rhs); }

   PrimitiveType get () const {return value;}
private:
  PrimitiveType value;
};


}

#endif