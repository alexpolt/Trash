#pragma once

#include <cstring>

#include "new.h"
#include "assert.h"
#include "macros.h"
#include "types.h"
#include "range.h"
#include "memory.h"
#include "algo.h"
#include "iterator.h"
#include "to-string.h"
#include "alloc-default.h"
#include "value.h"


namespace lib {



  TP<TN T, bool is_string = false>
  struct vector;


  using vector_b = vector< byte >;
  using vector_i = vector< int >;
  using vector_f = vector< float >;
  using vector_d = vector< double >;


  TP<TN T, bool is_string>
  struct vector {

    static constexpr ssize_t value_size = $size( T );

    using value_type = T;
    using size_type = ssize_t;
    using pointer = T*;
    using reference = T&;
    using const_pointer = T const*;
    using const_reference = T const&;
    using iterator = vector_iterator< vector >;
    using const_iterator = vector_iterator< vector const >;
    using allocator = value< allocator >;


    TP<TN> struct is_vector : type_false { };

    TP<TN U, bool is_str>
    struct is_vector< vector< U, is_str > > : type_true { };

    TP<TN U>
    static constexpr bool is_vector_v = is_vector< U >::value;

    static allocator create_alloc() { return alloc_default::create( "vector" ); }


    vector( allocator alloc = create_alloc() ) : _alloc { move( alloc ) } { }

    explicit vector( ssize_t size, allocator alloc = create_alloc() ) : vector{ move( alloc ) } {

      reserve( size ); 
    }

    explicit vector( ssize_t size, value_type value, 

                        allocator alloc = create_alloc() ) : vector{ move( alloc ) } {
      
      reserve( size ); 

      range{ 0, size } $do { push_back( value ); };
    }

    vector( vector const& other ) {
      
      _alloc = other._alloc->get_copy();

      clear();

      $this << other;
    }

    vector( vector&& other ) :
      _data{ move( other._data ) }, 
      _capacity{ move( other._capacity ) },
      _index{ move( other._index ) },
      _alloc{ move( other._alloc ) } { }

    TP<TN... UU>
    explicit vector( UU&&... args ) : vector{} {

      reserve( sizeof...( args ) );
      
      char dummy[] { ( $this << forward< UU >( args ), '\0' )... }; (void) dummy; 
    }

    vector( cstr other, allocator alloc = create_alloc() ) : vector{ alloc } { $this << other; }

    TP<TN U, ssize_t M>
    vector( U ( &args)[ M ], allocator alloc = create_alloc() ) : vector{ alloc } { $this << args; }


    vector& operator=( vector&& other ) {
      
      destroy();

      _data = move( other._data );
      _capacity = move( other._capacity );
      _index = move( other._index );
      _alloc = move( other._alloc );

      return $this; 
    }

    vector& operator=( vector const& other ) {
      
      clear();

      $this << other;

      return $this; 
    }

    TP<TN U>
    vector& operator=( U&& other ) {
      
      clear();

      $this << forward< U >( other ); 

      return $this; 
    }

    TP<TN U, ssize_t M>
    vector& operator=( U ( &args)[ M ] ) {
      
      clear(); 
      
      reserve( M ); 
      
      $this << args; 
      
      return $this; 
    }

    ~vector() { destroy(); }

    void destroy() { clear(); free(); }

    void free() {

      if( capacity() == 0 ) return;

      _alloc->free( move( _data ), value_size * capacity() );

      _capacity = 0; 
    }

    void clear() {

      if( size() and not is_primitive_v< value_type > ) {

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

      } else if( size() ) {

        memset( data(), 0, size_bytes() );
      }

      _index = 0;
    }

