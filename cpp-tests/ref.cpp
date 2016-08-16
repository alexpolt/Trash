#include <cstdio>
  
#define that (*this)

template<class T>
struct test {
  T& t;
  int a;
  test( T& t_ ) : t{t_} { t(); that.a = 3; }
};
void prnt(){
printf("test\n");
}


int main() {

test<void()> t{prnt};

printf(" a = %+05d\n", t.a );
}