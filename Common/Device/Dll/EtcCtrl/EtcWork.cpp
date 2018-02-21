/////////////////////////////////////////////////////////////////////////////
// EtcWork.cpp: implementation of the CEtcWork class.
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\EtcDvr.h"
#include "..\..\H\Dll\EtcWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CEtcWork::CEtcWork(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	m_pEtcDvr		= NULL;

	if (m_DeviceInfo.EtcComKind)
	{
		m_pEtcDvr	= new CEtcDvr(m_DeviceInfo, m_NetworkInfo);
	}
}

CEtcWork::~CEtcWork()
{
	if (m_DeviceInfo.EtcComKind)
	{
		if (m_pEtcDvr)	{	delete m_pEtcDvr;	m_pEtcDvr	= NULL;	}
	}
}

int CEtcWork::Initialize()
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtcDvr->Initialize();
	}
	
	return nReturn;
}

int CEtcWork::InitializeIP(LPCTSTR szIpAddress, int nPortNumber)
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtcDvr->InitializeIP(szIpAddress, nPortNumber);
	}

	return nReturn;
}

int CEtcWork::Deinitialize()
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtcDvr->Deinitialize();
	}

	return nReturn;
}

int CEtcWork::SendData(BYTE *SendBuff, int Length)
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtcDvr->SendData(SendBuff, Length);
	}

	return nReturn;
}
	
int CEtcWork::GetUseStatus()
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtcDvr->GetUseStatus();
	}

	return nReturn;
}

