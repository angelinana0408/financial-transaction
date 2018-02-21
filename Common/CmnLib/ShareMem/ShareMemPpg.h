#if !defined(AFX_SHAREMEMPPG_H__C199A086_F12C_4953_AA92_89A5E3B333E8__INCLUDED_)
#define AFX_SHAREMEMPPG_H__C199A086_F12C_4953_AA92_89A5E3B333E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShareMemPpg.h : Declaration of the CShareMemPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CShareMemPropPage : See ShareMemPpg.cpp.cpp for implementation.

class CShareMemPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CShareMemPropPage)
	DECLARE_OLECREATE_EX(CShareMemPropPage)

// Constructor
public:
	CShareMemPropPage();

// Dialog Data
	//{{AFX_DATA(CShareMemPropPage)
	enum { IDD = IDD_PROPPAGE_SHAREMEM };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CShareMemPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAREMEMPPG_H__C199A086_F12C_4953_AA92_89A5E3B333E8__INCLUDED)
