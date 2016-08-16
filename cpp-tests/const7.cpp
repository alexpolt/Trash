#include <cstdio>

struct a {
  a() { printf("%s\n",__func__); }
  ~a() { printf("%s\n",__func__); }
};
struct b {
  const a& a_;
  b() : a_{a{}} {
    printf("%s\n",__func__);
  }
  ~b() { printf("%s\n",__func__); }
};
void test() { printf("%s\n",__func__); }
int main() {
  b b1{};
  test();
}
