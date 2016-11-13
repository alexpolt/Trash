#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "memory.h"
#include "strong-ptr.h"
#include "allocator.h"
#include "value.h"


namespace lib {


  struct alloc_chunk : allocator {

    using next_ptr = char*;


    alloc_chunk( cstr name, ssize_t size_chunk ) { 
      
      $assert( size_chunk > $size( next_ptr ), "chunk size has to be > sizeof( char* )" );

      auto ch = new chunk{};

      ch->size = size_chunk;

      for( auto i : range{ 0, $array_size( ch->name ) - 1 } )
        
        if( name[ i ] ) ch->name[ i ] = name[ i ];

      _chunk = strong_ptr< chunk >{ ch, ch->name };
    }

    alloc_chunk( alloc_chunk const& other ) : _chunk{ other._chunk.lock() } { }

    alloc_chunk( alloc_chunk&& other ) : _chunk{ move( other._chunk ) } { }

    alloc_chunk& operator=( alloc_chunk other ) { 

      _chunk = move( other._chunk );
      
      return $this;
    }

    static auto create( cstr name, ssize_t size ) { 

      return value< allocator >::create< alloc_chunk >( name, size ); 
    }

    value< allocator > get_copy() const override {

      return value< allocator >::create< alloc_chunk >( $this );
    }

    void new_chunk() {

      log::memory, name(), " getting new chunk size = ", size_chunk(), log::endl;

      auto chunk = lib::alloc( size_chunk() );

      char** data_next = (char**) chunk;

      *data_next = _chunk->data;

      _chunk->data = (char*) chunk;

      _chunk->offset = $size( next_ptr );

      _chunk->total += size_chunk();
     }

    void* alloc( ssize_t sz ) override {

      $assert( sz <= size_chunk() - $size( next_ptr ), "size is greater than max size" );
      
      if( _chunk->data == nullptr or sz > available() ) new_chunk();

      auto ptr = _chunk->data + _chunk->offset;

      _chunk->offset += sz;

      log::memory, name(), " alloc( ", sz, " ), chunk = ", size_chunk();
      log::memory, ", total = ", size(), ", available = ", available(), log::endl;

      return ptr;
    }

    void free( void* ptr, ssize_t sz ) override { 

      log::memory, name(), " free( ", ptr, ", ", sz, " ), chunk = ", size_chunk();
      log::memory, ", total = ", size(), ", available = ", available(), log::endl;
    }

    ssize_t size_chunk() const { return _chunk->size; }

    ssize_t size() const override { return _chunk->total; }

    ssize_t available() const override { return size_chunk() - _chunk->offset; }

    cstr name() const override { return _chunk->name; }
    
    ssize_t offset() const { return _chunk->offset; }

    explicit operator bool() const { return (bool) _chunk; }

    struct chunk {

      ~chunk() { 

        while( data ) {
          
          auto next = *( char** ) data;

          lib::free( move( data ), size );

          data = next;
        }
      }

      char* data;
      ssize_t size;
      ssize_t offset;
      ssize_t total;
      char name[ 16 ];
    };

    strong_ptr< chunk > _chunk;
  };

}


