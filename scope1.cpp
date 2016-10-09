#include <cstdio>
#include <exception>
//hello

#define $paste_(a,b) a##b
#define $paste(a,b) $paste_(a,b)
#define $var $paste(var, __COUNTER__)
#define $on_return auto $var = sc_return_tag{} + [&]()
#define $on_except auto $var = sc_except_tag{} + [&]()
#define $on_noexcept auto $var = sc_noexcept_tag{} + [&]()

namespace {

  template<class T> struct sc_guard {
    T fn;
    sc_guard( T fn ) : fn{ (T&&) fn } {}
    ~sc_guard() noexcept(false) { fn(); }
  };

  template<class T, bool except> struct sc_guard_except {
    T fn;
      int exception_count = std::uncaught_exception();
    sc_guard_except( T fn ) : fn{ (T&&) fn } {}
    ~sc_guard_except() noexcept(except) { 
          if( !except && std::uncaught_exception() == exception_count ) fn(); 
          if(  except && std::uncaught_exception() >  exception_count ) fn(); 
  }
  };

  struct sc_return_tag {};
  struct sc_except_tag {};
  struct sc_noexcept_tag {};

  template<class T> sc_guard<T> operator+(sc_return_tag, T fn) {
    return sc_guard<T>{ (T&&) fn };
  }
  template<class T> sc_guard_except<T,true> operator+(sc_except_tag, T fn) {
    return sc_guard_except<T,true>{ (T&&) fn };
  }
  template<class T> sc_guard_except<T,false> operator+(sc_noexcept_tag, T fn) {
    return sc_guard_except<T,false>{ (T&&) fn };
  }
}
struct test2 {
  ~test2() noexcept(false) { throw 1; }
};

struct test {
  ~test() noexcept(false) { 

    $on_except {
      printf("~test %s on except\n",__func__);
    };
    $on_noexcept {
      printf("~test %s on noexcept\n",__func__);
    };
      
    throw new std::exception{}; 
  }
};

int main() {
try {
  $on_noexcept {
    printf("%s on noexcept\n",__func__);
  };

  $on_return {
    printf("exit1\n");
  };
  
  $on_except {
    printf("%s on except\n",__func__);
  };

  test t;
  } catch (...) { printf("caught\n"); }

  // your code goes here
  return 0;
}

