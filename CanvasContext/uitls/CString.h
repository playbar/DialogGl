#ifndef CString_h
#define CString_h

#include "RefCounted.h"
#include "RefPtr.h"


class CStringBuffer : public RefCounted<CStringBuffer>
{
public:
    const char* data() { return mutableData(); }
    size_t length() const { return m_length; }

private:
    friend class CString;
    friend class RefCounted<CStringBuffer>;
    // CStringBuffers are allocated out of the WTF buffer partition.
    void* operator new(size_t, void* ptr) { return ptr; };
    void operator delete(void*);

    static PassRefPtr<CStringBuffer> createUninitialized(size_t length);

    CStringBuffer(size_t length) : m_length(length) { }
    char* mutableData() { return reinterpret_cast<char*>(this + 1); }

    const unsigned m_length;
};

// A container for a null-terminated char array supporting copy-on-write
// assignment.  The contained char array may be null.
class CString 
{
public:
    CString() { }
    CString(const char*);
    CString(const char*, size_t length);
    CString(CStringBuffer* buffer) : m_buffer(buffer) { }
    static CString newUninitialized(size_t length, char*& characterBuffer);

    const char* data() const
    {
        return m_buffer ? m_buffer->data() : 0;
    }
    char* mutableData();
    size_t length() const
    {
        return m_buffer ? m_buffer->length() : 0;
    }

    bool isNull() const { return !m_buffer; }
    bool isSafeToSendToAnotherThread() const;

    CStringBuffer* buffer() const { return m_buffer.get(); }

private:
    void copyBufferIfNeeded();
    void init(const char*, size_t length);
    RefPtr<CStringBuffer> m_buffer;
};

bool operator==(const CString& a, const CString& b);
inline bool operator!=(const CString& a, const CString& b) { return !(a == b); }
bool operator==(const CString& a, const char* b);
inline bool operator!=(const CString& a, const char* b) { return !(a == b); }

#endif // CString_h

