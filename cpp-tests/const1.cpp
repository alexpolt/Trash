#include <cstdio>
#include "const.cpp"

void print1() {
extern void print2();
print2();
printf("\nprint1\n");
test<1>().print();
}


int main() {
print1();
}


