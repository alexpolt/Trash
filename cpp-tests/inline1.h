struct test {
  static inline void print();
};

void test::print() {
  static int a = 1;
}
