/////////////////////////////////////////////////////////////////////////////
// Tcpip.cpp: implementation of the CTcpip class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Tcpip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CTcpip::CTcpip(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;
	
	m_InitOpen		= FALSE;
	m_InitOpen2		= FALSE;
	m_InitOpen3		= FALSE;

	m_LineStatus	= LINE_CLOSE;
	m_LineStatus2	= LINE_CLOSE;
	m_LineStatus3	= LINE_CLOSE;

	m_pSocketCtrl	= new CSocketCtrl(NETWORK);
	m_pSocketCtrl2	= new CSocketCtrl2(NETWORK);
	m_pListenSocket = new CSocketCtrl3(this);
	m_pSocketCtrlB	= new CSocketCtrlB(NETWORK);
}

CTcpip::~CTcpip()
{
	if (m_pSocketCtrlB)	{	delete m_pSocketCtrlB;	m_pSocketCtrlB	= NULL;	}
	if (m_pListenSocket){	delete m_pListenSocket;	m_pListenSocket	= NULL;	}
	if (m_pSocketCtrl2)	{	delete m_pSocketCtrl2;	m_pSocketCtrl2	= NULL;	}
	if (m_pSocketCtrl)	{	delete m_pSocketCtrl;	m_pSocketCtrl	= NULL;	}
}

/////////////////////////////////////////////////////////////////////////////
//	ATMP
/////////////////////////////////////////////////////////////////////////////
int CTcpip::LineOpen(int OpenSec, int nAsyncFlag)
{
	DWORD	WaitTime;

	LineClose(OpenSec, nAsyncFlag);								// 2004.07.12
	m_InitOpen = TRUE;	

/////////////////////////////////////////////////////////////////////////////
	if (nAsyncFlag)												// 2004.07.12
	{
		return m_pSocketCtrl->OpenSocket();
	}
/////////////////////////////////////////////////////////////////////////////
																
	if (m_pSocketCtrl->OpenSocket())							// LINE OPEN
	{
		WaitTime = GetTickCount() + 1000 * OpenSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketCtrl->TCPConnected)
				break;
		} while (GetTickCount() < WaitTime);
	}

	if (m_pSocketCtrl->TCPConnected)							// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Open Fail");
		m_pSocketCtrl->TCPFirstConnected = FALSE;
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::LineOpenIP(LPCTSTR szIpAddress, int nPortNumber, int OpenSec, int nAsyncFlag)
{
	DWORD	WaitTime;

	LineClose(OpenSec, nAsyncFlag);								// 2004.07.12
	m_InitOpen = TRUE;	

/////////////////////////////////////////////////////////////////////////////
	if (nAsyncFlag)												// 2004.07.12
	{
		return m_pSocketCtrl->OpenSocketIP(szIpAddress, nPortNumber);
	}
/////////////////////////////////////////////////////////////////////////////
	if (m_pSocketCtrl->OpenSocketIP(szIpAddress, nPortNumber))	// LINE OPEN
	{
		WaitTime = GetTickCount() + 1000 * OpenSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (m_pSocketCtrl->TCPConnected)
				break;
		} while (GetTickCount() < WaitTime);
	}

	if (m_pSocketCtrl->TCPConnected)							// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Open Fail");
		m_pSocketCtrl->TCPFirstConnected = FALSE;
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::LineClose(int CloseSec, int nAsyncFlag)
{
/////////////////////////////////////////////////////////////////////////////
//	if (!m_pSocketCtrl->TCPFirstConnected)						// 2007.06.06
//	{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Close Connect Fail");
//		m_pSocketCtrl->TCPFirstConnected = FALSE;
//		m_pSocketCtrl->TCPConnected = FALSE;
//		return FALSE;
// 	}
/////////////////////////////////////////////////////////////////////////////

	if (m_InitOpen)												// LINE CLOSE 
	{
		m_InitOpen = FALSE;
		return (m_pSocketCtrl->CloseSocket());
	}
	return TRUE;
}

int CTcpip::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
//	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	m_pSocketCtrl->DATARecvLength = 0;							// RECV SIZE CLEAR

