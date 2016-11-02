#pragma once

#include "assert.h"
#include "macros.h"
#include "types.h"
#include "error.h"
#include "range.h"
#include "hash.h"
#include "vector.h"
#include "algo.h"

namespace lib {

  struct error_hash : error { using error::error; };

  #define $error_hash( $0 ) lib::error_hash{ __FILE__, __LINE__, __func__, $0 }


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

    static constexpr hash_type size_max = ( is_64bit ? 1ll << 56 : 1 << 24 ) - 2;
    static constexpr int try_max = 20;
    static constexpr int reserve_factor = 3;
    static constexpr ssize_t invalid_index = -1;

    hash_map() { }

    hash_map( ssize_t size ) {
    
      if( size ) reserve( size );      
    }

    #define $hash_map_init_hash() \
          auto hash1 = ( hash0 + i ) % size_table;          \
          auto hash2 = ( ~hash0 + i ) % size_table;         \
          auto hash3 = ( ~hash0 * hash0 + i ) % size_table; \
                                                                      \
          if( hash1 == hash2 or hash1 == hash3 or hash2 == hash3 ) {  \
                                                                      \
            hash0 = hasher::get_hash( key, i + 1 );                   \
                                                                      \
            continue;                                                 \
          }                                                           \
                                                                      \
          auto& hvalue1 = _hash_table[ hash1 ];                       \
          auto& hvalue2 = _hash_table[ hash2 ];                       \
          auto& hvalue3 = _hash_table[ hash3 ];                       \


    iterator operator[]( key_type const& key ) {

      auto size_table = _hash_table.size();

      auto hash0 = hasher::get_hash( key );

      if( _keys.size() > 0 ) {

        for( auto i : range{ 0, try_max } ) {
          
          $hash_map_init_hash();

          if( hvalue1.get_refcnt() == 0 or
                hvalue2.get_refcnt() == 0 or
                  hvalue3.get_refcnt() == 0 ) continue;

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

    bool insert( key_type key, value_type value ) {

      auto index = get_new_index( key );

      if( index == invalid_index ) return false;

      _keys[ index ] = move( key );
        
      _values[ index ] = move( value );

      return true;
    }

    ssize_t get_new_index( key_type const& key, ssize_t rehash_index = invalid_index ) {

      auto size_table = _hash_table.size();

      auto hash0 = hasher::get_hash( key );
      
      if( size_table == size_max ) $throw $error_hash( "maximum hash table size" );
      
      for( auto i : range{ 0, try_max } ) {

        if( size_table == 0 ) break;

        $hash_map_init_hash();
 
        if( hvalue1.get_refcnt() and
              hvalue2.get_refcnt() and
                hvalue3.get_refcnt() ) {

          ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();
          

          if( index >= _keys.size() ) continue;

          if( equal( key, _keys[ index ] ) ) return invalid_index;
        }

        hash_node* hash_ptr = nullptr;

        if( hvalue1.get_refcnt() == 0 ) hash_ptr = &hvalue1;
        else if( hvalue2.get_refcnt() == 0 ) hash_ptr = &hvalue2;
        else if( hvalue3.get_refcnt() == 0 ) hash_ptr = &hvalue3;
        else continue;

        hvalue1.set_refcnt( hvalue1.get_refcnt() + 1 );
        hvalue2.set_refcnt( hvalue2.get_refcnt() + 1 );
        hvalue3.set_refcnt( hvalue3.get_refcnt() + 1 );

        ssize_t index = invalid_index;

        if( rehash_index != invalid_index ) {

          index = rehash_index;        

        } else if( not _erased.empty() ) {

          index = _erased.pop_back();

        } else { 

          index = _keys.size();

          _keys.emplace_back();

          _values.emplace_back();
        }
 
        hash_type index_hashed = index xor hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        hash_ptr->set_hash( index_hashed );
        
        return index;
      }

      reserve();

      rehash( rehash_index );

      auto index = get_new_index( key );

      $assert( index != invalid_index, "hash insert unexpectedly failed" );

      return index;
    }

    void rehash( ssize_t rehash_index = invalid_index ) {

      if( _keys.size() == 0 ) return;

      $assert( rehash_index == invalid_index, "no recursive rehashing, try increasing table size or try_max of the hash_map" );

      ++_rehashes;

      bool result = true;

      for( auto i : range{ 0, _keys.size() } ) {

        auto index = get_new_index( _keys[ i ], i );

        result = result and index != invalid_index;
      }

      $assert( result, "rehash failed for some reason" );
    }

    void reserve( ssize_t size = 0 ) { 
      
      _hash_table.reserve( reserve_factor * size );
      _hash_table.clear();
      _hash_table.resize( _hash_table.capacity() );

      _keys.reserve( size );

      _values.reserve( size );
    }

    bool erase( key_type const& key ) {

      if( _keys.size() == 0 ) return false;

      auto size_table = _hash_table.size();

      auto hash0 = hasher::get_hash( key );

      for( auto i : range{ 0, try_max } ) {

        $hash_map_init_hash();

        if( hvalue1.get_refcnt() == 0 or
              hvalue2.get_refcnt() == 0 or
                hvalue3.get_refcnt() == 0 ) continue;

        ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        if( index >= _keys.size() ) continue;

        if( not equal( key, _keys[ index ] ) ) continue;

        hvalue1.set_refcnt( hvalue1.get_refcnt() - 1 );
        hvalue2.set_refcnt( hvalue2.get_refcnt() - 1 );
        hvalue3.set_refcnt( hvalue3.get_refcnt() - 1 );

        if( hvalue1.get_refcnt() == 0 ) hvalue1.set_hash( 0 );
        if( hvalue2.get_refcnt() == 0 ) hvalue2.set_hash( 0 );
        if( hvalue3.get_refcnt() == 0 ) hvalue3.set_hash( 0 );

        _keys[ index ] = key_type{};
        _values[ index ] = value_type{};
        
        _erased << index;

        return true;
      }
      
      return false;
    }

    bool erase( iterator it ) {

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

    auto size() const { return _values.size() - _erased.size(); }
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
    vector< size_type > _erased;
  };


}


