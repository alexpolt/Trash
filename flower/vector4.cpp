
#include <chrono>
#include <vector>

#include "lib/common.h"

TP<TN T0>
void measure1( T0& v0 );

TP<TN T0>
void measure2( T0& v0 );



int main() {

  int a0[1];
  int const a1[2]{};

  info, "is_array: ", lib::is_array_v<decltype(a0)>, lib::is_array_v<decltype(a1)>, endl;

  std::vector< int > v0{ 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5};
  lib::vector< int > v1{ 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5};


  info, "v1 source: ", v1, endl, endl;

  info, "--start measure--", endl;  measure1( v0 );  info, "--end measure--", endl;
  info, "--start measure--", endl;  measure2( v1 );  info, "--end measure--", endl;

}

TP<TN T0>
void measure1( T0& v0 ) {

  $escape( &v0 );

  auto begin = std::chrono::high_resolution_clock::now();

  std::vector< int > v1;

  for( auto i : range{ 0, 500'000 } ) {

      (void)i;

      std::vector< int > v2;
      v2 = v0;

//      v1.insert( v1.end(), v2.begin(), v2.end() );

      $clobber();
  }

  info, "v0.size = ", v1.size(), ", v0.capacity = ", v1.capacity(), ", bytes = ", (v1.capacity()) * $size( v1 ), endl;

  auto end = std::chrono::high_resolution_clock::now();

  uint dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  info, "dt = ", dt, endl;

}

TP<TN T0>
void measure2( T0& v0 ) {

  $escape( &v0 );

  auto begin = std::chrono::high_resolution_clock::now();
          
  lib::vector< int > v1;

//  log::memory.on();

  for( auto i : range{ 0, 500'000 } ) {

      (void)i;

      lib::vector< int  > v2;

      v2 = v0 ;

//      v1 << move( v2 );

      $clobber();
  }

  log::memory.off();

  info, "v0.size = ", v1.size(), ", v0.capacity = ", v1.capacity(), ", bytes = ", ( v1.capacity() ) * $size(v1), endl;

  auto end = std::chrono::high_resolution_clock::now();

  uint dt = std::chrono::duration_cast< std::chrono::milliseconds >( end - begin ).count();

  info, "dt = ", dt, endl;

}


