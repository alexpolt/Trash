#pragma once

#include "common.h"

namespace lib {


  $t<$n T0> struct type_tag {};


  struct object {

    virtual ~object() {}

    virtual lib::string to_string() const = 0;

    virtual value<object> get_object() = 0;

    virtual oid_t get_object_id() const = 0;

    virtual bool has_object( oid_t ) const = 0;

    virtual value< object > get_object( oid_t ) = 0;


    $t<$n T0> 
    bool has_object( type_tag<T0> ) const {
      return has_object( T0::object_id );
    }
    
    $t<$n T0> 
    value<T0> get_object( type_tag<T0> ) {

      union {
        value< object > r0;
        value< T0 > r1;
      };

      r0 = get_object( T0::object_id );

      return r1;
    }

  };


  $t<$n T0, $t1< $n > class... TT> struct object_factory {

    using create_f = value< object >(*)( T0& object );

    static create_f create_list[];
    static oid_t oid_list[];

    static bool has_object( oid_t oid ) {

      for( auto i : oid_list ) 

        if( i == oid ) return true;

      return false;

    }

    static value< object > get_object( oid_t oid, T0& object ) {

      uint counter{};

      for( auto i : oid_list ) {

        if( oid == i ) 
          return create_list[ counter ]( object );

        ++counter;
      }

      throw $error_object( oid, object.to_string().data() );

    }

  };

  $t<$n T0, $t1< $n > class... TT>
    oid_t object_factory< T0, TT... >::oid_list[] = { T0::object_id, TT< T0 >::object_id... };
    
  $t<$n T0, $t1< $n > class... TT>
    typename object_factory< T0, TT... >::create_f 
      object_factory< T0, TT... >::create_list[] = { T0::create, TT< T0 >::create... };

  $t<$n T0>
  struct bind_object_factory { 
    using type = lib::object_factory< T0 >;
  };

  $t< $t1< $t2<$n> class... > class T0, $t2<$n> class ... TT>
  struct bind_object_factory< T0< TT... > > { 
    using type = lib::object_factory< T0< TT... >, TT... >;
  };



  #define $interface( $0 )  static oid_t object_id = __COUNTER__; \
                            using object_type = $0; \
                            constexpr static lib::type_tag< $0 > tag{}; \
                            oid_t get_object_id() const override { return object_id; } \
                            lib::string to_string() const override { return #$0; }


  #define $object( $0 ) using object_factory = typename lib::bind_object_factory< $0 >::type; \
                        value<object> get_object( oid_t id ) override { \
                          return object_factory::get_object( id, $ ); \
                        } \
                        bool has_object( oid_t id ) const override { \
                          return object_factory::has_object( id ); \
                        } \
                        using object::has_object; \
                        using object::get_object; \
                        value< lib::object > get_object() override { \
                          return value< lib::object >::create< $0 >( $ ); \
                        } \
                        operator value< lib::object >() { \
                          return get_object(); \
                        } \
                        $t<$n Z0> explicit operator value< Z0 >() { \
                          return get_object( Z0::tag ); \
                        } \
                        $t<$n... ZZ> static auto create( ZZ&&... args ) { \
                          return value< lib::object >::create< $0 >( args... ); \
                        }

  #define $component( $0 ) \
                        value< lib::object > get_object() override { \
                          return object.get_object(); \
                        } \
                        value<object> get_object( oid_t id ) override { \
                          return object.get_object( id ); \
                        } \
                        bool has_object( oid_t id ) const override { \
                          return object.has_object( id ); \
                        } \
                        $t<$n... ZZ> static auto create( ZZ&&... args ) { \
                          return value< lib::object >::create< $0 >( args... ); \
                        } \
                        $0( T0& object0 ) : object{ object0 } { } \

}


