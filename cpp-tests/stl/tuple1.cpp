#include <tuple>
#include <typeinfo>
#include <vector>

template<int... T> struct N2PackType {};
template<int... T> struct N2Pack;
template<int N, int... T> struct N2Pack<N, T...> : N2Pack<N-1, N, T...> {};
template<int... T> struct N2Pack<0, T...> { using type = N2PackType<0,T...>; };

template<class T> struct store;
template<int... N> struct store< N2PackType<N...> > {
	template<class V, class... T>
	static void storeData( std::tuple<T...>& tuple, std::vector<V>& data  ) {
		char dummy[] = { (  data.insert(  end(data),  (V*)  &std::get<N>( tuple ),  (V*)&std::get<N>( tuple )  +  sizeof( std::get<N>( tuple ) )  ),  char(0)  )... };
		//char dummy[] = { ( std::get<N>( tuple ), char(0)  )... };
	}
};


template<class V, class... T>
std::tuple<T...>&  operator>>(  std::tuple<T...>& tuple, std::vector<V>& data  ) {
	static_assert(  sizeof...(T) > 0,  "sizeof... tuple = 0"  );
	
	store< typename N2Pack< sizeof...(T) - 1 >::type >::storeData( tuple, data );
	
 	//auto elem = std::get<0>( tuple );
	//data.insert(  end(data),  (char*)  &elem,  (char*)  &elem  +  sizeof(elem)  );
return tuple; }


int main() {

std::vector<char> data;
auto vec = std::make_tuple<int, int, float>( 15, 11, 1.0 );
vec >> data;

for( int i : data)
    printf("%d\n", i );

return 0;
}
