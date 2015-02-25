
#include "RasterGL.h"
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

void XGradientLinear::addColorStop( float index, ccColor4F color )
{
	mbDirty = true;
	if ( this->pGraData == NULL )
	{
		pGraData = new GradientData();
		pGraData->index = index;
		pGraData->color = color;
	}
	else
	{
		GradientData *p = pGraData;
		GradientData *pn = p->pNext;
		GradientData *cur = new GradientData();
		cur->color = color;
		cur->index = index;
		cur->pNext = NULL;
		while( p != NULL && pn != NULL )
		{
			if ( p->index < index && index < pn->index )
			{
				p->pNext = cur;
				cur->pNext = pn;
				return;
			}
			p = pn;
			pn = pn->pNext;
		}
		p->pNext = cur;

	}
}

void XGradientLinear::CreateTextrue()
{
	if ( ! mbDirty )
	{
		mbDirty = false;
		return;
	}
	GLubyte *pTexData = new GLubyte[miLen * 4 + 4 ];
	memset( pTexData, 0, miLen * 4 );
	ccColor4B *pdata = (ccColor4B*)pTexData;
	GradientData *pgd = pGraData;
	GradientData *pn = pgd->pNext;
	int icout = 0;
	while( pgd != NULL && pn != NULL )
	{
		int index1 = floor(pgd->index * miLen + 0.5);
		int step = floor( pn->index *miLen + 0.5 ) - index1;
		//pdata += index1;
		for ( int i = 0; i < step; i++ )
		{
			icout++;
			pdata->a = pgd->color.a * 255 - (pgd->color.a - pn->color.a ) * 255 * i / step;
			pdata->r = pgd->color.r * 255 - (pgd->color.r - pn->color.r ) * 255 * i / step;
			pdata->g = pgd->color.g * 255 - (pgd->color.g - pn->color.g ) * 255 * i / step;
			pdata->b = pgd->color.b * 255 - (pgd->color.b - pn->color.b ) * 255 * i / step;
			pdata++;
		}
		pgd = pn;
		pn = pn->pNext;
	}

	glGenTextures( 1, &texId );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, texId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)miLen, (GLsizei)1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexData );
	glBindTexture( GL_TEXTURE_2D, 0 );
	delete []pTexData;

	return;
}

void XGradientRadial::addColorStop( float index, ccColor4F color )
{
	mbDirty = true;
	GradientData *p = pGraData;
	GradientData *pn = p->pNext;
	GradientData *cur = new GradientData();
	cur->color = color;
	cur->index = index;
	cur->pNext = NULL;
	while( p != NULL && pn != NULL )
	{
		if ( p->index < index && index < pn->index )
		{
			p->pNext = cur;
			cur->pNext = pn;
			return;
		}
		p = pn;
		pn = pn->pNext;
	}
	p->pNext = cur;
}

void XGradientRadial::CreateTextrue()
{
	if ( ! mbDirty )
	{
		mbDirty = false;
		return;
	}
	GLubyte *pTexData = new GLubyte[miLen * 4 ];
	memset( pTexData, 0, miLen * 4 );
	ccColor4B *pdata = (ccColor4B*)pTexData;
	GradientData *pgd = pGraData;
	GradientData *pn = pgd->pNext;
	if ( pn == NULL )
	{
		return;
	}
	pgd->color = pn->color;
	while( pgd != NULL && pn != NULL )
	{
		int index1 = floor(pgd->index * miLen + 0.5);
		int step = floor( pn->index *miLen + 0.5 ) - index1;
		//pdata += index1;
		for ( int i = 0; i < step; i++ )
		{
			pdata->a = pgd->color.a * 255 - (pgd->color.a - pn->color.a ) * 255 * i / step;
			pdata->r = pgd->color.r * 255 - (pgd->color.r - pn->color.r ) * 255 * i / step;
			pdata->g = pgd->color.g * 255 - (pgd->color.g - pn->color.g ) * 255 * i / step;
			pdata->b = pgd->color.b * 255 - (pgd->color.b - pn->color.b ) * 255 * i / step;
			pdata++;
		}
		pgd = pn;
		pn = pn->pNext;
	}

	glGenTextures( 1, &texId );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, texId );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)miLen, (GLsizei)1, 0, GL_RGBA, GL_UNSIGNED_BYTE, pTexData );
	glBindTexture( GL_TEXTURE_2D, 0 );
	delete []pTexData;

}

XFillStyle::XFillStyle()
{

}

XFillStyle::XFillStyle( ccColor4F color )
{
	mFillType = FILL_COLOR;
	mColor = color;
}

XFillStyle::XFillStyle( XGradientLinear *gradient )
{
	mFillType = FILL_Gradient_Line;
	mpGradientLinear = gradient;
}

XFillStyle::XFillStyle( XGradientRadial *gradient )
{
	mFillType = FILL_Gradient_radius;
	mpGradientRadial = gradient;
}

XFillStyle::XFillStyle( XPattern *pattern )
{
	mFillType = FILL_PATTERN;
	mpPattern = pattern;
}

void XFillStyle::setFillType( ccColor4F color )
{
	mFillType = FILL_COLOR;
	mColor = color;
}
void XFillStyle::setFillType( XGradientLinear *gradient )
{
	mFillType = FILL_Gradient_Line;
	mpGradientLinear = gradient;
}

void XFillStyle::setFillType( XGradientRadial *gradient )
{
	mFillType = FILL_Gradient_radius;
	mpGradientRadial = gradient;
}

void XFillStyle::setFillType( XPattern *pattern )
{
	mFillType = FILL_PATTERN;
	mpPattern = pattern;
}

void EgPath::GenBuffer()
{
	glGenBuffers( 1, &muVbo );
}

void EgPath::BindBuffer()
{
	glBindBuffer( GL_ARRAY_BUFFER, muVbo );
}

void EgPath::BufferData(GLsizeiptr size, const GLvoid *data )
{
	glBufferData( GL_ARRAY_BUFFER, size, data, GL_STREAM_DRAW );
}

void EgPath::BufferSubData(GLuint offset,GLsizeiptr size, const GLvoid *data )
{
	glBufferSubData( GL_ARRAY_BUFFER, offset, size, data );
}

void EgPath::DeleteBuffer()
{
	glDeleteBuffers( 1, &muVbo );
}

void __stdcall vertexCallback(GLdouble *vertex)
{
	gsGLData[gVertexIndex].vertices.x = vertex[0];
	gsGLData[gVertexIndex].vertices.y = vertex[1];
	gVertexIndex ++;
}

