#include "static.h"
#include <stdio.h>

void print() {
  int* a = getdata();
  printf("data = %d\n", *a);
}
  