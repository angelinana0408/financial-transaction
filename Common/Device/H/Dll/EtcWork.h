/////////////////////////////////////////////////////////////////////////////
// EtcWork.h: interface for the CEtcWork class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_ETCWORK_H__B5A0C33A_15C0_42A0_BCAB_3D0CFA712938__INCLUDED_)
#define AFX_ETCWORK_H__B5A0C33A_15C0_42A0_BCAB_3D0CFA712938__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

class CEtcDvr;

class AFX_EXT_CLASS CEtcWork  
{
public:
	CEtcWork(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CEtcWork();
	
// Operations
public:
	int 		Initialize();
	int 		InitializeIP(LPCTSTR szIpAddress, int nPortNumber);
	int 		Deinitialize();
	int 		SendData(BYTE *SendBuff, int Length);
	int 		GetUseStatus();

private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	CEtcDvr*	m_pEtcDvr;
};

#endif // !defined(AFX_ETCWORK_H__B5A0C33A_15C0_42A0_BCAB_3D0CFA712938__INCLUDED_)
