#if !defined(AFX_REARAPPPG_H__0565024D_EBD6_4ED6_AE87_FFAA700AEC0B__INCLUDED_)
#define AFX_REARAPPPG_H__0565024D_EBD6_4ED6_AE87_FFAA700AEC0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// RearApPpg.h : Declaration of the CRearApPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CRearApPropPage : See RearApPpg.cpp.cpp for implementation.

class CRearApPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CRearApPropPage)
	DECLARE_OLECREATE_EX(CRearApPropPage)

// Constructor
public:
	CRearApPropPage();

// Dialog Data
	//{{AFX_DATA(CRearApPropPage)
	enum { IDD = IDD_PROPPAGE_REARAP };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CRearApPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REARAPPPG_H__0565024D_EBD6_4ED6_AE87_FFAA700AEC0B__INCLUDED)
