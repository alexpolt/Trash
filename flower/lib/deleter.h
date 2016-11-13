#pragma once

#include "macros.h"
#include "types.h"
#include "value.h"


namespace lib {


  struct deleter {

    virtual ~deleter() { }

    virtual void operator()( void* ) = 0;
  };

  using deleter_t = value< deleter >;

  TP<TN T>
  struct deleter_default : deleter {

    auto static create() { return value< deleter >::create< deleter_default >(); }

    void operator()( void* ptr ) override {

      delete (T*) ptr;
    }
  };

  TP<TN T>
  struct deleter_default< T[] > : deleter {

    auto static create() { return value< deleter >::create< deleter_default >(); }

    void operator()( void* ptr ) override {

      delete[] (T*) ptr;
    }
  };


}


