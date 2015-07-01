#ifndef WTF_RefPtr_h
#define WTF_RefPtr_h

#include "RawPtr.h"
#include "PassRefPtr.h"
#include <algorithm>

template<typename T> class RefPtr {
private:
	RefPtr(int);
	RefPtr &operator=(int);
public:
	RefPtr() : m_ptr(0) { }
	RefPtr(std::nullptr_t) : m_ptr(0) { }
	RefPtr(T* ptr) : m_ptr(ptr) { refIfNotNull(ptr); }
	template<typename U> RefPtr(const RawPtr<U>& ptr ) { refIfNotNull(m_ptr); }
	explicit RefPtr(T& ref) : m_ptr(&ref) { m_ptr->ref(); }
	RefPtr(const RefPtr& o) : m_ptr(o.m_ptr) { refIfNotNull(m_ptr); }
	template<typename U> RefPtr(const RefPtr<U>& o ) : m_ptr(o.get()) { refIfNotNull(m_ptr); }
	template<typename U> RefPtr(const PassRefPtr<U>&, EnsurePtrConvertibleArgDecl(U, T));

	bool isHashTableDeletedValue() const { return m_ptr == hashTableDeletedValue(); }

	~RefPtr() { derefIfNotNull(m_ptr); }

	T* get() const { return m_ptr; }

	void clear();
	PassRefPtr<T> release() { PassRefPtr<T> tmp = adoptRef(m_ptr); m_ptr = 0; return tmp; }

	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }

	bool operator!() const { return !m_ptr; }

	// This conversion operator allows implicit conversion to bool but not to other integer types.
	typedef T* (RefPtr::*UnspecifiedBoolType);
	operator UnspecifiedBoolType() const { return m_ptr ? &RefPtr::m_ptr : 0; }

	RefPtr& operator=(const RefPtr&);
	RefPtr& operator=(T*);
	RefPtr& operator=(const PassRefPtr<T>&);
	RefPtr& operator=(std::nullptr_t) { clear(); return *this; }

	template<typename U> RefPtr<T>& operator=(const RefPtr<U>&);
	template<typename U> RefPtr<T>& operator=(const PassRefPtr<U>&);
	template<typename U> RefPtr<T>& operator=(const RawPtr<U>&);

	void swap(RefPtr&);

	static T* hashTableDeletedValue() { return reinterpret_cast<T*>(-1); }

private:
	T* m_ptr;
};

template<typename T> template<typename U> inline RefPtr<T>::RefPtr(const PassRefPtr<U>& o, EnsurePtrConvertibleArgDefn(U, T))
	: m_ptr(o.leakRef())
{
}

template<typename T> inline void RefPtr<T>::clear()
{
	T* ptr = m_ptr;
	m_ptr = 0;
	derefIfNotNull(ptr);
}

template<typename T> inline RefPtr<T>& RefPtr<T>::operator=(const RefPtr& o)
{
	RefPtr ptr = o;
	swap(ptr);
	return *this;
}

template<typename T> template<typename U> inline RefPtr<T>& RefPtr<T>::operator=(const RefPtr<U>& o)
{
	RefPtr ptr = o;
	swap(ptr);
	return *this;
}

template<typename T> inline RefPtr<T>& RefPtr<T>::operator=(T* optr)
{
	RefPtr ptr = optr;
	swap(ptr);
	return *this;
}

template<typename T> inline RefPtr<T>& RefPtr<T>::operator=(const PassRefPtr<T>& o)
{
	RefPtr ptr = o;
	swap(ptr);
	return *this;
}

template<typename T> template<typename U> inline RefPtr<T>& RefPtr<T>::operator=(const PassRefPtr<U>& o)
{
	RefPtr ptr = o;
	swap(ptr);
	return *this;
}

template<typename T> template<typename U> inline RefPtr<T>& RefPtr<T>::operator=(const RawPtr<U>& o)
{
	RefPtr ptr = o.get();
	swap(ptr);
	return *this;
}

template<class T> inline void RefPtr<T>::swap(RefPtr& o)
{
	std::swap(m_ptr, o.m_ptr);
}

template<class T> inline void swap(RefPtr<T>& a, RefPtr<T>& b)
{
	a.swap(b);
}

template<typename T, typename U> inline bool operator==(const RefPtr<T>& a, const RefPtr<U>& b)
{
	return a.get() == b.get();
}

template<typename T, typename U> inline bool operator==(const RefPtr<T>& a, U* b)
{
	return a.get() == b;
}

template<typename T, typename U> inline bool operator==(T* a, const RefPtr<U>& b)
{
	return a == b.get();
}

template<typename T, typename U> inline bool operator!=(const RefPtr<T>& a, const RefPtr<U>& b)
{
	return a.get() != b.get();
}

template<typename T, typename U> inline bool operator!=(const RefPtr<T>& a, U* b)
{
	return a.get() != b;
}

template<typename T, typename U> inline bool operator!=(T* a, const RefPtr<U>& b)
{
	return a != b.get();
}

template<typename T, typename U> inline RefPtr<T> static_pointer_cast(const RefPtr<U>& p)
{
	return RefPtr<T>(static_cast<T*>(p.get()));
}

template<typename T> inline T* getPtr(const RefPtr<T>& p)
{
	return p.get();
}

template<typename T> class RefPtrValuePeeker {
public:
	RefPtrValuePeeker(T* p) : m_ptr(p) { }
	RefPtrValuePeeker(std::nullptr_t) : m_ptr(0) { }
	template<typename U> RefPtrValuePeeker(const RefPtr<U>& p) : m_ptr(p.get()) { }
	template<typename U> RefPtrValuePeeker(const PassRefPtr<U>& p) : m_ptr(p.get()) { }

	operator T*() const { return m_ptr; }
private:
	T* m_ptr;
};

#endif // WTF_RefPtr_h
