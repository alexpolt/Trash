
template<class T> void test() {}

template<class T> struct Test { void test(); };
template<class T> void Test<T>::test() {}

template<class T> struct A{};
