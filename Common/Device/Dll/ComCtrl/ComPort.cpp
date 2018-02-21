/////////////////////////////////////////////////////////////////////////////
// ComPort.cpp: implementation of the CComPort class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\ComDvr.h"
#include "..\..\H\Dll\ComEtc.h"
#include "..\..\H\Dll\ComUps.h"
#include "..\..\H\Dll\ComPort.h"
#include "..\..\H\Dll\ComRic.h"
#include "..\..\H\Dll\ComIrd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CComPort::CComPort(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	if (m_DeviceInfo.DvrDevice)
	{
		m_pDvr = new CComDvr(m_DeviceInfo, m_NetworkInfo);
	}

	if (m_DeviceInfo.EtcComKind)
	{
		m_pEtc = new CComEtc(m_DeviceInfo, m_NetworkInfo);
	}

	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		m_pUps = new CComUps(m_DeviceInfo, m_NetworkInfo);
	}

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		m_pRic = new CComRic(m_DeviceInfo, m_NetworkInfo);
	}

	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		m_pIrd = new CComIrd(m_DeviceInfo, m_NetworkInfo);
	}
}

CComPort::~CComPort()
{
	if (m_DeviceInfo.DvrDevice)
	{
		if (m_pDvr)		{	delete m_pDvr;		m_pDvr		= NULL;	}
	}

	if (m_DeviceInfo.EtcComKind)
	{
		if (m_pEtc)		{	delete m_pEtc;		m_pEtc	= NULL;	}
	}

	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		if (m_pUps)		{	delete m_pUps;		m_pUps		= NULL;	}
	}

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		if (m_pRic)		{	delete m_pRic;		m_pRic		= NULL;	}
	}

	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		if (m_pIrd)		{	delete m_pIrd;		m_pIrd		= NULL;	}
	}
}

/////////////////////////////////////////////////////////////////////////////
//	DVR
/////////////////////////////////////////////////////////////////////////////
int CComPort::DvrInitialize()
{
	int nReturn = FALSE;

	if (m_DeviceInfo.DvrDevice)
	{
		nReturn = m_pDvr->Open();
	}

	return nReturn;
}

int CComPort::DvrDeinitialize()
{
	int nReturn = FALSE;

	if (m_DeviceInfo.DvrDevice)
	{
		nReturn = m_pDvr->Close();
	}

	return nReturn;
}

int CComPort::DvrWrite(BYTE *SendBuff, int Length, int nSendDelayTime)
{
	int nReturn = FALSE;

	if (m_DeviceInfo.DvrDevice)
	{
		nReturn = m_pDvr->Write(SendBuff, Length, nSendDelayTime);
	}

	return nReturn;
}

CString CComPort::DvrGetStatus()
{
	CString	strReturn("");

	if (m_DeviceInfo.DvrDevice)
	{
		strReturn = m_pDvr->GetStatus();
	}
	else
		strReturn = MakeUnPack(ZERO5, 5);

	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	ETC(SECURITY)
/////////////////////////////////////////////////////////////////////////////
int CComPort::EtcInitialize()
{
	int nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtc->Open();
	}

	return nReturn;
}

int CComPort::EtcDeinitialize()
{
	int nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtc->Close();
	}

	return nReturn;
}

int CComPort::EtcWrite(BYTE *SendBuff, int Length, int nSendDelayTime)
{
	int nReturn = FALSE;

	if (m_DeviceInfo.EtcComKind)
	{
		nReturn = m_pEtc->Write(SendBuff, Length, nSendDelayTime);
	}

	return nReturn;
}

CString CComPort::EtcGetStatus()
{
	CString	strReturn("");

	if (m_DeviceInfo.EtcComKind)
	{
		strReturn = m_pEtc->GetStatus();
	}
	else
		strReturn = ZERO8;

	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	UPS
/////////////////////////////////////////////////////////////////////////////
int CComPort::UpsClearErrorCode()
{
	int		nReturn = FALSE;

	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		nReturn = TRUE;
	}

	return nReturn;
}

CString CComPort::UpsGetErrorCode()
{
	CString strReturn("");

	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		strReturn = "0000000";									// Error Code Adjust
	}

	return strReturn;
}

int CComPort::UpsGetDeviceStatus()
{
	int		nReturn = FALSE;

	nReturn = NORMAL;
	
	return nReturn;
}

int CComPort::UpsGetPowerStatus()
{
	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		return m_pUps->GetPowerStatus();
	}
	else
		return POWER_NORMAL;
}

int CComPort::UpsInitialize()
{
	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		return m_pUps->Open();
	}
	else
		return FALSE;
}

