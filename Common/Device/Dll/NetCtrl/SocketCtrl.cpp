/////////////////////////////////////////////////////////////////////////////
// SocketCtrl.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\SocketCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl

CSocketCtrl::CSocketCtrl(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;
	
	InitSocket();												// Variable Initialize
}

CSocketCtrl::~CSocketCtrl()
{
}

// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketCtrl, CAsyncSocket)
	//{{AFX_MSG_MAP(CSocketCtrl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl member functions

void CSocketCtrl::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnAccept ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CSocketCtrl::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnClose ERROR] [%d]", nErrorCode);
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] OnClose");
	TCPConnected = FALSE;

	CAsyncSocket::OnClose(nErrorCode);
}

void CSocketCtrl::OnConnect(int nErrorCode) 
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

void CSocketCtrl::OnOutOfBandData(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnOutOfBandData ERROR] [%d]", nErrorCode);
	
	CAsyncSocket::OnOutOfBandData(nErrorCode);
}

void CSocketCtrl::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnReceive ERROR] [%d]", nErrorCode);

	if (!nErrorCode)
		 ProcessData();
	
	CAsyncSocket::OnReceive(nErrorCode);
}

void CSocketCtrl::OnSend(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP OnSend ERROR] [%d]", nErrorCode);

	CAsyncSocket::OnSend(nErrorCode);
}

int CSocketCtrl::Receive(void* lpBuf, int nBufLen, int nFlags) 
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
///2014.04.19 by jl 当CardDataFlag为0时不在log中记录日志，此情况与插卡时磁道信息处理相同
		if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "CardDataFlag", 0) == 1)
		HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPIP RECVDATA]", (char*)lpBuf, nByte);
	}

	return nByte;
}

int CSocketCtrl::Send(const void* lpBuf, int nBufLen, int nFlags) 
{
	// TODO: Add your specialized code here and/or call the base class
///2014.04.19 by jl 当CardDataFlag为0时不在log中记录日志，此情况与插卡时磁道信息处理相同
	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "CardDataFlag", 0) == 1)	
	HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCPIP SENDDATA]", (char*)lpBuf, nBufLen);
	int nByte = CAsyncSocket::Send(lpBuf, nBufLen, nFlags);

	if (nByte != nBufLen)
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Send ERROR] [%d]", CheckTCPError());

	return nByte;
}

/////////////////////////////////////////////////////////////////////////////
int CSocketCtrl::InitSocket()
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

	pTCPRecvBuffer = (TCPHeader *)TCPRecvBuffer;

	pDATASendBuffer = (TCPHeader *)DATASendBuffer;

	CheckRecvLength = 0;										// Tran Recv Length
	CheckDataLength = 0;										// Tran Data Length

	return TRUE;
}

int CSocketCtrl::OpenSocket()
{
	char		IpAddress[256];
	int			PortNumber;

	sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_ConfBuff.BpIpAddress, 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress[4], 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress[8], 3),
									  Asc2Int(&m_ConfBuff.BpIpAddress[12], 3));
	PortNumber = Asc2Int(m_ConfBuff.BpPortNo, 5);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] OpenSocket Ip:%s Port:%d", IpAddress, PortNumber);

	InitSocket();

	if (!Socket(SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_CONNECT | FD_CLOSE))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Socket ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	if (!Connect((LPCTSTR)IpAddress, PortNumber))
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

int CSocketCtrl::OpenSocketIP(LPCTSTR szIpAddress, int nPortNumber)
{
	CString			strIpAddress(szIpAddress);
	CStringArray	strIpAddressArray;
	char			IpAddress[256];
	int				PortNumber;

	SplitString(strIpAddress, ".", strIpAddressArray);			// 2005.10.25
	if (strIpAddressArray.GetSize() >= 4)
	{
		sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(strIpAddressArray[0]),
										  Asc2Int(strIpAddressArray[1]),
										  Asc2Int(strIpAddressArray[2]),
										  Asc2Int(strIpAddressArray[3]));
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] OpenSocket Ip:%s Port:%d", szIpAddress, nPortNumber);
		return FALSE;
	}

	PortNumber = nPortNumber;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] OpenSocket Ip:%s Port:%d", IpAddress, PortNumber);

	InitSocket();

	if (!Socket(SOCK_STREAM, FD_READ | FD_WRITE | FD_OOB | FD_CONNECT | FD_CLOSE))
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP Socket ERROR] [%d]", CheckTCPError());
		return FALSE;
	}

	if (!Connect((LPCTSTR)IpAddress, PortNumber))
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

int CSocketCtrl::CloseSocket()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP/IP] CloseSocket");

	ShutDown(CAsyncSocket::both);
	Close();

	return TRUE;
}

