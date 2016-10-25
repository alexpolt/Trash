#pragma once

namespace lib {

  #define $on_return auto $var = lib::sc_return_tag + [&]()

  TP<TN T0> struct sc_guard {

    constexpr sc_guard( T0 fn ) : _fn{ (T0&&) fn } { }

    ~sc_guard() noexcept( false ) { _fn(); }

    T0 _fn;
  };

  constexpr struct sc_return_t { constexpr sc_return_t() { } } sc_return_tag;

  TP<TN T0> 
  constexpr auto operator+( sc_return_t, T0 fn ) { return sc_guard< T0 >{ (T0&&) fn }; }
  

}


