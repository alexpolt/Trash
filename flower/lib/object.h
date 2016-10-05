#pragma once

#include "common.h"

namespace lib {


  struct object {

    virtual ~object() {}


    virtual iid_t get_interface_id() const = 0;

    virtual oid_t get_object_id() const {
      return get_interface_id();
    }

    virtual lib::string to_string() const {
      char buf[64];
      snprintf( buf, sizeof( buf ), "0x%#X", (uintptr_t) this );
      return lib::string{ buf };
    }


    // working with components

    virtual value<object> get_object() = 0;

    virtual bool has_object( iid_t ) const = 0;

    virtual value< object > get_object( iid_t ) = 0;

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


  /* Helper template used by a primary object to ask for components. */

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

  /*
    Usage:
    $interface( interface_type );
    For example:
    $interface( car );
  */

  #define $interface( $0 )  static iid_t interface_id = __COUNTER__; \
                            using object_type = $0; \
                            constexpr static lib::type_tag< $0 > tag{}; \
                            iid_t get_interface_id() const override { return interface_id; } \

  /*
    Usage in an primary object type:
    $object( object_type, zero or more component template names );

    For example:
    $object( car, car_physics, car_ai );
  */

  #define $object( ... ) \
                        /* methods to create components instantiated with the object type */ \
                        using object_factory = lib::object_factory< __VA_ARGS__ >; \
                        using object::has_object; \
                        using object::get_object; \
                         value<object> get_object( iid_t id ) override { \
                          return object_factory::get_object( id, $ ); \
                        } \
                        bool has_object( iid_t id ) const override { \
                          return object_factory::has_object( id ); \
                        } \
                        /* getting the primary object */ \
                        value< lib::object > get_object() override { \
                          return value< lib::object >::create< $args_first( __VA_ARGS__ ) >( $ ); \
                        } \
                        operator value< lib::object >() { \
                          return get_object(); \
                        } \
                        $t<$n Z0> explicit operator value< Z0 >() { \
                          return get_object( Z0::tag ); \
                        } \
                        /* create( ... ): used by object_factory to create objects (copies) */ \
                        $t<$n... ZZ> static auto create( ZZ&&... args ) { \
                          return value< lib::object >::create< $args_first( __VA_ARGS__ )  >( args... ); \
                        }

  #define $cargs_1( $0 ) T0
  #define $cargs_2( $0, $1 ) $1
  #define $cobject_type( ... ) $apply( $paste( $cargs_, $args_size( __VA_ARGS__ ) ), __VA_ARGS__ )

  /*
    Usage in a component template (with one type parameter for the object type) class:
    $component( component_template_name ); or
    $component( component_specialization_name, object_type );

    For example:
    $component( car_physics ); or
    $component( car_physics, car_bmw );
  */

  #define $component( ... ) \
                        /* getting the primary object */ \
                        value< lib::object > get_object() override { \
                          return object.get_object(); \
                        } \
                        value<object> get_object( iid_t id ) override { \
                          return object.get_object( id ); \
                        } \
                        bool has_object( iid_t id ) const override { \
                          return object.has_object( id ); \
                        } \
                        /* create( ... ): used by object_factory to create objects */ \
                        $t<$n... ZZ> static auto create( ZZ&&... args ) { \
                          return value< lib::object >::create< $args_first( __VA_ARGS__ ) >( args... ); \
                        } \
                        /* a constructor: component( object_type& ) */\
                        $args_first( __VA_ARGS__ ) ( $cobject_type( __VA_ARGS__ ) & object0 ) : object{ object0 } { } 

}


