
#include "EgretFilter.h"
#include "gl/matrix.h"
#include "gl/glew.h"
#include "windows.h"
#include "png.h"
//////////////////////////////////////////////////////////////////////////

static const char vert_canvas[] =
"attribute vec3 a_position;"
"attribute vec2 a_textureCoordinate;"
"varying vec2 v_texCoord;"
"void main() {"
//"	vec2 position = ((u_transformMatrix * vec4(a_position, 1.0)).xy / u_resolution) * 2.0 - 1.0;"
//"	position *= vec2(1.0, u_flipY);"
//"	gl_Position = vec4(vec3(position, 1.0), 1.0);"
//"	vec2 position = ( vec4(a_position, 1.0).xy / u_resolution) * 2.0 - 1.0;"
//"	position *=vec2(1.0, -u_flipY);"
//"	gl_Position = vec4(position.xy, 0.0, 1.0);"
"	gl_Position = u_transformMatrix * vec4(a_position, 1.0);"
"	v_texCoord = a_textureCoordinate;"
"}";


static const char frag_alpha[] =
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0 ) * texture2D(u_image, v_texCoord).a;"
"}";

static const char frag_blurh[] =
"varying vec2 v_texCoord;"
"void main() {"
"	const int sampleRadius = 5;"
"	const int samples = sampleRadius * 2 + 1;"
"	vec2 one = vec2(1.0, 1.0) / u_textureSize;"
"	vec4 color = vec4(0, 0, 0, 0);"
"	for (int i = -sampleRadius; i <= sampleRadius; i++) {"
"		color += texture2D(u_image, v_texCoord + vec2(float(i) * one.x, 0));"
"	}"
"	color /= float(samples / 2);"
"	gl_FragColor = color;"
"}";

static const char frag_blurv[] =
"varying vec2 v_texCoord;"
"void main() {"
"	const int sampleRadius = 5;"
"	const int samples = sampleRadius * 2 + 1;"
"	vec2 one = vec2(1.0, 1.0) / u_textureSize;"
"	vec4 color = vec4(0, 0, 0, 0);"
"	for (int i = -sampleRadius; i <= sampleRadius; i++) {"
"		color += texture2D(u_image, v_texCoord + vec2(0, float(i) * one.y));"
"	}"
"	color /= float(samples);"
"	gl_FragColor = color;"
"}";

static const char frag_color[] =
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = u_colorMatrix * texture2D(u_image, v_texCoord) + u_vector;"
"}";

static const char frag_identity[] =
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = texture2D(u_image, v_texCoord);"
//"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0 );"
"}";

static const char frag_multiply[] =
"varying vec2 v_texCoord;"
"void main() {"
"	gl_FragColor = texture2D(u_image, v_texCoord);"
"}";


static const char frag_dropShadow[] =
"varying vec2 v_texCoord;"
"void main() {"
"	vec4 color = texture2D( u_image, v_texCoord);"
"	vec4 acolor = vec4( 1.0, 0.0, 1.0, 1.0);"
"	const int sampleRadius = 10;"
"	const int samples = sampleRadius * 2 + 1;"
"	vec2 one = vec2( 1.0, 1.0) / 256;"
"	vec4 colort = vec4(0, 0, 0, 0 );"
"	for( int i = -sampleRadius; i<= sampleRadius; i++ ) {"
"		colort += acolor * texture2D(u_image, v_texCoord + vec2(float(i) * one.x, 0)).a;"
"		colort += acolor * texture2D(u_image, v_texCoord + vec2(0, float(i) * one.y)).a;"
"	}"
"	colort /= float( 2 * samples);"
"	vec4 finalcolor = color * color.a + colort*( 1 - color.a);"
"	gl_FragColor = finalcolor;"
//"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0 );"
"}";


