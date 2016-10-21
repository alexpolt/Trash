
#include "../lib/log.h"
#include "../lib/global.h"


int main() {

  log::info, "Hello World", log::endl;

  auto& buf = global::buffer<>;

  buf[0] = 'A';
  buf[1] = '\0';

  log::info, buf.data(), log::endl;

}

