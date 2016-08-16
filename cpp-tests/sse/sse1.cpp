#include <cstdio>
#include <xmmintrin.h>

__m128 a[1] = {{1.0,1.0,1.0,1.0}};
__m128 b[1];
void test(__m128 *c) {
*b = _mm_add_ps(a[0],c[0]);
//printf( "%f,%f,%f,%f\n", b[0], b[0].m128_f32[1], b[0].m128_f32[2], b[0].m128_f32[3] );
}

int main() {
__m128 c[1] = {{2.0,2.0,2.0,2.0}};
test(c);
test(c);

return 0;
}

