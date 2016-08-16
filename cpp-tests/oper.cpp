#include <cstdio>


struct test { 
test(int){ printf("test(int)\n");}
void operator[](size_t){ printf("operator[]\n");}
};

int main() {
int a[] = {3};
  a[0];

}

