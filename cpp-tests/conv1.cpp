
struct test1 {
  int a;
  operator int() { a = 1; return 1; }
};

int b = test1();