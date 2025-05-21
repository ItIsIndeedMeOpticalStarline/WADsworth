#ifndef WAD_H
#define WAD_H

// This file contains declarations for the WAD manipulation interface
// We do not follow zero cost abstractions in this file,
// sometimes sacrafices will be made for the user's convenience.

#include "C.h"

C_BEGIN

#include "Annotations.h"
#include "WADDirectory.h"
#include "WADHeader.h"

#include <stdio.h>

// Order of these fields is important. 
// Do not move them!
typedef struct WAD_s
{
    WADHeader header;
    WADLump* lumps;
    wad_data* data;		// Oh Carmack...
    wad_int dataSize;
} WAD;

// Determines if the file pointed to by handle is a WAD.
ww_ok IsAWAD(bool* wwa_result r, FILE* const handle);

C_END

#endif