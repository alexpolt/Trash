#pragma once

#include "macros.h"
#include "types.h"

namespace lib {


  struct object {

    virtual ~object() {}

    virtual iid_t get_interface_id() const = 0;

    virtual cstr get_interface_name() const = 0;

    virtual oid_t get_object_id() const;

    virtual value< object > get_object() const = 0;    

    virtual cstr to_string() const; 

    // copying

    virtual value< object > get_copy() const;

    TP<TN T0>
    value< T0 > get_copy() const; 

    // working with components

    virtual bool has_object( iid_t ) const = 0;

    TP<TN T0> 
    bool has_object( type_tag<T0> ) const;

    virtual value< object > get_object( iid_t ) = 0;

    TP<TN T0> 
    value< T0 > get_object( type_tag<T0> );
  };


  inline cstr object::get_interface_name() const { return "object"; }

  inline oid_t object::get_object_id() const { return get_interface_id();  }

  inline cstr object::to_string() const {
    auto& buffer = global::get_buffer<char>();
    snprintf( buffer, $length( buffer ), "%s(%#X)", get_interface_name(), (uintptr_t) this );
    return buffer;
  }

  inline value< object > object::get_copy() const { return get_object(); };

  TP<TN T0>
  inline value< T0 > object::get_copy() const { return static_cast< value<T0> >( get_copy( ) ); }

  TP<TN T0> 
  inline bool object::has_object( type_tag<T0> ) const { return has_object( T0::interface_id ); }
  
  TP<TN T0> 
  inline value< T0 > object::get_object( type_tag<T0> ) { return static_cast< value<T0> >( get_object( T0::interface_id ) ); }




  /* Helper template used by a primary object to ask for components. */

  TP<TN T0, TN... TT> struct object_factory {

    using create_f = value< object > (*)( T0& );

    static create_f _create_list[];
    static iid_t _iid_list[];

    static bool has_object( iid_t iid ) {

      for( auto i : _iid_list ) 

        if( i == iid ) return true;

      return false;

    }

    static value< object > get_object( iid_t iid, T0& owner ) {

      ssize_t counter{};

      for( auto i : _iid_list ) {

        if( iid == i ) 
          return _create_list[ counter ]( owner );

        ++counter;
      }

      throw $error_object( iid, owner.to_string().data() );

    }

  };

  TP<TN T0, TN... TT>
    iid_t object_factory< T0, TT... >::_iid_list[] = { T0::interface_id, TT::interface_id... };
    
  TP<TN T0, TN... TT>
    typename object_factory< T0, TT... >::create_f 
      object_factory< T0, TT... >::_create_list[] = { T0::create, TT::create... };

  /*
    Usage:
    interface( interface_type );
    For example:
    interface( car );
  */

  #define $interface( $0 )  constexpr static lib::iid_t interface_id = __COUNTER__; \
                            using object_type = $0; \
                            constexpr static lib::type_tag< $0 > tag{}; \
                            lib::iid_t get_interface_id() const override { return interface_id; } \
                            cstr  get_interface_name() const override { return #$0; } 

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
                        value<object> get_object( lib::iid_t id ) override { \
                          return object_factory::get_object( id, $this ); \
                        } \
                        bool has_object( lib::iid_t id ) const override { \
                          return object_factory::has_object( id ); \
                        } \
                        /* getting a copy of the primary object */ \
                        value< lib::object > get_object() const override { \
                          return create( $this );\
                        } \
                        /* create factory method */ \
                        TP<TN... UU> static value< object > create( UU&&... args ) { \
                          return value< lib::object >::create< $args_first( __VA_ARGS__) >( args... ); \
                        }

  /*
    Usage in a component template (with one type parameter for the object type) class:
    component( component_template_name );

    For example:
    component( car_physics );
  */

  #define $component( $0 ) \
                        /* getting the primary object */ \
                        value< lib::object > get_object() const override { \
                          return _owner.get_object(); \
                        } \
                        value<object> get_object( lib::iid_t id ) override { \
                          return _owner.get_object( id ); \
                        } \
                        bool has_object( lib::iid_t id ) const override { \
                          return _owner.has_object( id ); \
                        } \
                        TP<TN U0> $0( U0&& owner ) : _owner{ owner } { } \
                        /* create factory method */ \
                        TP<TN U0> static auto create( U0&& owner ) { \
                          return value< object >::create< $0 >( owner ); \
                        } \
                        lib::template_arg_t< $0 >& _owner;

}


