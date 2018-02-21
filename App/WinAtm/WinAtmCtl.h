#if !defined(AFX_WINATMCTL_H__6519D0A0_3B2D_45E7_B955_D90F3E982ECB__INCLUDED_)
#define AFX_WINATMCTL_H__6519D0A0_3B2D_45E7_B955_D90F3E982ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxctl.h>
#include "Class.h"
#include "..\..\H\Dll\nhmwi.h"						

// WinAtmCtl.h : Declaration of the CWinAtmCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl : See WinAtmCtl.cpp for implementation.

class CWinAtmCtrl : public COleControl
{
	DECLARE_DYNCREATE(CWinAtmCtrl)

// Constructor
public:
	CWinAtmCtrl();

/////////////////////////////////////////////////////////////////////////////
// Attributes
/////////////////////////////////////////////////////////////////////////////
public:
#include "ClassPointer.h"

	CNHMWI	Mwi;												// Mwi Object
	
/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////
public:
	void	SetOwner(CWinAtmCtrl *pOwner);						// Set Owner

	int		AtmModeCtrl();										// Atm Mode Control		
	int		TerminateProcess();									// Terminate Process
	int		WindowsShutdownProcess(int nRebootFlag = FALSE);	// Windows Shutdown Process

	int		AtmHostCtrl();										// Atm Host Control

	int		ClerkProc();										// Clerk Procedure

/////////////////////////////////////////////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinAtmCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual void OnSetClientSite();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CWinAtmCtrl();

	DECLARE_OLECREATE_EX(CWinAtmCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CWinAtmCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CWinAtmCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CWinAtmCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CWinAtmCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//-------------------------------------------------------------------------
	afx_msg void OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue);	// AXINAX
	afx_msg void OnDisplayPinpadData(LPCTSTR szDataValue);					// AXINAX 2004.04.07

	DECLARE_EVENTSINK_MAP()										// AXINAX
	//-------------------------------------------------------------------------

// Dispatch maps
	//{{AFX_DISPATCH(CWinAtmCtrl)
	afx_msg long AtmStart();
	afx_msg long TraceDump(LPCTSTR szTraceData);
	afx_msg long SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CWinAtmCtrl)
	void FireOnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
		{FireEvent(eventidOnDisplayData,EVENT_PARAM(VTS_BSTR  VTS_BSTR), szDataName, szDataValue);}
	void FireOnDisplayPinpadData(LPCTSTR szDataValue)
		{FireEvent(eventidOnDisplayPinpadData,EVENT_PARAM(VTS_BSTR), szDataValue);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CWinAtmCtrl)
	dispidAtmStart = 1L,
	dispidTraceDump = 2L,
	dispidSetScreenData = 3L,
	eventidOnDisplayData = 1L,
	eventidOnDisplayPinpadData = 2L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINATMCTL_H__6519D0A0_3B2D_45E7_B955_D90F3E982ECB__INCLUDED)
