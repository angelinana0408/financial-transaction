/////////////////////////////////////////////////////////////////////////////
// EtcDvr.cpp: implementation of the CEtcDvr class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\EtcDvr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEtcDvr::CEtcDvr(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	m_pSocketDvr	= NULL;

	memset(m_IpAddress, 0, sizeof(m_IpAddress));
	m_PortNumber	= 0;
	
	m_InitOpen		= FALSE;
	m_LineStatus	= LINE_CLOSE;
}

CEtcDvr::~CEtcDvr()
{
	if (m_pSocketDvr)	{	delete m_pSocketDvr;	m_pSocketDvr	= NULL;	}
}

int CEtcDvr::Initialize()
{
	ThreadUse		= FALSE;
	Doing			= FALSE;
	AutoProc		= 0;

	OpenWaitTime	= 0;
	SendDelayTime	= 0;
	RecvWaitTime	= 0;
	
	ETCRecvLength	= 0;
	memset(ETCRecvBuffer, 0, sizeof(ETCRecvBuffer));

	DATASendLength	= 0;
	memset(DATASendLengthTbl, 0, sizeof(DATASendLengthTbl));
	DATASendInTbl	= 0;
	DATASendInTblTemp = 0;
	DATASendOutTbl	= 0;
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATASendBufferTbl, 0, sizeof(DATASendBufferTbl));

/////////////////////////////////////////////////////////////////////////////
	memset(m_IpAddress, 0, sizeof(m_IpAddress));
	sprintf(m_IpAddress, "%15.15s", IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ETCIP, "192.168.001.250"));
	m_PortNumber = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ETCPORT, 6868);

	if ((strlen(m_IpAddress) == 0)				||				// 2005.12.28
		(memcmp(m_IpAddress, "0.", 2) == 0)		||
		(memcmp(m_IpAddress, "00.", 3) == 0)	||
		(memcmp(m_IpAddress, "000.", 4) == 0)	||
		(m_PortNumber == 0))
		;
	else
	{
		m_pSocketDvr	= new CSocketDvr(m_IpAddress, m_PortNumber);
		Doing = TRUE;
		AfxBeginThread(EtcMainThread, this, THREAD_PRIORITY_NORMAL);
	}
	
	return TRUE;
}

int CEtcDvr::InitializeIP(LPCTSTR szIpAddress, int nPortNumber)
{
	ThreadUse		= FALSE;
	Doing			= FALSE;
	AutoProc		= 0;

	OpenWaitTime	= 0;
	SendDelayTime	= 0;
	RecvWaitTime	= 0;
	
	ETCRecvLength	= 0;
	memset(ETCRecvBuffer, 0, sizeof(ETCRecvBuffer));

	DATASendLength	= 0;
	memset(DATASendLengthTbl, 0, sizeof(DATASendLengthTbl));
	DATASendInTbl	= 0;
	DATASendInTblTemp = 0;
	DATASendOutTbl	= 0;
	DATASendOutTblTemp = 0;										// 2007.06.06
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATASendBufferTbl, 0, sizeof(DATASendBufferTbl));

/////////////////////////////////////////////////////////////////////////////
	memset(m_IpAddress, 0, sizeof(m_IpAddress));
	memcpy(m_IpAddress, szIpAddress, 15);
	m_PortNumber = nPortNumber;

	if ((strlen(m_IpAddress) == 0)				||				// 2005.12.28
		(memcmp(m_IpAddress, "0.", 2) == 0)		||
		(memcmp(m_IpAddress, "00.", 3) == 0)	||
		(memcmp(m_IpAddress, "000.", 4) == 0)	||
		(m_PortNumber == 0))
		;
	else
	{
		m_pSocketDvr	= new CSocketDvr(m_IpAddress, m_PortNumber);
		Doing = TRUE;
		AfxBeginThread(EtcMainThread, this, THREAD_PRIORITY_NORMAL);
	}

	return TRUE;
}

int CEtcDvr::Deinitialize()
{
	Doing = FALSE;

	return TRUE;
}

