#pragma once

#include "config.h"
#include "macros.h"
#include "types.h"
#include "ptr.h"
#include "range.h"
#include "error.h"
#include "global.h"
#include "memory.h"
#include "log.h"
#include "algo.h"
#include "to-string.h"
#include "scope-guard.h"
#include "vector.h"
#include "string.h"
#include "value.h"
#include "object.h"
#include "result.h"

//#include ""

using lib::object;
using lib::value;
using lib::owner; 
using lib::make_out;
using lib::range;
using lib::move;
using lib::forward;
using liblog::info;
using liblog::endl;
using lib::owner_ptr;
using lib::make_owner;
using lib::vector;
using lib::string;
using libr::result;
using libr::make_result;


