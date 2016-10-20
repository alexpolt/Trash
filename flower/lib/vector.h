#pragma once

#include <cstring>
#include <new>

#include "assert.h"
#include "macros.h"
#include "types.h"
#include "ptr.h"
#include "range.h"
#include "memory.h"
#include "algo.h"

namespace lib {


  TP<TN T0>
  struct vector_iterator;

  TP<TN T0, ssize_t N0>
  auto make_vector( T0 ( &data)[ N0 ] ); 


  TP<TN T0, ssize_t N0 = 0, bool is_string = false>
  struct vector {

    static constexpr ssize_t value_size = $size( T0 );

    using value_type = T0;
    using size_type = ssize_t;
    using pointer = T0*;
    using reference = T0&;
    using iterator = vector_iterator< vector >;
    using const_iterator = vector_iterator< vector const >;

    TP<TN U0>
    struct is_vector;

    TP<TN U0>
    static constexpr bool is_vector_v = is_vector< U0 >::value;


    vector( ) { if( N0 > 0 ) reserve( N0 ); }

    explicit vector( ssize_t size, value_type value = value_type{} ) { 
      
      reserve( size ); 

      for( auto i : range{ 0, size } ) emplace_back( value ), (void)i;

    }

    explicit vector( value_type ( &data)[ N0 ] ) : _data{ data }, _capacity{ N0 } { }

    vector( vector const& other ) { 

      if( other.size() > 0 ) { 

        $this << other;
      }
    }

    vector( vector&& other ) noexcept : 
      _data{ move( other._data ) }, 
      _capacity{ move( other._capacity ) },
      _index{ move( other._index ) } { }

    TP<TN... UU>
    explicit vector( UU&&... args ) { 

      reserve( sizeof...( args ) );
      
      char dummy[] { ( $this << forward< UU >( args ), '\0' )... }; (void) dummy; 
    }

    vector( cstr other ) { $this << other; }

    TP<TN U0, ssize_t M0>
    vector( U0 ( &args)[ M0 ] ) { $this << args; }

    TP<TN U0>
    vector& operator=( U0&& other ) { clear(); $this << forward< U0 >( other ); return $this; }

    TP<TN U0, ssize_t M0>
    vector& operator=( U0 ( &args)[ M0 ] ) { clear(); $this << args; return $this; }

    ~vector() { clear(); free(); }

    void free() {

      if( N0 > 0 ) return;

      if( capacity() == 0 ) return;

      $free( this, $out( _data ), value_size * capacity() );

      _capacity = 0; 
    }

    void clear() {
      
      if( not is_primitive_v< value_type > ) {
        
        if( size() >= 4 ) {

          for( auto i : range{ 0, size()/4 } ) {

            auto index = i * 4;
            
            _data[ index + 0 ].~value_type();
            _data[ index + 1 ].~value_type();
            _data[ index + 2 ].~value_type();
            _data[ index + 3 ].~value_type();
          }
        }

        auto rest = size() % 4;

        for( auto i : range{ size()-rest, size() } ) _data[ i ].~value_type();
      }

      _index = 0;
    }

    void reserve( ssize_t size_new = 0 ) {

      if( N0 > 0 ) return;

      size_new = max( 8, size_new );

      if( size_new <= available() ) return;

      if( capacity() > 0 ) {

        auto size_calc = capacity() < ( 1 << 22 ) ? capacity() * 4 : capacity() * 7 / 4;

        size_new = max( size_new, size_calc );
      }
      
      size_new = ( size_new + 3 ) & ~0b11;

      auto size_new_bytes = value_size * size_new;

      value_type* data_new = (value_type*) $alloc( this, size_new_bytes );

      if( not is_primitive_v< value_type > ) {

        if( size() >= 4 )
          for( auto i : range{ 0, size()/4 } ) {

            auto index = i * 4;
            
            new( &data_new[ index + 0 ] ) value_type{ move( _data[ index + 0 ] ) };
            new( &data_new[ index + 1 ] ) value_type{ move( _data[ index + 1 ] ) };
            new( &data_new[ index + 2 ] ) value_type{ move( _data[ index + 2 ] ) };
            new( &data_new[ index + 3 ] ) value_type{ move( _data[ index + 3 ] ) };
          }

        auto rest = size() % 4;

        for( auto i : range{ size()-rest, size() } )

          new( &data_new[ i ] ) value_type{ move( _data[ i ] ) };
        
      } else

        memcpy( data_new, data(), bytes() );

      free();

      _data = data_new;
      _capacity = size_new;
    }

