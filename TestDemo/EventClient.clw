; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEventClientDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EventClient.h"

ClassCount=3
Class1=CEventClientApp
Class2=CEventClientDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_EVENTCLIENT_DIALOG

[CLS:CEventClientApp]
Type=0
HeaderFile=EventClient.h
ImplementationFile=EventClient.cpp
Filter=N

[CLS:CEventClientDlg]
Type=0
HeaderFile=EventClientDlg.h
ImplementationFile=EventClientDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_RICHEDIT_XML

[CLS:CAboutDlg]
Type=0
HeaderFile=EventClientDlg.h
ImplementationFile=EventClientDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_EVENTCLIENT_DIALOG]
Type=1
Class=CEventClientDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_IP,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDIT_PORT,edit,1350631552
Control7=IDC_BUTTON_CONNECT,button,1342242816
Control8=IDC_STATIC,static,1342308352

