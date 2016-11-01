#pragma once


#if !defined( $CONFIG_STL )

#include <cstddef>

inline void* operator new( size_t, void* ptr ) noexcept { return ptr; }
inline void  operator delete( void*, void* ) noexcept { }

#else

#include <new>

#endif


