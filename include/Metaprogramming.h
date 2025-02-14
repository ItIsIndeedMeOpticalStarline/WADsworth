#ifndef METAPROGRAMMING_H
#define METAPROGRAMMING_H

#include "C.h"

C_BEGIN

// Used to mess with the C preprocessor
// Unless you know what you're doing, stick to Stringify(x)
#define Stringify_Impl_(x) #x

// Turns text into a string
// Types:
//  - [x]: Some text
// Return type: const char*
#define Stringify(x) Stringify_Impl_(#x)

C_END

#endif