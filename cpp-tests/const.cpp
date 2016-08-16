template<int N> struct test : test <N-1> {
  constexpr static float pi = 1.0f * N;
  constexpr static char const* hello = "hello 1";
  static const int n = N;
  using base = test<N-1>;
  void print() { printf("%d, %f, %s\n", n, pi, hello ); base::print(); }
};

template<> struct test<0> {
  constexpr static float pi = .0f;
  constexpr static char const* hello = "hello 2";
  static const int n = 0;
  void print() { printf("%d, %f, %s\n", n, pi, hello ); }
};

