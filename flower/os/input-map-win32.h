#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "vkey.h"
#include "action.h"


namespace lib {

  namespace os {


    inline action input_map( vkey key ) {

      static vkey vkeys[] { 

        vkey::a, vkey::s, vkey::w, vkey::d, vkey::space, 
        vkey::q, vkey::e,
        vkey::lbutton, vkey::rbutton, 
        vkey::scroll, vkey::control, vkey::menu
      };

      static action actions[] { 

        action::left, action::down, action::up, action::right, action::jump,
        action::bank_left, action::bank_right,
        action::attack1, action::attack2, 
        action::scroll, action::cursor, action::cursor_off
      };

      for( auto i : range{ 0, $array_size( vkeys ) } )

        if( vkeys[ i ] == key ) return actions[ i ];

      return action::null;
    }

  }
}


