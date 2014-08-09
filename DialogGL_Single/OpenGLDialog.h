// OpenGLDialog.h : Haupt-Header-Datei für die Anwendung OPENGLDIALOG
//

#if !defined(AFX_OPENGLDIALOG_H__77B6740A_D688_4CAA_AA2E_0AE536C07B5F__INCLUDED_)
#define AFX_OPENGLDIALOG_H__77B6740A_D688_4CAA_AA2E_0AE536C07B5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// Hauptsymbole

/////////////////////////////////////////////////////////////////////////////
// COpenGLDialogApp:
// Siehe OpenGLDialog.cpp für die Implementierung dieser Klasse
//

class COpenGLDialogApp : public CWinApp
{
public:
	COpenGLDialogApp();

// Überladungen
	// Vom Klassenassistenten generierte Überladungen virtueller Funktionen
	//{{AFX_VIRTUAL(COpenGLDialogApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementierung

	//{{AFX_MSG(COpenGLDialogApp)
		// HINWEIS - An dieser Stelle werden Member-Funktionen vom Klassen-Assistenten eingefügt und entfernt.
		//    Innerhalb dieser generierten Quelltextabschnitte NICHTS VERÄNDERN!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ fügt unmittelbar vor der vorhergehenden Zeile zusätzliche Deklarationen ein.

#endif // !defined(AFX_OPENGLDIALOG_H__77B6740A_D688_4CAA_AA2E_0AE536C07B5F__INCLUDED_)
