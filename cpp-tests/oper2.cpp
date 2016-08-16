#include <cstdio>

struct test;

void operator+(test& a, const test& b){ printf("::op+\n" ); }

struct test {
  void operator+(const test& b) { printf("test::op+\n"); }
  void print() {
    //using ::operator+;
    *this+test();
    //(*this).operator+(test());
  }
};

void func() {
   //test()+test();
 }

int main() {
  printf("start\n");
  test().print();
//  func();
  printf("finish\n");
}
