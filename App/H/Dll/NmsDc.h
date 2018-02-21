// NmsDc.h: interface for the CDcNms class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMSDC_H__8EF7D75C_C4A9_4730_8711_F4A777A13EFB__INCLUDED_)
#define AFX_NMSDC_H__8EF7D75C_C4A9_4730_8711_F4A777A13EFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

#include "SocketNms.h"

#define NMSSENDTBL			8
#define NMSBUFFSIZE			16384

/////////////////////////////////////////////////////////////////////////////
class AFX_EXT_CLASS CNmsDc
{
public:
	CNmsDc(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CNmsDc();

// Attributes
public:
	int		ThreadUse;
	int		Doing;
	int		AutoProc;
	
	ULONG	OpenWaitTime;
	ULONG	RecvWaitTime;
	ULONG	SendDelayTime;

	int		NMSRecvLength;										// Nms Recv Buffer
	BYTE	NMSRecvBuffer[NMSBUFFSIZE];			

	int		DATASendLength;										// Nms Send Buffer
	int		DATASendLengthTbl[NMSSENDTBL];
	int		DATASendInTbl;
	int		DATASendInTblTemp;
	int		DATASendOutTbl;
	int		DATASendOutTblTemp;
	BYTE	DATASendBuffer[NMSBUFFSIZE];
	BYTE	DATASendBufferTbl[NMSSENDTBL][NMSBUFFSIZE];
	
// Operations
public:
	int 	Initialize();
	int 	InitializeIP(LPCTSTR szIpAddress, int nPortNumber);
	int 	Deinitialize();
	int 	SendData(BYTE *SendBuff, int Length);
	int 	GetUseStatus();

// Private Interface
	int		NmsSendCmd();
	int		NmsSendCheck();
	int		NmsRecvCmd();
	int		NmsRecvBufferClear();

	int 	LineOpen(int OpenSec, int nAsyncFlag = FALSE);
	int 	LineClose(int CloseSec, int nAsyncFlag = FALSE);
	int 	SendData(BYTE *SendBuff, int Length, int SendSec);
	int 	RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 	GetStatus();
	int 	GetCloseStatus();
	
private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	CSocketNms* m_pSocketNms;

	char	m_IpAddress[256];
	int		m_PortNumber;

	int		m_InitOpen;
	int		m_LineStatus;
	int		m_LineCloseStatus;

	int		m_nNmsKind;
};

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////
	UINT	NmsMainThread(LPVOID pParam);
	UINT	NmsMainThreadSendRecv(LPVOID pParam);

#endif // !defined(AFX_NMSDC_H__8EF7D75C_C4A9_4730_8711_F4A777A13EFB__INCLUDED_)
