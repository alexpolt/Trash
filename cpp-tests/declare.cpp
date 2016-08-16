#include <cstdio>

void test(int) { printf("%s int\n",__func__); };
void calltest() { test( 0x1 ); };
void test(unsigned) { printf("%s unsigned\n",__func__); };


struct dummy {
  void static test(int) { printf("%s int\n",__func__); };
  void static calltest() { test( 0x1 ); };
  void static test(unsigned) { printf("%s unsigned\n",__func__); }; 
};


int main() {

  calltest();

  dummy::calltest();

}

