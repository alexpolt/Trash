#pragma once

#include "macros.h"
#include "types.h"
#include "error.h"

namespace lib {


  struct url {

    enum class scheme { file, web, db, null };


    static url create( cstr path ) {

      if( strstr( path, "file://" ) == path ) {

        return { path, scheme::file, path + $length( "file://" ) - 1 };
      }

      if( strstr( path, "http://" ) == path ) {

        return { path, scheme::file, path + $length( "http://" ) - 1 };
      }

      if( strstr( path, "db://" ) == path ) {

        return { path, scheme::file, path + $length( "db://" ) - 1 };
      }


      $throw $error_input( path );

      return {};
    }


    auto get_scheme() const { return _scheme; }
    
    auto get_path() const { return _path; }

    auto data() const { return _path_orig; }

    cstr _path_orig;
    scheme _scheme;
    cstr _path;

  };

}


