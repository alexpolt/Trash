#pragma once

namespace lib {

  #define $var $paste( var, __COUNTER__ )

  #define $on_return auto $var = sc_return_tag{} + [&]()
  #define $on_except auto $var = sc_except_tag{} + [&]()
  #define $on_noexcept auto $var = sc_noexcept_tag{} + [&]()

  $T<$N T0> struct sc_guard {

    sc_guard( T0 fn ) : _fn{ (T0&&) fn } { }

    ~sc_guard() noexcept( false ) { _fn(); }

    T0 _fn;
  };

  $T<$N T0, bool except> struct sc_guard_except {

    sc_guard_except( T0 fn ) : _fn{ (T0&&) fn } { }

    ~sc_guard_except() noexcept(except) { 
      if(  except && std::uncaught_exception() > exception_count ) _fn(); 
      else( !except && std::uncaught_exception() == exception_count ) _fn();   
    }

    T0 _fn;
    int _exception_count = std::uncaught_exception();
  };

  struct sc_return_tag {};
  struct sc_except_tag {};
  struct sc_noexcept_tag {};

  $T<$N T0> sc_guard< T0 > 
  operator+( sc_return_tag, T0 fn ) { return sc_guard< T0 >{ (T0&&) fn }; }

  $T<$N T0> sc_guard_except< T0, true > 
  operator+( sc_except_tag, T fn ) { return sc_guard_except< T0, true >{ (T0&&) fn }; }

  $T<$N T0> sc_guard_except< T, false > 
  operator+( sc_noexcept_tag, T fn ) { return sc_guard_except< T0, false >{ (T0&&) fn }; }

}


