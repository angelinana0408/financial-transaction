#if !defined(AFX_SHAREMEM_H__1EC95239_A4A9_4E54_847E_F0B3D10CF8B0__INCLUDED_)
#define AFX_SHAREMEM_H__1EC95239_A4A9_4E54_847E_F0B3D10CF8B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShareMem.h : main header file for SHAREMEM.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CShareMemApp : See ShareMem.cpp for implementation.

class CShareMemApp : public COleControlModule
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

#endif // !defined(AFX_SHAREMEM_H__1EC95239_A4A9_4E54_847E_F0B3D10CF8B0__INCLUDED)
