/////////////////////////////////////////////////////////////////////////////
// SocketCtrl2.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\SocketCtrl2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl2

CSocketCtrl2::CSocketCtrl2(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;
	
	InitSocket();												// Variable Initialize
	memset(m_Branch, 0, sizeof(m_Branch));						// 2004.08.26
	memcpy(m_Branch, &m_ConfBuff.BranchNum[0], 9);
	memset(m_AtmId, 0, sizeof(m_AtmId));
	memcpy(m_AtmId, &m_ConfBuff.AtmSerialNum[0], 8);
}

CSocketCtrl2::~CSocketCtrl2()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketCtrl2, CAsyncSocket)
	//{{AFX_MSG_MAP(CSocketCtrl2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl2 member functions

void CSocketCtrl2::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2 OnAccept ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CSocketCtrl2::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2 OnClose ERROR] [%d]", nErrorCode);
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2/IP] OnClose");
	TCPConnected = FALSE;

	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketCtrl2::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2 OnConnect ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnConnect(nErrorCode);

    if (!nErrorCode)
	{
		TCPFirstConnected = TRUE;
		TCPConnected = TRUE;
	}
	else 
		TCPConnected = FALSE;
}

void CSocketCtrl2::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2 OnOutOfBandData ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CSocketCtrl2::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2 OnReceive ERROR] [%d]", nErrorCode);

	if (!nErrorCode)
		 ProcessData();
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSocketCtrl2::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2 OnSend ERROR] [%d]", nErrorCode);

	CAsyncSocket::OnSend(nErrorCode);
}

int CSocketCtrl2::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	int nByte = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
	int TCPError;

	if (nByte <= 0)
	{
		TCPError = CheckTCPError();
		if (TCPError == 10035);									// No Error
		else MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2 Receive ERROR] [%d]", TCPError);
	}

	return nByte;
}

int CSocketCtrl2::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP2/IP SENDDATA]", (char*)lpBuf, nBufLen);
	int nByte = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);

	if (nByte != nBufLen)
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2 Send ERROR] [%d]", CheckTCPError());

	return nByte;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketCtrl2::InitSocket()
{
	TCPFirstConnected = FALSE;
	TCPConnected = FALSE;
	TCPErrID = 0;

	TCPRecvLength = 0;											// Tcp Recv Buffer
	memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));

	DATASendLength = 0;											// Tran Buffer
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));

	pTCPRecvBuffer = (TCPHeader2 *)TCPRecvBuffer;

	pDATASendBuffer = (TCPHeader2 *)DATASendBuffer;

	CheckRecvLength = 0;										// Tran Recv Length
	CheckDataLength = 0;										// Tran Data Length

	return TRUE;
}

int CSocketCtrl2::OpenSocket()
{
	char		IpAddress[256];
	int			PortNumber;

	sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_ConfBuff.BpIpAddress2, 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress2[4], 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress2[8], 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress2[12], 3));
	PortNumber = Asc2Int(m_ConfBuff.BpPortNo2, 5);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2/IP] OpenSocket Ip:%s Port:%d", IpAddress, PortNumber);

	InitSocket();

	if (!Socket(SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_CONNECT | FD_CLOSE))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2 Socket ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	if (!Connect((LPCTSTR)IpAddress, PortNumber))
	{
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2 Connect ERROR] [%d]", CheckTCPError());
			return FALSE;
		}
	}

	ULONG block = TRUE;
	if (!IOCtl(FIONBIO, &block))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2 IOCtl ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	return TRUE;
}

int CSocketCtrl2::CloseSocket()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP2/IP] CloseSocket");

	ShutDown(CAsyncSocket::both);
	Close();

	return TRUE;
}

int CSocketCtrl2::SendSocket(BYTE *SendBuff, int Length)
{
	DATASendLength = 0;											// SEND LENGTH CLEAR
	memset(DATASendBuffer, 0, TCPBUFFSIZE);						// SEND BUFFER CLEAR

	pDATASendBuffer->Length = Length;
	memcpy(pDATASendBuffer->Branch, m_Branch, 8);				// 2004.08.26
	memcpy(pDATASendBuffer->AtmId, m_AtmId, 8);
	memcpy(pDATASendBuffer->TxnCode, "1000", 4);
	memcpy(&DATASendBuffer[TCP_BASE_LEN2], SendBuff, Length);
	pDATASendBuffer->Mac = SendBuff[0];
	for (int i = 1; i < Length; i++)
		pDATASendBuffer->Mac ^= SendBuff[i];
	DATASendLength = Length + TCP_BASE_LEN2;					// DATA + LENGTH

	if (Send(DATASendBuffer, DATASendLength) != DATASendLength)
		return FALSE;
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketCtrl2::ProcessData()
{
	BYTE	TempBuffer[TCPBUFFSIZE];
	int		nByte;

	do															// Merge
	{
		nByte = Receive(TempBuffer, TCPBUFFSIZE);
		if (nByte <= 0)
			break;

		if ((TCPRecvLength + nByte) < TCPBUFFSIZE)
		{
			memcpy(&TCPRecvBuffer[TCPRecvLength], TempBuffer, nByte);
			TCPRecvLength += nByte;
		}
	} while (TRUE);
	
	while (TRUE)
	{
		if (CheckData())										// All Clear
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
		}
		
		break;
	} 

	return TRUE;
}

int CSocketCtrl2::CheckData()
{
	CheckRecvLength = TCPRecvLength;							// Tran Recv Length
	CheckDataLength = 0;										// Tran Data Length

	if (TCPRecvLength < TCP_BASE_LEN2)							// Header Length Check
		return FALSE;											// Recv Waiting

	CheckDataLength = pTCPRecvBuffer->Length;
	if (CheckDataLength > TCPBUFFSIZE)							// Data Length Adjust
		CheckDataLength = TCPBUFFSIZE;
	CheckRecvLength = CheckDataLength + TCP_BASE_LEN2;			// Data Length(Header Length Include)
	if (CheckRecvLength > TCPBUFFSIZE)							// Max Length Adjust
		CheckRecvLength = TCPBUFFSIZE;

	if (TCPRecvLength < CheckRecvLength)						// Max Length Check
		return FALSE;											// Recv Waiting

	// Send & Data Recv Ok
	if (memcmp(pTCPRecvBuffer->RtnCode, "0000", 4) == 0)
		DATASendLength = 0;										// Send Succ
	else
		DATASendLength = -1;									// Send Fail
		
	return TRUE;												// Clear
}	
	
int CSocketCtrl2::CheckTCPError()
{
	return (TCPErrID = WSAGetLastError());
}
