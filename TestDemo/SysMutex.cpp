// SysMutex.cpp: implementation of the CSysMutex class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SysMutex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysMutex::CSysMutex()
{
	InitializeCriticalSection(&m_crtRealDataList);
}

CSysMutex::~CSysMutex()
{
	DeleteCriticalSection(&m_crtRealDataList);
}

void CSysMutex::Lock()
{
	EnterCriticalSection(&m_crtRealDataList);
}

void CSysMutex::UnLock()
{
	LeaveCriticalSection(&m_crtRealDataList);
}
