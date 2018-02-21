#if !defined(AFX_SHAREMEMCTL_H__E3508729_D5A5_4E0B_BD20_2CACB53811FD__INCLUDED_)
#define AFX_SHAREMEMCTL_H__E3508729_D5A5_4E0B_BD20_2CACB53811FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShareMemCtl.h : Declaration of the CShareMemCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl : See ShareMemCtl.cpp for implementation.

class CShareMemCtrl : public COleControl
{
	DECLARE_DYNCREATE(CShareMemCtrl)

// Constructor
public:
	CShareMemCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShareMemCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CShareMemCtrl();

	DECLARE_OLECREATE_EX(CShareMemCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CShareMemCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CShareMemCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CShareMemCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CShareMemCtrl)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CShareMemCtrl)
	afx_msg long OpenShareData();
	afx_msg long CloseShareData();
	afx_msg long ClearShareData();
	afx_msg BSTR GetShareData(LPCTSTR szGetDataName);
	afx_msg long SetShareData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CShareMemCtrl)
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CShareMemCtrl)
	dispidOpenShareData = 1L,
	dispidCloseShareData = 2L,
	dispidClearShareData = 3L,
	dispidGetShareData = 4L,
	dispidSetShareData = 5L,
	//}}AFX_DISP_ID
	};
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHAREMEMCTL_H__E3508729_D5A5_4E0B_BD20_2CACB53811FD__INCLUDED)
