
struct B;
struct A {
  friend void test(A*) {}
};
struct B : A {
};

void test() {
  B b;
  test ( &b );
}