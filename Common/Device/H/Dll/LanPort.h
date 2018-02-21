/////////////////////////////////////////////////////////////////////////////
// LanPort.h: interface for the CLanPort class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LANPORT_H__6ADEA338_333D_4AE4_B49B_48B555E40FFF__INCLUDED_)
#define AFX_LANPORT_H__6ADEA338_333D_4AE4_B49B_48B555E40FFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Profiledef.h"

class CLanUps;

class AFX_EXT_CLASS CLanPort  
{
public:
	CLanPort(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CLanPort();

	int			UpsClearErrorCode();

	CString		UpsGetErrorCode();
	int			UpsGetDeviceStatus();
	int			UpsGetPowerStatus();

	int			UpsInitialize();
	int			UpsDeinitialize();
	int			UpsPowerOff(int nWaitSec);
	int			UpsPowerRestart(int nWaitSec, int nRestartSec);

private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	CLanUps*	m_pUps;
};

#endif // !defined(AFX_LANPORT_H__6ADEA338_333D_4AE4_B49B_48B555E40FFF__INCLUDED_)
