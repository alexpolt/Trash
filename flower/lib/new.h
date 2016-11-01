#pragma once

#include <cstddef>

#if 0 && defined( $CONFIG_STL )

inline void* operator new( size_t, void* ptr ) noexcept { return ptr; }
inline void  operator delete( void*, void* ) noexcept { }

#else

#include <new>

#endif


