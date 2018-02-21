/////////////////////////////////////////////////////////////////////////////
// NetWork.cpp: implementation of the CNetWork class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\Sna.h"
#include "..\..\H\Dll\Tcpip.h"
#include "..\..\H\Dll\NetWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CNetWork::CNetWork(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;

	m_pTcpip		= NULL;

	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			m_pSna = new CSna(NETWORK);
			break;
		case NET_TCPIP:
			WSADATA	WSAData;									/* Windows sockets info return      */
			WSAStartup(MAKEWORD(1,1), &WSAData);				// WSA Startup
			m_pTcpip = new CTcpip(NETWORK);
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
}

CNetWork::~CNetWork()
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			if (m_pSna)		{	delete m_pSna;		m_pSna		= NULL;	}
			break;
		case NET_TCPIP:
			if (m_pTcpip)	{	delete m_pTcpip;	m_pTcpip	= NULL;	}
			WSACleanup();										// WSA Cleanup
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	ATMP
/////////////////////////////////////////////////////////////////////////////
int CNetWork::LineOpen(int OpenSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:	
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:	
			return (m_pSna->LineOpen(OpenSec, nAsyncFlag));
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineOpen(OpenSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::LineOpenIP(LPCTSTR szIpAddress, int nPortNumber, int OpenSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:	
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:	
			return (m_pSna->LineOpen(OpenSec, nAsyncFlag));
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineOpenIP(szIpAddress, nPortNumber, OpenSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::LineClose(int CloseSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			return (m_pSna->LineClose(CloseSec, nAsyncFlag));
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineClose(CloseSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			return (m_pSna->SendData(SendBuff, Length, SendSec));
			break;
		case NET_TCPIP:
			return (m_pTcpip->SendData(SendBuff, Length, SendSec));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}
	
int CNetWork::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			return (m_pSna->RecvData(RecvBuff, Length, RecvSec));
			break;
		case NET_TCPIP:
			return (m_pTcpip->RecvData(RecvBuff, Length, RecvSec));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::GetStatus()
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			return (m_pSna->GetStatus());
			break;
		case NET_TCPIP:
			return (m_pTcpip->GetStatus());
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	EJR
/////////////////////////////////////////////////////////////////////////////
int CNetWork::LineOpen2(int OpenSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:	
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:	
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineOpen2(OpenSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::LineClose2(int CloseSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineClose2(CloseSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::SendData2(BYTE *SendBuff, int Length, int SendSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->SendData2(SendBuff, Length, SendSec));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}
	
int CNetWork::GetStatus2()
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->GetStatus2());
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//	ATMP(BID)
/////////////////////////////////////////////////////////////////////////////
int CNetWork::LineOpen3(int OpenSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:	
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:	
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineOpen3(OpenSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::LineClose3(int CloseSec, int nAsyncFlag)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->LineClose3(CloseSec, nAsyncFlag));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::SendData3(BYTE *SendBuff, int Length, int SendSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->SendData3(SendBuff, Length, SendSec));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}
	
int CNetWork::RecvData3(BYTE *RecvBuff, int *Length, int RecvSec)
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->RecvData3(RecvBuff, Length, RecvSec));
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

int CNetWork::GetStatus3()
{
	switch (m_ConfBuff.Interface)
	{
		case NET_IOC:
			break;
		case NET_HDLC:
			break;
		case NET_SWP:
			break;
		case NET_SDLC:
			break;
		case NET_TCPIP:
			return (m_pTcpip->GetStatus3());
			break;
		case NET_CCL:
			break;
		case NET_BAS:
			break;
		default:
			break;
	}
	return FALSE;
}

