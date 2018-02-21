// Loader.h : main header file for the LOADER application
//

#if !defined(AFX_LOADER_H__90D3E10A_ACB1_4229_A5BD_51A12EB70832__INCLUDED_)
#define AFX_LOADER_H__90D3E10A_ACB1_4229_A5BD_51A12EB70832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLoaderApp:
// See Loader.cpp for the implementation of this class
//

class CLoaderApp : public CWinApp
{
public:
	CLoaderApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaderApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL


// Implementation

	//{{AFX_MSG(CLoaderApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADER_H__90D3E10A_ACB1_4229_A5BD_51A12EB70832__INCLUDED_)