void __stdcall beginCallback( GLenum which )
{
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


XContext::XContext()
: m_uVbo(0)
, m_uBufferCapacity(0)
, m_nBufferCount(0)
, m_pBuffer(NULL)
, m_bDirty(false)
, mProgram( 0 )
{
	pCurPath = NULL;
	mEgPaths = NULL;
	pEndPath = NULL;
	//mLineWidth = 1;
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

void XContext::loadShaders()
{
	mProgram = new CCGLProgram();
	mProgram->initWithVertexShaderByteArray(shader_vert, shader_frag);
	mProgram->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
	mProgram->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
	mProgram->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
	mProgram->link();
	mProgram->updateUniforms();
}

XContext::~XContext()
{
    free(m_pBuffer);
    m_pBuffer = NULL;
    
    glDeleteBuffers(1, &m_uVbo);
    m_uVbo = 0;
}

void XContext::InitPolygon()
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
	return;
}

void XContext::UninitPolygon()
{
	gluDeleteTess(tobj);
}

void XContext::LineWidth(  float width )
{
	pCurPath->mLineWidth = width;
}

GLdouble gTmpData[3][3] = 
{
	80, 240, 0,
	140, 240, 0,
	110, 60, 0
};

void XContext::fill()
{

	if( pCurPath )
	{
		int ilen = sizeof(ccV2F_C4B_T2F);
		if ( pCurPath->cmdType == CTX_LINETO )
		{
			glUniform1i( (GLint)gUniforms[kCCuniformDrawType], FILL_COLOR );
			glUniform4fv( (GLint)gUniforms[kCCUniformFillColor], 1, (GLfloat*)&mpFillStyle->mColor );
			pCurPath->GenBuffer();
			pCurPath->BindBuffer();
			EgEdge *p = pCurPath->pEdges;
			InitPolygon();
			gluTessBeginPolygon(tobj, NULL);
			gluTessBeginContour(tobj);
			GLdouble xyz[3] = { pCurPath->startx, pCurPath->starty, 0 };
			for ( int i = 0; i < 3; i++ )
			{
				gluTessVertex( tobj, gTmpData[i], gTmpData[i] );
			}
			//gluTessVertex(tobj, xyz, xyz);
			//while( p )
			//{
			//	xyz[0] = p->endx;
			//	xyz[1] = p->endy;
			//	xyz[2] = 0;
			//	gluTessVertex(tobj, xyz, xyz);
			//	p = p->pNext;
			//}	
			gluTessEndContour(tobj);
			gluTessEndPolygon(tobj);
			pCurPath->mbufferLen = gVertexIndex * sizeof(ccV2F_C4B_T2F);
			pCurPath->BufferData( pCurPath->mbufferLen, gsGLData );
			pCurPath->pointCount = gVertexIndex;
			UninitPolygon();
		}
	}
	//EgPath *pTmpPath = mEgPaths;
	//while( pTmpPath )
	//{
	//	if ( pTmpPath->cmdType == CTX_LINETO )
	//	{
	//		CCPoint from( pTmpPath->startx, pTmpPath->starty );	
	//		EgEdge *p = pTmpPath->pEdges;
	//		ccColor4F color = {1.0, 0, 0, 1 };
	//		while( p )
	//		{
	//			CCPoint to( p->endx, p->endy );
	//			drawSegment( from, to, mLineWidth, mpFillStyle->mColor );
	//			from.x = to.x;
	//			from.y = to.y;
	//			p = p->pNext;
	//		}
	//	}
	//	else if( pTmpPath->cmdType == CTX_RECT )
	//	{
	//		EgEdge *pEdge = pTmpPath->pEdges;
	//		unsigned int vertex_cout = 2 * 6;
	//		ensureCapacity( vertex_cout );
	//		ccColor4F color = { 1.0, 0, 0, 1.0 };
	//		ccColor4B col = ccc4BFromccc4F( color );
	//		ccV2F_C4B_T2F_Triangle triangle =
	//		{
	//			{ vertex2( pTmpPath->startx,  pTmpPath->starty), col, __t( v2fzero) },
	//			{ vertex2( pEdge->endx, pEdge->endy ), col, __t( v2fzero ) },
	//			{ vertex2( pEdge->pNext->endx, pEdge->pNext->endy ), col, __t( v2fzero ) }
	//		};
	//		ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
	//		//ccV2F_C4B_T2F_Triangle triangle = { a, b, c };
	//		triangles[0] = triangle;
	//		ccV2F_C4B_T2F_Triangle triangle1 =
	//		{
	//			{ vertex2( pEdge->endx, pEdge->endy ), col, __t( v2fzero) },
	//			{ vertex2( pEdge->pNext->endx, pEdge->pNext->endy ), col, __t( v2fzero ) },
	//			{ vertex2( pEdge->pNext->pNext->endx, pEdge->pNext->pNext->endy ), col, __t( v2fzero ) }
	//		};
	//		triangles[1] = triangle1;
	//		m_nBufferCount += vertex_cout;
	//		m_bDirty = true;
	//	}
	//	else if( pTmpPath->cmdType == CTX_ARC )
	//	{
	//		unsigned int vertex_count = 3 * pTmpPath->count;
	//		ensureCapacity( vertex_count );
	//		EgEdge *p = pTmpPath->pEdges;
	//		int icout = 0;
	//		ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
	//		ccColor4F color = { 1.0, 0, 0, 1.0 };
	//		ccColor4B col = ccc4BFromccc4F( color );
	//		while( p != NULL && p->pNext != NULL )
	//		{
	//			ccV2F_C4B_T2F_Triangle triangle =
	//			{
	//				{ vertex2( pTmpPath->startx,  pTmpPath->starty), col, __t( v2fzero) },
	//				{ vertex2( p->endx, p->endy ), col, __t( v2fzero ) },
	//				{ vertex2( p->pNext->endx, p->pNext->endy ), col, __t( v2fzero ) }
	//			};
	//			triangles[icout] = triangle;
	//			icout++;
	//			//char chTmp[256];
	//			//sprintf( chTmp, "-->%d, x:%f, y:%f, xx:%f, yy:%f", icout, p->endx, p->endy,
	//			//	p->pNext->endx, p->pNext->endy );
	//			//OutputDebugStringA( chTmp );
	//			p = p->pNext;
	//		}
	//		m_nBufferCount += vertex_count;
	//		m_bDirty = true;
	//	}
	//	else if( pTmpPath->cmdType == CTX_QUADRATICCURVETO )
	//	{
	//		CCPoint from( pTmpPath->startx, pTmpPath->starty );	
	//		EgEdge *p = pTmpPath->pEdges;
	//		ccColor4F color = {1.0, 0, 0, 1 };

	//		float t = 0.0f;
	//		int segments = 60;
	//		float x = pTmpPath->startx;
	//		float y = pTmpPath->starty;
	//		for ( int i = 0; i < segments; i++ )
	//		{
	//			float x1 = powf(1 - t, 2) * pTmpPath->startx + 2.0f * (1 - t) * t * p->cpx + t * t * p->endx;
	//			float y1 = powf(1 - t, 2 ) *pTmpPath->starty + 2.0f * (1 - t) * t * p->cpy + t * t * p->endy;
	//			t += 1.0f / segments;
	//			CCPoint from( x, y );
	//			CCPoint to( x1, y1 );
	//			drawSegment( from, to, mLineWidth, mpFillStyle->mColor );
	//			char chTmp[256];
	//			sprintf( chTmp, "-->x1:%f, y1:%f, x2:%f, y2:%f", x, y, x1, y1 );
	//			OutputDebugStringA( chTmp );
	//			x = x1;
	//			y = y1;
	//		}
	//	}
	//	else if( pEndPath->cmdType == CTX_BEZIERCURVETO )
	//	{
	//		CCPoint from( pTmpPath->startx, pTmpPath->starty );	
	//		EgEdge *p = pTmpPath->pEdges;
	//		ccColor4F color = {1.0, 0, 0, 1 };

	//		float t = 0.0f;
	//		int segments = 60;
	//		float x = pTmpPath->startx;
	//		float y = pTmpPath->starty;
	//		float cp1x = pTmpPath->pEdges->cpx;
	//		float cp1y = pTmpPath->pEdges->cpy;
	//		float cp2x = pTmpPath->pEdges->pNext->cpx;
	//		float cp2y = pTmpPath->pEdges->pNext->cpy;
	//		float endx = pTmpPath->pEdges->endx;
	//		float endy = pTmpPath->pEdges->endy;
	//		for ( int i = 0; i < segments; i++ )
	//		{
	//			float x1 = powf(1 - t, 3) * pTmpPath->startx + 3.0f * powf(1 - t, 2) * t * cp1x + 3.0f * (1 - t) * t * t * cp2x + t * t * t * endx;
	//			float y1 = powf(1 - t, 3) * pTmpPath->starty + 3.0f * powf(1 - t, 2) * t * cp1y + 3.0f * (1 - t) * t * t * cp2y + t * t * t * endy;
	//			t += 1.0f / segments;
	//			CCPoint from( x, y );
	//			CCPoint to( x1, y1 );
	//			drawSegment( from, to, mLineWidth, mpFillStyle->mColor );
	//			x = x1;
	//			y = y1;
	//		}
	//	}
	//	pTmpPath = pTmpPath->pNext;
	//}
	return;
}

void XContext::beginPath()
{
	EgPath *pTmpPath = mEgPaths;
	while( pTmpPath != NULL )
	{
		EgEdge *pEdge = pTmpPath->pEdges;
		while( pEdge )
		{
			EgEdge *p = pEdge;
			pEdge = pEdge->pNext;
			delete p;
		}
		EgPath *ppath = pTmpPath;
		pTmpPath = pTmpPath->pNext;
		delete ppath;
	}
	mEgPaths = new EgPath();
	pCurPath = mEgPaths;
	memset( mEgPaths, 0, sizeof( EgPath ));
	pCurPath = mEgPaths;
	pEndPath = mEgPaths;
	pCurPath->pEndEdge = NULL;
	pCurPath->pEdges = NULL;
}

void XContext::closePath()
{
	EgEdge *p = new EgEdge();
	pCurPath->pEndEdge->pNext = p;
	p->pNext = NULL;
	p->cpx = pCurPath->startx;
	p->cpy = pCurPath->starty;
	p->endx = pCurPath->startx;
	p->endy = pCurPath->starty;
	return;

}

void XContext::stroke()
{
	if( pCurPath )
	{
		int ilen = sizeof(ccV2F_C4B_T2F);
		if ( pCurPath->cmdType == CTX_LINETO )
		{
			glUniform1i( (GLint)gUniforms[kCCuniformDrawType], FILL_COLOR );
			glUniform4fv( (GLint)gUniforms[kCCUniformFillColor], 1, (GLfloat*)&mpFillStyle->mColor );
			pCurPath->GenBuffer();
			pCurPath->BindBuffer();
			pCurPath->mbufferLen = (pCurPath->count - 1) * 3 * 6 * sizeof(ccV2F_C4B_T2F);
			pCurPath->BufferData( pCurPath->mbufferLen, gsGLData );
			pCurPath->pointCount = (pCurPath->count - 1) * 3 * 6;
			pCurPath->mCurIndex = 0;
			CCPoint from( pCurPath->startx, pCurPath->starty );	
			EgEdge *p = pCurPath->pEdges;
			ccColor4F color = {1.0, 0, 0, 1 };
			int ilen = 3 * 6 * sizeof(ccV2F_C4B_T2F );
			while( p )
			{
				CCPoint to( p->endx, p->endy );
				drawSegment( from, to, pCurPath->mLineWidth, mpFillStyle->mColor );
				pCurPath->BufferSubData( pCurPath->mCurIndex,  ilen, gsGLData );
				pCurPath->mCurIndex += ilen;
				from.x = to.x;
				from.y = to.y;
				p = p->pNext;
			}	
		}
		//else if( pCurPath->cmdType == CTX_RECT )
		//{
		//	EgEdge *pEdge = pCurPath->pEdges;
		//	CCPoint tl( pTmpPath->startx, pTmpPath->starty );
		//	CCPoint tr( pEdge->endx, pTmpPath->starty );
		//	CCPoint bl( pTmpPath->startx, pEdge->endy );
		//	CCPoint br( pEdge->endx, pEdge->endy );
		//	drawSegment( tl, tr, pTmpPath->mLineWidth, mpFillStyle->mColor );
		//	drawSegment( tr, br, pTmpPath->mLineWidth, mpFillStyle->mColor );
		//	drawSegment( br, bl, pTmpPath->mLineWidth, mpFillStyle->mColor );
		//	drawSegment( bl, tl, pTmpPath->mLineWidth, mpFillStyle->mColor );
		//	pCurPath = pTmpPath;
		//}
		//else if( pTmpPath->cmdType == CTX_ARC )
		//{
		//	unsigned int vertex_count = 3 * pTmpPath->count;
		//	ensureCapacity( vertex_count );
		//	EgEdge *p = pTmpPath->pEdges;
		//	int icout = 0;
		//	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
		//	ccColor4F color = { 1.0, 0, 0, 1.0 };
		//	ccColor4B col = ccc4BFromccc4F( color );
		//	while( p != NULL && p->pNext != NULL )
		//	{
		//		ccV2F_C4B_T2F_Triangle triangle =
		//		{
		//			{ vertex2( pTmpPath->startx,  pTmpPath->starty), col, __t( v2fzero) },
		//			{ vertex2( p->endx, p->endy ), col, __t( v2fzero ) },
		//			{ vertex2( p->pNext->endx, p->pNext->endy ), col, __t( v2fzero ) }
		//		};
		//		triangles[icout] = triangle;
		//		icout++;
		//		//char chTmp[256];
		//		//sprintf( chTmp, "-->%d, x:%f, y:%f, xx:%f, yy:%f", icout, p->endx, p->endy,
		//		//	p->pNext->endx, p->pNext->endy );
		//		//OutputDebugStringA( chTmp );
		//		p = p->pNext;
		//	}
		//	m_nBufferCount += vertex_count;
		//	m_bDirty = true;
		//}
		//else if( pTmpPath->cmdType == CTX_QUADRATICCURVETO )
		//{
		//	CCPoint from( pTmpPath->startx, pTmpPath->starty );	
		//	EgEdge *p = pTmpPath->pEdges;
		//	ccColor4F color = {1.0, 0, 0, 1 };

		//	float t = 0.0f;
		//	int segments = 60;
		//	float x = pTmpPath->startx;
		//	float y = pTmpPath->starty;
		//	for ( int i = 0; i < segments; i++ )
		//	{
		//		float x1 = powf(1 - t, 2) * pTmpPath->startx + 2.0f * (1 - t) * t * p->cpx + t * t * p->endx;
		//		float y1 = powf(1 - t, 2 ) *pTmpPath->starty + 2.0f * (1 - t) * t * p->cpy + t * t * p->endy;
		//		t += 1.0f / segments;
		//		CCPoint from( x, y );
		//		CCPoint to( x1, y1 );
		//		drawSegment( from, to, pTmpPath->mLineWidth, mpFillStyle->mColor );
		//		char chTmp[256];
		//		sprintf( chTmp, "-->x1:%f, y1:%f, x2:%f, y2:%f", x, y, x1, y1 );
		//		OutputDebugStringA( chTmp );
		//		x = x1;
		//		y = y1;
		//	}
		//}
		//else if( pCurPath->cmdType == CTX_BEZIERCURVETO )
		//{
		//	CCPoint from( pTmpPath->startx, pTmpPath->starty );	
		//	EgEdge *p = pTmpPath->pEdges;
		//	ccColor4F color = {1.0, 0, 0, 1 };

		//	float t = 0.0f;
		//	int segments = 60;
		//	float x = pTmpPath->startx;
		//	float y = pTmpPath->starty;
		//	float cp1x = pTmpPath->pEdges->cpx;
		//	float cp1y = pTmpPath->pEdges->cpy;
		//	float cp2x = pTmpPath->pEdges->pNext->cpx;
		//	float cp2y = pTmpPath->pEdges->pNext->cpy;
		//	float endx = pTmpPath->pEdges->endx;
		//	float endy = pTmpPath->pEdges->endy;
		//	for ( int i = 0; i < segments; i++ )
		//	{
		//		float x1 = powf(1 - t, 3) * pTmpPath->startx + 3.0f * powf(1 - t, 2) * t * cp1x + 3.0f * (1 - t) * t * t * cp2x + t * t * t * endx;
		//		float y1 = powf(1 - t, 3) * pTmpPath->starty + 3.0f * powf(1 - t, 2) * t * cp1y + 3.0f * (1 - t) * t * t * cp2y + t * t * t * endy;
		//		t += 1.0f / segments;
		//		CCPoint from( x, y );
		//		CCPoint to( x1, y1 );
		//		drawSegment( from, to, pTmpPath->mLineWidth, mpFillStyle->mColor );
		//		x = x1;
		//		y = y1;
		//	}
		//}
	
	}
	return;
}

void XContext::moveto( float x, float y )
{
	pCurPath->startx = x;
	pCurPath->starty = y;
	pCurPath->count++;
	pCurPath->cmdType = CTX_MOVETO;

}

void XContext::lineto( float x, float y )
{
	pCurPath->cmdType = CTX_LINETO;
	if ( pCurPath->pEdges == NULL )
	{
		pCurPath->pEdges = new EgEdge();
		memset( pCurPath->pEdges, 0, sizeof( EgEdge ));
		pCurPath->pEndEdge = pCurPath->pEdges;
		pCurPath->pEndEdge->cpx = x;
		pCurPath->pEndEdge->cpy = y;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = true;
	}
	else
	{
		EgEdge *p = new EgEdge();
		pCurPath->pEndEdge->pNext = p;
		pCurPath->pEndEdge = p;
		memset( pCurPath->pEndEdge, 0, sizeof( EgEdge ) );
		pCurPath->pEndEdge->cpx = x;
		pCurPath->pEndEdge->cpy = y;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = true;
	}
	pCurPath->count++;
	return;
}

void XContext::arc( float x, float y, float radius, float sAngle, float eAngle, bool counterclockwise )
{
	pCurPath->cmdType = CTX_ARC;
	pCurPath->startx = x;
	pCurPath->starty = y;

	GLfloat angle = sAngle;
	for ( angle = sAngle; angle <= eAngle; angle += 0.02f )
	{
		pCurPath->count++;
		float x1 = 0.0;
		float y1 = 0.0;
		if ( counterclockwise )
		{
			x1 = radius * cos( angle ) + x;
			y1 = radius * sin( angle ) + y;
		}
		else
		{
			x1 = radius * cos( angle ) + x;
			y1 = radius * -sin( angle ) + y;
		}
		

		//char chTmp[256];
		//sprintf( chTmp, "x:%f, y:%f", x1, y1 );
		//OutputDebugStringA( chTmp );
		
		EgEdge *p = new EgEdge();
		if ( pCurPath->pEdges == NULL )
		{
			pCurPath->pEdges = p;
			pCurPath->pEndEdge = p;
		}
		else
		{
			pCurPath->pEndEdge->pNext = p;
			pCurPath->pEndEdge = p;
		}
		p->pNext = NULL;
		p->cpx = x1;
		p->cpy = y1;
		p->endx = x1;
		p->endy = y1;
		p->isLine = true;
		
	}
	return;
}

void XContext::rect( float x, float y, float width, float height )
{
	if ( mEgPaths == NULL )
	{
		mEgPaths = new EgPath();
		memset( mEgPaths, 0, sizeof( EgPath ) );
		pCurPath = mEgPaths;
		pCurPath->pEndEdge = NULL;
		pCurPath->pEdges = NULL;
	}
	else
	{
		EgPath *path = new EgPath();
		memset( path, 0, sizeof( EgPath ) );
		pCurPath->pNext = path;
		pCurPath = path;
	}
	if ( pCurPath->pEdges == NULL )
	{
		pCurPath->pEdges = new EgEdge();
		memset( pCurPath->pEdges, 0, sizeof( EgEdge ));
		pCurPath->pEndEdge = pCurPath->pEdges;
	}
	EgEdge *p1 = pCurPath->pEndEdge;
	pCurPath->cmdType = CTX_RECT;
	pCurPath->startx = x;
	pCurPath->starty = y;
	p1->cpx = x;
	p1->cpy = y;
	p1->endx = x + width;
	p1->endy = y + height;
	p1->isLine = true;

}

void XContext::save()
{
	//暂时不实现
}

void XContext::restore()
{
	//暂时不实现
}

// p2
void XContext::quadraticCurveTo( float cpx, float cpy, float x, float y )
{
	pCurPath->cmdType = CTX_QUADRATICCURVETO;
	if ( pCurPath->pEdges == NULL )
	{
		pCurPath->pEdges = new EgEdge();
		memset( pCurPath->pEdges, 0, sizeof( EgEdge ));
		pCurPath->pEndEdge = pCurPath->pEdges;
		pCurPath->pEndEdge->cpx = cpx;
		pCurPath->pEndEdge->cpy = cpy;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = false;
	}
	else
	{
		EgEdge *p = new EgEdge();
		pCurPath->pEndEdge->pNext = p;
		pCurPath->pEndEdge = p;
		memset( pCurPath->pEndEdge, 0, sizeof( EgEdge ) );
		pCurPath->pEndEdge->cpx = cpx;
		pCurPath->pEndEdge->cpy = cpy;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = false;
	}
	return;
}

//p3
void XContext::fillRect( float x, float y, float width, float height )
{
	if ( mpFillStyle->mFillType == FILL_COLOR )
	{
		glUniform1i( (GLint)gUniforms[kCCuniformDrawType], FILL_COLOR );
		glUniform4fv( (GLint)gUniforms[kCCUniformFillColor], 1, (GLfloat*)&mpFillStyle->mColor );
		unsigned int vertex_count = 2 * 6;
		ensureCapacity( vertex_count );
		ccColor4B col = ccc4BFromccc4F( mpFillStyle->mColor );
		ccV2F_C4B_T2F_Triangle triangle =
		{
			{ vertex2( x, y), col, {0, 0} },
			{ vertex2( x, y + height), col, {0, 0 } },
			{ vertex2( x + width, y), col, { 0, 0 }  },
		};
		ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );

		triangles[0] = triangle;
		ccV2F_C4B_T2F_Triangle triangle1 =
		{
			{ vertex2( x, y + height), col, { 0, 0 } },
			{ vertex2( x + width, y), col,  { 0, 0 } },
			{ vertex2( x + width, y + height ), col, { 0, 0 } }
		};
		triangles[1] = triangle1;
		m_nBufferCount += vertex_count;
		m_bDirty = true;
	}
	else if( mpFillStyle->mFillType == FILL_PATTERN )
	{
		glUniform1i( (GLint)gUniforms[kCCuniformDrawType], FILL_PATTERN );
		kmMat4 texMat = 
		{
			mpFillStyle->mpPattern->width, 0, 0, 0,
			0, mpFillStyle->mpPattern->height, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		kmMat4 texMatIn;
		kmMat4 rotaMat;
		kmMat4 tranMat;
		kmMat4Identity( &rotaMat );
		kmMat4RotationZ( &rotaMat, -60 );
		kmMat4Identity( &texMatIn );
		kmMat4Inverse( &texMatIn, &texMat );
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpPattern->texId );
		if ( mpFillStyle->mpPattern->mRepeatePat == en_REPEAT )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}
		else if ( mpFillStyle->mpPattern->mRepeatePat == en_REPEAT_X )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		}
		else if( mpFillStyle->mpPattern->mRepeatePat == en_REPEAT_Y )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		}
		else if( mpFillStyle->mpPattern->mRepeatePat == en_NO_REPEAT )
		{
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
		}

		//kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
		glUniformMatrix4fv( gUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat );
		unsigned int vertex_count = 2 * 6;
		ensureCapacity( vertex_count );
		ccColor4B col = ccc4BFromccc4F( mpFillStyle->mColor );
		ccV2F_C4B_T2F_Triangle triangle =
		{
			{ vertex2( x, y + height), col, {0, 0 } },
			{ vertex2( x, y), col, {0, height / mpFillStyle->mpPattern->height } },
			{ vertex2( x + width, y + height ), col, { width / mpFillStyle->mpPattern->width, 0 }  },
		};
		ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );

		triangles[0] = triangle;
		ccV2F_C4B_T2F_Triangle triangle1 =
		{
			{ vertex2( x, y), col, { 0, height / mpFillStyle->mpPattern->height } },
			{ vertex2( x + width, y + height), col,  { width / mpFillStyle->mpPattern->width, 0 } },
			{ vertex2( x + width, y ), col, { width / mpFillStyle->mpPattern->width, height / mpFillStyle->mpPattern->height } }
		};
		triangles[1] = triangle1;
		m_nBufferCount += vertex_count;
		m_bDirty = true;
	}
	else if ( mpFillStyle->mFillType == FILL_Gradient_Line )
	{
		mpFillStyle->mpGradientLinear->CreateTextrue();

		glUniform1i( (GLint)gUniforms[kCCuniformDrawType], FILL_Gradient_Line );
		kmMat4 texMat = 
		{
			mpFillStyle->mpGradientLinear->miLen, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			mpFillStyle->mpGradientLinear->x, 0, 0, 1
		};
		kmMat4 texMatIn;
		kmMat4 rotaMat;
		kmMat4 tranMat;
		kmMat4Identity( &rotaMat ); 
		kmMat4RotationZ( &rotaMat, -mpFillStyle->mpGradientLinear->angle );
		kmMat4Identity( &texMatIn );
		kmMat4Inverse( &texMatIn, &texMat );
		kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
		//texMatIn.mat[12] = mpFillStyle->mpGradientLinear->x;
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpGradientLinear->texId );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

		//kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
		glUniformMatrix4fv( gUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat );
		unsigned int vertex_count = 2 * 6;
		ensureCapacity( vertex_count );
		ccColor4B col = ccc4BFromccc4F( mpFillStyle->mColor );
		ccV2F_C4B_T2F_Triangle triangle =
		{
			{ vertex2( x, y + height), col, {0, 0 } },
			{ vertex2( x, y), col, {0, 0 } },
			{ vertex2( x + width, y + height ), col, {0, 0 }  },
		};
		ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );

		triangles[0] = triangle;
		ccV2F_C4B_T2F_Triangle triangle1 =
		{
			{ vertex2( x, y), col, {0, 0 } },
			{ vertex2( x + width, y + height), col,  {0, 0 } },
			{ vertex2( x + width, y ), col, {0, 0 } }
		};
		triangles[1] = triangle1;
		m_nBufferCount += vertex_count;
		m_bDirty = true;

	}
	else if ( mpFillStyle->mFillType == FILL_Gradient_radius )
	{
		mpFillStyle->mpGradientLinear->CreateTextrue();

		glUniform1i( (GLint)gUniforms[kCCuniformDrawType], FILL_Gradient_radius );
		kmMat4 texMat = 
		{
			mpFillStyle->mpGradientLinear->miLen, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
		};
		kmMat4 texMatIn;
		kmMat4 rotaMat;
		kmMat4 tranMat;
		kmMat4Identity( &rotaMat ); 
		kmMat4Identity( &texMatIn );
		kmMat4Inverse( &texMatIn, &texMat );
		//texMatIn.mat[12] = mpFillStyle->mpGradientLinear->x;
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpGradientRadial->texId );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		//kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );
		glUniformMatrix4fv( gUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat );
		unsigned int vertex_count = 2 * 6;
		ensureCapacity( vertex_count );
		ccColor4B col = ccc4BFromccc4F( mpFillStyle->mColor );
		float rx = mpFillStyle->mpGradientRadial->x;
		float ry = mpFillStyle->mpGradientRadial->y;
		float r = mpFillStyle->mpGradientRadial->miLen;

		ccTex2F lt = { (x - rx) / r, (y - ry ) / r };
		ccTex2F lb = { (x - rx) / r, (y + height -ry ) / r};
		ccTex2F rt = { (x + width -rx ) / r, ( y - ry ) / r};
		ccTex2F rb = { (x + width -rx ) / r, ( y + height - ry ) / r };
		ccV2F_C4B_T2F_Triangle triangle =
		{
			{ vertex2( x, y + height), col, lb },
			{ vertex2( x, y), col, lt },
			{ vertex2( x + width, y + height ), col, rb  },
		};
		ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );

		triangles[0] = triangle;
		ccV2F_C4B_T2F_Triangle triangle1 =
		{
			{ vertex2( x, y), col, lt },
			{ vertex2( x + width, y + height), col,  rb },
			{ vertex2( x + width, y ), col, rt }
		};
		triangles[1] = triangle1;
		m_nBufferCount += vertex_count;
		m_bDirty = true;

	}
	return;
}

