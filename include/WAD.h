#ifndef WAD_H
#define WAD_H

// This file contains declarations for the WAD manipulation interface
// We do not follow zero cost abstractions in this file,
// sometimes sacrafices will be made for the user's convenience.

#include "C.h"

C_BEGIN

#include "Header.h"

// Order of these fields is important. Do not move them!
typedef struct WAD_s
{
	WADHeader header;
} WAD;

C_END

#endif