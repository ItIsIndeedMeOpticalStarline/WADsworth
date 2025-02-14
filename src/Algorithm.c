#include "Algorithm.h"

uint32 DJB2(byte* str)
{
	uint32 hash = 5381;
	size i = 0;

	while (i = *str++)
		hash = ((hash << 5) + hash) + i;

	return hash;
}