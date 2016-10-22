
#include "macros.h"
#include "types.h"
#include "value.h"


namespace libr {

  using namespace lib;

  TP<TN T0>
  struct result;

  TP<TN T0, TN... TT>
  auto make_result( TT&&... args ) { return result< T0 >{ forward< T0 >( args )... }; }


  enum error {

    failed, success
  };


  TP<TN T0>
  struct result {
    
    using value_type = T0;

    result() : _error{ failed } { }

    result( error e ) : _error{ e  } { }

    result( value_type&& value ) {
        
      _object = owner< T0 >::template create< T0 >( move( value ) );

      _error = success;
    }

    TP<TN... TT>
    explicit result( TT... args ) {
        
      _object = owner< T0 >::template create< T0 >( forward< TT >( args )... );

      _error = success;
    }

    auto& operator*() { return *_object; }
    auto& operator*() const { return *_object; }
    auto  operator->() { return *_object; }
    auto  operator->() const { return *_object; }

    explicit operator bool() { return _error == success; }

    owner< T0 > _object;
    error _error;
  };


}



