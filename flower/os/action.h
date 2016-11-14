#pragma once


namespace lib {
  
  namespace os {


    enum class action : unsigned char {

      null,
      
      minimize, maximize, close, resize, paint,

      up, down, left, right, jump, crouch, run, 
      crawl, use, drop, 

      bank_left, bank_right,

      scroll, move, cursor, cursor_off
      
      attack1, attack2, attack3, attack4, attack5,
     
    };

  }
}


