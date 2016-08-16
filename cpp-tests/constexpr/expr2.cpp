#include <cstdio>

template<int... T> struct seq;
template<int N, int... T> struct seq<N, T...> : seq<N-1, N, T...> {};
template<int... T> struct seq<0, T...> {
	using value = seq<0, T...>;
};

constexpr int exp2(int i) { return i*i; }

template<class T> struct Array;

template<int... T> struct Array<seq<T...>> {
	static int data[];
};

template<int... T> int Array<seq<T...>>::data[] = { exp2( T )... };

int main() {

using Array = Array<seq<10>::value>;

for(int i: Array::data) printf( "%d\n", i );

return 0;
}
