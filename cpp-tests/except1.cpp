#include <cstdio>
#include <vector>
//#include <stdexcept>
//#include <exception>

#define pf(str) printf("Function %s called\n", str)

void unexpected() { printf("unexpected\n"); }

struct A {
  A(){ pf("A()"); }
  A(const A&){ pf("A(const A&)"); }
  //A(A&&) { pf("A(A&&)"); }
  A(A&&) noexcept { pf("A(A&&)"); }
  ~A(){ pf("~A()"); }
  A& operator=(const A&){ pf("A& operator=(const A&)"); return *this; }
  A& operator=(A&&)noexcept(false){ pf("A& operator=(A&&)"); return *this; }
};

A a{};

void test() {

//std::set_terminate( unexpected );

/*
printf("a\n");
int* a = 0;
int b = *a;
printf("\na\n");
*/

std::vector<A> v(2);
printf("!!!\n\n");
v = A{};
printf("--\nassign\n");

v.push_back(A{});
printf("--\n");
v.push_back(A{});
printf("--\n");
v.push_back(A{});
printf("--\n");
v.push_back(A{});
printf("--\n");

//extern void thatsit();
//thatsit();

for(auto& i : v) printf("el\n");

printf("\n******\n");

std::vector<A> v2;
v2 = static_cast< std::vector<A>&& >( v );
printf("--\n");

}

int main() {
try {
test();
} catch(...) { printf("caught exception\n"); }
}