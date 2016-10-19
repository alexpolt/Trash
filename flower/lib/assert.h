#pragma once

#include <cassert>
#include "macros.h"

#define $assert( $0, ... ) assert( $0 and ( __VA_ARGS__ "" ) )


