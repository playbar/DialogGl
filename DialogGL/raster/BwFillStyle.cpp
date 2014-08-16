#include "BwFillStyle.h"
//#include "BwGLUtility.h"
#include "math.h"

BwFillStyle::BwFillStyle()
{

}

BwFillStyle::~BwFillStyle()
{

}


BwBitmapFill::BwBitmapFill()
{

}

BwBitmapFill::BwBitmapFill(BwType t, kmMat3 &m, BwTypeFill fill )
{

}

BwBitmapFill::~BwBitmapFill()
{

}

void BwBitmapFill::StylelMatrix(kmMat3 &mat )
{
	mMatrix = mat;
	//mMatrix = mat;
}

kmMat3 & BwBitmapFill::StylelMatrix()
{
	return mMatrix;
}

void BwBitmapFill::BitmapID(int id)
{
	mBitmapId = id;
}

int BwBitmapFill::BitmapID()
{
	return mBitmapId; 
};

void BwBitmapFill::BeginFillStyle()
{

}

void BwBitmapFill::EndFillStyle()
{

}

BwSolidFill::BwSolidFill()
{

}

BwSolidFill::~BwSolidFill()
{

}

void BwSolidFill::StyleColor( ccColor4F &color )
{
	mColor.a = color.a;
	mColor.r = color.r;
	mColor.b = color.b;
	mColor.g = color.g;
	return;
}

void BwSolidFill::BeginFillStyle()
{
	glColor4ub( mColor.b, mColor.g, mColor.r, mColor.a );
	return;
}

void BwSolidFill::EndFillStyle()
{

}

BwGradientRecord::BwGradientRecord()
{

}

BwGradientRecord::~BwGradientRecord()
{

}

void BwGradientRecord::GraRecColor( ccColor4F &color )
{
	mColor.a = color.a;
	mColor.r = color.r;
	mColor.b = color.b;
	mColor.g = color.g;
	return;
}

void BwGradientRecord::GraRecRatio( int ratio )
{
	mRatio = ratio;
}

BwGradientFill::BwGradientFill()
{
	glGenTextures(1, &mTexture );
	mbCraeteTex = false;
	mpData = NULL;
	mFocalPoint = 0;
	mWidth = 0;
	mHeight = 0;
}

BwGradientFill::~BwGradientFill()
{
	glDeleteTextures( 1, &mTexture );
	mTexture = 0;
	mbCraeteTex = false;
	if( mpData != NULL )
	{
		delete []mpData;
		mpData = NULL;
	}
	glDisable( mOglImgType );
}

void BwGradientFill::StylelMatrix( kmMat3 &mat )
{
	mMatrix = mat;
}

kmMat3 & BwGradientFill::StylelMatrix()
{
	return mMatrix;
}

void BwGradientFill::GradientType(int gt )
{
	menGradientType = (ENGradientType)gt;
}

int BwGradientFill::GradientType()
{
	return menGradientType;
}

void BwGradientFill::SpreadMode( int sm )
{
	menSpeadMode = (ENSpreadMode)sm;
}

int BwGradientFill::SpreadMode( )
{
	return menSpeadMode;
}

void BwGradientFill::InterpolationMode( int im )
{
	menInterpolationMode = (ENInterpolationMode)im;
}

int BwGradientFill::InterpolationMode()
{
	return menInterpolationMode;
}

void BwGradientFill::AddGradientRec(BwGradientRecord &gradrec )
{
	mGradientRecords.push_back( gradrec );
	return;
}

ccColor4F BwGradientFill::SampleGradient( int ratio )
{
	ccColor4F color;
	int cou = mGradientRecords.size();
	if( ratio < mGradientRecords[0].mRatio )
		return mGradientRecords[0].mColor;
	if( ratio >= mGradientRecords[ cou - 1 ].mRatio )
		return mGradientRecords[cou-1].mColor;

	for( int i = 1; i < cou; ++i )
	{
		BwGradientRecord &gr1 = mGradientRecords[i];
		if( gr1.mRatio < ratio )
			continue;
		BwGradientRecord &gr0 = mGradientRecords[i - 1 ];
		if( gr0.mRatio > ratio )
			continue;

		if( gr0.mRatio != gr1.mRatio )
		{
			int w0, w1, wT;
			w1 = gr1.mRatio - ratio;
			w0 = ratio - gr0.mRatio;
			wT = w1 + w0;
			color.a = ((w0 * gr0.mColor.a + w1 * gr1.mColor.a ) / wT );
			color.r = ((w0 * gr0.mColor.r + w1 * gr1.mColor.r ) / wT );
			color.g = ((w0 * gr0.mColor.g + w1 * gr1.mColor.g ) / wT );
			color.b = ((w0 * gr0.mColor.b + w1 * gr1.mColor.b ) / wT );
		}
		else
		{
			color = gr1.mColor;
		}
	}
	return color;

}

