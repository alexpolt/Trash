#pragma once

#include "action.h"


namespace lib {
  
  namespace os {


    inline cstr get_action_desc( action a ) {

      static cstr actions[] {

        "null",
        
        "minimize", "maximize", "close", "resize", "paint",

        "up", "down", "left", "right", "jump", "crouch", "run", 
        "crawl", "use", "drop", 
        
        "scroll", "move",
        
        "attack1", "attack2", "attack3", "attack4", "attack5",
      };

      return actions[ (int) a ];
    }

  }
}


