/////////////////////////////////////////////////////////////////////////////
// LanUps.h: interface for the CLanUps class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LANUPS_H__FB1E0949_127C_4ACB_9DBF_4235D62996AB__INCLUDED_)
#define AFX_LANUPS_H__FB1E0949_127C_4ACB_9DBF_4235D62996AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

#include "SocketUps.h"

#define UPSSENDTBL				8
#define UPSBUFFSIZE				8192

typedef struct tagUPSHeader
{
	BYTE	Data[1];
} UPSHeader;

/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CLanUps  
{
public:
	CLanUps(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CLanUps();

// Attributes
public:
	int		ThreadUse;
	int		Doing;
	int		AutoProc;

	ULONG	OpenWaitTime;
	ULONG	SendDelayTime;
	ULONG	RecvWaitTime;
	int		UpsIdleTime;
	int		LineOpenRetryCount;
	int		UpsFailCount;
	int		UpsBatLowCount;
	int		UpsAcStopCount;

	int		UPSRecvLength;										// Ups Recv Buffer
	BYTE	UPSRecvBuffer[UPSBUFFSIZE];			

	int		DATASendLength;										// Ups Send Buffer
	int		DATASendLengthTbl[UPSSENDTBL];
	int		DATASendInTbl;
	int		DATASendInTblTemp;
	int		DATASendOutTbl;
	int		DATASendOutTblTemp;
	BYTE	DATASendBuffer[UPSBUFFSIZE];
	BYTE	DATASendBufferTbl[UPSSENDTBL][UPSBUFFSIZE];

private:
	
// Operations
public:
	int		Open();
	int		Close();
	int		GetPowerStatus();
	int		PowerOff(int nWaitSec);

	int		UpsSendCmd();
	int		UpsSendCheck();
	int		UpsRecvCmd();
	int		UpsRecvBufferClear();

	int 	LineOpen(int OpenSec, int nAsyncFlag = FALSE);
	int 	LineClose(int CloseSec, int nAsyncFlag = FALSE);
	int 	SendData(BYTE *SendBuff, int Length, int SendSec);
	int 	RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 	GetStatus();

protected:

private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	char	m_IpAddress[256];
	int		m_PortNumber;

	CSocketUps*	m_pSocketUps;

	int		m_InitOpen;
	int		m_LineStatus;
};

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////
	UINT	UpsMainThread(LPVOID pParam);

#endif // !defined(AFX_LANUPS_H__FB1E0949_127C_4ACB_9DBF_4235D62996AB__INCLUDED_)
