
#include "EgretFilter.h"
#include "CCPointExtension.h"
#include "gl/glew.h"
#include "windows.h"
#include "png.h"

const GLchar * shader_frag =
#include "shader_frag.h"
const GLchar * shader_vert =
#include "shader_vert.h"

// ccVertex2F == CGPoint in 32-bits, but not in 64-bits (OS X)
// that's why the "v2f" functions are needed
static ccVertex2F v2fzero = {0.0f,0.0f};


#define GL_POINTS 200
static ccV2F_C4B_T2F gsGLData[ GL_POINTS ];
static int gVertexIndex = 0;
static GLenum gWhichTriangle = 0;

#define MAXCOMBINES (100 * 3)
static GLdouble gCombineVertex[ MAXCOMBINES];
static int      gCombineIndex = 0;

static inline ccVertex2F v2f(float x, float y)
{
    ccVertex2F ret = {x, y};
	return ret;
}

static inline ccVertex2F v2fadd(const ccVertex2F &v0, const ccVertex2F &v1)
{
	return v2f(v0.x+v1.x, v0.y+v1.y);
}

static inline ccVertex2F v2fsub(const ccVertex2F &v0, const ccVertex2F &v1)
{
	return v2f(v0.x-v1.x, v0.y-v1.y);
}

static inline ccVertex2F v2fmult(const ccVertex2F &v, float s)
{
	return v2f(v.x * s, v.y * s);
}

static inline ccVertex2F v2fperp(const ccVertex2F &p0)
{
	return v2f(-p0.y, p0.x);
}

static inline ccVertex2F v2fneg(const ccVertex2F &p0)
{
	return v2f(-p0.x, - p0.y);
}

static inline float v2fdot(const ccVertex2F &p0, const ccVertex2F &p1)
{
	return  p0.x * p1.x + p0.y * p1.y;
}

static inline ccVertex2F v2fforangle(float _a_)
{
	return v2f(cosf(_a_), sinf(_a_));
}

static inline ccVertex2F v2fnormalize(const ccVertex2F &p)
{
	CCPoint r = ccpNormalize(ccp(p.x, p.y));
	return v2f(r.x, r.y);
}

static inline ccVertex2F __v2f(const CCPoint &v)
{
//#ifdef __LP64__
	return v2f(v.x, v.y);
// #else
// 	return * ((ccVertex2F*) &v);
// #endif
}

static inline ccTex2F __t(const ccVertex2F &v)
{
	return *(ccTex2F*)&v;
}

static inline ccTex2F __t( float u, float v )
{
	ccTex2F tf = { u, v };
	return tf;
}


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

// implementation of CCDrawNode
void __stdcall vertexCallback(GLdouble *vertex)
{
	if ( GL_TRIANGLE_FAN == gWhichTriangle )
	{
		if ( gVertexIndex > 2 )
		{
			gsGLData[gVertexIndex] = gsGLData[0];
			gVertexIndex++;
			gsGLData[gVertexIndex] = gsGLData[ gVertexIndex - 2 ];
			gVertexIndex++;
			gsGLData[gVertexIndex].vertices.x = vertex[0];
			gsGLData[gVertexIndex].vertices.y = vertex[1];
			gVertexIndex ++;
		}
		else
		{
			gsGLData[gVertexIndex].vertices.x = vertex[0];
			gsGLData[gVertexIndex].vertices.y = vertex[1];
			gVertexIndex ++;
		}
	}
	else if( GL_TRIANGLE_STRIP == gWhichTriangle )
	{
		if ( gVertexIndex > 2 )
		{
			gsGLData[gVertexIndex] = gsGLData[gVertexIndex - 2 ];
			gVertexIndex++;
			gsGLData[gVertexIndex] = gsGLData[ gVertexIndex - 2 ];
			gVertexIndex++;
			gsGLData[gVertexIndex].vertices.x = vertex[0];
			gsGLData[gVertexIndex].vertices.y = vertex[1];
			gVertexIndex ++;
		}
		else
		{
			gsGLData[gVertexIndex].vertices.x = vertex[0];
			gsGLData[gVertexIndex].vertices.y = vertex[1];
			gVertexIndex ++;
		}
	}
	else
	{
		gsGLData[gVertexIndex].vertices.x = vertex[0];
		gsGLData[gVertexIndex].vertices.y = vertex[1];
		gVertexIndex ++;
	}
}

