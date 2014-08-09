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

	eglView = CCEGLView::sharedOpenGLView();
	eglView->CreateGL( this->m_hWnd );
	eglView->setViewName("HelloCpp");
	eglView->setFrameSize( 800, 600 );

	//drawNode = CCDrawNode::create();
	//drawNode->drawDot( ccp( 100, 100 ), 40, ccc4f( 0.1, 0.2, 0.1, 0.3 ));
	//CCDirector *pDirector = ;
	CCDirector::sharedDirector()->setOpenGLView( eglView );
	//pDirector->setOpenGLView( eglView );
	//pDirector->initScene();
	//CCSize frameSize = eglView->getFrameSize();

	mMainLayer = MainLayer::create();
	//mMainLayer->retain();
	//pDirector->addNode( mMainLayer );
	
	//mBwGLView.Create( this->GetDC()->GetSafeHdc() );
	//mBwGLView.OnSize(0, rect.Width(), rect.Height() );
	//openGLControl.Create(rect,this);

	return TRUE;  // Geben Sie TRUE zurück, außer ein Steuerelement soll den Fokus erhalten
}

// Wollen Sie Ihrem Dialogfeld eine Schaltfläche "Minimieren" hinzufügen, benötigen Sie 
//  den nachstehenden Code, um das Symbol zu zeichnen. Für MFC-Anwendungen, die das 
//  Dokument/Ansicht-Modell verwenden, wird dies automatisch für Sie erledigt.

void COpenGLDialogDlg::OnPaint() 
{
	//char pchTmp[ 256 ] = {0};
	//sprintf( pchTmp, "%s, %d", __FUNCTION__, __LINE__ );
	//OutputDebugStringA( pchTmp );
	//mBwGLView.OnPaint(); 
	//CCDirector::sharedDirector()->drawScene();

	mMainLayer->draw();
	//mMainLayer->visit();

	eglView->swapBuffers();

	//ccDrawColor4F( 0.1, 0.4, 0.2, 0.3 );
	//ccDrawPoint( ccp( 30, 30 ));
	//drawNode->draw();
	//return;

	if (IsIconic())
	{
		CPaintDC dc(this); // Gerätekontext für Zeichnen

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
