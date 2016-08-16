
#define that (*this)


struct test {
  int a_mb;
  char c_mb;
  void aaa( int a, char b );
  void bbb( int a );
};
  void test::aaa( int a_mb, char b ) {
    that.a_mb = a_mb;
    that.c_mb = b;
  }

  void test::bbb( int a ) {
    a_mb = a;
    c_mb = '!';
  }



test t;