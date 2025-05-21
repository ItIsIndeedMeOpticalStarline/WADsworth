#ifndef MEMORY_H
#define MEMORY_H

// Mostly just minor things related to dynamic memory allocation

#include "C.h"

C_BEGIN

#include "Annotations.h"
#include "Types.h"

#include <stdio.h>
#include <string.h>

// Allocate [bytes] ammount of memory and crash if we don't get it.
// NOTE: ANYTHING ALLOCATED WITH [Allocate] MUST BE REALLOCATED WITH [Reallocate]
// OR [TryReallocate] AND FREED WITH [FREE], OR THE MEMORY LEAK CHECKING BREAKS.
void* Allocate(size bytes);

#ifdef _DEBUG
    // Dump logs of all instances of [Allocate(size)], [TryAllocate(void*, size)], 
    // [ZAllocate(size)], and [TryZAllocate(void*, size)] being called without the
    // memory being freed by [Free(void*)]
    // Types:
    // - [outputStream]: FILE* const
    // Return type: void
    #define DumpMemoryLeaks(outputStream) DumpMemoryLeaks_((outputStream))

    // Internal function, use DumpMemoryLeaks(stream) macro instead.
    void DumpMemoryLeaks_(FILE* const outputStream);
#else
    #define DumpMemoryLeaks(outputStream)
#endif

// Free [memory] from the heap and set [memory] to be a NULL pointer
void Free(void* memory);

// Reallocate [memory] to be a size of [bytes] and crash if we don't get it.
void* Reallocate(void* memory, size bytes);

// Try to reallocate [memory] to be a size of [bytes], and throw an error if we
// don't get it.
ww_ok TryReallocate(void** wwa_result memory, size bytes);

// Try to allocate [bytes] ammount of memory and throw an error if we don't get it.
// NOTE: ANYTHING ALLOCATED WITH [TryAllocate] MUST BE REALLOCATED WITH [Reallocate]
// OR [TryReallocate] AND FREED WITH [FREE], OR THE MEMORY LEAK CHECKING BREAKS.
ww_ok TryAllocate(void** wwa_result resultingMemory, size bytes);

// Try to allocate [bytes] ammount of memory, set all memory to be 0x0, and throw 
// an error if either thing fails.
// NOTE: ANYTHING ALLOCATED WITH [TryZAllocate] MUST BE REALLOCATED WITH [Reallocate]
// OR [TryReallocate] AND FREED WITH [FREE], OR THE MEMORY LEAK CHECKING BREAKS.
ww_ok TryZAllocate(void** wwa_result resultingMemory, size bytes);

// Allocate [bytes] ammount of memory, set all memory to be 0x0, and crash if 
// either thing fails.
// NOTE: ANYTHING ALLOCATED WITH [ZAllocate] MUST BE REALLOCATED WITH [Reallocate]
// OR [TryReallocate] AND FREED WITH [FREE], OR THE MEMORY LEAK CHECKING BREAKS.
void* ZAllocate(size bytes);

#ifndef _MINWINBASE_
    // Basically ZeroMemory from the WIN32 API
    // Types:
    // - [memory]: void*
    // - [bytes]: size
    // Return type: void*
    #define WWZeroMemory(memory, bytes) memset((memory), 0, (bytes))
#endif

C_END

#endif