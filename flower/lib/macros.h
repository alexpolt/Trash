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


#define $this (*this)

#define $size( $0 ) int( sizeof( $0 ) )

#define $N typename
#define $T template
#define $T1 template
#define $T2 template

#define $static_assert( $0 ) static_assert( ( $0 ), #$0 )

#define $apply( $0, ... ) $0( __VA_ARGS__ )

#define $args_size__( $0, $1, $2, $3, N, ... ) N
#define $args_size_( ... ) $args_size__( __VA_ARGS__, 3, 2, 1, 0 )
#define $args_size( ... ) $args_size_( 0, ##__VA_ARGS__ )

#define $args_first( $0, ... ) $0

#define $apply1( $macro, $1 ) $macro( $1 );
#define $apply2( $macro, $1, $2 ) $apply1( $macro, $1 ) $apply1( $macro, $2 )
#define $apply3( $macro, $1, $2, $3 ) $apply2( $macro, $1, $2 ) $apply1( $macro, $3 )

#define $applyall( $macro, ... ) $apply( $paste( $check, $numargs(__VA_ARGS__) ), $macro, __VA_ARGS__ )


