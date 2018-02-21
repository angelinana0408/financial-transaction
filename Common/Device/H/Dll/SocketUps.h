#if !defined(AFX_SOCKETUPS_H__FAEFED10_767A_4F9F_8C2B_8E47B53E1663__INCLUDED_)
#define AFX_SOCKETUPS_H__FAEFED10_767A_4F9F_8C2B_8E47B53E1663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketUps.h : header file
//

#include "Profiledef.h"

#define TCPRECVTBL				8
#define TCPBUFFSIZE				8192

typedef struct tagTCPHeader
{
	BYTE	Data[1];
} TCPHeader;

/////////////////////////////////////////////////////////////////////////////
// CSocketUps command target

class AFX_EXT_CLASS CSocketUps : public CAsyncSocket
{
// Attributes
public:
	int		TCPFirstConnected;
	int		TCPConnected;
	int		TCPErrID;

	int		TCPRecvLength;						// Tcp Recv Buffer
	BYTE	TCPRecvBuffer[TCPBUFFSIZE];			
	
	int		DATASendLength;						// Ups Buffer
	int		DATARecvLength;
	int		DATARecvLengthTbl[TCPRECVTBL];
	int		DATARecvInTbl;
	int		DATARecvInTblTemp;
	int		DATARecvOutTbl;
	int		DATARecvOutTblTemp;
	BYTE	DATASendBuffer[TCPBUFFSIZE];
	BYTE	DATARecvBufferTbl[TCPRECVTBL][TCPBUFFSIZE];

private:
	TCPHeader *pTCPRecvBuffer;

	TCPHeader *pDATASendBuffer;

	int		CheckRecvLength;					// Ups Recv Length
	int		CheckDataLength;					// Ups Data Length

// Operations
public:
	CSocketUps(LPCTSTR szIpAddress, int nPortNumber);
	virtual ~CSocketUps();

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
	//{{AFX_VIRTUAL(CSocketUps)
	public:
	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnOutOfBandData(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocketUps)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETUPS_H__FAEFED10_767A_4F9F_8C2B_8E47B53E1663__INCLUDED_)
