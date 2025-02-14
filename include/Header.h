#ifndef HEADER_H
#define HEADER_H

// This file contains code relating to the Header of a WAD file, 
// which determines how the data from the WAD file is to be accessed.

#include "C.h"

C_BEGIN

#include "Types.h"

// One of the few "strings" in a WAD file without a length of 8 is the header's identification field.
// Use this when working with said field.
#define HEADER_IDENTIFICATION_SIZE LITERAL(4)

// Must be the first entry in the WAD struct.
typedef struct
{
	wad_char identification[HEADER_IDENTIFICATION_SIZE]; // Not null terminated.
	wad_int lumpsSize;
	wad_int infoTableOffset;
} WADHeader;

C_END

#endif