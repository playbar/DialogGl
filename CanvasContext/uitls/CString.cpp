
#include "CString.h"
#include "Globaldef.h"

//#include "wtf/PartitionAlloc.h"
//#include "wtf/WTF.h"
//#include <string.h>

using namespace std;


PassRefPtr<CStringBuffer> CStringBuffer::createUninitialized(size_t length)
{
    ASSERT(length < (numeric_limits<unsigned>::max() - sizeof(CStringBuffer)));

    // The +1 is for the terminating NUL character.
    size_t size = sizeof(CStringBuffer) + length + 1;
    CStringBuffer* stringBuffer = static_cast<CStringBuffer*>(malloc(size));
    return adoptRef(new (stringBuffer) CStringBuffer(length));
}

void CStringBuffer::operator delete(void* ptr)
{
    free(ptr);
}

CString::CString(const char* str)
{
    if (!str)
        return;

    init(str, strlen(str));
}

CString::CString(const char* str, size_t length)
{
    if (!str) {
        ASSERT(!length);
        return;
    }

    init(str, length);
}

void CString::init(const char* str, size_t length)
{
    ASSERT(str);

    m_buffer = CStringBuffer::createUninitialized(length);
    memcpy(m_buffer->mutableData(), str, length);
    m_buffer->mutableData()[length] = '\0';
}

char* CString::mutableData()
{
    copyBufferIfNeeded();
    if (!m_buffer)
        return 0;
    return m_buffer->mutableData();
}

CString CString::newUninitialized(size_t length, char*& characterBuffer)
{
    CString result;
    result.m_buffer = CStringBuffer::createUninitialized(length);
    char* bytes = result.m_buffer->mutableData();
    bytes[length] = '\0';
    characterBuffer = bytes;
    return result;
}

void CString::copyBufferIfNeeded()
{
    if (!m_buffer || m_buffer->hasOneRef())
        return;

    RefPtr<CStringBuffer> buffer = m_buffer.release();
    size_t length = buffer->length();
    m_buffer = CStringBuffer::createUninitialized(length);
    memcpy(m_buffer->mutableData(), buffer->data(), length + 1);
}

bool CString::isSafeToSendToAnotherThread() const
{
    return !m_buffer || m_buffer->hasOneRef();
}

bool operator==(const CString& a, const CString& b)
{
    if (a.isNull() != b.isNull())
        return false;
    if (a.length() != b.length())
        return false;
    return !memcmp(a.data(), b.data(), a.length());
}

bool operator==(const CString& a, const char* b)
{
    if (a.isNull() != !b)
        return false;
    if (!b)
        return true;
    return !strcmp(a.data(), b);
}
