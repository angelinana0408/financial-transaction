// SocketCtrl3.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\SocketCtrl3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl3

CSocketCtrl3::CSocketCtrl3(CTcpip *m_Tcp)
{
	m_pTcpip = m_Tcp;
}

CSocketCtrl3::~CSocketCtrl3()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CSocketCtrl3, CAsyncSocket)
	//{{AFX_MSG_MAP(CSocketCtrl3)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl3 event functions

void CSocketCtrl3::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (nErrorCode)
		MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "[TCP3 OnAccept ERROR] [%d]", nErrorCode);

	m_pTcpip->AcceptRequest3();
	
	CAsyncSocket::OnAccept(nErrorCode);
}
/////////////////////////////////////////////////////////////////////////////
int CSocketCtrl3::CloseSocket()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TCP3/IP] CloseSocket");
	
	ShutDown(CAsyncSocket::both);
	Close();
	
	return TRUE;
}
