/////////////////////////////////////////////////////////////////////////////
// NetWork.h: interface for the CNetWork class.
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_NETWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
#define AFX_NETWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LcpRam.h"
#include "Profiledef.h"

class CSna;
class CTcpip;

class AFX_EXT_CLASS CNetWork  
{
public:
	CNetWork(NETWORKINFO NETWORK);
	virtual ~CNetWork();

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

private:
	NETWORKINFO	m_ConfBuff;
	
	CSna*		m_pSna;
	CTcpip*		m_pTcpip;
};

#endif // !defined(AFX_NETWORK_H__2B18FA40_04D5_11D5_A80E_00E09880ACB0__INCLUDED_)
