
#ifndef __EGRETFILTER_H__
#define __EGRETFILTER_H__

#include "CCNode.h"
#include "ccTypes.h"
#include "platformtype.h"
#include "vector"
using namespace std;

enum PROGRAMRTYPE
{
	enFilter_BLURH = 0,
	enFilter_BLURR,
	enFilter_COLOR,
	enFilter_ALPHA,
	enFilter_MULTIPLY,
	enFilter_IDENTITY,
	enFilter_COUNT
};

enum
{
	enUni_resolution = 0,
	enUni_flipY,
	enUni_time,
	enUni_transformMatrix,
	enUni_image,
	enUni_color,
	enUni_textureSize,
	enUni_colorMatrix,
	enUni_vector,
	enUni_Count,
};

#define	enUni_resolution_s		"u_resolution"
#define enUni_flipY_s			"u_flipY" 
#define enUni_time_s			"u_time"
#define enUni_transformMatrix_s	"u_transformMatrix"
#define enUni_image_s			"u_image"
#define enUni_color_s			"u_color"
#define enUni_textureSize_s		"u_textureSize"
#define enUni_colorMatrix_s		"u_colorMatrix"
#define enUni_vector_s			"u_vector"

enum
{
	enAtt_position,
	enAtt_textureCoordinate,
	enAtt_Count,
};

#define enAtt_position_s			"a_position"
#define enAtt_textureCoordinate_s	"a_textureCoordinate"

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
public:
	void fillRect( float x, float y, float width, float height );

public:
	EgretFilter();
	void loadShaders();
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
    
private:
    void ensureCapacity(unsigned int count);
    //void render();
private:
	CCGLProgram *mProgram;
};

#endif // __CCDRAWNODES_CCDRAW_NODE_H__
