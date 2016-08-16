#include <cstdio>

int main() {

float d;

scanf( " %f", &d );
printf("read value %hf\n", d);

int a = *reinterpret_cast<int*>(&d);
printf("a = %#x, exp = %d\n", a, a>>23 );
int digits = ((a >> 23) -127)* 16 / 52;
printf("digits %d\n", digits );

}