void XContext::strokeRect( float x, float y, float width, float height )
{
	CCPoint tl( x, y );
	CCPoint tr( x + width, y );
	CCPoint bl( x, y + height );
	CCPoint br( x + width, y + height );
	drawSegment( tl, tr, pCurPath->mLineWidth, mpFillStyle->mColor );
	drawSegment( tr, br, pCurPath->mLineWidth, mpFillStyle->mColor );
	drawSegment( br, bl, pCurPath->mLineWidth, mpFillStyle->mColor );
	drawSegment( bl, tl, pCurPath->mLineWidth, mpFillStyle->mColor );
	return;
}

void XContext::clearRect( float x, float y, float width, float height )
{
	unsigned int vertex_count = 2 * 6;
	ensureCapacity( vertex_count );
	ccV2F_C4B_T2F_Triangle triangle =
	{
		{ vertex2( x, y), mbgcolor, __t( v2fzero) },
		{ vertex2( x + width, y), mbgcolor, __t( v2fzero ) },
		{ vertex2( x, y + height), mbgcolor, __t( v2fzero ) }
	};
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
	//ccV2F_C4B_T2F_Triangle triangle = { a, b, c };
	triangles[0] = triangle;
	ccV2F_C4B_T2F_Triangle triangle1 =
	{
		{ vertex2( x + width, y), mbgcolor, __t( v2fzero) },
		{ vertex2( x, y + height), mbgcolor, __t( v2fzero ) },
		{ vertex2( x + width, y + height ), mbgcolor, __t( v2fzero ) }
	};
	triangles[1] = triangle1;
	m_nBufferCount += vertex_count;
	m_bDirty = true;
}

