#include "Files.h"

#include "Error.h"
#include "Memory.h"

#include <memory.h>
#include <stdlib.h>
#include <wchar.h>

void Delete_Path(Path* p)
{
	free(p->extension);
	free(p->fileName);
	free(p->pathToFile);
}

ww_ok New_Path(Path* result r, const ww_char* filePathCStr)
{
	const size filePathLength = wcslen(filePathCStr);
	size lastFound = filePathLength;
	bool foundExtension = false, foundFileName = false;
	for (size i = filePathLength; i > 0; i--)
	{
		switch (filePathCStr[i])
		{
			case L'.':
			{
				if (!foundExtension)
				{
					foundExtension = true;

					const size extensionLength = i - lastFound;
					r->extension = ZAllocate(sizeof *r->extension * extensionLength);

					// TODO: [errno_t] to [error_type] translation
					errno_t err = wmemcpy_s(r->extension, extensionLength, filePathCStr + i, filePathLength - i);
					if (err != 0)
					{
						free(r->extension);
						r->extension = NULL;

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

					lastFound = i;
				}
			} break;
			case L'/': // Fallthrough
			case L'\\':
			{
				if (!foundFileName)
				{
					foundFileName = true;

					const size fileNameLength = i - lastFound;
					r->fileName = ZAllocate(sizeof *r->fileName * fileNameLength);

					if (!_set_errno(0))
						Panic(ERROR_TYPE_UNREACHABLE);

					// TODO: [errno] to [error_type] translation
					errno_t err = wmemcpy_s(r->fileName, fileNameLength, filePathCStr + i, filePathLength - i);
					if (err != 0)
					{
						free(r->extension);
						r->extension = NULL;

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

					lastFound = i;
				}
			} break;
		}
	}

	const size pathToFileLength = filePathLength - lastFound;
	r->pathToFile = ZAllocate(sizeof * r->pathToFile * pathToFileLength);

	// TODO: [errno] to [error_type] translation
	errno_t err = wmemcpy_s(r->pathToFile, pathToFileLength, filePathCStr, filePathLength);
	if (err != 0)
	{
		free(r->extension);
		r->extension = NULL;

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

	// TODO: Check that supports OSes other than Windows and that are not bad
	if (pathToFileLength != 0 && !iswalpha(r->pathToFile[0]))
		r->absolute = false;
	else
		r->absolute = true;

	return true;
}

ww_ok ToCStr_Path(ww_char* result r, Path* p)
{
	size rLength = wcslen(p->extension) + wcslen(p->fileName) + wcslen(p->pathToFile) + 1; // One extra for null terminator

	r = ZAllocate(sizeof *r * rLength);

	// TODO: [errno_t] to [error_type] translation
	errno_t err = wcscat_s(r, rLength, p->pathToFile);
	if (err != 0)
	{
		free(r);
		r = NULL;

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
	err = wcscat_s(r, rLength, p->fileName);
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
			default: Panic(ERROR_TYPE_UNREACHABLE); break;
		}
	}

	// TODO: [errno_t] to [error_type] translation
	err = wcscat_s(r, rLength, p->extension);
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
			default: Panic(ERROR_TYPE_UNREACHABLE); break;
		}
	}

	return true;
}

ww_ok TryOpenFileFromPath(FILE* result r, const Path* const filePath, const ww_char* mode)
{
	ww_char* pathCStr = NULL;

	if (!ToCStr_Path(pathCStr, filePath))
		return false;

	if (!TryOpenFileFromCStr(r, pathCStr, mode))
	{
		free(pathCStr);
		return false;
	}

	free(pathCStr);
	return true;
}

ww_ok TryOpenFileFromCStr(FILE* result r, const ww_char* const filePathCStr, const ww_char* mode)
{
	errno_t err = _wfopen_s(&r, filePathCStr, mode);
	if (err != 0)
	{
		r = NULL;

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