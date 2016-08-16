struct test1 {
int a; 
};

struct test2 {
  test1 data;
  void print() const {
    const_cast<int&>( data.a ) = 1;
  }
};