static const char frag_glow[] =
"varying vec2 v_texCoord;"
"void main() {"
"	vec4 color = texture2D( u_image, v_texCoord);"
"	vec4 acolor = vec4( 1.0, 0.0, 1.0, 1.0);"
"	const int sampleRadius = 10;"
"	const int samples = sampleRadius * 2 + 1;"
"	vec2 one = vec2( 1.0, 1.0) / 256;"
"	vec4 colort = vec4(0, 0, 0, 0 );"
"	for( int i = -sampleRadius; i<= sampleRadius; i++ ) {"
"		colort += acolor * texture2D(u_image, v_texCoord + vec2(float(i) * one.x, 0)).a;"
"		colort += acolor * texture2D(u_image, v_texCoord + vec2(0, float(i) * one.y)).a;"
"	}"
"	colort /= float( 2 * samples);"
"	gl_FragColor = colort;"
//"	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0 );"
"}";

///////////////////////////////////

typedef struct 
{
	unsigned char* data;
	int size;
	int offset;
}ImageSource;

static void memReadFuncPng(png_structp png_ptr, png_bytep data, png_size_t length)
{
	ImageSource* isource = (ImageSource*) png_get_io_ptr(png_ptr);
	if (isource->offset + length <= isource->size)
	{
		memcpy(data, isource->data + isource->offset, length);
		isource->offset += length;
	}
	else
	{
		png_error(png_ptr, "pngReaderCallback failed");
	}
}

EgretFilter::EgretFilter()
: m_uBufferCapacity(0)
, m_nBufferCount(0)
, m_pBuffer(NULL)
{
   	mPrograme[enFilter_BLURH].mFraBuffer = frag_blurh;
	mPrograme[enFilter_BLURV].mFraBuffer = frag_blurv;
	mPrograme[enFilter_COLOR].mFraBuffer = frag_color;
	mPrograme[enFilter_ALPHA].mFraBuffer = frag_alpha;
	mPrograme[enFilter_MULTIPLY].mFraBuffer = frag_multiply;
	mPrograme[enFilter_IDENTITY].mFraBuffer = frag_identity;
	mPrograme[enFilter_DROPSHADOW].mFraBuffer = frag_dropShadow;
	mPrograme[enFilter_GLOW].mFraBuffer = frag_glow;
}

void EgretFilter::loadShaders()
{
	for (int i = 0; i < enFilter_COUNT; i++)
	{
		mPrograme[i].program.initWithVertexShaderByteArray(vert_canvas, mPrograme[i].mFraBuffer);
		mPrograme[i].program.BindAttributeLocation(enAtt_position_s, enAtt_position);
		mPrograme[i].program.BindAttributeLocation(enAtt_textureCoordinate_s, enAtt_textureCoordinate);
		mPrograme[i].program.link();
		mPrograme[i].mUinform[enUni_resolution] =	mPrograme[i].program.getUniformLocationForName(enUni_resolution_s);
		mPrograme[i].mUinform[enUni_flipY] =		mPrograme[i].program.getUniformLocationForName(enUni_flipY_s);
		mPrograme[i].mUinform[enUni_time] =			mPrograme[i].program.getUniformLocationForName(enUni_time_s);
		mPrograme[i].mUinform[enUni_transformMatrix] = mPrograme[i].program.getUniformLocationForName(enUni_transformMatrix_s);
		mPrograme[i].mUinform[enUni_image] =		mPrograme[i].program.getUniformLocationForName(enUni_image_s);
		mPrograme[i].mUinform[enUni_color] =		mPrograme[i].program.getUniformLocationForName(enUni_color_s);
		mPrograme[i].mUinform[enUni_textureSize] =	mPrograme[i].program.getUniformLocationForName(enUni_textureSize_s);
		mPrograme[i].mUinform[enUni_colorMatrix] =	mPrograme[i].program.getUniformLocationForName(enUni_color_s);
		mPrograme[i].mUinform[enUni_vector] =		mPrograme[i].program.getUniformLocationForName(enUni_textureSize_s);
		mPrograme[i].program.use();
		glUniform1i(mPrograme[i].mUinform[enUni_image], 0);
		glUniform2f(mPrograme[i].mUinform[enUni_resolution], mWidth, mHeight);
		glUniform2f(mPrograme[i].mUinform[enUni_textureSize], 256, 256);
		float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
		glUniform4fv(mPrograme[i].mUinform[enUni_color], 4, color );
		glUniform1f(mPrograme[i].mUinform[enUni_flipY], 1.0);

	}
}

