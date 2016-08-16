#include <cstdio>

struct A{ int a{5}; void print() const { printf("a = %d\n", a); } };
struct test {
A &a;
  test() : a( *  reinterpret_cast<A*>( nullptr ) ) {}
  void print() { a.print(); }
};

//void print() { int a[] = {-1,-1,-1,-1,-1,-1,-1,-1}; }

int main() {
test t;
//print();
//t.print();
}

