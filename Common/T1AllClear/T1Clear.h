// T1Clear.h : main header file for the T1CLEAR application
//

#if !defined(AFX_T1CLEAR_H__096C0626_BC6E_4C40_BD3D_A2E423372CC9__INCLUDED_)
#define AFX_T1CLEAR_H__096C0626_BC6E_4C40_BD3D_A2E423372CC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CT1ClearApp:
// See T1Clear.cpp for the implementation of this class
//

class CT1ClearApp : public CWinApp
{
public:
	CT1ClearApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CT1ClearApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CT1ClearApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_T1CLEAR_H__096C0626_BC6E_4C40_BD3D_A2E423372CC9__INCLUDED_)
