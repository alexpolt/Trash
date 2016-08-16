namespace A {
struct INT {};
void test( INT ) {}
}

struct B {
  friend void test( B ) {}
  void test2() {
    A::INT a;
    test ( a );
  }
};

int main(){

int a;
B b;
test( b );

return 0;
}
