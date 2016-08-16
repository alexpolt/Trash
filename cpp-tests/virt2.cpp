#include <cstdio>

struct test {
  virtual ~test() = default;
  virtual void a() const;
};
void test::a() {}
struct test1 : test { ~test1(){printf("%s\n",__func__);} };

int main() {
const test& t = test1{};
t.~test();
}

