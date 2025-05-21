#ifndef ERRORS_H
#define ERRORS_H

// WADsworth is a single threaded application, (for now,) so this error system is rather simple.
// A global hash table of stacks is created at runtime. Whenever a function fails, data related 
// to the error is pushed into the stack associated with the function name.
// Errors are pushed onto the stack using the "Error" and "Panic" functions.
// "Error" pushes the error up the call hierarchy for someone else to deal with, whereas Panic
// will attempt to write a crash report then halt execution immediately.
// Peek at the top of a stack using the "Peek" function.
// Errors can be popped from their associated stack using the "Resolve" function.
// Errors can be marked as ignored using the "Ignore" function.

#include "C.h"

C_BEGIN

#include "Metaprogramming.h"
#include "Types.h"

#include <stdbool.h>
#include <stdio.h>

// Specifies the error type of [Error]
typedef enum error_type_e
{
    ERROR_TYPE_BAD_LOCALE,
    ERROR_TYPE_INVALID_ARGUMENT,
    ERROR_TYPE_OUT_OF_MEMORY,
    ERROR_TYPE_OUT_OF_RANGE,
    ERROR_TYPE_UNREACHABLE,

    ERROR_TYPE_COUNT
} error_type;

// Return a general description of what the error_type means
const ww_char* ErrorTypeToString(error_type type);

// Holds general information related to an error thrown by WADsworth
typedef struct Error_s
{
    char* callerName;
    bool ignored;
    error_type type;

    #ifdef _DEBUG
        const char* file;
        int line;
    #endif
} Error;

// Write all errors in the global error hashtable to the hashtable's internal stream.
void Dump(void);

#ifdef _DEBUG
    // Internal function, use Error(type) macro instead.
    void Error_(char* functionName, error_type type, const char* file, int line);

    // Call to push an error to the global error hashtable.
    // Types:
    // - [type]: error_type
    // Return type: void
    #define Error(type) Error_(__func__, (type), __FILE__, __LINE__)

    // Internal function, use Panic(type) macro instead.
    void Panic_(char* functionName, error_type type, const char* file, int line);

    // Call to push an error to the global error hashtable, attempt to create a crash log, 
    // then halt execution.
    // Types:
    // - [type]: error_type
    // Return type: void
    #define Panic(type) Panic_(__func__, (type), __FILE__, __LINE__)
#else
    // Internal function, use Error(type) macro instead.
    void Error_(char* functionName, error_type type);

    // Call to push an error to the global error hashtable.
    // Types:
    // - [type]: error_type
    // Return type: void
    #define Error(type) Error_(__func__, (type))

    // Internal function, use Panic(type) macro instead.
    void Panic_(char* functionName, error_type type);

    // Call to push an error to the global error hashtable, attempt to create a crash log, 
    // then halt execution.
    // Types:
    // - [type]: error_type
    // Return type: void
    #define Panic(type) Panic_(__func__, (type))
#endif

// Call to mark [error] as ignored.
void Ignore(const Error* const error);

// Internal function, use Peek(function) macro instead.
const Error Peek_(char* functionName);

// Call to peek at the top of the error stack associated with [function].
// [function] should be the function that the error was created in.
// Return type: Error
#define Peek(function) Peek_(Stringify(function))

// Internal function, use Resolve(function) macro instead.
void Resolve_(char* functionName);

// Call to pop from the error stack associated with [functionName].
// [function] should be the function that the error was created in.
// Return type: const Error* const
#define Resolve(function) Resolve_(Stringify(function))

// Set the stream the global error hashtable will attempt to write to when [Dump(void)] is called
void SetErrorHashtableStream(FILE* stream);

C_END

#endif