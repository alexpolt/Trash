#pragma once

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T>
  struct handle : nocopy {

    using value_type = T;
    using deleter_t = void(*)( value_type );

    handle() { }

    handle( handle&& other ) : _h{ move( other._h ) }, _d{ move( other._d ) } { }

    handle( value_type h, deleter_t d ) : _h{ move( h ) }, _d{ move( d ) } { }

    auto& operator=( handle&& other ) { 

      _h = move( other._h );
      _d = move( other._d );

      return $this;
    }

    ~handle() { if( _d ) _d( move( _h ) ); _d = nullptr; }

    auto& get() { return _h; }
    auto& get() const { return _h; }

    operator value_type&() { return _h; }

    explicit operator bool() const { return _h; }

    value_type _h{};
    deleter_t _d{};
  };

}


