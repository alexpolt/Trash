
#include "../lib/vector.h"

using namespace lib;

TP<TN T0, ssize_t N0>
void print( vector< T0, N0 > & v0 ) {

  log::info, "size = ", v0.size(), ", capacity = ", v0.capacity(), ", empty = ", v0.empty(), log::endl;

  log::info, "vector: ";

  for( auto& e : v0 ) log::info, (int)e, ", ";

  log::info, log::endl;
}

struct test {
  test(int i) : _value{i} { }
//  test(int i) : _value{i}{ log::info, "test(",_value,")", log::endl; }
//  ~test() { log::info, "~test(",_value,")", log::endl; }
//  test(test const&o) : _value{o._value}{ log::info, "test(const&)", log::endl; }
//  test(test&&o) : _value{o._value}{ log::info, "test(&&)", log::endl; }
//  test& operator=(test const&o) { _value = o._value; log::info, "operator=()", log::endl; return $this; }
//  test& operator=(test&&o) { _value = o._value; log::info, "operator=(&&)", log::endl; return $this; }
  operator int() { return _value; }
  int _value;
};


int main() {

  log::memory.on();

  vector< test > v0{};


  v0 << 1;

  print( v0 );

  int test[]={1,2,3};

  auto v1 = vector<int>{test};

  log::info, "\nv1\n";
  v1 << test;
  v1.erase( 1 );
  print( v1 );

{
  log::info, "\nmove v1\n";
  vector<float> v2{ move( v1 ) };
  log::info, "move v1 -\n";
  print( v1 );
  print( v2 );
}
  log::info, "\nappend to v0\n";


  v0.append( test, $length( test ) );

  print( v0 );

  v0.pop_back();

  print( v0 );

  auto it0 = begin( v0 );

  it0[2] = 5;

  vector< int > v3;
  log::info, "alloc1 = ", lib::alloc::get_stats().alloc.load(), log::endl;

  v3 << v0;

  log::info, "alloc2 = ", lib::alloc::get_stats().alloc.load(), log::endl;


  print( v3 );

  log::info, "alloc = ", lib::alloc::get_stats().alloc.load(), log::endl;

}

