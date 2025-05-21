#ifndef WADSTR_H
#define WADSTR_H

// This file contains declarations for an abstraction not present in WAD files, the WADStr.
// Since (almost) all strings in a WAD file are 8 characters regardless of length, 
// and if they are 8 characters not null terminated by the file specification,
// this abstraction hopes to make mistakes with null termination less common.

#include "C.h"

C_BEGIN

#include "Annotations.h"
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

// Compare two WADStrs, [lhs] being the left-hand side and [rhs] being the right-hand side
ww_comp Compare_WADStr(const WADStr* const lhs, const WADStr* const rhs);

// Create a new wadstring from an ascii string buffer
ww_ok NewFromAsciiCStr_WADStr(WADStr* wwa_result r, const wad_char* cStr);

// Convert a WADStr to a ascii CStr to work with
ww_ok ToUTF8CStr_WADStr(ww_char** wwa_result r, const WADStr* const wadStr);

C_END

#endif