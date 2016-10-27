#pragma once

#include "macros.h"
#include "types.h"
#include "out-ref.h"


namespace lib {


  TP<TN T0>
  struct handle : nocopy {

    using deleter_t = void(*)( T0 );

    handle() { }

    handle( handle&& other ) : _h{ move( other._h ) }, _d{ move( other._d ) } { }

    handle( T0 h, deleter_t d ) : _h{ move( h ) }, _d{ move( d ) } { }

    auto& operator=( handle&& other ) { 

      _h = move( other._h );
      _d = move( other._d );

      return $this;
    }

    ~handle() { if( _d ) _d( move( _h ) ); _d = nullptr; }

    T0& get() { return _h; }

    operator T0&() { return _h; }

    T0 _h{};    
    deleter_t _d{};
  };

}


