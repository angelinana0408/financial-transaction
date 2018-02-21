/////////////////////////////////////////////////////////////////////////////
// Sna.cpp: implementation of the CSna class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Sna.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CSna::CSna(NETWORKINFO NETWORK)
{
	m_ConfBuff		= NETWORK;
	
	m_InitOpen		= FALSE;
	m_LineStatus	= LINE_CLOSE;

	m_pSnaCtrl		= new CSnaCtrl(NETWORK);
}

CSna::~CSna()
{
	if (m_pSnaCtrl)	{	delete m_pSnaCtrl;	m_pSnaCtrl	= NULL;	}
}

/////////////////////////////////////////////////////////////////////////////
int CSna::LineOpen(int OpenSec, int nAsyncFlag)
{
	int		Ret;

	LineClose(OpenSec, nAsyncFlag);								// 2004.07.12
	m_InitOpen = TRUE;	

	Ret = m_pSnaCtrl->OpenSna();								// LINE OPEN
	if (Ret)													// OPEN OK
		return TRUE;
	else 
	{															// OPEN ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNA] Open Fail");
		m_pSnaCtrl->SNAConnected = FALSE;
		return FALSE;
	}
}

int CSna::LineClose(int CloseSec, int nAsyncFlag)
{
	if (m_InitOpen)												// LINE CLOSE : 2007.06.06
	{
		m_InitOpen = FALSE;
		return (m_pSnaCtrl->CloseSna());
	}
	return TRUE;
}

int CSna::SendData(BYTE *SendBuff, int Length, int SendSec)
{
	int		Ret;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.19
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSnaCtrl->SNAConnected)								// 2004.07.12
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNA] Send Connect Fail");
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	Ret = m_pSnaCtrl->SendSna(SendBuff, Length, SendSec);		// SEND DATA
	if (Ret)													// SEND OK
		return TRUE;
	else 
	{															// SEND ERROR
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNA] Send Fail");
		m_pSnaCtrl->SNAConnected = FALSE;
		return FALSE;
	}
}

int CSna::RecvData(BYTE *RecvBuff, int *Length, int RecvSec)
{
	int		Ret;

/////////////////////////////////////////////////////////////////////////////
	Delay_Msg(500);												// Performance Adjust 2004.07.19
/////////////////////////////////////////////////////////////////////////////
	if (!m_pSnaCtrl->SNAConnected)								// 2004.07.12
	{
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNA] Recv Connect Fail");
		}
		*Length = 0;
		return FALSE;
	}
/////////////////////////////////////////////////////////////////////////////

	for (int i = 0; i < 5; i++)									// 2004.07.20
	{
		Ret = m_pSnaCtrl->RecvSna(RecvBuff, Length, RecvSec);	// RECV DATA
		if (Ret)
		{
			if (memcmp(RecvBuff, "A", 1) == 0)					// Garbage : 2004.07.20
				break;
		}
		else
		{
			break;
		}
	}

	if (Ret)													// RECV OK
		return TRUE;
	else 
	{															// RECV ERROR
		if (RecvSec)											// RECV WAIT TIME EXIST
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[SNA] Recv Fail");
			m_pSnaCtrl->SNAConnected = FALSE;
		}
		*Length = 0;
		return FALSE;
	}
}

int CSna::GetStatus()
{
	m_pSnaCtrl->GetStatusSna();									// GET STATUS
	
	if (m_pSnaCtrl->SNAConnected)								// Connected Type
		m_LineStatus = LINE_OPEN;
	else m_LineStatus = LINE_CLOSE;

	return m_LineStatus;
}