/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketCtrl->TCPFirstConnected)						// 2004.07.12
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send Connect Fail");
		m_pSocketCtrl->TCPFirstConnected = FALSE;
		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_pSocketCtrl->SendSocket(SendBuff, Length))			// SEND DATA
	{
		WaitTime = GetTickCount() + 1000 * SendSec;
		do
		{
			Delay_Msg();
			GetStatus();										// LINE STATUS CONTROL
			if (!m_pSocketCtrl->DATASendLength)
				break;
			if (!m_pSocketCtrl->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}

	Ret = m_pSocketCtrl->DATASendLength;
	m_pSocketCtrl->DATASendLength = 0;							// SEND LENGTH CLEAR
	if (!Ret)													// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send Fail");
//		if (!m_pSocketCtrl->TCPConnected)						// 2009.01.19 : CLOSESOCKET
			m_pSocketCtrl->TCPFirstConnected = FALSE;

		m_pSocketCtrl->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
//	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketCtrl->TCPFirstConnected)						// 2004.07.12
	{
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Recv Connect Fail");
			m_pSocketCtrl->TCPFirstConnected = FALSE;
			m_pSocketCtrl->TCPConnected = FALSE;
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
			if (m_pSocketCtrl->DATARecvInTbl != m_pSocketCtrl->DATARecvOutTbl)
				break;
			if (!m_pSocketCtrl->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}
	else
	{
		GetStatus();											// LINE STATUS CONTROL
	}

	Ret = m_pSocketCtrl->DATARecvSocket(RecvBuff, Length);		// RECV DATA
	if (Ret)													// RECV OK
	{
		m_pSocketCtrl->DATARecvLength = 0;						// RECV LENGTH CLEAR
		return TRUE;
	}
	else 
	{															// RECV ERROR
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Recv Fail");
//			if (!m_pSocketCtrl->TCPConnected)					// 2009.01.19 : CLOSESOCKET
				m_pSocketCtrl->TCPFirstConnected = FALSE;

			m_pSocketCtrl->TCPConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
}

int CTcpip::GetStatus()
{
	switch (m_ConfBuff.InterfaceKind)
	{
/////////////////////////////////////////////////////////////////////////////
		case 9:													// 9:SDB(SZ)	: 2005.04.21
		case 107:												// 107:CB(TYC)	: 2009.04.07
		case 2001:												// 2001:CB(NTC)	: 2006.02.06
		case 2004:												// 2004:CB(NTC)XML	: 2008.03.27
		case 3002:												// 3002:RCB(SZC): 2007.01.05
		case 3003:												// 3003:RCB(KS) : 2007.03.26
		case 3009:												// 3009:RCB(KS) : 2010.03.24
		case 2500:												// 2500:POST-TY(HNC) : 2007.05.15 : ATMU   ³¤Á¬½Ó
		case 3006:												// 3006:RCB(CS) : 2008.11.27
		case 3008:												// 3008:RCB(SZ) : 2010.07.08
		case 7000:												// 7000:BCTC(PBOC) : 2010.10.20 : BHC
			if (m_pSocketCtrl->TCPConnected)					// Connected Type
				m_LineStatus = LINE_OPEN;
			else m_LineStatus = LINE_CLOSE;
			break;
			
/////////////////////////////////////////////////////////////////////////////
		default:												// OTHER
			if (m_pSocketCtrl->TCPFirstConnected)				// Non Connected Type
				m_LineStatus = LINE_OPEN;
			else m_LineStatus = LINE_CLOSE;
			break;
	}

	return m_LineStatus;
}

/////////////////////////////////////////////////////////////////////////////
//	EJR
/////////////////////////////////////////////////////////////////////////////
int CTcpip::LineOpen2(int OpenSec, int nAsyncFlag)
{
	DWORD	WaitTime;

	LineClose2(OpenSec, nAsyncFlag);							// 2004.07.12
	m_InitOpen2 = TRUE;	
	
/////////////////////////////////////////////////////////////////////////////
	if (nAsyncFlag)												// 2004.07.12
	{
		return m_pSocketCtrl2->OpenSocket();
	}
/////////////////////////////////////////////////////////////////////////////
																
	if (m_pSocketCtrl2->OpenSocket())							// LINE OPEN
	{
		WaitTime = GetTickCount() + 1000 * OpenSec;
		do
		{
			Delay_Msg();
			GetStatus2();										// LINE STATUS CONTROL
			if (m_pSocketCtrl2->TCPConnected)
				break;
		} while (GetTickCount() < WaitTime);
	}

	if (m_pSocketCtrl2->TCPConnected)							// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Open2 Fail");
		m_pSocketCtrl2->TCPFirstConnected = FALSE;
		m_pSocketCtrl2->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::LineClose2(int CloseSec, int nAsyncFlag)
{
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketCtrl2->TCPFirstConnected)						// 2007.06.06
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Close2 Connect Fail");
		m_pSocketCtrl2->TCPFirstConnected = FALSE;
		m_pSocketCtrl2->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_InitOpen2)											// LINE CLOSE 
	{
		m_InitOpen2 = FALSE;
		return (m_pSocketCtrl2->CloseSocket());
	}
	return TRUE;
}

int CTcpip::SendData2(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.26
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSocketCtrl2->TCPFirstConnected)						// 2004.07.12
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send2 Connect Fail");
		m_pSocketCtrl2->TCPFirstConnected = FALSE;
		m_pSocketCtrl2->TCPConnected = FALSE;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	if (m_pSocketCtrl2->SendSocket(SendBuff, Length))			// SEND DATA
	{
		WaitTime = GetTickCount() + 1000 * SendSec;
		do
		{
			Delay_Msg();
			GetStatus2();										// LINE STATUS CONTROL
			if (m_pSocketCtrl2->DATASendLength <= 0)
				break;
			if (!m_pSocketCtrl2->TCPConnected)					// 2004.06.22
				break;
		} while (GetTickCount() < WaitTime);
	}

	Ret = m_pSocketCtrl2->DATASendLength;
	m_pSocketCtrl2->DATASendLength = 0;							// SEND LENGTH CLEAR
	if (Ret == 0)												// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send2 Fail");
		m_pSocketCtrl2->TCPFirstConnected = FALSE;
		m_pSocketCtrl2->TCPConnected = FALSE;
		return FALSE;
	}
}

