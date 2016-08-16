#include <cstdio>
#include <type_traits>

struct test1 {
  test1(test1&&) noexcept = default; 
  ~test1() noexcept = default;
};
struct test2 {
  test2(test2&&) noexcept(false) = default; 
  ~test2() noexcept(false) = default;
};
struct test3 {
};


int main(){ 
  printf( "t1 = %d, t2 = %d, t3 = %d\n", std::is_nothrow_move_constructible<test1>::value );
}

