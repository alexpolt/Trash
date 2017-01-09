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

    struct hash_node;

    static constexpr int _reserve_init = 4;
    static constexpr int _hash_functions = 3;
    static constexpr int _load_factor = 39; // 10 * hash table size / number of elements
    static constexpr ssize_t _invalid_index = -1;
    static constexpr ssize_t _size_max = is_64bit ? 1ll << 56 : 1 << 28;
    static constexpr ssize_t _hash_table_size_max = _size_max << 2;

    static allocator create_alloc() { return alloc_default::create( "hash_map" ); }

    hash_map( allocator alloc = create_alloc() ) : 
      _hash_table{ alloc->get_copy() }, 
      _keys{ alloc->get_copy() }, 
      _values{ alloc->get_copy() }, 
      _hashes{ alloc->get_copy() }, 
      _alloc{ move( alloc ) } { };

    hash_map( ssize_t size, allocator alloc = create_alloc() ) : hash_map{ move( alloc ) } {
      
      reserve( size );
    }

    void init_hash( key_type const& key, hash_type& hash0, 

                      ssize_t& hash1, ssize_t& hash2, ssize_t& hash3, bool rehashing = false ) {

      auto size_table = _hash_table.size();

      ssize_t mask = size_table - 1;

      if( not rehashing ) {

        hash0 = hasher::get_hash( key );
      }

      if( hash0 == 0 ) hash0 = 1;
     
      $assert( hash0, "hash of the key is zero?" );

      hash1 = hash0 & mask;
      hash2 = ~hash0 >> 1 & mask;
      hash3 = ( ~hash0 >> 1 ^ hash0 ) & mask;

      if( hash1 == hash2 ) { ++hash2; hash2 = hash2 & mask; }
      if( hash1 == hash3 ) { ++hash3; hash3 = hash3 & mask; }
      if( hash2 == hash3 ) { ++hash3; hash3 = hash3 & mask; }
      if( hash1 == hash3 ) { ++hash3; hash3 = hash3 & mask; }

      $assert( hash1 != hash2 and hash2 != hash3 and hash3 != hash1, "init_hash failed" );
    }
    
    bool find_bucket( key_type const& key, ssize_t* index, hash_node** bucket1 = nullptr, 
    
                        hash_node** bucket2 = nullptr, hash_node** bucket3 = nullptr ) {

      hash_type hash0 = *index ? _hashes[ *index ] : hash_type{};

      ssize_t hash1, hash2, hash3;

      init_hash( key, hash0, hash1, hash2, hash3, *index );

      auto size_table = _hash_table.size();

      auto hash_mask = size_table - 1;

      for( auto i : range{ 0, _hash_table.size() } ) {
       
        auto offset = i;

        auto& hvalue1 = _hash_table[ ( hash1 + offset ) & hash_mask ];
        auto& hvalue2 = _hash_table[ ( hash2 + offset ) & hash_mask ];
        auto& hvalue3 = _hash_table[ ( hash3 + offset ) & hash_mask ];

        if( hvalue1.get_refcnt() == 0 or
              hvalue2.get_refcnt() == 0 or
                hvalue3.get_refcnt() == 0 ) break;

        ssize_t index2 = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        if( index2 >= _keys.size() ) continue;

        if( hash0 != _hashes[ index2 ] or not equal( key, _keys[ index2 ] ) ) continue;

        *index = index2;

        if( bucket1 ) {

          *bucket1 = &hvalue1;
          *bucket2 = &hvalue2;
          *bucket3 = &hvalue3;
        }

        return true;
      }

      return false;
    }

    iterator find( key_type const& key ) { return $this[ key ]; }

    iterator operator[]( key_type const& key ) {

      if( not size() ) return _values.end();

      ssize_t index{};

      if( not find_bucket( key, &index ) ) return _values.end();

      return _values.begin() + index;
    }

    const_iterator operator[]( key_type const& key ) const {

      return ( const_cast< hash_map& > ( $this ) )[ key ];
    }

    iterator insert( key_type key, value_type value ) {

      if( size() == _size_max ) 

        $throw $error_hash( "maximum hash table size" );

      if( _hash_table.size() == 0 ) reserve( _reserve_init );

      if( size() and 10 * _hash_table.size() / size() < _load_factor ) rehash( false );

      auto index_new = _keys.size();

      auto key_hash = set_new_index( key, index_new );

      if( not key_hash ) return _values.end();

      _keys << move( key );
      _values << move( value );
      _hashes << key_hash;

      return _values.begin() + index_new;
    }

    hash_type set_new_index( key_type const& key, ssize_t index_new, bool rehashing = false ) {

      hash_type hash0 = rehashing ? _hashes[ index_new ] : hash_type{};

      ssize_t hash1, hash2, hash3;

      init_hash( key, hash0, hash1, hash2, hash3, rehashing );

      auto size_table = _hash_table.size();
     
      auto hash_mask = size_table - 1;

      for( auto i : range{ 0, _hash_table.size() } ) {

        auto offset = i;

        if( i > _search_max ) _search_max = i;

        auto& hvalue1 = _hash_table[ ( hash1 + offset ) & hash_mask ];
        auto& hvalue2 = _hash_table[ ( hash2 + offset ) & hash_mask ];
        auto& hvalue3 = _hash_table[ ( hash3 + offset ) & hash_mask ];

        auto refcnt1 = hvalue1.get_refcnt();
        auto refcnt2 = hvalue2.get_refcnt();
        auto refcnt3 = hvalue3.get_refcnt();

        if( refcnt1 and refcnt2 and refcnt3 ) {

          ssize_t index = hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

          if( rehashing and index >= index_new ) continue;

          if( index >= _keys.size() ) continue;

          if( hash0 == _hashes[ index ] and equal( key, _keys[ index ] ) ) {

            return false;
          }
        }

        hash_node* hash_ptr = nullptr;

        if( refcnt1 == 0 ) 
               hash_ptr = &hvalue1; 

        if( refcnt2 == 0 ) { 
          if( not hash_ptr ) 
               hash_ptr = &hvalue2; 
          else hvalue2.set_hash( hash0 );
        }

        if( refcnt3 == 0 ) {
          if( not hash_ptr ) 
               hash_ptr = &hvalue3;
          else hvalue3.set_hash( ~hash0 >> 1 );
        }

        if( not hash_ptr ) continue;
 
        hvalue1.set_refcnt( refcnt1 + 1 );
        hvalue2.set_refcnt( refcnt2 + 1 );
        hvalue3.set_refcnt( refcnt3 + 1 );

        hash_ptr->set_hash( hash_type{} );

        hash_type index_hashed = index_new xor hvalue1.get_hash() xor hvalue2.get_hash() xor hvalue3.get_hash();

        hash_ptr->set_hash( index_hashed );
        
        return hash0;
      }
      
      rehash( rehashing );

      auto key_hash = set_new_index( key, index_new );

      $assert( key_hash, "hash insert unexpectedly failed" );

      return key_hash;
    }

    void rehash( bool rehashing ) {

      $assert( not rehashing, "no recursive rehashing, try increasing table size or try_max of the hash_map" );

      ++_rehashes;

      $assert( size(), "hash_map size can't zero in rehashing" );

      reserve();

      for( auto i : range{ 0, _keys.size() } ) {

        auto key_hash = set_new_index( _keys[ i ], i, true );

        $assert( key_hash, "rehash failed for some reason" );
      }

    }

    void reserve( ssize_t size = 0 ) {

      _keys.reserve( size );
      _values.reserve( size );
      _hashes.reserve( size );

      if( size == 0 ) {

        size = _hash_table.size() * 2;

        if( size >= _hash_table_size_max ) 

          $throw $error_hash( "maximum hash table size" );
      }

      $assert( ( size & ( size - 1 ) ) == 0, "size is not a power of two" );

      _hash_table.clear();
      _hash_table.reserve( size, true );
      _hash_table.resize( size );
   }

    iterator erase( key_type const& key ) {

      if( size() == 0 ) return _values.end();

      ssize_t index{};

      hash_node *hvalue1, *hvalue2, *hvalue3;

      auto result = find_bucket( key, &index, &hvalue1, &hvalue2, &hvalue3 );

      if( not result ) return _values.end();

      hvalue1->set_refcnt( hvalue1->get_refcnt() - 1 );
      hvalue2->set_refcnt( hvalue2->get_refcnt() - 1 );
      hvalue3->set_refcnt( hvalue3->get_refcnt() - 1 );

      if( index + 1 == size() ) {

        _keys.pop_back();
        _values.pop_back();
        _hashes.pop_back();

        return _values.end();

      } else {
  
        ssize_t index_back = size() - 1;

        auto result = find_bucket( _keys.back(), &index_back, &hvalue1, &hvalue2, &hvalue3 );

        $assert( result, "erase: lookup of the last element misteriously failed" );

        hvalue1->set_refcnt( hvalue1->get_refcnt() - 1 );
        hvalue2->set_refcnt( hvalue2->get_refcnt() - 1 );
        hvalue3->set_refcnt( hvalue3->get_refcnt() - 1 );

        _hashes[ index ] = _hashes.back();

        auto key_hash = set_new_index( _keys.back(), index, true );

        $assert( key_hash, "erase reinsert of the last element has failed" );

        _keys[ index ] = _keys.pop_back();
        _values[ index ] = _values.pop_back();
        _hashes[ index ] = _hashes.pop_back();

        return _values.begin() + index;
      }
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

    auto size() const { return _values.size(); }
    auto size_max() const { return _size_max; }
    auto empty() const { return size() == 0; }
    auto rehashes() const { return _rehashes; }
    auto search_max() const { return _search_max; }

    void set_allocator( allocator alloc ) { _alloc = move( alloc ); }

    allocator& get_allocator() { return _alloc; }


    struct hash_node {

      static constexpr bool is_primitive = true;

      static constexpr hash_type _mask = is_64bit ? 0x00FF'FFFF'FFFF'FFFF : 0x0FFF'FFFF;
      static constexpr int _mask_shift = is_64bit ? 56 : 28;
      static constexpr int _refcnt_max = ~_mask >> _mask_shift;

      auto get_hash() const { return _hash & _mask; }
      
      void set_hash( hash_type hash ) { _hash = ( _hash & ~_mask ) | ( hash & _mask ); }

      ssize_t get_refcnt() const { return _hash >> _mask_shift; }

      void set_refcnt( int count ) { 

        if( count > _refcnt_max ) count = _refcnt_max;

        _hash = ( _hash & _mask ) | ( count << _mask_shift ); 
      }

      hash_type _hash{};
    };

    vector< hash_node > _hash_table;
    vector< key_type > _keys;
    vector< value_type > _values;
    vector< hash_type > _hashes;
    allocator _alloc;
    ssize_t _rehashes{};
    ssize_t _search_max{};
  };


}


