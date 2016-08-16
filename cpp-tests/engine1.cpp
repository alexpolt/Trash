#include <cstdio>
#include <new>

struct test_i {
    virtual void print() = 0;
};
struct test_box {
    void *data1, *data2;
    operator test_i*() { return reinterpret_cast<test_i*>( this ); }
};
struct test : test_i {
    void *data;
    void print() { printf("I'm test!\n"); }
    operator test_box() { 
        printf("%s\n",__func__); 
        test_box t; 
        new(&t) test{*this}; 
    return t; }
};


int main()
{
    test test;
    test_box test_val = test;
    test_i *test_if = test_val;
    test_if->print();
   
   return 0;
}

