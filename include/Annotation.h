#ifndef ANNOTATION_H
#define ANNOTATION_H

#include "C.h"

C_BEGIN

// Place in front of foreword declarations.
// Why not use a comment?
// Well...
// A lot of modern IDEs display a function signature and the associated comment.
// The signature is not either the declaration or definition, but whichever 
// comes first in the file. -_-;
// This could be remidied by placing the comment over both functions, but what 
// if someone changes a function and one of the comments, but not the other?
// Documentation inconsistency.
// So this exists for consistency's sake.
#define foreword_declaration

// Place in front of parameters that are to be return values assigned to via 
// a pointer. No tuples in C, at least not yet.
#define result

C_END

#endif