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

#ifndef __CCGEMETRY_H__
#define __CCGEMETRY_H__

#include <vector>
#include "CCObject.h"
#include "platformtype.h"

/**
 * @addtogroup data_structures
 * @{
 */

class CC_DLL CCPoint
{
public:
    float x;
    float y;

public:
    CCPoint();
    CCPoint(float x, float y);
    CCPoint(const CCPoint& other);
    CCPoint& operator= (const CCPoint& other);
    void setPoint(float x, float y);
    bool equals(const CCPoint& target) const;
};


class CCPointArray : public CCObject
{
public:

	/** creates and initializes a Points array with capacity */
	static CCPointArray* create(unsigned int capacity);

	virtual ~CCPointArray();
	CCPointArray();

	/** initializes a Catmull Rom config with a capacity hint */
	bool initWithCapacity(unsigned int capacity);

	/** appends a control point */
	void addControlPoint(CCPoint controlPoint);

	/** inserts a controlPoint at index */
	void insertControlPoint(CCPoint &controlPoint, unsigned int index);

	/** replaces an existing controlPoint at index */
	void replaceControlPoint(CCPoint &controlPoint, unsigned int index);

	/** get the value of a controlPoint at a given index */
	CCPoint getControlPointAtIndex(unsigned int index);

	/** deletes a control point at a given index */
	void removeControlPointAtIndex(unsigned int index);

	/** returns the number of objects of the control point array */
	unsigned int count();

	/** returns a new copy of the array reversed. User is responsible for releasing this copy */
	CCPointArray* reverse();

	/** reverse the current control point array inline, without generating a new one */
	void reverseInline();

	virtual CCObject* copyWithZone(CCZone *zone);

	const std::vector<CCPoint*>* getControlPoints();

	void setControlPoints(std::vector<CCPoint*> *controlPoints);
private:
	/** Array that contains the control points */
	std::vector<CCPoint*> *m_pControlPoints;
};


class CC_DLL CCSize
{
public:
    float width;
    float height;

public:
    CCSize();
    CCSize(float width, float height);
    CCSize(const CCSize& other);
    CCSize& operator= (const CCSize& other);
    void setSize(float width, float height);
    bool equals(const CCSize& target) const;
};

class CC_DLL CCRect
{
public:
    CCPoint origin;
    CCSize  size;

public:
    CCRect();    
    CCRect(float x, float y, float width, float height);
    CCRect(const CCRect& other);
    CCRect& operator= (const CCRect& other); 
    void setRect(float x, float y, float width, float height);
    float getMinX() const; /// return the leftmost x-value of current rect
    float getMidX() const; /// return the midpoint x-value of current rect
    float getMaxX() const; /// return the rightmost x-value of current rect
    float getMinY() const; /// return the bottommost y-value of current rect
    float getMidY() const; /// return the midpoint y-value of current rect
    float getMaxY() const; /// return the topmost y-value of current rect
    bool equals(const CCRect& rect) const;   
    bool containsPoint(const CCPoint& point) const;
    bool intersectsRect(const CCRect& rect) const;
};


#define CCPointMake(x, y) CCPoint((float)(x), (float)(y))
#define CCSizeMake(width, height) CCSize((float)(width), (float)(height))
#define CCRectMake(x, y, width, height) CCRect((float)(x), (float)(y), (float)(width), (float)(height))
#define ccp(__X__,__Y__) CCPointMake((float)(__X__), (float)(__Y__))

const CCPoint CCPointZero = CCPointMake(0,0);

/* The "zero" size -- equivalent to CCSizeMake(0, 0). */ 
const CCSize CCSizeZero = CCSizeMake(0,0);

/* The "zero" rectangle -- equivalent to CCRectMake(0, 0, 0, 0). */ 
const CCRect CCRectZero = CCRectMake(0,0,0,0);

// end of data_structure group
/// @}

/** Returns the Cardinal Spline position for a given set of control points, tension and time */
CCPoint ccCardinalSplineAt(CCPoint &p0, CCPoint &p1, CCPoint &p2, CCPoint &p3, float tension, float t);


#endif // __CCGEMETRY_H__
