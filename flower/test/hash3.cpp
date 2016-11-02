#include <chrono>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdio>
#include <cstring>


#include "lib/common.h"
#include "os/common.h"
#include "lib/alloc-chunk.h"

void measure1( vector< string >& lines );
void measure2( std::vector< std::string >& lines );


int main() {

  //lib::log::lock.on();
  //lib::log::memory.on();

  cstr filename = "hitch4.txt";

  os::file f0{ filename };

  f0.open();

  vector< string > lines;
  std::vector< std::string > stdlines;

  int c = 0;

  string s{ lib::alloc_chunk::create( 4096 ) };

  while( true ) {

    if( c == 148500 ) {

      //lib::log::lock.on();
      //lib::log::memory.on();
    }

    auto l = f0.get_line( s );

    if( not l.size() )  break;

    l.pop_back();
    l.back() = '\0';
 
    stdlines.push_back( std::string( l.data() ) );

    //info, c, ": ", l.data(), endl;

    lines << move( l );

    ++c;

    //if( c > 20 ) break;
  }

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

  auto& v = map0.values();
  auto& k = map0.keys();

  int n = 0;

  for( auto i : range{ 0, v.size() } ) {

    if( v[ i ] > 1000 ) info, ++n, ": ", k[ i ].data(), " - ", v[ i ], endl;
  }

  info, endl;

  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  info, "hash table size = ", map0._hash_table.size(), ", keys = ", k.size(), endl;
  info, "rehash = ", map0.rehashes(), endl, endl;

  printf( "dt = %ld\n\n", (long)dt );


}

void measure2( std::vector< std::string >& lines ) {

  std::unordered_map< std::string, int > map0{};

  auto begin = std::chrono::high_resolution_clock::now();

  for( auto& l : lines ) {

    if( ! map0.count( l ) ) {

      map0.insert( { std::move(l), 1 } );

    } else {

      map0[ l ]++;
    }

  }

  int n = 0;

  for( auto& v : map0 ) {

    if( v.second > 1000 ) info, ++n, ": ", v.first.data(), " - ", v.second, endl;
  }

  info, endl;

  auto end = std::chrono::high_resolution_clock::now();

  auto dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  info, "bucket count = ", map0.bucket_count(), ", load factor = ", map0.load_factor(), endl;
  
  uint empty = 0, max = 0;

  for( auto i : range{ 0, (int)map0.bucket_count() } ) { 
    if( map0.bucket_size( i ) > max ) max = map0.bucket_size(i);
    if( map0.bucket_size( i ) == 0 ) empty++;
  }

  info, "empty buckets ", empty, ", max = ", max, endl,endl;

  printf( "dt = %ld\n\n", (long)dt );

}


