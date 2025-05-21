#include "Files.h"

#include "Errors.h"
#include "Memory.h"

#include <ctype.h>
#include <stdlib.h>

void Delete_Path(Path* p)
{
    free(p->extension);
    p->extension = NULL;
    free(p->fileName);
    p->fileName = NULL;
    free(p->pathToFile);
    p->pathToFile = NULL;
}

ww_ok ExtractSegment_(ww_char** wwa_result r, const ww_char* const pathCStr, size startOffset, size endOffset)
{
    assert(r != NULL);

    // Example:
    // [pathLength] == 31
    // [startOffset] == 21 (^)
    // [segmentEnd] == 29 (*)
    // . / T h i s / i s / a     r  e  l  a  t  i  v  e  /  f  i  l  e     p  a  t  h  .  \0
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24 25 26 27 28 29 30 31
    //														^						*

    // [+ 1] because [startOffset] and [segmentEnd] are zero-indexed, [+ 1] for null terminator
    const size extensionLength = endOffset - startOffset + 1 + 1;
    *r = ZAllocate(sizeof **r * extensionLength);

    if (_set_errno(0))
        Panic(ERROR_TYPE_UNREACHABLE);

    // TODO: [errno_t] to [error_type] translation
    // [extensionLength - 1] because we don't want to write over the null terminator from ZAllocate
    errno_t err = memcpy_s
    (
        *r,
        extensionLength,
        pathCStr + startOffset,
        extensionLength - 1
    );
    if (err != 0)
    {
        free(*r);
        *r = NULL;

        switch (err)
        {
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
            default: Panic(ERROR_TYPE_UNREACHABLE); break;
        }
    }

    return true;
}

ww_ok New_Path(Path* wwa_result r, const ww_char* const filePathCStr)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    const size filePathLength = strlen(filePathCStr);
    if (filePathLength < 1)
        return false;

    // [- 1] Makes our job easier later :)
    size lastFound = filePathLength - 1;
    bool isFilePath = true, foundExtension = false, foundFileName = false;
    // [- 1] So we don't spend time checking the null terminator
    for (size i = filePathLength - 1; i > 0; i--)
    {
        switch (filePathCStr[i])
        {
            case '.':
            {
                if (!foundExtension)
                {
                    foundExtension = true;

                    ExtractSegment_(&r->extension, filePathCStr, i, lastFound);

                    lastFound = i;
                }
            } break;
            case ':': // Fallthrough
            case '/': // Fallthrough
            case '\\':
            {
                if (!foundExtension)
                {
                    isFilePath = false;
                    break;
                }

                if (!foundFileName)
                {
                    foundFileName = true;

                    // [i + 1] to trim leading slash, [lastFound - 1] to trim the trailing .
                    ExtractSegment_(&r->fileName, filePathCStr, i + 1, lastFound - 1);

                    lastFound = i;
                }
            } break;
        }

        // We've got everything we need so let's scram
        if (foundExtension && foundFileName)
            break;

        // This isn't a path to a file so we don't need to do further evaluation in this loop
        if (!isFilePath)
            break;
    }

    if (!foundFileName)
    {
        // This is just a file name in our directory
        if (foundExtension)
        {
            r->absolute = false;

            // [lastFound - 1] to trim the trailing .
            ExtractSegment_(&r->fileName, filePathCStr, 0, lastFound - 1);

            r->pathToFile = ZAllocate(sizeof *r->pathToFile);

            return true;
        }
        else // Just a path to a directory
        {
            r->extension = ZAllocate(sizeof *r->extension);
            r->fileName = ZAllocate(sizeof *r->fileName);
        }
    }

    ExtractSegment_(&r->pathToFile, filePathCStr, 0, lastFound);

    #ifndef _WIN32
    #error Paths returned from New_Path(Path*, const ww_char* const) may not have absolute field \
set correctly
    #endif

    // TODO: Check that supports OSes other than Windows and that are not bad
    if (filePathLength > 1 &&  r->pathToFile[1] == ':')
        r->absolute = true;
    else
        r->absolute = false;

    return true;
}

ww_ok ToCStr_Path(ww_char** wwa_result r, const Path* const p)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    size rLength = strlen(p->extension) + strlen(p->fileName) + strlen(p->pathToFile) + 1; // One extra for null terminator

    *r = ZAllocate(sizeof **r * rLength);

    // TODO: [errno_t] to [error_type] translation
    errno_t err = strcat_s(*r, rLength, p->pathToFile);
    if (err != 0)
    {
        free(*r);
        *r = NULL;

        switch (errno)
        {
            case EINVAL:
            {
                Error(ERROR_TYPE_INVALID_ARGUMENT);
                return false;
            } break;
            default: Panic(ERROR_TYPE_UNREACHABLE); break;
        }
    }

    // TODO: [errno_t] to [error_type] translation
    err = strcat_s(*r, rLength, p->fileName);
    if (err != 0)
    {
        free(*r);
        *r = NULL;

        switch (err)
        {
            case EINVAL: 
            {
                Error(ERROR_TYPE_INVALID_ARGUMENT);
                return false;
            } break;
            default: Panic(ERROR_TYPE_UNREACHABLE); break;
        }
    }

    // TODO: [errno_t] to [error_type] translation
    err = strcat_s(*r, rLength, p->extension);
    if (err != 0)
    {
        free(*r);
        *r = NULL;

        switch (err)
        {
            case EINVAL:
            {
                Error(ERROR_TYPE_INVALID_ARGUMENT);
                return false;
            } break;
            default: Panic(ERROR_TYPE_UNREACHABLE); break;
        }
    }

    return true;
}

ww_ok TryOpenFileFromPath(FILE** wwa_result r, const Path* const filePath, const ww_char* mode)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    ww_char* pathCStr = NULL;

    if (!ToCStr_Path(&pathCStr, filePath))
        return false;

    if (!TryOpenFileFromCStr(r, pathCStr, mode))
    {
        free(pathCStr);
        return false;
    }

    free(pathCStr);
    return true;
}

ww_ok TryOpenFileFromCStr(FILE** wwa_result r, const ww_char* const filePathCStr, const ww_char* mode)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    errno_t err = fopen_s(r, filePathCStr, mode);
    if (err != 0)
    {
        *r = NULL;

        switch (err)
        {
            case EINVAL:
            {
                Error(ERROR_TYPE_INVALID_ARGUMENT);
                return false;
            } break;
            default: Panic(ERROR_TYPE_UNREACHABLE); break;
        }
    }

    return true;
}