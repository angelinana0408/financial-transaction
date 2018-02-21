#if !defined(AFX_SOCKETCTRL3_H__3DCC09B6_8832_4031_A840_C21F9139EB29__INCLUDED_)
#define AFX_SOCKETCTRL3_H__3DCC09B6_8832_4031_A840_C21F9139EB29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SocketCtrl3.h : header file
//
#include "..\..\H\Dll\Tcpip.h"

class CTcpip;

/////////////////////////////////////////////////////////////////////////////
// CSocketCtrl3 command target

class AFX_EXT_CLASS CSocketCtrl3 : public CAsyncSocket
{
// Attributes
public:

private:
	CTcpip*	m_pTcpip;
	
// Operations
public:
	CSocketCtrl3(CTcpip *m_Tcp);
	virtual ~CSocketCtrl3();

	int	CloseSocket();

private:

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocket3)
	virtual void OnAccept(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CSocket3)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKETCTRL3_H__3DCC09B6_8832_4031_A840_C21F9139EB29__INCLUDED_)
