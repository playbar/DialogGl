#include "stdafx.h"
#include "BwGLUtility.h"
#include <math.h>

GLuint		gProObject;
ESMatrix 	gOrthographic;
ESMatrix 	gMatrix;

GLint	gShaderPosition;
GLint	gShaderTexCoord;
GLint 	gShaderImgTexture;
GLint	gvpMatrix;

GLint 	gColorFlag;
GLint 	gColorLoc;

GLuint	gTextureTmp;
GLuint	gMainTexture;
GLuint	gFrameBuffer;
int 	gWidth;
int		gHeight;

HGLRC gContext = NULL;
HDC gDisplay = NULL;



void GLMatrixIdentity(ESMatrix *result)
{
	memset(result, 0x0, sizeof(ESMatrix));
	result->m[0][0] = 1.0f;
	result->m[1][1] = 1.0f;
	result->m[2][2] = 1.0f;
	result->m[3][3] = 1.0f;
}


void GLOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ)
{
	float       deltaX = right - left;
	float       deltaY = top - bottom;
	float       deltaZ = farZ - nearZ;
	ESMatrix    ortho;

	if ( (deltaX == 0.0f) || (deltaY == 0.0f) || (deltaZ == 0.0f) )
		return;

	GLMatrixIdentity(&ortho);
	ortho.m[0][0] = 2.0f / deltaX;
	ortho.m[3][0] = -(right + left) / deltaX;
	ortho.m[1][1] = 2.0f / deltaY;
	ortho.m[3][1] = -(top + bottom) / deltaY;
	ortho.m[2][2] = -2.0f / deltaZ;
	ortho.m[3][2] = -(nearZ + farZ) / deltaZ;

	GLMatrixMultiply(result, &ortho, result);
}


void GLMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB)
{
	ESMatrix    tmp;
	int         i;

	for (i=0; i<4; i++)
	{
		tmp.m[i][0] =	(srcA->m[i][0] * srcB->m[0][0]) +
			(srcA->m[i][1] * srcB->m[1][0]) +
			(srcA->m[i][2] * srcB->m[2][0]) +
			(srcA->m[i][3] * srcB->m[3][0]) ;

		tmp.m[i][1] =	(srcA->m[i][0] * srcB->m[0][1]) +
			(srcA->m[i][1] * srcB->m[1][1]) +
			(srcA->m[i][2] * srcB->m[2][1]) +
			(srcA->m[i][3] * srcB->m[3][1]) ;

		tmp.m[i][2] =	(srcA->m[i][0] * srcB->m[0][2]) +
			(srcA->m[i][1] * srcB->m[1][2]) +
			(srcA->m[i][2] * srcB->m[2][2]) +
			(srcA->m[i][3] * srcB->m[3][2]) ;

		tmp.m[i][3] =	(srcA->m[i][0] * srcB->m[0][3]) +
			(srcA->m[i][1] * srcB->m[1][3]) +
			(srcA->m[i][2] * srcB->m[2][3]) +
			(srcA->m[i][3] * srcB->m[3][3]) ;
	}
	memcpy(result, &tmp, sizeof(ESMatrix));
}

bool GLCreateHWND(HWND& hWnd,int stencil)
{
	HDC hDC = ::GetDC(hWnd);

	if (!GLCreateHDC(hDC,stencil))
	{
		::ReleaseDC(hWnd, hDC);

		return false;
	}
	::ReleaseDC(hWnd, hDC);
	return true;
}

bool GLCreateHDC(HDC& hDC,int stencil)
{
	if (!hDC)
	{
		return false;
	}
	if (!GLSetDCPixelFormat(hDC,stencil))
	{
		return false;
	}
	gDisplay = hDC;
	gContext = wglCreateContext(gDisplay);
	wglMakeCurrent(gDisplay, gContext);
	
	return true;
}

void GLDestroy()
{
	if (gContext != NULL)
	{
		wglMakeCurrent(NULL,NULL);
		wglDeleteContext(gContext);
	}
}

void GLMakeCurrent(bool disableOther)
{
	if (gContext != NULL)
	{
		if (disableOther) 
			wglMakeCurrent(NULL,NULL);
		wglMakeCurrent(gDisplay, gContext);
	}
}

bool GLSetDCPixelFormat(HDC& hDC,int stencil)
{
	int pixelFormat;
	DEVMODE resolution;
	char text[256];
	//PIXELFORMAT->BPP = DESKTOP->BPP
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &resolution);

	static PIXELFORMATDESCRIPTOR pixelFormatDesc =
	{
		sizeof (PIXELFORMATDESCRIPTOR),             
		1,                                          
		PFD_DRAW_TO_WINDOW |                        
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,                              
		resolution.dmBitsPerPel,                                         
		0, 0, 0, 0, 0, 0,                           
		0, 
		0,                                       
		0, 
		0, 0, 0, 0,                              
		16,                                         
		stencil,                                          
		0,                                          
		0,                             
		0,                                          
		0, 0, 0                                     
	};
	pixelFormat = ChoosePixelFormat (hDC,
		&pixelFormatDesc);
	if (!SetPixelFormat(hDC, pixelFormat, 
		&pixelFormatDesc)) 
	{
		return false ;
	}
	return true;
}