void XContext::clip()
{
	//暂时不实现
	//glEnable( GL_SCISSOR_TEST );
	//glScissor( pCurPath->startx, mHeight - pCurPath->pEndEdge->endy, 
	//	pCurPath->pEndEdge->endx - pCurPath->startx, pCurPath->pEndEdge->endy - pCurPath->starty);
}

void XContext::bezierCurveTo( float cp1x, float cp1y, float cp2x, float cp2y, float x, float y )
{
	pCurPath->cmdType = CTX_BEZIERCURVETO;
	if ( pCurPath->pEdges == NULL )
	{
		pCurPath->pEdges = new EgEdge();
		memset( pCurPath->pEdges, 0, sizeof( EgEdge ));
		pCurPath->pEndEdge = pCurPath->pEdges;
		pCurPath->pEndEdge->cpx = cp1x;
		pCurPath->pEndEdge->cpy = cp1y;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = false;

		EgEdge *p = new EgEdge();
		memset( p, 0, sizeof( EgEdge ));
		pCurPath->pEndEdge->pNext = p;
		pCurPath->pEndEdge = p;
		pCurPath->pEndEdge->cpx = cp2x;
		pCurPath->pEndEdge->cpy = cp2y;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = false;
	}
	else
	{
		EgEdge *p = new EgEdge();
		pCurPath->pEndEdge->pNext = p;
		pCurPath->pEndEdge = p;
		memset( pCurPath->pEndEdge, 0, sizeof( EgEdge ) );
		pCurPath->pEndEdge->cpx = cp1x;
		pCurPath->pEndEdge->cpy = cp1y;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = false;

		EgEdge *p1 = new EgEdge();
		memset( p1, 0, sizeof( EgEdge ));
		pCurPath->pEndEdge->pNext = p1;
		pCurPath->pEndEdge = p1;
		pCurPath->pEndEdge->cpx = cp2x;
		pCurPath->pEndEdge->cpy = cp2y;
		pCurPath->pEndEdge->endx = x;
		pCurPath->pEndEdge->endy = y;
		pCurPath->pEndEdge->isLine = false;
	}
	return;
}

