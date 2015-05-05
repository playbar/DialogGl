
#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#if defined(_USRDLL)
	#define CC_DLL     __declspec(dllexport)
#else         /* use a DLL library */
	#define CC_DLL     __declspec(dllimport)
#endif


#endif
