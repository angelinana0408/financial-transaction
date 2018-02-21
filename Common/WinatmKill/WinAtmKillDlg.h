// WinAtmKillDlg.h : header file
//

#if !defined(AFX_WINATMKILLDLG_H__895027BD_8751_4893_814D_A4BFD825FEDB__INCLUDED_)
#define AFX_WINATMKILLDLG_H__895027BD_8751_4893_814D_A4BFD825FEDB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define		_REGFILE					_T("Update.reg")
#define		_UPDATEFILE					_T("Update.bat")
#define		_RUNFILE					_T("Run.bat")

#define		_DEF_EXERELEPATH			_T("C:\\T1ATM\\APP\\EXERELE")
#define		_DEF_UPDATEPATH				_T("C:\\UPDATE")

#define		STRBUFFSIZE					32768
#define		REG_BIN						REG_BINARY
#define		REG_INT						REG_DWORD_LITTLE_ENDIAN
#define		REG_STR						REG_SZ
#define		REG_MULTI					REG_MULTI_SZ

#define		_REGKEY_DEVICE				_T("SOFTWARE\\ATM\\APP\\DEVICE")
#define		_REGKEY_DEVICE_APINSTALL	_T("APINSTALL")

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillDlg dialog

class CWinAtmKillDlg : public CDialog
{
// Construction
public:
	CWinAtmKillDlg(CWnd* pParent = NULL);						// standard constructor

/////////////////////////////////////////////////////////////////////////////
	int		AtmUpdate();	
	int		WinExecWaitEnd(LPCTSTR lpCmdLind, UINT uCmdShow);
	int		DeleteFiles(LPCTSTR DirName, int DeleteDays = 31);

/////////////////////////////////////////////////////////////////////////////
	int		RegQueryValueExt(	LPCTSTR hKeyName, 
								LPCTSTR lpszValueName, 
								DWORD dwType,
					 			DWORD dwLen, 
								LPVOID lpszData);

	int		RegSetValueExt(		LPCTSTR hKeyName, 
								LPCTSTR lpszValueName, 
								DWORD dwType, 
								DWORD dwLen, 
								LPVOID lpszData);

	int		RegGetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int InitValue = NULL);
	CString	RegGetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, LPCTSTR InitValue = NULL);

	int		RegSetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int SetValue);
	int		RegSetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, CString SetValue);

// Dialog Data
	//{{AFX_DATA(CWinAtmKillDlg)
	enum { IDD = IDD_WINATMKILL_DIALOG };
	CString	m_S_VALUE;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinAtmKillDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);			// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CWinAtmKillDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINATMKILLDLG_H__895027BD_8751_4893_814D_A4BFD825FEDB__INCLUDED_)