void XContext::arcTo( float x1, float y1, float x2, float y2, float r )
{
	// 暂不实现
}

bool XContext::isPointInPath( float x, float y )
{
	//暂不实现
	return false;
}

XGradientLinear *XContext::CreateLinearGradient( float x1, float y1, float x2, float y2 )
{
	XGradientLinear *p = new XGradientLinear();
	memset( p, 0, sizeof( XGradientLinear ) );
	float flen = sqrt( (x2-x1) * (x2 - x1) + (y2 - y1) * ( y2 - y1 ));
	p->miLen = ceil( flen );
	p->x = x1;
	p->angle = atan2( ( y2 - y1), (x2 - x1 ) );// * 180 / 3.1415926;
	return p;
}

XPattern *XContext::CreatePattern( GLuint texId, REPEAT_PAT repat)
{
	XPattern *p = new XPattern();
	p->texId = texId;
	p->mRepeatePat = en_REPEAT;
	return p;
}
XGradientRadial *XContext::CreateRadialGradient( float xStart, float ySttart, float radiusStart, 
										  float xEnd, float yEnd, float radiusEnd )
{
	XGradientRadial *p = new XGradientRadial();
	memset( p, 0, sizeof( XGradientRadial ) );
	p->mbDirty = false;
	p->pGraData = new GradientData();
	p->pGraData->index = radiusStart;
	p->x = xStart;
	p->y = ySttart;
	p->miLen = radiusEnd;
	return p;
}

