#pragma once

#include <cstdio>
#include "macros.h"
#include "types.h"
#include "to-string.h"
#include "owner-ptr.h"
#include "object.h"

namespace lib {

  namespace log {


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


    enum class log_type { info, error, debug, memory, lock, task };

    TP<log_type T0>   
    struct log_t {

      log_t( bool is_on = false ) { get_flag() = is_on; get_logger() = new log_display{}; }

      void on() { get_flag() = true; }
      void off() { get_flag() = false; }

      void log( cstr str ) { if( get_flag() ) get_logger()->log( str ); }
      void log( endl_t endl ) { if( get_flag() ) get_logger()->log( endl ); }

      void set_logger( logger* l ) { get_logger() = move( l ); }

      auto& get_flag() const { static bool flag; return flag; }
      auto& get_logger() const { static owner_ptr< logger > l; return l; }

    };


    namespace { 

      log_t< log_type::info > info{ true };
      log_t< log_type::error > error;
      log_t< log_type::debug > debug;
      log_t< log_type::memory > memory;
      log_t< log_type::lock > lock;
      log_t< log_type::lock > task;
    }

    
    TP<log_type T0, TN T1, TN = disable_if_t< is_container_v< T1 > or is_primitive_v< T1 > >>
    inline log_t< T0 > operator,( log_t< T0 > const&, T1 ) = delete;
    
    
    TP<log_type T0, TN T1, ssize_t N0, TN = disable_if_t< is_same_v< no_const_t< T1 >, char > >>
    inline auto& operator,( log_t< T0 > &logger, T1 (&data)[ N0 ] ) { 

      for( auto& e : data ) logger, e, ", ";

      return logger; 
    }

    TP<log_type T0, TN T1, TN = enable_if_t< is_container_v< T1 > >>
    inline auto& operator,( log_t< T0 > &logger, T1 const& data ) { 

      for( auto const& e : data ) logger, e, ", ";

      return logger; 
    }

    TP<log_type T0>
    inline auto& operator,( log_t< T0 > &logger, endl_t endl ) { 

      logger.log( endl );

      return logger; 
    }

    TP<log_type T0>
    inline auto& operator,( log_t< T0 > &logger, char* data ) { 

      logger.log( data );

      return logger;
    }

    TP<log_type T0>
    inline auto& operator,( log_t< T0 > &logger, cstr data ) { 

      logger.log( data );

      return logger;
    }

    TP<log_type T0, TN T1, cstr (T1::*)() const = &T1::to_string>
    inline auto& operator,( log_t< T0 > &logger, T1 const& data ) { 

      logger.log( data.to_string() ); 

      return logger;
    }

    TP<log_type T0, TN T1, cstr (object::*)() const = &T1::to_string>
    inline auto& operator,( log_t< T0 > &logger, T1 const& data ) { 

      logger.log( data.to_string() ); 

      return logger;
    }

    TP<log_type T0, TN T1, cstr (*)( T1 const& ) = &to_string>
    inline auto& operator,( log_t< T0 > &logger, T1 const& data ) { 

      logger.log( to_string( data ) ); 

      return logger;
    }

    TP<log_type T0, TN T1, cstr (T1::*)() const = &T1::operator(), TN = void>
    inline auto& operator,( log_t< T0 > &logger, T1 const& data ) { 

      logger.log( data() ); 

      return logger;
    }

}

}



