/////////////////////////////////////////////////////////////////////////////
// LanPort.cpp: implementation of the CLanPort class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\LanUps.h"
#include "..\..\H\Dll\LanPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CLanPort::CLanPort(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		m_pUps = new CLanUps(m_DeviceInfo, m_NetworkInfo);
	}
}

CLanPort::~CLanPort()
{
	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		if (m_pUps)		{	delete m_pUps;		m_pUps		= NULL;	}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	UPS
/////////////////////////////////////////////////////////////////////////////
int CLanPort::UpsClearErrorCode()
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		nReturn = TRUE;
	}

	return nReturn;
}

CString CLanPort::UpsGetErrorCode()
{
	CString strReturn("");

	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		strReturn = "0000000";									// Error Code Adjust
	}

	return strReturn;
}

int CLanPort::UpsGetDeviceStatus()
{
	int		nReturn = FALSE;

	nReturn = NORMAL;
	
	return nReturn;
}

int CLanPort::UpsGetPowerStatus()
{
	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		return m_pUps->GetPowerStatus();
	}
	else
		return POWER_NORMAL;
}

int CLanPort::UpsInitialize()
{
	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		return m_pUps->Open();
	}
	else
		return FALSE;
}

int CLanPort::UpsDeinitialize()
{
	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		return m_pUps->Close();
	}
	else
		return FALSE;
}

int CLanPort::UpsPowerOff(int nWaitSec)
{
	if (m_DeviceInfo.PowerHandler == EUL_TYPE)
	{
		return m_pUps->PowerOff(nWaitSec);
	}
	else
		return FALSE;
}

int CLanPort::UpsPowerRestart(int nWaitSec, int nRestartSec)
{
	return FALSE;
}
