#include "WADStr.h"

#include "Errors.h"
#include "Memory.h"
#include "WWMath.h"

#include <stdlib.h>
#include <wchar.h>

ww_comp Compare_WADStr(const WADStr* const lhs, const WADStr* const rhs)
{
    return (ww_comp)Clamp
    (
        (*((uint64*)lhs->data) - *((uint64*)rhs->data)), 
        (uint64)LESS, 
        (uint64)GREATER
    );
}

ww_ok NewFromAsciiCStr_WADStr(WADStr* wwa_result r, const wad_char* cStr)
{
    *r = (WADStr){ .data = { 0 } };

    // memcpy_s and not strncpy_s because strncpy_s writes a null terminator for you that would
    // break compatability here
    errno_t err = memcpy_s(r->data, sizeof *r->data * WAD_STR_DATA_LENGTH, cStr, strnlen_s(cStr, WAD_STR_DATA_LENGTH));
    if (err != 0)
    {
        free(r->data);
        r = NULL;

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

ww_ok ToUTF8CStr_WADStr(ww_char** wwa_result r, const WADStr* const wadStr)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    const size wadStrLength = strnlen_s(wadStr->data, WAD_STR_DATA_LENGTH);

    const size rLength = wadStrLength + 1;
    r = ZAllocate(sizeof *r * rLength);

    errno_t err = memcpy_s(r, rLength, wadStr->data, wadStrLength);
    if (err != 0)
    {
        free(r);
        r = NULL;

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