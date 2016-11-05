#pragma once

#include <cstdio>

#include "macros.h"
#include "types.h"
#include "value.h"


namespace lib {


  struct allocator {

    virtual ~allocator() { }

    virtual char* alloc( ssize_t ) = 0;

    virtual void free( void*, ssize_t ) = 0;

    virtual ssize_t size() const = 0;

    virtual ssize_t available() const = 0;

    virtual value< allocator > get_copy() const = 0;
  };

}


