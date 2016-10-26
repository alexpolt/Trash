#pragma once

#include <cassert>

#define $assert( $0, ... ) assert( $0 and ( __VA_ARGS__ "" ) )


