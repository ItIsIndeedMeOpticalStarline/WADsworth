#ifndef STR_H
#define STR_H

// This file contains declarations for an abstraction not present in WAD files, the WADStr.
// Since (almost) all strings in a WAD file are 8 characters regardless of length, 
// and if they are 8 characters not null terminated by the file specification,
// this abstraction hopes to make mistakes with null termination less common.

#include "C.h"

C_BEGIN

#include "Types.h"

// We always use this as a limit for functions used on WAD strings of undetermined length.
#define WAD_STR_DATA_LENGTH LITERAL(8)

// Could have typed this as a array typedef but this was done to prevent ambiguity and 
// degredation when passing WADStrs around.
// Keep in mind that WADStrs are DEFINITIONALLY NOT ALWAYS NULL TERMINATED
typedef struct WADStr_s
{
	wad_char data[WAD_STR_DATA_LENGTH]; // Not null terminated.
} WADStr;

// Convert a WADStr to a CStr to work with
ww_ok ToCStr_WADStr(wad_char* r, const WADStr const* wadStr);

C_END

#endif