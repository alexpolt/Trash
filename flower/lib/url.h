#pragma once

#include "macros.h"
#include "types.h"
#include "error.h"
#include "algo.h"
#include "hash.h"


namespace lib {


  struct url {

    enum class scheme : int { null, file, web, db };

    static int scheme_size( scheme s ) { 
      static int offset[] { 0,  $array_size( "file://" ) - 1, 
                                $array_size( "web://" ) - 1, 
                                $array_size( "db://" ) - 1 };
      return offset[ (int)s ];
    }

    url() { }

    url( scheme s, cstr p ) : _scheme{ s }, _path{ p } { }

    url( cstr path ) {

      _path = path;

      if( strstr( path, "file://" ) == path ) _scheme = scheme::file;
      else if( strstr( path, "web://" ) == path ) _scheme = scheme::web;
      else if( strstr( path, "db://" ) == path ) _scheme = scheme::db;
      else $throw $error_input( path );
    }

    auto scheme() const { return _scheme; }
    auto location() const { return _path + scheme_size( scheme() ); }
    auto path() const { return _path; }
    uint to_hash( int seed ) const { return hash32<>::get_hash( path() ); }

    bool operator==( url const& right ) const { return equal( path(), right.path() ); }
    bool operator<( url const& right ) const { return less( path(), right.path() ); }

    enum scheme _scheme{};
    cstr _path{};
  };

}


