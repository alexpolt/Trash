#pragma once


namespace lib {
  
  namespace os {


    enum class action : unsigned char {

      null,
      
      minimize, maximize, close, resize, paint,

      up, down, left, right, jump, crouch, run, 
      crawl, use, drop, 
      
      scroll, move,
      
      attack1, attack2, attack3, attack4, attack5,
     
    };

  }
}


