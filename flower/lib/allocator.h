#pragma once

#include "macros.h"
#include "types.h"
#include "value.h"


namespace lib {


  struct allocator {

    virtual ~allocator() { }

    virtual void* alloc( ssize_t ) = 0;

    virtual void free( void*, ssize_t ) = 0;

    virtual ssize_t size() const = 0;

    virtual ssize_t available() const = 0;

    virtual cstr name() const = 0;

    virtual void set_name( cstr ) = 0;

    virtual cstr to_string() const { return name(); }

    virtual value< allocator > get_copy() const = 0;
  };

}


