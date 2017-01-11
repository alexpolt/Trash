#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <cstring>


#include "lib/common.h"
#include "os/file.h"
#include "lib/alloc-chunk.h"

void measure1( vector< string >& lines );
void measure2( std::vector< std::string >& lines );


int main() {

  //lib::log::lock.on();
  //lib::log::memory.on();

  hash_map< string, int > map0{};

  cstr filename = "passwords.txt";
  //filename = "passwords2.txt";
  //filename = "hitch5.txt";

  lib::os::file f0{ filename };

  f0.open();

  vector< string > lines;
  std::vector< std::string > stdlines;

  int c = 0;

  string s{ 32, lib::alloc_chunk::create( "hitch", 1 << 22 ) };

  info, "_load_factor = ", map0._load_factor, ", multiplier = ", map0._multiplier, endl;

  while( true ) {

    if( c == 544650 ) {

      //lib::log::lock.on();
      //lib::log::memory.on();
    }

    s.clear();

    f0.get_line( s );

    auto l = s;

    if( not l.size() )  break;

    l.pop_back();
    l.back() = '\0';
 
    stdlines.push_back( std::string( l.data() ) );
    lines << move( l );

    //info, c, ": ", l.data(), endl;


    ++c;

    //if( c > 1'000'000 ) break;
  }

  info, "count = ", c, "\n", endl;
  
  //info, "lock count = ", lib::global::lock_map<>._lock_map.size(), "\n", endl;

  //lib::log::lock.off();
  //lib::log::memory.off();

  measure1( lines );
  measure2( stdlines );

  //getchar();
  
}


void measure1( vector< string >& lines ) {

  hash_map< string, int > map0{};

  auto begin = std::chrono::high_resolution_clock::now();

  for( auto& l : lines ) {

    auto it = map0[ l ];

    if( it ) {

      (*it)++;

    } else {

      map0.insert( move(l), 1 );
    }

  }

  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  info, "hash table size = ", map0._hash_table.size(), ", keys = ", map0.keys().size(), endl;
  info, "rehash = ", map0.rehashes(), ", search = ", map0.search_max(), endl, endl;

  printf( "dt = %ld\n\n", (long)dt );


}

void measure2( std::vector< std::string >& lines ) {

  using map = std::unordered_map< std::string, int >;
  std::unordered_map< std::string, int > map0{};

  auto begin = std::chrono::high_resolution_clock::now();

  for( auto& l : lines ) {

    if( ! map0.count( l ) ) {

      map0.insert( { std::move(l), 1 } );

    } else {

      map0[ l ]++;
    }

  }

  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  info, "size = ", map0.size(), ", bucket count = ", map0.bucket_count(), ", load factor = ", map0.load_factor(), endl;
  
  uint empty = 0, max = 0;

  for( auto i : range{ 0, (int)map0.bucket_count() } ) { 
    if( map0.bucket_size( i ) > max ) max = map0.bucket_size(i);
    if( map0.bucket_size( i ) == 0 ) empty++;
  }

  info, "empty buckets ", empty, ", max = ", max, ", value_type size = ", $size( map::value_type ),  endl,endl;

  printf( "dt = %ld\n\n", (long)dt );

}


