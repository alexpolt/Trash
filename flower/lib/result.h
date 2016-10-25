
#include "macros.h"
#include "types.h"
#include "value.h"


namespace lib {


  enum class result {

    failed, success
  };


  TP<TN T0>
  struct optional;

  TP<TN T0, TN... TT>
  auto make_optional( TT&&... args ) { return optional< T0 >{ forward< T0 >( args )... }; }


  TP<TN T0>
  struct optional {
    
    using value_type = T0;

    optional() : _error{ result::failed } { }

    optional( result e ) : _error{ e  } { }

    optional( value_type&& value ) {
        
//      _object = value< T0 >::template create< T0 >( move( value ) );

      _error = result::success;
    }

    TP<TN... TT>
    explicit optional( TT... args ) {
        
//      _object = value< T0 >::template create< T0 >( forward< TT >( args )... );

      _error = result::success;
    }

    auto& operator*() { return *_object; }
    auto& operator*() const { return *_object; }
    auto  operator->() { return *_object; }
    auto  operator->() const { return *_object; }

    explicit operator bool() { return _error == result::success; }

    value< T0 > _object;
    result _error;
  };


}



