#ifndef ___BWGLUTIL_H__
#define ___BWGLUTIL_H__

#include <windows.h>
#include "gl/glew.h"
//#include <gl3\gl3.h>

struct ESMatrix
{
public:
	GLfloat   m[4][4];
};

extern GLuint		gProObject;
extern ESMatrix 	gOrthographic;
extern ESMatrix 	gMatrix;

extern GLint	gShaderPosition;
extern GLint	gShaderTexCoord;
extern GLint 	gShaderImgTexture;
extern GLint	gvpMatrix;

extern GLint 	gColorFlag;
extern GLint 	gColorLoc;

extern GLuint	gTextureTmp;
extern GLuint	gMainTexture;
extern GLuint	gFrameBuffer;
extern int		gWidth;
extern int		gHeight;


#ifdef __cplusplus
extern "C" {
#endif

	void GLMatrixIdentity(ESMatrix *result);
	void GLOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);
	void GLMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

	bool GLCreateHDC(HDC& hDC,int  stencil = 0);
	bool GLCreateHWND(HWND& hWnd,int stencil = 0);
	void GLDestroy();
	void GLMakeCurrent(bool disableOther = true);
	bool GLSetDCPixelFormat(HDC& hDC,int stencil);

	GLint CreateGLEnv( );
	GLuint GLLoadShader ( GLenum type,  const char *shaderSrc );
	void GLBindOffScreen();
	void GLEnvChanged(  int width, int height );
	void GLLoadImgDataTex( int width, int height, unsigned char * data );
	void GLSetDrawOffScreen(bool bOffScreen );
	void GLPaint( GLuint textureID );
	void GLFlush();
	void GLLoadImgDataTextuer( GLuint textureID, int width, int height, unsigned char * data );
	void GLPaintTexture( GLuint textureID,
		GLfloat left, GLfloat top, GLfloat right, GLfloat bottom,
		GLfloat ul, GLfloat vt, GLfloat ur, GLfloat vb );
	void GLDrawLine(GLfloat sx, GLfloat sy, GLfloat ex, GLfloat ey, int width, unsigned long nColor );
	void GLDrawRect(GLfloat left, GLfloat top, GLfloat right, GLfloat bottom, GLenum mode, unsigned long nColor );
	void GLDrawEllipse(GLfloat left, GLfloat top, GLfloat right, GLfloat bottom, GLenum mode, unsigned long nColor );

#ifdef __cplusplus
}
#endif


#endif //  ___BWGLUTIL_H__
