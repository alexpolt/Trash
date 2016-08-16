#include <cstdio>

struct Dummy {
int a;
Dummy(int a_) : a(a_) {}

void test(int){
  printf("dummy a = %d\n", a );
}
};

struct Dummy2 : Dummy {
using Dummy::test;
Dummy2(int a_) : Dummy(a_) {}
void test(int){
  printf("dummy a = %d\n", a+1 );
}
};

struct Test {
union {
    Dummy2 d;
    int t;
};

  Test() : d{1} {}

};

int main() {

Test t;
t.d.test(2);

return 0;
}
