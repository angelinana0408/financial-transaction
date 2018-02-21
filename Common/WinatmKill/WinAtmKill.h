// WinAtmKill.h : main header file for the WINATMKILL application
//

#if !defined(AFX_WINATMKILL_H__3D23ECAF_C362_431A_8FD1_F7BD56E5DA81__INCLUDED_)
#define AFX_WINATMKILL_H__3D23ECAF_C362_431A_8FD1_F7BD56E5DA81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillApp:
// See WinAtmKill.cpp for the implementation of this class
//

class CWinAtmKillApp : public CWinApp
{
public:
	CWinAtmKillApp();
	void CWinAtmKillApp::NTShutdownProcess() ;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinAtmKillApp)
	public:
	virtual BOOL InitInstance();
	virtual int Run();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinAtmKillApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINATMKILL_H__3D23ECAF_C362_431A_8FD1_F7BD56E5DA81__INCLUDED_)