    void reserve( ssize_t size_new = 0 ) {

      size_new = max( 1, size_new );

      if( size_new <= available() ) return;

      if( capacity() > 0 ) {

        ssize_t size_calc = capacity() < ( 1 << 22 ) ? capacity() * 4 : capacity() * 7 / 4;

        $assert( size_calc > 0, "vector reserve overflow ( capacity )" );

        size_new = max( size_new, size_calc );
      }
      
      auto size_new_bytes = value_size * size_new;

      $assert( size_new_bytes > 0, "vector reserve overflow ( bytes )" );

      value_type* data_new = nullptr;

      data_new = (value_type*) _alloc->alloc( size_new_bytes );

      if( size() and not is_primitive_v< value_type > ) {

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
        
      } else if( size() ) {
        
        memcpy( data_new, data(), size_bytes() );
      }

      free();

      _data = data_new;

      _capacity = size_new;
    }

    void resize( ssize_t size_new ) {

      if( size_new <= size() ) return;

      if( size_new > capacity() ) reserve( size_new );

      if( not is_primitive_v< value_type > ) {

        for( auto i : range{ size(), size_new } )

          new( &data()[ i ] ) value_type{};

      } else {

        memset( data() + size(), 0, ( size_new - size() ) * value_size );
      }

      set_size( size_new );
    }

    void check_size() const { $assert( size() > 0, "vector is empty" ); }

    auto& front() { check_size(); return _data[ 0 ]; }
    auto& back() { check_size(); return _data[ _index - 1 ]; }
    auto const& front() const { check_size(); return _data[ 0 ]; }
    auto const& back() const { check_size(); return _data[ _index - 1 ]; }

    void push_back( value_type value ) {

      if( available() == 0 ) reserve();

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

      if( it == --end() ) {

        pop_back();

        return begin();
      }

      *it = pop_back();

      return it; 
    }

    auto& operator[]( ssize_t index ) { $assert( index < size(), "out of bounds" ); return _data[ index ]; }

    auto const& operator[]( ssize_t index ) const { $assert( index < size(), "out of bounds" ); return _data[ index ]; }


    TP<TN U = char, TN = enable_if_t< is_string and $size( U )>>
    auto& operator<<( value_type* other ) { 

      return operator<<( ( value_type const* ) other );
    }

    TP<TN U = char, TN = enable_if_t< is_string and $size( U )>>
    auto& operator<<( value_type const* other ) { 

      if( other == nullptr ) return $this;

      if( size() > 0 ) pop_back();

      while( *other ) push_back( *other++ );

      push_back( '\0' );

      return $this;
    }

    TP<TN U, cstr (*)( U const& ) = to_string, 
              TN = enable_if_t< is_string and not is_same_v< U, value_type* > >>
    auto& operator<<( U other ) {
      
      $this << to_string( other );

      return $this; 
    }


    TP<TN U = char, TN = enable_if_t< not is_string and $size( U )>>
    auto& operator<<( value_type other ) { 

      push_back( move( other ) ); 

      return $this; 
    }

    TP<TN U, TN = enable_if_t< not is_string and not is_container_v< U > >, TN = void>
    auto& operator<<( U other ) { 
      
      push_back( move( other ) ); 
      
      return $this; 
    }

    TP<TN U0, 
       TN U1 = select_t< not is_const_v< no_ref_t< U0 > > and not is_ref_v< U0 >, move_t, copy_t >,
       TN = enable_if_t< not is_string and 
                              is_container_v< no_cref_t< U0 > > and 
                                not is_vector_v< no_cref_t< U0 > > >, TN = void>
    auto& operator<<( U0&& other ) { 

      ssize_t size = other.size();

      if( size == 0 ) return $this;

      reserve( size );

      for( auto& e : other )

          push_back( U1::copymove( e ) );

      return $this;
    }

    TP<TN U0,
       TN U1 = select_t< not is_const_v< no_ref_t< U0 > > and not is_ref_v< U0 >, move_t, copy_t >,
       TN = enable_if_t< not is_string and is_vector_v< no_cref_t< U0 > > and
                         ( not is_primitive_v< typename no_cref_t< U0 >::value_type > or 
                                  ( is_primitive_v< typename no_cref_t< U0 >::value_type > and 
                                    value_size not_eq no_cref_t< U0 >::value_size )
                         ) >>

