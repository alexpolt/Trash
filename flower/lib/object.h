#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "value.h"
#include "to-string.h"


namespace lib {

  TP<TN, bool> struct shared_ptr;

  TP<TN T0>
  using weak_ptr = shared_ptr< T0, true >;


  struct object {

    virtual ~object() {}


    virtual oid_t get_object_id() const;

    virtual cstr to_string() const; 


    virtual bool has_object( iid_t ) const = 0;

    virtual value< object > get_object( iid_t ) = 0;

    TP<TN T0> bool has_object( type_tag<T0> ) const;

    TP<TN T0> value< T0 > get_object( type_tag<T0> );
  };


  struct interface : object {

    virtual iid_t get_interface_id() const = 0;

    virtual cstr get_interface_name() const = 0;

  };


  struct component : interface {

    virtual lib::weak_ptr< lib::object > get_owner() = 0;
  };



  inline oid_t object::get_object_id() const { return ( oid_t ) this; }

  inline cstr object::to_string() const { return lib::to_string( "object( 0x%p )", this ); }

 
  TP<TN T0> 
  inline bool object::has_object( type_tag<T0> ) const { return has_object( T0::interface_id ); }
  
  TP<TN T0> 
  inline value< T0 > object::get_object( type_tag<T0> ) { return type_cast< value< T0 >&& >( get_object( T0::interface_id ) ); }


  struct error_object : error {

    error_object( cstr file, int line, cstr func, iid_t iid, cstr msg ) : error{ file, line, func } {

      auto l = strlen( error::get_buffer() );

      auto ptr = error::get_buffer() + l;

      snprintf( ptr, $array_size( error::get_buffer() ) - l, ": object %d not found in object( %s )", iid, msg );
    } 
  };

  #define $error_object( $0, $1 ) error_object{ __FILE__, __LINE__, __func__, $0, $1 }



  /* Helper template used by a primary object to ask for components. */

  TP<TN T0, TN... TT> struct object_factory {

    using create_f = value< object > (*)( lib::weak_ptr< T0 > );

    static create_f _create_list[];
    static iid_t _iid_list[];


    static bool has_object( iid_t iid ) {

      for( auto i : _iid_list ) 

        if( i == iid ) return true;

      return false;
    }

    static value< object > get_object( iid_t iid, T0* ptr ) {

      ssize_t counter{};

      for( auto i : _iid_list ) {

        if( iid == i ) 
          return _create_list[ counter ]( lib::weak_ptr< T0>{ ptr } );

        ++counter;
      }

      $throw $error_object( iid, ptr->to_string() );

      return value< object >{};
    }

  };

  TP<TN T0, TN... TT>
    iid_t object_factory< T0, TT... >::_iid_list[] = { TT::interface_id... };
    
  TP<TN T0, TN... TT>
    typename object_factory< T0, TT... >::create_f 
      object_factory< T0, TT... >::_create_list[] = { &TT::create... };

  /*
    Usage:
    interface( interface_type );
    For example:
    interface( car );
  */

  #define $interface( $0 )  constexpr static lib::iid_t interface_id = __COUNTER__; \
                            constexpr static lib::type_tag< $0 > tag{}; \
                            lib::iid_t get_interface_id() const override { return interface_id; } \
                            cstr get_interface_name() const override { return #$0; } 

  /*
    Usage in an primary object type:
    object( object_type, zero or more component template names );

    For example:
    object( car, car_physics, car_ai );
  */

  #define $object( ... ) \
                        /* methods to create components instantiated with the object type */ \
                        using object_factory = lib::object_factory< __VA_ARGS__ >; \
                        using object::has_object; \
                        using object::get_object; \
                        value< lib::object > get_object( lib::iid_t id ) override { \
                          return object_factory::get_object( id, this ); \
                        } \
                        bool has_object( lib::iid_t id ) const override { \
                          return object_factory::has_object( id ); \
                        }

  /*
    Usage in a component template (with one type parameter for the object type) class:
    component( component_template_name );

    For example:
    component( car_physics );
  */

  #define $component( $0 ) \
                        value< lib::object > get_object( lib::iid_t id ) override { \
                          return _owner->get_object( id ); \
                        } \
                        bool has_object( lib::iid_t id ) const override { \
                          return _owner->has_object( id ); \
                        } \
                        TP<TN U0> static auto create( lib::weak_ptr< U0 > owner ) { \
                          return value< lib::object >::create< $0 >( move( owner ) ); \
                        }

  #define $component_template( $0 ) \
                        $component( $0 ) \
                        lib::weak_ptr< lib::object > get_owner() override { \
                          return _owner.lock(); \
                        } \
                        $0( lib::weak_ptr< lib::template_arg_t< $0 > > owner ) : _owner{ move( owner ) } { } \
                        lib::weak_ptr< lib::template_arg_t< $0 > > _owner;


}


