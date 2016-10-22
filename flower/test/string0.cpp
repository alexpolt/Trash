
#include "../lib/common.h"

void string3( string );

int main () {

  printf("%p\n",info.get_logger().get());

  string s0{ "test" };

  string3( s0 );

  auto s2 = s0;

  info, s2.data(), endl;
  

  info, s2, endl;

  string s3{ move( s2 ) };
  s3 = move( s0 );

  liblog::memory.on();

  char h[]={'1','3','\0','5','6'};

  s3 = h;

  info, "4: ", s3.data(), endl;
  info, "4: ", s3, endl;

  info, "----", endl;

  info, "5: ", s0.data(), endl;
  info, "5: ", s0, endl;

  s0 = "heelo";

  info, "5: ", s0.data(), endl;
  info, "5: ", s0, endl;

  string s1{ "str2: " };

  for( auto i : range{ 0, 10 } ) s1 << s0;


  info, s1.data(), endl;


}

