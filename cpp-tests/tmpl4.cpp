#include <cstdio>

template<class C> struct dummy;
template<> struct dummy<int> {
  using value = int;
};
template<> struct dummy<char> {
  static const int value = 1;
};
template<class C> using test = typename dummy<C>::value;

int main() {
test<int> a = 2;
printf("%d %d\n", a, test<char>);
}
