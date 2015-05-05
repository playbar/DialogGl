#ifndef _SMART_POINT_H_
#define _SMART_POINT_H_

/*
����ָ�루����ʹ�ã�������棬ģ��ʵ�֣�?
@Author: chishaxie
*/

//ΪDebug׼���Ķ��ԣ�Release�治��Ӱ��Ч�ʣ�
#ifdef _DEBUG
#include<assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif


template<typename T>
class SmartPtr
{
public:
	
	SmartPtr() :pc( 0 ) 
	{
		count = new int;
		*count = 0;
	}
	
	SmartPtr(T* point)
	{
		pc = point;
		count = new int;
		*count = 1;
	}
	
	~SmartPtr()
	{
		if( --count == 0 )
		{
			delete pc;
			pc = NULL;
		}
	}

	SmartPtr& operator=(SmartPtr<T> &pdata )
	{
		if ( pc != pdata.pc )
		{
			if ( *count == 1 )
			{
				delete pc;
			}
			pc = pdata.pc;
		}
		(*count)++;
		return *this;
	}

	SmartPtr& operator=( T* pdata )
	{
		if( pc != pdata )
		{	
			if ( (*count) == 1 )
			{
				delete pc;
			}
			pc = pdata;
		}
		(*count) ++;
		return *this;
	}
	
	T* operator->()
	{
		return pc;
	}
	
	T& operator*()
	{
		return *pc;
	}
	T * GetData()
	{
		return pc;
	}
	void addref()
	{
		(*count)++;
	}

	void release()
	{
		if ( --(*count) == 0 )
		{
			delete pc;
			delete count;
		}
	}
private:
	T* pc;
	int *count;
};

#endif
