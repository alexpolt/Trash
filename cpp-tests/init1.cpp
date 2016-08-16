
#include <cstdio>

struct A {
  A() { printf("%s\n",__func__); }
};
struct B {
  B() { printf("%s\n",__func__); }
};
struct test {
  A a;
  B b;
  test() : b(), a() {}
};

int main() {
test();

}
