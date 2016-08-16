#include <vector>
#include <deque>
#include <stdio.h>

template< typename T>
class tmpl 
{
public:
	T x;
	tmpl() : x() {  }
	~tmpl() { }
	void test() { printf( "Hello world!\n"); }
};
class b { public: b(void){ printf("constructor b\n"); } b(int a){ printf("constructor b\n"); }  b(const b& b){ printf("b copy constructor\n"); } };

template<typename T>
void test() { T a = 5; }

int main() {

tmpl<b> t1;
t1.test();

test<b>();

return 0;
}
