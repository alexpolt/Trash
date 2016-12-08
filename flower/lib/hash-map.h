#pragma once

#include "assert.h"
#include "macros.h"
#include "types.h"
#include "error.h"
#include "range.h"
#include "hash.h"
#include "vector.h"
#include "algo.h"
#include "alloc-default.h"
#include "value.h"


namespace lib {

  struct error_hash : error { using error::error; };

  #define $error_hash( $0 ) lib::error_hash{ $file_line, $0 }


  TP<TN K, TN V, bool is_64bit = false>
  struct hash_map {

    using size_type = ssize_t;
    using value_type = V;
    using key_type = K;
    using hasher = select_t< is_64bit, hash64< K >, hash32< K > >;
    using hash_type = typename hasher::value_type;
    using pointer = V*;
    using reference = V&;
    using iterator = vector_iterator< vector< value_type > >;
    using const_iterator = vector_iterator< vector< value_type > const >;
    using allocator = value< allocator >;

    static constexpr int _try_max = 8;
    static constexpr int _reserve_init = 3;
    static constexpr int _hash_functions = 3;
    static constexpr ssize_t _invalid_index = -1;
    static constexpr hash_type _size_max = ( is_64bit ? 1ll << 56 : 1 << 28 ) / 3;

    static allocator create_alloc() { return alloc_default::create( "hash_map" ); }

    hash_map( allocator alloc = create_alloc() ) : 
      _hash_table{ alloc->get_copy() }, 
      _keys{ alloc->get_copy() }, 
      _values{ alloc->get_copy() }, 
      _index_free{ alloc->get_copy() },
      _key_deleted{ alloc->get_copy() },
      _alloc{ move( alloc ) } { };

    hash_map( ssize_t size, allocator alloc = create_alloc() ) : hash_map{ move( alloc ) } {
      
      reserve( size );
    }

    void init_hash( hash_type hash0, uint i, hash_type& hash1, hash_type& hash2, hash_type& hash3 ) {

      auto size_table = _hash_table.size();

      hash1 = ( hash0 + i ) % size_table;
      hash2 = ( ~hash0 + 1 + i ) % size_table;
      hash3 = ( hash0 * hash0 + 2 + i ) % size_table;
    }

    iterator find( key_type const& key ) { return $this[ key ]; }

    iterator operator[]( key_type const& key ) {

      auto hash0 = hasher::get_hash( key );

      if( size() > 0 ) {

        for( auto i : range{ 0, _try_max } ) {

          hash_type hash1{}, hash2{}, hash3{};

          init_hash( hash0, i, hash1, hash2, hash3 );

          if( hash1 == hash2 or hash1 == hash3 or hash2 == hash3 ) continue;
          
          auto& hvalue1 = _hash_table[ hash1 ];
          auto& hvalue2 = _hash_table[ hash2 ];
          auto& hvalue3 = _hash_table[ hash3 ];

          if( hvalue1.get_refcnt() == 0 or
                hvalue2.get_refcnt() == 0 or
                  hvalue3.get_refcnt() == 0 ) continue;

          ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

          if( index >= _keys.size() ) continue;

          if( _key_deleted[ index ] ) continue;

          if( not equal( key, _keys[ index ] ) ) continue;

          return _values.begin() + index;
        }
      }

      return _values.end();
    }

    const_iterator operator[]( key_type const& key ) const {

      return ( const_cast< hash_map& > ( $this ) )[ key ];
    }

    iterator insert( key_type key, value_type value ) {

      if( size() == _size_max ) 

        $throw $error_hash( "maximum hash table size" );

      auto index = get_new_index( key );

      if( index == _invalid_index ) return _values.end();

      _keys[ index ] = move( key );
      _key_deleted[ index ] = false;
      _values[ index ] = move( value );

      return _values.begin() + index;
    }

    ssize_t get_new_index( key_type const& key, ssize_t rehash_index = _invalid_index ) {

      if( _hash_table.size() == 0 ) reserve( _reserve_init );

      hash_type hash0 = hasher::get_hash( key );

      $assert( hash0, "hash of the key should not be zero" );
      
      for( auto i : range{ 0, _try_max } ) {

        hash_type hash1{}, hash2{}, hash3{};

        init_hash( hash0, i, hash1, hash2, hash3 );

        if( hash1 == hash2 or hash1 == hash3 or hash2 == hash3 ) continue;

        auto& hvalue1 = _hash_table[ hash1 ];
        auto& hvalue2 = _hash_table[ hash2 ];
        auto& hvalue3 = _hash_table[ hash3 ];

        if( hvalue1.get_refcnt() and
              hvalue2.get_refcnt() and
                hvalue3.get_refcnt() ) {

          ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();
          
          if( index >= _keys.size() ) continue;

          if( _key_deleted[ index ] ) continue;

          if( equal( key, _keys[ index ] ) ) return _invalid_index;
        }

        hash_node* hash_ptr = nullptr;

        if( hvalue1.get_refcnt() == 0 ) hash_ptr = &hvalue1;
        else if( hvalue2.get_refcnt() == 0 ) hash_ptr = &hvalue2;
        else if( hvalue3.get_refcnt() == 0 ) hash_ptr = &hvalue3;
        else continue;

        hvalue1.set_refcnt( hvalue1.get_refcnt() + 1 );
        hvalue2.set_refcnt( hvalue2.get_refcnt() + 1 );
        hvalue3.set_refcnt( hvalue3.get_refcnt() + 1 );

        ssize_t index = _invalid_index;

        if( rehash_index != _invalid_index ) {

          index = rehash_index;        

        } else if( not _index_free.empty() ) {

          index = _index_free.pop_back();

        } else { 

          index = _keys.size();

          _keys.emplace_back();
          _key_deleted.emplace_back();
          _values.emplace_back();
        }
 
        hash_type index_hashed = index xor hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        hash_ptr->set_hash( index_hashed );
        
        return index;
      }
      
      reserve();

      rehash( rehash_index );

      auto index = get_new_index( key );

      $assert( index != _invalid_index, "hash insert unexpectedly failed" );

      return index;
    }

