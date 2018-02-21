/////////////////////////////////////////////////////////////////////////////
// NmsWork.cpp: implementation of the CNmsWork class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\NmsDc.h"
#include "..\..\H\Dll\NmsWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CNmsWork::CNmsWork(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	m_pNmsDc		= NULL;

	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			m_pNmsDc	= new CNmsDc(m_DeviceInfo, m_NetworkInfo);
			break;
		default:
			break;
	}
}

CNmsWork::~CNmsWork()
{
	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			if (m_pNmsDc)	{	delete m_pNmsDc;	m_pNmsDc	= NULL;	}
			break;
		default:
			break;
	}
}

int CNmsWork::Initialize()
{
	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			return (m_pNmsDc->Initialize());
			break;
		default:
			break;
	}
	return FALSE;
}

int CNmsWork::InitializeIP(LPCTSTR szIpAddress, int nPortNumber)
{
	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			return (m_pNmsDc->InitializeIP(szIpAddress, nPortNumber));
			break;
		default:
			break;
	}
	return FALSE;
}

int CNmsWork::Deinitialize()
{
	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			return (m_pNmsDc->Deinitialize());
			break;
		default:
			break;
	}
	return FALSE;
}

int CNmsWork::SendData(BYTE *SendBuff, int Length)
{
	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			return (m_pNmsDc->SendData(SendBuff, Length));
			break;
		default:
			break;
	}
	return FALSE;
}
	
int CNmsWork::GetUseStatus()
{
	switch (m_DeviceInfo.NmsDevice)
	{
		case NMS_ZL:
			break;
		case NMS_DC:
		case NMS_ICBC:
		case NMS_RCBFZ:
		case NMS_CBHEB:
			return (m_pNmsDc->GetUseStatus());
			break;
		default:
			break;
	}
	return FALSE;
}

