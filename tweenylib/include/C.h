#ifndef C_H
#define C_H

// May want to split the WAD I/O portion of WADsworth to a library for use in other projects...
// If so, doing all the extern "C" work then would be a pain. Better to work ahead.

#ifdef __cplusplus
extern "C" 
{
#endif
    #ifdef __cplusplus
        // If using this header in C++ this marks the beginning of C code for the C++ compiler.
        #define C_BEGIN extern "C" {
    #else
        // If using this header in C++ this marks the beginning of C code for the C++ compiler.
        #define C_BEGIN
    #endif

    #ifdef __cplusplus
        // If using this header in C++ this marks the end of C code for the C++ compiler.
        #define C_END }
    #else
        // If using this header in C++ this marks the end of C code for the C++ compiler.
        #define C_END
    #endif

    #ifdef __cplusplus
        // If using this header in C++ this tells the compiler this is a line of C code.
        #define C extern "C"
    #else
        // If using this header in C++ this tells the compiler this is a line of C code.
        #define C
    #endif

    #ifdef __cplusplus
        // Wrapper for macro constants because C++ shits the bed sometimes. REMEMBER TO USE FOR ALL
        // MACRO CONSTANTS WITH LITERAL VALUES!
        #define LITERAL(T) T
    #else
        // Wrapper for macro constants because C++ shits the bed sometimes. REMEMBER TO USE FOR ALL
        // MACRO CONSTANTS WITH LITERAL VALUES!
        #define LITERAL(T) (T)
    #endif
#ifdef __cplusplus
}
#endif

#endif