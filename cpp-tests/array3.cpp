#include <cstdio>
#include <typeinfo>
#include <initializer_list>

int main() {

auto a1 = {1};
printf("a1 type %s", typeid(a1).name());

}
