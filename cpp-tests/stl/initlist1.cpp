#include <initializer_list>
//#include <staticassert>

struct Test {
	float a,b,c;

	Test( std::initializer_list<float> list );
	void test();
};

Test::Test( std::initializer_list<float> list )  {  auto it = list.begin(); a = *it++; }
void Test::test() { a++; }

int main() {

Test t{10.0,5,3};
t.test();

return 0;
}