GLint CreateGLEnv( )
{
	static char vShaderStr[] =
		"uniform mat4 u_TransMatrix;					\n"
		"attribute vec4 a_position;   					\n"
		"attribute vec2 a_texCoord;   					\n"
		"varying vec2 v_texCoord;    	 				\n"
		"void main()                  					\n"
		"{                            					\n"
		"   gl_Position = u_TransMatrix * a_position; 	\n"
		"   v_texCoord = a_texCoord;  					\n"
		"}                            					\n";

	static char fShaderStr[] =
		"varying vec2 v_texCoord;    	 							\n"
		"uniform sampler2D s_ImgTexture;                     		\n"
		"uniform int specColorFlag; 						   		\n"
		"uniform vec4 specColor; 							   		\n"
		"void main()                                         		\n"
		"{                                                   		\n"
		//"	   gl_FragColor = texture2D( s_ImgTexture, v_texCoord );\n"
		" if (specColorFlag == 0) 								\n"
		" {gl_FragColor = texture2D( s_ImgTexture, v_texCoord );} \n"
		" else {gl_FragColor = specColor;} 						\n"
		"}                                                   		\n";

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = GLLoadShader ( GL_VERTEX_SHADER, vShaderStr );
	if ( vertexShader == 0 )
		return 0;

	fragmentShader = GLLoadShader ( GL_FRAGMENT_SHADER, fShaderStr );
	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return 0;
	}

	gProObject = glCreateProgram ( );

	glAttachShader ( gProObject, vertexShader );
	glAttachShader ( gProObject, fragmentShader );

	// Link the program
	glLinkProgram ( gProObject );

	// Check the link status
	glGetProgramiv ( gProObject, GL_LINK_STATUS, &linked );

	if ( !linked )
	{
		GLint infoLen = 0;

		glGetProgramiv ( gProObject, GL_INFO_LOG_LENGTH, &infoLen );

		if ( infoLen > 1 )
		{
			char* infoLog = (char *)malloc (sizeof(char) * infoLen );

			glGetProgramInfoLog ( gProObject, infoLen, NULL, infoLog );
			free ( infoLog );
		}

		glDeleteProgram ( gProObject );
		return 0;
	}

	// Free up no longer needed shader resources
	glDeleteShader ( vertexShader );
	glDeleteShader ( fragmentShader );

	gShaderPosition = glGetAttribLocation ( gProObject, "a_position" );
	gShaderTexCoord = glGetAttribLocation ( gProObject, "a_texCoord" );

	gvpMatrix 	     = glGetUniformLocation( gProObject, "u_TransMatrix" );
	gShaderImgTexture = glGetUniformLocation ( gProObject, "s_ImgTexture" );

	gColorFlag		= glGetUniformLocation( gProObject, "specColorFlag" );
	gColorLoc		= glGetUniformLocation( gProObject, "specColor" );

	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

	glGenTextures(1, &gMainTexture );
	glGenTextures(1, &gTextureTmp);

	//GlesBindOffScreen();

	//	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//	glBindTexture(GL_TEXTURE_2D, gTextureTmp);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	//	glTexImage2D(GL_TEXTURE_2D,
	//			0,
	//			GL_RGBA,
	//			gWidth,
	//			gHeight,
	//			0,
	//			GL_RGBA,
	//			GL_UNSIGNED_BYTE,
	//			NULL );

	return gProObject;
}

GLuint GLLoadShader ( GLenum type,  const char *shaderSrc )
{
	GLuint shader;
	GLint compiled;
	// Create the shader object
	shader = glCreateShader ( type );
	if ( shader == 0 )
		return 0;
	// Load the shader source
	glShaderSource ( shader, 1, &shaderSrc, NULL );
	// Compile the shader
	glCompileShader ( shader );
	// Check the compile status
	glGetShaderiv ( shader, GL_COMPILE_STATUS, &compiled );
	if ( !compiled )
	{
		GLint infoLen = 0;
		glGetShaderiv ( shader, GL_INFO_LOG_LENGTH, &infoLen );
		if ( infoLen > 1 )
		{
			char* infoLog = (char *)malloc (sizeof(char) * infoLen );
			glGetShaderInfoLog ( shader, infoLen, NULL, infoLog );
			free ( infoLog );
		}
		glDeleteShader ( shader );
		return 0;
	}
	return shader;
}

