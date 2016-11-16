#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "memory.h"
#include "allocator.h"
#include "value.h"


namespace lib {


  struct alloc_default : allocator {

    alloc_default( cstr name ) : _name{ name } { }

    static auto create( cstr name ) { 
      
      return value< allocator >::create< alloc_default >( name ); 
    }

    value< allocator > get_copy() const override {

      return create( _name );
    }

    void* alloc( ssize_t sz ) override {

      log::memory, name(), " alloc( ", sz, " )", log::endl;

      return lib::alloc( sz );
    }

    void free( void* ptr, ssize_t sz ) override { 

      log::memory, name(), " free( ", ptr, ", ", sz, " )", log::endl;

      lib::free( ptr, sz );
    }

    ssize_t size() const override { return 0; }

    ssize_t available() const override { return 0; }

    cstr name() const override { return _name; }

    void set_name( cstr name ) override { _name = name; }

    cstr _name;
 };

}


