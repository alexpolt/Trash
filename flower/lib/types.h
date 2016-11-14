#pragma once

#include "macros.h"

using uint = unsigned;
using uchar = unsigned char;
using cstr = char const*;
using byte = unsigned char;
using llong = long long;
using usize_t = uint;
using ssize_t = int;

namespace lib {
  
  using iid_t = unsigned;
  using oid_t = unsigned;

  TP<TN T> struct type_tag {};

  struct empty_struct { };

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
  TP<TN T, TN... TT> struct type_first< T, TT... > { using type = T; };
  TP<TN... TT>
  using type_first_t = typename type_first< TT... >::type;

  TP<TN T> T declval();

  TP<ssize_t...> using void_v = void;
  TP<TN...> using void_t = void;

  TP<TN T> struct identity { using type = T; };


  TP<TN T0, TN T1>
  struct is_same : type_false { };

  TP<TN T>
  struct is_same< T, T > : type_true { };

  TP<TN T0, TN T1>
  constexpr bool is_same_v = is_same< T0, T1 >::value;


  TP<bool N, TN T1, TN T2>
  struct select { using type = T2; };

  TP<TN T1, TN T2>
  struct select< true, T1, T2 > { using type = T1; };

  TP<bool N, TN T1, TN T2>
  using select_t = typename select< N, T1, T2 >::type;


  TP<bool N, TN T = void>
  struct enable_if { };

  TP<TN T>
  struct enable_if< true, T > { using type = T; };

  TP<bool N, TN T = void>
  using enable_if_t = TN enable_if< N, T >::type;

  TP<bool N, TN T = void>
  struct disable_if { };

  TP<TN T>
  struct disable_if< false, T > { using type = T; };

  TP<bool N, TN T = void>
  using disable_if_t = TN disable_if< N, T >::type;

  TP<bool... NN> struct all { };
  TP<bool N, bool... NN> struct all< N, NN... > { static constexpr bool value = N and all< NN... >::value; };
  TP<bool N> struct all< N > { static constexpr bool value = N; };
  TP<bool... NN>
  constexpr bool all_v = all< NN... >::value;


  TP<TN T> struct is_array : type_false { using type = T; };
  TP<TN T> struct is_array< T[] > : type_true { using type = T; };
  TP<TN T, ssize_t N> struct is_array< T[N] > : type_true { using type = T; };

  TP<TN T>
  constexpr bool is_array_v = is_array< T >::value;

  TP<TN T> 
  using no_array_t = typename is_array< T >::type;


  TP<TN T> struct is_ref : type_false { using type = T; };
  TP<TN T> struct is_ref< T& > : type_true { using type = T; };
  TP<TN T> struct is_ref< T&& > : type_true { using type = T; };

  TP<TN T> 
  constexpr bool is_ref_v = is_ref< T >::value;

  TP<TN T> 
  using no_ref_t = typename is_ref< T >::type;

  TP<TN T> struct is_ptr : type_false { using type = T; };
  TP<TN T> struct is_ptr< T* > : type_true { using type = T; };

  TP<TN T> 
  constexpr bool is_ptr_v = is_ptr< T >::value;

  TP<TN T> 
  using no_ptr_t = typename is_ptr< T >::type;

  TP<TN T> struct is_const : type_false { using type = T; };
  TP<TN T> struct is_const< const T > : type_true { using type = T; };

  TP<TN T> 
  constexpr bool is_const_v = is_const< T >::value;

  TP<TN T>
  using no_const_t = typename is_const< T >::type;

  TP<TN T> 
  constexpr bool is_cref_v = is_const_v< no_ref_t< T > >;

  TP<TN T>
  using no_cref_t = no_const_t< no_ref_t< T > >;

  TP<TN T> 
  constexpr bool is_cptr_v = is_const_v< no_ptr_t< T > >;

  TP<TN T>
  using no_cptr_t = no_const_t< no_ptr_t< T > >;

  TP<TN T> 
  constexpr bool is_void_v = is_same_v< no_cref_t< no_const_t< T > >, void >;


  TP<TN T, TN = void >
  struct is_primitive_class : type_false { }; 

  TP<TN T> 
  struct is_primitive_class< T, void_v< T::is_primitive >> { 

    static constexpr bool value = T::is_primitive; 
  };

  TP<TN T>
  constexpr bool is_primitive_class_v = is_primitive_class< T >::value;
  
  TP<TN T, TN = select_t< is_ptr_v< T >, int, T > > 
  struct is_primitive : type_false { }; 