    void check_size() const { $assert( size() > 0, "vector us empty" ); }

    auto& front() { check_size(); return _data[ 0 ]; }
    auto& back() { check_size(); return _data[ _index - 1 ]; }
    auto const& front() const { check_size(); return _data[ 0 ]; }
    auto const& back() const { check_size(); return _data[ _index - 1 ]; }

    void push_back( value_type value ) {

      if( available() == 0 ) {

        $assert( N0 == 0, "statically sized vector exceeded" );

        reserve();
      } 

      new( &_data[ _index++ ] ) value_type{ move( value ) }; 
    }

    value_type pop_back() {
      
      check_size();

      auto& value = back();

      --_index;

      return move( value );
    }

    TP<TN... UU>
    void emplace_back( UU&&... args ) { push_back( value_type{ forward< UU >( args )... } ); }

    iterator erase( size_type index ) { return erase( begin() + index ); }

    iterator erase( iterator it ) {

      $assert( it < end(), "iterator is not valid" );

      if( it == --end() ) 

        pop_back();

      else 

        *it = pop_back();

      return it; 
    }

    auto& operator[]( ssize_t index ) { $assert( index < size(), "out of bounds" ); return _data[ index ]; }
    auto const& operator[]( ssize_t index ) const { $assert( index < size(), "out of bounds" ); return _data[ index ]; }


    TP<TN U0 = char, TN = enable_if_t< is_string and $size( U0 )>>
    auto& operator<<( value_type const* other ) { 

      if( size() > 0 ) pop_back();

      while( *other ) push_back( *other++ );

      push_back( '\0' );

      return $this;
    }

    TP<TN U0, char = str_format< select_t< is_ptr_v< U0 >, void*, U0 > >::format[ 0 ], 
              TN = enable_if_t< is_string and not is_same_v< U0, value_type* > >>
    auto& operator<<( U0 other ) {
      
      $this << to_string( other );

      return $this; 
    }


    TP<TN U0 = char, TN = enable_if_t< not is_string and $size( U0 )>>
    auto& operator<<( value_type other ) { 

      push_back( move( other ) ); 

      return $this; 
    }

    TP<TN U0, TN = enable_if_t< not is_string and not is_container_v< U0 > >, TN = void>
    auto& operator<<( U0 other ) { 
      
      push_back( move( other ) ); 
      
      return $this; 
    }

    TP<TN U0, TN = enable_if_t< not is_string and 
                                is_container_v< no_cref_t< U0 > > and 
                                not is_vector< no_cref_t< U0 > >::value>, TN = void>
    auto& operator<<( U0&& other_ ) { 
      
      auto &other = const_cast< no_cref_t< U0 >& >( other_ );

      for( auto& e : other ) {

        if( not is_const_v< no_ref_t< U0 > > and not is_ref_v< U0 > )

          push_back( move( e ) );
        else
          
          push_back( e );

      }

      return $this;
    }

    TP< TN U0, 
        TN = enable_if_t< not is_string and is_vector< no_cref_t< U0 > >::value and
                          ( not  is_primitive_v< typename no_cref_t< U0 >::value_type >  or 
                              ( is_primitive_v< typename no_cref_t< U0 >::value_type >  and 
                              value_size not_eq no_cref_t< U0 >::value_size )
                           ) >>

    auto& operator<<( U0&& other_ ) { 

      auto &other = const_cast< no_cref_t< U0 >& >( other_ );

      ssize_t size = other.size();

      reserve( size );

      constexpr bool can_move = not is_const_v< no_ref_t< U0 > > and not is_ref_v< U0 >;

      if( size >= 4 ) 
        for( auto i : range{ 0, size/4 } ) {

          auto index = i * 4;

          if( can_move ) { 

            push_back( move( other[ index + 0 ] ) ); 
            push_back( move( other[ index + 1 ] ) ); 
            push_back( move( other[ index + 2 ] ) ); 
            push_back( move( other[ index + 3 ] ) ); 
            
          } else {

            push_back( other[ index + 0 ] ); 
            push_back( other[ index + 1 ] ); 
            push_back( other[ index + 2 ] ); 
            push_back( other[ index + 3 ] ); 

          }
      } 

      auto rest = size % 4;

      for( auto index : range{ size - rest, size } ) 

        if( can_move ) 

            push_back( move( other[ index ] ) ); 
        else
            push_back( other[ index ] ); 

      if( is_vector_v< U0 > and can_move ) other._index = 0;

      return $this; 
    }

