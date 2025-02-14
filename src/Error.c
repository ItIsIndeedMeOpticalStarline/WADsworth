#include "Algorithm.h"
#include "Annotation.h"
#include "Error.h"
#include "Memory.h"

#include <assert.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

// See if two errors are equal
bool Equal_Error(const Error* a, const Error* b)
{
	// Don't check against values only present in _DEBUG for consistency
	// If you do decide to add in _DEBUG specific checks, be sure to change 
	// the implementation of [Ignore(const Error* const)].
	if (a->ignored != b->ignored || a->type != b->type || strcmp(a->callerName, b->callerName) != 0)
		return false;

	return true;
}


const ww_char* ErrorTypeToString(error_type type)
{
	static const ww_char* const strings[ERROR_TYPE_COUNT] =
	{
		"Argument provided was invalid.",
		"Failed a memory allocation.",
		"Went out of a provided range.",
		"Managed to activate unreachable code."
	};

	// Hack-y array size calculation. 
	// Only use this if we're doing something similar to this context.
	static_assert(sizeof(strings) / sizeof(*strings) == ERROR_TYPE_COUNT, L"You added an error type but forgot to update ErrorTypeToString(error_type)\n");

	return strings[type];
}

// Used to hold error data in memory without using extra memory for holding redundant pointers
typedef struct ErrorData_s_
{
	byte ignored : 1;
	error_type type;

	#ifdef _DEBUG
		const char* file;
		int line;
	#endif
} ErrorData_;

// Stack used to store the errors thrown from a specific function.
typedef struct ErrorDataStack_s_
{
	ErrorData_* data;
	size count, size;
} ErrorDataStack_;

// How big the stack is by default.
#define ERROR_DATA_STACK_DEFAULT_SIZE_ LITERAL(16)

// Allocate the internal memory for [es]. 
// Must free [es] with [Free_ErrorDataStack_(ErrorDataStack_*)].
void Alloc_ErrorDataStack_(ErrorDataStack_* eds)
{
	eds->data = malloc(sizeof *eds->data * ERROR_DATA_STACK_DEFAULT_SIZE_);
	if (eds->data == NULL)
	{
		fputws(L"Allocation of error data stack failed.\n", stderr);
		exit(EXIT_FAILURE);
	}

	eds->count = 0;
	eds->size = ERROR_DATA_STACK_DEFAULT_SIZE_;
}

// Frees the internal memory of [eds] allocated by [Alloc_ErrorDataStack_(ErrorStack* es)].
void Free_ErrorDataStack_(ErrorDataStack_* eds)
{
	free(eds->data);
	eds->data = NULL;

	eds->count = 0;
	eds->size = 0;
}

foreword_declaration void Resize_ErrorDataStack_(ErrorDataStack_* eds, size newSize);

// Resize [eds] to be twice as big with a minimum of [ERROR_DATA_STACK_DEFAULT_SIZE_].
void Grow_ErrorDataStack_(ErrorDataStack_* eds) { Resize_ErrorDataStack_(eds, Max(ERROR_DATA_STACK_DEFAULT_SIZE_, eds->size * 2)); }

// Returns a reference to the top element of [eds].
ErrorData_* Peek_ErrorDataStack_(ErrorDataStack_* eds) { return &eds->data[eds->count - 1]; }

// Remove top element of [eds].
void Pop_ErrorDataStack_(ErrorDataStack_* eds) { eds->count--; }

// Push [errorData] to the top of [es].
void Push_ErrorDataStack_(ErrorDataStack_* eds, ErrorData_ errorData)
{
	if (eds->count + 1 > eds->size)
		Grow_ErrorDataStack_(eds);

	eds->data[eds->count++] = errorData;
}

// Resize the internal memory from [eds] to newSize. 
// Will deallocate values, and may cause dangling pointers.
void Resize_ErrorDataStack_(ErrorDataStack_* eds, size newSize)
{
	Error* newData = realloc(eds->data, sizeof *eds->data * newSize);
	if (newData == NULL)
	{
		fputws(L"Reallocation of Error Stack failed.\n", stderr);
		exit(EXIT_FAILURE);
	}

	eds->data = newData;
	eds->count = Min(eds->count, newSize);
	eds->size = newSize;
}

