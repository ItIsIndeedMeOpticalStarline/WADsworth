#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// Algorithms that should have been in the standard library. :/

#include "C.h"

C_BEGIN

#include "Types.h"

// Classic DJB2 hash. Use internet for a more in depth explanation.
uint32 DJB2(byte* str);

C_END

#endif