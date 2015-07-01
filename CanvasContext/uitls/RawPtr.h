
#ifndef WTF_RawPtr_h
#define WTF_RawPtr_h

#include <algorithm>

template<typename T>
class RawPtr {
private:
	RawPtr(int);
	RawPtr & operator=(int);
public:
	RawPtr() : m_ptr(0) { }
	RawPtr(std::nullptr_t) : m_ptr(0) { }
	RawPtr(T* ptr) : m_ptr(ptr) { }
	explicit RawPtr(T& reference) : m_ptr(&reference) { }
	RawPtr(const RawPtr& other)
		: m_ptr(other.get())
	{
	}

	template<typename U>
	RawPtr(const RawPtr<U>& other)
		: m_ptr(other.get())
	{
	}

	// Hash table deleted values, which are only constructed and never copied or destroyed.
	//RawPtr(int ival ) : m_ptr(hashTableDeletedValue()) { }
	bool isHashTableDeletedValue() const { return m_ptr == hashTableDeletedValue(); }

	T* get() const { return m_ptr; }
	void clear() { m_ptr = 0; }
	RawPtr<T> release()
	{
		RawPtr<T> tmp = m_ptr;
		m_ptr = 0;
		return tmp;
	}
	T* leakRef()
	{
		T* ptr = m_ptr;
		m_ptr = 0;
		return ptr;
	}

	template<typename U>
	RawPtr& operator=(U* ptr)
	{
		m_ptr = ptr;
		return *this;
	}

	template<typename U>
	RawPtr& operator=(RawPtr<U> ptr)
	{
		m_ptr = ptr.get();
		return *this;
	}

	RawPtr& operator=(std::nullptr_t)
	{
		m_ptr = 0;
		return *this;
	}

	operator T*() const { return m_ptr; }
	T& operator*() const { return *m_ptr; }
	T* operator->() const { return m_ptr; }
	bool operator!() const { return !m_ptr; }

	void swap(RawPtr& o)
	{
		std::swap(m_ptr, o.m_ptr);
	}

	static T* hashTableDeletedValue() { return reinterpret_cast<T*>(-1); }

private:
	T* m_ptr;
};

template<typename T, typename U> inline RawPtr<T> static_pointer_cast(const RawPtr<U>& p)
{
	return RawPtr<T>(static_cast<T*>(p.get()));
}

template<typename T> inline T* getPtr(const RawPtr<T>& p)
{
	return p.get();
}

#endif
