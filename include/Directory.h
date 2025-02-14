#ifndef DIRECTORY_H
#define DIRECTORY_H

// This file contains declarations related to the Directory, which associates data in a WAD file with a name.
// Would have made more sense to call this section of the WAD file the "Dictionary" instead of "Directory,"
// but alas, the name is set in stone. Curse you Carmak!!

#include "C.h"

C_BEGIN

#include "WADStr.h"

// Order of these fields is important. Do not move them!
typedef struct WADLump_s
{
	wad_int filePos;
	wad_int size;
	WADStr name;
} WADLump;

// A virtual lump is a lump which has no associated data. 
// When checking for a virtual lump, check against this value *and* VIRTUAL_WAD_LUMP_SIZE instead of 0s in case we change virtual lumps in the future.
#define VIRTUAL_WAD_LUMP_FILE_POS LITERAL(0)

// A virtual lump is a lump which has no associated data. 
// When checking for a virtual lump, check against this value *and* VIRTUAL_WAD_LUMP_FILE_POS instead of 0s in case we change virtual lumps in the future.
#define VIRTUAL_WAD_LUMP_SIZE LITERAL(0)

C_END

#endif