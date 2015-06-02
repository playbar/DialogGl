
#ifndef __EGRETFILTER_H__
#define __EGRETFILTER_H__

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


struct EgEdge
{
	float cpx;  //control point 
	float cpy;
	//float endx;
	//float endy;
	GLdouble endxyz[3];
	bool isLine;
	EgEdge *pNext;
};

struct EgPath
{
public:	
	GLdouble xyz[3];

	EgEdge *pEdges;
	EgEdge *pCurEdge;
	int count; // edge count
	int pointCount; 

	EgPath *pNext;
	XPattern *strokeStyle;
	XPattern *fillStyle;
	float mLineWidth;

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


class CC_DLL EgretFilter
{
protected:
    GLuint      m_uVbo;
    unsigned int    m_uBufferCapacity;
    GLsizei         m_nBufferCount;
    ccV2F_C4B_T2F   *m_pBuffer;
    
    ccBlendFunc     m_sBlendFunc;
    
    bool            m_bDirty;

public:
	
	XPattern *mpFillStyle;
	XPattern *mpStrokeStyle;
	ccColor4B mbgcolor;
	int mWidth;
	int mHeight;
public:
	void fillRect( float x, float y, float width, float height );

private:
	
	EgPath *mEgPaths;
	EgPath *pCurPath;
	
	float mcurx;
	float mcury;


public:
    static EgretFilter* create();
    virtual ~EgretFilter();
    
    virtual bool init();
	void initTest();
	GLuint initTexData( const void *pData, int width, int height );
	void testDrawTex();
	unsigned char* DecodePngData(unsigned char* fData, long fSize, int& width, int& height);
	void testDrawTexWithMatixCoord();
	void drawFrameBuffer();
    
    void clear();
    
    ccBlendFunc getBlendFunc() const;
    void setBlendFunc(const ccBlendFunc &blendFunc);
    
    EgretFilter();

	void loadShaders();

public:
	void InitPolygon();
	void UninitPolygon();

private:
	GLUtesselator *tobj;
    
private:
    void ensureCapacity(unsigned int count);
    //void render();

private:
	CCGLProgram *mProgram;
};

#endif // __CCDRAWNODES_CCDRAW_NODE_H__
