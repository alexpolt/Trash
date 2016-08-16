#include <cstdio>
#include <cstddef>

//__declspec(align(32))
struct a1 {
int a,b,c;
char d;
};

#pragma pack(1)
struct __declspec(align(8)) b1 {
char a;
int b;
};

int main() {
  printf("sizeof(b1) = %d\n", sizeof(b1));
  printf("offsetof(b1.b) = %d\n", offsetof(b1,b));
}
