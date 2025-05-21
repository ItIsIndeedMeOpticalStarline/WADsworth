#include "WAD.h"

#include "Errors.h"

#include <errno.h>
#include <string.h>

ww_ok IsAWAD(bool* wwa_result r, FILE* const handle)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    if (handle == NULL)
    {
        r = NULL;
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    fseek(handle, 0, SEEK_END);
    long size = ftell(handle);
    fseek(handle, 0, SEEK_SET);
    
    if (size < sizeof(WADHeader))
    {
        *r = false;
        return true;
    }

    if (_set_errno(0))
        Panic(ERROR_TYPE_UNREACHABLE);

    wad_char identification[HEADER_IDENTIFICATION_SIZE] = { 0 };
    fread_s
    (
        identification, 
        sizeof *identification * HEADER_IDENTIFICATION_SIZE, 
        sizeof *identification, 
        HEADER_IDENTIFICATION_SIZE, 
        handle
    );

    // TODO: [errno_t] to [error_type] translation.
    if (errno == EINVAL)
    {
        r = NULL;
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    if (identification[0] != headerIdentificationInternal[0] && 
        identification[0] != headerIdentificationPatch[0])
    {
        *r = false;
        return true;
    }

    // Hack-y strncmp use to compare part of a string. Debated using memcmp, but that just seemed
    // misleading. Just remember that since we are offseting from the string by one *and* not using
    // null terminated strings, we must also lower our number of characters by one.
    if (strncmp(identification + 1, "WAD", HEADER_IDENTIFICATION_SIZE - 1) != 0)
    {
        *r = false;
        return true;
    }

    *r = true;
    return false;
}