    void rehash( ssize_t rehash_index = _invalid_index ) {

      if( size() == 0 ) return;

      $assert( rehash_index == _invalid_index, "no recursive rehashing, try increasing table size or try_max of the hash_map" );

      ++_rehashes;

      bool result = true;

      for( auto i : range{ 0, _keys.size() } ) {

        if( _key_deleted[ i ] ) continue;

        auto index = get_new_index( _keys[ i ], i );

        result = result and index != _invalid_index;
      }

      $assert( result, "rehash failed for some reason" );
    }

    void reserve( ssize_t size = 0 ) {

      _hash_table.reserve( _hash_functions * size );
      _hash_table.clear();
      _hash_table.resize( _hash_table.capacity() );

      _keys.reserve( size );
      _key_deleted.reserve( size );
      _values.reserve( size );
    }

    iterator erase( key_type const& key ) {

      if( size() == 0 ) return _values.end();

      auto hash0 = hasher::get_hash( key );

      for( auto i : range{ 0, _try_max } ) {

        hash_type hash1{}, hash2{}, hash3{};

        init_hash( hash0, i, hash1, hash2, hash3 );

        if( hash1 == hash2 or hash1 == hash3 or hash2 == hash3 ) continue;
        
        auto& hvalue1 = _hash_table[ hash1 ];
        auto& hvalue2 = _hash_table[ hash2 ];
        auto& hvalue3 = _hash_table[ hash3 ];

        if( hvalue1.get_refcnt() == 0 or
              hvalue2.get_refcnt() == 0 or
                hvalue3.get_refcnt() == 0 ) continue;

        ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        if( index >= _keys.size() ) continue;

        if( _key_deleted[ index ] ) continue;

        if( not equal( key, _keys[ index ] ) ) continue;

        hvalue1.set_refcnt( hvalue1.get_refcnt() - 1 );
        hvalue2.set_refcnt( hvalue2.get_refcnt() - 1 );
        hvalue3.set_refcnt( hvalue3.get_refcnt() - 1 );

        if( hvalue1.get_refcnt() == 0 ) hvalue1.set_hash( 0 );
        if( hvalue2.get_refcnt() == 0 ) hvalue2.set_hash( 0 );
        if( hvalue3.get_refcnt() == 0 ) hvalue3.set_hash( 0 );

        _keys[ index ] = key_type{};
        _key_deleted[ index ] = true;
        _values[ index ] = value_type{};
        
        _index_free << index;

        return _values.begin() + index + 1;
      }
      
      return _values.end();
    }

    iterator erase( iterator it ) {

      return erase( _keys[ it.get_index() ] );
    }

    iterator begin() { return _values.begin(); }
    iterator end() { return _values.end(); }

    const_iterator begin() const { return _values.begin(); }
    const_iterator end() const { return _values.end(); }

    const_iterator cbegin() const { return _values.begin(); }
    const_iterator cend() const { return _values.end(); }

    auto& values() { return _values; }
    auto& keys() { return _keys; }

    auto& values() const { return _values; }
    auto& keys() const { return _keys; }

    auto& key( iterator it ) { return _keys[ it.get_index() ]; }
    auto& key( iterator it ) const { return _keys[ it.get_index() ]; }

    auto size() const { return _values.size() - _index_free.size(); }
    auto size_max() const { return _size_max; }
    auto empty() const { return size() == 0; }
    auto rehashes() const { return _rehashes; }

    void set_allocator( allocator alloc ) { _alloc = move( alloc ); }

    allocator& get_allocator() { return _alloc; }


    struct hash_node {

      static constexpr bool is_primitive = true;

      static constexpr hash_type _mask = is_64bit ? 0x00FF'FFFF'FFFF'FFFF : 0x0FFF'FFFF;
      static constexpr int _mask_shift = is_64bit ? 56 : 28;
      static constexpr int _refcnt_max = ~_mask >> _mask_shift;

      auto get_hash() const { return _hash & _mask; }
      
      void set_hash( hash_type hash ) { _hash = ( _hash & ~_mask ) | ( hash & _mask ); }

      auto get_refcnt() const { return _hash >> _mask_shift; }

      void set_refcnt( int count ) { 

        if( count > _refcnt_max ) count = _refcnt_max;

        _hash = ( _hash & _mask ) | ( count << _mask_shift ); 
      }

      hash_type _hash;
    };

    vector< hash_node > _hash_table;
    vector< key_type > _keys;
    vector< value_type > _values;
    vector< size_type > _index_free;
    vector< bool > _key_deleted;
    allocator _alloc;
    ssize_t _rehashes{};
  };


}


