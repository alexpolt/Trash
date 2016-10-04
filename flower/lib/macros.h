#pragma once

#define $paste_( $0, $1 ) $0##$1
#define $paste( $0, $1 ) $paste_( $0, $1 )

#define $string_( $0 ) #$0
#define $string( $0 ) $string( $0 )

#define $empty_( ... )
#define $empty( ... ) $empty_( __VA_ARGS__ )

//skip 0
$empty( __COUNTER__ )

#define $escape( $0 ) asm volatile( "" : : "g" ((void*)$0) : "memory" )
#define $clobber( $0 ) asm volatile( "" ::: "memory" )


#define $ (*this)

#define $n typename
#define $t template
#define $t1 template
#define $t2 template

#define $static_assert( $0 ) static_assert( ( $0 ), #$0 )

#define $apply( $0, ... ) $0( __VA_ARGS__ )

#define $args_size__( $0, $1, $2, $3, N, ... ) N
#define $args_size_( ... ) $args_size__( __VA_ARGS__, 3, 2, 1, 0 )
#define $args_size( ... ) $args_size_( 0, ##__VA_ARGS__ )

#define $check1( $macro, $1 ) $macro( $1 );
#define $check2( $macro, $1, $2 ) $check1( $macro, $1 ) $check1( $macro, $2 )
#define $check3( $macro, $1, $2, $3 ) $check2( $macro, $1, $2 ) $check1( $macro, $3 )

#define $checkall( $macro, ... ) $apply( $paste( $check, $numargs(__VA_ARGS__) ), $macro, __VA_ARGS__ )


