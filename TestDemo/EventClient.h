// EventClient.h : main header file for the EVENTCLIENT application
//

#if !defined(AFX_EVENTCLIENT_H__28B1E089_5DDB_49B4_8DFF_4B0CBE93D668__INCLUDED_)
#define AFX_EVENTCLIENT_H__28B1E089_5DDB_49B4_8DFF_4B0CBE93D668__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEventClientApp:
// See EventClient.cpp for the implementation of this class
//

class CEventClientApp : public CWinApp
{
public:
	CEventClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEventClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTCLIENT_H__28B1E089_5DDB_49B4_8DFF_4B0CBE93D668__INCLUDED_)
