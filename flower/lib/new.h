#pragma once

#include <cstddef>


inline void* operator new( size_t, void* ptr ) noexcept { return ptr; }

inline void  operator delete( void*, void* ) noexcept { }


