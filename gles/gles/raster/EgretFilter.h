
#ifndef __EGRETFILTER_H__
#define __EGRETFILTER_H__

#include "ccTypes.h"
#include "platformtype.h"
#include "EgFrameBuffer.h"
#include "ProgramData.h"

struct TextureInfo
{
	GLuint texId;
	int width;
	int height;
};


class CC_DLL EgretFilter
{
public:
	
	TextureInfo mTexInfo;
public:
	
	EgretFilter();
	static EgretFilter* create(int width, int height);
	virtual ~EgretFilter();
	void loadShaders(); 

	virtual bool init(int width, int height);
	void initTest();
	GLuint initTexData( const void *pData, int width, int height );
	unsigned char* DecodePngData(unsigned char* fData, long fSize, int& width, int& height);
	void beginPaint();
	void endPaint();
	void drawFrameBuffer();
	void dropShadowFilter();
	void dropShadowFilterTest();
	void dropGlowFilter();
	void DrawTexture(GLuint texId, float x, float y, float w, float h);
	void DrawFrameTexture(float x, float y, float width, float height);
    void clear();
    
    void ensureCapacity(unsigned int count);
    //void render();
protected:
	unsigned int    m_uBufferCapacity;
	GLsizei         m_nBufferCount;
	ccV3F_C4B_T2F   *m_pBuffer;
	float mWidth;
	float mHeight;
private:
	ProgramData mPrograme[enFilter_COUNT];
	EgFrameBuffer frameBufferA;
	EgFrameBuffer frameBufferB;
};

#endif // __CCDRAWNODES_CCDRAW_NODE_H__
