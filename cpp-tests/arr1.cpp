#include <stdio.h>


void print( char a[100] ) {
  printf("sizeof a[100] = %d, *a= %d\n", sizeof(a), sizeof(*a) );
}

int main(void) {

print(0);

}

