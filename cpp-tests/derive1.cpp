struct A {
};
struct B : private A {
  void test(B* b) {
    A* a = static_cast<A*>( b );
  }
};


void test() {
  B b;
  //A* a = static_cast<A*>(&b);
}
