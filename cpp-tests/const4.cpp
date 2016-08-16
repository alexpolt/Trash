#define type typename
#define tmpl template

tmpl<type T> int test( const T ) { return 1; }

int a = test<const int>( 3 );
