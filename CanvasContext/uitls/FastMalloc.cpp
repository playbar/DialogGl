#include "FastMalloc.h"
#include "Globaldef.h"
#include <stdlib.h>
#include <string.h>


void* fastZeroedMalloc(size_t n)
{
    void* result = fastMalloc(n);
    memset(result, 0, n);
    return result;
}

char* fastStrDup(const char* src)
{
    size_t len = strlen(src) + 1;
    char* dup = static_cast<char*>(fastMalloc(len));
    memcpy(dup, src, len);
    return dup;
}

// TODO: remove these two.
void releaseFastMallocFreeMemory() { }

FastMallocStatistics fastMallocStatistics()
{
    FastMallocStatistics statistics = { 0, 0, 0 };
    return statistics;
}


void fastMallocShutdown()
{
}

void* fastMalloc(size_t n)
{
    void* result = malloc(n);
    ASSERT(result);  // We expect tcmalloc underneath, which would crash instead of getting here.

    return result;
}

void fastFree(void* p)
{
    free(p);
}

void* fastRealloc(void* p, size_t n)
{
    void* result = realloc(p, n);
    ASSERT(result);  // We expect tcmalloc underneath, which would crash instead of getting here.

    return result;
}


