#include "stdafx.h"
#include "SysUtil.h"
#include "SysMessage.h"
bool SysUtil::m_bInit=false;

SysUtil::SysUtil()
{
}

SysUtil::~SysUtil()
{
}

int SysUtil::CreateSocket()
{
    int nSocket;
    nSocket = (int) socket(PF_INET, SOCK_STREAM, 0);
    return nSocket;
}

int SysUtil::CheckSocketResult(int nResult)
{
    //	check result;
    if (nResult == -1)
        return 0;
    else
        return 1;
}

int SysUtil::CheckSocketValid(int nSocket)
{
    //	check socket valid
    if (nSocket == -1)
        return 0;
    else
        return 1;
}

int SysUtil::BindPort(int nSocket, int nPort)
{
    int rc;
    int optval = 1;
    rc = setsockopt(nSocket, SOL_SOCKET, SO_REUSEADDR,
                    (const char *) & optval, sizeof (int));

    if (!CheckSocketResult(rc))
	{
		return 0;
	}
        
    sockaddr_in name;
    memset(&name, 0, sizeof (sockaddr_in));
    name.sin_family = AF_INET;
    name.sin_port = htons((unsigned short) nPort);
    name.sin_addr.s_addr = INADDR_ANY;

    rc = bind(nSocket, (sockaddr *) & name, sizeof (sockaddr_in));
    if (!CheckSocketResult(rc))
	{
		return 0;
	}
        
    return 1;
}

int SysUtil::ConnectSocket(int nSocket, const char * szHost, int nPort)
{
/*	struct hostent* pRHost = NULL;

	try
	{
		pRHost=gethostbyname(szHost);
		if(pRHost==0)
		{
			return -1;
		}
	}catch(...)
	{
		return -1;
	}


//	struct in_addr in;





    memcpy(&in.s_addr, pRHost->h_addr_list[0],sizeof (in.s_addr));
 */   sockaddr_in name;
    memset(&name,0,sizeof(sockaddr_in));
    name.sin_family=AF_INET;
    name.sin_port=htons((unsigned short)nPort);
//	name.sin_addr.s_addr=in.s_addr;
	name.sin_addr.s_addr=inet_addr(szHost);

	int rc=connect((SOCKET)nSocket,(sockaddr *)&name,sizeof(sockaddr_in));

	if(rc>=0)
		return 0;
	return -1;

}

int SysUtil::CloseSocket(int nSocket)
{
    int rc=1;
	if(!CheckSocketValid(nSocket))
	{
		return rc;
	}

	int n=shutdown((SOCKET)nSocket,SD_BOTH);
	if(n==-1)
	{
		int mmm=0;
	}

	Sleep(10);
	int nn=closesocket((SOCKET)nSocket);
	if(nn==-1)
	{
		int mmm=0;
	}


	return 0;
}

int SysUtil::ListenSocket(int nSocket, int nMaxQueue)
{
    int rc = 0;
    rc = listen(nSocket, nMaxQueue);
    return CheckSocketResult(rc);
}

void SysUtil::SetSocketNotBlock(int nSocket)
{
    //	改变文件句柄为非阻塞模式
	ULONG optval2=1;
	ioctlsocket((SOCKET)nSocket,FIONBIO,&optval2);
}

int SysUtil::CheckSocketError(int nResult)
{
	//	检查非阻塞套接字错误
	if(nResult>0)
		return -1;
	if(nResult==0)
		return 0;
	
	if(WSAGetLastError()==WSAEWOULDBLOCK)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int SysUtil::SocketWrite(int nSocket, char * pBuffer, int nLen, int nTimeout)
{
    int nOffset=0;
	int nWrite;
	int nLeft=nLen;
	int nLoop=0;
	int nTotal=0;
	

	/*
	 *	这里的nTimeout其实是重发次数，而不是实际的时间
	 */
	int nNewTimeout=nTimeout*10;
	while((nLoop<=nNewTimeout) && (nLeft>0))
	{
		nWrite=send(nSocket,pBuffer+nOffset,nLeft,0);
		if(nWrite==0)
		{
			return -1;
		}

		if(nWrite == SOCKET_ERROR)
		{
			if(WSAGetLastError()!=WSAEWOULDBLOCK)
			{	
				return -1;
			}
		}

		if(nWrite<0)
		{
			return nWrite;
		}
		
		nOffset+=nWrite;
		nLeft-=nWrite;
		nTotal+=nWrite;
		if(nLeft>0)
		{
			//	延时100ms
			SysSleep(10);
		}
		nLoop++;
	}

	return nTotal;
}

int SysUtil::SocketRead(int nSocket, void * pBuffer, int nLen)
{
    if(nSocket==-1)
		return -1;
	int len=0;

	len=recv((SOCKET) nSocket,(char *)pBuffer,nLen,0);

	if(len==0)
	{
		return -1;
	}

	if(len==-1)
	{

		int localError=WSAGetLastError();
		if(localError==WSAEWOULDBLOCK)
		{
			return 0;
		}

		return -1;
	}

	if(len>0)
		return len;
	else
		return -1;
}

void SysUtil::SysSleep(long ms)
{
	Sleep(ms);
}


int SysUtil::ConnectSocket(int nSocket, unsigned long serverip, int port)
{
	struct sockaddr_in ServerAddress;
    ServerAddress.sin_family=AF_INET;
    ServerAddress.sin_addr.s_addr = serverip;
    ServerAddress.sin_port=ntohs(port);

	int rc=connect((SOCKET)nSocket,(sockaddr *)&ServerAddress,sizeof(sockaddr_in));
	int nError=GetLastError();
	if(rc>=0)
		return 0;
	return -1;
}

int SysUtil::InitNetLib()
{
	if(!m_bInit)
	{
		m_bInit=true;

		WSADATA wsd;
		int nRet = ::WSAStartup(MAKEWORD(2, 2), &wsd);
		if(nRet != 0)
		{
			return -1;
		}
	}

	return 0;
}

int SysUtil::ReleaseNetLib()
{
	::WSACleanup();
	return 0;
}

int SysUtil::ReceiveTimer(int fd)
{
	int            iRet=0;
	fd_set          rset;
	struct timeval tv;
	
	FD_ZERO(&rset);
	FD_SET(fd,&rset);
	tv.tv_sec=2;
	tv.tv_usec=0;
	
	iRet=select(fd+1,&rset,NULL,NULL,&tv);
	if(FD_ISSET(fd,&rset))
	{
		return 1;
	}
	return 0;
}

int SysUtil::SearchHeadPos(char *chBuffer, int nDataLen)
{

	int end,i,j;
	end = nDataLen - 8;  /* 计算结束位置*/
	
	if ( end > 0 )              
	{
		for ( i = 0; i <= end; i++ )
		{
			//循环比较
			for ( j = i; chBuffer[j] == SYS_NET_MSGHEAD[j-i]; j++ )
			{
				if ( SYS_NET_MSGHEAD[j-i+1] == '\0') 
				{
					return i;           /* 找到了子字符串   */
				}
			}
		}
	}
	
	return -1;
}

int SysUtil::SearchTailPos(char *chBuffer, int nDataLen)
{
	int end,i,j;
	end = nDataLen - 8;  /* 计算结束位置*/
	
	if ( end > 0 )              
	{
		for ( i = 0; i <= end; i++ )
		{
			//循环比较
			for ( j = i; chBuffer[j] == SYS_NET_MSGTAIL[j-i]; j++ )
			{
				if ( SYS_NET_MSGTAIL[j-i+1] == '\0') 
				{
					return i;           /* 找到了子字符串   */
				}
			}
		}
	}
	
	return -1;

}
