/////////////////////////////////////////////////////////////////////////////
// NmsDc.cpp: implementation of the CNmsDc class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\NmsDc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNmsDc::CNmsDc(DEVICEINFO DEVICE, NETWORKINFO NETWORK)
{
	m_DeviceInfo	= DEVICE;
	m_NetworkInfo	= NETWORK;

	m_pSocketNms	= NULL;

	memset(m_IpAddress, 0, sizeof(m_IpAddress));
	m_PortNumber	= 0;
	
	m_InitOpen		= FALSE;
	m_LineStatus	= LINE_CLOSE;
	m_LineCloseStatus	= LINE_CLOSE;

	m_nNmsKind		= 0;
	m_nNmsKind		= IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSKIND, 0);
}

CNmsDc::~CNmsDc()
{
	if (m_pSocketNms)	{	delete m_pSocketNms;	m_pSocketNms	= NULL;	}
}

int CNmsDc::Initialize()
{
	ThreadUse		= FALSE;
	Doing			= FALSE;
	AutoProc		= 0;

	OpenWaitTime	= 0;
	SendDelayTime	= 0;
	RecvWaitTime	= 0;
	
	NMSRecvLength	= 0;
	memset(NMSRecvBuffer, 0, sizeof(NMSRecvBuffer));

	DATASendLength	= 0;
	memset(DATASendLengthTbl, 0, sizeof(DATASendLengthTbl));
	DATASendInTbl	= 0;
	DATASendInTblTemp = 0;
	DATASendOutTbl	= 0;
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATASendBufferTbl, 0, sizeof(DATASendBufferTbl));

/////////////////////////////////////////////////////////////////////////////
	memset(m_IpAddress, 0, sizeof(m_IpAddress));
	sprintf(m_IpAddress, "%15.15s", IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSIP, "192.168.001.250"));
	m_PortNumber = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSPORT, 6868);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Initialize()m_IpAddress[%15.15s]", m_IpAddress);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Initialize()m_PortNumber[%d]", m_PortNumber);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Initialize()m_nNmsKind[%d]", m_nNmsKind);

	if ((strlen(m_IpAddress) == 0)				||				// 2005.12.28
		(memcmp(m_IpAddress, "0.", 2) == 0)		||
		(memcmp(m_IpAddress, "00.", 3) == 0)	||
		(memcmp(m_IpAddress, "000.", 4) == 0)	||
		(m_PortNumber == 0))
		;
	else
	{
		m_pSocketNms	= new CSocketNms(m_IpAddress, m_PortNumber);
		Doing = TRUE;
		if (m_nNmsKind)
			AfxBeginThread(NmsMainThreadSendRecv, this, THREAD_PRIORITY_NORMAL);
		else
			AfxBeginThread(NmsMainThread, this, THREAD_PRIORITY_NORMAL);
	}
	
	return TRUE;
}

int CNmsDc::InitializeIP(LPCTSTR szIpAddress, int nPortNumber)
{
	ThreadUse		= FALSE;
	Doing			= FALSE;
	AutoProc		= 0;

	OpenWaitTime	= 0;
	SendDelayTime	= 0;
	RecvWaitTime	= 0;
	
	NMSRecvLength	= 0;
	memset(NMSRecvBuffer, 0, sizeof(NMSRecvBuffer));

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
		m_pSocketNms	= new CSocketNms(m_IpAddress, m_PortNumber);
		Doing = TRUE;
		if (m_nNmsKind)
			AfxBeginThread(NmsMainThreadSendRecv, this, THREAD_PRIORITY_NORMAL);
		else
			AfxBeginThread(NmsMainThread, this, THREAD_PRIORITY_NORMAL);
	}

	return TRUE;
}

int CNmsDc::Deinitialize()
{
	Doing = FALSE;

	return TRUE;
}

