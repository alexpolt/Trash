#include <cstdio>

template<class... T> struct test {
  virtual void print( T... args ) {
    printf("test1\n");
  }
};

template<class... T> struct test2 : test<T...> {
  virtual void print( T... args ) {
    printf("test2\n");
  }
};


int main() {
  test2<int,char,int> t;
  t.print(0,0,0);
}

