struct X {}; 

void foo( X ) {}

namespace bar{
  void foo() {}
}

void test() {
  void foo();
  //using bar::foo;
  
  foo( X() );
  
}
