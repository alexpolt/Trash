#pragma once

#include <cstdio>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/ptr.h"
#include "lib/vector.h"
#include "lib/value.h"
#include "lib/to-string.h"
#include "lib/log.h"
#include "link-map.h"


namespace lib {


  using value_o = value< object >;


  struct object : nocopy_vbase {


    virtual oid_t get_object_id() const;

    virtual cstr to_string() const; 


    virtual bool has_object( iid_t ) const = 0;

    virtual value_o get_object( iid_t ) = 0;

    TP<TN T> bool has_object( type_tag< T > ) const;

    TP<TN T> value< T > get_object( type_tag< T > );


    virtual void child_add( object_wp, bool = false );

    virtual void child_remove( object_wp const&, bool = false );

    virtual vector< object_wp > const& get_children() const;

    virtual vector< object_wp > const& get_parents() const;
  };




  struct interface : object {

    virtual iid_t get_interface_id() const = 0;

    virtual cstr get_interface_name() const = 0;

  };


  struct component : interface {

    virtual object_wp get_owner() = 0;
  };



  inline oid_t object::get_object_id() const { return ( oid_t ) this; }

  inline cstr object::to_string() const { return lib::to_string( "object( 0x%p )", this ); }

 
  TP<TN T> 
  inline bool object::has_object( type_tag< T > ) const { return has_object( T::interface_id ); }
  
  TP<TN T> 
  inline value< T > object::get_object( type_tag< T > ) { return type_cast< value< T >&& >( get_object( T::interface_id ) ); }



  inline void object::child_add( object_wp child, bool ignore_duplicates ) { 

    global::link_map<>.link( object_wp{ this }, move( child ), ignore_duplicates ); 
  }
  
  inline void object::child_remove( object_wp const& child, bool ignore_duplicates ) { 

    global::link_map<>.unlink( this, child, ignore_duplicates ); 
  }

  inline vector< object_wp > const& object::get_children() const { 

    return global::link_map<>.get_children( this ); 
  }

  inline vector< object_wp > const& object::get_parents() const { 
    
    return global::link_map<>.get_parents( this ); 
  }


  struct error_object : error {

    error_object( cstr file, iid_t iid, cstr msg ) {

      auto ptr = error::get_buffer();

      snprintf( ptr, $array_size( error::get_buffer() ), 
        "%s: object %d not found in object( %s )", file, iid, msg );

      log::error, error::get_buffer(), log::endl;
    }
  };

  #define $error_object( $0, $1 ) error_object{ $file_line, $0, $1 }



  TP<TN T0, TN... TT> struct object_factory {

    using create_f = value_o (*)( lib::weak_ptr< T0 > );

    static create_f _create_list[];
    static iid_t _iid_list[];


    static bool has_object( iid_t iid ) {

      for( auto i : _iid_list ) 

        if( i == iid ) return true;

      return false;
    }

    static value_o get_object( iid_t iid, T0* ptr ) {

      ssize_t counter{};

      for( auto i : _iid_list ) {

        if( iid == i ) 
          return _create_list[ counter ]( lib::weak_ptr< T0 >{ ptr } );

        ++counter;
      }

      $throw $error_object( iid, ptr->to_string() );

      return value_o{};
    }

  };

  TP<TN T0, TN... TT>
    iid_t object_factory< T0, TT... >::_iid_list[] = { TT::interface_id... };
    
  TP<TN T0, TN... TT>
    typename object_factory< T0, TT... >::create_f 
      object_factory< T0, TT... >::_create_list[] = { &TT::create... };


  #define $interface( $0 )  constexpr static lib::iid_t interface_id = __COUNTER__; \
                            constexpr static lib::type_tag< $0 > tag{}; \
                            lib::iid_t get_interface_id() const override { return interface_id; } \
                            cstr get_interface_name() const override { return #$0; } 

  #define $object( ... ) \
                        using object_factory = lib::object_factory< __VA_ARGS__ >; \
                        using object::has_object; \
                        using object::get_object; \
                        value< lib::object > get_object( lib::iid_t id ) override { \
                          return object_factory::get_object( id, this ); \
                        } \
                        bool has_object( lib::iid_t id ) const override { \
                          return object_factory::has_object( id ); \
                        }


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


