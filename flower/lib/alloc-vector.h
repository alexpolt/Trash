#pragma once

#include "macros.h"
#include "types.h"
#include "memory.h"
#include "strong-ptr.h"
#include "allocator.h"
#include "value.h"
#include "range.h"
#include "vector.h"


namespace lib {

  TP<ssize_t N>
  struct alloc_vector : allocator {


    alloc_vector( cstr name ) { 

      auto data = new data{};

      for( auto i : range{ 0, $array_size( data->name ) - 1 } ) 
        
        if( name[ i ] ) data->name[ i ] = name[ i ];

      _data = strong_ptr< data_t >{ data, data->name };
    }

    alloc_vector( alloc_vector const& other ) : _data{ other._data.lock() } { }

    alloc_vector( alloc_vector&& other ) : _data{ move( other._data ) } { }

    alloc_vector& operator=( alloc_vector other ) { 

      _data = move( other._data );
      
      return $this;
    }

    static auto create( cstr name ) { 
      
      return value< allocator >::create< alloc_vector >( name ); 
    }

    value< allocator > get_copy() const override {

      return value< allocator >::create< alloc_vector >( $this );
    }

    void* alloc( ssize_t sz ) override {

      if( _data->list_free.size() ) {

        log::memory, name(), " alloc( ", sz, " ), new = ", _data->list_new.size();
        log::memory, ", free = ", _data->list_free.size() - 1, ", unit size = ", N, log::endl;

        return _data->list_free.pop_back();
      }

     _data->list_new.emplace_back();

     log::memory, name(), " alloc( ", sz, " ), new = ", _data->list_new.size() + 1;
     log::memory, ", free = ", _data->list_free.size(), ", unit size = ", N, log::endl;

     return (void*) &_data->list_new.back();
    }

    void free( void* ptr, ssize_t sz ) override { 

      log::memory, name(), " free( ", ptr, ", ", sz, " ), new = ", _data->list_new.size();
      log::memory, ", free = ", _data->list_free.size() + 1, ", unit size = ", N, log::endl;

      _data->list_free.push_back( ptr );
    }

    ssize_t size() const override { return _data->list_new.size() * $size( data_unit ); }

    ssize_t available() const override { return _data->list_free.size() * $size( data_unit ); }

    cstr name() const override { return _data->name; }


    struct data_unit {

      constexpr static bool is_primitive = true;

      char data[ N ];
    };

    struct data {

      vector< data_unit > list_new;

      vector< void* > list_free;

      char name[ 16 ];
    };

    strong_ptr< data > _data;
 };

}


