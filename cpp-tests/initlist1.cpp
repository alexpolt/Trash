#include <initializer_list>
#include <cstdio>
#include <typeinfo>
#include <type_traits>

#define pf(str) printf("Function %s called\n", str)

struct A {
  A(){ pf("A()"); }
  A(const A&){ pf("A(const A&)"); }
  //A(A&&) { pf("A(A&&)"); }
  A(A&&) noexcept { pf("A(A&&)"); }
  ~A(){ pf("~A()"); }
  A& operator=(const A&){ pf("A& operator=(const A&)"); return *this; }
  A& operator=(A&&){ pf("A& operator=(A&&)"); return *this; }
};

template<class u>
void print( const std::initializer_list<u>& arg ) {
  printf("arg type %s\n", typeid(u).name() );
  printf("rvalue=%d, lvalue=%d\n", std::is_rvalue_reference<u>::value, std::is_lvalue_reference<u>::value );
  A a = arg.begin()[ 0 ];
}

int main() {

print( { A{}, A{} } );


}