    TP<TN U0, ssize_t M0, bool is_str, TN = enable_if_t< is_primitive_v< U0 > and $size( U0 ) == value_size>>
    auto& operator<<( vector< U0, M0, is_str > const& other ) { 

      auto size_other = other.size();

      reserve( size_other );
      
      auto index = size();

      auto null_char = 0;

      if( is_str and size() > 0 ) null_char = 1;

      void *to = &data()[ index - null_char ], *from = other.data();

      memcpy( to, from, size_other * value_size );

      _index += size_other - null_char;

      return $this;
    }


    TP<TN U0, ssize_t M0, TN = enable_if_t< not is_string and sizeof( U0 ) >>
    auto& operator<<( U0 ( &other)[ M0 ] ) { for( auto& e : other ) push_back( e ); return $this; }


    TP<TN U0>
    void append( U0 const* data, ssize_t size ) { 
      
      for( auto i : range{ 0, size } ) push_back( data[ i ] );
    }


    iterator begin() { return vector_iterator< vector >{ $this, 0 }; }
    iterator end() { return vector_iterator< vector >{ $this, size() }; }

    const_iterator begin() const { return vector_iterator< vector const >{ $this, 0 }; }
    const_iterator end() const { return vector_iterator< vector const >{ $this, size() }; }

    const_iterator cbegin() const { return vector_iterator< vector const >{ $this, 0 }; }
    const_iterator cend() const { return vector_iterator< vector const >{ $this, size() }; }

    auto data() const { return _data; }
    auto size() const { return _index; }
    auto capacity() const { return _capacity; }
    auto available() const { return capacity() - size(); }
    auto bytes() const { return size() * value_size; }
    bool empty() const { return size() == 0; }


    pointer _data{};
    size_type _capacity{};
    size_type _index{};
  };


  TP<TN T0, ssize_t N0, bool is_string>
  TP<TN U0>
  struct vector< T0, N0, is_string >::is_vector : type_false { };

  TP<TN T0, ssize_t N0, bool is_string>
  TP<TN U0, ssize_t M0, bool is_str>
  struct vector< T0, N0, is_string >::is_vector< vector< U0, M0, is_str > > : type_true { };



  TP<TN T0>
  struct vector_iterator {

    using iterator = vector_iterator;

    auto operator=( iterator other ) { 
      
      $assert( &_object == &other._object, "iterators from different objects" );

      _index = other._index; 

      return $this; 
    }

    auto operator->() { return &_object[ _index ]; }
    auto& operator[]( ssize_t index ) { return _object[ _index + index ]; }
    auto& operator*() { return _object[ _index ]; }

    auto operator==( iterator other ) { return _index == other._index; }
    auto operator!=( iterator other ) { return _index != other._index; }
    auto operator<( iterator other ) { return _index < other._index; }

    auto operator+( ssize_t index ) { return iterator{ _object, _index + index }; }
    auto operator-( ssize_t index ) { return iterator{ _object, _index - index }; }

    auto operator+=( ssize_t index ) { return iterator{ _object, _index + index }; }
    auto operator-=( ssize_t index ) { return iterator{ _object, _index - index }; }

    auto operator++(int) { return iterator{ _object, _index++ }; }
    auto operator--(int) { return iterator{ _object, _index-- }; }

    auto operator++() { return iterator{ _object, ++_index }; }
    auto operator--() { return iterator{ _object, --_index }; }

    operator vector_iterator< T0 const >() { return vector_iterator< T0 const >{ _object, _index }; }

    T0& _object;
    ssize_t _index;
  };

  TP<TN T0, ssize_t N0>
  auto make_vector( T0 ( &data)[ N0 ] ) { return vector< T0, N0 >{ data };  }


}