void BwGradientFill::CreateGradientTex()
{
	if( menGradientType == en_LINEAR )
	{
		mOglImgType = GL_TEXTURE_1D;
		if( mpData != NULL )
		{
			delete []mpData;
			mpData = NULL;
		}
		mWidth = 256;
		mHeight = 1;
		mpData = new unsigned char[ 256 * 4 ];
		memset( mpData, 0, 256 * 4 );
		unsigned char* pTmp = mpData;
		ccColor4F c1, c2;
		ccColor4F color;
		int cr1, cr2;
		int w1, w2, wT;
		cr1 = 0;
		cr2 = mGradientRecords[0].mRatio;
		c1 = c2 = mGradientRecords[0].mColor;
		int ci = 1;
		for( int i = 0; i < 256; i++ )
		{
			if( i > cr2 )
			{
				cr1 = cr2;
				c1 = c2;
				if( ci < mGradientRecords.size() )
				{
					cr2 = mGradientRecords[ci].mRatio;
					c2 = mGradientRecords[ci].mColor;
					ci++;
				}
				else
				{
					cr2 = 256;
				}
			}
			w1 = cr2 - i;
			w2 = i - cr1;
			wT = cr2 - cr1;
			if( wT > 0 )
			{
				color.r = ((w1 * c1.r + w2 * c2.r ) / wT );
				color.g = ((w1 * c1.g + w2 * c2.g ) / wT );
				color.b = ((w1 * c1.b + w2 * c2.b ) / wT );
				color.a = ((w1 * c1.a + w2 * c2.a ) / wT );
			}
			else
			{
				color = c1;
			}
			
			memcpy(pTmp, (unsigned char*)&color, 4 );
			pTmp += 4;
		}
		pTmp = NULL;

		//XDrawDib dib;
		//dib.Create( 256, 1, 32 );
		//memcpy( dib.GetBuffer(), mpData, 256 * 4 );
		//dib.SaveFile("D:\\pic\\test.bmp");

		return;
	}
	else if( menGradientType == en_RADIAL )
	{
		mOglImgType = GL_TEXTURE_2D;
		if( mpData != NULL )
		{
			delete []mpData;
			mpData = NULL;
		}
		mWidth = 64;
		mHeight = 64;
		mpData = new unsigned char[ 64 * 64 * 4 ];
		memset( mpData, 0, 64 * 64 * 4 );
		unsigned char *pTmp = mpData;
		for( int j = 0; j < 64; j++ )
		{
			for( int i = 0; i < 64; i++ )
			{
				float radiusy = 32.0f;
				float radiusx = radiusy + abs( radiusy * mFocalPoint );
				float y = ( j - radiusy ) / radiusy;
				float x = ( i - radiusx ) / radiusx;
				int ratio = floor( 255.5f * sqrt( x*x + y* y ) );
				if( ratio > 255 )
					ratio = 255;
				ccColor4F color = SampleGradient( ratio );
				memcpy(pTmp, (unsigned char*)&color, 4 );
				pTmp += 4;
			}
		} //end of j
		//XDrawDib dib;
		//dib.Create( 64, 64, 32 );
		//memcpy( dib.GetBuffer(), mpData, 64 * 64 * 4 );
		//dib.SaveFile("D:\\pic\\test.bmp");
		//dib.GetAddress() = mpData;
		return;
	}
	return;
}

void BwGradientFill::setup()
{
	glEnable( mOglImgType );

	glBindTexture( mOglImgType, mTexture );
	upload();
	glDisable( mOglImgType );
	return;
}

void BwGradientFill::upload()
{
	glTexParameteri( mOglImgType, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	if( mOglImgType == GL_TEXTURE_1D )
	{
		glTexImage1D( GL_TEXTURE_1D, 0, GL_RGBA, mWidth, 0, GL_RGBA, GL_UNSIGNED_BYTE, mpData );
	}
	else
	{
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mpData );
	}

	if( mpData != NULL )
	{
		delete []mpData;
		mpData = NULL;
	}

	return;
}

