template<class T> struct test {
static T t;
static void n( int a ) { t = a;  }
static void n( unsigned a ) { t[0] = a;  }
};

void (*test_g)( unsigned ) = & test< int[1] >::n;

