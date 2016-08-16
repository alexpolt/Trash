
char b;
//int a = reinterpret_cast<int>( b );

struct a1{};
struct a2{};
a1 a;
a2 s = reinterpret_cast<a2>( a );