int CEtcDvr::SendData(BYTE *SendBuff, int Length)
{
	DATASendInTblTemp = DATASendInTbl;
	DATASendInTblTemp++;
	if (DATASendInTblTemp >= ETCSENDTBL)						// In Tbl Temp Adjust
		DATASendInTblTemp = 0;

	if (DATASendInTblTemp != DATASendOutTbl)					// Data Tbl Ok
	{
		DATASendLengthTbl[DATASendInTbl] = Length;
		memcpy(DATASendBufferTbl[DATASendInTbl], SendBuff, Length);

		DATASendInTbl = DATASendInTblTemp;						// In Tbl Adjust
	}

	return TRUE;
}
	
int CEtcDvr::GetUseStatus()
{
	return ThreadUse;
}

/////////////////////////////////////////////////////////////////////////////
UINT EtcMainThread(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "EtcMainThread()");

	CEtcDvr*	m_pEtcDvr = (CEtcDvr*)pParam;

/////////////////////////////////////////////////////////////////////////////
	m_pEtcDvr->ThreadUse = TRUE;
/////////////////////////////////////////////////////////////////////////////
	m_pEtcDvr->AutoProc = 0;									// Initial Open Value

	while (m_pEtcDvr->Doing)
	{
		Delay_Msg(100);											// Performance Adjust
		switch (m_pEtcDvr->AutoProc)
		{
			case 0:												// Send Check
				if (m_pEtcDvr->EtcSendCheck())
					m_pEtcDvr->AutoProc++;						// Next Procedure
				break;

			case 1:												// Open Command
				m_pEtcDvr->LineOpen(30, TRUE);							
				m_pEtcDvr->OpenWaitTime = TimerSet(30);
				m_pEtcDvr->AutoProc++;							// Next Procedure
				break;

			case 2:												// Open Wait
				if (CheckTimer(m_pEtcDvr->OpenWaitTime))
					m_pEtcDvr->AutoProc = 1;					// Open Command
				else
				if (m_pEtcDvr->GetStatus() == LINE_OPEN)
					m_pEtcDvr->AutoProc++;						// Next Procedure
				break;

			case 3:												// Send Command
				m_pEtcDvr->EtcSendCmd();
				m_pEtcDvr->AutoProc++;							// Next Procedure
				break;

			case 4:												// Line Close
				m_pEtcDvr->LineClose(30);
				m_pEtcDvr->AutoProc = 0;						// Send Check
				break;
				
			default:
				m_pEtcDvr->AutoProc = 0;						// Send Check
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	m_pEtcDvr->ThreadUse = FALSE;
	m_pEtcDvr->AutoProc = 0;									// Initial Open Value
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "EtcMainThread():return()");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CEtcDvr::EtcSendCmd()
{
	if (EtcSendCheck())											// Data Ok
	{
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		memcpy(DATASendBuffer, DATASendBufferTbl[DATASendOutTbl], DATASendLengthTbl[DATASendOutTbl]);
		DATASendLength = DATASendLengthTbl[DATASendOutTbl];

		DATASendOutTblTemp = DATASendOutTbl;					// 2007.06.06
		DATASendOutTblTemp++;
		if (DATASendOutTblTemp >= ETCSENDTBL)					// Out Tbl Adj.
			DATASendOutTblTemp = 0;
		DATASendOutTbl = DATASendOutTblTemp;

		if (!DATASendLength)
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	
	SendData(DATASendBuffer, DATASendLength, 10);
	return TRUE;
}

int CEtcDvr::EtcSendCheck()
{
	if (DATASendInTbl != DATASendOutTbl)						// Data Ok
		return TRUE;
	else
		return FALSE;
}

int CEtcDvr::EtcRecvCmd()
{
	int		nRecvResult = FALSE;

	nRecvResult = RecvData(ETCRecvBuffer, &ETCRecvLength, 0);
	if (nRecvResult == FALSE)
		return FALSE;

	if (ETCRecvLength < 4)
		return FALSE;

HexDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "[TCPIP RECVDATA]", (char*)ETCRecvBuffer, ETCRecvLength);

	return TRUE;
}

int CEtcDvr::EtcRecvBufferClear()
{
	while (RecvData(ETCRecvBuffer, &ETCRecvLength, 0));

	ETCRecvLength = 0;
	memset(ETCRecvBuffer, 0, sizeof(ETCRecvBuffer));
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CEtcDvr::LineOpen(int OpenSec, int nAsyncFlag)
{
	DWORD	WaitTime;

	LineClose(OpenSec, nAsyncFlag);								// 2004.07.12
	m_InitOpen = TRUE;	

/////////////////////////////////////////////////////////////////////////////
	if (nAsyncFlag)												// 2004.07.12
	{
		return m_pSocketDvr->OpenSocket();
	}
/////////////////////////////////////////////////////////////////////////////
																
	if (m_pSocketDvr->OpenSocket())								// LINE OPEN
	{
		WaitTime = GetTickCount() + 1000 * OpenSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketDvr->TCPConnected)
				break;
		} while (GetTickCount() < WaitTime);
	}

	if (m_pSocketDvr->TCPConnected)								// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_DUMP_MODE, "EtcLog", __FILE__, __LINE__, "[TCP/IP] Open Fail");
		m_pSocketDvr->TCPFirstConnected = FALSE;
		m_pSocketDvr->TCPConnected = FALSE;
		return FALSE;
	}
}

