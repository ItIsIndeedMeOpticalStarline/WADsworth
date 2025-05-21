#ifndef ENDIANNESS_H
#define ENDIANNESS_H

// WADs store integers in little-endian format. These functions are tools to ensure propper
// endianess compatability across other OSes.

#include "C.h"

C_BEGIN

#include "Types.h"

// Swap endianness of [original]
int32 SwapInt32(int32 original);

#ifdef WW_BIG_ENDIAN
    // Swap endianness of [original] to little endian, if needed
    // Types:
    // - [original]: int32
    // Return type: wad_int
    #define HostToWadInt(original) (wad_int)SwapInt32((original))

    // Swap endianness of [original] to system endianess, if needed
    // Types:
    // - [original]: wad_int
    // Return type: int32
    #define WadIntToHost(original) SwapInt32((int32)(original))
#else
    // Swap endianness of [original] to little endian, if needed
    // Types:
    // - [original]: int32
    // Return type: wad_int
    #define HostToWadInt(original) (wad_int)(original)

    // Swap endianness of [original] to system endianess, if needed
    // Types:
    // - [original]: wad_int
    // Return type: int32
    #define WadIntToHost(original) (int32)(original)
#endif

C_END

#endif