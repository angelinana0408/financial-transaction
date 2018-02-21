#include "stdafx.h"
#include "CmnLibEx.h"

//////////////////////////////////////////////////////////////////////
//	Shared Define
//////////////////////////////////////////////////////////////////////
#define	_REGKEY_SHAREDDATA			_T("SOFTWARE\\ATM\\SHAREDDATA")

CRITICAL_SECTION	m_csLock;									// 2005.10.07

//////////////////////////////////////////////////////////////////////
int WINAPI OpenShareData()
{
	InitializeCriticalSection(&m_csLock);
	return TRUE;
}

int WINAPI CloseShareData()
{
	DeleteCriticalSection(&m_csLock);
	return TRUE;
}

int WINAPI ClearShareData()
{
	return TRUE;
}

CString WINAPI GetShareData(CString szGetDataName)
{
	CString	strRet("");

	EnterCriticalSection(&m_csLock);
	strRet = RegGetStr(_REGKEY_SHAREDDATA, szGetDataName);
	LeaveCriticalSection(&m_csLock);

	return strRet;
}

int WINAPI SetShareData(CString szSetDataName, CString szSetDataValue)
{
	int		nRet = FALSE;

	EnterCriticalSection(&m_csLock);
	nRet = RegSetStr(_REGKEY_SHAREDDATA, szSetDataName, szSetDataValue);
	LeaveCriticalSection(&m_csLock);

	return nRet;
}
