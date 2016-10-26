#pragma once

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T0>
  struct vector_iterator {

    using iterator = vector_iterator;
    using size_type = typename T0::size_type;

    auto operator=( iterator other ) { 
      
      $assert( &_object == &other._object, "iterators from different objects" );

      _index = other._index; 

      return $this; 
    }

    auto operator->() { return &_object[ _index ]; }
    auto& operator[]( ssize_t index ) { return _object[ _index + index ]; }
    auto& operator*() { return _object[ _index ]; }

    auto operator->() const { return &_object[ _index ]; }
    auto& operator[]( ssize_t index ) const { return _object[ _index + index ]; }
    auto& operator*() const { return _object[ _index ]; }

    bool operator==( iterator other ) const { return _index == other._index; }
    bool operator!=( iterator other ) const { return _index != other._index; }
    bool operator<( iterator other ) const { return _index < other._index; }

    auto operator+( ssize_t index ) const { return iterator{ _object, _index + index }; }
    auto operator-( ssize_t index ) const { return iterator{ _object, _index - index }; }

    auto& operator+=( ssize_t index ) { _index+=index; return $this; }
    auto& operator-=( ssize_t index ) { _index-=index; return $this; }

    auto operator++( int ) { return iterator{ _object, _index++ }; }
    auto operator--( int ) { return iterator{ _object, _index-- }; }

    auto& operator++() { ++_index; return $this; }
    auto& operator--() { --_index; return $this; }

    operator vector_iterator< T0 const >() const { return vector_iterator< T0 const >{ _object, _index }; }

    explicit operator bool() const { return _index not_eq _object.size(); }

    T0& _object;

    size_type _index;
  };

}


