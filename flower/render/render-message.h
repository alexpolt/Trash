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

        static vector< message* > messages_free[ message::type::size  ]{
          
          { ( (void)NN, alloc_default::create( "render messages ptrs" ) ) }...
        };

        return messages_free;
      }

      vector< message* > ( &messages_ptrs )[ message::type::size ] = 

        create_messages_ptrs( index_list_t< message::type::size >{} );

    }


    struct render_message : nocopy {

      render_message() { }

      render_message( message* ptr, message::type t ) : _data{ ptr }, _type{ t } { }

      render_message( render_message&& other ) : _data{ move( other._data ) }, _type{ other._type } { }

      render_message& operator=( render_message&& other ) {

        _data = move( other._data );
        _type = other._type;

        return $this;
      }

      ~render_message() {

        if( _data )

          global::messages_ptrs[ _type ].push_back( move( _data ) );
      }

      cstr to_string() const { 

        return lib::to_string( "render_msg( %s, %p )", message::get_desc( _type ), (void*) _data ); 
      }

      auto data() { return _data; }
      auto type() { return _type; }

      message* _data{};
      message::type _type{};
    };


    TP<TN T>
    auto create_message() {

      render_message msg;

      if( global::messages_ptrs[ T::type ].size() ) {

        auto ptr = global::messages_ptrs[ T::type ].pop_back();

        msg = render_message{ ptr, T::type };

      } else {

        global::messages<>.push_back( owner_ptr< message >{ new T{} } );

        auto ptr = global::messages<>.back().get();

        msg = render_message{ ptr, T::type };
      }

      log::render, "create message ", msg, log::endl;

      return msg;
    }


  }
}