// Inbetween struct for data structuring
typedef struct ErrorHashtableElement_s_
{
	const char* key;
	ErrorDataStack_ value;
} ErrorHashtableElement_;

// The hashtable that stores each error data stack associated with each function name.
// Only a defined struct so we can make copies of [errors_].
// End a function with more than one in memory and I will destroy you.
struct ErrorHashtable_s_
{
	bool initialized;
	FILE* stream;
	ErrorHashtableElement_* data;
	size count, size;
} errors_ = { 0 };

// How big the hashtable is by default.
#define ERROR_HASHTABLE_DEFAULT_SIZE_ LITERAL(16)

void Dump(void)
{
	if (!errors_.initialized)
		InitializeErrors_();

	if (errors_.count == 0)
		return;

	FILE* realStream = stderr;
	if (errors_.stream != NULL && ftell(errors_.stream) < 0)
		realStream = errors_.stream;

	for (size i = 0; i < errors_.count; i++)
	{
		for (size j = 0; j < errors_.data[i].value.count; j++)
		{
			const ErrorData_* const ed = &errors_.data[i].value.data[j];
			#ifdef _DEBUG
				int res = fwprintf_s
				(
					realStream, 
					ed->ignored ? L"ERROR (IGNORED): %S from %s in %s on line %i.\n" : L"ERROR: %S from %s in %s on line %i.\n", 
					ErrorTypeToString(ed->type), 
					errors_.data[i].key, 
					ed->file, 
					ed->line
				);
			#else
				int res = fwprintf_s
				(
					realStream,
					ed->ignored ? L"ERROR (IGNORED): %S from %s in.\n" : L"ERROR: %S from %s.\n",
					ErrorTypeToString(ed->type),
					errors_.data[i].key,
				);
			#endif

			if (res < 0)
			{
				fputws(L"Failed to dump error stack.\n", stderr);
				exit(EXIT_FAILURE);
			}
		}
	}
}

#ifdef _DEBUG
	void Error_(const char* functionName, error_type type, const char* file, int line)
	{
		if (!errors_.initialized)
			InitializeErrors_();

		Insert_(functionName, (ErrorData_){ .ignored = false, .type = type, .file = file, .line = line });
	}

	void Panic_(const char* functionName, error_type type, const char* file, int line)
	{
		if (!errors_.initialized)
			InitializeErrors_();

		Insert_(functionName, (ErrorData_){ .ignored = false, .type = type, .file = file, .line = line });

		Dump();

		exit(EXIT_FAILURE);
	}
#else
	void Error_(const char* functionName, error_type type)
	{
		if (!errors_.initialized)
			InitializeErrors_();

		Insert_(functionName, (ErrorData_){ .ignored = false, .type = type });
	}

	void Panic_(const char* functionName, error_type type)
	{
		if (!errors_.initialized)
			InitializeErrors_();

		Insert_(functionName, (ErrorData_){ .ignored = false, .type = type });

		Dump();

		exit(EXIT_FAILURE);
	}
#endif

foreword_declaration void InsertInto_(struct ErrorHashtable_s_* eh, const char* functionName, ErrorData_ error);