EgretFilter::~EgretFilter()
{
    free(m_pBuffer);
    m_pBuffer = NULL;
   
}

EgretFilter* EgretFilter::create(int width, int height )
{
    EgretFilter* pRet = new EgretFilter();
    pRet->init( width, height );
	pRet->initTest();
	pRet->loadShaders();
    return pRet;
}

void EgretFilter::ensureCapacity(unsigned int count)
{
    if(m_nBufferCount + count > m_uBufferCapacity)
    {
		m_uBufferCapacity += MAX(m_uBufferCapacity, count);
		m_pBuffer = (ccV3F_C4B_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV3F_C4B_T2F));
	}
}



bool EgretFilter::init(int width, int height)
{
	mWidth = width;
	mHeight = height;
    //setShaderProgram( mProgram );
    
    ensureCapacity(512);
    
	frameBufferB.init( 512, 512);
	frameBufferA.init( 512, 512);
    
    return true;
}


void EgretFilter::initTest()
{
	FILE *pFile = fopen( "c:/test.png", "rb" );
	fseek( pFile, 0, SEEK_END );
	int ilen = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );
	unsigned char *pData = new unsigned char[ilen ];
	fread( pData, ilen, 1, pFile );
	fclose( pFile );

	int width = 0;
	int height = 0;
	unsigned char *pImgData = DecodePngData( pData, ilen, width, height );

	GLuint texid = initTexData( pImgData, width, height );
	//makeCheckImages();
	//GLuint texid = initTexData( checkImage, checkImageWidth, checkImageHeight );
	mTexInfo.texId = texid;
	mTexInfo.width = width;
	mTexInfo.height = height;

}


unsigned char* EgretFilter::DecodePngData(unsigned char* fData, long fSize, int& width, int& height)
{
	unsigned char* image_data = NULL;
#ifdef _WIN32
	png_structp png_ptr;
	png_infop info_ptr;
	int bit_depth, color_type;
	png_bytep *row_pointers = NULL;
	int rowbytes;

	/* Create a png read struct */
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr)
	{
		return NULL;
	}

	/* Create a png info struct */
	info_ptr = png_create_info_struct (png_ptr);
	if (!info_ptr)
	{
		png_destroy_read_struct (&png_ptr, NULL, NULL);
		return NULL;
	}

	/* Initialize the setjmp for returning properly after a libpng error occured */
	if (setjmp (png_jmpbuf (png_ptr)))
	{
		png_destroy_read_struct (&png_ptr, &info_ptr, NULL);
		if (row_pointers)
			free (row_pointers);
		return NULL;
	}

	ImageSource imgsource;
	imgsource.data = fData;
	imgsource.size = fSize;
	imgsource.offset = 0;
	png_set_read_fn(png_ptr, &imgsource, memReadFuncPng);

	/* Read png info */
	png_read_info (png_ptr, info_ptr);

	/* Get some usefull information from header */
	bit_depth = png_get_bit_depth (png_ptr, info_ptr);
	color_type = png_get_color_type (png_ptr, info_ptr);

	/* Convert index color images to RGB images */
	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb (png_ptr);

	/* Convert RGB images to RGBA images */
	if (color_type == PNG_COLOR_TYPE_RGB)
		png_set_filler(png_ptr, 0xff, PNG_FILLER_AFTER);

	/* Convert 1-2-4 bits grayscale images to 8 bits grayscale. */
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_gray_1_2_4_to_8 (png_ptr);

	if (png_get_valid (png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha (png_ptr);

	if (bit_depth == 16)
		png_set_strip_16 (png_ptr);
	else if (bit_depth < 8)
		png_set_packing (png_ptr);

	/* Update info structure to apply transformations */
	png_read_update_info (png_ptr, info_ptr);

	/* Retrieve updated information */
	png_get_IHDR (png_ptr, info_ptr, (png_uint_32*)&width, (png_uint_32*)&height, &bit_depth, &color_type, NULL, NULL, NULL);

	rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	if ((image_data =(unsigned char *) malloc(height * rowbytes)) == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return NULL;
	}

	/* Setup a pointer array.  Each one points at the begening of a row. */
	if ((row_pointers =(png_bytepp) malloc(height * sizeof(png_bytep))) == NULL)
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(image_data);
		return NULL;
	}
	for (int i = 0; i < height; i++)
		row_pointers[height - 1 - i] = image_data + i*rowbytes;

	/* Read pixel data using row pointers */
	png_read_image (png_ptr, row_pointers);

	/* Finish decompression and release memory */
	png_read_end (png_ptr, NULL);
	png_destroy_read_struct (&png_ptr, &info_ptr, NULL);

	/* We don't need row pointers anymore */
	free (row_pointers);

#endif
	return image_data;

}

