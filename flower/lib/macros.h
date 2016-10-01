#pragma once

#define $empty_( ... )
#define $empty( ... ) $empty_( __VA_ARGS__ )

//skip 0
$empty( __COUNTER__ )

#define $ (*this)

#define $n typename
#define $t template

#define $static_assert( $0 ) static_assert( ( $0 ), #$0 )