void GLBindOffScreen()
{
	glGenFramebuffers(1, &gFrameBuffer);
	glBindTexture(GL_TEXTURE_2D, gMainTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, gWidth, gHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gMainTexture, 0);
	return;
}


void GLEnvChanged(  int width, int height )
{
	gWidth = width;
	gHeight = height;
	GLMatrixIdentity( &gOrthographic );
	GLOrtho(&gOrthographic, 0, width, -height, 0.0f, 10.0f, -10.0f );
	ESMatrix modelview;
	GLMatrixIdentity( &gMatrix );
	GLMatrixIdentity( &modelview );
	GLMatrixMultiply( &gMatrix, &modelview, &gOrthographic );

	glViewport(0, 0, width, height );
	return;
}

void GLLoadImgDataTex( int width, int height, unsigned char * data )
{
	glBindTexture(GL_TEXTURE_2D, gTextureTmp);
	glTexImage2D(GL_TEXTURE_2D,
		0,
		GL_RGBA,
		width,
		height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		data );
	return;
}

void GLSetDrawOffScreen(bool bOffScreen )
{
	if ( bOffScreen )
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gFrameBuffer);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void GLPaint( GLuint textureID )
{
	ESMatrix matrixTrans;
	ESMatrix modelview;
	GLMatrixIdentity( &modelview );

	//esTranslate( &modelview, -fx, fy, 0.0 );
	GLMatrixMultiply( &matrixTrans, &modelview, &gOrthographic );

	GLfloat vVertices[] =
	{
		0, 	 0, 		0.0f, 0.0f, 1.0f,	// TexCoord 0
		0, 	 -gHeight,	0.0f, 0.0f, 0.0f,	// TexCoord 1
		gWidth, -gHeight,	0.0f, 1.0f, 0.0f,  	// TexCoord 2
		gWidth, 0,			0.0f, 1.0f, 1.0f	// TexCoord 3
	};

	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	// Use the program object
	glUseProgram ( gProObject );

	// Load the vertex position
	glVertexAttribPointer ( gShaderPosition, 3, GL_FLOAT,
		GL_FALSE, 5 * sizeof(GLfloat), vVertices );
	glEnableVertexAttribArray ( gShaderPosition );

	// Load the texture coordinate
	glVertexAttribPointer ( gShaderTexCoord, 2, GL_FLOAT,
		GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );
	glEnableVertexAttribArray ( gShaderTexCoord );

	glUniform1i ( gColorFlag, 0 );
	// Bind the base map
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture ( GL_TEXTURE_2D, textureID );
	//glEnableClientState(GL_VERTEX_ARRAY);

	glUniformMatrix4fv( gvpMatrix, 1, GL_FALSE, (GLfloat *)&matrixTrans );

	glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
	//glDisableClientState(GL_VERTEX_ARRAY);
	return;
}

void GLFlush()
{
	glFlush();
}

void GLLoadImgDataTextuer( GLuint textureID, int width, int height, unsigned char * data )
{
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		width, height,
		0, GL_RGBA, GL_UNSIGNED_BYTE, data );
	return;
}

void GLPaintTexture( GLuint textureID,
	GLfloat left, GLfloat top, GLfloat right, GLfloat bottom,
	GLfloat ul, GLfloat vt, GLfloat ur, GLfloat vb )
{
	ESMatrix matrixTrans;
	ESMatrix modelview;
	GLMatrixIdentity( &modelview );

	//esTranslate( &modelview, -fx, fy, 0.0 );
	GLMatrixMultiply( &matrixTrans, &modelview, &gOrthographic );

	GLfloat ul1 = ul;
	GLfloat vt1 = 1 - vb;
	GLfloat ur1 = ur;
	GLfloat vb1 = 1 - vt;

	GLfloat vVertices[] =
	{
		// x,	 y,		z,		u,	v
		left, -top,		0.0f, 	ul1, vb1,	// TexCoord 0
		left, -bottom,	0.0f,	ul1, vt1,	// TexCoord 1
		right,-bottom,	0.0f,	ur1, vt1, // TexCoord 2
		right,-top,		0.0f,	ur1, vb1	// TexCoord 3
	};

	GLushort indices[] = { 0, 1, 2, 0, 2, 3 };

	// Use the program object
	glUseProgram ( gProObject );

	// Load the vertex position
	glVertexAttribPointer ( gShaderPosition, 3, GL_FLOAT,
		GL_FALSE, 5 * sizeof(GLfloat), vVertices );
	glEnableVertexAttribArray ( gShaderPosition );

	// Load the texture coordinate
	glVertexAttribPointer ( gShaderTexCoord, 2, GL_FLOAT,
		GL_FALSE, 5 * sizeof(GLfloat), &vVertices[3] );
	glEnableVertexAttribArray ( gShaderTexCoord );

	glUniform1i ( gColorFlag, 0 );
	// Bind the base map
	glActiveTexture ( GL_TEXTURE0 );
	glBindTexture ( GL_TEXTURE_2D, textureID );

	glUniformMatrix4fv( gvpMatrix, 1, GL_FALSE, (GLfloat *)&matrixTrans );

	glDrawElements ( GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices );
	return;

}