void __stdcall beginCallback( GLenum which )
{
	gWhichTriangle = which;
	int i = 0;
}

void __stdcall endCallback( )
{
	int i = 0;
}

void __stdcall errorCallback( GLenum errorCode )
{
	int i = 0;
}

void __stdcall combineCallback(GLdouble coords[3], 
							   GLdouble *vertex_data[4],
							   GLfloat weight[4], GLdouble **dataOut )
{
	GLdouble *vertex = gCombineVertex + gCombineIndex;
	gCombineIndex += 3;
	vertex[0] = coords[0];
	vertex[1] = coords[1];
	vertex[2] = coords[2];
	*dataOut = vertex;
}


EgretFilter::EgretFilter()
: m_uVbo(0)
, m_uBufferCapacity(0)
, m_nBufferCount(0)
, m_pBuffer(NULL)
, m_bDirty(false)
, mProgram( 0 )
{
	mbgcolor.r = 0;
	mbgcolor.g = 0;
	mbgcolor.b = 255;
	mbgcolor.a = 255;
	mWidth = 0;
	mHeight = 0;
    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;
	loadShaders();
}

void EgretFilter::loadShaders()
{
	mProgram = new CCGLProgram();
	mProgram->initWithVertexShaderByteArray(shader_vert, shader_frag);
	mProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	mProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	mProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	mProgram->link();
	mProgram->updateUniforms();
}

EgretFilter::~EgretFilter()
{
    free(m_pBuffer);
    m_pBuffer = NULL;
    
    glDeleteBuffers(1, &m_uVbo);
    m_uVbo = 0;
}

void EgretFilter::InitPolygon()
{
	tobj = gluNewTess();
	gluTessCallback(tobj, GLU_TESS_VERTEX,	(void (__stdcall *)())vertexCallback);
	gluTessCallback(tobj, GLU_TESS_BEGIN,	(void (__stdcall *)())beginCallback);
	gluTessCallback(tobj, GLU_TESS_END,		(void (__stdcall *)())endCallback);
	gluTessCallback(tobj, GLU_TESS_ERROR,	(void (__stdcall *)())errorCallback);
	gluTessCallback(tobj, GLU_TESS_COMBINE, (void (__stdcall *)())combineCallback);
	gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
	gCombineIndex = 0;
	gVertexIndex = 0;
	gWhichTriangle = 0;
	return;
}

void EgretFilter::UninitPolygon()
{
	gluDeleteTess(tobj);
	gCombineIndex = 0;
	gVertexIndex = 0;
}


//p3
void EgretFilter::fillRect( float x, float y, float width, float height )
{
	glUniform1i((GLint)gUniforms[kCCuniformDrawType], 4);
	kmMat4 texMat =
	{
		mpFillStyle->width, 0, 0, 0,
		0, mpFillStyle->height, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	kmMat4 texMatIn;
	kmMat4 rotaMat;
	kmMat4 tranMat;
	kmMat4Identity(&rotaMat);
	kmMat4RotationZ(&rotaMat, -60);
	kmMat4Identity(&texMatIn);
	kmMat4Inverse(&texMatIn, &texMat);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpFillStyle->texId);
			
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	

	//kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
	glUniformMatrix4fv(gUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat);
	unsigned int vertex_count = 2 * 6;
	ensureCapacity(vertex_count);
	ccColor4B col = {0, 0, 0, 0 };
	ccV2F_C4B_T2F_Triangle triangle =
	{
		{ vertex2(x, y + height), col, { 0, 0 } },
		{ vertex2(x, y), col, { 0, height / mpFillStyle->height } },
		{ vertex2(x + width, y + height), col, { width / mpFillStyle->width, 0 } },
	};
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)(m_pBuffer + m_nBufferCount);

	triangles[0] = triangle;
	ccV2F_C4B_T2F_Triangle triangle1 =
	{
		{ vertex2(x, y), col, { 0, height / mpFillStyle->height } },
		{ vertex2(x + width, y + height), col, { width / mpFillStyle->width, 0 } },
		{ vertex2(x + width, y), col, { width / mpFillStyle->width, height / mpFillStyle->height } }
	};
	triangles[1] = triangle1;
	m_nBufferCount += vertex_count;
	m_bDirty = true;
	
	return;

}

