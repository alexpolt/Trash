#pragma once

#include "assert.h"
#include <new>

void* operator new( unsigned ) { $assert( false, "new operator is banned" ); }

//void* operator new( unsigned, void* ptr ) noexcept { return ptr; }


