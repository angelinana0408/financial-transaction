/////////////////////////////////////////////////////////////////////////////
// NMSWORK.h: interface for the CNMSWORK class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_NMSWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
#define AFX_NMSWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

class CNmsDc;

class AFX_EXT_CLASS CNmsWork
{
public:
	CNmsWork(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CNmsWork();

	int 		Initialize();
	int 		InitializeIP(LPCTSTR szIpAddress, int nPortNumber);
	int 		Deinitialize();
	int 		SendData(BYTE *SendBuff, int Length);
	int 		GetUseStatus();

private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	CNmsDc*		m_pNmsDc;
};

#endif // !defined(AFX_NMSWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
