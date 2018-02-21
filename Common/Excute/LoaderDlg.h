// LoaderDlg.h : header file
//

#if !defined(AFX_LOADERDLG_H__CE5EE651_561C_45CA_B4E2_B453AEDA4AA9__INCLUDED_)
#define AFX_LOADERDLG_H__CE5EE651_561C_45CA_B4E2_B453AEDA4AA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\AddIns\HtmlCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg dialog

/////////////////////////////////////////////////////////////////////////////
#define	_REGKEY_DEVICE					_T("SOFTWARE\\ATM\\APP\\DEVICE")
#define	_REGKEY_DEVICE_LOADINGSTEP		_T("LOADINGSTEP")
																// 2005.03.09
/////////////////////////////////////////////////////////////////////////////

class CLoaderDlg : public CDialog
{
// Construction
public:
	CLoaderDlg(CWnd* pParent = NULL);							// standard constructor

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

	void		OnStartTimer();
	void		OnStopTimer();

// Dialog Data
	//{{AFX_DATA(CLoaderDlg)
	enum { IDD = IDD_LOADER_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoaderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);			// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLoaderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADERDLG_H__CE5EE651_561C_45CA_B4E2_B453AEDA4AA9__INCLUDED_)