  TP<TN T> struct is_primitive< T, int > : type_true { };
  TP<TN T> struct is_primitive< T, uint > : type_true { };
  TP<TN T> struct is_primitive< T, short > : type_true { };
  TP<TN T> struct is_primitive< T, char > : type_true { };
  TP<TN T> struct is_primitive< T, bool > : type_true { };
  TP<TN T> struct is_primitive< T, byte > : type_true { };
  TP<TN T> struct is_primitive< T, float > : type_true { };
  TP<TN T> struct is_primitive< T, double > : type_true { };
  TP<TN T> struct is_primitive< T, long > : type_true { };
  TP<TN T> struct is_primitive< T, long long > : type_true { };

  TP<TN T>
  constexpr bool is_primitive_v = is_primitive_class_v< T > or 
                                  is_primitive< no_const_t< T > >::value;


  TP<TN T, ssize_t N> T* begin( T ( &data)[ N ] ) { return data; }
  TP<TN T, ssize_t N> T  end  ( T ( &data)[ N ] ) { return &data[ N ]; }

  TP<TN T> 
  auto begin( T& data ) { return data.begin(); }

  TP<TN T> 
  auto end  ( T& data ) { return data.end(); }

  TP<TN T> 
  auto begin( T const& data ) { return data.begin(); }

  TP<TN T> 
  auto end  ( T const& data ) { return data.end(); }


  TP<TN T, TN = enable_if_t< not is_primitive_v< no_ref_t< T > > and is_ref_v< T > >> 
  no_ref_t< T >&& move( T&& value ) noexcept { 

    return static_cast< no_ref_t< T >&& > ( value ); 
  }

  TP<TN T, TN = enable_if_t< is_primitive_v< no_ref_t< T > > >> 
  T move( T& value ) noexcept { 
    
    auto value2 = value; value = T{}; return value2; 
  }

  struct move_t {

    TP<TN T> 
    static T copymove( T& value ) noexcept { return lib::move( value ); }
  };

  struct copy_t {

    TP<TN T> 
    static T& copymove( T& value ) noexcept { return value; }
  };

  TP<TN T>
  void swap( T& left, T& right ) noexcept {
    auto tmp = move( left );
    left = move( right );
    right = move( tmp );
  }


  TP<TN T> 
  select_t< is_ref_v< T >, T, T&& > forward( no_ref_t< T >& value ) noexcept { 

    return static_cast< select_t< is_ref_v< T >, T, T&& > >( value ); 
  } 

  TP<TN U0, TN U1> 
  U0 type_cast( U1&& value ) noexcept { 

    char* from = (char*) &value;

    return static_cast< U0 >( *(no_cref_t< U0 >*) from );
  }


  TP<TN T, TN = void, TN = void, TN = void, TN = void>
  struct is_container : type_false { };

  TP<TN T>
  struct is_container< T,  disable_if_t< is_primitive_v< T > >,
                            disable_if_t< is_array_v< T > >,
                            decltype( (void) ( typename T::iterator( T::*)() ) & T::begin ),
                            decltype( (void) ( typename T::iterator( T::*)() ) & T::end )
                     > : type_true { };
  TP<TN T> 
  constexpr bool is_container_v = is_container< T >::value;

  struct is_base {

    using yes = char(&)[1];
    using no = char(&)[2];

    TP<TN T>
    static yes check( T );
    TP<TN T>
    static no check( ... );
  };

  TP<TN T0, TN T1>
  constexpr bool is_base_v = $size( is_base::check< T0* >( declval< T1* >() ) ) == 1 ? true : false;


  TP<TN>
  struct is_template : type_false { };

  TP< TP1<TN...> class T, TN... TT >
  struct is_template< T< TT... > > : type_true { using type = TN type_first< TT... >::type; };

  TP<TN T>
  using template_arg_t = TN is_template< T >::type;

  TP<ssize_t... NN> struct index_list { };

  TP<ssize_t N, ssize_t N_ = N, TN T = index_list<>> 
  struct make_index_list;

  TP<ssize_t N, ssize_t N_, ssize_t... NN> 
  struct make_index_list< N, N_, index_list< NN...> > : 
          make_index_list< N - 1, N_, index_list< NN..., N_ - N> > { };

  TP<ssize_t N_, ssize_t... NN> 
  struct make_index_list< 0, N_, index_list< NN...> > { using type = index_list< NN... >; };

  TP<ssize_t N>
  using index_list_t = typename make_index_list< N >::type;

}




