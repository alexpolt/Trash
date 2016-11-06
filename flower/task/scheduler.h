#pragma once

#include "lib/macros.h"
#include "lib/types.h"
#include "lib/error.h"
#include "lib/value.h"
#include "lib/log.h"
#include "lib/sequence.h"

#include "task.h"


namespace lib {

  namespace task {


    inline void run( value< task > task0 ) {

      log::task, task0, log::endl;

      $try {

        auto result = task0();

        (void) result;

      } $catch( error const& e ) {
        
        log::error, e, log::endl;
      }
    }


    struct task_tag;

    TP<TN T0> 
    tid_t operator+( task_desc desc, T0 fn ) { 
      
      auto id = desc.id = global::gen_id< task_tag >();

      auto t = value< task >::create< task_basic< T0 > >( move( fn ), desc );

      run( move( t ) );

      return id;
    }
   
    #define $task( ... ) \
      lib::task::task_desc{ $file_line $args_first( __VA_ARGS__ ) } + \
        [ $args_second( __VA_ARGS__ ) ]() -> task::result 



  }

}


