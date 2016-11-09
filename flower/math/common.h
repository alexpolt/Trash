#pragma once


#include "types.h"
#include "constant.h"
#include "math.h"
#include "matrix-funcs.h"
#include "to-string.h"


using lib::math::operator ""_rad;

#define $define_using_vec( $0 ) using $paste( lib::math::vec, $0 );

$applyall( $define_using_vec, 2f, 3f, 4f )
$applyall( $define_using_vec, 2d, 3d, 4d )
$applyall( $define_using_vec, 2i, 3i, 4i )

#define $define_using_mat( $0 ) using $paste( lib::math::mat, $0 );

$applyall( $define_using_mat, 2f, 3f, 4f )
$applyall( $define_using_mat, 2d, 3d, 4d )
$applyall( $define_using_mat, 2i, 3i, 4i )


namespace math = lib::math;


