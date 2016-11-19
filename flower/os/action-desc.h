#pragma once

#include "action.h"


namespace lib {
  
  namespace os {


    inline cstr get_action_desc( action a ) {

      static cstr actions[] {

        "null",
        
        "cancel", "minimize", "maximize", "close", "resize", "paint", "activate",
        "on", "off", "lowpower",

        "up", "down", "left", "right", "jump", "crouch", "run", 
        "crawl", "use", "drop",

        "roll-left", "roll-right",
        
        "scroll", "move", "cursor", "cursor-on",
        
        "attack1", "attack2", "attack3", "attack4", "attack5",
      };

      return actions[ (int) a ];
    }

  }
}


