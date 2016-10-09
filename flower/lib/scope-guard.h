#pragma once

namespace lib {

  #define $on_return auto $var = lib::sc_return_tag + [&]()

  $T<$N T0> struct sc_guard {

    sc_guard( T0 fn ) : _fn{ (T0&&) fn } { }

    ~sc_guard() noexcept( false ) { _fn(); }

    T0 _fn;
  };

  struct sc_return_t { } sc_return_tag;

  $T<$N T0> sc_guard< T0 > 
  operator+( sc_return_t, T0 fn ) { return sc_guard< T0 >{ (T0&&) fn }; }
  

/*
  #define $on_except auto $var = lib::sc_except_tag + [&]()
  #define $on_noexcept auto $var = lib::sc_noexcept_tag + [&]()

  $T<$N T0, bool except> struct sc_guard_except {

    sc_guard_except( T0 fn ) : _fn{ (T0&&) fn } { }

    ~sc_guard_except() noexcept(except) { 
      if(  except && std::uncaught_exception() > exception_count ) _fn(); 
      else( !except && std::uncaught_exception() == exception_count ) _fn();   
    }

    T0 _fn;
    int _exception_count = std::uncaught_exception();
  };

  struct sc_except_t { } sc_except_tag;
  struct sc_noexcept_t { } sc_noexcept_tag;

  $T<$N T0> sc_guard_except< T0, true > 
  operator+( sc_except_t, T fn ) { return sc_guard_except< T0, true >{ (T0&&) fn }; }

  $T<$N T0> sc_guard_except< T0, false > 
  operator+( sc_noexcept_t, T fn ) { return sc_guard_except< T0, false >{ (T0&&) fn }; }
*/

}


