#include <cstdio>

int main() {

float a1 = 1.0f;
float b1 = 2.0f;

printf("a = %a, b= %a\n", a1, b1);

int a2 = *( reinterpret_cast<int*>( &a1 ) );
int b2 = *( reinterpret_cast<int*>( &b1 ) );

printf("a = %x, b= %x, diff = %x\n",a2, b2, b2-a2);


return 0;
}
