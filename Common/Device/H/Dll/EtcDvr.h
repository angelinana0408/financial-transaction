// EtcDvr.h: interface for the CEtcDvr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ETCDVR_H__22230EE0_E5FB_4031_A82B_6DF5BD4D4B7C__INCLUDED_)
#define AFX_ETCDVR_H__22230EE0_E5FB_4031_A82B_6DF5BD4D4B7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

#include "SocketDvr.h"

#define ETCSENDTBL				8
#define ETCBUFFSIZE				16384

class AFX_EXT_CLASS CEtcDvr  
{
public:
	CEtcDvr(DEVICEINFO DEVICE, NETWORKINFO NETWORK);
	virtual ~CEtcDvr();
// Attributes
public:
	int		ThreadUse;
	int		Doing;
	int		AutoProc;
	
	ULONG	OpenWaitTime;
	ULONG	RecvWaitTime;
	ULONG	SendDelayTime;

	int		ETCRecvLength;										// ETC Recv Buffer
	BYTE	ETCRecvBuffer[ETCBUFFSIZE];			

	int		DATASendLength;										// ETC Send Buffer
	int		DATASendLengthTbl[ETCSENDTBL];
	int		DATASendInTbl;
	int		DATASendInTblTemp;
	int		DATASendOutTbl;
	int		DATASendOutTblTemp;
	BYTE	DATASendBuffer[ETCBUFFSIZE];
	BYTE	DATASendBufferTbl[ETCSENDTBL][ETCBUFFSIZE];
	
// Operations
public:
	int 	Initialize();
	int 	InitializeIP(LPCTSTR szIpAddress, int nPortNumber);
	int 	Deinitialize();
	int 	SendData(BYTE *SendBuff, int Length);
	int 	GetUseStatus();

// Private Interface
	int		EtcSendCmd();
	int		EtcSendCheck();
	int		EtcRecvCmd();
	int		EtcRecvBufferClear();

	int 	LineOpen(int OpenSec, int nAsyncFlag = FALSE);
	int 	LineClose(int CloseSec, int nAsyncFlag = FALSE);
	int 	SendData(BYTE *SendBuff, int Length, int SendSec);
	int 	RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 	GetStatus();
	
private:
	DEVICEINFO	m_DeviceInfo;
	NETWORKINFO	m_NetworkInfo;
	
	CSocketDvr* m_pSocketDvr;

	char	m_IpAddress[256];
	int		m_PortNumber;

	int		m_InitOpen;
	int		m_LineStatus;
};

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////
	UINT	EtcMainThread(LPVOID pParam);

#endif // !defined(AFX_ETCDVR_H__22230EE0_E5FB_4031_A82B_6DF5BD4D4B7C__INCLUDED_)