int CNmsDc::SendData(BYTE *SendBuff, int Length)
{
	DATASendInTblTemp = DATASendInTbl;
	DATASendInTblTemp++;
	if (DATASendInTblTemp >= NMSSENDTBL)						// In Tbl Temp Adjust
		DATASendInTblTemp = 0;

	if (DATASendInTblTemp != DATASendOutTbl)					// Data Tbl Ok
	{
		DATASendLengthTbl[DATASendInTbl] = Length;
		memcpy(DATASendBufferTbl[DATASendInTbl], SendBuff, Length);

		DATASendInTbl = DATASendInTblTemp;						// In Tbl Adjust
	}

	return TRUE;
}
	
int CNmsDc::GetUseStatus()
{
	return ThreadUse;
}

/////////////////////////////////////////////////////////////////////////////
UINT NmsMainThread(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsMainThread()");
	CNmsDc*	m_pNmsDc = (CNmsDc*)pParam;

/////////////////////////////////////////////////////////////////////////////
	m_pNmsDc->ThreadUse = TRUE;
/////////////////////////////////////////////////////////////////////////////
	m_pNmsDc->AutoProc = 0;										// Initial Open Value

	while (m_pNmsDc->Doing)
	{
		Delay_Msg(100);											// Performance Adjust
		switch (m_pNmsDc->AutoProc)
		{
			case 0:												// Send Check
				if (m_pNmsDc->NmsSendCheck())
					m_pNmsDc->AutoProc++;						// Next Procedure
				break;

			case 1:												// Open Command
				m_pNmsDc->LineOpen(30, TRUE);							
				m_pNmsDc->OpenWaitTime = TimerSet(30);
				m_pNmsDc->AutoProc++;							// Next Procedure
				break;

			case 2:												// Open Wait
				if (CheckTimer(m_pNmsDc->OpenWaitTime))
					m_pNmsDc->AutoProc = 4;						// Open Command
				else
				if (m_pNmsDc->GetStatus() == LINE_OPEN)
					m_pNmsDc->AutoProc++;						// Next Procedure
				break;

			case 3:												// Send Command
				m_pNmsDc->NmsSendCmd();
				m_pNmsDc->AutoProc++;							// Next Procedure
				break;

			case 4:												// Line Close
				m_pNmsDc->LineClose(30);
				m_pNmsDc->OpenWaitTime = TimerSet(6);
				m_pNmsDc->AutoProc++;							// Close Wait
				break;
				
			case 5:												// Line Close
				if (CheckTimer(m_pNmsDc->OpenWaitTime))
					m_pNmsDc->AutoProc = 0;						// Send Check
				else
				if (m_pNmsDc->GetCloseStatus() == LINE_CLOSE)
					m_pNmsDc->AutoProc = 0;						// Send Check
				break;
				
			default:
				m_pNmsDc->AutoProc = 0;							// Send Check
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	m_pNmsDc->ThreadUse = FALSE;
	m_pNmsDc->AutoProc = 0;										// Initial Open Value
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsMainThread():return()");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
UINT NmsMainThreadSendRecv(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsMainThreadSendRecv()");
	CNmsDc*	m_pNmsDc = (CNmsDc*)pParam;

/////////////////////////////////////////////////////////////////////////////
	m_pNmsDc->ThreadUse = TRUE;
/////////////////////////////////////////////////////////////////////////////
	m_pNmsDc->AutoProc = 0;										// Initial Open Value

	while (m_pNmsDc->Doing)
	{
		Delay_Msg(100);											// Performance Adjust
		switch (m_pNmsDc->AutoProc)
		{
			case 0:												// Send Check
				if (m_pNmsDc->NmsSendCheck())
					m_pNmsDc->AutoProc++;						// Next Procedure
				break;

			case 1:												// Open Command
				m_pNmsDc->LineOpen(30, TRUE);							
				m_pNmsDc->OpenWaitTime = TimerSet(30);
				m_pNmsDc->AutoProc++;							// Next Procedure
				break;

			case 2:												// Open Wait
				if (CheckTimer(m_pNmsDc->OpenWaitTime))
					m_pNmsDc->AutoProc = 5;						// Open Command
				else
				if (m_pNmsDc->GetStatus() == LINE_OPEN)
					m_pNmsDc->AutoProc++;						// Next Procedure
				break;

			case 3:												// Send Command
				if (m_pNmsDc->NmsSendCmd())
					m_pNmsDc->AutoProc++;						// Next Procedure
				else
					m_pNmsDc->AutoProc = 5;						// Next Procedure
				break;

			case 4:												// Line Close
				m_pNmsDc->NmsRecvCmd();
				m_pNmsDc->AutoProc++;							// Next Procedure
				break;
				
			case 5:												// Line Close
				m_pNmsDc->LineClose(30);
				m_pNmsDc->OpenWaitTime = TimerSet(6);
				m_pNmsDc->AutoProc++;							// Close Wait
				break;
				
			case 6:												// Line Close
				if (CheckTimer(m_pNmsDc->OpenWaitTime))
					m_pNmsDc->AutoProc = 0;						// Send Check
				else
				if (m_pNmsDc->GetCloseStatus() == LINE_CLOSE)
					m_pNmsDc->AutoProc = 0;						// Send Check
				break;
				
			default:
				m_pNmsDc->AutoProc = 0;							// Send Check
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	m_pNmsDc->ThreadUse = FALSE;
	m_pNmsDc->AutoProc = 0;										// Initial Open Value
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsMainThreadSendRecv():return()");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CNmsDc::NmsSendCmd()
{
	int nResult = FALSE;

	if (NmsSendCheck())											// Data Ok
	{
		memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
		memcpy(DATASendBuffer, DATASendBufferTbl[DATASendOutTbl], DATASendLengthTbl[DATASendOutTbl]);
		DATASendLength = DATASendLengthTbl[DATASendOutTbl];

		if (m_nNmsKind)
			;
		else
		{
			DATASendOutTblTemp = DATASendOutTbl;				// 2007.06.06
			DATASendOutTblTemp++;
			if (DATASendOutTblTemp >= NMSSENDTBL)				// Out Tbl Adj.
				DATASendOutTblTemp = 0;
			DATASendOutTbl = DATASendOutTblTemp;
		}

		if (!DATASendLength)
			return FALSE;
	}
	else
	{
		return FALSE;
	}
	
	nResult = SendData(DATASendBuffer, DATASendLength, 10);
	return nResult;
}

int CNmsDc::NmsSendCheck()
{
	if (DATASendInTbl != DATASendOutTbl)						// Data Ok
		return TRUE;
	else
		return FALSE;
}

int CNmsDc::NmsRecvCmd()
{
	int		nRecvResult = FALSE;

	nRecvResult = RecvData(NMSRecvBuffer, &NMSRecvLength, 30);
	if (nRecvResult == FALSE)
		return FALSE;

	if (NMSRecvLength < 4)
		return FALSE;

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPIP RECVDATA]", (char*)NMSRecvBuffer, NMSRecvLength);

	if (memcmp(&NMSRecvBuffer[33], ZERO2, 0) == 0)
	{
		DATASendOutTblTemp = DATASendOutTbl;					// 2007.06.06
		DATASendOutTblTemp++;
		if (DATASendOutTblTemp >= NMSSENDTBL)					// Out Tbl Adj.
			DATASendOutTblTemp = 0;
		DATASendOutTbl = DATASendOutTblTemp;
	}
	else
		return FALSE;

	return TRUE;
}

int CNmsDc::NmsRecvBufferClear()
{
	while (RecvData(NMSRecvBuffer, &NMSRecvLength, 0));

	NMSRecvLength = 0;
	memset(NMSRecvBuffer, 0, sizeof(NMSRecvBuffer));
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CNmsDc::LineOpen(int OpenSec, int nAsyncFlag)
{
	DWORD	WaitTime;

	LineClose(OpenSec, nAsyncFlag);								// 2004.07.12
	m_InitOpen = TRUE;	

/////////////////////////////////////////////////////////////////////////////
	if (nAsyncFlag)												// 2004.07.12
	{
		return m_pSocketNms->OpenSocket();
	}
/////////////////////////////////////////////////////////////////////////////
																
	if (m_pSocketNms->OpenSocket())								// LINE OPEN
	{
		WaitTime = GetTickCount() + 1000 * OpenSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketNms->TCPConnected)
				break;
		} while (GetTickCount() < WaitTime);
	}

	if (m_pSocketNms->TCPConnected)								// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Open Fail");
		m_pSocketNms->TCPFirstConnected = FALSE;
		m_pSocketNms->TCPConnected = FALSE;
		return FALSE;
	}
}

int CNmsDc::LineClose(int CloseSec, int nAsyncFlag)
{
/////////////////////////////////////////////////////////////////////////////
//	if (!m_pSocketNms->TCPFirstConnected)						// 2011.09.21
//	{
//		if (m_InitOpen)
//		{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Close Connect Fail");
//		}
//		m_pSocketNms->TCPFirstConnected = FALSE;
//		m_pSocketNms->TCPConnected = FALSE;
//		return FALSE;
//	}
/////////////////////////////////////////////////////////////////////////////

	if (m_InitOpen)												// LINE CLOSE 
	{
		m_InitOpen = FALSE;
		return (m_pSocketNms->CloseSocket());
	}
	return TRUE;
}

int CNmsDc::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	m_pSocketNms->DATARecvLength = 0;							// RECV SIZE CLEAR

/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketNms->TCPFirstConnected)						// 2004.07.12
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send Connect Fail");
		m_pSocketNms->TCPFirstConnected = FALSE;
		m_pSocketNms->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_pSocketNms->SendSocket(SendBuff, Length))				// SEND DATA
	{
		WaitTime = GetTickCount() + 1000 * SendSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (!m_pSocketNms->DATASendLength)
				break;
			if (!m_pSocketNms->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}

	Ret = m_pSocketNms->DATASendLength;
	m_pSocketNms->DATASendLength = 0;							// SEND LENGTH CLEAR
	if (!Ret)													// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send Fail");
		if (!m_pSocketNms->TCPConnected)						// 2009.01.19 : CLOSESOCKET
			m_pSocketNms->TCPFirstConnected = FALSE;

		m_pSocketNms->TCPConnected = FALSE;
		return FALSE;
	}
}