int CSocketCtrl::SendSocket(BYTE *SendBuff, int Length)
{
	DATASendLength = 0;											// SEND LENGTH CLEAR
	memset(DATASendBuffer, 0, TCPBUFFSIZE);						// SEND BUFFER CLEAR

	switch (m_ConfBuff.InterfaceKind)
	{
/////////////////////////////////////////////////////////////////////////////
		case 0:													// 0:CUP(WH)	: 2004.05.08
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 1:													// 1:BOC(SD)	: 2004.05.08
		case 2:													// 2:ABC(SD)	: 2004.06.18
		case 3:													// 3:ABC(FZ)	: 2004.06.25
		case 4:													// 4:RCC(GZ)	: 2004.11.05
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 5:													// 5:ABC(ZJG)	: 2004.11.05
			pDATASendBuffer->Data[0] = 0x01;
			if (Length >= 4)									// Length Not Include Crc Length
				memcpy(&pDATASendBuffer->Data[1], GetSprintf("%3.3d", Length - 4).GetBuffer(0), 3);
			else
				memcpy(&pDATASendBuffer->Data[1], "000", 3);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
		
/////////////////////////////////////////////////////////////////////////////
		case 6:													// 6:CCB(JX)	: 2004.11.05
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 7:													// 7:ABC(BJ)	: 2004.11.05
			pDATASendBuffer->Data[0] = HIBYTE(LOWORD(Length));	// 2005.03.08
			pDATASendBuffer->Data[1] = LOBYTE(LOWORD(Length));
			memcpy(&pDATASendBuffer->Data[2], SendBuff, Length);
			DATASendLength = Length + 2;
			break;

/////////////////////////////////////////////////////////////////////////////
		case 8:													// 0:POST(SC)	: 2005.02.28
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 9:													// 9:SDB(SZ)	: 2005.04.21
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 10:												// 10:CUP(TJ)	 : 2005.05.12
		case 11:												// 11:CUP(BCTC)	 : 2005.08.17
		case 12:												// 12:CUP(JN)	 : 2005.09.11
		case 13:												// 13:CUP(WH8583): 2005.10.20
		case 14:												// 14:CUP(BJ)    : 2006.03.04
		case 15:												// 15:CUP(SH)    : 2006.05.08
		case 16:												// 16:CUP(XM)    : 2006.05.08
		case 17:												// 17:CUP(BJHX)  : 2006.05.20
		case 18:												// 18:CUP(BJGD)	 : 2006.07.07
		case 19:												// 19:CUP(DL)	 : 2006.07.15
		case 20:												// 20:CUP(SHGD)	 : 2006.09.22
		case 21:												// 21:CUP(JX)	 : 2007.03.06
		case 22:												// 22:CUP(QD)    : 2009.07.20
		case 23:												// 23:CUP(YT)    : 2009.07.20
		case 24:												// 24:CUP(TJ)    : 2009.09.24 : CUP(TJ)
		case 25:												// 25:CUP(XX)    : 2005.09.11
		case 26:												// 26:CUP(XX)    : 2005.09.11
		case 27:												// 27:CUP(XX)    : 2005.09.11
		case 28:												// 28:CUP(XX)    : 2005.09.11
		case 29:												// 29:CUP(XX)    : 2005.09.11
		case 30:												// 30:CUP(XX)    : 2005.09.11

		case 100:												// 100:CUP(DLABC) : 2006.12.07
		case 101:												// 101:RCB(HF)	  : 2007.10.30
		case 102:												// 102:RCB(JX)	  : 2008.01.16
		case 103:												// 103:CB(TS)	  : 2008.06.10
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 104:												// 104:RCB(GS)	  : 2008.08.29
			pDATASendBuffer->Data[0] = HIBYTE(LOWORD(Length));
			pDATASendBuffer->Data[1] = LOBYTE(LOWORD(Length));
			memcpy(&pDATASendBuffer->Data[2], SendBuff, Length);
			DATASendLength = Length + 2;
			break;

/////////////////////////////////////////////////////////////////////////////			
		case 105:												// 105:HYB(BJ)	: 2008.11.05
		case 106:												// 106:WRB(BJ)	: 2009.02.12
		case 107:												// 107:CB(TYC)	: 2009.04.07
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 108:												// 108:ABC(XM)	: 2009.04.10
			pDATASendBuffer->Data[0] = HIBYTE(LOWORD(Length));	// 2005.03.08
			pDATASendBuffer->Data[1] = LOBYTE(LOWORD(Length));
			memcpy(&pDATASendBuffer->Data[2], SendBuff, Length);
			DATASendLength = Length + 2;
			break;

/////////////////////////////////////////////////////////////////////////////
		case 109:												// 109:CB(ERDS)	  : 2009.04.20
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 110:												// 110:SHB(BJ)	  : 2009.05.20
		case 111:												// 111:RCB(FZ)	  : 2009.07.20
		case 112:												// 112:CB(TY)	  : 2009.07.20
		case 113:												// 113:CB(HEB)	  : 2009.09.01
		case 114:												// 114:IBK(TJ)	  : 2011.01.06
		case 115:												// 115:CB(AS)	  : 2011.01.06
		case 116:												// 116:CB(WF)	  : 2011.07.25
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1000:												// 1000:CB(JN)    : 2005.10.25
			if ((memcmp(&SendBuff[1], "RQK", 3) == 0)	||
				(memcmp(&SendBuff[1], "CWC", 3) == 0)	||
				(memcmp(&SendBuff[1], "OEX", 3) == 0)	||
				(memcmp(&SendBuff[1], "TTI", 3) == 0)	||
				(memcmp(&SendBuff[1], "RWT", 3) == 0)	||
				(memcmp(&SendBuff[1], "CWD", 3) == 0)	||
				(memcmp(&SendBuff[1], "INQ", 3) == 0)	||
				(memcmp(&SendBuff[1], "PIN", 3) == 0)	||
				(memcmp(&SendBuff[1], "TFW", 3) == 0))
			{
				memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4), 4);
				memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
				DATASendLength = Length + 4;					// DATA + LENGTH
			}
			else
			{
				memcpy(pDATASendBuffer->Data, GetSprintf("%04x", Length), 4);
				memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
				DATASendLength = Length + 4;					// DATA + LENGTH
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1001:												// 1001:HXB(XA)	  : 2006.04.21
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1002:												// 1002:CB(LZ)	  : 2006.06.01
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1003:												// 1003:CB(ZB)	  : 2006.06.01
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1004:												// 1004:CUP(GD)	  : 2006.06.28
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

////////////////////////////////////////////////////////////////////////////
		case 1005:												// 1005:CB(XA)	  : 2006.08.10
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

////////////////////////////////////////////////////////////////////////////
		case 1006:												// 1006:CB(ZH)	  : 2006.09.13
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

////////////////////////////////////////////////////////////////////////////
		case 1007:												// 1007:CB(SY)	  : 2006.09.13
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1008:												// 1008:HXB(XA)T-DES   : 2008.02.28
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 2000:												// 2000:CB(NT)    : 2005.11.11
		case 2001:												// 2001:CB(NTC)   : 2006.02.06
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 2003:												// 2003:CB(NT) XML: 2008.03.27 : FT
		case 2004:												// 2004:CB(NTC)XML: 2008.03.27 : FT
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

		case 2500:												//2500:POST-TY(HNC)			// 2007.05.15 : ATMU   长连接
		case 2501:												//2501:POST-TY(HN)			// 2007.05.15 : ATMU   短连接
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;
			
/////////////////////////////////////////////////////////////////////////////
		case 3000:												// 3000:HXB(WX)	  : 2006.12.07
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 3001:												// 3001:RCB(SZ)   : 2007.01.05
		case 3002:												// 3002:RCB(SZC)  : 2007.01.05
		case 3003:												// 3003:RCB(KS)   : 2007.03.26
		case 3004:												// 3004:RCB(NB)   : 2007.11.07
		case 3006:												// 3006:RCB(CS)	  : 2008.11.26
		case 3007:												// 3007:RCB(XZ)	  : 2008.11.26
		case 3008:												// 3008:RCB(SZ)   : 2010.07.08(NEW)
		case 3009:												// 3009:RCB(KS)TMP: 2011.03.24

		case 4000:												// 4000:CB(JZ)    : 2006.12.10
		case 4001:												// 4001:BOCOM(BJ) : 2007.01.17
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 4002:												// 4002:ICBC(SY)   : 2007.04.20
			DATASendLength = Length + 2;
			pDATASendBuffer->Data[0] = HIBYTE(LOWORD(DATASendLength));
			pDATASendBuffer->Data[1] = LOBYTE(LOWORD(DATASendLength));
			memcpy(&pDATASendBuffer->Data[2], SendBuff, Length);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 4003:												// 4003:CB(ZG)	  : 2007.06.28
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
		case 5000:												// 5000:CB(CZ)	   : 2007.04.20
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;

/////////////////////////////////////////////////////////////////////////////
		case 6000:												// 6000:HXB(DL)	   : 2007.12.03 : ZL
			memcpy(pDATASendBuffer->Data, SendBuff, Length);
			DATASendLength = Length;							// DATA
			break;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
		case 7000:												// 7000:CUP(PBOC) : 2010.10.20
			DATASendLength = 0;
			pDATASendBuffer->Data[DATASendLength++] = A_STX;
			pDATASendBuffer->Data[DATASendLength++] = HIBYTE(LOWORD(Length));
			pDATASendBuffer->Data[DATASendLength++] = LOBYTE(LOWORD(Length));
			memcpy(&pDATASendBuffer->Data[DATASendLength], SendBuff, Length);
			DATASendLength += Length;
			pDATASendBuffer->Data[DATASendLength++] = A_ETX;
			pDATASendBuffer->Data[DATASendLength++] = MakeBcc(&pDATASendBuffer->Data[1], DATASendLength - 1);
			break;
/////////////////////////////////////////////////////////////////////////////
		default:												// OTHER
			memcpy(pDATASendBuffer->Data, Int2Asc(Length, 4).GetBuffer(0), 4);
			memcpy(&pDATASendBuffer->Data[4], SendBuff, Length);
			DATASendLength = Length + 4;						// DATA + LENGTH
			break;
	}

	if (Send(DATASendBuffer, DATASendLength) != DATASendLength)
		return FALSE;
	
	DATASendLength = 0;											// SEND LENGTH CLEAR
	return TRUE;
}

