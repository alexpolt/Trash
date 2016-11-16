#pragma once

#include "macros.h"
#include "types.h"
#include "memory.h"
#include "strong-ptr.h"
#include "allocator.h"
#include "value.h"
#include "range.h"


namespace lib {


  struct alloc_stat : allocator {

    struct data_t;

    alloc_stat( cstr name ) { 

      auto data = new data_t{};

      for( auto i : range{ 0, $array_size( data->name ) - 1 } ) 
        
        if( name[ i ] ) data->name[ i ] = name[ i ];

      _data = strong_ptr< data_t >{ data, data->name };
    }

    alloc_stat( alloc_stat const& other ) : _data{ other._data.lock() } { }

    alloc_stat( alloc_stat&& other ) : _data{ move( other._data ) } { }

    alloc_stat& operator=( alloc_stat other ) { 

      _data = move( other._data );
      
      return $this;
    }

    static auto create( cstr name ) { 
      
      return value< allocator >::create< alloc_stat >( name ); 
    }

    value< allocator > get_copy() const override {

      return value< allocator >::create< alloc_stat >( $this );
    }

    void* alloc( ssize_t sz ) override {

      _data->total += sz;

      log::memory, name(), " alloc( ", sz, " ), total = ", _data->total, log::endl;

      return lib::alloc( sz );
    }

    void free( void* ptr, ssize_t sz ) override { 

      log::memory, name(), " free( ", sz, " ), total = ", _data->total, log::endl;

      lib::free( ptr, sz );
    }

    ssize_t size() const override { return _data->total; }

    ssize_t available() const override { return 0; }

    cstr name() const override { return _data->name; }

    void set_name( cstr name ) { 

      for( auto i : range{ 0, $array_size( _data->name ) - 1 } )
        
        if( name[ i ] ) _data->name[ i ] = name[ i ];
    }
 

    struct data_t {

      ssize_t total;
      
      char name[ 16 ];
    };

    strong_ptr< data_t > _data;
 };

}


