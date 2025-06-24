#include "WWLocale.h"

#include "Errors.h"
#include "Memory.h"

#include <limits.h>
#include <locale.h>

#ifdef _WIN32
    #define NOMINMAX
    #define WIN32_LEAN_AND_MEAN
    #include "Windows.h"
#endif

// Convert [cStr] in ASCII (US-ASCII) to UTF8 and allocate result in [r]
ww_char* AsciiToUTF8(const wad_char* const buffer, size strLength)
{
    ww_char* result;
    if (strLength == 0)
    {
        result = Allocate(sizeof *result);
        *result = '\0';
        return result;
    }

    result = ZAllocate(sizeof *result * (strLength + 1));

    // Could use memcpy_s here, but memcpy_s can fail and it'd go against my own rules to use 
    // normal memcpy. The compiler will probably optimize it away anyways.
    for (size i = 0; i < strLength; i++)
        result[i] = (ww_char)buffer[i];

    return result;
}

ww_char* Latin1ToUTF8(const uint8* const buffer, size strLength)
{
    ww_char* result;

    if (strLength == 0)
    {
        result = Allocate(sizeof *result);
        *result = '\0';
        return result;
    }

    // Latin 1 characters can be, at max 2 bytes of UTF8. We use this to our advantage by allocating
    // a buffer that is garunteed to always have enough room for the UTF8 string and null 
    // terminator.
    result = ZAllocate(sizeof *result * (strLength * 2 + 1));

    ww_char* utf8Iterator = result;
    for (size i = 0; i < strLength; i++)
    {
        uint8 current = buffer[i];
        if (current < 128)
            *utf8Iterator++ = current;
        else
        {
            *utf8Iterator++ = 0xC0 | (current >> 6);
            *utf8Iterator++ = 0x80 | (current & 0x3F);
        }
    }
    
    // Shrink the result buffer back down to only what's needed. Remember to + 1 for the null
    // terminator!
    result = Reallocate(result, sizeof *result * (strnlen_s(result, strLength * 2) + 1));

    return result;
}

ww_ok UTF16ToUTF8(ww_char** wwa_result r, const wchar_t* const wBuffer, size strLength)
{
    if (strLength == 0)
    {
        *r = Allocate(sizeof **r);
        **r = '\0';
        return true;
    }

    #ifdef _WIN32
        if (strLength > INT_MAX)
        {
            Error(ERROR_TYPE_OUT_OF_RANGE);
            return false;
        }

        const int rByteLength = WideCharToMultiByte
        (
            CP_UTF8, 
            WC_ERR_INVALID_CHARS, 
            wBuffer,
            (int)strLength,
            NULL, 
            0, 
            NULL, 
            NULL
        );
        if (rByteLength == 0)
        {
            // TODO: more [error_type] specificity
            switch (GetLastError())
            {
                case ERROR_INSUFFICIENT_BUFFER:	// Fallthrough
                case ERROR_INVALID_FLAGS:		// Fallthrough
                case ERROR_INVALID_PARAMETER:	// Fallthrough
                case ERROR_NO_UNICODE_TRANSLATION: { Error(ERROR_TYPE_INVALID_ARGUMENT); } break;
                default: Panic(ERROR_TYPE_UNREACHABLE); break;
            }
        }

        *r = Allocate(rByteLength);
        if (WideCharToMultiByte
            (
                CP_UTF8, 
                WC_ERR_INVALID_CHARS, 
                wBuffer,
                (int)strLength,
                *r,
                rByteLength, 
                NULL, 
                NULL
            ) 
           == 0)
        {
            // TODO: more [error_type] specificity
            switch (GetLastError())
            {
                case ERROR_INSUFFICIENT_BUFFER:	// Fallthrough
                case ERROR_INVALID_FLAGS:		// Fallthrough
                case ERROR_INVALID_PARAMETER:	// Fallthrough
                case ERROR_NO_UNICODE_TRANSLATION: { Error(ERROR_TYPE_INVALID_ARGUMENT); } break;
                default: Panic(ERROR_TYPE_UNREACHABLE); break;
            }
        }
    #else
        #error UTF16ToUTF8(ww_char*, wchar_t*) relies on WIN32 API. Tough shit.
    #endif

    return true;
}

void UTF8Me(void)
{
    if (setlocale(LC_ALL, ".UTF-8") == NULL)
        Panic(ERROR_TYPE_BAD_LOCALE);
}