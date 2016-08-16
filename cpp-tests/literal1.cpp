#include <cstdio>


void operator"" _t(const char* str, unsigned size) {
printf("%s\n", str );
}

constexpr int fac(int n) { return n * ( n == 1 ? 1 : fac(n-1) ); }

int main() {

  printf("fac(5) = %d\n", fac(5));
  "test"_t;
}
