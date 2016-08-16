#include <xmmintrin.h>

__m128 a = {1,1,1,1};

int main() {
volatile __m128 a1;

a1 = _mm_add_ps( a, a );


return 0;
}
