#pragma once


namespace lib {
  
  namespace os {


    enum class action : unsigned char {

      null,
      
      cancel, minimize, maximize, close, resize, paint,

      up, down, left, right, jump, crouch, run, 
      crawl, use, drop, 

      roll_left, roll_right,

      scroll, move, cursor, cursor_on,
      
      attack1, attack2, attack3, attack4, attack5,
     
    };

  }
}


