#ifndef __RASTERE_GL__H__
#define __RASTERE_GL__H__

#include "BwShapeRecord.h"
//#include "gl3.h"
#include <Windows.h>
//#include <gl\GL.h>
//#include <gl\GLU.h>
#include <vector>
#include <map>
using namespace std;
#include "cocos2d.h"
USING_NS_CC;

/* Using Global
GLuint		gProObject;
ESMatrix 	gOrthographic;
GLint	gShaderPosition;
GLint	gShaderTexCoord;
GLint 	gShaderImgTexture;
GLint	gvpMatrix;

GLint 	gColorFlag;
GLint 	gColorLoc;

EGLSurface gSurface;
EGLDisplay gDisplay;
EGLContext gContext;
EGLContext gShareContext;
*/

#define GLUCALLBACKTYPE void (*)()

enum ENBlendMode 
{
	en_BLEND_MODE_NONE = 0,
	en_BLEND_MODE_ADD,
	en_BLEND_MODE_ALPHA,
	en_BLEND_MODE_DARKEN,
	en_BLEND_MODE_DIFFERENCE,
	en_BLEND_MODE_ERASE,
	en_BLEND_MODE_HARDLIGHT,
	en_BLEND_MODE_INVERT,
	en_BLEND_MODE_LAYER,
	en_BLEND_MODE_LIGHTEN,
	en_BLEND_MODE_OVERLAY,
	en_BLEND_MODE_MULTIPLY,
	en_BLEND_MODE_SCREEN,
	en_BLEND_MODE_SUBTRACT
};

enum ENBitmapFilter
{
	en_BTM_FILTER_NONE = 0,
	en_BTM_FILTER_BEVEL,
	en_BTM_FILTER_BLUR,
	en_BTM_FILTER_COLOR_MATRIX,
	en_BTM_FILTER_CONVOLUTION,
	en_BTM_FILTER_DROP_SHADOW,
	en_BTM_FILTER_GLOW,
	en_BTM_FILTER_GRADIENT_BEVEL,
	en_BTM_FILTER_GRADIENT_GLOW
};

struct ST9Patch
{
public:
	ST9Patch();
	~ST9Patch();

public:
	/*
	--mpt1----------mpt2---
	|					  |	
	mpt3				  mpt4
	|					  |
	mpt5				  mpt6
	|					  |
	---mpt7---------mpt8---
	*/

	CCPoint mpt1;
	CCPoint mpt2;
	CCPoint mpt3;
	CCPoint mpt4;
	CCPoint mpt5;
	CCPoint mpt6;
	CCPoint mpt7;
	CCPoint mpt8;
};

struct oglVertex 
{
	oglVertex(GLdouble x, GLdouble y, GLdouble z = 0.0): _x(x), _y(y), _z(z){}
	oglVertex(const CCPoint& p): _x(p.x), _y(p.y), _z(0.0){}

	GLdouble _x;
	GLdouble _y;
	GLdouble _z;
};

typedef vector<BwPath > PathVec;
typedef vector<BwPath*> PathPtrVec;
typedef map<const BwPath*, vector<oglVertex> > PathPointMap;

//class Tesselator
//{
//public:
//	Tesselator();  
//	~Tesselator();
//
//	void beginPolygon();
//
//	void feed(std::vector<oglVertex>& vertices);
//
//	void tesselate();
//
//	void beginContour();
//	void endContour();
//
//	static void error(GLenum error);
//
//	static void combine(GLdouble coords [3], void *vertex_data[4],
//		GLdouble weight[4], void **outData );
//private:
//	GLUtesselator* _tessobj;
//};

class RasterGL
{
public:
	RasterGL();
	~RasterGL();

public:
	void Init(int width, int height );
	void Uninit();
	void GetCacheData( unsigned char *pdata, int width, int height );

public:
	// color for soild file
	void GlesDrawShape( BwShapeRecord & rec, kmMat3 &matrix, ccColor4F &cx );

private:
	void ShowAllPt( BwShapeRecord & rec );
	void GlesDrawSubShape( BwPath &pPath, kmMat3 &matrix, ccColor4F &cx, vector<BwFillStyle*> &fillStyle,
		vector<BwLineStyle> &lineStyle );

	void AnalyzePath( vector<BwPath> &paths, bool &haveShape, bool &haveOutline );
	vector< vector<BwPath>::iterator > FindSubShape( vector<BwPath> &paths );
	
	void DrawSubShape( vector<BwPath>&paths, kmMat3 &mat, ccColor4F &cx, vector<BwFillStyle*>&fillStyle,
		vector<BwLineStyle>&lineStyle );
	void draw_outline( vector<BwPath>&path_vec, PathPointMap& pathpoints, kmMat3 &mat, ccColor4F &cx,
		 vector<BwLineStyle>&lineStyle);

	bool apply_line_style( BwLineStyle &style, kmMat3 &mat,ccColor4F &cx );

	vector<BwPath> NormalizePaths( vector<BwPath>&paths );
	BwPath ReversePath( BwPath &curPath );
	PathPointMap GetPathPoints( PathVec &path_vec );

public:
	void MaskBegin();
	void MaskAddPath( BwPath *path );	
	void MaskEnd();		//apply mask

	void EnableBlend( ENBlendMode mode );
	void DisableBlend();

	void EnalbeBitmapFilter( ENBitmapFilter filter );
	void DisableBitmapFilter();

	void Begin9Patch(ST9Patch * patch );
	void End9Patch();

public:
	void DrawOffScreen( bool bOff = false );
	void QualityAntiShift( int antiShift );
	int QualityAntiShift();
protected:
	int	 mAntiShift;
protected:

	//Tesselator _tesselator;

	unsigned int mTexture;		// init 
	int mWidth;
	int Height;

	vector< BwPath * > mVecMask;
	bool mbDrawMask;
	ENBlendMode mBlendMode;
	ENBitmapFilter mBitmapFilter;
	ST9Patch *mpSt9Patch;

public:
	CCRect edgeClip;

};

#endif
