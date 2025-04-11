// AlarmMsgHelper.cpp: implementation of the CAlarmMsgHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AlarmMsgHelper.h"
#include "SysUtil.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char CAlarmMsgHelper::Msg_Buffer_[sizeof(NET_PACKET_MSG)*MAX_BUFFERD_MESSAGE_NUMBER];
std::vector<NET_PACKET_MSG*> CAlarmMsgHelper::msgVec_;
int CAlarmMsgHelper::m_nMsgBufferCount=0;
CSysMutex   CAlarmMsgHelper::msg_mutex_;

int CAlarmMsgHelper::m_Socket;

CAlarmMsgHelper::CAlarmMsgHelper()
{

}

CAlarmMsgHelper::~CAlarmMsgHelper()
{

}

void CAlarmMsgHelper::InitMessage()
{
	for (int i = 0; i < MAX_BUFFERD_MESSAGE_NUMBER; i++)
	{
		char* chMsgBuffer=Msg_Buffer_;
		NET_PACKET_MSG* pMsg=(NET_PACKET_MSG*)(chMsgBuffer+i*sizeof(NET_PACKET_MSG));
		msgVec_.push_back(pMsg);
	}
	
	return;
}

int CAlarmMsgHelper::Get_Message(NET_PACKET_MSG *&pMsg, int &nMsgIndex)
{
	CMutexGuard guard(msg_mutex_);

	pMsg=msgVec_[m_nMsgBufferCount];
	
	nMsgIndex=m_nMsgBufferCount;

	m_nMsgBufferCount++;
	if(m_nMsgBufferCount>MAX_BUFFERD_MESSAGE_NUMBER-1)
	{
		m_nMsgBufferCount=0;
	}
	
	return 0;
}

int CAlarmMsgHelper::RegisterAlarmConnection(int nSocket)
{
	m_Socket=nSocket;
	return 0;
}

int CAlarmMsgHelper::SendEvent(int nMsgIndex)
{
	NET_PACKET_MSG* pMsg=NULL;
	Get_Message(nMsgIndex,pMsg);

	if(pMsg)
	{
		int nSendLen=pMsg->msg_head.packet_len+sizeof(NET_PACKET_MSG);
		SysUtil::SocketWrite(m_Socket,(char*)pMsg,nSendLen,100);
	}
	return 0;
}

int CAlarmMsgHelper::Get_Message(int nMsgIndex,NET_PACKET_MSG *&pMsg)
{
	CMutexGuard guard(msg_mutex_);
	
	if(nMsgIndex > -1 && nMsgIndex < MAX_BUFFERD_MESSAGE_NUMBER)
	pMsg=msgVec_[nMsgIndex];
	
	return 0;
}

int CAlarmMsgHelper::PublishEvent(int nEventType)
{

	return 0;
}




