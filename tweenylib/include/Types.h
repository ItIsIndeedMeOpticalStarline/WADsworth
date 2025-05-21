#ifndef TYPES_H
#define TYPES_H

// This file contains declarations of all the primitive types used by WAD files and WADsworth code.
// WAD file types are prefixed with wad_, and WADsworth by ww_.
// Types prefixed by neither are primitives that *should* be the the C standard library but are not.

#include "C.h"

C_BEGIN

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

// Use these types as much as possible. char, short, int, long, and unsigned are not our friends in
// compatability
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

typedef float fpnt32;
static_assert
(
    sizeof(fpnt32) == 4,
    "fpnt32 should be 32 bits! Please add and alternate typedef for your system/compiler.\n"
);

typedef double fpnt64;
static_assert
(
    sizeof(fpnt64) == 8, 
    "fpnt64 should be 64 bits! Please add and alternate typedef for your system/compiler.\n"
);

typedef intptr_t intptr;
typedef uintptr_t uintptr;
#pragma endregion

// Bytes are definitionally unsigned. If you add a signed byte to this file I will kill you.
typedef uint8_t byte;

// WADs use ASCII chars only. WADsworth uses UTF-8 chars for the UI.
// Please dear god use the correct corresponding typedefs.
typedef int8_t wad_char;

// WADs use 32 bit signed integers only.
typedef int32_t wad_int;

// Carmack didn't store the data in a WAD file as an array of bytes or as
// an untyped pointer, either of which would have made more sense for a 
// data stream. It should theoretically be fine to use a uint8_t typedef
// but for maximum compatibility this is done.
typedef int8_t wad_data;

// WADs use ASCII chars only. WADsworth uses UTF-8 chars for the UI. 
// Please dear god use the correct corresponding typedefs.
typedef int8_t ww_char;

// Returned from WADsworth functions that can fail.
// A value of true means the function was successful.
// A value of false means the function failed. 
// If the function failed check the error flags set in the global error stack.
typedef bool ww_ok;

// Returned from any and all WADsworth comparison functions.
// -1 or [LESS] if the left-hand side is less than the right-hand side.
// 0 or [EQUAL] if the left-hand side and right-hand side are equal.
// 1 or [GREATER] if the left-hand side is greater than the right-hand side.
typedef int_fast8_t ww_comp;

// Returned from a comparison function if the left-hand side is less than the right-hand side.
#define LESS LITERAL((ww_comp)-1)

// Returned from a comparison function if the left-hand side and right-hand side are equal.
#define EQUAL LITERAL((ww_comp)0)

// Returned from a comparison function if the left-hand side is greater than the right-hand side.
#define GREATER LITERAL((ww_comp)1)

C_END

#endif