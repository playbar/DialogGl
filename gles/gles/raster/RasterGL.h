/* Copyright (c) 2012 Scott Lembcke and Howling Moon Software
 * Copyright (c) 2012 cocos2d-x.org
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*
 * Code copied & pasted from SpacePatrol game https://github.com/slembcke/SpacePatrol
 *
 * Renamed and added some changes for cocos2d
 *
 */

#ifndef __RASTERGL_H__
#define __RASTERGL_H__

#include "CCNode.h"
#include "ccTypes.h"
#include "platformtype.h"
#include "map"
#include "vector"
using namespace std;

#include "tess.h"

/** CCDrawNode
 Node that draws dots, segments and polygons.
 Faster than the "drawing primitives" since they it draws everything in one single batch.
 
 @since v2.1
 */


enum Gradient_Type
{
	Gradient_Line = 0,
	Gradient_radius,
};

struct XGradient
{
	Gradient_Type gradientType;
	float xStart, yStart;
	float xEnd, yEnd;
	float radiusStart, radiusEnd;

	typedef map<float, ccColor4F> XColors;
	XColors mXColors;

	void addColorStop( float index, ccColor4F color );

};

enum REPEAT_PAT
{
	en_REPEAT = 0,
	en_REPEAT_X,
	en_REPEAT_Y,
	en_NO_REPEAT
};

struct XPattern
{
	REPEAT_PAT mRepeatePat;
	GLuint texId;
};


struct XFillStyle
{
	enum FILLTYPE
	{
		FILL_COLOR = 0,
		FILL_GRADIENT,
		FILL_PATTERN 
	};
	FILLTYPE mFillType;
	union
	{
		ccColor4F *mpColor;
		XGradient *mpGradient;
		XPattern *mpPattern;
	};
	//XFillStyle( XColor *color );
	//XFillStyle( XGradient *gradient );
	//XFillStyle( XPattern *pattern );
};

struct XStrokeStyle
{
	enum STROKEYPE
	{
		en_STROKE_COLOR = 0,
		en_STROKEGRADIENT,
		en_STROKEPATTERN 
	};
	STROKEYPE mFillType;
	union
	{
		ccColor4F *mpColor;
		XGradient *mpGradient;
		XPattern *mpPattern;
	};
};

enum LINECAP
{
	LINECAP_BUTT = 0,
	LINECAP_ROUND,
	LINECAP_SQUARE,
};

enum LINEJOIN
{
	LINEJOIN_BEVEL = 0,
	LINEJOIN_ROUND,
	LINEJOIN_MITER,
};

enum CMDType
{
	CTX_FILL = 0,
	CTX_BEGINPATH,
	CTX_CLOSEPATH,
	CTX_STROKE,
	CTX_MOVETO,
	CTX_LINETO,
	CTX_ARC,
	CTX_RECT,
	CTX_SAVE,
	CTX_RESTORE,
	CTX_QUADRATICCURVETO,
	CTX_FILLRECT,
	CTX_STROKERECT,
	CTX_CLEARRECT,
	CTX_CLIP,
	CTX_BEZIERCURVETO,
	CTX_ARCTO,
	CTX_MAX,
};

struct XCommand
{
	CMDType cmdType;
	union 
	{
		XStrokeStyle strokeStyle;
		XFillStyle   fillStyle;
	};

};

struct EgEdge
{
	float cpx;  //control point 
	float cpy;
	float endx;
	float endy;
	bool isLine;
	EgEdge *pNext;
};

struct EgPath
{
	float startx;
	float starty;

	EgEdge *pEdges;
	EgEdge *pCurEdge;
	EgPath *pNext;
	ccV2F_C4B_T2F *mBuffer;
	int count;

	union 
	{
		XStrokeStyle *strokeStyle;
		XFillStyle   *fillStyle;
	};

};


class CC_DLL XContext
{
protected:
    GLuint      m_uVbo;
    
    unsigned int    m_uBufferCapacity;
    GLsizei         m_nBufferCount;
    ccV2F_C4B_T2F   *m_pBuffer;
    
    ccBlendFunc     m_sBlendFunc;
    
    bool            m_bDirty;

public:
	float mLineWidth;
	XFillStyle mpFileStyle;
	XStrokeStyle mpStrokeStyle;
public:
	// p1
	void fill();
	void beginPath();
	void closePath();
	void stroke();
	void moveto( float x, float y );
	void lineto( float x, float y );
	void arc( float x, float y, float radius, float sAngle, float eAngle, bool counterclockwise );
	void rect( float x, float y, float width, float height );
	void save();
	void restore();

	// p2
	void quadraticCurveTo( float cpx, float cpy, float x, float y );
	//p3
	void fillRect( float x, float y, float width, float height );
	void strokeRect( float x, float y, float width, float height );
	void clearRect( float x, float y, float width, float height );
	void clip();
	void bezierCurveTo( float cp1x, float cp1y, float cp2x, float cp2y, float x, float y );
	void arcTo( float x1, float y1, float x2, float y2, float r );
	bool isPointInPath( float x, float y );

	void DrawCommand();

public:
	XGradient *CreateLinearGradient( float x1, float y1, float x2, float y2 );
	XPattern *CreatePattern( GLuint texId, REPEAT_PAT repat);
	XGradient *CreateRadialGradient( float xStart, float ySttart, float radiusStart, 
		float xEnd, float yEnd, float radiusEnd );
	void addColorStop( float index, ccColor4F color );


private:
	vector< XGradient *> mVecGradient;
	vector< XPattern *>mVecPattern;
	EgPath *mEgPaths;
	EgPath *pCurPath;
	float mcurx;
	float mcury;


public:
    static XContext* create();
    virtual ~XContext();
    
    virtual bool init();
    virtual void draw();
    
    /** draw a dot at a position, with a given radius and color */
    void drawDot(const CCPoint &pos, float radius, const ccColor4F &color);
    
    /** draw a segment with a radius and color */
    void drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color);
    
	/** draw a polygon with a fill color and line color */
	void drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor);

	void drawTriangle( const CCPoint &p1, const CCPoint &p2, const CCPoint &p3, const ccColor4F &color );
    
    /** Clear the geometry in the node's buffer. */
    void clear();
    
    ccBlendFunc getBlendFunc() const;
    void setBlendFunc(const ccBlendFunc &blendFunc);
    
    XContext();

	void loadShaders();

public:
	void beginPolygon();
	void endPolygon();
	void drawAllPolygon();

private:
	GLUtesselator *tobj;
    
private:
    void ensureCapacity(unsigned int count);
    void render();

private:
	CCGLProgram *mProgram;
};

#endif // __CCDRAWNODES_CCDRAW_NODE_H__
