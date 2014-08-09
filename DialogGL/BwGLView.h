#ifndef __BWGLVIEW_H__
#define __BWGLVIEW_H__

#include "BwGLUtility.h"

/////////////////////////////////////////////////////////////////////////////
class BwGLView
{
public:
	BwGLView();
public:
	//void Create(CRect rect,CWnd* parent);
	void Create( HDC hdc );
	virtual ~BwGLView();

protected:
	void InitGL();
	void DrawGLScene();
	
protected:

	HDC	m_hDC;
	float mfRotation;

public:
	void OnPaint();
	void OnSize(UINT nType, int cx, int cy);
//	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDC* pDC);

	//afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	//DECLARE_MESSAGE_MAP()
};

#endif //__BWGLVIEW_H__
