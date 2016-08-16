#include <functional>

int main() {
int a;
auto lamb1 = [=]() mutable ->void{ a = 1; };
auto func1 = std::function<void()>( lamb1 );
}

