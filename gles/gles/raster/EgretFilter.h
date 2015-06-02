
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

struct XPattern
{
	GLuint texId;
	int width;
	int height;
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

public:
    static EgretFilter* create();
    virtual ~EgretFilter();
    
    virtual bool init();
	void initTest();
	GLuint initTexData( const void *pData, int width, int height );
	unsigned char* DecodePngData(unsigned char* fData, long fSize, int& width, int& height);
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
