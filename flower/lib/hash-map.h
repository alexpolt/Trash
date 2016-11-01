#pragma once

#include "assert.h"
#include "macros.h"
#include "types.h"
#include "error.h"
#include "range.h"
#include "hash.h"
#include "vector.h"

namespace lib {

  struct error_hash : error { using error::error; };

  #define $error_hash( $0 ) lib::error_hash{ __FILE__, __LINE__, __func__, $0 }


  TP<TN K, TN V, bool is_64bit = false>
  struct hash_map {

    using value_type = V;
    using key_type = K;
    using hasher = select_t< is_64bit, hash64< K >, hash32< K > >;
    using hash_type = typename hasher::value_type;
    using pointer = V*;
    using reference = V&;
    using iterator = vector_iterator< vector< value_type > >;
    using const_iterator = vector_iterator< vector< value_type > const >;

    static constexpr hash_type size_max = ( is_64bit ? 1ll << 56 : 1 << 24 ) - 2;
    static constexpr int try_max = 5;
    static constexpr int recursive_rehashes_max = 3;

    hash_map() { }

    hash_map( ssize_t size ) : _hash_table{ 3 * size }, _keys{ size }, _values{ size } { 

      _hash_table.resize( _hash_table.capacity() );      
    }

    #define $hash_map_hash1( $0, $1, $2 ) ( $0 + $1 ) % $2
    #define $hash_map_hash2( $0, $1, $2 ) ( ~$0 + $1 ) % $2
    #define $hash_map_hash3( $0, $1, $2 ) ( ~$0*$0 + $1 ) % $2

    iterator operator[]( key_type const& key ) {

      auto size_table = _hash_table.size();

      auto hash0 = hasher::get_hash( key );

      if( _keys.size() > 0 ) {

        for( auto i : range{ 0, try_max } ) {
          
          auto hash1 = $hash_map_hash1( hash0, i, size_table );
          auto hash2 = $hash_map_hash2( hash0, i, size_table );
          auto hash3 = $hash_map_hash3( hash0, i, size_table );

          if( hash1 == hash2 or hash1 == hash3 or hash2 == hash3 ) {

            hash0 = hasher::get_hash( key, i + 1 );
            
            continue;
          }

          auto& hvalue1 = _hash_table[ hash1 ];
          auto& hvalue2 = _hash_table[ hash2 ];
          auto& hvalue3 = _hash_table[ hash3 ];

          if( not hvalue1.get_refcnt() or
                not hvalue2.get_refcnt() or
                  not hvalue3.get_refcnt() ) continue;

          ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

          if( index >= _keys.size() ) continue;

          if( not equal( key, _keys[ index ] ) ) continue;

          return _values.begin() + index;
        }
      }

      return _values.end();
    }

    const_iterator operator[]( key_type const& key ) const {

      return ( const_cast< hash_map& > ( $this ) )[ key ];
    }

    TP<TN U>
    bool insert( key_type key, U&& value, bool rehash_count = 0 ) {

      auto size_table = _hash_table.size();

      auto hash0 = hasher::get_hash( key );
      
      if( size_table == size_max ) $throw $error_hash( "maximum hash table size" );
      
      for( auto i : range{ 0, try_max } ) {

        if( not size_table ) break;

        auto hash1 = $hash_map_hash1( hash0, i, size_table );
        auto hash2 = $hash_map_hash2( hash0, i, size_table );
        auto hash3 = $hash_map_hash3( hash0, i, size_table );

        if( hash1 == hash2 or hash1 == hash3 or hash2 == hash3 ) {

          hash0 = hasher::get_hash( key, i + 1 );

          continue;
        }

        auto& hvalue1 = _hash_table[ hash1 ];
        auto& hvalue2 = _hash_table[ hash2 ];
        auto& hvalue3 = _hash_table[ hash3 ];


        if( hvalue1.get_refcnt() and
              hvalue2.get_refcnt() and
                hvalue3.get_refcnt() ) {

          ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();
          

          if( index >= _keys.size() ) continue;


          if( equal( key, _keys[ index ] ) ) return false;
        }

        hash_node* hash_ptr = nullptr;

        if( not hvalue1.get_refcnt() ) hash_ptr = &hvalue1;
        else if( not hvalue2.get_refcnt() ) hash_ptr = &hvalue2;
        else if( not hvalue3.get_refcnt() ) hash_ptr = &hvalue3;
        else continue;

        hvalue1.set_refcnt( hvalue1.get_refcnt() + 1 );
        hvalue2.set_refcnt( hvalue2.get_refcnt() + 1 );
        hvalue3.set_refcnt( hvalue3.get_refcnt() + 1 );

        auto index = _keys.size();

        hash_type index_hashed = index xor hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        hash_ptr->set_hash( index_hashed );
        
        _keys << move( key );

        if( not rehash_count ) {
          
          _values << forward< U >( value );
        }

        return true;
      }

      rehash( rehash_count );

      auto result = insert( key, move( value ) );

      $assert( result, "hash insert unexpectedly failed" );

      return result;
    }

    void rehash( int rehash_count ) {

      _hash_table.reserve();
      _hash_table.clear();
      _hash_table.resize( _hash_table.capacity() );

      if( not _keys.size() ) return;

      ++rehash_count;

      $assert( rehash_count <= recursive_rehashes_max, "rehashing failed, try increasing table size or try_max of the hash_map" );

      ++_rehashes;

      vector< key_type > keys_orig{ _keys.size() };

      swap( keys_orig, _keys );

      bool result = true;

      for( auto i : range{ 0, keys_orig.size() } ) {

        result = result and insert( move( keys_orig[ i ] ), _values[ i ], rehash_count );
      }

      keys_orig.set_size( 0 );

      $assert( result, "rehash failed for some reason" );
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

    auto size() const { return _values.size(); }
    auto empty() const { return size() == 0; }
    auto rehashes() const { return _rehashes; }


    struct hash_node {

      static constexpr bool is_primitive = true;

      static constexpr hash_type _mask = is_64bit ? 0x00FF'FFFF'FFFF'FFFF : 0x0FFF'FFFF;
      static constexpr hash_type _mask_shift = is_64bit ? 56 : 28;

      auto get_hash() { return _hash & _mask; }
      void set_hash( hash_type hash ) { _hash = ( _hash & ~_mask ) | ( hash & _mask ); }
      auto get_refcnt() { return _hash >> _mask_shift; }
      void set_refcnt( int count ) { _hash = ( _hash & _mask ) | ( count << _mask_shift ); }

      hash_type _hash;
    };

    ssize_t _rehashes{};
    vector< hash_node > _hash_table;
    vector< key_type > _keys;
    vector< value_type > _values;
  };


}


