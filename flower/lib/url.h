#pragma once

#include "macros.h"
#include "types.h"
#include "error.h"


namespace lib {


  struct url {

    enum class scheme : int { null, file, web, db };

    static int scheme_size( scheme s ) const { 
      static int offset[] { 0,  $array_size( "file://" ) - 1, 
                                $array_size( "web://" ) - 1, 
                                $array_size( "db://" ) - 1 };
      return offset[ (int)s ];
    }

    url() { }

    url( scheme s, cstr p ) : _scheme{ s }, _path{ p } { }

    static url create( cstr path ) {

      if( strstr( path, "file://" ) == path ) return url{ scheme::file, path };

      if( strstr( path, "web://" ) == path ) return url{ scheme::web, path };

      if( strstr( path, "db://" ) == path ) return url{ scheme::db, path };

      $throw $error_input( path );

      return url{};
    }


    auto scheme() const { return _scheme; }
    auto location() const { return _path + scheme_size( scheme() ); }
    auto path() const { return _path; }
    uint to_hash( int seed ) const { return hash<>::get_hash

    enum scheme _scheme{};
    cstr _path{};
  };

}


