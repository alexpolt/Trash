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

    bool operator==( iterator other ) { return _index == other._index; }
    bool operator!=( iterator other ) { return _index != other._index; }
    bool operator<( iterator other ) { return _index < other._index; }

    auto operator+( ssize_t index ) { return iterator{ _object, _index + index }; }
    auto operator-( ssize_t index ) { return iterator{ _object, _index - index }; }

    auto operator+=( ssize_t index ) { return iterator{ _object, _index + index }; }
    auto operator-=( ssize_t index ) { return iterator{ _object, _index - index }; }

    auto operator++( int ) { return iterator{ _object, _index++ }; }
    auto operator--( int ) { return iterator{ _object, _index-- }; }

    auto operator++() { return iterator{ _object, ++_index }; }
    auto operator--() { return iterator{ _object, --_index }; }

    operator vector_iterator< T0 const >() { return vector_iterator< T0 const >{ _object, _index }; }

    operator bool() { return _index not_eq _object.size(); }

    T0& _object;

    size_type _index;
  };

}


