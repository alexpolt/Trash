#include <cstdio>

extern "C" __declspec(dllexport)  void test() {
  printf("test dll\n");
}

int __stdcall DllMain( int instance, int reason, void* reserved ) {
  printf("dll start code %d\n", reason);
  return 1;
}