    auto& operator<<( U0&& other ) {

      ssize_t size = other.size();

      if( size == 0 ) return $this;

      reserve( size );

      if( size >= 4 ) {

        for( auto i : range{ 0, size/4 } ) {

          auto index = i * 4;

          push_back( U1::copymove( other[ index + 0 ] ) );
          push_back( U1::copymove( other[ index + 1 ] ) );
          push_back( U1::copymove( other[ index + 2 ] ) );
          push_back( U1::copymove( other[ index + 3 ] ) );
        } 
      }

      auto rest = size % 4;

      for( auto index : range{ size - rest, size } ) 

         push_back( U1::copymove( other[ index ] ) );

      if( not is_const_v< no_ref_t< U0 > > and not is_ref_v< U0 > ) other._index = 0;

      return $this; 
    }

    TP<TN U, bool is_str, TN = enable_if_t< is_primitive_v< U > and $size( U ) == value_size>>
    auto& operator<<( vector< U, is_str > const& other ) { 

      auto size_other = other.size();

      if( size_other == 0 ) return $this;

      reserve( size_other );
      
      auto index = size();

      auto null_char = 0;

      if( is_str and size() > 0 ) null_char = 1;

      void *to = &data()[ index - null_char ], *from = other.data();

      memcpy( to, from, size_other * value_size );

      _index += size_other - null_char;
      
      return $this;
    }


    TP<TN U, ssize_t M, TN = enable_if_t< not is_string and sizeof( U ) >>
    auto& operator<<( U ( &other)[ M ] ) { 

      for( auto& e : other ) push_back( e ); 
      
      return $this; 
    }


    TP<TN U>
    void append( U const* data, ssize_t size ) { 
      
      for( auto i : range{ 0, size } ) push_back( data[ i ] );
    }


    iterator begin() { return iterator{ $this, 0 }; }
    iterator end() { return iterator{ $this, size() }; }

    const_iterator begin() const { return const_iterator{ $this, 0 }; }
    const_iterator end() const { return const_iterator{ $this, size() }; }

    const_iterator cbegin() const { return const_iterator{ $this, 0 }; }
    const_iterator cend() const { return const_iterator{ $this, size() }; }

    void set_size( size_type size ) { _index = size; }

    void set_allocator( allocator alloc ) { _alloc = move( alloc ); }

    allocator get_allocator() { return _alloc->get_copy(); }

    auto data() const { return _data; }
    auto size() const { return _index; }
    auto capacity() const { return _capacity; }
    auto available() const { return capacity() - size(); }
    auto size_bytes() const { return size() * value_size; }
    bool empty() const { return size() == 0; }


    pointer _data{};
    size_type _capacity{};
    size_type _index{};
    allocator _alloc;
  };



  TP<TN T>
  bool operator==( vector< T, true > const& left, cstr right ) {

    ssize_t size_right = strlen( right ) + 1;

    if( left.size() != size_right ) return false;

    return memcmp( left.data(), right, left.size() ) == 0;
  }

  TP<TN T>
  bool operator==( cstr left, vector< T, true > const& right ) {
  
    return operator==( right, left );
  }

  TP<TN T, bool is_string, TN = enable_if_t< is_primitive_v< T > >>
  bool operator==( vector< T, is_string > const& left,  vector< T, is_string > const& right ) {

    if( left.size() != right.size() ) return false;

    return memcmp( left.data(), right.data(), left.size() ) == 0;
  }


  TP<TN T0, TN T1, bool is_str0, bool is_str1,
      TN = disable_if_t< is_same_v< T0, T1 > and is_primitive_v< T0 > and is_primitive_v< T1 > >>
  bool operator==( vector< T0, is_str0 > const& left, vector< T1, is_str1 > const& right ) {

    if( left.size() != right.size() ) return false;

    for( auto i : range{ 0, left.size() } )

      if( left[ i ] != right[ i ] ) return false;

    return true;
  }



}




