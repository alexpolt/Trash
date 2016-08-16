#include <cstdio>
#include <climits>

void print(unsigned long long a) { printf("%#.16llx\n",a); }

int main() {

int s = INT_MAX;

print( s+1 );

}
