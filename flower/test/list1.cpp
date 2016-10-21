
#include <map>

#include "../lib/common.h"


int main() {

  std::map< int, int > l0{ {1,1}, {2,1}, {3,1}, {4,6}, {5,6} };

  vector< int > v0;

  info, lib::is_container_v< decltype( l0 ) >, ", ", 
        lib::is_primitive_v< decltype( l0 )>, ", ", 
        lib::is_array_v< decltype( l0 ) >, endl;

  info, lib::is_container_v< decltype( v0 ) >, ", ", 
        lib::is_primitive_v< decltype( v0 )>, ", ", 
        lib::is_array_v< decltype( v0 ) >, endl;

  //std::list<int> l1;
  //l1 = lib::move( l0 );

  info, "list: ", l0, endl;

  //v0 << lib::move( l0 );

  //info, "vector: ", v0, endl;

  //info, "list: ", l0, endl;

}


