#include <functional>
#include <algorithm>
#include <vector>
#include <set>
//#include <cstdio>
#include <random>
#include <iostream>
#include <chrono>
#include <iterator>

int main() {
auto seed = std::chrono::system_clock::now().time_since_epoch().count();
auto dice = std::bind( std::uniform_int_distribution<int>{1,100}, std::default_random_engine{  seed } );

std::vector<int> list(20);

generate( begin(list), end(list), dice );

copy( begin(list), end(list), std::ostream_iterator<int>( std::cout, " " ) );

std::cout << std::endl;

std::set<int> uniquelist { begin(list), end(list) };

copy( begin(uniquelist), end(uniquelist), std::ostream_iterator<int>( std::cout, " " ) );


return 0;
}

