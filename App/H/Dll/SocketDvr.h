#if !defined(AFX_SOCKETDVR_H__3DA4B49B_D6EC_4DAB_B238_01FB5D24C3BA__INCLUDED_)
#define AFX_SOCKETDVR_H__3DA4B49B_D6EC_4DAB_B238_01FB5D24C3BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketDvr.h : header file
//

#include "Profiledef.h"

#define TCPRECVTBL				8
#define TCPBUFFSIZE				16384

typedef struct tagETCHeader
{
	BYTE	Data[1];
} ETCHeader;

/////////////////////////////////////////////////////////////////////////////
// CSocketDvr command target

class AFX_EXT_CLASS CSocketDvr : public CAsyncSocket
{
// Attributes
public:
	int		TCPFirstConnected;
	int		TCPConnected;
	int		TCPErrID;

	int		TCPRecvLength;						// Tcp Recv Buffer
	BYTE	TCPRecvBuffer[TCPBUFFSIZE];			
	
	int		DATASendLength;						// Nms Buffer
	int		DATARecvLength;
	int		DATARecvLengthTbl[TCPRECVTBL];
	int		DATARecvInTbl;
	int		DATARecvInTblTemp;
	int		DATARecvOutTbl;
	int		DATARecvOutTblTemp;
	BYTE	DATASendBuffer[TCPBUFFSIZE];
	BYTE	DATARecvBufferTbl[TCPRECVTBL][TCPBUFFSIZE];

private:
	ETCHeader *pTCPRecvBuffer;

	ETCHeader *pDATASendBuffer;

	int		CheckRecvLength;					// Nms Recv Length
	int		CheckDataLength;					// Nms Data Length

// Operations
public:
	CSocketDvr(LPCTSTR szIpAddress, int nPortNumber);
	virtual ~CSocketDvr();

	int		InitSocket();
	int		OpenSocket();
	int		CloseSocket();
	int		SendSocket(BYTE *SendBuff, int Length);
	int		DATARecvSocket(BYTE *RecvBuff, int *Length);

private:
	char	m_IpAddress[256];
	int		m_PortNumber;

	int		ProcessData();
	int		CheckData();
	int		CheckTCPError();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketDvr)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocketDvr)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETDVR_H__3DA4B49B_D6EC_4DAB_B238_01FB5D24C3BA__INCLUDED_)
