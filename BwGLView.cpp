#include "stdafx.h"
#include "OpenGLDialog.h"
#include "BwGLView.h"
//#include "XGraphics.h"
//#include "BwImgProcess.h"
//#include "gl\glew.h"
#include <gl3\gl3.h>

//XGraphics gGraphics;

BwGLView::BwGLView()
{
	m_hDC = NULL;
	mfRotation = 0.0f;
}

BwGLView::~BwGLView()
{
	GLDestroy();
}

void BwGLView::InitGL()
{
	//glShadeModel(GL_SMOOTH);
	//glClearColor( 1.0f, 1.0f, 1.0f, 0.0f);
	//glClearDepth(1.0f);							
	////glEnable(GL_DEPTH_TEST);					
	//glDepthFunc(GL_LEQUAL);	
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void BwGLView::DrawGLScene()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glLoadIdentity();

//	glTranslatef(0.0f,0.0f,-5.0f);
	//glRotatef(mfRotation,0.0f,1.0f,0.0f);

	//glBegin( GL_TRIANGLES );
	//	glColor4f(1.0f, 0.0f, 0.0f, 1.0f );
	//	glVertex2d( 100.0f, 50.0f );
	//	glColor4f(0.0f, 1.0f, 0.0f, 1.0f );
	//	glVertex2f(450.0f, 400.0f );
	//	glColor4f(0.0f, 0.0f, 1.0f, 1.0f );
	//	glVertex2f( 450.0f, 50.0f );
	//glEnd();

	//glBegin(GL_TRIANGLES);
	//glColor3f(1.0f,0.0f,0.0f);
	//glVertex3f( 600.0f,-400.0f,0.0f);
	//glColor3f(0.0f,1.0f,0.0f);
	//glVertex3f(200.0f,-400.0f,0.0f);
	//glColor3f(1.0f,1.0f,1.0f);
	//glVertex3f(400.0f, -100.0f,0.0f);
	//glEnd();

	//glBegin(GL_QUADS);
	//glColor3f(1.0f,0.0f,0.0f);
	//glVertex3f(-1.1f,-1.1,0.0f);
	//glVertex3f(1.1f,-1.1f,0.0f);
	//glVertex3f(1.1f,1.1f,0.0f);
	//glVertex3f(-1.1f,1.1f,0.0f);
	//glEnd();

	//SwapBuffers( m_hDC );
}


void BwGLView::Create( HDC hdc )
{
	m_hDC = hdc;

	GLCreateHDC( hdc ); 
	//glewInit();
	CreateGLEnv();
	//GLBindOffScreen();
	//GLSetDrawOffScreen( false );
	//InitGL();
	int width = 0;
	int height = 0;
	//unsigned char * pData = BwDecodeImgFile("d:/test.jpg", width, height, 0 );
	//GLLoadImgDataTextuer( gTextureTmp, width, height, pData );
	return;
}

void BwGLView::OnPaint() 
{
	mfRotation += 0.01f;

	if (mfRotation >= 360.0f)
	{
		mfRotation -= 360.0f;
	}
	/** OpenGL section **/
	//GLMakeCurrent();
	//DrawGLScene();
	//unsigned short * pTmp = utf8ToUnicode("test");
	
	//graphics.DrawString(pTmp, 10, 10, 4 );
	GLDrawLine(40, 40, 100, 100, 2, 0xFF00FF00 );

	GLFlush();
	return;
}

void BwGLView::OnSize(UINT nType, int cx, int cy) 
{
	GLEnvChanged( cx, cy );
	return;
	//glViewport( 0, 0, cx, cy );	
	//glMatrixMode(GL_PROJECTION);						
	//glLoadIdentity();
	//glOrtho( 0.0f, cx, -cy, 0.0f, 10.0f, -10.0f);
	//glMatrixMode(GL_MODELVIEW);						
	//glLoadIdentity();
}


BOOL BwGLView::OnEraseBkgnd(CDC* pDC) 
{
	//GLDrawLine(40, 100, 40, 100, 2, 0xFF00FF00 );
	//GLFlush();
	//GLMakeCurrent( false );
	OnPaint();
	return TRUE;
}
