#pragma once

#include <cstdio>
#include "macros.h"
#include "types.h"
#include "ptr.h"
#include "value.h"
#include "to-string.h"

namespace lib {

  namespace log {

    using namespace lib;

    constexpr struct endl_t { constexpr endl_t() { } } endl;


    struct logger {

      virtual ~logger() { }
      virtual void log( cstr ) = 0;
      virtual void log( endl_t ) = 0;
    };


    struct log_display : logger {

      void log( cstr str ) override { printf( "%s", str ); }
      void log( endl_t endl ) override { printf( "\n" ); }
    };


    enum class log_type { info, error, debug, memory }; 

    TP<log_type T0>   
    struct log_t {

      log_t( bool is_on = false ) { get_flag() = is_on; get_logger() = new log_display{}; }

      void on() { get_flag() = true; }
      void off() { get_flag() = false; }

      void log( cstr str ) { if( get_flag() ) get_logger()->log( str ); }
      void log( endl_t endl ) { if( get_flag() ) get_logger()->log( endl ); }

      void set_logger( owner_ptr< logger > logger ) { get_logger() = move( logger ); }

      auto& get_flag() const { static bool flag; return flag; }
      auto& get_logger() const { static owner_ptr< logger > logger; return logger; }

    };


    namespace { 

      log_t< log_type::info > info{ true };
      log_t< log_type::error > error;
      log_t< log_type::debug > debug;
      log_t< log_type::memory > memory;
    }


    TP<log_type T0, TN... TT> log_t< T0 > operator,( log_t< T0 >, TT... ) = delete;
    
    TP<log_type T0, TN T1, TN = enable_if_t< is_container_v< T1 > >>
    auto operator,( log_t< T0 > logger, T1 const& data ) { 

      for( auto& e : data ) logger, e, ", ";

      return logger; 
    }

    TP<log_type T0>
    auto operator,( log_t< T0 > logger, endl_t endl ) { 

      logger.log( endl );

      return logger; 
    }

    TP<log_type T0>
    auto operator,( log_t< T0 > logger, cstr data ) { 

      logger.log( data );

      return logger;
    }

    TP<log_type T0, TN T1, cstr (T1::*)() const = T1::to_string>
    auto operator,( log_t< T0 > logger, T1 const& data ) { 

      logger.log( data.to_string() ); 

      return logger;
    }

    TP<log_type T0, TN T1, cstr (*)( T1 const& ) = to_string>
    auto operator,( log_t< T0 > logger, T1 const& data ) { 

      logger.log( to_string( data ) ); 

      return logger;
    }

    TP<log_type T0, TN T1, cstr (T1::*)() const = T1::operator(), TN = void>
    auto operator,( log_t< T0 > logger, T1 const& data ) { 

      logger.log( data() ); 

      return logger;
    }


  }

}


