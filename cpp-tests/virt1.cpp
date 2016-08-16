struct a {
virtual void  test1() = 0;
};
struct b1 : virtual a {
  void test1() {}
};
struct b2 : virtual a {
  void test1() {}
};

struct c : b1,b2 {

};

void test() {
c c{};
c.b1::test1();
}

