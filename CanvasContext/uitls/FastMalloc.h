#ifndef WTF_FastMalloc_h
#define WTF_FastMalloc_h

#include <cstddef>

// Initialization is implicit on first use.
void fastMallocShutdown();

// These functions crash safely if an allocation fails.
void* fastMalloc(size_t);
void* fastZeroedMalloc(size_t);
void* fastRealloc(void*, size_t);
char* fastStrDup(const char*);
void fastFree(void*);
void releaseFastMallocFreeMemory();

struct FastMallocStatistics 
{
    size_t reservedVMBytes;
    size_t committedVMBytes;
    size_t freeListBytes;
};

FastMallocStatistics fastMallocStatistics();

#endif /* WTF_FastMalloc_h */
