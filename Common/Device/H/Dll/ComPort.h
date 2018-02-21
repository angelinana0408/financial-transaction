//////////////////////////////////////////////////////////////////////
// ComPort.h: interface for the CComPort class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPORT_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
#define AFX_COMPORT_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Profiledef.h"

class CComDvr;
class CComEtc;
class CComUps;
class CComRic;
class CComIrd;

class AFX_EXT_CLASS CComPort  
{
public:
	CComPort(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CComPort();

	int 		DvrInitialize();
	int 		DvrDeinitialize();
	int 		DvrWrite(BYTE *SendBuff, int Length, int nSendDelayTime);
	CString		DvrGetStatus();

	int 		EtcInitialize();
	int 		EtcDeinitialize();
	int 		EtcWrite(BYTE *SendBuff, int Length, int nSendDelayTime);
	CString		EtcGetStatus();

	int 		UpsClearErrorCode();
	CString		UpsGetErrorCode();
	int 		UpsGetDeviceStatus();
	int 		UpsGetPowerStatus();
	int 		UpsInitialize();
	int 		UpsDeinitialize();
	int 		UpsPowerOff(int nWaitSec);
	int 		UpsPowerRestart(int nWaitSec, int nRestartSec);

	int 		RicClearErrorCode();
	CString		RicGetErrorCode();
	int 		RicGetDeviceStatus();
	int			RicGetDataStatus();
	CString		RicGetIcCardSerialNo();
	int 		RicInitialize();
	int 		RicDeinitialize();
	int 		RicRfReset();
	int			RicDetectCard();
	int 		RicCpuReset();
	CString		RicSelectFile();
	int			RicVerifyPIN(LPCTSTR szPinData);
	CString		RicReadBalance();
	CString		RicInitializeForLoad(LPCTSTR szAmount, LPCTSTR szTerminalID, int nKeyIndex);
	CString		RicLoadEP(LPCTSTR szDate, LPCTSTR szTime, LPCTSTR szMac2);
	CString		RicGetRejectCode();
	int 		RicEntryEnable();
	int 		RicEntryDisable();
	CString		RicCheckDeviceAction(int nCheckTime);

	int 		IrdClearErrorCode();
	CString		IrdGetErrorCode();
	int 		IrdGetDeviceStatus();
	int			IrdGetDataStatus();
	CString		IrdGetData();
	int 		IrdInitialize();
	int 		IrdDeinitialize();
	int 		IrdRead();
	int 		IrdEntryEnable();
	int 		IrdEntryDisable();
	CString		IrdCheckDeviceAction(int nCheckTime);

private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	CComDvr*	m_pDvr;
	CComEtc*	m_pEtc;
	CComUps*	m_pUps;
	CComRic*	m_pRic;
	CComIrd*	m_pIrd;
};

#endif // !defined(AFX_COMPORT_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