// Resize [errors_] to be twice as big with a minimum of [ERROR_HASHTABLE_DEFAULT_SIZE_].
// Memory has to be copied and freed because hashes change as [errors_] is resized. 
// Don't call too much.
void GrowErrors_(void) 
{
	const size newSize = Max(ERROR_HASHTABLE_DEFAULT_SIZE_, errors_.size * 2);
	struct ErrorHashtable_s_ new = { 0 };

	new.data = malloc(sizeof *new.data * newSize);
	if (new.data == NULL)
	{
		fputws(L"Reallocation of error hashtable failed.\n", stderr);
		exit(EXIT_FAILURE);
	}
	if (ZeroMemory(new.data, sizeof *new.data * newSize) != 0)
	{
		fputws(L"Zeroing of error hashtable failed.\n", stderr);
		exit(EXIT_FAILURE);
	}

	new.count = errors_.count;
	new.size = newSize;
	new.initialized = true;

	for (size i = 0; i < errors_.count; i++)
	{
		ErrorDataStack_* oldValue = &errors_.data[i].value;

		for (size j = 0; j < oldValue->count; j++)
			InsertInto_(&new, errors_.data[i].key, oldValue->data[j]);

		Free_ErrorDataStack_(oldValue);
	}

	free(errors_.data);

	errors_ = new;
}

void Ignore(const Error* const error)
{
	if (!errors_.initialized)
		InitializeErrors_();

	const uint32 cappedHash = DJB2(error->callerName) % errors_.size;
	ErrorDataStack_* errorStack = &errors_.data[cappedHash].value;

	for (size i = 0; i < errorStack->count; i++)
	{
		// No need to set the file and line fields in the Error initialized here, the resulting variable is only used for comparison and is promptly discarded
		if (Equal_Error(error, &(Error){ .callerName = errors_.data[cappedHash].key, .ignored = errorStack->data[i].ignored, .type = errorStack->data[i].type }))
		{
			errorStack->data[i].ignored = true;
			break;
		}
	}
}

// Allocate internal memory of [errors_].
// This memory should remain for the program's entire runtime, so there is no way to free it.
void InitializeErrors_(void)
{
	errors_.data = malloc(sizeof *errors_.data * ERROR_HASHTABLE_DEFAULT_SIZE_);
	if (errors_.data == NULL)
	{
		fputws(L"Allocation of error hashtable failed.\n", stderr);
		exit(EXIT_FAILURE);
	}
	if (ZeroMemory(errors_.data, sizeof *errors_.data * ERROR_HASHTABLE_DEFAULT_SIZE_) != 0)
	{
		fputws(L"Zeroing of error hashtable failed.\n", stderr);
		exit(EXIT_FAILURE);
	}

	errors_.count = 0;
	errors_.size = ERROR_HASHTABLE_DEFAULT_SIZE_;
	errors_.initialized = true;
}

// Insert key/value pair [functionName] and [errorData] into [errors_].
void Insert_(const char* functionName, ErrorData_ errorData)
{
	if (!errors_.initialized)
		InitializeErrors_();

	InsertInto_(&errors_, functionName, errorData);
}

// Insert into a given hashtable [eh] instead of into global variable [errors_]. 
// Useful for copying.
void InsertInto_(struct ErrorHashtable_s_* eh, const char* functionName, ErrorData_ errorData)
{
	if ((fpnt64)eh->count / (fpnt64)eh->size >= 0.5)
		GrowErrors_();

	ErrorDataStack_* errorStack = &eh->data[DJB2(functionName) % eh->size].value;

	if (errorStack->data == NULL)
		Alloc_ErrorDataStack_(errorStack);

	Push_ErrorDataStack_(errorStack, errorData);
}

const Error Peek_(const char* functionName)
{
	if (!errors_.initialized)
	{
		InitializeErrors_();
		return;
	}

	const ErrorData_* errorData = Peek_ErrorDataStack_(&errors_.data[DJB2(functionName) % errors_.size]);

	#ifdef _DEBUG
		return (Error){ .callerName = functionName, .type = errorData->type, .file = errorData->file, .line = errorData->line };
	#else
		return (Error){ .callerName = functionName, .type = errorData->type };
	#endif
}

void Resolve_(const char* functionName)
{
	if (!errors_.initialized)
	{
		InitializeErrors_();
		return;
	}

	Pop_ErrorDataStack_(&errors_.data[DJB2(functionName) % errors_.size].value);
}

void SetErrorHashtableStream(FILE* stream)
{
	if (!errors_.initialized)
		InitializeErrors_();

	errors_.stream = stream;
}