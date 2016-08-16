#include <cstdio>
struct test {
  friend void print(test) { printf("test\n");}
};

void print();

int main() {

print(test());

}
