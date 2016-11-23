#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/owner-ptr.h"
#include "lib/vector.h"
#include "lib/alloc-default.h"
#include "lib/to-string.h"
#include "types.h"


namespace lib {

  namespace render {


    namespace global {

      TP<TN...>
      vector< owner_ptr< message > > messages{ alloc_default::create( "render messages" ) };

      TP<ssize_t... NN>
      auto& create_messages_ptrs( index_list< NN... > ) {

        static vector< message* > messages_free[ message::mtype::size  ]{
          
          { ( (void)NN, alloc_default::create( "render messages ptrs" ) ) }...
        };

        return messages_free;
      }

      TP<TN...>
      vector< message* > ( &messages_ptrs )[ message::mtype::size ] = 

        create_messages_ptrs( index_list_t< message::mtype::size >{} );

    }


    struct render_message : nocopy {

      render_message() { }

      render_message( message* ptr, message::mtype t ) : _data{ ptr }, _type{ t } { }

      render_message( render_message&& other ) : _data{ move( other._data ) }, _type{ other._type } { }

      render_message& operator=( render_message&& other ) {

        _data = move( other._data );
        _type = other._type;

        return $this;
      }

      ~render_message() {

        if( _data )

          global::messages_ptrs<>[ _type ].push_back( move( _data ) );
      }

      cstr to_string() const { 

        return lib::to_string( "message( %s, %p )", message::get_desc( _type ), (void*) _data ); 
      }

      auto data() { return _data; }
      auto type() { return _type; }

      message* _data{};
      message::mtype _type{};
    };


    TP<TN T>
    auto create_message( T msg ) {

      render_message rmsg;

      if( global::messages_ptrs<>[ T::type ].size() ) {

        auto ptr = global::messages_ptrs<>[ T::type ].pop_back();

        *ptr = move( msg );

        rmsg = render_message{ ptr, T::type };

      } else {

        global::messages<>.push_back( owner_ptr< message >{ new T{ move( msg ) } } );

        auto ptr = global::messages<>.back().get();
        
        rmsg = render_message{ ptr, T::type };
      }

      log::render, rmsg, " created", log::endl;

      return rmsg;
    }


  }
}



