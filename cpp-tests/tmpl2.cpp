#include <cstdio>
#define pr printf
#define tp template
#define tn typename

tp<tn T> struct newtype;
tp<> struct newtype<void> { using type = void; };
tp<> struct newtype<int> { using type = int; };

tp<tn T, tn U = void> struct test {
  static void print() { pr("A\n"); }
};

tp<tn T> struct test<T> {
//tp<tn T> struct test<T, tn newtype<T>::type> {
  static void print() { pr("B\n"); }
};

int main() {

  test<int,void>::print();

}
