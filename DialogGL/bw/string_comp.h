/*
 *  string_comp.h
 *  jqbar
 *
 *  Created by lining on 10-10-14.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _STRING_COMP_
#define _STRING_COMP_


#include <ctype.h>
#include <string.h> // zhengjl 2011.03


#if defined(__cplusplus) && __cplusplus
extern "C" {
#endif
	
	char *strupr(char *string);
	char *strlwr(char *string);
	unsigned short* gg_wcsupr(unsigned short* string);
	unsigned short* gg_wcslwr(unsigned short* string);
	size_t gg_wcslen(const unsigned short* string);
	int gg_wcsncmp(const unsigned short* lhs, const unsigned short* rhs, int len);
	int gg_wcscmp(const unsigned short* lhs, const unsigned short* rhs);	
	const unsigned short* gg_wcschr(const unsigned short* string, unsigned short c);
	
	int gg_strnicmp(const char* lhs, const char* rhs, int len);	
	int gg_stricmp(const char* lhs, const char* rhs);
	
	int gg_wcsnicmp(const unsigned short* lhs, const unsigned short* rhs, int len);	
	int gg_wcsicmp(const unsigned short* lhs, const unsigned short* rhs);
	
	
	unsigned short * gg_wcsstr (const unsigned short * wcs1, const unsigned short * wcs2);
	
	
#if defined(__cplusplus) && __cplusplus
}
#endif


#endif