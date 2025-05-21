#include "Memory.h"

#include "Errors.h"
#include "WWMath.h"

#include <assert.h>
#include <stdlib.h>

// TODO: Annotate file

#ifdef _DEBUG
    typedef struct MemoryInstance_s_
    {
        const char* file;
        uint8 ignored : 1;
        int line;
        struct MemoryInstance_s_* next, * prev;
    } MemoryInstance_;

    typedef struct Allocation_s_
    {
        void* key;
        MemoryInstance_* value;
    } Allocation_;

    void FreeMemoryInstances_Allocation_(Allocation_* a)
    {
        MemoryInstance_* iterator = a->value;
        while (iterator->next != NULL)
        {
            if (iterator->prev != NULL)
                free(iterator->prev);

            iterator = iterator->next;
        }

        free(iterator);
    }

    void InsertMemoryInstance_Allocation_(Allocation_* a, const char* file, int line)
    {
        if (a->value == NULL)
        {
            a->value = malloc(sizeof *a->value);
            if (a->value == NULL)
                Panic(ERROR_TYPE_OUT_OF_MEMORY);

            a->value->file = file;
            a->value->ignored = false;
            a->value->line = line;
            a->value->next = NULL;
            a->value->prev = NULL;
        }
        else
        {
            MemoryInstance_* iterator = a->value;
            while (iterator->next != NULL)
                iterator = iterator->next;

            iterator->next = malloc(sizeof *iterator->next);
            if (iterator->next == NULL)
                Panic(ERROR_TYPE_OUT_OF_MEMORY);

            iterator->next->file = file;
            iterator->next->ignored = false;
            iterator->next->line = line;
            iterator->next->next = NULL;
            iterator->next->prev = iterator;
        }
    }

    struct AllocationHashtable_s_
    {
        bool initialized;
        FILE* stream;
        Allocation_* data;
        size count, size;
    } allocations_ = { 0 };

    #define ALLOCATION_HASHTABLE_DEFAULT_SIZE_ LITERAL(16)

    void DumpMemoryLeaks_(FILE* const outputStream)
    {
        for (size i = 0; i < allocations_.count; i++)
        {
            MemoryInstance_* iterator = allocations_.data[i].value;

            while (iterator != NULL)
            {
                int res = fprintf_s
                (
                    outputStream, 
                    u8"MEMORY LEAK: in %s on line %i%s\n", 
                    iterator->file, 
                    iterator->line, 
                    iterator->ignored ? u8" [IGNORED]." : u8"."
                );
                if (res < 0)
                {
                    fputs(u8"Failed to dump memory leaks.\n", stderr);
                    exit(EXIT_FAILURE);
                }

                iterator = iterator->next;
            }
        }
    }

    wwa_foreword_declaration void InsertMemoryInstanceInto_(struct AllocationHashtable_s_* ah, void* memory, const char* file, int line);

    void GrowAllocations_(void)
    {
        const size newSize = Max(ALLOCATION_HASHTABLE_DEFAULT_SIZE_, allocations_.size * 2);
        struct AllocationHashtable_s_ new = { 0 };

        new.data = malloc(sizeof *new.data * newSize);
        if (new.data == NULL)
            Panic(ERROR_TYPE_OUT_OF_MEMORY);

        new.count = allocations_.count;
        new.size = newSize;
        new.stream = allocations_.stream;
        new.initialized = true;

        for (size i = 0; i < allocations_.count; i++)
        {
            MemoryInstance_* iterator = allocations_.data[i].value;

            while (iterator->next != NULL)
            {
                iterator = iterator->next;
                InsertMemoryInstanceInto_
                (
                    &new, 
                    allocations_.data[i].key,
                    iterator->file, 
                    iterator->line
                );
            }

            FreeMemoryInstances_Allocation_(&allocations_.data[i]);
        }

        free(allocations_.data);

        allocations_ = new;
    }

    // TODO: finish
    void NotToBeDeallocated(void* memory, const char* file, int line)
    {
        (void)memory;
        (void)file;
        (void)line;
    }

    void InitializeAllocations_(void)
    {
        allocations_.data = malloc(sizeof *allocations_.data * ALLOCATION_HASHTABLE_DEFAULT_SIZE_);
        if (allocations_.data == NULL)
            Panic(ERROR_TYPE_OUT_OF_MEMORY);

        allocations_.count = 0;
        allocations_.size = ALLOCATION_HASHTABLE_DEFAULT_SIZE_;
        allocations_.stream = NULL;
        allocations_.initialized = true;
    }

    void InsertMemoryInstance_(void* memory, const char* file, int line)
    {
        if (!allocations_.initialized)
            InitializeAllocations_();

        InsertMemoryInstanceInto_(&allocations_, memory, file, line);
    }

    void InsertMemoryInstanceInto_(struct AllocationHashtable_s_* ah, void* memory, const char* file, int line)
    {
        if ((fpnt64)ah->count / (fpnt64)ah->size >= 0.5)
            GrowAllocations_();

        Allocation_* allocation = &ah->data[(uintptr)memory % ah->size];

        if (allocation->value == NULL)
        {
            InsertMemoryInstance_Allocation_(allocation, file, line);
        }
    }
#endif

void* Allocate(size bytes)
{
    void* memory = malloc(bytes);
    if (memory == NULL)
        Panic(ERROR_TYPE_OUT_OF_MEMORY);

    return memory;
}

void Free(void* memory)
{
    free(memory);
    memory = NULL;
}

void* Reallocate(void* memory, size bytes)
{
    void* newMemory = realloc(memory, bytes);
    if (newMemory == NULL)
    {
        if (memory == NULL)
            Panic(ERROR_TYPE_INVALID_ARGUMENT);
        else
            Panic(ERROR_TYPE_OUT_OF_MEMORY);
    }
    
    memory = NULL;

    return newMemory;
}

ww_ok TryReallocate(void** wwa_result r, size bytes)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    void* newMemory = realloc(*r, bytes);
    if (newMemory == NULL)
    {
        if (*r == NULL)
            Error(ERROR_TYPE_INVALID_ARGUMENT);
        else
            Error(ERROR_TYPE_OUT_OF_MEMORY);

        return false;
    }

    *r = newMemory;

    return true;
}

ww_ok TryAllocate(void** wwa_result r, size bytes)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    *r = malloc(bytes);
    if (*r == NULL)
    {
        Error(ERROR_TYPE_OUT_OF_MEMORY);
        return false;
    }

    return true;
}

ww_ok TryZAllocate(void** wwa_result r, size bytes)
{
    if (r == NULL)
    {
        Error(ERROR_TYPE_INVALID_ARGUMENT);
        return false;
    }

    *r = malloc(bytes);
    if (*r == NULL)
    {
        Error(ERROR_TYPE_OUT_OF_MEMORY);
        return false;
    }

    WWZeroMemory(*r, bytes);

    return true;
}

void* ZAllocate(size bytes)
{
    void* memory = malloc(bytes);
    if (memory == NULL)
        Panic(ERROR_TYPE_OUT_OF_MEMORY);

    WWZeroMemory(memory, bytes);

    return memory;
}