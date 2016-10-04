#pragma once

#define $paste_( $0, $1 ) $0##$1
#define $paste( $0, $1 ) paste_( $0, $1 )

#define $string_( $0 ) #$0
#define $string( $0 ) $string( $0 )

#define $empty_( ... )
#define $empty( ... ) $empty_( __VA_ARGS__ )

//skip 0
$empty( __COUNTER__ )

#define $ (*this)

#define $n typename
#define $t template
#define $t1 template
#define $t2 template

#define $static_assert( $0 ) static_assert( ( $0 ), #$0 )

#define $check1( $macro, $1 ) $macro( $1 );
#define $check2( $macro, $1, $2 ) $check1( $macro, $1 ) $check1( $macro, $2 )
#define $check3( $macro, $1, $2, $3 ) $check2( $macro, $1, $2 ) $check1( $macro, $3 )

#define $checkall_( $macro, $check, ... ) $check( $macro, __VA_ARGS__ )
#define $checkall( $macro, ... ) $checkall_( $macro, $paste( $check, $numargs(__VA_ARGS__) ), __VA_ARGS__ )


