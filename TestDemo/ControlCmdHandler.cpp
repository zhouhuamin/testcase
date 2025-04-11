// ControlCmdHandler.cpp: implementation of the CControlCmdHandler class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ControlCmdHandler.h"
#include "SysUtil.h"
#include "AlarmMsgHelper.h"


#include <string>
using namespace std;

bool CControlCmdHandler::m_bConnected = false;

void HandlerProc(void * pParam)
{
    if (!pParam)
    {
        return;
    }
    CControlCmdHandler* pHandler = (CControlCmdHandler*) pParam;
    if (!pHandler)
    {
        return ;
    }

    pHandler->svc();

    SysUtil::SysSleep(500);
  
    return ;
}

CControlCmdHandler::CControlCmdHandler(void)
{
//    dwRecvBuffLen = 0;
    bGoWork_ = true;
    socket_ = -1;
    bThread_flag_ = false;
	m_hWndParent = NULL;

	CAlarmMsgHelper::InitMessage();
}

CControlCmdHandler::~CControlCmdHandler(void)
{
    if (bGoWork_)
    {
        bGoWork_ = false;
        SysUtil::SysSleep(1000);
    }

    m_bConnected = false;
    if (socket_ > 0)
    {
        SysUtil::CloseSocket(socket_);
    }
}

int CControlCmdHandler::open(void*)
{
	CAlarmMsgHelper::RegisterAlarmConnection(socket_);

	int rcvbuf=2*1024*1024; 
	int rcvbufsize=sizeof(int);
	setsockopt(socket_,SOL_SOCKET,SO_RCVBUF,(char*)&rcvbuf,rcvbufsize);


    Register();

    if (!bThread_flag_)
    {
		bThread_flag_=TRUE;
        _beginthread(HandlerProc,NULL,this);
    }

    return 0;
}

int CControlCmdHandler::svc()
{
    char* chRecvBuffer=new char[MAX_MSG_BODYLEN*50];
	int nRecvLen=0;
 	char*  chDest = new char[MAX_MSG_BODYLEN*50];
 
    while (bGoWork_)
    {
        if (socket_ > 0)
        {   
            /*阻塞接收信令*/
            if(SysUtil::ReceiveTimer(socket_) > 0)
			{
				int nRet=SysUtil::SocketRead(socket_,chRecvBuffer+nRecvLen,MAX_MSG_BODYLEN*50-nRecvLen);
				
				if(nRet>0)
				{
					nRecvLen+=nRet;
					int nOffset=0;
					int nLen=0;

					memset(chDest,0,MAX_MSG_BODYLEN*50);
					while(VerifyRecvPacket(chRecvBuffer,chDest,nRecvLen,nOffset,nLen)==0)
					{
						NET_PACKET_HEAD* pMsgHead=(NET_PACKET_HEAD*)(chDest+nOffset);
						int nPacketLen=pMsgHead->packet_len;

						if(pMsgHead->msg_type==SYS_MSG_EVENT_SUBSCRIBE_ACK)
						{

							continue;
						}


						if(pMsgHead->msg_type==SYS_MSG_SYSTEM_CTRL)
						{
							SendData();		
						}
							
					}

				}
				else
				{
					SysUtil::CloseSocket(socket_);
					socket_ = -1;
					m_bConnected = false;

					nRecvLen=0;
					
					SysUtil::SysSleep(50);
				}
			}        
        }
        else
        {
            SysUtil::SysSleep(500);
        }
    }


	delete [] chRecvBuffer;
	delete [] chDest;
    SysUtil::SysSleep(50);

	return 0;
}

void CControlCmdHandler::Register()
{

	
	char chReq[1024]={0};
	memset(chReq,1024,0);
	
	int sendLen=0;
	memcpy(chReq,SYS_NET_MSGHEAD,8); //包头数据
	sendLen+=8;
	
	NET_PACKET_MSG* pMsg=(NET_PACKET_MSG*)(chReq+sendLen);
	
	NET_PACKET_HEAD* pHead=(NET_PACKET_HEAD*)(chReq+sendLen);
	pHead->msg_type=1;
	pHead->packet_len=sizeof(NVP_REGISTER);
	
	sendLen+=sizeof(NET_PACKET_HEAD);
	
	
	
	NVP_REGISTER* pTrigger=(NVP_REGISTER*)pMsg->msg_body;
	
	
	sprintf(pTrigger->device_tag,"WEIGHT");
	sprintf(pTrigger->device_id,"15000000");
	
	
	sendLen+=sizeof(NVP_REGISTER);
	
	memcpy(chReq+sendLen,SYS_NET_MSGTAIL,8); //包头数据
	sendLen+=8;	


	int nRet=SysUtil::SocketWrite(socket_,chReq,sendLen,100);
	
}

void CControlCmdHandler::SetParentHwnd(HWND hWnd)
{
	m_hWndParent = hWnd;
}

int CControlCmdHandler::ConnectToAlarmServer(short port, char* chIP)
{
    if (socket_ > 0)
    {
        SysUtil::CloseSocket(socket_);
        socket_ = -1;
    }

    socket_ = SysUtil::CreateSocket();
    if (SysUtil::ConnectSocket(socket_, chIP, port) == 0)
    {

		set_keep_live(3,5);

        m_bConnected = true;
        open(NULL);
        return 0;
    }
    else
    {
		SysUtil::CloseSocket(socket_);
		socket_=-1;
        return -1;
    }
}

