
constexpr int func(int i) { return i*2; }

template<int N>
struct array {

int data[ N ];

constexpr array() : data{} {}
constexpr int& operator[](int i) { return data[i]; }

};

template<int N>
constexpr array<N> init( int (*func)(int), array<N> arr = array<N>(), int i = N ) {
	return i > 0 && ( arr[i-1] = func( i ), 1) ? arr : init( func, arr, i-1 );
}

constexpr array<10> data = init<10>( &func );