void BwGradientFill::BeginFillStyle()
{
	glEnable( mOglImgType );
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );
	if( ! mbCraeteTex )
	{
		CreateGradientTex();
		setup();
		mbCraeteTex = true;
	}

	glEnable( mOglImgType );
	glEnable( GL_TEXTURE_GEN_S );
	glEnable( GL_TEXTURE_GEN_T );

	glBindTexture( mOglImgType, mTexture );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,  GL_REPLACE );
	
	glTexParameteri( mOglImgType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( mOglImgType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

	//if( mOglImgType == GL_TEXTURE_1D )
	//{
	//	float sx = FixedToFloat( mMatrix.a );
	//	float w = sx * 1638.4;
	//	float sy = FixedToFloat( mMatrix.d );
	//	float tx = FixedToFloat( mMatrix.tx );
	//	float ty = FixedToFloat( mMatrix.ty );
	//	float h = sy * 1638.4;
	//	float inv_width = 1.0 / w;
	//	float inv_height = 1.0/ h;
	//	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	//	float p[4] = { 0,0,0,0 };
	//	p[0] = inv_width;// mMatrix.a / 65536.0f * inv_width;
	//	p[3] = inv_width * tx;

	//	//p[1] = 0.01f;// mMatrix.c / 65536.0f * inv_width;
	//	//p[2] = 0.0f;
	//	//p[3] = 0.1f; // mMatrix.tx * inv_width;
	//	//p[0] = mMatrix.a / 65536.0f * inv_width;
	//	//p[1] = mMatrix.c / 65536.0f * inv_width;
	//	//p[2] = 0.0f;
	//	//p[3] = mMatrix.tx * inv_width;
	//	glTexGenfv( GL_S, GL_OBJECT_PLANE, p );
	//	//glTexGeni(GL_S,GL_TEXTURE_GEN_MODE ,GL_OBJECT_LINEAR); 

	//	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	//	p[0] = 0;
	//	p[1] = inv_height;
	//	p[2] = 0;
	//	p[3] = inv_height * ty;
	//	//p[0] = mMatrix.b / 65536.0f * inv_height;
	//	//p[1] = mMatrix.c / 65536.0f * inv_height;
	//	//p[2] = 0.0f;
	//	//p[3] = mMatrix.ty * inv_height;
	//	glTexGenfv( GL_T, GL_OBJECT_PLANE, p );
	//}
	//else
	//{
	//	float sx = FixedToFloat( mMatrix.a );
	//	float wd2 = sx * 819.2;
	//	float sy = FixedToFloat( mMatrix.d );
	//	float tx = FixedToFloat( mMatrix.tx );
	//	float ty = FixedToFloat( mMatrix.ty );
	//	float hd2 = sy * 819.2;
	//	float w = wd2 * 2;
	//	float h = hd2 * 2;
	//	float inv_width = 1.0 / w;
	//	float inv_height = 1.0/ h;
	//	glTexGeni( GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	//	float p[4] = { 0,0,0,0 };
	//	p[0] = inv_width;
	//	p[3] = inv_width * tx;
	//	glTexGenfv( GL_S, GL_OBJECT_PLANE, p );

	//	glTexGeni( GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR );
	//	p[0] = 0;
	//	p[1] = inv_height;
	//	p[2] = 0;
	//	p[3] = inv_height - ty;
	//	glTexGenfv( GL_T, GL_OBJECT_PLANE, p );
	//}
	return;
}

void BwGradientFill::EndFillStyle()
{

}

void BwGradientFill::TextureID( int id)
{
	mTexture = id;
}
GLuint BwGradientFill::TextureID( )
{
	return mTexture;
}


BwLineStyle::BwLineStyle()
{

}

BwLineStyle::~BwLineStyle()
{

}

void BwLineStyle::LineWidth( int width )
{
	mWidth = width;
}
int BwLineStyle::LineWidth( )
{
	return mWidth;
}

void BwLineStyle::LineColor( ccColor4F &color )
{
	mColor = color;
}

ccColor4F& BwLineStyle::LineColor()
{
	return mColor;
}


void BwLineStyle::MiterLimitFactor( float miter )
{
	mfMiterLimitFactor = miter;
}

float BwLineStyle::MiterLimitFactor( )
{
	return mfMiterLimitFactor;
}

void BwLineStyle::BeginLineStyle()
{
	// b, r »»Î»
	//mColor.nColor = 0xFF00FF00;
	//glUseProgram ( gProObject );
	//glUniform1i ( gColorFlag, 1 );
	//float src_a = mColor.a / 255.0f;
	//float src_b = mColor.r / 255.0f;
	//float src_g = mColor.g / 255.0f;
	//float src_r = mColor.b / 255.0f;
	//float sc[4] = { src_r, src_g, src_b, src_a };
	//glUniform4fv(gColorLoc, 1, sc );
	//glLineWidth( mWidth );
	return;
}

void BwLineStyle::EndLineStyle()
{
	//glLineWidth( 1 );
	//glUniform1i( gColorFlag, 0 );
	return;
}