int CSocketCtrl::DATARecvSocket(BYTE *RecvBuff, int *Length)
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
int CSocketCtrl::ProcessData()
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

int CSocketCtrl::CheckData()
{
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 0)							// 0:CUP(WH)	: 2004.05.08
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
	if (m_ConfBuff.InterfaceKind == 1)							// 1:BOC(SD)	: 2004.05.08
	{
		int		TCP_BASE_LEN = 0;									

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 1)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (pTCPRecvBuffer->Data[0] == 'A')						// 'A' Host Response
			CheckDataLength =  43;
		else
		if (pTCPRecvBuffer->Data[0] == 'B')						// 'B' Host Response
			CheckDataLength =  75;
		else
		if (pTCPRecvBuffer->Data[0] == 'C')						// 'C' Host Response
			CheckDataLength =  422;
		else
		if (pTCPRecvBuffer->Data[0] == 'H')						// 'H' Host Response
			CheckDataLength =  27;
		else
		if (pTCPRecvBuffer->Data[0] == 'Q')						// 'Q' Host Response
			CheckDataLength =  70;
		else
		if (pTCPRecvBuffer->Data[0] == 'R')						// 'R' Host Response
		{														
			if (memcmp(pDATASendBuffer->Data, "INQ", 3) == 0)	// INQ Host Response
				CheckDataLength =  106;
			else												
			if (memcmp(pDATASendBuffer->Data, "RCD", 3) == 0)	// RCD Host Response
				CheckDataLength =  336;
			else
			{
				TCPRecvLength = 0;
				memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
				return FALSE;
			}
		}
		else
		if (pTCPRecvBuffer->Data[0] == 'S')						// 'S' Host Response
			CheckDataLength =  62;
		else
		if (pTCPRecvBuffer->Data[0] == 'T')						// 'T' Host Response
			CheckDataLength =  47;
		else
		if (pTCPRecvBuffer->Data[0] == 'W')						// 'W' Host Response
			CheckDataLength =  70;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	if ((m_ConfBuff.InterfaceKind == 2)		||					// 2:ABC(SD)	: 2004.06.18
		(m_ConfBuff.InterfaceKind == 3))						// 3:ABC(FZ)	: 2004.06.25
	{															
		int		TCP_BASE_LEN = 0;								
																
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 2)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (pTCPRecvBuffer->Data[1] == 'A')						// 'A' Host Response
			CheckDataLength =  24;
		else 
		if (pTCPRecvBuffer->Data[1] == 'B')						// 'B' Host Response
			CheckDataLength =  32;
		else 
		if (pTCPRecvBuffer->Data[1] == 'G')						// 'G' Host Response
			CheckDataLength =  31;			
		else 
		if (pTCPRecvBuffer->Data[1] == 'I')						// 'I' Host Response
			CheckDataLength =  TCPRecvLength;				
		else
		if (pTCPRecvBuffer->Data[1] == 'N')						// 'N' Host Response
			CheckDataLength =  104;
		else
		if (pTCPRecvBuffer->Data[1] == 'Q')						// 'Q' Host Response
			CheckDataLength =  76;
		else
		if (pTCPRecvBuffer->Data[1] == 'R')						// 'R' Host Response
			CheckDataLength =  100;		
		else 
		if (pTCPRecvBuffer->Data[1] == 'S')						// 'S' Host Response
			CheckDataLength =  61;				
		else 
		if (pTCPRecvBuffer->Data[1] == 'T')						// 'T' Host Response
			CheckDataLength =  31;	
		else 
		if (pTCPRecvBuffer->Data[1] == 'U')						// 'U' Host Response
			CheckDataLength =  35;			
		else
		if (pTCPRecvBuffer->Data[1] == 'W')						// 'W' Host Response
			CheckDataLength =  76;
		else
		if (pTCPRecvBuffer->Data[1] == 'Z')						// 'Z' Host Response
			CheckDataLength =  76;	
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	if (m_ConfBuff.InterfaceKind == 4)							// 4:RCC(GZ)	: 2004.11.05
	{
		int		TCP_BASE_LEN = 0;									

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 2)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (pTCPRecvBuffer->Data[1] == 'A')						// 'A' Host Response
			CheckDataLength =  18;
		else
		if (pTCPRecvBuffer->Data[1] == 'B')						// 'B' Host Response
			CheckDataLength =  26;
		else
		if (pTCPRecvBuffer->Data[1] == 'C')						// 'C' Host Response
			CheckDataLength =  111;
		else
		if (pTCPRecvBuffer->Data[1] == 'D')						// 'D' Host Response
			CheckDataLength =  72;
		else
		if (pTCPRecvBuffer->Data[1] == 'H')						// 'H' Host Response
			CheckDataLength =  27;
		else
		if (pTCPRecvBuffer->Data[1] == 'L')						// 'L' Host Response
			CheckDataLength =  28;
		else
		if (pTCPRecvBuffer->Data[1] == 'Q')						// 'Q' Host Response
			CheckDataLength =  114;
		else
		if (pTCPRecvBuffer->Data[1] == 'R')						// 'R' Host Response
			CheckDataLength =  138;
		else
		if (pTCPRecvBuffer->Data[1] == 'S')						// 'S' Host Response
			CheckDataLength =  74;
		else
		if (pTCPRecvBuffer->Data[1] == 'T')						// 'T' Host Response
			CheckDataLength =  80;
		else
		if (pTCPRecvBuffer->Data[1] == 'U')						// 'U' Host Response
			CheckDataLength =  84;
		else
		if (pTCPRecvBuffer->Data[1] == 'V')						// 'V' Host Response
			CheckDataLength =  26;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	if (m_ConfBuff.InterfaceKind == 5)							// 5:ABC(ZJG)	: 2004.11.05
	{
		int		TCP_BASE_LEN = 4;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[1], 3);
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
	if (m_ConfBuff.InterfaceKind == 6)							// 6:CCB(JX)	: 2004.11.05
	{
		int		TCP_BASE_LEN = 0;									

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 2)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (pTCPRecvBuffer->Data[1] == 'A')						// 'A' Host Response
			CheckDataLength =  36;
		else
		if (pTCPRecvBuffer->Data[1] == 'B')						// 'B' Host Response
			CheckDataLength =  44;
		else
		if (pTCPRecvBuffer->Data[1] == 'I')						// 'I' Host Response
		{														// RDW Host Response
			if (memcmp(&pDATASendBuffer->Data[1], "RDW", 3) == 0)
				CheckDataLength =  74;
			else												// RFM Host Response	
			if (memcmp(&pDATASendBuffer->Data[1], "RFM", 3) == 0)
				CheckDataLength =  TCPRecvLength;				
			else
			{
				TCPRecvLength = 0;
				memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
				return FALSE;
			}
		}
		else
		if (pTCPRecvBuffer->Data[1] == 'Q')						// 'Q' Host Response
			CheckDataLength =  88;
		else
		if (pTCPRecvBuffer->Data[1] == 'R')						// 'R' Host Response
			CheckDataLength =  112;
		else
		if (pTCPRecvBuffer->Data[1] == 'S')						// 'S' Host Response
			CheckDataLength =  73;
		else
		if (pTCPRecvBuffer->Data[1] == 'T')						// 'T' Host Response
			CheckDataLength =  43;
		else
		if (pTCPRecvBuffer->Data[1] == 'U')						// 'U' Host Response
			CheckDataLength =  43;
		else
		if (pTCPRecvBuffer->Data[1] == 'W')						// 'W' Host Response
			CheckDataLength =  88;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	if ((m_ConfBuff.InterfaceKind == 7)		||					// 7:ABC(BJ)	: 2004.05.08
		(m_ConfBuff.InterfaceKind == 104)	||					// 104:RCB(GS)	: 2008.08.29
		(m_ConfBuff.InterfaceKind == 108))						// 108:ABC(XM)	: 2009.04.10
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
	if (m_ConfBuff.InterfaceKind == 8)							// 0:POST(SC)	: 2005.02.28
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
		(m_ConfBuff.InterfaceKind == 23)	||					// 23:CUP(YT)	 : 2009.07.20
		(m_ConfBuff.InterfaceKind == 24)	||					// 24:CUP(TJ)	 : 2009.09.24 : CUP(TJ)
		(m_ConfBuff.InterfaceKind == 25)	||					// 25:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 26)	||					// 26:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 27)	||					// 27:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 28)	||					// 28:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 29)	||					// 29:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 30)	||					// 30:CUP(XX)	 : 2005.09.11
		(m_ConfBuff.InterfaceKind == 106))						// 106:WRB(BJ)	 : 2009.02.12
	{
		int		TCP_BASE_LEN = 12;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[8], 4);
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
	if (m_ConfBuff.InterfaceKind == 100)						//100:CUP(DLABC) : 2006.12.07
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
	if (m_ConfBuff.InterfaceKind == 101)						// 101:RCB(HF)	: 2007.10.30
	{
		int		TCP_BASE_LEN = 0;
			
		if (memcmp(&TCPRecvBuffer[0], "AP", 2) == 0)
			CheckDataLength = 42;
		else
		if (memcmp(&TCPRecvBuffer[0], "BP", 2) == 0)
			CheckDataLength = 74;
		else
		if (memcmp(&TCPRecvBuffer[0], "CP", 2) == 0)
			CheckDataLength = 152;
		else
		if (memcmp(&TCPRecvBuffer[0], "EP", 2) == 0)
			CheckDataLength = 71;
		else
		if (memcmp(&TCPRecvBuffer[0], "HP", 2) == 0)
			CheckDataLength = 43;
		else
		if (memcmp(&TCPRecvBuffer[0], "KP", 2) == 0)
			CheckDataLength = 142;
		else
		if (memcmp(&TCPRecvBuffer[0], "PP", 2) == 0)
			CheckDataLength = 147;
		else
		if (memcmp(&TCPRecvBuffer[0], "QP", 2) == 0)
			CheckDataLength = 90;
		else
		if (memcmp(&TCPRecvBuffer[0], "RP", 2) == 0)
			CheckDataLength = 110;
		else
		if (memcmp(&TCPRecvBuffer[0], "SP", 2) == 0)
			CheckDataLength = 70;
		else
		if (memcmp(&TCPRecvBuffer[0], "TP", 2) == 0)
			CheckDataLength = 55;
		else
		if (memcmp(&TCPRecvBuffer[0], "WP", 2) == 0)
			CheckDataLength = 90;
		else
		if (memcmp(&TCPRecvBuffer[0], "4106", 4) == 0)
			CheckDataLength = 4;
		else
		if (memcmp(&TCPRecvBuffer[0], "4107", 4) == 0)
			CheckDataLength = 4;
		else
		if (memcmp(&TCPRecvBuffer[0], "4108", 4) == 0)
			CheckDataLength = 4;
		else
		if (memcmp(&TCPRecvBuffer[0], "4444", 4) == 0)
			CheckDataLength = TCPRecvLength;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}
												
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
												
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;
												
		DATARecvInTblTemp = DATARecvInTbl;						// Data Recv Ok
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
				DATARecvInTblTemp = 0;

		// Data Recv Ok
		if (DATARecvInTblTemp != DATARecvOutTbl)
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
						&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
													
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 102)						// 102:RCB(JX)	: 2008.01.16
	{
		int		TCP_BASE_LEN = 0;									
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		if (TCPRecvLength < 3)									// Header Length Check
			return FALSE;										// Recv Waiting
		
		if (memcmp(pTCPRecvBuffer->Data, "AAP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ABP", 3) == 0)
			CheckDataLength = 50;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ADP", 3) == 0)
			CheckDataLength = 101;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AEP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AFP", 3) == 0)
			CheckDataLength = 143;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AGP", 3) == 0)
			CheckDataLength = 44;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AHP", 3) == 0)
			CheckDataLength = 59;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AQP", 3) == 0)
			CheckDataLength = 83;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ARP", 3) == 0)
			CheckDataLength = 99;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ASP", 3) == 0)
			CheckDataLength = 61;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ATP", 3) == 0)
			CheckDataLength = 46;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AUP", 3) == 0)
			CheckDataLength = 50;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AWP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}
		
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
	if (m_ConfBuff.InterfaceKind == 103)						// 103:CB(TS)	: 2008.06.10
	{
		int		TCP_BASE_LEN = 0;
				
		if (memcmp(&TCPRecvBuffer[0], "AP", 2) == 0)
			CheckDataLength = 42;
		else
		if (memcmp(&TCPRecvBuffer[0], "BP", 2) == 0)
			CheckDataLength = 125;
		else
		if (memcmp(&TCPRecvBuffer[0], "CP", 2) == 0)
			CheckDataLength = 109;
		else
		if (memcmp(&TCPRecvBuffer[0], "HP", 2) == 0)
			CheckDataLength = 43;
		else
		if (memcmp(&TCPRecvBuffer[0], "IP", 2) == 0)
			CheckDataLength = 54;
		else
		if (memcmp(&TCPRecvBuffer[0], "QP", 2) == 0)
			CheckDataLength = 90;
		else
		if (memcmp(&TCPRecvBuffer[0], "RP", 2) == 0)
			CheckDataLength = 110;
		else
		if (memcmp(&TCPRecvBuffer[0], "SP", 2) == 0)
			CheckDataLength = 70;
		else
		if (memcmp(&TCPRecvBuffer[0], "TP", 2) == 0)
			CheckDataLength = 55;
		else
		if (memcmp(&TCPRecvBuffer[0], "WP", 2) == 0)
			CheckDataLength = 90;
		else
		if (memcmp(&TCPRecvBuffer[0], "XP", 2) == 0)
			CheckDataLength = 62;
		else
		if (memcmp(&TCPRecvBuffer[0], "4106", 4) == 0)
			CheckDataLength = 4;
		else
		if (memcmp(&TCPRecvBuffer[0], "4107", 4) == 0)
			CheckDataLength = 4;
		else
		if (memcmp(&TCPRecvBuffer[0], "4108", 4) == 0)
			CheckDataLength = 4;
		else
		if (memcmp(&TCPRecvBuffer[0], "4444", 4) == 0)
			CheckDataLength = TCPRecvLength;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}
		
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;
		
		DATARecvInTblTemp = DATARecvInTbl;						// Data Recv Ok
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		// Data Recv Ok
		if (DATARecvInTblTemp != DATARecvOutTbl)
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 105)						// 105:HYB(BJ)	: 2008.11.05
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
	if (m_ConfBuff.InterfaceKind == 107)						// 107:CB(TYC)	: 2009.04.07
	{
		int		TCP_BASE_LEN = 0;
				
		if (memcmp(&TCPRecvBuffer[0], "ABP", 3) == 0)
			CheckDataLength = 55;
		else
		if (memcmp(&TCPRecvBuffer[0], "AAP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "ASP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "ARP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "AQP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "AWP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}
		
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;
		
		DATARecvInTblTemp = DATARecvInTbl;						// Data Recv Ok
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		// Data Recv Ok
		if (DATARecvInTblTemp != DATARecvOutTbl)
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
				&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
			
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 109)						// 109:CB(ERDS)		 : 2009.04.20 : BHC
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
	if (m_ConfBuff.InterfaceKind == 110)						// 110:SHB(BJ)	: 2009.05.20
	{
		int		TCP_BASE_LEN = 8;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		CheckDataLength = Asc2Int(pTCPRecvBuffer->Data, 8);
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
			memcpy(DATARecvBufferTbl[DATARecvInTbl], TCPRecvBuffer, CheckRecvLength);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength;

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
	if (m_ConfBuff.InterfaceKind == 112)						// 112:CB(TY)	: 2009.07.20
	{
		int		TCP_BASE_LEN = 0;
				
		if (memcmp(&TCPRecvBuffer[0], "ABP", 3) == 0)
			CheckDataLength = 39;
		else
		if (memcmp(&TCPRecvBuffer[0], "AAP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "ASP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "ARP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "AQP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		if (memcmp(&TCPRecvBuffer[0], "AWP", 3) == 0)
			CheckDataLength = TCPRecvLength;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}
		
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
		
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;
		
		DATARecvInTblTemp = DATARecvInTbl;						// Data Recv Ok
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;
		
		// Data Recv Ok
		if (DATARecvInTblTemp != DATARecvOutTbl)
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

		CheckDataLength = Asc2Int(&pTCPRecvBuffer->Data[8], 4);
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
	if (m_ConfBuff.InterfaceKind == 115)						// 115:CB(AS)		 : 2011.01.06
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
	if (m_ConfBuff.InterfaceKind == 116)						// 116:CB(WF)	: 2011.07.25
	{
		int		TCP_BASE_LEN = 12;
		
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length
		
		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		CheckDataLength = MAKEWORD(TCPRecvBuffer[1], TCPRecvBuffer[0]) - TCP_BASE_LEN + 2;
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
	if (m_ConfBuff.InterfaceKind == 1000)						// 1000:CB(JN)	: 2004.10.25
	{
		int		TCP_BASE_LEN = 4;
		char	szTempBuff[256];

		if ((memcmp(&pDATASendBuffer->Data[5], "RQK", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "CWC", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "OEX", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "TTI", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "RWT", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "CWD", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "INQ", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "PIN", 3) == 0)	||
			(memcmp(&pDATASendBuffer->Data[5], "TFW", 3) == 0))
		{
			CheckRecvLength = TCPRecvLength;					// Tran Recv Length
			CheckDataLength = 0;								// Tran Data Length

			if (TCPRecvLength < TCP_BASE_LEN)					// Header Length Check
				return FALSE;									// Recv Waiting

			CheckDataLength = Asc2Int(pTCPRecvBuffer->Data, 4);
			if (CheckDataLength > TCPBUFFSIZE)					// Data Length Adjust
				CheckDataLength = TCPBUFFSIZE;
			CheckRecvLength = CheckDataLength + TCP_BASE_LEN;	// Data Length(Header Length Include)
			if (CheckRecvLength > TCPBUFFSIZE)					// Max Length Adjust
				CheckRecvLength = TCPBUFFSIZE;

			if (TCPRecvLength < CheckRecvLength)				// Max Length Check
				return FALSE;									// Recv Waiting
		}
		else
		{
			CheckRecvLength = TCPRecvLength;					// Tran Recv Length
			CheckDataLength = 0;								// Tran Data Length

			if (TCPRecvLength < TCP_BASE_LEN)					// Header Length Check
				return FALSE;									// Recv Waiting

			memset(szTempBuff, 0, sizeof(szTempBuff));
			MakePack(TCPRecvBuffer, szTempBuff, 4);

			CheckDataLength = MAKEWORD(szTempBuff[1], szTempBuff[0]);
			if (CheckDataLength > TCPBUFFSIZE)					// Data Length Adjust
				CheckDataLength = TCPBUFFSIZE;
			CheckRecvLength = CheckDataLength + TCP_BASE_LEN;	// Data Length(Header Length Include)
			if (CheckRecvLength > TCPBUFFSIZE)					// Max Length Adjust
				CheckRecvLength = TCPBUFFSIZE;

			if (TCPRecvLength < CheckRecvLength)				// Max Length Check
				return FALSE;									// Recv Waiting
		}

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
	if ((m_ConfBuff.InterfaceKind == 1001)	||					// 1001:HXB(XA)		 : 2006.04.21 : DC
		(m_ConfBuff.InterfaceKind == 1008)	||					// 1008:HXB(XA)T-DES : 2008.02.28 : DC
		(m_ConfBuff.InterfaceKind == 3000)	||					// 3000:HXB(WX)		 : 2006.12.07 : YY
		(m_ConfBuff.InterfaceKind == 3005)	||					// 3005:HXB(WX)T-DES : 2008.02.28 : YY
		(m_ConfBuff.InterfaceKind == 6000))						// 6000:HXB(DL)		 : 2007.12.03 : ZL
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
	if (m_ConfBuff.InterfaceKind == 1002)						// 1002:CB(LZ)	: 2006.06.01
	{
		int		TCP_BASE_LEN = 0;
			
		if (memcmp(&TCPRecvBuffer[0], "TP", 2) == 0)
			CheckDataLength = 55;
		else
		if (memcmp(&TCPRecvBuffer[0], "QP", 2) == 0)
			CheckDataLength = 90;
		else
		if (memcmp(&TCPRecvBuffer[0], "WP", 2) == 0)
			CheckDataLength = 90;
		else
		if (memcmp(&TCPRecvBuffer[0], "RP", 2) == 0)
			CheckDataLength = 110;
		else
		if (memcmp(&TCPRecvBuffer[0], "FP", 2) == 0)
			CheckDataLength = 127;
		else
		if (memcmp(&TCPRecvBuffer[0], "SP", 2) == 0)
			CheckDataLength = 70;
		else
		if (memcmp(&TCPRecvBuffer[0], "AP", 2) == 0)
			CheckDataLength = 42;
		else
		if (memcmp(&TCPRecvBuffer[0], "BP", 2) == 0)
			CheckDataLength = 74;
		else
		if (memcmp(&TCPRecvBuffer[0], "HP", 2) == 0)
			CheckDataLength = 43;
		else
		if (memcmp(&TCPRecvBuffer[0], "CP", 2) == 0)
			CheckDataLength = 152;
		else
		if (memcmp(&TCPRecvBuffer[0], "RWT", 3) == 0)
			CheckDataLength = 84;
		else
		if (memcmp(&TCPRecvBuffer[0], "410600", 6) == 0)
			CheckDataLength = 6;
		else
		if (memcmp(&TCPRecvBuffer[0], "4106", 4) == 0)
			CheckDataLength = 24;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}
												
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
												
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;
												
		DATARecvInTblTemp = DATARecvInTbl;						// Data Recv Ok
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
				DATARecvInTblTemp = 0;

		// Data Recv Ok
		if (DATARecvInTblTemp != DATARecvOutTbl)
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
						&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
													
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 1003)	||					// 1003:CB(ZB)	: 2006.06.01
		(m_ConfBuff.InterfaceKind == 3003)	||					// 3003:RCB(KS)	: 2007.03.26
		(m_ConfBuff.InterfaceKind == 3004)	||					// 3004:RCB(NB)	: 2007.11.07
		(m_ConfBuff.InterfaceKind == 3007)	||					// 3007:RCB(XZ) : 2008.11.26
		(m_ConfBuff.InterfaceKind == 3009)	||					// 3009:RCB(KS)TMP: 2011.03.24
		(m_ConfBuff.InterfaceKind == 4003))						// 4003:CB(ZG)	: 2007.06.28
	{
		int		TCP_BASE_LEN = 0;
		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = TCPRecvLength;						// Tran Data Length

		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;
												
		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;
												
		DATARecvInTblTemp = DATARecvInTbl;						// Data Recv Ok
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
				DATARecvInTblTemp = 0;

		// Data Recv Ok
		if (DATARecvInTblTemp != DATARecvOutTbl)
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
						&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN;
													
			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if ((m_ConfBuff.InterfaceKind == 1004)	||					// 1004:CUP(GD)	 : 2006.06.28 : DC
		(m_ConfBuff.InterfaceKind == 2003)	||					// 2003:CB(NT)XML: 2008.03.27
		(m_ConfBuff.InterfaceKind == 2004)	||					// 2004:CB(NTC)XML: 2008.03.27
		(m_ConfBuff.InterfaceKind == 5000))						// 5000:CB(CZ)	 : 2007.04.20 : YKT
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
	if (m_ConfBuff.InterfaceKind == 1005)						// 1005:CB(XA)	: 2006.08.10
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
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 1006)						// 1006:CB(ZH)	: 2006.09.13
	{
		int		TCP_BASE_LEN = 0;									

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 2)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (pTCPRecvBuffer->Data[1] == 'B')						// 'B' Host Response
			CheckDataLength =  26;
		else
		if (pTCPRecvBuffer->Data[1] == 'G')						// 'G' Host Response
			CheckDataLength =  62;
		else
		if (pTCPRecvBuffer->Data[1] == 'J')						// 'J' Host Response
			CheckDataLength =  85;
		else
		if (pTCPRecvBuffer->Data[1] == 'Q')						// 'Q' Host Response
			CheckDataLength =  108;
		else
		if (pTCPRecvBuffer->Data[1] == 'R')						// 'R' Host Response
			CheckDataLength =  130;
		else
		if (pTCPRecvBuffer->Data[1] == 'S')						// 'S' Host Response
			CheckDataLength =  81;
		else
		if (pTCPRecvBuffer->Data[1] == 'T')						// 'T' Host Response
			CheckDataLength =  62;
		else
		if (pTCPRecvBuffer->Data[1] == 'W')						// 'W' Host Response
			CheckDataLength =  108;
		else
		if ((pTCPRecvBuffer->Data[1] == 'D')	||				// 'D' Host Response
			(pTCPRecvBuffer->Data[1] == 'U')	||				// 'U' Host Response
			(pTCPRecvBuffer->Data[1] == 'V'))					// 'V' Host Response
			CheckDataLength =  TCPRecvLength;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	if (m_ConfBuff.InterfaceKind == 1007)						// 1007:CB(SY)	 : 2006.09.13 : DC
	{
		int		TCP_BASE_LEN = 0;									

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 2)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (pTCPRecvBuffer->Data[1] == 'A')						// 'A' Host Response
			CheckDataLength =  19;
		else
		if (pTCPRecvBuffer->Data[1] == 'B')						// 'B' Host Response
			CheckDataLength =  27;
		else
		if (pTCPRecvBuffer->Data[1] == 'G')						// 'G' Host Response
			CheckDataLength =  80;
		else
		if (pTCPRecvBuffer->Data[1] == 'Q')						// 'Q' Host Response
			CheckDataLength =  TCPRecvLength;
		else
		if (pTCPRecvBuffer->Data[1] == 'R')						// 'R' Host Response
			CheckDataLength =  150;
		else
		if (pTCPRecvBuffer->Data[1] == 'S')						// 'S' Host Response
			CheckDataLength =  94;
		else
		if (pTCPRecvBuffer->Data[1] == 'T')						// 'T' Host Response
			CheckDataLength =  80;
		else
		if (pTCPRecvBuffer->Data[1] == 'U')						// 'U' Host Response
			CheckDataLength =  78;
		else
		if (pTCPRecvBuffer->Data[1] == 'V')						// 'V' Host Response
			CheckDataLength =  27;
		else
		if (pTCPRecvBuffer->Data[1] == 'W')						// 'W' Host Response
			CheckDataLength =  130;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	if ((m_ConfBuff.InterfaceKind == 2000)	||					// 2000:CB(NT)	: 2005.11.11
		(m_ConfBuff.InterfaceKind == 2001))						// 2001:CB(NTC)	: 2006.02.06
	{
		int		TCP_BASE_LEN = 0;

		if ((TCPRecvLength == 4)	&&
			(memcmp(TCPRecvBuffer, "0000", 4) == 0))
			CheckDataLength = 4;
		else
		if ((memcmp(&TCPRecvBuffer[1], "CWD", 3) == 0)  ||
		    (memcmp(&TCPRecvBuffer[1], "PIN", 3) == 0)  ||
			(memcmp(&TCPRecvBuffer[1], "CWN", 3) == 0)  ||
			(memcmp(&TCPRecvBuffer[1], "INE", 3) == 0)  ||
			(memcmp(&TCPRecvBuffer[1], "TFR", 3) == 0))
			CheckDataLength = 305;
		else
		if (memcmp(&TCPRecvBuffer[1], "RQK", 3) == 0)
			CheckDataLength = 84;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN;		// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;

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
	if ((m_ConfBuff.InterfaceKind == 113)	||					// 113:CB(HEB)	 : 2009.09.01
		(m_ConfBuff.InterfaceKind == 3001)	||					// 3001:RCB(SZ)	 : 2007.01.05
		(m_ConfBuff.InterfaceKind == 3002)	||					// 3002:RCB(SZC) : 2007.01.05
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
	if (m_ConfBuff.InterfaceKind == 3008)						// 3008:RCB(SZ)   : 2010.07.08(NEW)
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
	if (m_ConfBuff.InterfaceKind == 4000)						//4000:CB(JZ) : 2006.12.10
	{
		int		TCP_BASE_LEN = 0;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (memcmp(TCPRecvBuffer, "RJA", 3) == 0)
			CheckDataLength = 64;
		else
			CheckDataLength = TCPRecvLength;

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

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
	if (m_ConfBuff.InterfaceKind == 4001)						//4001:BOCOM(BJ)	: 2007.01.17
	{
		int		TCP_BASE_LEN = 0;									

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting

		if (TCPRecvLength < 3)									// Header Length Check
			return FALSE;										// Recv Waiting

		if (memcmp(pTCPRecvBuffer->Data, "AAP", 3) == 0)
			CheckDataLength = 18;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ABP", 3) == 0)
			CheckDataLength = 26;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ADC", 3) == 0)
			CheckDataLength = 23;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ADD", 3) == 0)
			CheckDataLength = 203;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ACP", 3) == 0)
			CheckDataLength = 111;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ADP", 3) == 0)
			CheckDataLength = 72;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AEP", 3) == 0)
			CheckDataLength = 31;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AFP", 3) == 0)
			CheckDataLength = 44;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AHP", 3) == 0)
			CheckDataLength = 27;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ALP", 3) == 0)
			CheckDataLength = 31;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AQP", 3) == 0)
			CheckDataLength = 224;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ARP", 3) == 0)
			CheckDataLength = 244;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ASP", 3) == 0)
			CheckDataLength = 205;
		else
		if (memcmp(pTCPRecvBuffer->Data, "ATP", 3) == 0)
			CheckDataLength = 45;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AUP", 3) == 0)
			CheckDataLength = 49;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AVP", 3) == 0)
			CheckDataLength = 26;
		else
		if (memcmp(pTCPRecvBuffer->Data, "AWP", 3) == 0)
			CheckDataLength = 224;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

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
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_ConfBuff.InterfaceKind == 7000)						// 7000:CUP(PBOC)  : 2010.10.20
	{
		int		TCP_BASE_LEN = 3;

		CheckRecvLength = TCPRecvLength;						// Tran Recv Length
		CheckDataLength = 0;									// Tran Data Length

		if (TCPRecvLength < TCP_BASE_LEN)						// Header Length Check
			return FALSE;										// Recv Waiting
		
		if (pTCPRecvBuffer->Data[0] == A_STX)					// 'STX' Host Response
			;
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

		CheckDataLength = MAKEWORD(TCPRecvBuffer[2], TCPRecvBuffer[1]);
		if (CheckDataLength > TCPBUFFSIZE)						// Data Length Adjust
			CheckDataLength = TCPBUFFSIZE;
		CheckRecvLength = CheckDataLength + TCP_BASE_LEN + 2;	// Data Length(Header Length Include)
		if (CheckRecvLength > TCPBUFFSIZE)						// Max Length Adjust
			CheckRecvLength = TCPBUFFSIZE;

		if (TCPRecvLength < CheckRecvLength)					// Max Length Check
			return FALSE;										// Recv Waiting
		
		if ((pTCPRecvBuffer->Data[TCPRecvLength - 2] == A_ETX)	&&
			(pTCPRecvBuffer->Data[TCPRecvLength - 1] == MakeBcc(&pTCPRecvBuffer->Data[1], TCPRecvLength - 2)))
			;													// 'ETX' Host Response
		else
		{
			TCPRecvLength = 0;
			memset(TCPRecvBuffer, 0, sizeof(TCPRecvBuffer));
			return FALSE;
		}

		// Data Recv Ok
		DATARecvInTblTemp = DATARecvInTbl;
		DATARecvInTblTemp++;
		if (DATARecvInTblTemp >= TCPRECVTBL)					// In Tbl Temp Adjust
			DATARecvInTblTemp = 0;

		if (DATARecvInTblTemp != DATARecvOutTbl)				// Data Tbl Ok
		{
			memcpy(DATARecvBufferTbl[DATARecvInTbl], 
					&TCPRecvBuffer[TCP_BASE_LEN], CheckRecvLength - TCP_BASE_LEN - 2);
			DATARecvLengthTbl[DATARecvInTbl] = CheckRecvLength - TCP_BASE_LEN - 2;

			DATARecvInTbl = DATARecvInTblTemp;					// In Tbl Adjust
		}
	}

	return TRUE;												// Clear
}	
	
int CSocketCtrl::CheckTCPError()
{
	return (TCPErrID = WSAGetLastError());
}
