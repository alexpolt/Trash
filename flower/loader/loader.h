#pragma once

#include <cstring>

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/assert.h"
#include "lib/log.h"
#include "lib/url.h"
#include "types.h"
#include "error.h"
#include "loader-file.h"


namespace lib {

  namespace loader {


    inline loader_ptr create( url location ) {

      if( location.scheme() == url::scheme::file ) {

        log::loader, "creating file loader for ", location.path(), log::endl;

        return loader_ptr{ new loader_file{ location } };
      }
      
      $assert( false, "no such scheme" );

      return loader_ptr{};
    }


  }
}


