/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/


#include "CCObject.h"
//#include "CCAutoreleasePool.h"
#include "ccMacros.h"
//#include "script_support/CCScriptSupport.h"


CCZone::CCZone(CCObject *pObject)
{
	m_pCopyObject = pObject;
}

CCObject* CCCopying::copyWithZone(CCZone *pZone)
{
    CC_UNUSED_PARAM(pZone);
    return 0;
}

CCObject::CCObject(void)
: m_nLuaID(0)
, m_uReference(1) // when the object is created, the reference count of it is 1
, m_uAutoReleaseCount(0)
{
    static unsigned int uObjectCount = 0;

    m_uID = ++uObjectCount;
}

CCObject::~CCObject(void)
{
 
}

CCObject* CCObject::copy()
{
    return copyWithZone(0);
}

void CCObject::release(void)
{
    --m_uReference;

    if (m_uReference == 0)
    {
        delete this;
    }
}

void CCObject::retain(void)
{
    ++m_uReference;
}

CCObject* CCObject::autorelease(void)
{
    return this;
}

bool CCObject::isSingleReference(void)
{
    return m_uReference == 1;
}

unsigned int CCObject::retainCount(void)
{
    return m_uReference;
}

bool CCObject::isEqual(const CCObject *pObject)
{
    return this == pObject;
}