void XContext::DrawCommand()
{
	EgPath *pTmpPath = mEgPaths;
	while( pTmpPath )
	{
		if ( pTmpPath->cmdType == CTX_LINETO )
		{
			CCPoint from( pTmpPath->startx, pTmpPath->starty );	
			EgEdge *p = pTmpPath->pEdges;
			ccColor4F color = {1.0, 0, 0, 1 };
			while( p )
			{
				CCPoint to( p->endx, p->endy );
				drawSegment( from, to, pTmpPath->mLineWidth, mpFillStyle->mColor );
				from.x = to.x;
				from.y = to.y;
				p = p->pNext;
			}
		}
		else if( pTmpPath->cmdType == CTX_RECT )
		{
			EgEdge *pEdge = pTmpPath->pEdges;
			unsigned int vertex_cout = 2 * 6;
			ensureCapacity( vertex_cout );
			ccColor4F color = { 1.0, 0, 0, 1.0 };
			ccColor4B col = ccc4BFromccc4F( color );
			ccV2F_C4B_T2F_Triangle triangle =
			{
				{ vertex2( pTmpPath->startx,  pTmpPath->starty), col, __t( v2fzero) },
				{ vertex2( pEdge->endx, pEdge->endy ), col, __t( v2fzero ) },
				{ vertex2( pEdge->pNext->endx, pEdge->pNext->endy ), col, __t( v2fzero ) }
			};
			ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
			//ccV2F_C4B_T2F_Triangle triangle = { a, b, c };
			triangles[0] = triangle;
			ccV2F_C4B_T2F_Triangle triangle1 =
			{
				{ vertex2( pEdge->endx, pEdge->endy ), col, __t( v2fzero) },
				{ vertex2( pEdge->pNext->endx, pEdge->pNext->endy ), col, __t( v2fzero ) },
				{ vertex2( pEdge->pNext->pNext->endx, pEdge->pNext->pNext->endy ), col, __t( v2fzero ) }
			};
			triangles[1] = triangle1;
			m_nBufferCount += vertex_cout;
			m_bDirty = true;
		}
		else if( pTmpPath->cmdType == CTX_ARC )
		{
			unsigned int vertex_count = 3 * pTmpPath->count;
			ensureCapacity( vertex_count );
			EgEdge *p = pTmpPath->pEdges;
			int icout = 0;
			ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
			ccColor4F color = { 1.0, 0, 0, 1.0 };
			ccColor4B col = ccc4BFromccc4F( color );
			while( p != NULL && p->pNext != NULL )
			{
				ccV2F_C4B_T2F_Triangle triangle =
				{
					{ vertex2( pTmpPath->startx,  pTmpPath->starty), col, __t( v2fzero) },
					{ vertex2( p->endx, p->endy ), col, __t( v2fzero ) },
					{ vertex2( p->pNext->endx, p->pNext->endy ), col, __t( v2fzero ) }
				};
				triangles[icout] = triangle;
				icout++;
				//char chTmp[256];
				//sprintf( chTmp, "-->%d, x:%f, y:%f, xx:%f, yy:%f", icout, p->endx, p->endy,
				//	p->pNext->endx, p->pNext->endy );
				//OutputDebugStringA( chTmp );
				p = p->pNext;
			}
			m_nBufferCount += vertex_count;
			m_bDirty = true;
		}
		else if( pTmpPath->cmdType == CTX_QUADRATICCURVETO )
		{
			CCPoint from( pTmpPath->startx, pTmpPath->starty );	
			EgEdge *p = pTmpPath->pEdges;
			ccColor4F color = {1.0, 0, 0, 1 };

			float t = 0.0f;
			int segments = 60;
			float x = pTmpPath->startx;
			float y = pTmpPath->starty;
			for ( int i = 0; i < segments; i++ )
			{
				float x1 = powf(1 - t, 2) * pTmpPath->startx + 2.0f * (1 - t) * t * p->cpx + t * t * p->endx;
				float y1 = powf(1 - t, 2 ) *pTmpPath->starty + 2.0f * (1 - t) * t * p->cpy + t * t * p->endy;
				t += 1.0f / segments;
				CCPoint from( x, y );
				CCPoint to( x1, y1 );
				drawSegment( from, to, pTmpPath->mLineWidth, mpFillStyle->mColor );
				char chTmp[256];
				sprintf( chTmp, "-->x1:%f, y1:%f, x2:%f, y2:%f", x, y, x1, y1 );
				OutputDebugStringA( chTmp );
				x = x1;
				y = y1;
			}
		}
		else if( pCurPath->cmdType == CTX_BEZIERCURVETO )
		{
			CCPoint from( pTmpPath->startx, pTmpPath->starty );	
			EgEdge *p = pTmpPath->pEdges;
			ccColor4F color = {1.0, 0, 0, 1 };

			float t = 0.0f;
			int segments = 60;
			float x = pTmpPath->startx;
			float y = pTmpPath->starty;
			float cp1x = pTmpPath->pEdges->cpx;
			float cp1y = pTmpPath->pEdges->cpy;
			float cp2x = pTmpPath->pEdges->pNext->cpx;
			float cp2y = pTmpPath->pEdges->pNext->cpy;
			float endx = pTmpPath->pEdges->endx;
			float endy = pTmpPath->pEdges->endy;
			for ( int i = 0; i < segments; i++ )
			{
				float x1 = powf(1 - t, 3) * pTmpPath->startx + 3.0f * powf(1 - t, 2) * t * cp1x + 3.0f * (1 - t) * t * t * cp2x + t * t * t * endx;
				float y1 = powf(1 - t, 3) * pTmpPath->starty + 3.0f * powf(1 - t, 2) * t * cp1y + 3.0f * (1 - t) * t * t * cp2y + t * t * t * endy;
				t += 1.0f / segments;
				CCPoint from( x, y );
				CCPoint to( x1, y1 );
				drawSegment( from, to, pTmpPath->mLineWidth, mpFillStyle->mColor );
				x = x1;
				y = y1;
			}
		}
		pTmpPath = pTmpPath->pNext;
	}
	return;
}

XContext* XContext::create()
{
    XContext* pRet = new XContext();
    pRet->init();
	pRet->initTest();
    return pRet;
}

void XContext::ensureCapacity(unsigned int count)
{
    if(m_nBufferCount + count > m_uBufferCapacity)
    {
		m_uBufferCapacity += MAX(m_uBufferCapacity, count);
		m_pBuffer = (ccV2F_C4B_T2F*)realloc(m_pBuffer, m_uBufferCapacity*sizeof(ccV2F_C4B_T2F));
	}
}



bool XContext::init()
{

    m_sBlendFunc.src = CC_BLEND_SRC;
    m_sBlendFunc.dst = CC_BLEND_DST;

	mpFillStyle = new XFillStyle();

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


void XContext::initTest()
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
	pattern->mRepeatePat = en_REPEAT;
	mpFillStyle->setFillType( pattern );

}


