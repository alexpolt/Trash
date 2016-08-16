#include <vector>

struct A {};

struct B : A;

struct Test {
	//Test() = default;
	Test(const Test&){}
};
struct B:A {};

std::vector<Test> t{5};