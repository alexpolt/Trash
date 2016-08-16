#ifndef alexpolt_result
#  define alexpolt_result

#include "config.h"
#include "util.h"
#include "usertypes/cstring.h"
#include "usertypes/string.h"
#include "error.h"

namespace alexpolt { 

  /*
    result type. Somewhat like a boost::optional type.
 
    Alex Poltavsky, 2013
    alexpolt@yandex.ru
  */

  
#define test_return( result, ret ) if( ! result ) { result.log( CC(__FILE__ , _str), __LINE__ ); return ret; }
#define log_return( result, ret ) result.log( CC(__FILE__, _str), __LINE__ ); return ret;
#define log_result( result ) result.log( CC(__FILE__, _str), __LINE__ );
#define test_terminate( result ) if( ! result ) assert( !"result is not valid" );
  
  template<class T> struct result;

  template<class T> 
  result<T> make_result( T&& value ) {
    return result<T>{ util::forward<T>( value ) };
  }
  
  template<class T> 
  result<T> make_error( cstring msg ) {
    return result<T>{ error::code::failed, msg };
  }

  template<class T> 
  result<T> make_error( string&& msg ) {
    return result<T>{ error::code::failed, util::move( msg ) };
  }

  template<class T> 
  result<T> make_error( ) {
    return result<T>{ error::code::failed };
  }

  using ok = result<error::code>;
  inline ok make_ok();

  namespace {
    struct privatetype;
  }

  template<class T> struct result {
    struct tempdata { char data[ sizeof( T ) ];  };
    union {
      tempdata data;
      T value;
    };

    using only_by_value = typename util::conditional< util::is_simple<T>::value, T, privatetype& >::type;

    result ( only_by_value value_ ) : value{ value_ } { }

    template<class U>
    result( const result<U>& ) = delete;

    template<class U>
    result<T>& operator=( const result<U>& ) = delete;
  
    using only_moving = typename util::conditional< util::is_simple<T>::value, privatetype&, T >::type;

    result( only_moving&& value_ ) : value{ util::move( value_ ) } { 
      error::setSuccess(); 
    }
  
    result( error::code code, cstring msg ) : data{}  {  error::setError( code, msg );  }
    result( error::code code, string&& msg ) : data{}  {  error::setError( code, util::move( msg ) );  }
    result( error::code code ) : data{}  {  error::setErrorCode( code );  }
  
    ~result() { 
      if( error::isSuccess() ) value.~T(); 
    }
  
    result( result<T>&& r ) : data{} { 
      data = r.data; 
      r.data = tempdata{}; 
    }
  
    result<T>& operator=( result<T>&& r ) {
      data = r.data; 
      r.data = tempdata{}; 
      return *this;
    }
  
    bool operator!() { 
      return error::isFailed(); 
    }

    explicit operator bool() { 
      return error::isSuccess(); 
    }
  
    operator T() { 
      if( error::isSuccess() ) {
        return util::move( value ); 
      }
      assert( !"Result is invalid" );
      return util::move( value );
    }

    T* operator->() {
      if( error::isSuccess() ) 
         return &value;
      assert( !"Result is invalid" );
      return &value;
    }

   void log( cstring file, int line ) { 
     error::log( file, line );  
   }

 
  };

  //template<> ok::operator ok() {}

  inline ok make_ok() { return ok{ error::code::success, cnullstring }; }
  inline ok make_notok( cstring str ) { return ok{ error::code::failed, str }; }
  inline ok make_notok( string&& str ) { return ok{ error::code::failed, util::move( str ) }; }


}

#endif //alexpolt_result


