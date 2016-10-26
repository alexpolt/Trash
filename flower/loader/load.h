#pragma once


#include "lib/macros.h"
#include "lib/types.h"

#include "types.h"
#include "loader.h"


namespace lib {

  namespace loader {


    shared_ptr< vector > load( cstr location ) {

      auto loader = get_loader( location );

      return loader.load( location );
    }
    
    

  }

}


