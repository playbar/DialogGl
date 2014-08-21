// OpenGLDialogDlg.cpp : Implementierungsdatei
//

#include "stdafx.h"
#include "OpenGLDialog.h"
#include "OpenGLDialogDlg.h"
//#include "cctypes.h"
#include "set"

//#include "CCDrawingPrimitives.h"

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
	eglView->CreateView( m_hWnd, 0, 0, 821, 586 );
	eglView->ShowView( true );
	CCDirector::sharedDirector()->setOpenGLView( eglView );
	CCDirector::sharedDirector()->initScene();


	eglView->MakeCurrent( true );
	drawNode = CCDrawNode::create();
	
	ccV2F_C4F_T2F vct[4] = 
	{
		{ {100, 100},{ 1, 0, 0, 1 }, {0, 0 } },
		{ {200, 200},{0, 1, 0, 1},   {0, 0 } },
		{ {300, 100}, {0, 0, 1, 1}, {0, 0}},
		{ {200, 0}, { 1, 1, 0, 1}, { 0, 0 } }
	};
	//vct[1] = { {200, 200}, {0, 0.5, 0, 1}, {0, 0}};
	//vct[2] = 
	//vct[3] = { {200, 0}, { 0, 1, 0, 1}, { 0, 0 } };
	//drawNode->drawPolygon( vct, 4, 0, ccc4f( 0, 0, 0, 0) );
	ccV2F_C4F_T2F p1 = { {100, 100},{ 1, 0, 0, 1 }, {0, 0 } };
	ccV2F_C4F_T2F p2 = { {200, 200},{0, 1, 0, 1},   {0, 0 } };
	ccV2F_C4F_T2F p3 = { {300, 100}, {0, 0, 1, 1}, {0, 0}};
	drawNode->setPosition( 100, 100 );
	drawNode->drawTriangle( p1, p2, p3 );

	//drawNode->setPosition( 300, 300 );
	//drawNode->drawPolygon(circle, 18, green, 1, green );//绘制这个多边形!
	//drawNode->drawDot( ccp( 200, 200 ), 50, ccc4f( 1.0, 0, 0, 1.0f ));
	//drawNode->drawAllPolygon();
	//drawNode->endPolygon();
	//eglView->AddNode( drawNode );

	CCDirector::sharedDirector()->addNode( drawNode );
	//CCSprite *sprite = CCSprite::create( "test.png" );
	//sprite->setPosition( ccp(100, 300) );
	//sprite->setAnchorPoint( ccp( 0, 0.5 ));
	//CCDirector::sharedDirector()->addNode( sprite );
	eglView->MakeCurrent( false );

	return TRUE;  // Geben Sie TRUE zur點k, au遝r ein Steuerelement soll den Fokus erhalten
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfl鋍he "Minimieren" hinzuf黦en, ben鰐igen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. F黵 MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch f黵 Sie erledigt.

void COpenGLDialogDlg::OnPaint() 
{
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
