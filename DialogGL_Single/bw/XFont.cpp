
#include "XFont.h"
#include "math.h"
#include "XGraphics.h"

XFont::XFont()
{
	mFTLibrary = 0;
	mFTFace = 0;
	mFTHeight = 0;
	mFontStyle = 0x0;
	mFontColor = 0x88FF9900;
	mCharWidth = 0;
	mCharHeight = 0;
}

XFont::~XFont()
{
	//Free();
}

inline int next_p2 (int a )
{
	int rval=1;
	while(rval<a)
		rval<<=1;
	return rval;
}

void XFont::Init(const char * fname, unsigned int h)
{
	mFTHeight = h;
	//初始化FreeType库..
	if (FT_Init_FreeType( &mFTLibrary ))
		return;
	//加载一个字体,取默认的Face,一般为Regualer
	if (FT_New_Face( mFTLibrary, fname, 0, &mFTFace ))
		return;
	//大小要乘64.这是规定。照做就可以了。
	FT_Select_Charmap(mFTFace, FT_ENCODING_UNICODE);

	SetFontStyle();

//	FT_HAS_HORIZONTAL(mFTFace);
//	FT_Matrix matrix; /* transformation matrix */
//	FT_UInt glyph_index;
//	FT_Vector pen;
//	//给它设置个旋转矩阵
//	float angle = -5/180.0 * 3.14;
//	//float angle = 1.0f;
//	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
//	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
//	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
//	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
//	FT_Set_Transform( mFTFace, &matrix, &pen );

}

void XFont::SetFontStyle()
{
	FT_Set_Char_Size( mFTFace, mFTHeight<< 6, mFTHeight << 6, 96, 96);

	FT_Matrix matrix; /* transformation matrix */
	FT_UInt glyph_index;
	FT_Vector pen;
	float angle = 0;
	if( mFontStyle & XGraphics::XF_ITALIC )
	{
		angle = -10/180.0 * 3.14;
	}
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );
	FT_Set_Transform( mFTFace, &matrix, &pen );

	int strength = 1;
	if( mFontStyle & XGraphics::XF_STRIKE )
	{
		strength = 1 << 9;	//加粗
	}
	FT_Outline_Embolden(&mFTFace->glyph->outline, strength);

}

void XFont::SetFontColor( unsigned long color)
{
	mFontColor = color;
}

void XFont::Free()
{
	FT_Done_Face(mFTFace);
	FT_Done_FreeType(mFTLibrary);
	if( mTexId != 0 )
	{
		glDeleteTextures( 1, &mTexId );
		mTexId = 0;
	}
}

GLuint XFont::CharTexture(unsigned short ch)
{
	if( mTexId != 0 )
	{
		glDeleteTextures( 1, &mTexId );
		mTexId = 0;
	}
	if(FT_Load_Glyph( mFTFace, FT_Get_Char_Index( mFTFace, ch ), FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP ))
		return 0;

	SetFontStyle();
	//int strength = 1 << 9;	//加粗
	//FT_Outline_Embolden(&mFTFace->glyph->outline, strength);
	//得到字模
	FT_Glyph glyph;
	if(FT_Get_Glyph( mFTFace->glyph, &glyph ))
		return 0;
	//转化成位图
	FT_Render_Glyph( mFTFace->glyph, FT_RENDER_MODE_NORMAL );
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	//取道位图数据
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;
	//把位图数据拷贝自己定义的数据区里.这样旧可以画到需要的东西上面了。
	mCharWidth = bitmap.width;
	mCharHeight =  bitmap.rows;
	if( mFontStyle & XGraphics::XF_UNDERLINE )
	{
		mCharHeight =  bitmap.rows + 10;
	}
	int width  =  next_p2(mCharWidth);
	int height =  next_p2(mCharHeight);

	mU = (GLfloat)mCharWidth / (GLfloat)width;
	mV = (GLfloat)mCharHeight/ (GLfloat)height ;

	glGenTextures(1, &mTexId);
	glBindTexture(GL_TEXTURE_2D, mTexId);
	unsigned char* expanded_data = new unsigned char[ 3 * width * height];
	unsigned char * pcolor = (unsigned char *)&mFontColor;
	memset(expanded_data, 0, 3 * width * height );
	if( mFontStyle & XGraphics::XF_UNDERLINE )
	{
		//memset(expanded_data, 0xFF, 3 * 3 * width );
		for(int j=0; j <mCharHeight -10 ; j++)
		{
			for(int i=0; i < width; i++)
			{
				if( j < 3 )
					memcpy( &expanded_data[3* (width * j + i) ], pcolor, 3 );
				if( i < mCharWidth && bitmap.buffer[i + mCharWidth*j] )
					memcpy( &expanded_data[3*(i+(mCharHeight - j)*width)], pcolor, 3 );
			}
		}
	}
	else
	{
		for(int j=0; j < mCharHeight ; j++)
		{
			for(int i=0; i < width; i++)
			{
				if( i < mCharWidth && bitmap.buffer[i + mCharWidth*j] )
					memcpy( &expanded_data[3*(i+(mCharHeight - j)*width)], pcolor, 3 );
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				GL_UNSIGNED_BYTE, expanded_data);
	// Set the filtering mode
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	delete[]expanded_data;
	return mTexId;
}

int XFont::MeasureChar( unsigned short ch )
{
	if(FT_Load_Glyph( mFTFace, FT_Get_Char_Index( mFTFace, ch ), FT_LOAD_DEFAULT|FT_LOAD_NO_BITMAP ))
		return 0;

	SetFontStyle();
	//int strength = 1 << 9;	//加粗
	//FT_Outline_Embolden(&mFTFace->glyph->outline, strength);
	//得到字模
	FT_Glyph glyph;
	if(FT_Get_Glyph( mFTFace->glyph, &glyph ))
		return 0;
	//转化成位图
	FT_Render_Glyph( mFTFace->glyph, FT_RENDER_MODE_NORMAL );
	FT_Glyph_To_Bitmap( &glyph, ft_render_mode_normal, 0, 1 );
	FT_BitmapGlyph bitmap_glyph = (FT_BitmapGlyph)glyph;
	//取道位图数据
	FT_Bitmap& bitmap=bitmap_glyph->bitmap;
	mCharWidth = bitmap.width;
	mCharHeight =  bitmap.rows;
	return mCharWidth;
}



