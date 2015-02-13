// OpenGLDialogDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "OpenGLDialog.h"
#include "OpenGLDialogDlg.h"
//#include "cctypes.h"
#include "set"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenGLDialogDlg Dialogfeld

COpenGLDialogDlg::COpenGLDialogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenGLDialogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenGLDialogDlg)
	//}}AFX_DATA_INIT
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf benötigt
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COpenGLDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenGLDialogDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COpenGLDialogDlg, CDialog)
	//{{AFX_MSG_MAP(COpenGLDialogDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// COpenGLDialogDlg Nachrichten-Handler

BOOL COpenGLDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Symbol für dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Großes Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	
	// ZU ERLEDIGEN: Hier zusätzliche Initialisierung einfügen
	
	CRect rect;
	this->GetWindowRect( rect );
	ScreenToClient(rect);

	eglView = EGLView::sharedOpenGLView();
	//eglView->CreateGL( this->m_hWnd );
	//eglView->Create( this->m_hWnd );
	//eglView->setFrameSize( 800, 600 );
	eglView->CreateView( m_hWnd, 0, 0, rect.Width(), rect.Height() );
	eglView->setGLDefaultValues();
	pctx = XContext::create();
	pctx->mWidth = rect.Width();
	pctx->mHeight = rect.Height();

	//fillRectWithPattern();
	//fillRectWithColor();
	fillRectGradientLinear();


	//XGradientLinear *pgradient = pctx->CreateLinearGradient( 0, 0, 50, 0 );
	//ccColor4F color = {255, 0, 0, 255};
	//pgradient->addColorStop( 0, color );
	//ccColor4F blue = { 0, 255, 0, 255 };
	//pgradient->addColorStop( 1, blue );

	//pctx->mpFillStyle->setFillType( pgradient );
	//pctx->fillRect( 20, 20, 150, 100 );

	//ccColor4F *color = new ccColor4F;
	//color->r = 0.0;
	//color->b = 0;
	//color->g = 1.0;
	//color->a = 1.0;
	//pctx->mpFillStyle->mpColor = color;
	//pctx->rect(50,20,200,120);
	//pctx->stroke();
	//pctx->clip();
	//
	//pctx->fillRect(0,0,200,120);

	//pctx->strokeRect( 200, 200, 100, 100 );
	//pctx->clearRect( 210, 210, 20, 20 );
	//pctx->beginPath();
	//pctx->moveto( 20, 20 );
	//pctx->quadraticCurveTo( 20, 200, 200, 20 );
	//pctx->bezierCurveTo( 20, 100, 100, 100, 100, 20 );
	//pctx->stroke();
	//pctx->arc( 300, 300, 150, 0, 1.65, true );
	//pctx->stroke();
	//pctx->rect( 120, 120, 150, 100 );
	//pctx->fill();
	//pctx->fillRect( 120, 120, 100, 100 );

	// »æÖÆÖ±Ïß
	//pctx->beginPath();
	//pctx->mLineWidth = 10;
	//ccColor4F *color = new ccColor4F;
	//color->a = 1.0;
	//color->r = 1.0;
	//color->b = 0;
	//color->g = 1.0;
	//pctx->mpFileStyle.mpColor = color;
	//pctx->moveto( 50, 50 );
	//pctx->lineto( 300, 150 );
	//pctx->stroke();

	//pctx->beginPath();
	//pctx->mLineWidth = 2;
	//////ccColor4F *color1 = new ccColor4F;
	////color->a = 1.0;
	////color->r = 0.0;
	////color->b = 1.0;
	////color->g = 1.0;
	////pctx->mpFileStyle.mpColor = color;
	//pctx->moveto( 80, 240 );
	//pctx->lineto( 140, 240 );
	//pctx->lineto( 110, 60 );
	//pctx->lineto( 240, 50 );
	//pctx->lineto( 20, 20 );
	//pctx->lineto( 0, 70 );
	//pctx->lineto( 100, 50 );
	//pctx->lineto( 80, 240 );
	//
	////pctx->closePath();
	//pctx->stroke();
	////pctx->fill();
	
	
	
	return TRUE;  
} 

