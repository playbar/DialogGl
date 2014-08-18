#ifndef __BwFILLSTYLE_H__
#define __BwFILLSTYLE_H__

#include <vector>
//#include "gl3.h"
#include <Windows.h>
//#include <gl\GL.h>
//#include "gl/glew.h"
#include "CCGL.h"

#include "cocos2d.h"
USING_NS_CC;


using namespace std;

class BwGradientRecord
{
public:
	BwGradientRecord();
	virtual ~BwGradientRecord();

	void GraRecColor( ccColor4F &color );
	void GraRecRatio( int ratio );

public:
	int	mRatio;
	ccColor4F mColor;
};

class FillStyle
{
public:
	FillStyle();
	virtual ~FillStyle();

public:
	virtual void BeginFillStyle(){};
	virtual void EndFillStyle(){};
	virtual void StylelMatrix(kmMat3 &mat ){};
	//virtual BwMatrix& StylelMatrix(){ return BwMatrix(); };

	virtual void StyleColor(ccColor4F &color ){};

	virtual void GradientType(int gt ){};
	virtual int GradientType(){ return 0; };

	virtual void SpreadMode( int sm ){};
	virtual int SpreadMode( ){ return 0; };

	virtual void InterpolationMode( int im ){};
	virtual int InterpolationMode(){ return 0; };

	virtual void AddGradientRec(BwGradientRecord &gradrec ){};

	virtual void BitmapID(int id){};
	virtual int BitmapID(){ return 0; };

	virtual void TextureID( int id){};
	virtual unsigned int TextureID( ){ return 0; };


};

class BitmapFill : public FillStyle
{
public:
	enum BwTypeFill
	{
		en_REPEATING_BITMAP_FILL = 0x40,
		en_CLIPPED_BITMAP_FILL = 0x41,
		en_NONSMOOTHED_REPEATING_BITMAP = 0x42,
		en_NONSMOOTHED_CLIPPED_BITMAP = 0x43
	};
	enum BwType 
	{
		en_CLIPPED,
		en_TILED
	};

public:
	BitmapFill();
	~BitmapFill();

	BitmapFill(BwType t, kmMat3 &m, BwTypeFill fill );

	void StylelMatrix(kmMat3 &mat );
	kmMat3 &StylelMatrix();

	void BitmapID(int id );
	int BitmapID();

public:
	virtual void BeginFillStyle();
	virtual void EndFillStyle();

public:
	BwType 		menType;
	BwTypeFill	menTypeFill;
	//BwGLDib 	*mBwGLDib;	//using smart point
	int mBitmapId;
	kmMat3	mMatrix;
};

class SolidFill : public FillStyle
{
public:
	SolidFill();
	~SolidFill();
	void StyleColor( ccColor4F &color );

	virtual void BeginFillStyle();
	virtual void EndFillStyle();

public:
	ccColor4F mColor;
};


class GradientFill : public FillStyle
{
	enum ENGradientType
	{
		en_LINEAR = 0x10,
		en_RADIAL = 0x12,
		en_FOCAL_RADIAL = 0x13
	};

	enum ENSpreadMode
	{
		en_SPREAD_PAD = 0,
		en_SPREAD_REPEAT = 1,
		en_SPREAD_REFLECT = 2,
		en_SPREAD_RESERVED = 3
	};

	enum ENInterpolationMode
	{
		en_INT_RGB = 0,
		en_INT_LINEAR_RGB = 1,
		en_INT_RESERVED = 2
	};

public:
	GradientFill();
	virtual ~GradientFill();
	void StylelMatrix( kmMat3 &mat );
	kmMat3 & StylelMatrix();

	void GradientType(int gt );
	int GradientType();

	void SpreadMode( int sm );
	int SpreadMode( );

	void InterpolationMode( int im );
	int InterpolationMode();

	void AddGradientRec(BwGradientRecord &gradrec );

	ccColor4F SampleGradient( int ratio );
	
public:
	virtual void BeginFillStyle();
	virtual void EndFillStyle();

	virtual void TextureID( int id);
	virtual unsigned int TextureID( );

protected:
	void CreateGradientTex();
	void setup();
	void upload();

public:
	bool mbCraeteTex;
	unsigned int mTexture;
	int mWidth;
	int mHeight;
	GLenum mOglImgType;
	unsigned char *mpData;
	ENInterpolationMode menInterpolationMode;
	ENSpreadMode		menSpeadMode;
	ENGradientType		menGradientType;
	vector< BwGradientRecord> mGradientRecords;
	float 				mFocalPoint;
	kmMat3			mMatrix;
};

class LineStyle
{
	enum ENJoinStyle
	{
		en_JOIN_ROUND = 0,
		en_JOIN_BEVEL = 1,
		en_JOIN_MITER = 2
	};

	enum ENCapStyle
	{
		en_CAP_ROUND = 0,
		en_CAP_NONE = 1,
		en_CAP_SQUARE = 2
	};

public:
	LineStyle();
	virtual ~LineStyle();

public:
	void LineWidth( int width );
	int LineWidth( );

	void LineColor( ccColor4F &color );
	//void LineColor( RGBI &color );
	ccColor4F& LineColor();
	void MiterLimitFactor( float miter );
	float MiterLimitFactor();

public:
	virtual void BeginLineStyle();
	virtual void EndLineStyle();

public:
	int mWidth;
	ccColor4F mColor;

	ENCapStyle menStartCapStyle;
	ENJoinStyle menJoinStyle;

	bool mbHasFillFlag;
	bool mbNoHScaleFlag;
	bool mbNoVScaleFlag;
	bool mbPixelHintingFlag;
	bool mbNoClose;
	ENCapStyle menEndCatStyl;

	float mfMiterLimitFactor;

};

#endif