unsigned char* XContext::DecodePngData(unsigned char* fData, long fSize, int& width, int& height)
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

GLuint XContext::initTexData( const void *pData, int width, int height )
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

void XContext::testDrawTex()
{
	glUniform1i( (GLint)gUniforms[kCCuniformDrawType], 1 );
	GLfloat verts[4][9] = 
	{
		{0.0f,  256.0f,0.0f,	0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f},
		{0.0f,  0.0f, 0.0f,		0.0f, 1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{256.0f, 256.0f,0.0f,	1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f},
		{256.0f, 00.0f, 0.0f,	1.0f, 1.0f,	0.0f, 1.0f, 0.0f, 1.0f},
	};

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpPattern->texId );
	//int ilen = sizeof(ccV2F_C4F_T2F);
	glEnableVertexAttribArray( kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &verts[0][0] );

	//color
	glEnableVertexAttribArray( kCCVertexAttrib_Color );
	glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), &verts[0][5]);
	
	glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  9 * sizeof(GLfloat), &verts[0][3] );
	
	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	return;
}

void XContext::testDrawTexWithMatixCoord()
{
	glUniform1i( (GLint)gUniforms[kCCuniformDrawType], 4 );

	kmMat4 texMat = 
	{
		64, 0, 0, 0,
		0, 64, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};
	kmMat4 texMatIn;
	kmMat4 rotaMat;
	kmMat4 tranMat;
	kmMat4Identity( &rotaMat );
	kmMat4RotationX( &rotaMat, 3.14 );
	kmMat4Identity( &texMatIn );
	kmMat4Inverse( &texMatIn, &texMat );
	//kmMat4Multiply( &texMatIn, &texMatIn, &rotaMat );

	kmVec3 in = { 64.0f, 64.0f, 0.0f };
	kmVec3 out;
	kmVec3Transform( &out, &in, &texMatIn );

	glUniformMatrix4fv( gUniforms[kCCUniformTexMatrix], (GLsizei)1, GL_FALSE, texMatIn.mat );
	GLfloat verts[4][9] = 
	{
		{0.0f,  64.0f,0.0f,	0.0f, 0.0f,	0.0f, 0.0f, 1.0f, 1.0f},
		{0.0f,  0.0f, 0.0f,	0.0f, 1.0f,	1.0f, 0.0f, 0.0f, 1.0f},
		{64.0f, 64.0f,0.0f,	1.0f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f},
		{64.0f, 0.0f, 0.0f,	1.0f, 1.0f,	0.0f, 1.0f, 0.0f, 1.0f},
	};

	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpPattern->texId );
	//int ilen = sizeof(ccV2F_C4F_T2F);
	glEnableVertexAttribArray( kCCVertexAttrib_Position);
	glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE,  9 * sizeof( GLfloat), &verts[0][0] );

	//color
	//glEnableVertexAttribArray( kCCVertexAttrib_Color );
	//glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 9*sizeof(GLfloat), &verts[0][5]);

	//glEnableVertexAttribArray( kCCVertexAttrib_TexCoords );
	//glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE,  9 * sizeof(GLfloat), &verts[0][3] );

	glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
	return;
}

void XContext::render()
{
    //if (m_bDirty)
    //{
    //    glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    //    glBufferData(GL_ARRAY_BUFFER, sizeof(ccV2F_C4B_T2F)*m_uBufferCapacity, m_pBuffer, GL_STREAM_DRAW);
    //    m_bDirty = false;
    //}

	if ( mpFillStyle->mFillType == FILL_COLOR )
	{
		glBindBuffer( GL_ARRAY_BUFFER, pCurPath->muVbo );
		glEnableVertexAttribArray(kCCVertexAttrib_Position);
		glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
		glDrawArrays(GL_TRIANGLES, 0, pCurPath->pointCount );
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableVertexAttribArray( kCCVertexAttrib_Position );
	}

	if ( mpFillStyle->mFillType == FILL_PATTERN )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpPattern->texId );
		glEnableVertexAttribArray( kCCVertexAttrib_Position );
		glVertexAttribPointer( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof( ccV2F_C4B_T2F),( GLvoid*)offsetof(ccV2F_C4B_T2F, vertices));
		glDrawArrays( GL_TRIANGLES, 0, m_nBufferCount );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glDisableVertexAttribArray( kCCVertexAttrib_Position );
	}
	if ( mpFillStyle->mFillType == FILL_Gradient_Line )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpGradientLinear->texId );
		glEnableVertexAttribArray( kCCVertexAttrib_Position );
		glVertexAttribPointer( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof( ccV2F_C4B_T2F),( GLvoid*)offsetof(ccV2F_C4B_T2F, vertices));
		glDrawArrays( GL_TRIANGLES, 0, m_nBufferCount );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glDisableVertexAttribArray( kCCVertexAttrib_Position );
	}
	if ( mpFillStyle->mFillType == FILL_Gradient_radius )
	{
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, mpFillStyle->mpGradientRadial->texId );
		glEnableVertexAttribArray( kCCVertexAttrib_Position );
		glVertexAttribPointer( kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof( ccV2F_C4B_T2F),( GLvoid*)offsetof(ccV2F_C4B_T2F, vertices));
		glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
		glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));

		glDrawArrays( GL_TRIANGLES, 0, m_nBufferCount );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glDisableVertexAttribArray( kCCVertexAttrib_Position );
		glDisableVertexAttribArray( kCCVertexAttrib_TexCoords );
	}

    //ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    //glBindBuffer(GL_ARRAY_BUFFER, m_uVbo);
    // vertex
	
    //glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, vertices));
    
    // glEnableVertexAttribArray(kCCVertexAttrib_Color);
    //glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, colors));
    
    // glEnableVertexAttribArray(kCCVertexAttrib_TexCoords);
    //glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, sizeof(ccV2F_C4B_T2F), (GLvoid *)offsetof(ccV2F_C4B_T2F, texCoords));

  
    
    //CC_INCREMENT_GL_DRAWS(1);
}

void XContext::draw()
{
    ccGLBlendFunc(m_sBlendFunc.src, m_sBlendFunc.dst);
    
    //getShaderProgram()->use();
	mProgram->use();
	mProgram->setMatrixValue();
    //getShaderProgram()->setUniformsForBuiltins();

	//testDrawTexWithMatixCoord();
	//return;
    
    render();
}

void XContext::drawDot(const CCPoint &pos, float radius, const ccColor4F &color)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
	
	ccV2F_C4B_T2F a = {{pos.x - radius, pos.y - radius}, ccc4BFromccc4F(color), {-1.0, -1.0} };
	ccV2F_C4B_T2F b = {{pos.x - radius, pos.y + radius}, ccc4BFromccc4F(color), {-1.0,  1.0} };
	ccV2F_C4B_T2F c = {{pos.x + radius, pos.y + radius}, ccc4BFromccc4F(color), { 1.0,  1.0} };
	ccV2F_C4B_T2F d = {{pos.x + radius, pos.y - radius}, ccc4BFromccc4F(color), { 1.0, -1.0} };
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
    ccV2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    ccV2F_C4B_T2F_Triangle triangle1 = {a, c, d};
	triangles[0] = triangle0;
	triangles[1] = triangle1;
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;
}

