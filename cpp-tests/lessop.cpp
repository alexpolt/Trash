struct type_int { type_int(int) {} };
struct type_unsigned { type_unsigned(unsigned) {} };

bool operator <(type_unsigned, type_int) = delete;

void test() { type_unsigned(0)<1; }
