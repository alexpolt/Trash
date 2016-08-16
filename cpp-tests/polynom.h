/*
#include <cstdio>
#include <cmath>
*/
#include <complex>
using std::complex;

template<class T> class Polynom {
public:

	Polynom( size_t s ) : size(s), data( new T[ s ]() ) {}
	Polynom( int s ) : Polynom( size_t( s ) ) {}

	template<class... A>
	Polynom( A... args ) : size( sizeof...(args) ), data( new T[ sizeof...(args) ]{args...} ) {}

	Polynom() = delete;

	Polynom( const Polynom& p ) : size(p.size), data( new T[ size ]() ) {
		for( size_t i = 0; i < size; i++ )
			data[ i ] = p.data[ i ];
	}
	Polynom( Polynom&& p ) : size( p.size ), data( p.data ) {
		p.data = 0;
		p.data = nullptr;
	}

	~Polynom() { delete[] data; }
	
	T operator()( T x ) {
		T result = 0;
		for( size_t i = 0; i < size; i++ ) result += pow(x, i) * data[i];
	return result;
	}

	template<class F>
	void print( F printf ) {
		static_assert( sizeof(T) == 4, "print error" );
		for( size_t i = 0; i < size; i++ ) printf( "%.2f*x%d + ", data[ i ], i );
		printf( "\n" );
	}
	
	template<class A> 
	friend Polynom<A> operator*( Polynom<A>&, Polynom<A>& );

	unsigned size;
	T* data;
};

template<> complex<float> Polynom< complex<float> >::operator()( complex<float> x ) {
	complex<float> result = 0;
	for( size_t i = 0; i < size; i++ ) result += complex<float>{ std::pow( x, i ) } * data[i];
return result;
}



	template<class T>
	Polynom<T> operator*( Polynom<T>& a, Polynom<T>& b ) {
		Polynom<T> c{ a.size + b.size - 1 };
		for( size_t bi = 0; bi < b.size; bi++ )
		for( size_t ai = 0; ai < a.size; ai++ )
			c.data[ai+bi] = c.data[ai+bi] + a.data[ai] * b.data[bi];
		return c;
	}

constexpr long long unsigned fac(long long unsigned num) { return num > 1 ? num * fac(num-1) : 1; }

/*
int main() {

Polynom<float> sin{ .0f, 1.0f, .0f, -1.0f/fac(3), .0f, 1.0f/fac(5), .0f, -1.0f/fac(7) };
sin.print( printf );

for(float a=0; a<3.141592/2; a+=3.141592/20 ) {
	int deg = 180.0f / 3.1415f * a;
	printf("angle = %d, sin = %.03f\n", deg, sin(a) );
}

return 0;
}
*/