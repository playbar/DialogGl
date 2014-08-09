#ifndef __XFONT_H__
#define __XFONT_H__

#include <gl/gl.h>
#include "ft2build.h"

#include FT_FREETYPE_H
#include "freetype/freetype.h"
#include "freetype/ftglyph.h"
#include "freetype/ftoutln.h"

class XFont
{
public:
	XFont();
	~XFont();
	void Init(const char * fname, unsigned int h);
	void SetFontStyle();
	void SetFontColor(unsigned long color);
	void Free();
	GLuint CharTexture(unsigned short ch);
	int MeasureChar( unsigned short ch );

	GLuint  mTexId;
	GLfloat mU;
	GLfloat mV;
	int 	mCharWidth;
	int 	mCharHeight;
	unsigned long mFontStyle;
	unsigned long mFontColor;

private:
	FT_Library mFTLibrary;
	FT_Face mFTFace;
	int mFTHeight;

};


#endif
