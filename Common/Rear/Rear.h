// Rear.h : main header file for the REAR application
//

#if !defined(AFX_REAR_H__E66B9354_1AD6_4BDB_8623_ACC430A239A3__INCLUDED_)
#define AFX_REAR_H__E66B9354_1AD6_4BDB_8623_ACC430A239A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRearApp:
// See Rear.cpp for the implementation of this class
//

class CRearApp : public CWinApp
{
public:
	CRearApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRearApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRearApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REAR_H__E66B9354_1AD6_4BDB_8623_ACC430A239A3__INCLUDED_)
