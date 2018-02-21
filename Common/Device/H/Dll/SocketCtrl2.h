#if !defined(AFX_SOCKETCTRL2_H__59FA2E51_0FD2_44C1_A5A7_E608E9CF3D09__INCLUDED_)
#define AFX_SOCKETCTRL2_H__59FA2E51_0FD2_44C1_A5A7_E608E9CF3D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketCtrl2.h : header file
//

#include "Profiledef.h"

#define TCPBUFFSIZE				16384

#define TCP_BASE_LEN2			32				// ATMP -> ATM

typedef struct tagTCPHeader2
{
	DWORD	Length;
	BYTE	Branch[8];
	BYTE	AtmId[8];
	BYTE	TxnCode[4];
	BYTE	RtnCode[4];
	BYTE	Filler[3];
	BYTE	Mac;
} TCPHeader2;

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl2 command target

class AFX_EXT_CLASS CSocketCtrl2 : public CAsyncSocket
{
// Attributes
public:
	int		TCPFirstConnected;
	int		TCPConnected;
	int		TCPErrID;

	int		TCPRecvLength;						// Tcp Recv Buffer
	BYTE	TCPRecvBuffer[TCPBUFFSIZE];			
	
	int		DATASendLength;						// Tran Buffer
	BYTE	DATASendBuffer[TCPBUFFSIZE];

private:
	TCPHeader2 *pTCPRecvBuffer;

	TCPHeader2 *pDATASendBuffer;

	int		CheckRecvLength;					// Tran Recv Length
	int		CheckDataLength;					// Tran Data Length

	char	m_Branch[256];
	char	m_AtmId[256];

// Operations
public:
	CSocketCtrl2(NETWORKINFO NETWORK);
	virtual ~CSocketCtrl2();

	int InitSocket();
	int OpenSocket();
	int CloseSocket();
	int SendSocket(BYTE *SendBuff, int Length);

private:
	NETWORKINFO	m_ConfBuff;
	
	int ProcessData();
	int CheckData();
	int CheckTCPError();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketCtrl2)
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
	//{{AFX_MSG(CSocketCtrl2)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCTRL2_H__6E66A161_3EFF_11D5_A80C_00E09880ACB0__INCLUDED_)
