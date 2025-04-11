// AlarmMsgHelper.h: interface for the CAlarmMsgHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined _ALARM_MESSAGE_HELPER_H_
#define _ALARM_MESSAGE_HELPER_H_

#include "MutexGuard.h"
#include "SysMessage.h"
#include <vector>
using namespace std;

#define MAX_BUFFERD_MESSAGE_NUMBER                (100)

class CAlarmMsgHelper  
{
public:
	CAlarmMsgHelper();
	virtual ~CAlarmMsgHelper();


	static int RegisterAlarmConnection(int nSocket);
	static int Get_Message(NET_PACKET_MSG *&pMsg,int& nMsgIndex);
	static void InitMessage();
	static int SendEvent(int nMsgIndex);
public:
	static int PublishEvent(int nEventType);
	static int Get_Message(int nMsgIndex,NET_PACKET_MSG *&pMsg);

	static int  m_Socket;
	static CSysMutex                    msg_mutex_;
	static std::vector<NET_PACKET_MSG*> msgVec_;
	static int m_nMsgBufferCount;
	static char Msg_Buffer_[sizeof(NET_PACKET_MSG)*MAX_BUFFERD_MESSAGE_NUMBER];

};

#endif 
