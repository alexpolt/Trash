struct test {
int a;
int b;
};

constexpr int* addr( test& t ) { return &t.b; }

test t;
constexpr int* a = addr( t );

void print() { *a = 0; }