int CTcpip::GetStatus2()
{
//	if (m_pSocketCtrl2->TCPConnected)							// Connected Type
	if (m_pSocketCtrl2->TCPFirstConnected)						// Non Connected Type
		m_LineStatus2 = LINE_OPEN;
	else m_LineStatus2 = LINE_CLOSE;

	return m_LineStatus2;
}

/////////////////////////////////////////////////////////////////////////////
//	ATMP(BID)
/////////////////////////////////////////////////////////////////////////////
int CTcpip::LineOpen3(int OpenSec, int nAsyncFlag)
{
	int		PortNumber;
	char	IpAddress[256];
	DWORD	dwError = 0;

	memset(IpAddress, 0, sizeof(IpAddress));
																// 2005.08.25
	sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_ConfBuff.BpIpAddress3, 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress3[4], 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress3[8], 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress3[12], 3));
	PortNumber = Asc2Int(m_ConfBuff.BpPortNo3, 5);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Open3() Ip:%s Port:%d", IpAddress, PortNumber);

	if (PortNumber == 0) 
		return FALSE;
	
	LineClose3(OpenSec, nAsyncFlag);
	m_InitOpen3 = TRUE;	
		
	m_pSocketCtrlB->InitSocket();

	m_pSocketCtrlB->CurrHostName.Format("%s", IpAddress);		// 2005.08.25

	if (!m_pListenSocket->Create(PortNumber))
	{
		dwError = GetLastError();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Create Fail[%d]", dwError);		
		return FALSE;
	}
	if (!m_pListenSocket->Listen())
	{
		dwError = GetLastError();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Listen Fail[%d]", dwError);		
		return FALSE;
	}

	m_LineStatus3 = LINE_OPEN;
	return TRUE;
}

int CTcpip::LineClose3(int CloseSec, int nAsyncFlag)
{
	if (m_InitOpen3)											// LINE CLOSE : 2007.06.06
	{
		m_InitOpen3 = FALSE;
		return (m_pListenSocket->CloseSocket());
	}
	return TRUE;
}

int CTcpip::SendData3(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;
	DWORD	WaitTime;

	if (m_pSocketCtrlB->SendSocket(SendBuff, Length))			// SEND DATA
	{
		WaitTime = GetTickCount() + 1000 * SendSec;
		do
		{
			Delay_Msg();
			GetStatus3();										// LINE STATUS CONTROL
			if (!m_pSocketCtrlB->DATASendLength)
				break;
		} while (GetTickCount() < WaitTime);
	}

	Ret = m_pSocketCtrlB->DATASendLength;
	m_pSocketCtrlB->DATASendLength = 0;							// SEND LENGTH CLEAR
	if (!Ret)													// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Send3 Fail");		
		return FALSE;
	}
}

int CTcpip::RecvData3(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;
	DWORD	WaitTime;

	if (RecvSec)
	{
		WaitTime = GetTickCount() + 1000 * RecvSec;
		do
		{
			Delay_Msg();
			GetStatus3();										// LINE STATUS CONTROL
			if (m_pSocketCtrlB->DATARecvInTbl != m_pSocketCtrlB->DATARecvOutTbl)
				break;
		} while (GetTickCount() < WaitTime);
	}
	else
	{
		GetStatus3();											// LINE STATUS CONTROL
	}

	Ret = m_pSocketCtrlB->DATARecvSocket(RecvBuff, Length);		// RECV DATA
	if (Ret)													// RECV OK
	{
		m_pSocketCtrlB->TCPRecvLength = 0;						// RECV LENGTH CLEAR
		return TRUE;
	}
	else 
	{															// RECV ERROR
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] Recv3 Fail");
		}
		*Length = 0;
		return FALSE;
	}
}

int CTcpip::GetStatus3()
{
	if (m_InitOpen3) 
		m_LineStatus3 = LINE_OPEN;
	else m_LineStatus3 = LINE_CLOSE;

	return m_LineStatus3;
}

int CTcpip::AcceptRequest3()
{
	m_pListenSocket->Accept(*m_pSocketCtrlB);
	return TRUE;
}