EgretFilter* EgretFilter::create()
{
    EgretFilter* pRet = new EgretFilter();
    pRet->init();
	pRet->initTest();
    return pRet;
}

void EgretFilter::ensureCapacity(unsigned int count)
{
    if(m_nBufferCount + count > m_uBufferCapacity)
    {
		m_uBufferCapacity += MAX(m_uBufferCapacity, count);
		m_pBuffer = (ccV2F_C4B_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV2F_C4B_T2F));
	}
}



bool EgretFilter::init()
{

    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;

	mpFillStyle = new XPattern();

    //setShaderProgram( mProgram );
    
    ensureCapacity(512);
    
    glGenBuffers(1, &m_uVbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)* m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
    
    glEnableVertexAttribArray(kCCVertexAttrib_Position);
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
    
    glEnableVertexAttribArray(kCCVertexAttrib_Color);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
    
    glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    m_bDirty = true;
    
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

	XPattern *pattern = new XPattern();
	pattern->texId = texid;
	pattern->width = width;
	pattern->height = height;
	mpFillStyle =  pattern;

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

void EgretFilter::drawFrameBuffer()
{
	ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);

	mProgram->use();
	mProgram->setMatrixValue();

	glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
	m_bDirty = false;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mpFillStyle->texId);
	glEnableVertexAttribArray(kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid*)offsetof(ccV2F_C4B_T2F, vertices));
	glDrawArrays(GL_TRIANGLES, 0, m_nBufferCount);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(kCCVertexAttrib_Position);

	return;
}

void EgretFilter::clear()
{
    m_nBufferCount = 0;
    m_bDirty = true;
}


//GLdouble star[5][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	125.0, 200.0, 0.0, 1.0, 1.0, 0.0,
//	200.0, 50.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 0.0, 0.0,
//	200.0, 150.0, 0.0, 0.0, 1.0, 0.0
//};

//GLdouble star[5][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	125.0, 200.0, 0.0, 1.0, 1.0, 0.0,
//	200.0, 50.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 0.0, 0.0,
//	200.0, 150.0, 0.0, 0.0, 1.0, 0.0
//};

//GLdouble star[8][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	150.0, 50.0, 0.0, 1.0, 0.0, 0.0,
//	150.0, 150.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 1.0, 0.0,
//	70.0, 70.0, 0.0, 1.0, 0.0, 0.0,
//	70.0, 120.0, 0.0, 0.0, 1.0, 1.0,
//	120.0, 120.0, 0.0, 1.0, 1.0, 0.0,
//	120.0, 70.0, 0.0, 1.0, 0.0, 1.0
//};
//
//GLdouble star[10][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	150.0, 50.0, 0.0, 1.0, 0.0, 0.0,
//	//150.0, 70.0, 0.0, 0.0, 1.0, 1.0,
//	//120.0, 70.0, 0.0, 1.0, 0.0, 1.0,
//	70.0, 70.0, 0.0, 1.0, 0.0, 0.0,
//	70.0, 120.0, 0.0, 0.0, 1.0, 1.0,
//	120.0, 120.0, 0.0, 1.0, 1.0, 0.0,
//	120.0, 70.0, 0.0, 1.0, 0.0, 1.0,
//	150.0, 70.0, 0.0, 0.0, 1.0, 1.0,
//	150.0, 150.0, 0.0, 0.0, 1.0, 1.0,
//	50.0, 150.0, 0.0, 1.0, 1.0, 0.0
//};

//GLdouble star[8][6] =
//{
//	50.0, 50.0, 0.0, 1.0, 0.0, 1.0,
//	50.0, 200.0, 0.0, 1.0, 0.0, 0.0,
//	100.0, 200.0, 0.0, 0.0, 1.0, 1.0,
//	100.0, 100.0, 0.0, 1.0, 1.0, 0.0,
//	150.0, 100.0, 0.0, 1.0, 0.0, 0.0,
//	150.0, 200.0, 0.0, 0.0, 1.0, 1.0,
//	200.0, 200.0, 0.0, 1.0, 1.0, 0.0,
//	200.0, 50.0, 0.0, 1.0, 0.0, 1.0
//};



ccBlendFunc EgretFilter::getBlendFunc() const
{
    return m_sBlendFunc;
}

void EgretFilter::setBlendFunc(const ccBlendFunc &blendFunc)
{
    m_sBlendFunc = blendFunc;
}
