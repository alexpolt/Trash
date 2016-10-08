#pragma once

namespace lib {

  $T<$N T0> struct range_t {
  
    struct iterator {
      operator T0&() { return index; }
      T0 operator *() { return index; }
      bool operator !=( T0 other ) { return index != other; }
      T0 index;
    };

    iterator begin() const { return iterator{ _begin }; }
    iterator end() const { return iterator{ _end }; }

    T0 _begin;
    T0 _end;

  };

  using range = range_t< int >;

}

