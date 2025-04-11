// SysMutex.h: interface for the CSysMutex class.
//
//////////////////////////////////////////////////////////////////////

#if !defined _SYS_MUTEX_H_
#define _SYS_MUTEX_H_


class CSysMutex  
{
public:
	void UnLock();
	void Lock();
	CSysMutex();
	virtual ~CSysMutex();

private:
	CRITICAL_SECTION    m_crtRealDataList;

};

#endif 
