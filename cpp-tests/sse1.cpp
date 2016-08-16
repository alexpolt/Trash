#include <emmintrin.h>


int main() {
  char t;
  __attribute__((aligned(16))) float test;
  __m128 a = _mm_load_ps( &test );
  __m128 b = a + a;
}