int CEtcDvr::LineClose(int CloseSec, int nAsyncFlag)
{
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketDvr->TCPFirstConnected)						// 2007.06.06
	{
		if (m_InitOpen)
		{
MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "[TCP/IP] Close Connect Fail");
		}
		m_pSocketDvr->TCPFirstConnected = FALSE;
		m_pSocketDvr->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_InitOpen)												// LINE CLOSE 
	{
		m_InitOpen = FALSE;
		return (m_pSocketDvr->CloseSocket());
	}
	return TRUE;
}

int CEtcDvr::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	m_pSocketDvr->DATARecvLength = 0;							// RECV SIZE CLEAR

/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketDvr->TCPFirstConnected)						// 2004.07.12
	{
MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "[TCP/IP] Send Connect Fail");
		m_pSocketDvr->TCPFirstConnected = FALSE;
		m_pSocketDvr->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_pSocketDvr->SendSocket(SendBuff, Length))				// SEND DATA
	{
		WaitTime = GetTickCount() + 1000 * SendSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (!m_pSocketDvr->DATASendLength)
				break;
			if (!m_pSocketDvr->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}

	Ret = m_pSocketDvr->DATASendLength;
	m_pSocketDvr->DATASendLength = 0;							// SEND LENGTH CLEAR
	if (!Ret)													// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "[TCP/IP] Send Fail");
		if (!m_pSocketDvr->TCPConnected)						// 2009.01.19 : CLOSESOCKET
			m_pSocketDvr->TCPFirstConnected = FALSE;

		m_pSocketDvr->TCPConnected = FALSE;
		return FALSE;
	}
}

int CEtcDvr::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketDvr->TCPFirstConnected)						// 2004.07.12
	{
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "[TCP/IP] Recv Connect Fail");
			m_pSocketDvr->TCPFirstConnected = FALSE;
			m_pSocketDvr->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (RecvSec)
	{
		WaitTime = GetTickCount() + 1000 * RecvSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketDvr->DATARecvInTbl != m_pSocketDvr->DATARecvOutTbl)
				break;
			if (!m_pSocketDvr->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}
	else
	{
		GetStatus();											// LINE STATUS CONTROL
	}

	Ret = m_pSocketDvr->DATARecvSocket(RecvBuff, Length);		// RECV DATA
	if (Ret)													// RECV OK
	{
		m_pSocketDvr->DATARecvLength = 0;						// RECV LENGTH CLEAR
		return TRUE;
	}
	else 
	{															// RECV ERROR
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "EtcLog", __FILE__, __LINE__, "[TCP/IP] Recv Fail");
			if (!m_pSocketDvr->TCPConnected)					// 2009.01.19 : CLOSESOCKET
				m_pSocketDvr->TCPFirstConnected = FALSE;

			m_pSocketDvr->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
}

int CEtcDvr::GetStatus()
{
//	if (m_pSocketDvr->TCPConnected)								// Connected Type
	if (m_pSocketDvr->TCPFirstConnected)						// Non Connected Type
		m_LineStatus = LINE_OPEN;
	else m_LineStatus = LINE_CLOSE;

	return m_LineStatus;
}
