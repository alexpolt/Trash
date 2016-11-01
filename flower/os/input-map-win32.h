#pragma once

#include "lib/macros.h"
#include "lib/types.h"

#include "vkey.h"

namespace lib {
  namespace os {


    inline vkey input_map( vkey key ) {

      static int vkeys[] = { VK_A,    VK_S,    VK_W,  VK_D,     VK_SPACE, VK_LMOUSE_BUTTON,  };
      static int vkmap[] = { VK_LEFT, VK_DOWN, VK_UP, VK_RIGHT, VK_JUMP,  VK_ATTACK1,  };

      for( auto i : range{ 0, $array_size( vkeys ) } )

        if( vkeys[ i ] == key ) return vkmap[ i ];

      return VK_NULL;
    }

  }
}


