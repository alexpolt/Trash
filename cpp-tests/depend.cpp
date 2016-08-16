template<class T> struct test {
  static const int a = sizeof(T::a);
};

test<int> t;
