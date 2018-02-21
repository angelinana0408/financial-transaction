// T1ClearDlg.h : header file
//

#if !defined(AFX_T1CLEARDLG_H__041502CD_4F5F_4FDC_AE7C_9D1C3817E63A__INCLUDED_)
#define AFX_T1CLEARDLG_H__041502CD_4F5F_4FDC_AE7C_9D1C3817E63A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CT1ClearDlg dialog
#include "TeoStatic.h"

class CT1ClearDlg : public CDialog
{
// Construction
public:
	CT1ClearDlg(CWnd* pParent = NULL);	// standard constructor

// Mehtods
	void	SetTitle();											// SetTitle
	int		KillProcess(LPCTSTR szAtmName);						// Kill Processor, has that name
	void	AtmClear();											// Clear...

// Dialog Data
	//{{AFX_DATA(CT1ClearDlg)
	enum { IDD = IDD_T1CLEAR_DIALOG };
	CTeoStatic	m_stTitle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CT1ClearDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CT1ClearDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_T1CLEARDLG_H__041502CD_4F5F_4FDC_AE7C_9D1C3817E63A__INCLUDED_)
