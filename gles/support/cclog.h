#ifndef __CCLOG_H__
#define __CCLOG_H__

void CCLog(const char * pszFormat, ...);

#define CCLOG(format, ...)      CCLog(format, ##__VA_ARGS__)

#endif

