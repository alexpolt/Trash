#include <cstdio>
#include <cassert>

#include "../config.h"
#include "../testing.h"
#include "../heap.h"
#include "../types/object.h"

using namespace alexpolt;

struct MyObject :Object {
    int t;
    char a;
    ~MyObject() {}
};

struct Dummy {
    Dummy() { printf("%s()\n", __func__); }
    Dummy(const Dummy&) { printf("%s copy\n", __func__); }
    Dummy(Dummy&&) { printf("%s move\n", __func__); }
    Dummy& operator=(const Dummy& r) { printf("%s copy=\n", __func__); return *this; }
    Dummy& operator=(Dummy&& r) { printf("%s move=\n", __func__); return *this; }
    ~Dummy() { printf("%s\n", __func__); }
};


struct Test {
    int t; char a;
    Test() { printf("%s()\n", __func__); }
    Test(Dummy) { printf("%s(Dummy)\n", __func__); }
    Test(Dummy,Dummy) { printf("%s(Dummy,Dummy)\n", __func__); }
    Test(int t_, char a_) : t{t_}, a{a_} { printf("%s(int, char)\n", __func__); }
    Test(const Test&) { printf("%s copy\n", __func__); }
    Test(Test&&) { printf("%s move\n", __func__); }
    Test& operator=(const Test& r) { printf("%s copy=\n", __func__); return *this; }
    Test& operator=(Test&& r) { printf("%s move=\n", __func__); return *this; }
    ~Test() { printf("%s\n", __func__); }
};


int main() {

TEST_BEGIN( "CreateObject" );
//TEST(  );

heap::print();

Test a3 = make_object<Test>( Dummy{} );
auto a4 = make_object<Test>( 10, char{1} );

heap::print();

  
TEST_END();

return 0;
}




