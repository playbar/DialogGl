#ifndef __DEFINE_H__
#define __DEFINE_H__


typedef char  XTCHAR;

typedef unsigned char	XU8;
typedef unsigned short	XU16;
typedef unsigned long	XU32;
typedef signed char		XS8;
typedef signed short	XS16;
typedef signed long		XS32;
typedef __int64			XU64;
typedef __int64			XS64;

typedef float			XF32;
typedef double			XF64;
typedef signed int		XSINT;
typedef unsigned int	XUINT;
typedef short			XSHORT;
typedef int				XINT;
typedef long			XLONG;
typedef void			XANY;
typedef void*		    XPANY;
typedef int				XBOOL;

typedef	unsigned short 		  XWCHAR;
typedef const unsigned short *XCWSTR;
typedef const unsigned short *XPCWSTR;
typedef const char			 *XCTSTR;
typedef const char			 *XPCTSTR;
typedef char	*XPTSTR;
typedef XWCHAR*  XPWSTR;

typedef void  XVOID,*XPVOID;
typedef const void* XPCVOID;

typedef void*	XHANDLE;
typedef XUINT	XSOCKET;


#define XK_LEFT		0x25
#define XK_RIGHT	0x27
#define XK_UP		0x26
#define XK_DOWN		0x28
#define XINFINITE       0xFFFFFFFF
#define XNULL			0
#define XTRUE			1
#define XFALSE			0


#endif
