#if !defined _SYS_MESSAGE_H_
#define _SYS_MESSAGE_H_

#pragma warning(disable: 4200)

#include <map>
#include <vector>
#include <list>
using namespace std;

#define MAX_MSG_BODYLEN  20*1024

#define SYS_NET_MSGHEAD                "0XJZTECH"
#define SYS_NET_MSGTAIL                "NJTECHJZ"


#define SYS_SYSTEM                             0X00

#define SYS_MSG_SYSTEM_MSG_KEEPLIVE            SYS_SYSTEM+3
#define SYS_MSG_SYSTEM_MSG_KEEPLIVE_ACK        SYS_SYSTEM+4

#define SYS_MSG_SYSTEM_CTRL                   SYS_SYSTEM+0X09 


#define SYS_MSG_EVENT_SUBSCRIBE               SYS_SYSTEM+0X11                                        //事件订阅
#define SYS_MSG_EVENT_SUBSCRIBE_ACK           SYS_SYSTEM+0X12                                        


#define SYS_MSG_SYSEVENT_PUBLISH                  SYS_SYSTEM+0X20                                     //事件发布
#define SYS_EVENT_TYPE_UPLOAD_CUSTOMS_DATA  SYS_SYSTEM+0X01


#define EVENT_SERVICE_TYPE_CLIENT              0X00
#define EVENT_SERVICE_TYOE_CONTROLER           0X01

struct NET_PACKET_HEAD
{
    int msg_type;
    int packet_len;
	char version_no[16];
    int proxy_count;
    int net_proxy[10];
};

struct NET_PACKET_MSG
{
    NET_PACKET_HEAD msg_head;
    char msg_body[MAX_MSG_BODYLEN];
};

struct NVP_REGISTER
{
	char                     device_tag[32];
	char                     device_id[32];
};



struct T_SysEventData
{
	char                     event_id[32];
	int                       is_data_full;
	char                      device_tag[32];                                  //1,启动录像 2，停止录像
	int                       xml_data_len;
	char                      xml_data[];
};



struct NET_EVENT_SUBSCRIBE
{
    char     client_type;
    char     user_name[32];
    char     pass_word[32];
    char     event_serial[32];
	
};

struct NET_EVENT_SUBSCRIBE_ACK
{
    int result;
};


struct T_Upload_Customs_Data
{
    char szChannelNo[32];
    char szSequenceNo[32];
    int  is_data_full;                            //数据是否完整
    int  nCustomsDataLen;
    char szCustomsData[];
};

struct NET_EVENT
{
    char main_type[32];
    int  sub_type;
};


struct NET_EVENT_UPLOAD_CUSTOMSDATA
{
    char main_type[32];                   //用来区别各个不同的通道集中控制器
    int  sub_type;                        //区别不同的事件，对应不同的结构
    T_Upload_Customs_Data customs_data;
};

struct NET_SYS_CTRL 
{
	char event_id[32];
};
#endif