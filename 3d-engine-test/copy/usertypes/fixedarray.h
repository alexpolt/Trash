#ifndef alexpolt_fixedarray
#  define alexpolt_fixedarray

#include "../config.h"
#include "iterator.h"

namespace alexpolt {

  /*
    Small fixed array type.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  template<class T, size_t N> struct fixed_array {
    T list[ N ];
    size_t pos_m;

    static const ptrdiff_t invalid_position = -1;

    fixed_array() = default;
    ~fixed_array() = default;

    delete_copy_assign( fixed_array )
    delete_copy_assign_move( fixed_array )
    
    bool empty() { return pos_m == 0; }

    void insert( T value ) {
      if( pos_m == N ) 
        assert( !"Reached limit" );
      list[ pos_m++ ] = value;
    }

    void insert( size_t n, T value ) {
      if( n < 0 || n >= pos_m ) assert( !"Invalid position to insert" );
      list[ n ] = value;
    }

    void erase( size_t n ) {
      if( n < pos_m ) {
        while( n + 1 < pos_m ) {
              list[ n ] = list[ n+1 ];
              n++;
        }
        list[ n ] = T{};
        pos_m--;
      }
    }

    void erase( T value ) {
      size_t n = find ( value );
      if( n < 0 || n >= pos_m ) assert( !"Can't find value" );
      erase( n );
    }

    template<class U>
    bool erase( U comp ) {
      size_t n = find ( comp );
      if( n >= 0 && n < pos_m ) {
        list[ n ] = T{};
        return true;
      }
      return false;
    }


    size_t find( T value ) {
      size_t n = invalid_position;
      if( empty() ) return n;
      for( n = 0; n < pos_m; n++ )
        if( list[ n ] == value ) break;
      if( n == pos_m ) n = invalid_position;
    return n;
    }

    template<class U>
    size_t find( U comp ) {
      size_t n = invalid_position;
      if( empty() ) return n;
      for( n = 0; n < pos_m; n++ )
        if( comp( list[ n ] ) ) break;
      if( n == pos_m ) n = invalid_position;
    return n;
    }

    T operator[]( size_t n ) {
      return list[ n ];
    }

    ptrdiff_t notfound() { return invalid_position; }

    size_t size() { return pos_m; }

    template<class> friend struct array_iterator;
    array_iterator<T> begin() { return make_array_iterator( list ); }  
    array_iterator<T> end() { return make_array_iterator( list + pos_m ); }  

  };



}

#endif //alexpolt_fixedarray