void XContext::drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color)
{
    unsigned int vertex_count = 10*3;
    ensureCapacity(vertex_count);
	
	ccVertex2F a = __v2f(from);
	ccVertex2F b = __v2f(to);
	
	
	ccVertex2F n = v2fnormalize(v2fperp(v2fsub(b, a)));
	ccVertex2F t = v2fperp(n);
	
	ccVertex2F nw = v2fmult(n, radius);
	ccVertex2F tw = v2fmult(t, radius);
	ccVertex2F v0 = v2fsub(b, v2fadd(nw, tw));
	ccVertex2F v1 = v2fadd(b, v2fsub(nw, tw));
	ccVertex2F v2 = v2fsub(b, nw);
	ccVertex2F v3 = v2fadd(b, nw);
	ccVertex2F v4 = v2fsub(a, nw);
	ccVertex2F v5 = v2fadd(a, nw);
	ccVertex2F v6 = v2fsub(a, v2fsub(nw, tw));
	ccVertex2F v7 = v2fadd(a, v2fadd(nw, tw));
	
	
	//ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *) gsGLData; //(m_pBuffer + m_nBufferCount);
	
    ccV2F_C4B_T2F_Triangle triangles0 = {
        {v0, ccc4BFromccc4F(color), __t(v2fneg(v2fadd(n, t)))},
        {v1, ccc4BFromccc4F(color), __t(v2fsub(n, t))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[0] = triangles0;
	
    ccV2F_C4B_T2F_Triangle triangles1 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v1, ccc4BFromccc4F(color), __t(v2fsub(n, t))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[1] = triangles1;
	
    ccV2F_C4B_T2F_Triangle triangles2 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n))},
        {v2, ccc4BFromccc4F(color), __t(v2fneg(n))},
    };
	triangles[2] = triangles2;

    ccV2F_C4B_T2F_Triangle triangles3 = {
        {v3, ccc4BFromccc4F(color), __t(n)},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n))},
        {v5, ccc4BFromccc4F(color), __t(n) },
    };
    triangles[3] = triangles3;

    ccV2F_C4B_T2F_Triangle triangles4 = {
        {v6, ccc4BFromccc4F(color), __t(v2fsub(t, n))},
        {v4, ccc4BFromccc4F(color), __t(v2fneg(n)) },
        {v5, ccc4BFromccc4F(color), __t(n)},
    };
	triangles[4] = triangles4;

    ccV2F_C4B_T2F_Triangle triangles5 = {
        {v6, ccc4BFromccc4F(color), __t(v2fsub(t, n))},
        {v7, ccc4BFromccc4F(color), __t(v2fadd(n, t))},
        {v5, ccc4BFromccc4F(color), __t(n)},
    };
	triangles[5] = triangles5;
	
	//m_nBufferCount += vertex_count;
	
	m_bDirty = true;
}

 struct ExtrudeVerts {ccVertex2F offset, n;};

void XContext::drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor)
{
	struct ExtrudeVerts* extrude = (struct ExtrudeVerts*)malloc(sizeof(struct ExtrudeVerts)*count);
	memset(extrude, 0, sizeof(struct ExtrudeVerts)*count);
	
	for(unsigned int i = 0; i < count; i++)
    {  
		ccVertex2F v0 = __v2f(verts[(i-1+count)%count]);
		ccVertex2F v1 = __v2f(verts[i]);
		ccVertex2F v2 = __v2f(verts[(i+1)%count]);
        
		ccVertex2F n1 = v2fnormalize(v2fperp(v2fsub(v1, v0)));
		ccVertex2F n2 = v2fnormalize(v2fperp(v2fsub(v2, v1)));
		
		ccVertex2F offset = v2fmult(v2fadd(n1, n2), 1.0/(v2fdot(n1, n2) + 1.0));
        struct ExtrudeVerts tmp = {offset, n2};
		extrude[i] = tmp;
	}
	
	bool outline = (fillColor.a > 0.0 && borderWidth > 0.0);
	
	unsigned int triangle_count = 3*count - 2;
	unsigned int vertex_count = 3*triangle_count;
    ensureCapacity(vertex_count);
	
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle *)(m_pBuffer + m_nBufferCount);
	ccV2F_C4B_T2F_Triangle *cursor = triangles;
	
	float inset = (outline == 0.0 ? 0.5 : 0.0);
	for(unsigned int i = 0; i < count-2; i++)
    {
		ccVertex2F v0 = v2fsub(__v2f(verts[0  ]), v2fmult(extrude[0  ].offset, inset));
		ccVertex2F v1 = v2fsub(__v2f(verts[i+1]), v2fmult(extrude[i+1].offset, inset));
		ccVertex2F v2 = v2fsub(__v2f(verts[i+2]), v2fmult(extrude[i+2].offset, inset));
		
        ccV2F_C4B_T2F_Triangle tmp = {
            {v0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
            {v1, ccc4BFromccc4F(fillColor), __t(v2fzero)},
            {v2, ccc4BFromccc4F(fillColor), __t(v2fzero)},
        };

		*cursor++ = tmp;
	}
	
	for(unsigned int i = 0; i < count; i++)
    {
		int j = (i+1)%count;
		ccVertex2F v0 = __v2f(verts[i]);
		ccVertex2F v1 = __v2f(verts[j]);
		
		ccVertex2F n0 = extrude[i].n;
		
		ccVertex2F offset0 = extrude[i].offset;
		ccVertex2F offset1 = extrude[j].offset;
		
		if(outline)
        {
			ccVertex2F inner0 = v2fsub(v0, v2fmult(offset0, borderWidth));
			ccVertex2F inner1 = v2fsub(v1, v2fmult(offset1, borderWidth));
			ccVertex2F outer0 = v2fadd(v0, v2fmult(offset0, borderWidth));
			ccVertex2F outer1 = v2fadd(v1, v2fmult(offset1, borderWidth));
			
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {inner1, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {outer1, ccc4BFromccc4F(borderColor), __t(n0)}
            };
			*cursor++ = tmp1;

            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, ccc4BFromccc4F(borderColor), __t(v2fneg(n0))},
                {outer0, ccc4BFromccc4F(borderColor), __t(n0)},
                {outer1, ccc4BFromccc4F(borderColor), __t(n0)}
            };
			*cursor++ = tmp2;
		}
        else {
			ccVertex2F inner0 = v2fsub(v0, v2fmult(offset0, 0.5));
			ccVertex2F inner1 = v2fsub(v1, v2fmult(offset1, 0.5));
			ccVertex2F outer0 = v2fadd(v0, v2fmult(offset0, 0.5));
			ccVertex2F outer1 = v2fadd(v1, v2fmult(offset1, 0.5));
			
            ccV2F_C4B_T2F_Triangle tmp1 = {
                {inner0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {inner1, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {outer1, ccc4BFromccc4F(fillColor), __t(n0)}
            };
			*cursor++ = tmp1;

            ccV2F_C4B_T2F_Triangle tmp2 = {
                {inner0, ccc4BFromccc4F(fillColor), __t(v2fzero)},
                {outer0, ccc4BFromccc4F(fillColor), __t(n0)},
                {outer1, ccc4BFromccc4F(fillColor), __t(n0)}
            };
			*cursor++ = tmp2;
		}
	}
	
	m_nBufferCount += vertex_count;
	
	m_bDirty = true;

    free(extrude);
}

void XContext::drawTriangle( const CCPoint &p1, const CCPoint &p2, const CCPoint &p3, const ccColor4F &color )
{
	unsigned int vertex_count = 2 * 3;
	ensureCapacity( vertex_count );
	ccColor4B col = ccc4BFromccc4F( color );
	ccV2F_C4B_T2F_Triangle triangle =
	{
		{ vertex2( p1.x, p1.y), col, __t( v2fzero) },
		{ vertex2( p2.x, p2.y), col, __t( v2fzero ) },
		{ vertex2( p3.x, p3.y), col, __t( v2fzero ) }
	};
	ccV2F_C4B_T2F_Triangle *triangles = (ccV2F_C4B_T2F_Triangle*)( m_pBuffer + m_nBufferCount );
	//ccV2F_C4B_T2F_Triangle triangle = { a, b, c };
	triangles[0] = triangle;
	m_nBufferCount += vertex_count;
	m_bDirty = true;
	return;
}

void XContext::clear()
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



ccBlendFunc XContext::getBlendFunc() const
{
    return m_sBlendFunc;
}

void XContext::setBlendFunc(const ccBlendFunc &blendFunc)
{
    m_sBlendFunc = blendFunc;
}
