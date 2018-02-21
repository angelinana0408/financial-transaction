// RearDlg.h : header file
//

#if !defined(AFX_REARDLG_H__3A72DF49_6D48_4913_9013_85C3A84483D0__INCLUDED_)
#define AFX_REARDLG_H__3A72DF49_6D48_4913_9013_85C3A84483D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\AddIns\HtmlCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CRearDlg dialog

class CRearDlg : public CDialog
{
// Construction
public:
	CRearDlg(CWnd* pParent = NULL);								// standard constructor

	// ------------------------------------------------------------------------
	// VARIABLES 
	// ------------------------------------------------------------------------
	CHtmlCtrl	m_HtmlView;
	CRect		m_rOplRect, m_rSplRect;
	int			m_Timer;

public:
	// ------------------------------------------------------------------------
	// FUNCTIONS
	// ------------------------------------------------------------------------
	void		OnResizeWindow();
	void		InitializeDisplayDevice();

	void		OnStartTimer();
	void		OnStopTimer();

// Dialog Data
	//{{AFX_DATA(CRearDlg)
	enum { IDD = IDD_REAR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRearDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);			// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CRearDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REARDLG_H__3A72DF49_6D48_4913_9013_85C3A84483D0__INCLUDED_)
