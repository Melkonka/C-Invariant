#ifndef INVARIANT_HOST_H
#define INVARIANT_HOST_H

#include <type_traits>
#include <tuple>

namespace inv {

//FailPolicy must define:
//1.: a static function trigger_assert() - if any invariant fail it's called: it can call an assert, terminate, throw an exception, debugbreak etc.
//2.: using enabled = std::true_type/false_type. If it's false, the invariants won't be checked. (for example, in release builds the checks may not be desired)
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

  invariant_host (PrimitiveType value) : value (value) {check ();}

  template<PrimitiveType Value>
  invariant_host (std::integral_constant<PrimitiveType, Value>) : value (Value) 
  {
    static_assert (Value >= invariant_1::lower_bound && Value <= invariant_1::upper_bound, "Integral_constant is out of range");
  }

  invariant_host (const type& other) = default;
  type& operator= (const type& other) = default;

  //temporary hack, checks likes this will move into invariant policy classes
  template<typename T, typename = std::enable_if_t<T::invariant_1::lower_bound >= invariant_1::lower_bound && T::invariant_1::upper_bound <= invariant_1::upper_bound>>
  invariant_host (T other) : value (other.get ()) {}

  template<typename T, typename = std::enable_if_t<T::invariant_1::lower_bound >= invariant_1::lower_bound && T::invariant_1::upper_bound <= invariant_1::upper_bound>>
  type& operator= (T other)
  {
    value = other.get ();
    return *this;
  }

  template<PrimitiveType Value>
  type& operator= (std::integral_constant<PrimitiveType, Value>)
  {
    static_assert (Value >= invariant_1::lower_bound && Value <= invariant_1::upper_bound, "Integral_constant is out of range");
    value = Value;
    return *this;
  }

  type& operator= (PrimitiveType other) 
  {
    value = other;
    check ();
    return *this;
  } 

   type& operator++ () 
   {
     ++value;
     check ();
     return *this;
   }

   type& operator-- ()
   {
     --value;
     check ();
     return *this;
   }

   type operator++ (int)
   {
     type tmp (*this);
     operator++ ();
     return tmp;
   }

   type operator-- (int)
   {
     type tmp (*this);
     operator-- ();
     return tmp;
   }

   template<typename T>
   type& operator+= (T other)
   {
     value += other;
     check ();
     return *this;
   }

   template<typename T>
   type& operator-= (T other)
   {
     value -= other;
     check ();
     return *this;
   }

   template<typename T>
   type& operator*= (T other)
   {
     value *= other;
     check ();
     return *this;
   }

   template<typename T>
   type& operator/= (T other)
   {
     value /= other;
     check ();
     return *this;
   }

   PrimitiveType get () const noexcept { return value; }
   operator PrimitiveType () const noexcept { return value; }

private:
  PrimitiveType value;

  void check () { check_impl (typename FailPolicy::enabled ()); }
  
  void check_impl (std::true_type) const
  {
    if (!(Invariants::check (value) && ...))
      FailPolicy::trigger_assert ();
  }

  void check_impl (std::false_type) const noexcept {}
};
}

#endif