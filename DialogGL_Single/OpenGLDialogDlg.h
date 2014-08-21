// OpenGLDialogDlg.h : Header-Datei
//

#if !defined(AFX_OPENGLDIALOGDLG_H__920A32A4_A76B_48E8_AE75_0942A4B378C2__INCLUDED_)
#define AFX_OPENGLDIALOGDLG_H__920A32A4_A76B_48E8_AE75_0942A4B378C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include "BwGLView.h"


#include "EGLView.h"
#include "RasterGL.h"
//#include "CCDrawNode.h"


/////////////////////////////////////////////////////////////////////////////
// COpenGLDialogDlg Dialogfeld

class COpenGLDialogDlg : public CDialog
{
// Konstruktion
public:
	COpenGLDialogDlg(CWnd* pParent = NULL);	// Standard-Konstruktor

// Dialogfelddaten
	//{{AFX_DATA(COpenGLDialogDlg)
	enum { IDD = IDD_OPENGLDIALOG_DIALOG };
	//}}AFX_DATA

	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(COpenGLDialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV-Unterstützung
	//}}AFX_VIRTUAL

// Implementierung
protected:
	HICON m_hIcon;

	EGLView* eglView;
	RasterGL *pRaster;
	//MainLayer *mMainLayer;
	//BwGLView mBwGLView;

	// Generierte Message-Map-Funktionen
	//{{AFX_MSG(COpenGLDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_OPENGLDIALOGDLG_H__920A32A4_A76B_48E8_AE75_0942A4B378C2__INCLUDED_)
