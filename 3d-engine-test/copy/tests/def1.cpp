#include <cstdio>

void test(int = 1);

void test(int a = 2) { printf("%d\n",a); }

int main() {

test();


return 0;
}
