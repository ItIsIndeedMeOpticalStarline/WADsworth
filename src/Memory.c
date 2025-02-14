#include "Error.h"
#include "Memory.h"

#include <assert.h>
#include <stdlib.h>

void* Allocate(size bytes)
{
	void* memory = malloc(bytes);
	if (memory == NULL)
		Panic(ERROR_TYPE_OUT_OF_MEMORY);

	return memory;
}

ww_ok TryAllocate(void* resultingMemory, size bytes)
{
	resultingMemory = malloc(bytes);
	if (resultingMemory == NULL)
	{
		Error(ERROR_TYPE_OUT_OF_MEMORY);
		return false;
	}

	return true;
}

ww_ok TryZAllocate(void* resultingMemory, size bytes)
{
	resultingMemory = malloc(bytes);
	if (resultingMemory == NULL)
	{
		Error(ERROR_TYPE_OUT_OF_MEMORY);
		return false;
	}
	errno_t err = ZeroMemory(resultingMemory, bytes);
	switch (err)
	{
		case 0: break;
		case EINVAL:
		{ 
			Error(ERROR_TYPE_INVALID_ARGUMENT);
			return false;
		} break;
		case ERANGE:
		{
			Error(ERROR_TYPE_OUT_OF_RANGE);
			return false;
		} break;
		default:
		{
			Panic(ERROR_TYPE_UNREACHABLE);
			return false;
		} break;
	}

	return true;
}

void* ZAllocate(size bytes)
{
	void* memory = malloc(bytes);
	if (memory == NULL)
		Panic(ERROR_TYPE_OUT_OF_MEMORY);
	errno_t err = ZeroMemory(memory, bytes);
	switch (err)
	{
		case 0: break;
		case EINVAL: Panic(ERROR_TYPE_INVALID_ARGUMENT); break;
		case ERANGE: Panic(ERROR_TYPE_OUT_OF_RANGE); break;
		default: Panic(ERROR_TYPE_UNREACHABLE); break;
	}

	return memory;
}