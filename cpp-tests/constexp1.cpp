#define tp template
#define tn typename
#define $ (*this)

tp<tn T> struct meter {
  T value;
  constexpr meter() : value{} {}
  constexpr explicit meter(T value_arg) : value{ value_arg } {}
  //tp<tn U> explicit constexpr operator meter<U>() { return meter<U>{ static_cast<U>($.value) }; }
  //explicit constexpr operator U()=delete;
  constexpr operator T() { return $.value; }
};

meter<int> test(meter<long>){ return meter<int>{}; }

constexpr meter<int> operator"" _m( unsigned long long value_arg ) { return meter<int>{ static_cast<int>( value_arg ) }; }

meter<int> m;// = 3_m * 2_m;

//meter<int> a = test( static_cast<meter<long>>( m ) );
meter<int> a = test( m );


