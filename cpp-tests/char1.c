#include <stdio.h>

void test(char a) {
  printf("%c\n", a);
}

int main() {
char a = 5;
printf("sizeof(char) = %.2d, sizeof(a) = %.2d\n", sizeof (char), sizeof('a'));
test(a);
}