void COpenGLDialogDlg::fillRectWithColor()
{
	ccColor4F color = { 1, 0.9, 0, 1 };
	pctx->mpFillStyle->setFillType( color );
	pctx->fillRect( 20, 20, 100, 100 );
}

void COpenGLDialogDlg::fillRectWithPattern()
{
	XPattern *pat = new XPattern();
	FILE *pFile = fopen( "c:/test.png", "rb" );
	fseek( pFile, 0, SEEK_END );
	int ilen = ftell( pFile );
	fseek( pFile, 0, SEEK_SET );
	unsigned char *pData = new unsigned char[ilen ];
	fread( pData, ilen, 1, pFile );
	fclose( pFile );

	int width = 0;
	int height = 0;

	unsigned char *pImgData = pctx->DecodePngData( pData, ilen, width, height );
	pat->texId = pctx->initTexData( pImgData, width, height );
	pat->width = width;
	pat->height = height;
	pat->mRepeatePat = en_REPEAT_X;

	delete []pData;
	//free( pImgData );

	pctx->mpFillStyle->setFillType( pat );
	pctx->fillRect( 20, 20, 300, 300 );

}

void COpenGLDialogDlg::fillRectGradientLinear()
{
	XGradientLinear *pgradient = pctx->CreateLinearGradient( 40, 0, 100, 100 );
	ccColor4F red = { 1, 0, 0, 1 };
	ccColor4F green = { 0, 1, 0, 1 };
	ccColor4F blue = { 0, 0, 1, 1 };
	ccColor4F rg = { 1, 1, 0, 1 };
	pgradient->addColorStop( 0, red );
	pgradient->addColorStop( 1, green );
	pgradient->addColorStop( 0.5, blue );
	pgradient->addColorStop( 0.75, rg );
	pctx->mpFillStyle->setFillType( pgradient );
	pctx->fillRect( 0, 0, 100, 100 );
	return;
}

void COpenGLDialogDlg::OnPaint() 
{
	//glEnable( GL_SCISSOR_TEST );
	//glClearColor(1.0f, 0.0f, 0.0f, 0.0f); 
	//glScissor( 50, 200, 100, 500 );
	//glClear(GL_COLOR_BUFFER_BIT);  
	pctx->draw();
	//glDisable( GL_SCISSOR_TEST );
	eglView->swapBuffers();

	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Symbol in Client-Rechteck zentrieren
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Symbol zeichnen
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, während der Benutzer
//  das zum Symbol verkleinerte Fenster mit der Maus zieht.
HCURSOR COpenGLDialogDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



void COpenGLDialogDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	//eglView->setFrameSize( cx, cy );
	//mBwGLView.OnSize(nType, cx, cy );
	return;
}


BOOL COpenGLDialogDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//char pchTmp[ 256 ] = {0};
	//sprintf( pchTmp, "%s, %d", __FUNCTION__, __LINE__ );
	//OutputDebugStringA( pchTmp );
	//mBwGLView.OnEraseBkgnd(pDC );
	return CDialog::OnEraseBkgnd(pDC);
}


void COpenGLDialogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonDown(nFlags, point);
	//CCEGLView::sharedOpenGLView()->MsgLBtnDown( nFlags, point.x, point.y );
	return;
}


void COpenGLDialogDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnLButtonUp(nFlags, point);
	//CCEGLView::sharedOpenGLView()->MsgLBtnUp( nFlags, point.x, point.y );
	return;
}


void COpenGLDialogDlg::OnMButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMButtonDblClk(nFlags, point);
}


void COpenGLDialogDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMButtonDown(nFlags, point);
}


void COpenGLDialogDlg::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnMButtonUp(nFlags, point);
}


void COpenGLDialogDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnLButtonDblClk(nFlags, point);
}
