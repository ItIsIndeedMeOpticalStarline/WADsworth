#include "Endianness.h"

int32 SwapInt32(int32 original)
{
    return 
    (int32)
    (
        (((uint32)original & 0x000000FFu) << 24) |
        (((uint32)original & 0x0000FF00u) << 8) |
        (((uint32)original & 0x00FF0000u) >> 8) |
        (((uint32)original & 0xFF000000u) >> 24)
    );
}