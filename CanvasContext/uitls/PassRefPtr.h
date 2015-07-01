

#ifndef WTF_PassRefPtr_h
#define WTF_PassRefPtr_h
#include "TypeTraits.h"
#include "PassRefPtr.h"
#include "RawPtr.h"


template<typename T> class RefPtr;
template<typename T> class PassRefPtr;
template<typename T> PassRefPtr<T> adoptRef(T*);

inline void adopted(const void*) { }

inline void requireAdoption(const void*) { }

template<typename T> void refIfNotNull(T* ptr)
{
	if (ptr != 0) {
		requireAdoption(ptr);
		ptr->ref();
	}
}

template<typename T> void derefIfNotNull(T* ptr)
{
	if (LIKELY(ptr != 0))
		ptr->deref();
}

template<typename T> class PassRefPtr
{
	PassRefPtr(int);
public:
	PassRefPtr() : m_ptr(0) { }
	PassRefPtr(std::nullptr_t) : m_ptr(0) { }
	PassRefPtr(T* ptr) : m_ptr(ptr) { refIfNotNull(ptr); }
	template<typename U> PassRefPtr(const RawPtr<U>& ptr, EnsurePtrConvertibleArgDecl(U, T)) : m_ptr(ptr.get()) { refIfNotNull(m_ptr); }
	explicit PassRefPtr(T& ptr) : m_ptr(&ptr) { m_ptr->ref(); }
	PassRefPtr(const PassRefPtr& o) : m_ptr(o.leakRef()) { }
	template<typename U> PassRefPtr(const PassRefPtr<U>& o, EnsurePtrConvertibleArgDecl(U, T)) : m_ptr(o.leakRef()) { }

	~PassRefPtr() { derefIfNotNull(m_ptr); }

	template<typename U> PassRefPtr(const RefPtr<U>&, EnsurePtrConvertibleArgDecl(U, T));

	T* get() const { return m_ptr; }

	T* leakRef() const;

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }

	bool operator!() const { return !m_ptr; }

	// This conversion operator allows implicit conversion to bool but not to other integer types.
	typedef T* (PassRefPtr::*UnspecifiedBoolType);
	operator UnspecifiedBoolType() const { return m_ptr ? &PassRefPtr::m_ptr : 0; }

	friend PassRefPtr adoptRef<T>(T*);

private:
	enum AdoptRefTag { AdoptRef };
	PassRefPtr(T* ptr, AdoptRefTag) : m_ptr(ptr) { }

	PassRefPtr& operator=(const PassRefPtr&) { COMPILE_ASSERT(!sizeof(T*), PassRefPtr_should_never_be_assigned_to); return *this; }

	mutable T* m_ptr;
};

template<typename T> template<typename U> inline PassRefPtr<T>::PassRefPtr(const RefPtr<U>& o, EnsurePtrConvertibleArgDefn(U, T))
	: m_ptr(o.get())
{
	T* ptr = m_ptr;
	refIfNotNull(ptr);
}

template<typename T> inline T* PassRefPtr<T>::leakRef() const
{
	T* ptr = m_ptr;
	m_ptr = 0;
	return ptr;
}

template<typename T, typename U> inline bool operator==(const PassRefPtr<T>& a, const PassRefPtr<U>& b)
{
	return a.get() == b.get();
}

template<typename T, typename U> inline bool operator==(const PassRefPtr<T>& a, const RefPtr<U>& b)
{
	return a.get() == b.get();
}

template<typename T, typename U> inline bool operator==(const RefPtr<T>& a, const PassRefPtr<U>& b)
{
	return a.get() == b.get();
}

template<typename T, typename U> inline bool operator==(const PassRefPtr<T>& a, U* b)
{
	return a.get() == b;
}

template<typename T, typename U> inline bool operator==(T* a, const PassRefPtr<U>& b)
{
	return a == b.get();
}

template<typename T, typename U> inline bool operator!=(const PassRefPtr<T>& a, const PassRefPtr<U>& b)
{
	return a.get() != b.get();
}

template<typename T, typename U> inline bool operator!=(const PassRefPtr<T>& a, const RefPtr<U>& b)
{
	return a.get() != b.get();
}

template<typename T, typename U> inline bool operator!=(const RefPtr<T>& a, const PassRefPtr<U>& b)
{
	return a.get() != b.get();
}

template<typename T, typename U> inline bool operator!=(const PassRefPtr<T>& a, U* b)
{
	return a.get() != b;
}

template<typename T, typename U> inline bool operator!=(T* a, const PassRefPtr<U>& b)
{
	return a != b.get();
}

template<typename T> inline PassRefPtr<T> adoptRef(T* p)
{
	adopted(p);
	return PassRefPtr<T>(p, PassRefPtr<T>::AdoptRef);
}

template<typename T, typename U> inline PassRefPtr<T> static_pointer_cast(const PassRefPtr<U>& p)
{
	return adoptRef(static_cast<T*>(p.leakRef()));
}

template<typename T> inline T* getPtr(const PassRefPtr<T>& p)
{
	return p.get();
}

#endif // WTF_PassRefPtr_h
