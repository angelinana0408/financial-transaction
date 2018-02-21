/////////////////////////////////////////////////////////////////////////////
// Tcpip.h: interface for the CTcpip class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_TCPIP_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_)
#define AFX_TCPIP_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

#include "SocketCtrl.h"
#include "SocketCtrl2.h"
#include "SocketCtrl3.h"
#include "SocketCtrlB.h"

class CSocketCtrl3;

class AFX_EXT_CLASS CTcpip
{
public:
	CTcpip(NETWORKINFO NETWORK);
	virtual ~CTcpip();

	int 		LineOpen(int OpenSec, int nAsyncFlag = FALSE);
	int 		LineOpenIP(LPCTSTR szIpAddress, int nPortNumber, int OpenSec, int nAsyncFlag = FALSE);
	int 		LineClose(int CloseSec, int nAsyncFlag = FALSE);
	int 		SendData(BYTE *SendBuff, int Length, int SendSec);
	int 		RecvData(BYTE *RecvBuff, int *Length, int RecvSec);
	int 		GetStatus();

	int 		LineOpen2(int OpenSec, int nAsyncFlag = FALSE);
	int 		LineClose2(int CloseSec, int nAsyncFlag = FALSE);
	int 		SendData2(BYTE *SendBuff, int Length, int SendSec);
	int 		GetStatus2();

	int 		LineOpen3(int OpenSec, int nAsyncFlag = FALSE);
	int 		LineClose3(int CloseSec, int nAsyncFlag = FALSE);
	int 		SendData3(BYTE *SendBuff, int Length, int SendSec);
	int 		RecvData3(BYTE *RecvBuff, int *Length, int RecvSec);
	int 		GetStatus3();
	int			AcceptRequest3();

private:
	NETWORKINFO	m_ConfBuff;
	
	CSocketCtrl*	m_pSocketCtrl;
	CSocketCtrl2*	m_pSocketCtrl2;
	CSocketCtrl3*	m_pListenSocket;
	CSocketCtrlB*	m_pSocketCtrlB;

	int			m_InitOpen;
	int			m_InitOpen2;
	int			m_InitOpen3;
	
	int			m_LineStatus;
	int			m_LineStatus2;
	int			m_LineStatus3;
};

#endif // !defined(AFX_TCPIP_H__7A3C6C85_3D61_11D5_A80C_00E09880ACB0__INCLUDED_)