int CComPort::UpsDeinitialize()
{
	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		return m_pUps->Close();
	}
	else
		return FALSE;
}

int CComPort::UpsPowerOff(int nWaitSec)
{
	if (m_DeviceInfo.PowerHandler == EUC_TYPE)
	{
		return m_pUps->PowerOff(nWaitSec);
	}
	else
		return FALSE;
}

int CComPort::UpsPowerRestart(int nWaitSec, int nRestartSec)
{
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	RIC
/////////////////////////////////////////////////////////////////////////////
int CComPort::RicClearErrorCode()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->ClearErrorCode();
	}
	else
		return FALSE;
}

CString CComPort::RicGetErrorCode()
{
	CString strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->GetErrorCode();
	}
	else
		return strReturn;
}

int CComPort::RicGetDeviceStatus()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->GetDeviceStatus();
	}
	else
		return FALSE;
}

int CComPort::RicGetDataStatus()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->GetDataStatus();
	}
	else
		return FALSE;
}

CString CComPort::RicGetIcCardSerialNo()
{
	CString strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->GetIcCardSerialNo();
	}
	else
		return strReturn;
}

int CComPort::RicInitialize()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->Open();
	}
	else
		return FALSE;
}

int CComPort::RicDeinitialize()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->Close();
	}
	else
		return FALSE;
}

int CComPort::RicRfReset()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->RfReset();
	}
	else
		return FALSE;
}

int CComPort::RicDetectCard()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->DetectCard();
	}
	else
		return FALSE;
}

int CComPort::RicCpuReset()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->CpuReset();
	}
	else
		return FALSE;
}

CString CComPort::RicSelectFile()
{
	CString	strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->SelectFile();
	}
	else
		return strReturn;
}

int CComPort::RicVerifyPIN(LPCTSTR szPinData)
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->VerifyPIN(szPinData);
	}
	else
		return FALSE;
}

CString CComPort::RicReadBalance()
{
	CString	strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->ReadBalance();
	}
	else
		return strReturn;
}

CString CComPort::RicInitializeForLoad(LPCTSTR szAmount, LPCTSTR szTerminalID, int nKeyIndex)
{
	CString	strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->InitializeForLoad(szAmount, szTerminalID, nKeyIndex);
	}
	else
		return strReturn;
}

CString CComPort::RicLoadEP(LPCTSTR szDate, LPCTSTR szTime, LPCTSTR szMac2)
{
	CString	strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->LoadEP(szDate, szTime, szMac2);
	}
	else
		return strReturn;
}

CString CComPort::RicGetRejectCode()
{
	CString strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->GetRejectCode();
	}
	else
		return strReturn;
}

int CComPort::RicEntryEnable()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->EntryEnable();
	}
	else
		return FALSE;
}

int CComPort::RicEntryDisable()
{
	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->EntryDisable();
	}
	else
		return FALSE;
}

CString CComPort::RicCheckDeviceAction(int nCheckTime)
{
	CString strReturn("");

	if (m_DeviceInfo.RicDevice == RIC_CBTS)
	{
		return m_pRic->CheckDeviceAction(nCheckTime);
	}
	else
		return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	IRD
/////////////////////////////////////////////////////////////////////////////
int CComPort::IrdClearErrorCode()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->ClearErrorCode();
	}
	else
		return FALSE;
}

CString CComPort::IrdGetErrorCode()
{
	CString strReturn("");

	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->GetErrorCode();
	}
	else
		return strReturn;
}

int CComPort::IrdGetDeviceStatus()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->GetDeviceStatus();
	}
	else
		return FALSE;
}

int CComPort::IrdGetDataStatus()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->GetDataStatus();
	}
	else
		return FALSE;
}

CString CComPort::IrdGetData()
{
	CString strReturn("");

	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->GetData();
	}
	else
		return strReturn;
}

int CComPort::IrdInitialize()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->Open();
	}
	else
		return FALSE;
}

int CComPort::IrdDeinitialize()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->Close();
	}
	else
		return FALSE;
}

int CComPort::IrdRead()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->Read();
	}
	else
		return FALSE;
}

int CComPort::IrdEntryEnable()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->EntryEnable();
	}
	else
		return FALSE;
}

int CComPort::IrdEntryDisable()
{
	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->EntryDisable();
	}
	else
		return FALSE;
}

CString CComPort::IrdCheckDeviceAction(int nCheckTime)
{
	CString strReturn("");

	if (m_DeviceInfo.IrdDevice == IRD_CBTS)
	{
		return m_pIrd->CheckDeviceAction(nCheckTime);
	}
	else
		return strReturn;
}

