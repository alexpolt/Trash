template<class t, class u = int> struct test {
//  static_assert( sizeof (t) == 1, "not char" );
};

template<class u> struct test<int, u> {
  static_assert( sizeof (u) == 1, "not char" );
};

template struct test<int>;
