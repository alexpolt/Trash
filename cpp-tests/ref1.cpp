
struct test { int a,b; };

test test1() { test t; t.a = 1; return t; }
test& test2() { test t; t.a = 1; return t; }

int main() {

  test1();
  test2();


}
