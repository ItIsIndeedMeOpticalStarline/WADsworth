#ifndef WWMATH_H
#define WWMATH_H

// General mathematical functions which should exist in math.h but do not because god is dead

#include "C.h"

C_BEGIN

#include "Types.h"

// Damn you WIN32 API
#define NOMINMAX

int8 Max8(int8 a, int8 b);
uint8 MaxU8(uint8 a, uint8 b);
int16 Max16(int16 a, int16 b);
uint16 MaxU16(uint16 a, uint16 b);
int32 Max32(int32 a, int32 b);
uint32 MaxU32(uint32 a, uint32 b);
int64 Max64(int64 a, int64 b);
uint64 MaxU64(uint64 a, uint64 b);
fpnt32 MaxF32(fpnt32 a, fpnt32 b);
fpnt64 MaxF64(fpnt64 a, fpnt64 b);

// Call to get the maximum value, [a] or [b]
// Types:
// - [a]: _Generic, see implementation
// - [b]: _Generic, see implementation
// Return type: _Generic, see implementation
#define Max(a, b) _Generic(((a) + (b)),	\
    int8: Max8,							\
    uint8: MaxU8,						\
    int16: Max16,						\
    uint16: MaxU16,						\
    int32: Max32,						\
    uint32: MaxU32,						\
    int64: Max64,						\
    uint64: MaxU64,						\
    fpnt32: MaxF32,						\
    fpnt64: MaxF64						\
    )((a), (b))

int8 Min8(int8 a, int8 b);
uint8 MinU8(uint8 a, uint8 b);
int16 Min16(int16 a, int16 b);
uint16 MinU16(uint16 a, uint16 b);
int32 Min32(int32 a, int32 b);
uint32 MinU32(uint32 a, uint32 b);
int64 Min64(int64 a, int64 b);
uint64 MinU64(uint64 a, uint64 b);
fpnt32 MinF32(fpnt32 a, fpnt32 b);
fpnt64 MinF64(fpnt64 a, fpnt64 b);

// Call to get the minimum value, [a] or [b]
// Types:
// - [a]: _Generic, see implementation
// - [b]: _Generic, see implementation
// Return type: _Generic, see implementation
#define Min(a, b) _Generic(((a) + (b)),	\
    int8: Min8,							\
    uint8: MinU8,						\
    int16: Min16,						\
    uint16: MinU16,						\
    int32: Min32,						\
    uint32: MinU32,						\
    int64: Min64,						\
    uint64: MinU64,						\
    fpnt32: MinF32,						\
    fpnt64: MinF64						\
    )((a), (b))

// TODO: Will not work but also will not fail if minimum > maximum
// Call to limit [value] to be between [minimum] and [maximum]
// Types:
// - [value]: _Generic, see implementation
// - [minimum]: _Generic, see implementation
// - [maximum]: _Generic, see implementation
// Return type: _Generic, see implementation
#define Clamp(value, minimum, maximum) Max((minimum), Min((value), (maximum)))

C_END

#endif