GLuint EgretFilter::initTexData( const void *pData, int width, int height )
{
	GLuint texId = 0;
	glGenTextures( 1, &texId );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, texId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pData );
	glBindTexture( GL_TEXTURE_2D, 0 );
	return texId;

}

void EgretFilter::beginPaint()
{
	glViewport(0, 0, mWidth, mHeight);
	mPrograme[enFilter_IDENTITY].program.use();
	glClearColor(0.0f, 0.0f, 0.0f, 0.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	kmMat4 orthoMatrix;
	kmMat4Identity(&orthoMatrix);
	kmMat4OrthographicProjection(&orthoMatrix, 0, mWidth, mHeight, 0, -1024, 1024);
	glUniformMatrix4fv(mPrograme[enFilter_IDENTITY].mUinform[enUni_transformMatrix], 1, GL_FALSE, orthoMatrix.mat);

}

void EgretFilter::endPaint()
{

}


void EgretFilter::dropShadowFilter()
{
	//frameBufferA.beginPaint(&mPrograme[enFilter_IDENTITY]);
	frameBufferA.beginPaint(&mPrograme[enFilter_ALPHA ]);
	DrawTexture(mTexInfo.texId, 128, 128, 128, 128 );
	frameBufferA.endPatin();

	frameBufferB.beginPaint(&mPrograme[enFilter_BLURH]);
	DrawTexture(frameBufferA.getTexId(), 0, 0, 512, 512);
	frameBufferB.endPatin();

	frameBufferA.beginPaint(&mPrograme[enFilter_BLURV]);
	DrawTexture(frameBufferB.getTexId(), 0, 0, 512, 512);
	frameBufferA.endPatin();

	frameBufferB.beginPaint(&mPrograme[enFilter_MULTIPLY]);
	kmMat4 orthoMatrix;
	kmMat4 tranMat;
	kmMat4Identity(&tranMat);
	kmMat4Identity(&orthoMatrix);
	//kmMat4Translation(&tranMat, -10, 30, 0);
	kmMat4OrthographicProjection(&orthoMatrix, 0, 512, 512, 0, -1024, 1024);
	kmMat4Multiply(&orthoMatrix, &orthoMatrix, &tranMat);
	glUniformMatrix4fv(mPrograme[enFilter_MULTIPLY].mUinform[enUni_transformMatrix], 1, GL_FALSE, orthoMatrix.mat);
	DrawTexture(frameBufferA.getTexId(), 0, 0, 512, 512);
	frameBufferB.endPatin();

	frameBufferB.beginPaint(&mPrograme[enFilter_IDENTITY]);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawTexture(mTexInfo.texId, 128, 128, 128, 128);
	glDisable(GL_BLEND);
	frameBufferB.endPatin();
	//
	beginPaint();
	DrawTexture( frameBufferB.getTexId(), 10, 10, 512, 512);
	endPaint();

	return;
}

void EgretFilter::dropShadowFilterTest()
{
	glViewport(0, 0, mWidth, mHeight);
	mPrograme[enFilter_DROPSHADOW].program.use();
	glClearColor(1.0f, 1.0f, 1.0f, 0.0F);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	kmMat4 orthoMatrix;
	kmMat4Identity(&orthoMatrix);
	kmMat4OrthographicProjection(&orthoMatrix, 0, mWidth, mHeight, 0, -1024, 1024);
	glUniformMatrix4fv(mPrograme[enFilter_IDENTITY].mUinform[enUni_transformMatrix], 1, GL_FALSE, orthoMatrix.mat);
	DrawTexture(mTexInfo.texId, 100, 100, 256, 256);
	return;
}

void EgretFilter::dropGlowFilter()
{
	//frameBufferA.beginPaint(&mPrograme[enFilter_IDENTITY]);
	frameBufferA.beginPaint(&mPrograme[enFilter_GLOW]);
	DrawTexture(mTexInfo.texId, 0, 0, 256, 256);
	frameBufferA.endPatin();

	frameBufferB.beginPaint(&mPrograme[enFilter_MULTIPLY]);
	kmMat4 orthoMatrix;
	kmMat4 tranMat;
	kmMat4Identity(&tranMat);
	kmMat4Identity(&orthoMatrix);
	//kmMat4Translation(&tranMat, 10, 10, 0);
	kmMat4OrthographicProjection(&orthoMatrix, 0, 256, 256, 0, -1024, 1024);
	kmMat4Multiply(&orthoMatrix, &orthoMatrix, &tranMat);
	glUniformMatrix4fv(mPrograme[enFilter_MULTIPLY].mUinform[enUni_transformMatrix], 1, GL_FALSE, orthoMatrix.mat);
	DrawTexture(frameBufferA.getTexId(), 0, 0, 256, 256);
	frameBufferB.endPatin();

	frameBufferB.beginPaint(&mPrograme[enFilter_IDENTITY]);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawTexture(mTexInfo.texId, 0, 0, 256, 256);
	glDisable(GL_BLEND);
	frameBufferB.endPatin();

	beginPaint();
	DrawTexture(frameBufferB.getTexId(), 100, 100, 256, 256);
	endPaint();

}

void EgretFilter::DrawTexture(GLuint texId, float x, float y, float w, float h)
{
	GLfloat verts[] =
	{
		x, y, 0.0f,			0.0f, 1.0f,
		x + w, y, 0.0f,		1.0f, 1.0f,
		x, y + h, 0.0f,		0.0f, 0.0f,
		x + w, y + h, 0.0f, 1.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texId);
	glEnableVertexAttribArray(enAtt_position);
	glEnableVertexAttribArray(enAtt_textureCoordinate);
	glVertexAttribPointer(enAtt_position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, &verts[0]);
	glVertexAttribPointer(enAtt_textureCoordinate, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, &verts[3]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	return;
}


void EgretFilter::DrawFrameTexture(float x, float y, float w, float h)
{
	GLfloat verts[] =
	{
		x, y, 0.0f, 0.0f, 1.0f,
		x + w, y, 0.0f, 1.0f, 1.0f,
		x, y + h, 0.0f, 0.0f, 0.0f,
		x + w, y + h, 0.0f, 1.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mTexInfo.texId );
	glEnableVertexAttribArray(enAtt_position);
	glEnableVertexAttribArray(enAtt_textureCoordinate);
	glVertexAttribPointer(enAtt_position, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, &verts[0]);
	glVertexAttribPointer(enAtt_textureCoordinate, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, &verts[3]);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	return;
}


void EgretFilter::clear()
{
	m_nBufferCount = 0;
}
