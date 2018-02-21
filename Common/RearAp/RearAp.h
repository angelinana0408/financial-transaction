#if !defined(AFX_REARAP_H__0D02E2CD_C943_4A08_B799_1B9BD4F2001F__INCLUDED_)
#define AFX_REARAP_H__0D02E2CD_C943_4A08_B799_1B9BD4F2001F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RearAp.h : main header file for REARAP.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CRearApApp : See RearAp.cpp for implementation.

class CRearApApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REARAP_H__0D02E2CD_C943_4A08_B799_1B9BD4F2001F__INCLUDED)
