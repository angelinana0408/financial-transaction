// SocketDvr.cpp : implementation file
//

#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\SocketDvr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketDvr

CSocketDvr::CSocketDvr(LPCTSTR szIpAddress, int nPortNumber)
{
	char IpAddress[256];

	memset(IpAddress, 0, sizeof(IpAddress));
	memset(m_IpAddress, 0, sizeof(m_IpAddress));

	memcpy(IpAddress, szIpAddress, 15);

	sprintf(m_IpAddress, "%d.%d.%d.%d", Asc2Int(IpAddress, 3),
										Asc2Int(&IpAddress[4], 3),
										Asc2Int(&IpAddress[8], 3),
									  	Asc2Int(&IpAddress[12], 3));
	m_PortNumber = nPortNumber;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CSocketNms() m_IpAddress[%s]m_PortNumber[%d]", m_IpAddress, m_PortNumber);

	InitSocket();												// Variable Initialize
}

CSocketDvr::~CSocketDvr()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketDvr, CAsyncSocket)
	//{{AFX_MSG_MAP(CSocketDvr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketDvr member functions

void CSocketDvr::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnAccept ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CSocketDvr::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnConnect ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnConnect(nErrorCode);

    if (!nErrorCode)
	{
		TCPFirstConnected = TRUE;
		TCPConnected = TRUE;
	}
	else 
		TCPConnected = FALSE;
}

void CSocketDvr::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnClose ERROR] [%d]", nErrorCode);
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] OnClose");
	TCPConnected = FALSE;
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketDvr::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnOutOfBandData ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CSocketDvr::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnReceive ERROR] [%d]", nErrorCode);

	if (!nErrorCode)
		 ProcessData();
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSocketDvr::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnSend ERROR] [%d]", nErrorCode);

	CAsyncSocket::OnSend(nErrorCode);
}

int CSocketDvr::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	int nByte = CAsyncSocket::Receive(lpBuf, nBufLen, nFlags);
	int TCPError;

	if (nByte <= 0)
	{
		TCPError = CheckTCPError();
		if (TCPError == 10035);									// No Error
		else MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Receive ERROR] [%d]", TCPError);
	}
	else
	{
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCPIP RECVDATA]", (char*)lpBuf, nByte);
	}

	return nByte;
}

int CSocketDvr::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class

HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCPIP SENDDATA]", (char*)lpBuf, nBufLen);
	int nByte = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);

	if (nByte != nBufLen)
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Send ERROR] [%d]", CheckTCPError());

	return nByte;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketDvr::InitSocket()
{
	TCPFirstConnected = FALSE;
	TCPConnected = FALSE;
	TCPErrID = 0;

	TCPRecvLength = 0;											// Tcp Recv Buffer
	memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));

	DATASendLength = 0;											// Tran Buffer
	DATARecvLength = 0;
	memset(DATARecvLengthTbl, 0, sizeof(DATARecvLengthTbl));
	DATARecvInTbl = 0;
	DATARecvInTblTemp = 0;
	DATARecvOutTbl = 0;
	DATARecvOutTblTemp = 0;										// 2007.06.06
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));
	memset(DATARecvBufferTbl, 0, sizeof(DATARecvBufferTbl));

	pTCPRecvBuffer = (ETCHeader *)TCPRecvBuffer;

	pDATASendBuffer = (ETCHeader *)DATASendBuffer;

	CheckRecvLength = 0;										// Tran Recv Length
	CheckDataLength = 0;										// Tran Data Length

	return TRUE;
}

int CSocketDvr::OpenSocket()
{
	InitSocket();

	if (!Socket(SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_CONNECT | FD_CLOSE))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Socket ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	if (!Connect((LPCTSTR)m_IpAddress, m_PortNumber))
	{
		if (GetLastError() != WSAEWOULDBLOCK)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Connect ERROR] [%d]", CheckTCPError());
			return FALSE;
		}
	}

	ULONG block = TRUE;
	if (!IOCtl(FIONBIO, &block))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP IOCtl ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	return TRUE;
}

int CSocketDvr::CloseSocket()
{
	MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] CloseSocket");

	ShutDown(CAsyncSocket::both);
	Close();

	return TRUE;
}

int CSocketDvr::SendSocket(BYTE *SendBuff, int Length)
{
	DATASendLength = 0;											// SEND LENGTH CLEAR
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));			// SEND BUFFER CLEAR

	memcpy(pDATASendBuffer->Data, SendBuff, Length);
	DATASendLength = Length;									// DATA

	if (Send(DATASendBuffer, DATASendLength) != DATASendLength)
		return FALSE;
	
	DATASendLength = 0;											// SEND LENGTH CLEAR
	return TRUE;
}

int CSocketDvr::DATARecvSocket(BYTE *RecvBuff, int *Length)
{
	if (DATARecvInTbl == DATARecvOutTbl)						// No Data
	{
		*Length = 0;
		return FALSE;
	}

	memcpy(RecvBuff, DATARecvBufferTbl[DATARecvOutTbl], DATARecvLengthTbl[DATARecvOutTbl]);
	*Length = DATARecvLengthTbl[DATARecvOutTbl];
	DATARecvLength = DATARecvLengthTbl[DATARecvOutTbl];

	DATARecvOutTblTemp = DATARecvOutTbl;						// 2007.06.06
	DATARecvOutTblTemp++;
	if (DATARecvOutTblTemp >= TCPRECVTBL)						// Out Tbl Adj.
		DATARecvOutTblTemp = 0;
	DATARecvOutTbl = DATARecvOutTblTemp;

	if (!DATARecvLength)
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketDvr::ProcessData()
{
	BYTE	TempBuffer[TCPBUFFSIZE];
	int		nByte;
	int		CheckDataFlag;

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
		CheckDataFlag = CheckData();
		
		if (CheckDataFlag)		
		{
			if (TCPRecvLength > CheckRecvLength)				// Split
			{
				TCPRecvLength -= CheckRecvLength;
				memcpy(TCPRecvBuffer, &TCPRecvBuffer[CheckRecvLength], TCPRecvLength);
				continue;										// 2 Data
			}
			else												// Initialize
			{
				TCPRecvLength = 0;
				memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			}
		}
		
		break;
	} 

	return TRUE;
}

int CSocketDvr::CheckData()
{
	int		TCP_BASE_LEN = 0;
	CheckRecvLength = TCPRecvLength;							// Tran Recv Length
	CheckDataLength = TCPRecvLength;							// Tran Data Length


	if (CheckDataLength > TCPBUFFSIZE)							// Data Length Adjust
		CheckDataLength = TCPBUFFSIZE;
	CheckRecvLength = CheckDataLength + TCP_BASE_LEN;			// Data Length(Header Length Include)
	if (CheckRecvLength > TCPBUFFSIZE)							// Max Length Adjust
		CheckRecvLength = TCPBUFFSIZE;

	if (TCPRecvLength < CheckRecvLength)						// Max Length Check
		return FALSE;

	// Data Recv Ok
	DATARecvInTblTemp = DATARecvInTbl;
	DATARecvInTblTemp++;
	if (DATARecvInTblTemp >= TCPRECVTBL)						// In Tbl Temp Adjust
		DATARecvInTblTemp = 0;

	if (DATARecvInTblTemp != DATARecvOutTbl)					// Data Tbl Ok
	{
		memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
		DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

		DATARecvInTbl = DATARecvInTblTemp;						// In Tbl Adjust
	}
	
	return TRUE;												// Clear
}	
	
int CSocketDvr::CheckTCPError()
{
	return (TCPErrID = WSAGetLastError());
}

