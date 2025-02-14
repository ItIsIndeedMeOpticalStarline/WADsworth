#ifndef TYPES_H
#define TYPES_H

// This file contains declarations of all the primitive types used by WAD files and WADsworth code.
// WAD file types are prefixed with wad_, and WADsworth by ww_.
// Types prefixed by neither are primitives that *should* be the the C standard library but are not.

#include "C.h"

C_BEGIN

#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// Use these types as much as possible. char, short, int, long, and unsigned are not our friends in compatability
#pragma region Shorthand types
typedef size_t size;
typedef rsize_t rsize;

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float_t fpnt32;
typedef double_t fpnt64;
#pragma endregion

// Bytes are definitionally unsigned. If you add a signed byte to this file I will kill you.
typedef uint8_t byte;

// WADs use ASCII chars only, and all chars as a result are 8 bits. WADsworth uses UTF16 wide chars for the UI. 
// Do not mix these up.
typedef int8_t wad_char;

// WADs use 32 bit signed integers only.
typedef int32_t wad_int;

// WADs use ASCII chars only, and all chars as a result are 8 bits. WADsworth uses UTF16 wide chars for the UI. 
// Do not mix these up, or make a narrowing conversion from ww_char to wad_char.
typedef wchar_t ww_char;

// Returned from WADsworth functions that can fail
// A value of true means the function was successful.
// A value of false means the function failed. 
// If the function failed check the error flags set in the global error stack.
typedef bool ww_ok;

C_END

#endif