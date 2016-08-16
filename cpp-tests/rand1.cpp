#include <cstdio>
#include <cstdlib>

int main()  {

srand(0);
int i{};
while( i < 100 ) {
  printf("%d ",rand()%100);
  i++;
}

}
