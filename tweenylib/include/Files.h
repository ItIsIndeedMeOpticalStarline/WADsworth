#ifndef FILES_H
#define FILES_H

#include "C.h"

C_BEGIN

#include "Annotations.h"
#include "Types.h"

#include <stdio.h>

// Contains multiple strings that are parts of a classic "file path."
// Also contains some data on whether the path is absolute or not
typedef struct Path_s
{
    bool absolute;
    ww_char* extension;
    ww_char* fileName;
    ww_char* pathToFile;
} Path;

// Frees a path [p]
void Delete_Path(Path* p);

// Creates a path [r] from [filePathCStr].
ww_ok New_Path(Path* wwa_result r, const ww_char* const filePathCStr);

// Allocates a CStr [r] composed of the components held by [p] appended together.
ww_ok ToCStr_Path(ww_char** wwa_result r, const Path* const p);

// Opens a file from a path [filePath] and returns a handle [r].
ww_ok TryOpenFileFromPath(FILE** wwa_result r, const Path* const filePath, const ww_char* mode);

// Opens a file from a wcstr [filePathCStr] and returns a handle [r].
ww_ok TryOpenFileFromCStr(FILE** wwa_result r, const ww_char* const filePathCStr, const ww_char* mode);

C_END

#endif