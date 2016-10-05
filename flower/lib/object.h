#pragma once

#include "common.h"

namespace lib {


  $t<$n T0> struct type_tag {};


  struct object {

    virtual ~object() {}

    virtual value<object> get_object() = 0;

    virtual iid_t get_interface_id() const = 0;

    virtual bool has_object( iid_t ) const = 0;

    virtual value< object > get_object( iid_t ) = 0;

    virtual lib::string to_string() const {
      char buf[64];
      snprintf( buf, sizeof( buf ), "0x%#X", (uintptr_t) this );
      return lib::string{ buf };
    }


    $t<$n T0> 
    bool has_object( type_tag<T0> ) const {
      return has_object( T0::interface_id );
    }
    
    $t<$n T0> 
    value<T0> get_object( type_tag<T0> ) {

      union {
        value< object > r0;
        value< T0 > r1;
      };

      r0 = get_object( T0::interface_id );

      return r1;
    }

  };


  $t<$n T0, $t1< $n > class... TT> struct object_factory {

    using create_f = value< object >(*)( T0& object );

    static create_f create_list[];
    static iid_t iid_list[];

    static bool has_object( iid_t iid ) {

      for( auto i : iid_list ) 

        if( i == iid ) return true;

      return false;

    }

    static value< object > get_object( iid_t iid, T0& object ) {

      uint counter{};

      for( auto i : iid_list ) {

        if( iid == i ) 
          return create_list[ counter ]( object );

        ++counter;
      }

      throw $error_object( iid, object.to_string().data() );

    }

  };

  $t<$n T0, $t1< $n > class... TT>
    iid_t object_factory< T0, TT... >::iid_list[] = { T0::interface_id, TT< T0 >::interface_id... };
    
  $t<$n T0, $t1< $n > class... TT>
    typename object_factory< T0, TT... >::create_f 
      object_factory< T0, TT... >::create_list[] = { T0::create, TT< T0 >::create... };



  #define $interface( $0 )  static iid_t interface_id = __COUNTER__; \
                            using object_type = $0; \
                            constexpr static lib::type_tag< $0 > tag{}; \
                            iid_t get_interface_id() const override { return interface_id; } \


  #define $object( $0, ... ) \
                        using object_factory = lib::object_factory< $0, __VA_ARGS__ >; \
                        value<object> get_object( iid_t id ) override { \
                          return object_factory::get_object( id, $ ); \
                        } \
                        bool has_object( iid_t id ) const override { \
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

  #define $component_args_0( ) T0
  #define $component_args_1( $0 ) $0
  #define $component_object_type( ... ) $apply( $paste( $component_args_, $args_size( __VA_ARGS__ ) ), __VA_ARGS__ )

  #define $component( $0, ... ) \
                        value< lib::object > get_object() override { \
                          return object.get_object(); \
                        } \
                        value<object> get_object( iid_t id ) override { \
                          return object.get_object( id ); \
                        } \
                        bool has_object( iid_t id ) const override { \
                          return object.has_object( id ); \
                        } \
                        $t<$n... ZZ> static auto create( ZZ&&... args ) { \
                          return value< lib::object >::create< $0 >( args... ); \
                        } \
                        $0( $component_object_type( __VA_ARGS__ ) & object0 ) : object{ object0 } { } 

}


