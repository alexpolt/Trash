#include <cstdio>
#include <cassert>
#include "../config.h"
#include "../testing.h"
#include "../heap.h"
#include "../result.h"

using namespace alexpolt;

struct Dummy {
    int a = 0;
    Dummy() { printf("%s()\n", __func__); }
    Dummy(const Dummy&r) { printf("%s copy\n", __func__); }
    Dummy(Dummy&&r) { r.a++; printf("%s move\n", __func__); }
    Dummy& operator=(const Dummy& r) { printf("%s copy=\n", __func__); return *this; }
    Dummy& operator=(Dummy&& r) { printf("%s move=\n", __func__); return *this; }
    ~Dummy() { printf("%s %d\n", __func__, a++); }

};

template<class T> result<T> test1() {
  T d; return util::move( d );
}

template<class T> result<T> test2() {
  T d; return result<T>{ util::move( d ) };
}

template<class T> result<T> test3() {
  return result<T>{ T{} };
}

result<Dummy> test4() {
  auto r2 = make_error<Dummy>( string( "Error!" ) );
  test_return( r2 );
}

int main() {

TEST_BEGIN( "util" );
//TEST(  );

result<Dummy> r1 = test1<Dummy>();
Dummy d = r1;
printf("--1\n");

result<Dummy> r0 = test3<Dummy>();


printf("--2\n");

ok ok = make_ok();

printf("--3\n");

result<Dummy> r2 = test4();
r2.log();


TEST_END();

return 0;
}


