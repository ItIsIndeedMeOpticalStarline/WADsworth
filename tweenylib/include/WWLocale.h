#ifndef WWLOCALE_H
#define WWLOCALE_H

#include "C.h"

C_BEGIN

#include "Annotations.h"
#include "Types.h"

// Convert [buffer] in ASCII (US-ASCII) with length of [strLength] to UTF8 and return the allocated
// string
ww_char* AsciiToUTF8(const wad_char* const buffer, size strLength);

// Convert [buffer] in Latin-1 (ISO 8859-1) with length of [strLength] to UTF8 and return the
// allocated string
ww_char* Latin1ToUTF8(const uint8* const buffer, size strLength);

// Convert [wBuffer] with length of [strLength] in UTF16 to UTF8 and allocate result in [r]
ww_ok UTF16ToUTF8(ww_char** wwa_result r, const wchar_t* const wBuffer, size strLength);

// Set global locale to UTF8
void UTF8Me(void);

C_END

#endif