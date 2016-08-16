#include <cstdio>

#define tn typename
#define tp template
#define $this (*this)

tp<int... T> struct list{};
tp<int N> struct number{ static const int value = N; };

tp<tn... T> struct max;

tp<int H, int... T> struct max< list<H, T...> > {
  using max_t = max< number<H>, list<T...>, list<> >;
  static const int value = max_t::value;
  using rest = tn max_t::rest;
};

tp<tn M, int H, int... T, int... R> struct max< M, list<H, T...>, list<R...> > {
  static const int max_val_new = M::value > H ? M::value : H;
  static const int max_val_old  = M::value > H ? H : M::value;
  using max_t = max< number<max_val_new>, list<T...>, list<R..., max_val_old> >;
  static const int value = max_t::value;
  using rest = tn max_t::rest;
};

tp<tn M, int... R> struct max< M, list<>, list<R...> > {
  using rest = list<R...>;
  static const int value = M::value;
};


tp<tn... T> struct sort;
tp<int... T> struct sort< list<T...> > {
  using type = tn sort< list<T...>, list<> >::type;
};

tp<int... T, int... R> struct sort< list<T...>, list<R...> > {
  using max_t = max< list<T...> >;
  using type = tn sort< tn max_t::rest, list<R..., max_t::value> >::type;
};

tp<int... R> struct sort< list<>, list<R...> > {
  using type = list<R...>;
};

tp<tn...> struct array_initializer;
tp<int... N> struct array_initializer< list<N...> > {
  static int data[];
  static int size;
};
tp<int... N> int array_initializer< list<N...> >::data[] = {N...};
tp<int... N> int array_initializer< list<N...> >::size = sizeof...( N );

#define number_list 345,123,4565,-345,-3,1,0,34
using sorted_list_t = array_initializer< sort< list<number_list> >::type >;

int main() {

//for( int i : sorted_list_t::data ) printf("%d ", i);
for( int i = 0; i < sorted_list_t::size; i++ ) printf("%d ", sorted_list_t::data[ i ] );

printf("\n");

}

