#ifndef INVARIANT_HOST_H
#define INVARIANT_HOST_H

#include <type_traits>
#include <tuple>

namespace inv {

//FailPolicy must define:
//1.: a static function assert() - that's we fail, it can call an assert, terminate, throw an exception, debugbreak etc.
//2.: using enabled = std::true_type/false_type. If it's false, the invariants checks doesn't happen. (for example, in release builds the checks may not be desired)
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

  invariant_host (const type& other) = default;
  type& operator= (const type& other) = default;

  type& operator= (PrimitiveType other) 
  {
    value = other;
    check ();
    return *this;
  } 

   operator PrimitiveType () const {return value;}

   type& operator++() 
   {
     ++value;
     check ();
     return *this;
   }

   type& operator--()
   {
     --value;
     check ();
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