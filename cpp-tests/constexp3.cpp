//constexpr void max( int* a, int* b ) { (void)( *a > *b ); }

int a = 1;
int b = 2;

constexpr int* c = &a;

constexpr int d = *c;

