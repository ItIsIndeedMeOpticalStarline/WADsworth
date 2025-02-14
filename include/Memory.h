#ifndef MEMORY_H
#define MEMORY_H

// Mostly just minor things related to dynamic memory allocation

#include "C.h"

C_BEGIN

#include "Types.h"

#include <string.h>

// Allocate [bytes] ammount of memory and crash if we don't get it.
void* Allocate(size bytes);

// Try to allocate [bytes] ammount of memory and throw an error if we don't get it.
ww_ok TryAllocate(void* resultingMemory, size bytes);

// Try to allocate [bytes] ammount of memory, set all memory to be 0x0, and throw 
// an error if either thing fails.
ww_ok TryZAllocate(void* resultingMemory, size bytes);

// Allocate [bytes] ammount of memory, set all memory to be 0x0, and crash if 
// either thing fails
void* ZAllocate(size bytes);

#ifndef _MINWINBASE_
	// Basically ZeroMemory from the WIN32 API, but safer (and more annoying)
	// Types:
	// - [memory]: void*
	// - [size]: rsize
	// Return type: errno_t
	#define ZeroMemory(memory, size) memset_s((memory), (size), 0, (size))
#endif

#ifndef _WINBASE_
	// Basically ZeroMemorySecure from the WIN32 API
	// Types:
	// - [memory]: void*
	// - [size]: size
	// Return type: void*
	#define ZeroMemorySecure(memory, size) memset_explicit((memory), 0, (size))
#endif

C_END

#endif