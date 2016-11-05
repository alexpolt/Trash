#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "memory.h"
#include "strong-ptr.h"
#include "allocator.h"
#include "value.h"


namespace lib {


  struct alloc_chunk : allocator {

    using next_ptr = char*;

    alloc_chunk() { }

    alloc_chunk( ssize_t chunk_size ) : _chunk{ new chunk{} } { 
      
      _chunk->size = chunk_size;

      $assert( chunk_size > $size( next_ptr ), "chunk size has to be > sizeof( char* )" );
    }

    alloc_chunk( alloc_chunk const& other ) : _chunk{ other._chunk.lock() } { }

    alloc_chunk( alloc_chunk&& other ) : _chunk{ move( other._chunk ) } { }

    alloc_chunk& operator=( alloc_chunk other ) { 

      _chunk = move( other._chunk );
      
      return $this;
    }

    static auto create( ssize_t size ) { return value< allocator >::create< alloc_chunk >( size ); }

    value< allocator > get_copy() const override {

      return value< allocator >::create< alloc_chunk >( $this );
    }

    void new_chunk() {

      auto chunk = (char*) $alloc( _chunk.get(), _chunk->size );

      char** data_next = (char**) chunk;

      *data_next = _chunk->data;

      _chunk->data = chunk;

      _chunk->offset = $size( next_ptr );
     }

    char* alloc( ssize_t sz ) override {

      log::memory, "alloc_chunk::alloc( ", sz, " ), chunk size = ", size(), " ( ", size_true(), " ), ";
      log::memory, "available = ", available(), log::endl;

      $assert( sz <= size_true(), "size is greater than max size" );
      
      if( _chunk->data == nullptr or sz > available() ) {

        log::memory, "chunk size exceeded, allociate another one", log::endl;

        new_chunk();
      }

      auto ptr = _chunk->data + _chunk->offset;

      _chunk->offset += sz;

      return ptr;
    }

    void free( void* ptr, ssize_t sz ) override { 

      log::memory, "alloc)chunk::free( ", ptr, ", ", sz, " ), chunk size = ", size();
      log::memory, " ( ", size_true(), " ), available = ", available(), log::endl;
    }

    ssize_t size_true() const { return _chunk->size - $size( next_ptr ); }

    ssize_t size() const override { return _chunk->size; }

    ssize_t available() const override { return size() - _chunk->offset; }

    ssize_t offset() const { return _chunk->offset; }

    explicit operator bool() const { return (bool) _chunk; }

    struct chunk {

      ~chunk() { 

        while( data ) {
          
          auto next = *( char** ) data;

          $free( this, move( data ), size );

          data = next;
        }
      }

      char* data;
      ssize_t size;
      ssize_t offset;
    };

    strong_ptr< chunk > _chunk;
  };

}


