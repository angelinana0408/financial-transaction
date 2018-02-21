/////////////////////////////////////////////////////////////////////////////
// SocketCtrlB.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\SocketCtrlB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrlB

CSocketCtrlB::CSocketCtrlB(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;
	CurrHostName	= "";										// 2005.08.25

	InitSocket();
}

CSocketCtrlB::~CSocketCtrlB()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketCtrlB, CAsyncSocket)
	//{{AFX_MSG_MAP(CSocketCtrlB)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrlB member functions

void CSocketCtrlB::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCPB OnClose ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketCtrlB::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCPB OnReceive ERROR] [%d]", nErrorCode);

	if (!nErrorCode)
		ProcessData();
	
	CAsyncSocket::OnReceive(nErrorCode);
}

int CSocketCtrlB::Receive(void* lpBuf, int nBufLen, int nFlags) 
{
	int		TCPError;
	CString strHostName("");
	UINT	HostPort = 0;

	CAsyncSocket::GetPeerName(strHostName, HostPort);

	if (m_ConfBuff.InterfaceKind == 11)							// 11:CUP(BCTC)	: 2005.08.17
	{
		if ((CurrHostName.GetLength() == 0)		||				// 2005.08.25
			(CurrHostName.Left(2) == "0.")		||				// 2005.08.31
			(CurrHostName.Left(3) == "00.")		||
			(CurrHostName.Left(4) == "000."))
			;
		else
		{
			if (CurrHostName != strHostName)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPBIP RECVDATA] CurrHostName[%s] strHostName[%s]", CurrHostName, strHostName);
				CloseSocket();
				return FALSE;	
			}
		}
	}

	int nByte = CAsyncSocket::Receive(lpBuf, nBufLen);

	if (nByte <= 0)
	{
		TCPError = CheckTCPError();
		if (TCPError == 10035);									// No Error
		else MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPB Receive ERROR] [%d]", TCPError);
	}
	else
	{
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPBIP RECVDATA]", (char*)lpBuf, nByte);
	}
	
	return nByte;
}

int CSocketCtrlB::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
	
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPBIP SENDDATA]", (char*)lpBuf, nBufLen);
	int nByte = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);

	if (nByte != nBufLen)
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPB Send ERROR] [%d]", CheckTCPError());

	return nByte;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketCtrlB::InitSocket()
{
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

	pTCPRecvBuffer = (TCPHeaderB *)TCPRecvBuffer;

	CheckRecvLength = 0;										// Tran Recv Length
	CheckDataLength = 0;										// Tran Data Length

	return TRUE;
}

int CSocketCtrlB::CloseSocket()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPB/IP] CloseSocket");

	ShutDown(CAsyncSocket::both);
	Close();
	
	return TRUE;
}

