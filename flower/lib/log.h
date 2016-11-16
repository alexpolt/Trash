#pragma once

#include <cstdio>
#include "macros.h"
#include "types.h"
#include "to-string.h"
#include "new.h"



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


    enum class log_type { info, error, warn, debug, malloc, memory, lock, task, link, event, input,
                          timer };

    constexpr cstr log_type_str[] = { "info: ", "error: ", "warn: ", "debug: ", "malloc: ", "memory: ", 
                                      "lock: ", "task: ", "link: ", "event: ", "input: ",
                                      "timer: "
                                    };

    TP<log_type T>
    struct log_t {

      log_t( bool is_on = false ) { 
        
        _flag = is_on; 

        new( _logger ) log_display{};
      }

      ~log_t() { }

      void on() { _flag = true; }

      void off() { _flag = false; }

      void log( cstr str ) { 

        if( _flag and *type_cast< ssize_t*>( _logger ) ) { 

          if( _endl ) {
            
            get_logger()->log( log_type_str[ (int) T ] );

            _endl = false;
          }

          get_logger()->log( str ); 
        } 
      }

      void log( endl_t endl ) { 

        if( _flag and *type_cast< ssize_t*>( _logger ) ) get_logger()->log( endl );

        _endl = true;
      }

      logger* get_logger() { return type_cast< logger* >( &_logger ); }

      TP<TN U>
      void set_logger( U l ) { 
        
        static_assert( $size( U ) <= $array_size( _logger ), "new logger exceeded available space" );

        new( _logger ) U{ move( l ) }; 
      }

      alignas( alignof( logger ) )
      static char _logger[ $size( log_display ) ];

      static bool _flag;

      static bool _endl;
    };

    TP<log_type T> bool log_t< T >::_flag{ false };
    TP<log_type T> bool log_t< T >::_endl{ true };
    TP<log_type T> 
    alignas( alignof( logger ) ) char log_t< T >::_logger[];


    namespace { 

      log_t< log_type::info > info{ true };
      log_t< log_type::error > error{ true };
      log_t< log_type::warn > warn{ true };
      log_t< log_type::debug > debug;
      log_t< log_type::malloc > malloc;
      log_t< log_type::memory > memory;
      log_t< log_type::lock > lock;
      log_t< log_type::task > task;
      log_t< log_type::link > link;
      log_t< log_type::event > event;
      log_t< log_type::input > input;
      log_t< log_type::timer > timer;

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

    TP<log_type T0, TN T1, TN = enable_if_t< is_same_v< no_const_t< T1 >, char > or 
                                             is_same_v< no_const_t< T1 >, byte > 
                                           >>
    inline auto& operator,( log_t< T0 > &logger, T1* data ) { 

      logger.log( (cstr) data );

      return logger;
    }

    TP<log_type T0, TN T1, decltype( &T1::to_string ) = &T1::to_string>
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



