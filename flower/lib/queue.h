#pragma once

#include "macros.h"
#include "types.h"
#include "assert.h"
#include "vector.h"
#include "iterator.h"
#include "value.h"
#include "alloc-default.h"


namespace lib {


  TP<TN T, usize_t N = 0>
  struct queue {

    using value_type = T;
    using size_type = usize_t;
    using pointer = T*;
    using reference = T&;
    using const_pointer = T const*;
    using const_reference = T const&;
    using iterator = vector_iterator< queue >;
    using const_iterator = vector_iterator< queue const >;
    using allocator = value< allocator >;

    static_assert( ( N & ( N - 1 ) ) == 0, "queue size should be a power of 2" );

    static auto create_alloc() { return alloc_default::create( "queue" ); }


    queue( ssize_t size = N, allocator alloc = create_alloc() ) : 

      _size_queue( size ), _data{ size, move( alloc ) } {

      if( N == 0 ) 

        $assert( size > 0 and ( size & ( size - 1 ) ) == 0, "queue size should be a power of 2" );

      _data.resize( N );
    }

    void push( value_type value ) {

      $assert( available(), "queue is full" );

      _data[ _right++ % size_queue() ] = move( value );
    }

    value_type pop() {

      $assert( size(), "queue is empty" );

      return move( _data[ _left++ % size_queue() ] );
    }

    auto& operator[]( size_type index ) {

      $assert( _left <= index and index < _right, "out of bounds access" );

      return _data[ index % size_queue() ];
    }

    auto const& operator[]( size_type index ) const {

      $assert( _left <= index and index < _right, "out of bounds access" );

      return _data[ index % size_queue() ];
    }

    auto& operator<<( value_type value ) {
      
      push( move( value ) );

      return $this;
    }

    auto& operator>>( value_type& value ) {

      value = pop();

      return $this;
    }

    iterator begin() { return iterator{ $this, _left }; }
    iterator end() { return iterator{ $this, _right }; }

    const_iterator begin() const { return const_iterator{ $this, _left }; }
    const_iterator end() const { return const_iterator{ $this, _right }; }

    auto size() const { return _right - _left; }
    auto size_queue() const { return _size_queue; }
    auto available() const { return size_queue() - size(); }
    auto index() const { return _right; }

    usize_t _size_queue;
    vector< T > _data;
    usize_t _left{};
    usize_t _right{};
  };


}

