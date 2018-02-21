#if !defined(AFX_SOCKETCTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_SOCKETCTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketCtrl.h : header file
//

#include "Profiledef.h"

#define TCPRECVTBL				8
#define TCPBUFFSIZE				16384

typedef struct tagTCPHeader
{
	BYTE	Data[1];
} TCPHeader;

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl command target

class AFX_EXT_CLASS CSocketCtrl : public CAsyncSocket
{
// Attributes
public:
	int		TCPFirstConnected;
	int		TCPConnected;
	int		TCPErrID;

	int		TCPRecvLength;						// Tcp Recv Buffer
	BYTE	TCPRecvBuffer[TCPBUFFSIZE];			
	
	int		DATASendLength;						// Tran Buffer
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

	int		CheckRecvLength;					// Tran Recv Length
	int		CheckDataLength;					// Tran Data Length

// Operations
public:
	CSocketCtrl(NETWORKINFO NETWORK);
	virtual ~CSocketCtrl();

	int InitSocket();
	int OpenSocket();
	int OpenSocketIP(LPCTSTR szIpAddress, int nPortNumber);
	int CloseSocket();
	int SendSocket(BYTE *SendBuff, int Length);
	int DATARecvSocket(BYTE *RecvBuff, int *Length);

private:
	NETWORKINFO	m_ConfBuff;
	
	int ProcessData();
	int CheckData();
	int CheckTCPError();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketCtrl)
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
	//{{AFX_MSG(CSocketCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCTRL_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
