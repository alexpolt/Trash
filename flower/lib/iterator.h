#pragma once

#ifdef $CONFIG_STL
# include <iterator>
#endif

#include "macros.h"
#include "types.h"


namespace lib {


  TP<TN T0>
  struct vector_iterator {

    using value_type = typename T0::value_type;
    using pointer = select_t< is_const_v< T0 >, typename T0::const_pointer, typename T0::pointer >;
    using const_pointer = typename T0::const_pointer;
    using reference = select_t< is_const_v< T0 >, typename T0::const_reference, typename T0::reference >;
    using const_reference = typename T0::const_reference;
    using difference_type = typename T0::size_type;
    using iterator = vector_iterator;
    using size_type = typename T0::size_type;

    auto operator=( iterator other ) { 
      
      $assert( &_object == &other._object, "iterators from different objects" );

      _index = other._index; 

      return $this; 
    }

    pointer operator->() { return &_object[ _index ]; }
    reference operator[]( ssize_t index ) { return _object[ _index + index ]; }
    reference operator*() { return _object[ _index ]; }

    const_pointer operator->() const { return &_object[ _index ]; }
    const_reference operator[]( ssize_t index ) const { return _object[ _index + index ]; }
    const_reference operator*() const { return _object[ _index ]; }

    bool operator==( iterator other ) const { return _index == other._index; }
    bool operator!=( iterator other ) const { return _index != other._index; }
    bool operator<( iterator other ) const { return _index < other._index; }

    auto operator+( ssize_t index ) const { return iterator{ _object, _index + index }; }
    auto operator-( ssize_t index ) const { return iterator{ _object, _index - index }; }

    auto operator-( iterator other ) const { 

      $assert( &_object == &other._object, "iterators from different objects" );

      return _index - other._index;
    }

    auto& operator+=( ssize_t index ) { _index+=index; return $this; }
    auto& operator-=( ssize_t index ) { _index-=index; return $this; }

    auto operator++( int ) { return iterator{ _object, _index++ }; }
    auto operator--( int ) { return iterator{ _object, _index-- }; }

    auto& operator++() { ++_index; return $this; }
    auto& operator--() { --_index; return $this; }

    operator vector_iterator< T0 const >() const { return vector_iterator< T0 const >{ _object, _index }; }

    explicit operator bool() const { return _index != _object.size(); }

    size_type get_index() const { return _index; }

    T0& _object;

    size_type _index;
  };

}

#ifdef $CONFIG_STL

namespace std {

  TP<TN T>
  struct iterator_traits< lib::vector_iterator< T > > {
    using value_type = typename T::value_type;
    using reference = typename T::reference;
    using difference_type = typename T::size_type;
    using iterator_category = random_access_iterator_tag;
   };
}

#endif



