#pragma once

#include "macros.h"

using uint = unsigned;
using uchar = unsigned char;
using cstr = char const*;
using llong = long long;
using ssize_t = int;

namespace lib {
  
  using iid_t = unsigned;
  using oid_t = unsigned;

  TP<TN T0> struct type_tag {};

  struct empty_base { };

  struct nocopy { 
    nocopy() { } 
    nocopy( nocopy const& ) = delete; 
    nocopy& operator=( nocopy const& ) = delete;
    nocopy( nocopy&& ) = default;
    nocopy& operator=( nocopy&& ) = default;
  };


  struct type_true { constexpr static int value = 1; };
  struct type_false { constexpr static int value = 0; };


  TP<TN...> struct type_first;
  TP<TN T0, TN... TT> struct type_first< T0, TT... > { using type = T0; };


  TP<TN T0> T0 declval();

  TP<TN...> using void_t = void;

  TP<TN T0> struct identity { using type = T0; };


  TP<bool N0, TN T1, TN T2>
  struct select { using type = T2; };

  TP<TN T1, TN T2>
  struct select< true, T1, T2 > { using type = T1; };

  TP<bool N0, TN T1, TN T2>
  using select_t = typename select< N0, T1, T2 >::type;


  TP<bool N0, TN T0 = void>
  struct enable_if { };

  TP<TN T0>
  struct enable_if< true, T0 > { using type = T0; };

  TP<bool N0, TN T0 = void>
  using enable_if_t = TN enable_if< N0, T0 >::type;

  TP<bool N0, TN T0 = void>
  struct disable_if { };

  TP<TN T0>
  struct disable_if< false, T0 > { using type = T0; };

  TP<bool N0, TN T0 = void>
  using disable_if_t = TN disable_if< N0, T0 >::type;

  TP<TN T0> struct is_array : type_false { };
  TP<TN T0, ssize_t N0> struct is_array< T0[N0] > : type_true { };
  TP<TN T0>
  constexpr bool is_array_v = is_array< T0 >::value;


  TP<TN T0> struct is_ref : type_false { };
  TP<TN T0> struct is_ref< T0& > : type_true { };
  TP<TN T0> struct is_ref< T0&& > : type_true { };
  TP<TN T0> 
  constexpr bool is_ref_v = is_ref< T0 >::value;

  TP<TN T0> struct is_ptr : type_false { };
  TP<TN T0> struct is_ptr< T0* > : type_true { };
  TP<TN T0> 
  constexpr bool is_ptr_v = is_ptr< T0 >::value;

  TP<TN T0> struct no_ptr { using type = T0; };
  TP<TN T0> struct no_ptr< T0* > { using type = T0; };
  TP<TN T0> 
  using no_ptr_t = typename no_ptr< T0 >::type;

  TP<TN T0> struct no_ref { using type = T0; };
  TP<TN T0> struct no_ref< T0& > { using type = T0; };
  TP<TN T0> struct no_ref< T0&& > { using type = T0; };
  TP<TN T0> 
  using no_ref_t = typename no_ref< T0 >::type;

  TP<TN T0> struct no_const { using type = T0; };
  TP<TN T0> struct no_const< T0 const > { using type = T0; };
  TP<TN T0>
  using no_const_t = typename no_const< T0 >::type;

  TP<TN T0>
  using no_cref_t = no_const_t< no_ref_t< T0 > >;

  TP<TN T0> struct is_const : type_false { };
  TP<TN T0> struct is_const< const T0 > : type_true { };
  TP<TN T0> 
  constexpr bool is_const_v = is_const< T0 >::value;

  TP<TN T0> 
  constexpr bool is_cref_v = is_const_v< no_ref_t< T0 > >;

  TP<TN T0> 
  constexpr bool is_cptr_v = is_const_v< no_ptr_t< T0 > >;


  TP<TN> struct is_primitive : type_false { };

  TP<> struct is_primitive< int > : type_true { };
  TP<> struct is_primitive< uint > : type_true { };
  TP<> struct is_primitive< char > : type_true { };
  TP<> struct is_primitive< bool > : type_true { };
  TP<> struct is_primitive< uchar > : type_true { };
  TP<> struct is_primitive< float > : type_true { };
  TP<> struct is_primitive< double > : type_true { };
  TP<> struct is_primitive< long > : type_true { };
  TP<> struct is_primitive< long long > : type_true { };

  TP<TN T0>
  constexpr bool is_primitive_v = is_primitive< no_const_t< no_ptr_t< T0 > > >::value;


  TP<TN T0, ssize_t N0> T0* begin( T0 ( &data)[ N0 ] ) { return data; }
  TP<TN T0, ssize_t N0> T0  end  ( T0 ( &data)[ N0 ] ) { return &data[ N0 ]; }

  TP<TN T0, typename T0::iterator( T0::*)() = &T0::begin > 
  typename T0::iterator begin( T0& data ) { return data.begin(); }

  TP<TN T0, TN T0::iterator( T0::*)() = &T0::end   > 
  typename T0::iterator end  ( T0& data ) { return data.end(); }

  TP<TN T0, TN T0::iterator( T0::*)() = &T0::begin > 
  typename T0::const_iterator begin( T0 const& data ) { return data.begin(); }

  TP<TN T0, TN T0::iterator( T0::*)() = &T0::end   > 
  typename T0::const_iterator end  ( T0 const& data ) { return data.end(); }


  TP<TN T0, TN = enable_if_t< is_ref_v< T0 > >> 
  no_ref_t< T0 >&& move( T0&& value ) { 

    return static_cast< no_ref_t< T0 >&& > ( value ); 
  }

  TP<TN T0, TN = enable_if_t< is_primitive_v< T0 > >>
  T0 move( T0& value ) { 
    
    auto value_orig = value; value = T0{}; return value_orig; 
  }

  TP<TN T0> 
  T0* move( T0*& ptr ) { 
    
    auto ptr_orig = ptr; ptr = nullptr; return ptr_orig; 
  }

 
  TP<TN T0> 
  typename select< is_ref_v< T0 >, T0, T0&& >::type 
  forward( no_ref_t< T0 >& value ) { 

    return static_cast< typename select< is_ref_v< T0 >, T0, T0&& >::type >( value ); 
  } 


  TP<TN T0, TN = void, TN = void, TN = void, TN = void>
  struct is_container : type_false { };

  TP<TN T0>
  struct is_container< T0,  disable_if_t< is_primitive_v< T0 > >,
                            disable_if_t< is_array_v< T0 > >,
                            decltype( (void) begin( declval< T0& >() ) ), 
                            decltype( (void) end( declval< T0& >() ) ) > : type_true { };
  TP<TN T0> 
  constexpr bool is_container_v = is_container< T0 >::value;



  TP<TN>
  struct is_template : type_false { };

  TP< TP1<TN...> class T0, TN... TT >
  struct is_template< T0< TT... > > : type_true { using type = TN type_first< TT... >::type; };

  TP<TN T0>
  using template_arg_t = TN is_template< T0 >::type;


}


