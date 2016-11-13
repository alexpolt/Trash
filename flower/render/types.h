#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/owner-ptr.h"
#include "lib/vector.h"
#include "lib/alloc-default.h"
#include "lib/to-string.h"


namespace lib {

  namespace render {


    namespace message {


      enum type : int {

        mesh, material, cbuffer, size
      };


      struct base {

        virtual ~base() { }
      };

    }


    namespace global {
      
      TP<TN T=void>
      vector< owner_ptr< message::base > > messages{ alloc_default::create( "render messages" ) };

      TP<TN T=void>
      vector< message::base* > messages_free[ message::type::size + 1 ]{
        
        { alloc_default::create( "render msgs ptrs" ) },
        { alloc_default::create( "render msgs ptrs" ) },
        { alloc_default::create( "render msgs ptrs" ) },
        { alloc_default::create( "render msgs ptrs" ) },
      };
    }


    struct render_message : nocopy {

      render_message() { }

      render_message( message::base* ptr, message::type t ) : _data{ ptr }, _type{ t } { }

      render_message( render_message&& other ) : _data{ move( other._data ) }, _type{ other._type } { }

      render_message& operator=( render_message&& other ) {

        _data = move( other._data );
        _type = other._type;

        return $this;
      }

      ~render_message() {

        if( _data )

          global::messages_free<>[ _type ].push_back( move( _data ) );
      }

      cstr to_string() const { 

        return lib::to_string( "render_msg( type = %d, data = %p)", (int)_type, (void*)_data ); 
      }

      auto data() { return _data; }
      auto type() { return _type; }

      message::base* _data{};
      message::type _type{};
    };


    TP<TN T>
    auto create_message() {

      if( global::messages_free<>[ T::type ].size() ) {

        auto ptr = global::messages_free<>[ T::type ].pop_back();

        return render_message{ ptr, T::type };
      } 

      global::messages<>.push_back( owner_ptr< message::base >{ new T{} } );

      auto ptr = global::messages<>.back().get();

      return render_message{ ptr, T::type };
    }

  }
}




