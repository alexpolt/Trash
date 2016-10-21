
#include "lib/common.h"

#include "math/common.h"

int main() {

  lib::log::memory.on();

  libm::vec3f v0{ 1, 2, 3 };
  libm::vec3f v1{ 1, 1, 1 };

  info, "v0 = ", v0, endl;
  info, "v1 = ", v1, endl, endl;

  libm::mat3f m0{ 3, 3, 3 };
  libm::mat3f m1{ v0, v0, v1 };

  info, "m0 = ", endl, m0, endl, endl;
  info, "m1 = ", endl, m1, endl, endl;


  {
    auto m2 = m1 + m0;

    info, m2, endl, endl;
  }


  {
    auto m2 = m1 * v0;

    info, "m1 * v0 = ", m2, endl, endl;
  }


  {
    auto m2 = v0 * m1;

    info, "v0 * m1 = ", m2, endl, endl;
  }

}

