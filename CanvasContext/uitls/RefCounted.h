#ifndef RefCounted_h
#define RefCounted_h

class RefCountedBase
{
public:
    void ref()
    {
        ++m_refCount;
    }

    bool hasOneRef() const
    {
        return m_refCount == 1;
    }

    int refCount() const
    {
        return m_refCount;
    }

    void relaxAdoptionRequirement()
    {

    }

protected:
    RefCountedBase()
        : m_refCount(1)
    {
    }

    ~RefCountedBase()
    {

    }

    // Returns whether the pointer should be freed or not.
    bool derefBase()
    {
        if (m_refCount == 1) {
            return true;
        }
        --m_refCount;

        return false;
    }

private:
    int m_refCount;

};


template<typename T> class RefCounted : public RefCountedBase
{
private:
	RefCounted(const RefCounted&);
	RefCounted& operator==(const RefCounted&);
    WTF_MAKE_FAST_ALLOCATED;

public:
    void deref()
    {
        if (derefBase())
            delete static_cast<T*>(this);
    }
protected:
    RefCounted()
    {
    }
};


#endif // RefCounted_h
