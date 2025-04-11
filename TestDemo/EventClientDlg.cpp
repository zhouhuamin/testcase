// EventClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EventClient.h"
#include "EventClientDlg.h"
#include "ControlCmdHandler.h"
#include "SysUtil.h"
#include "AlarmMsgHelper.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
char  CEventClientDlg::m_StaEventServerIP[32];
int   CEventClientDlg::m_StaeventServerPort;


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventClientDlg dialog

CEventClientDlg::CEventClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventClientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEventClientDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_nGoWork=TRUE;
}

void CEventClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEventClientDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CEventClientDlg, CDialog)
	//{{AFX_MSG_MAP(CEventClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WMU_SYS_EVENT,OnRecvNetEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEventClientDlg message handlers

BOOL CEventClientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT_IP,"192.168.1.188");
	SetDlgItemInt(IDC_EDIT_PORT,9000);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEventClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEventClientDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEventClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}



unsigned __stdcall ConnectToAlarmServerThreadFunc( void* pArg )
{
	CEventClientDlg* pMainDlg=(CEventClientDlg*)pArg;
	CControlCmdHandler * handler = new CControlCmdHandler;
	
	if(NULL == pMainDlg)
	{
		delete handler;
	
		return 0;
	}
	
	
	
	handler->SetParentHwnd(pMainDlg->m_hWnd);
	int nTimerCount=0;
	while (pMainDlg->m_nGoWork)
	{
		if (!CControlCmdHandler::m_bConnected)
		{
			if (handler->ConnectToAlarmServer(CEventClientDlg::m_StaeventServerPort,CEventClientDlg::m_StaEventServerIP) == -1)
			{
				
			}
			else
			{
				CControlCmdHandler::m_bConnected = true;
			}
		}
		else
		{
			nTimerCount++;
			if(nTimerCount % 12 ==0)
			{
				handler->SendHeartbeatMsg();
			}
			
			
			if(nTimerCount>10000)
			{
				nTimerCount=0;
			}


//			handler->SendData();

		}
		
		//select代替sleep,等待一段时间间隔5s
		for(int i=0;i<100;i++)
		{
			SysUtil::SysSleep(50);
			
			if(!pMainDlg->m_nGoWork)
			{
				delete handler;
		
				_endthreadex(0);
				return 0;
			}
			
		}
		
	}
	

	delete handler;
	_endthreadex(0);
	return 0;
}



void CEventClientDlg::OnButtonConnect() 
{
	// TODO: Add your control notification handler code here

	CString strServerIP;
	GetDlgItemText(IDC_EDIT_IP,strServerIP);
	strcpy(m_StaEventServerIP,strServerIP);

	m_StaeventServerPort=GetDlgItemInt(IDC_EDIT_PORT);


	//启动单独的线程，连接报警服务
	unsigned threadID=0;
	HANDLE hThread = (HANDLE)_beginthreadex( NULL, 0, &ConnectToAlarmServerThreadFunc, this, NULL, &threadID );
	CloseHandle( hThread );

}


LRESULT CEventClientDlg::OnRecvNetEvent(WPARAM wParam,LPARAM lParam)
{

	int nIndex=(int)lParam;

	NET_PACKET_MSG* pMsg=NULL;
	CAlarmMsgHelper::Get_Message(nIndex,pMsg);

	if(pMsg)
	{
		NET_EVENT* pEvent=(NET_EVENT*)pMsg->msg_body;
		if(pEvent->sub_type==SYS_EVENT_TYPE_UPLOAD_CUSTOMS_DATA)
		{
			NET_EVENT_UPLOAD_CUSTOMSDATA* pCustomosData=(NET_EVENT_UPLOAD_CUSTOMSDATA*)pMsg->msg_body;

			CString strXML=pCustomosData->customs_data.szCustomsData+38;
			SetDlgItemText(IDC_RICHEDIT_XML,strXML);
		}
	}
	
	return 0;
}
