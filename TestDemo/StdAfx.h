// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8A98F756_53D1_4513_872D_A53FD95E89B1__INCLUDED_)
#define AFX_STDAFX_H__8A98F756_53D1_4513_872D_A53FD95E89B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


#define WMU_SYS_EVENT							(WM_USER+1010)


#define SIO_KEEPALIVE_VALS _WSAIOW(IOC_VENDOR,4)

struct TCP_KEEPALIVE_VALUE
{
	unsigned long on_off;
	unsigned long keep_alive_time;
	unsigned long keep_alive_interval;
} ;




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8A98F756_53D1_4513_872D_A53FD95E89B1__INCLUDED_)
