
struct test { int i; };

constexpr int* pointer( test& arg) { return &arg.i; }

test t;
int* a = pointer( t );

int main() {

int* a = pointer( t );

}