int CSocketCtrlB::SendSocket(BYTE *SendBuff, int Length)
{
	DATASendLength = 0;											// SEND LENGTH CLEAR
	memset(DATASendBuffer, 0, sizeof(DATASendBuffer));			// SEND BUFFER CLEAR

	switch (m_ConfBuff.InterfaceKind)
	{
/////////////////////////////////////////////////////////////////////////////
		case 0:													// 0:CUP(WH)	: 2004.05.08
		case 1:													// 1:BOC(SD)	: 2004.05.08
		case 2:													// 2:ABC(SD)	: 2004.06.18
		case 3:													// 3:ABC(FZ)	: 2004.06.25
		case 4:													// 4:RCC(GZ)	: 2004.11.05
		case 5:													// 5:ABC(ZJG)	: 2004.11.05
		case 6:													// 6:CCB(JX)	: 2004.11.05
			break;

/////////////////////////////////////////////////////////////////////////////
		case 7:													// 7:ABC(BJ)	: 2004.11.05
			DATASendBuffer[0] = HIBYTE(LOWORD(Length));			// 2005.03.08
			DATASendBuffer[1] = LOBYTE(LOWORD(Length));
			memcpy(&DATASendBuffer[2], SendBuff, Length);
			DATASendLength = Length + 2;
			break;

/////////////////////////////////////////////////////////////////////////////
		case 8:													// 0:POST(SC)	: 2005.02.28
			break;

/////////////////////////////////////////////////////////////////////////////
		case 9:													// 9:SDB(SZ)	: 2005.04.21
			memcpy(&DATASendBuffer[0], Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&DATASendBuffer[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 10:												// 10:CUP(TJ)	 : 2005.05.12
		case 11:												// 11:CUP(BCTC)	 : 2005.08.17
		case 12:												// 12:CUP(JN)	 : 2005.09.11
		case 13:												// 13:CUP(WH8583): 2005.10.20
		case 14:												// 14:CUP(BJ)	 : 2006.03.04
		case 15:												// 15:CUP(SH)    : 2006.05.08
		case 16:												// 16:CUP(XM)    : 2006.05.08
		case 17:												// 17:CUP(BJHX)	 : 2006.05.20
		case 18:												// 18:CUP(BJGD)	 : 2006.07.07
		case 19:												// 19:CUP(DL)	 : 2006.07.15
		case 20:												// 20:CUP(SHGD)	 : 2006.09.22
		case 21:												// 21:CUP(JX)	 : 2007.03.06
		case 22:												// 22:CUP(QD)    : 2009.07.20
		case 23:												// 23:CUP(YT)    : 2009.07.20
		case 24:												// 24:CUP(TJ)	 : 2009.09.24 : CUP(TJ)
		case 25:												// 25:CUP(XX)    : 2005.09.11
		case 26:												// 26:CUP(XX)    : 2005.09.11
		case 27:												// 27:CUP(XX)    : 2005.09.11
		case 28:												// 28:CUP(XX)    : 2005.09.11
		case 29:												// 29:CUP(XX)    : 2005.09.11
		case 30:												// 30:CUP(XX)    : 2005.09.11

		case 100:												//100:CUP(DLABC) : 2006.12.07
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 104:												// 104:RCB(GS)	  : 2008.08.29
			DATASendBuffer[0] = HIBYTE(LOWORD(Length));			// 2005.03.08
			DATASendBuffer[1] = LOBYTE(LOWORD(Length));
			memcpy(&DATASendBuffer[2], SendBuff, Length);
			DATASendLength = Length + 2;
			break;	

		case 105:												// 105:HYB(BJ)	: 2008.11.05
		case 106:												// 106:WRB(BJ)	: 2009.02.12
		case 108:												// 108:ABC(XM)	: 2009.04.10
		case 111:												// 111:RCB(FZ)	: 2009.07.20
		case 113:												// 113:CB(HEB)	: 2009.09.01
		case 114:												// 114:IBK(TJ)	: 2011.01.06
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1000:												// 1000:CB(JN)   : 2005.10.20
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 1001:												// 1001:HXB(XA)		 : 2006.04.21
			memcpy(&DATASendBuffer[0], Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&DATASendBuffer[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
/////////////////////////////////////////////////////////////////////////////
		case 1002:												// 1002:CB(LZ)	 : 2006.06.01
		case 1003:												// 1003:CB(ZB)	 : 2006.06.01
		case 1004:												// 1004:CUP(GD)	 : 2006.06.28
		case 1005:												// 1005:CB(XA)	 : 2006.08.10
		case 1006:												// 1006:CB(ZH)	 : 2006.09.13
		case 1007:												// 1007:CB(SY)	 : 2006.09.13
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 2500:												//2500:POST-TY(HNC)			// 2007.05.15 : ATMU   长连接
		case 2501:												//2501:POST-TY(HN)			// 2007.05.15 : ATMU   短连接
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1008:												// 1008:HXB(XA)T-DES : 2008.02.28
		case 2003:												// 2003:CB(NT)XML	: 2008.03.27
		case 2004:												// 2004:CB(NTC)XML	: 2008.03.27
		case 3000:												// 3000:HXB(WX)		: 2006.12.07
			memcpy(&DATASendBuffer[0], Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&DATASendBuffer[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 3001:												// 3001:RCB(SZ)  : 2007.01.05
		case 3002:												// 3002:RCB(SZC) : 2007.01.05
		case 3006:												// 3006:RCB(CS)  : 2008.11.26
		case 3008:												// 3008:RCB(SZ)  : 2010.07.08(NEW)
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 3005:												// 3005:HXB(WX)T-DES : 2008.02.28
			memcpy(&DATASendBuffer[0], Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&DATASendBuffer[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 4001:												// 4001:BOCOM(BJ): 2007.01.17
			memcpy(DATASendBuffer, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 4002:												// 4002:ICBC(SY): 2007.04.20
			DATASendLength = Length + 2;
			DATASendBuffer[0] = HIBYTE(LOWORD(DATASendLength));
			DATASendBuffer[1] = LOBYTE(LOWORD(DATASendLength));
			memcpy(&DATASendBuffer[2], SendBuff, Length);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 6000:												// 6000:HXB(DL)		 : 2007.12.03
			memcpy(&DATASendBuffer[0], Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&DATASendBuffer[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		default:												// OTHER
			memcpy(&DATASendBuffer[0], Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&DATASendBuffer[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
	}
	
	if (Send(DATASendBuffer, DATASendLength) != DATASendLength)
		return FALSE;
	
	DATASendLength = 0;											// SEND LENGTH CLEAR
	return TRUE;
}

int CSocketCtrlB::DATARecvSocket(BYTE *RecvBuff, int *Length)
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

int CSocketCtrlB::ProcessData()
{
	BYTE	TempBuffer[TCPBUFFSIZE];
	int		nByte;
	int		CheckDataFlag;

	InitSocket();

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

int CSocketCtrlB::CheckData()
{
	if ((m_ConfBuff.InterfaceKind == 7)	||						//  7:ABC(BJ)	: 2004.05.08
		(m_ConfBuff.InterfaceKind == 104))						//104:RCB(GS)	: 2008.08.29
	{
		int		TCP_BASE_LEN = 2;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = MAKEWORD(TCPRecvBuffer[1], TCPRecvBuffer[0]);
																// 2005.03.08
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 9)							// 9:SDB(SZ)	: 2005.04.21
	{
		int		TCP_BASE_LEN = 4;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = Asc2Int(pTCPRecvBuffer->Data, 4);
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;	

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 10)	||					// 10:CUP(TJ)	 : 2005.05.12
		(m_ConfBuff.InterfaceKind == 11)	||					// 11:CUP(BCTC)	 : 2005.08.17
		(m_ConfBuff.InterfaceKind == 12)	||					// 12:CUP(JN)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 13)	||					// 13:CUP(WH8583): 2005.10.20
		(m_ConfBuff.InterfaceKind == 14)	||					// 14:CUP(BJ)	 : 2006.03.04
		(m_ConfBuff.InterfaceKind == 15)	||					// 15:CUP(SH)    : 2006.05.08
		(m_ConfBuff.InterfaceKind == 16)	||					// 16:CUP(XM)    : 2006.05.08
		(m_ConfBuff.InterfaceKind == 17)	||					// 17:CUP(BJHX)	 : 2006.05.20
		(m_ConfBuff.InterfaceKind == 18)	||					// 18:CUP(BJGD)	 : 2006.07.07
		(m_ConfBuff.InterfaceKind == 19)	||					// 19:CUP(DL)	 : 2006.07.15
		(m_ConfBuff.InterfaceKind == 20)	||					// 20:CUP(SHGD)	 : 2006.09.22
		(m_ConfBuff.InterfaceKind == 21)	||					// 21:CUP(JX)	 : 2007.03.06
		(m_ConfBuff.InterfaceKind == 22)	||					// 22:CUP(QD)	 : 2009.07.20
		(m_ConfBuff.InterfaceKind == 23)	||					// 23:CUP(YT) 	 : 2009.07.20
		(m_ConfBuff.InterfaceKind == 24)	||					// 24:CUP(TJ)	 : 2009.09.24 : CUP(TJ)
		(m_ConfBuff.InterfaceKind == 25)	||					// 25:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 26)	||					// 26:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 27)	||					// 27:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 28)	||					// 28:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 29)	||					// 29:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 30)	||					// 30:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 100)	||					//100:CUP(DLABC) : 2006.12.07
		(m_ConfBuff.InterfaceKind == 106))						//106:WRB(BJ)	 : 2009.02.12
	{
		int		TCP_BASE_LEN = 12;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = TCPRecvLength - TCP_BASE_LEN;			// 2006.11.14
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;	

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 108)						// 108:ABC(XM)	: 2009.04.10
	{
		int		TCP_BASE_LEN = 4;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[0], 4);
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}		
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 111)						// 111:RCB(FZ)	 : 2009.07.20
	{
		int		TCP_BASE_LEN = 8;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		CheckDataLength = MAKEWORD(TCPRecvBuffer[1], TCPRecvBuffer[0]) - TCP_BASE_LEN;
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 114)						// 114:IBK(TJ)	 : 2011.01.06
	{
		int		TCP_BASE_LEN = 12;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = TCPRecvLength - TCP_BASE_LEN;			// 2006.11.14
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;	

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 1000)	||					// 1000:CB(JN)	 : 2005.10.20 : DC
		(m_ConfBuff.InterfaceKind == 1002)	||					// 1002:CB(LZ)	 : 2006.06.01 : DC
		(m_ConfBuff.InterfaceKind == 1003)	||					// 1003:CB(ZB)	 : 2006.06.01 : DC
		(m_ConfBuff.InterfaceKind == 1004)	||					// 1004:CUP(GD)	 : 2006.06.28 : DC
		(m_ConfBuff.InterfaceKind == 1005)	||					// 1005:CB(XA)	 : 2006.08.10 : DC
		(m_ConfBuff.InterfaceKind == 1006)	||					// 1006:CB(ZH)	 : 2006.09.13 : DC
		(m_ConfBuff.InterfaceKind == 1007))						// 1007:CB(SY)	 : 2006.09.13 : DC
	{
		int		TCP_BASE_LEN = 16;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength == 4)
			TCP_BASE_LEN = 0;

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength == 4)
			CheckDataLength = TCPRecvLength;
		else
			CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[8], 8);

		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;	

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 105)	||					// 105:HYB(BJ)	: 2008.11.05
		(m_ConfBuff.InterfaceKind == 1001)	||					// 1001:HXB(XA)		 : 2006.04.21 : DC
		(m_ConfBuff.InterfaceKind == 1008)	||					// 1008:HXB(XA)T-DES : 2008.02.28 : DC
		(m_ConfBuff.InterfaceKind == 3000)	||					// 3000:HXB(WX)		 : 2006.12.07 : YY
		(m_ConfBuff.InterfaceKind == 3005)	||					// 3005:HXB(WX)T-DES : 2008.02.28 : YY
		(m_ConfBuff.InterfaceKind == 6000))						// 6000:HXB(DL)		 : 2007.12.03 : ZL
	{
		int		TCP_BASE_LEN = 4;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[0], 4);
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;	

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 113)		||				// 113:CB(HEB)	 : 2011.03.03
		(m_ConfBuff.InterfaceKind == 2003)		||				// 2003:CB(NT)XML: 2008.03.27
		(m_ConfBuff.InterfaceKind == 2004))						// 2004:CB(NTC)XML: 2008.03.27
	{
		int		TCP_BASE_LEN = 4;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[0], 4);
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 2500)	||					//2500:POST-TY(HNC)			// 2007.05.15 : ATMU   长连接
		(m_ConfBuff.InterfaceKind == 2501))						//2501:POST-TY(HN)			// 2007.05.15 : ATMU   短连接
	{
		int		TCP_BASE_LEN = 21;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
//		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[8], 4);
		CheckDataLength = pTCPRecvBuffer->Data[0] * 256 + pTCPRecvBuffer->Data[1];
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + 2;					// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 3001)		||				// 3001:RCB(SZ)	 : 2007.01.05
		(m_ConfBuff.InterfaceKind == 3002)		||				// 3002:RCB(SZC) : 2007.01.05
		(m_ConfBuff.InterfaceKind == 3006))						// 3006:RCB(CS)  : 2008.11.26
	{
		int		TCP_BASE_LEN = 8;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		CheckDataLength = MAKEWORD(TCPRecvBuffer[1], TCPRecvBuffer[0]) - TCP_BASE_LEN;
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 3008)						// 3008:RCB(SZ) : 2010.07.08(NEW)
	{
		int		TCP_BASE_LEN = 21;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
//		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[8], 4);
		CheckDataLength = pTCPRecvBuffer->Data[0] * 256 + pTCPRecvBuffer->Data[1];
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + 2;					// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 4001)						// 4001:BOCOM(BJ) : 2007.01.17
	{
		int		TCP_BASE_LEN = 0;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = TCPRecvLength;
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;	

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 4002)						// 4002:ICBC(SY): 2007.04.20
	{
		int		TCP_BASE_LEN = 2;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = MAKEWORD(TCPRecvBuffer[1], TCPRecvBuffer[0]);
																// 2005.03.08
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength;						// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}

	return TRUE;												// Clear
}	
	
int CSocketCtrlB::CheckTCPError()
{
	return (TCPErrID = WSAGetLastError());
}