void GLDrawLine(GLfloat sx, GLfloat sy, GLfloat ex, GLfloat ey, 
	int width, unsigned long nColor )
{
	GLfloat vVertices[] =
	{
		sx, -sy, 0.0f,	// TexCoord 1
		ex, -ey, 0.0f	// TexCoord 0
	};

	// Use the program object
	glUseProgram ( gProObject );

	glUniform1i ( gColorFlag, 1 );
	float src_a = ((nColor & 0xff000000) >> 24) / 255.0f;
	float src_b = ((nColor & 0x00ff0000) >> 16) / 255.0f;
	float src_g = ((nColor & 0x0000ff00) >> 8) / 255.0f;
	float src_r = (nColor & 0x000000ff) / 255.0f;
	float sc[4] = { src_r, src_g, src_b, src_a };
	glUniform4fv(gColorLoc, 1, sc );

	glLineWidth( width );
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vVertices);
	glUniformMatrix4fv( gvpMatrix, 1, GL_FALSE, (GLfloat *)&gMatrix );
	glDrawArrays(GL_LINE_LOOP, 0, 2 );
	glLineWidth( 1 );
	glUniform1i ( gColorFlag, 0 );
	return;
}

void GLDrawRect(GLfloat left, GLfloat top, GLfloat right, GLfloat bottom, GLenum mode, unsigned long nColor )
{
	GLfloat vVertices[] =
	{
		left,  -bottom, 0.0f,
		left,  -top,    0.0f,
		right, -top,    0.0f,
		right, -bottom, 0.0f
	};

	// Use the program object
	glUseProgram ( gProObject );

	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

	glUniform1i ( gColorFlag, 1 );
	//m_nBackColor = 0xFFFF00FF;
	float src_a= ((nColor & 0xff000000) >> 24) / 255.0f;
	float src_b= ((nColor & 0x00ff0000) >> 16) / 255.0f;
	float src_g= ((nColor & 0x0000ff00) >> 8) / 255.0f;
	float src_r=  (nColor & 0x000000ff) / 255.0f;
	float sc[4] = { src_r, src_g, src_b, src_a };
	glUniform4fv(gColorLoc, 1, sc );
	//glUniform4f(gColorLoc,sc[0],sc[1],sc[2],sc[3]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vVertices);
	glUniformMatrix4fv( gvpMatrix, 1, GL_FALSE, (GLfloat *)&gMatrix );
	glDrawArrays(mode, 0, 4 );
	glUniform1i ( gColorFlag, 0 );
	return;
}


void GLDrawEllipse(GLfloat left, GLfloat top, GLfloat right, GLfloat bottom, GLenum mode, unsigned long nColor )
{
	int px = 0;
	int py = 0;
	const double pi = 3.1415926;
	float angle = 0;
	int w = abs(right - left) / 2;
	int h = abs( bottom - top) / 2;
	GLfloat vVertices[360 * 3] = {0};
	for (int i = 0; i < 360; i += 1 )
	{
		angle = i + 0.5;
		px = w + w*cos(angle/180.0 *pi);
		py = h + h*sin(angle/180.0 *pi);
		vVertices[i*3]	 = left + px;
		vVertices[i*3 + 1] = -top - py;
		vVertices[i*3 + 2] = 0.0;
	}

	// Use the program object
	glUseProgram ( gProObject );

	//glEnable (GL_BLEND);
	//glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);

	glUniform1i ( gColorFlag, 1 );
	//m_nBackColor = 0xFFFF00FF;
	float src_a= ((nColor & 0xff000000) >> 24) / 255.0f;
	float src_b= ((nColor & 0x00ff0000) >> 16) / 255.0f;
	float src_g= ((nColor & 0x0000ff00) >> 8) / 255.0f;
	float src_r=  (nColor & 0x000000ff) / 255.0f;
	float sc[4] = { src_r, src_g, src_b, src_a };
	glUniform4fv(gColorLoc, 1, sc );
	//glUniform4f(gColorLoc,sc[0],sc[1],sc[2],sc[3]);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), vVertices);
	glUniformMatrix4fv( gvpMatrix, 1, GL_FALSE, (GLfloat *)&gMatrix );
	glDrawArrays(mode, 0, 4 );
	glUniform1i ( gColorFlag, 0 );
	return;
}

