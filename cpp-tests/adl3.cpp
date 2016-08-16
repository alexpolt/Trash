struct test{};

void bar(test) {}

struct test2{
  void bar() {}
  void foo() { bar( test() ); }
};
