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
	// Beachten Sie, dass LoadIcon unter Win32 keinen nachfolgenden DestroyIcon-Aufruf ben鰐igt
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

	// Symbol f黵 dieses Dialogfeld festlegen. Wird automatisch erledigt
	//  wenn das Hauptfenster der Anwendung kein Dialogfeld ist
	SetIcon(m_hIcon, TRUE);			// Gro遝s Symbol verwenden
	SetIcon(m_hIcon, FALSE);		// Kleines Symbol verwenden
	
	// ZU ERLEDIGEN: Hier zus鋞zliche Initialisierung einf黦en
	
	CRect rect;
	this->GetWindowRect( rect );
	ScreenToClient(rect);


	eglView = CCEGLView::sharedOpenGLView();
	eglView->CreateGL( this->m_hWnd );
	//eglView->Create( this->m_hWnd );
	eglView->setFrameSize( 800, 600 );
	eglView->setGLDefaultValues();

	pRaster = RasterGL::create();
	static ccColor4F green ={0,1,0,1};
	float fRadius=200.0f;
	const int nCount=100;
	//const float coef= 2.0f* (float)M_PI/nCount;
	static CCPoint circle[nCount];

	//circle[0].x = 100;
	//circle[0].y = 100;
	//circle[4].x = -100;
	//circle[4].y = 100;
	//circle[3].x = -100;
	//circle[3].y = -100;
	//circle[2].x = 100;
	//circle[2].y = -100;
	//circle[1].x = 0;
	//circle[1].y = 0;
	//for(unsigned int i= 0;i< nCount;i++)
	//{
	//	float rads =i*coef;
	//	circle[i].x= fRadius* cosf(rads);
	//	circle[i].y= fRadius* sinf(rads);
	//}
	
	circle[0].x = 50;
	circle[0].y = 100;
	circle[1].x = 0;
	circle[1].y = 0;
	circle[2].x = -50;
	circle[2].y = 100;
	circle[3].x = -100;
	circle[3].y = 50;
	circle[4].x = -150;
	circle[4].y = 100;
	circle[5].x = -100;
	circle[5].y = 0;
	circle[6].x = -120;
	circle[6].y = -100;
	circle[7].x = -80;
	circle[7].y = -50;
	circle[8].x = -30;
	circle[8].y = -140;
	circle[9].x = -10;
	circle[9].y = -60;
	circle[10].x = 0;
	circle[10].y = -80;
	circle[11].x = 40;
	circle[11].y = -30;
	circle[12].x = 110;
	circle[12].y = -100;
	circle[13].x = 120;
	circle[13].y = -80;
	circle[14].x = 200;
	circle[14].y = 0;
	circle[15].x = 150;
	circle[15].y = 50;
	circle[16].x = 150;
	circle[16].y = 100;
	circle[17].x = 100;
	circle[17].y = 50;

	pRaster->drawDot( ccp( 200, 200), 50, ccc4f( 1, 0, 1, 1 ) );
	
	//pRaster->drawPolygon(circle, 18, green, 1, green );//绘制这个多边形!
	//pRaster->beginPolygon();
	//pRaster->drawAllPolygon();
	//pRaster->endPolygon();

	//drawNode->drawDot( ccp( 100, 100 ), 40, ccc4f( 1, 0, 1, 1 ));
	//CCDirector *pDirector = ;
	//CCDirector::sharedDirector()->setOpenGLView( eglView );
	//pDirector->setOpenGLView( eglView );
	//pDirector->initScene();
	//CCSize frameSize = eglView->getFrameSize();

	//mMainLayer = MainLayer::create();
	//mMainLayer->retain();
	//pDirector->addNode( mMainLayer );
	
	//mBwGLView.Create( this->GetDC()->GetSafeHdc() );
	//mBwGLView.OnSize(0, rect.Width(), rect.Height() );
	//openGLControl.Create(rect,this);

	return TRUE;  
} 


void COpenGLDialogDlg::OnPaint() 
{
	//ccDrawColor4B(255,0,0,255);
	//ccDrawLine( ccp(100, 100), ccp( 800, 600) );
	//ccDrawQuadBezier( ccp(0, 600), ccp( 400, 0), ccp( 800, 600), 6);
	//drawNode->draw();
	//pRaster->visit();
	pRaster->draw();
	eglView->swapBuffers();

	if (IsIconic())
	{
		CPaintDC dc(this); // Ger鋞ekontext f黵 Zeichnen

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

// Die Systemaufrufe fragen den Cursorform ab, die angezeigt werden soll, w鋒rend der Benutzer
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
