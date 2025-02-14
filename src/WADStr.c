#include "Error.h"
#include "Memory.h"
#include "WADStr.h"

#include <stdlib.h>
#include <wchar.h>

ww_ok ToCStr_WADStr(wad_char* r, const WADStr const* wadStr)
{
	const size wadStrLength = wcsnlen_s(wadStr->data, WAD_STR_DATA_LENGTH);

	const size rLength = wadStrLength + 1;
	r = ZAllocate(sizeof *r * rLength);

	errno_t err = wmemcpy_s(r, rLength, wadStr->data, wadStrLength);
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