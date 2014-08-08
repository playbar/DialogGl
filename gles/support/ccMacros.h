
#ifndef __CCMACROS_H__
#define __CCMACROS_H__


#define CC_SAFE_DELETE(p)            do { if(p) { delete (p); (p) = 0; } } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = 0; } } while(0)
#define CC_SAFE_FREE(p)                do { if(p) { free(p); (p) = 0; } } while(0)
#define CC_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define CC_SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = 0; } } while(0)
#define CC_SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)
#define CC_BREAK_IF(cond)            if(cond) break

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam
#define CCAssert(cond, msg)

#define CCRANDOM_0_1() ((float)rand()/RAND_MAX)

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);\
	void operator=(const TypeName&)

#define FLT_EPSILON     1.192092896e-07F        /* smallest such that 1.0+FLT_EPSILON != 1.0 */

#endif


