#include <vector>
#include <type_traits>
//#include <ini

template<typename T>
void init( std::initializer_list<T> ) {
  static_assert( std::is_same<T, std::vector<char>::iterator>::value, "is same" );
}

int main() {

std::vector<int> v1{1,2,3,4,5};

std::vector<float> v2{begin(v1), end(v1)};

init( {begin(v1), end(v1)} );
}
