// EventClientDlg.h : header file
//

#if !defined(AFX_EVENTCLIENTDLG_H__3B1BDE3B_72EE_4866_AEC7_31302B423A7F__INCLUDED_)
#define AFX_EVENTCLIENTDLG_H__3B1BDE3B_72EE_4866_AEC7_31302B423A7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEventClientDlg dialog

class CEventClientDlg : public CDialog
{
// Construction
public:
	CEventClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CEventClientDlg)
	enum { IDD = IDD_EVENTCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEventClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL


	
// Implementation
protected:
	HICON m_hIcon;


	

	// Generated message map functions
	//{{AFX_MSG(CEventClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConnect();
	//}}AFX_MSG
	afx_msg LRESULT OnRecvNetEvent(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	static  char  m_StaEventServerIP[32];
	static  int   m_StaeventServerPort;
	BOOL      m_nGoWork;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTCLIENTDLG_H__3B1BDE3B_72EE_4866_AEC7_31302B423A7F__INCLUDED_)
