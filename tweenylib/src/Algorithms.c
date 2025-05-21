#include "Algorithms.h"

uint32 DJB2(byte* str)
{
    uint32 hash = 5381;
    int32 i = 0;

    while ((i = *str++) != '\0')
        hash = ((hash << 5) + hash) + i;

    return hash;
}