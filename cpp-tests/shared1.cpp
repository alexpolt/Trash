#include <memory>


int main() {

auto ptr = std::shared_ptr<int>{};
*ptr = 5;
return 0;
}