int CControlCmdHandler::ReceiveTimer(int fd)
{
	int                   iret = 0;
    fd_set                rset;
    struct timeval        tv;
	
    FD_ZERO(&rset);
    FD_SET(fd, &rset);
    tv.tv_sec  = 0;
    tv.tv_usec = 100*1000;
	
    iret = select(fd + 1, &rset, NULL, NULL, &tv);
	
    return iret;
}



int CControlCmdHandler::SendHeartbeatMsg()
{
	char chReq[1024]={0};
	
	int sendLen=0;
	memcpy(chReq,SYS_NET_MSGHEAD,8); //包头数据
	sendLen+=8;
	
	NET_PACKET_HEAD* pHead=(NET_PACKET_HEAD*)(chReq+sendLen);
	pHead->msg_type=SYS_MSG_SYSTEM_MSG_KEEPLIVE;
	
	pHead->packet_len=0;
	
	sendLen+=sizeof(NET_PACKET_HEAD);
	
	memcpy(chReq+sendLen,SYS_NET_MSGTAIL,8); //包头数据
	sendLen+=8;	
	int nRet=SysUtil::SocketWrite(socket_,chReq,sendLen,100);

	return 0;
}

int CControlCmdHandler::set_keep_live(int keep_alive_times, int keep_alive_interval)
{

	TCP_KEEPALIVE_VALUE inKeepAlive = {0}; //输入参数
	unsigned long ulInLen = sizeof(TCP_KEEPALIVE_VALUE);
	
	TCP_KEEPALIVE_VALUE outKeepAlive = {0}; //输出参数
	unsigned long ulOutLen = sizeof(TCP_KEEPALIVE_VALUE);
	
	unsigned long ulBytesReturn = 0;
	
	//设置socket的keep alive为5秒，并且发送次数为3次
	inKeepAlive.on_off = keep_alive_times;
	inKeepAlive.keep_alive_interval = keep_alive_interval*1000; //两次KeepAlive探测间的时间间隔
	inKeepAlive.keep_alive_time = keep_alive_interval*1000;     //开始首次KeepAlive探测前的TCP空闭时间
	
	
	outKeepAlive.on_off = keep_alive_times;
	outKeepAlive.keep_alive_interval = keep_alive_interval*1000; //两次KeepAlive探测间的时间间隔
	outKeepAlive.keep_alive_time = keep_alive_interval*1000;     //开始首次KeepAlive探测前的TCP空闭时间
	
	
	if (WSAIoctl((unsigned int)socket_, SIO_KEEPALIVE_VALS,
		(LPVOID)&inKeepAlive, ulInLen,
		(LPVOID)&outKeepAlive, ulOutLen,
		&ulBytesReturn, NULL, NULL) == SOCKET_ERROR)
	{
		
	}

	return 0;
	
}

int CControlCmdHandler::VerifyRecvPacket(char *chRecvBuffer, char *chDest, int &nRecLen, int &nOffset, int &nLen)
{


	/*缓冲区长度小于最小帧长度*/
    if(nRecLen < sizeof(NET_PACKET_HEAD))
    {
        return -1;
    }
	
	int nHeadPos=SysUtil::SearchHeadPos(chRecvBuffer,nRecLen);
	if(nHeadPos<0)
	{
		return -1;
	}
	
	int nTailPos=SysUtil::SearchTailPos(chRecvBuffer,nRecLen);
	if(nTailPos<0)
	{
		return -1;
	}
	
	memcpy(chDest,chRecvBuffer+nHeadPos,(nTailPos-nHeadPos)+8);
	
	if(nRecLen-nTailPos-8 > 0)
	{
		//接收的数据可能是连续的包
		memcpy(chRecvBuffer,chRecvBuffer+nTailPos+8,nRecLen-nTailPos-8);
	}
	
	nOffset=nHeadPos=8;
	nLen=nTailPos-nHeadPos;
	nRecLen=nRecLen-nTailPos-8;
	
    return 0;
}

int CControlCmdHandler::SendData()
{

	char chReq[1024]={0};
	memset(chReq,1024,0);
	
	int sendLen=0;
	
	memcpy(chReq,SYS_NET_MSGHEAD,8); //包头数据
	sendLen+=8;
	
	NET_PACKET_MSG* pMsg=(NET_PACKET_MSG*)(chReq+sendLen);
	
	NET_PACKET_HEAD* pHead=(NET_PACKET_HEAD*)(chReq+sendLen);
	
	pHead=(NET_PACKET_HEAD*)(chReq+sendLen);
	pHead->msg_type=5;
	pHead->packet_len=sizeof(T_SysEventData);
	
	sendLen+=sizeof(NET_PACKET_HEAD);
	
	
	T_SysEventData* pReadData = (T_SysEventData*)(chReq+sendLen);
	
    strcpy(pReadData->event_id, "1500000002");
    strcpy(pReadData->device_tag, "WEIGHT");
	
	
	
    char szXMLGatherInfo[10 * 1024] = {0};
	sprintf(szXMLGatherInfo,
		"<WEIGHT>\n"
		"<GROSS_WT>%s</GROSS_WT>\n"
		"</WEIGHT>\n"
		, "67899"
		);
	
	pReadData->is_data_full=1;
    strcpy(pReadData->xml_data, szXMLGatherInfo);
	
	pReadData->xml_data_len=strlen(szXMLGatherInfo)+1;
	
	pHead->packet_len=sizeof(T_SysEventData)+pReadData->xml_data_len;
	
	sendLen+=sizeof(T_SysEventData)+pReadData->xml_data_len;
	
	memcpy(chReq+sendLen,SYS_NET_MSGTAIL,8); //包头数据
	sendLen+=8;	
	int nRet=SysUtil::SocketWrite(socket_,chReq,sendLen,100);
	return 0;

}
