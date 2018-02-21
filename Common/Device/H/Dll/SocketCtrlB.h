#if !defined(AFX_SOCKETCTRLB_H__9E3D3638_44D2_48D8_BD4B_925DD7ABEE25__INCLUDED_)
#define AFX_SOCKETCTRLB_H__9E3D3638_44D2_48D8_BD4B_925DD7ABEE25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketCtrlB.h : header file
//

#include "Profiledef.h"

#define TCPRECVTBL				8
#define TCPBUFFSIZE				16384

typedef struct tagTCPHeaderB
{
	BYTE	Data[1];
} TCPHeaderB;

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrlB command target

class AFX_EXT_CLASS CSocketCtrlB : public CAsyncSocket
{
// Attributes
public:
	int		TCPErrID;
	CString	CurrHostName;
	
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
	TCPHeaderB *pTCPRecvBuffer;
	
	int		CheckRecvLength;					// Tran Recv Length
	int		CheckDataLength;					// Tran Data Length

// Operations
public:
	CSocketCtrlB(NETWORKINFO NETWORK);
	virtual ~CSocketCtrlB();

	int InitSocket();
	int	CloseSocket();
	int SendSocket(BYTE *SendBuff, int Length);
	int	DATARecvSocket(BYTE *RecvBuff, int *Length);

private:
	NETWORKINFO	m_ConfBuff;
	
	int	ProcessData();
	int	CheckData();
	int	CheckTCPError();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocketCtrlB)
	public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual int Receive(void* lpBuf, int nBufLen, int nFlags = 0);
	virtual int Send(const void* lpBuf, int nBufLen, int nFlags = 0);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocketCtrlB)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCTRLB_H__9E3D3638_44D2_48D8_BD4B_925DD7ABEE25__INCLUDED_)