int CNmsDc::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketNms->TCPFirstConnected)						// 2004.07.12
	{
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Recv Connect Fail");
			m_pSocketNms->TCPFirstConnected = FALSE;
			m_pSocketNms->TCPConnected = FALSE;
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
			if (m_pSocketNms->DATARecvInTbl != m_pSocketNms->DATARecvOutTbl)
				break;
			if (!m_pSocketNms->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}
	else
	{
		GetStatus();											// LINE STATUS CONTROL
	}

	Ret = m_pSocketNms->DATARecvSocket(RecvBuff, Length);		// RECV DATA
	if (Ret)													// RECV OK
	{
		m_pSocketNms->DATARecvLength = 0;						// RECV LENGTH CLEAR
		return TRUE;
	}
	else 
	{															// RECV ERROR
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Recv Fail");
			if (!m_pSocketNms->TCPConnected)					// 2009.01.19 : CLOSESOCKET
				m_pSocketNms->TCPFirstConnected = FALSE;
			
			m_pSocketNms->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
}

int CNmsDc::GetStatus()
{
//	if (m_pSocketNms->TCPConnected)								// Connected Type
	if (m_pSocketNms->TCPFirstConnected)						// Non Connected Type
		m_LineStatus = LINE_OPEN;
	else m_LineStatus = LINE_CLOSE;

	return m_LineStatus;
}

int CNmsDc::GetCloseStatus()
{
	if (m_pSocketNms->TCPConnected)								// Not Received OnClose Event
		m_LineCloseStatus = LINE_OPEN;
	else m_LineCloseStatus = LINE_CLOSE;

	return m_LineCloseStatus;
}

