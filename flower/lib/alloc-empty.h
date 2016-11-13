#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "memory.h"
#include "allocator.h"
#include "value.h"


namespace lib {


  struct alloc_empty : allocator {

    alloc_empty( cstr name ) : _name{ name } { }

    static auto create( cstr name ) { 
      
      return value< allocator >::create< alloc_empty >( name ); 
    }

    value< allocator > get_copy() const override {

      return create( _name );
    }

    void* alloc( ssize_t sz ) override {

      $assert( false, "no memory allocation is allowed for this allocator" );

      return nullptr;
    }

    void free( void* ptr, ssize_t sz ) override { 

      log::memory, name(), " free( ", ptr, ", ", sz, " )", log::endl;

      lib::free( ptr, sz );
    }

    ssize_t size() const override { return 0; }

    ssize_t available() const override { return 0; }

    cstr name() const override { return _name; }

    cstr _name;
 };

}


