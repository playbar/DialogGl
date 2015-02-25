
#ifndef __RASTERGL_H__
#define __RASTERGL_H__

#include "CCNode.h"
#include "ccTypes.h"
#include "platformtype.h"
#include "map"
#include "vector"
using namespace std;

#include "tess.h"

// 着色器填充方式
enum CTX_FILLTYPE
{
	FILL_NONE = 0,
	FILL_COLOR = 1,
	FILL_Gradient_Line = 2,
	FILL_Gradient_radius = 3,
	FILL_PATTERN = 4,	
	FILL_TEST = 5,
};

struct GradientData
{
	float index;
	ccColor4F color;
	GradientData *pNext;
};

struct CC_DLL XGradientLinear
{
	GLuint texId;
	int miLen;
	float x;
	float angle;
	GradientData *pGraData;
	bool mbDirty;

	void addColorStop( float index, ccColor4F color );
	void CreateTextrue();

};

struct CC_DLL XGradientRadial
{
	GLuint texId;
	int miLen;
	float x;
	float y;
	GradientData *pGraData;
	bool mbDirty;

	void addColorStop( float index, ccColor4F color );
	void CreateTextrue();

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
	int width;
	int height;
};

struct CC_DLL XFillStyle
{
	CTX_FILLTYPE mFillType;
	union
	{
		ccColor4F mColor;
		XGradientLinear *mpGradientLinear;
		XGradientRadial *mpGradientRadial;
		XPattern *mpPattern;
	};
	XFillStyle();
	XFillStyle( ccColor4F color );
	XFillStyle( XGradientLinear *gradient );
	XFillStyle( XGradientRadial *gradient );
	XFillStyle( XPattern *pattern );
	void setFillType( ccColor4F  color );
	void setFillType( XGradientLinear *gradient );
	void setFillType( XGradientRadial *gradient );
	void setFillType( XPattern *pattern );

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
		XGradientLinear *mpGradientLinear;
		XGradientRadial *mpGradientRadial;
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
public:	
	float startx;
	float starty;

	EgEdge *pEdges;
	EgEdge *pEndEdge;

	EgPath *pNext;
	CMDType cmdType;
	int count;
	XStrokeStyle *strokeStyle;
	XFillStyle   *fillStyle;

public:
	bool mbDirty;
	GLuint muVbo;  // 数组空间
	int mbufferLen;	//数组空间大小
	int mCurIndex;	//数组当前使用的位置

	void GenBuffer();
	void BindBuffer();
	void BufferData(GLsizeiptr size, const GLvoid *data );
	void BufferSubData(GLuint offset,GLsizeiptr size, const GLvoid *data );
	void DeleteBuffer();
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
	XFillStyle *mpFillStyle;
	XStrokeStyle *mpStrokeStyle;
	ccColor4B mbgcolor;
	int mWidth;
	int mHeight;
public:
	// p1
	void fill();
	void beginPath();
	void closePath();
	void stroke();
	void moveto( float x, float y );
	void lineto( float x, float y );
	void arc( float x, float y, float radius, float sAngle, float eAngle, bool counterclockwise = true );
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
	XGradientLinear *CreateLinearGradient( float x1, float y1, float x2, float y2 );
	XPattern *CreatePattern( GLuint texId, REPEAT_PAT repat);
	XGradientRadial *CreateRadialGradient( float xStart, float ySttart, float radiusStart, 
		float xEnd, float yEnd, float radiusEnd );

private:
	//vector< XGradientLinear *> mVecGradient;
	//vector< XPattern *>mVecPattern;
	EgPath *mEgPaths;
	EgPath *pEndPath;
	EgPath *pCurPath;
	float mcurx;
	float mcury;


public:
    static XContext* create();
    virtual ~XContext();
    
    virtual bool init();
	void initTest();
	GLuint initTexData( const void *pData, int width, int height );
	void testDrawTex();
	unsigned char* DecodePngData(unsigned char* fData, long fSize, int& width, int& height);
	void testDrawTexWithMatixCoord();
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
