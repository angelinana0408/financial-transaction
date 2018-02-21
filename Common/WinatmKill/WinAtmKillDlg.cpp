// WinAtmKillDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinAtmKill.h"
#include "WinAtmKillDlg.h"

#include "io.h"
#include "direct.h"
#include "Shlwapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillDlg dialog

CWinAtmKillDlg::CWinAtmKillDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinAtmKillDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinAtmKillDlg)
	m_S_VALUE = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWinAtmKillDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinAtmKillDlg)
	DDX_Text(pDX, IDC_SECOND, m_S_VALUE);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinAtmKillDlg, CDialog)
	//{{AFX_MSG_MAP(CWinAtmKillDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillDlg message handlers

BOOL CWinAtmKillDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

////////////////////////////////////////////////////////////////////////////////
	CFileStatus	FileStatus;

	if (CFile::GetStatus(_RUNFILE, FileStatus))					// Find Run.bat File
	{
		WinExecWaitEnd(_RUNFILE, SW_HIDE);
	}
	else
	{
		return FALSE;											// Return
	}

	AtmUpdate();												// Ap Update
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_APINSTALL, "APINSTALLOK");

	PostMessage(WM_USER);										// Windows ShutDown

//////////////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinAtmKillDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinAtmKillDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinAtmKillDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWinAtmKillDlg::OnButton1()								// For Test
{
	for (int i = 30; i >= 0; i--)
	{
		char temp[81];
		itoa(i, temp, 10);
		
		m_S_VALUE = temp; 
		UpdateData(FALSE);
		Sleep(1000);
	}

	PostMessage(WM_USER);
}

#define		WAIT_SEC	10

BOOL CWinAtmKillDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->wParam == VK_RETURN)
		return TRUE;

	if (pMsg->message == WM_USER + 1)
	{	
		for (int i = WAIT_SEC; i >= 0; i--)
		{
			char temp[81];
			itoa(i, temp, 10);
			
			m_S_VALUE = temp; 
			UpdateData(FALSE);
			Sleep(1000);
		}

		PostMessage(WM_USER + 2);
	}

	return CDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
int CWinAtmKillDlg::AtmUpdate()
{
	CString strCMD("");
	CString strFileName("");

	_mkdir(_DEF_UPDATEPATH);

	strCMD.Format("%s\\%s %s\\%s", 	_DEF_EXERELEPATH,
									"pkzip25 -ext -directories=full -overwrite=all",
									_DEF_UPDATEPATH,
									"V*.zip c:\\");
	WinExecWaitEnd(strCMD, SW_SHOW);							// 2005.04.19
	
	// Update.reg
	strFileName.Format("%s\\%s", _DEF_UPDATEPATH, _REGFILE);
	if (_access((LPCTSTR)strFileName, 0) == 0)
	{
		strCMD.Format("regedit /s %s", strFileName);
		WinExecWaitEnd(strCMD, SW_HIDE);
	}

	// Update.bat
	strFileName.Format("%s\\%s", _DEF_UPDATEPATH, _UPDATEFILE);
	if (_access((LPCTSTR)strFileName, 0) == 0)
	{
		strCMD.Format("%s", strFileName);
		WinExecWaitEnd(strCMD, SW_HIDE);
	}

	// Delete Update Files
	DeleteFiles(_DEF_UPDATEPATH, -1);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CWinAtmKillDlg::WinExecWaitEnd(LPCTSTR lpCmdLind, UINT uCmdShow)
{
	PROCESS_INFORMATION	pi;
	STARTUPINFO			si;
	BOOL				b;

	if (uCmdShow == SW_HIDE)
	{
		ZeroMemory(&si, sizeof(STARTUPINFO));
		b = CreateProcess(NULL, const_cast<LPTSTR>(lpCmdLind), NULL, NULL, TRUE,
							NORMAL_PRIORITY_CLASS|CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
	}
	else
	{
		ZeroMemory(&si, sizeof(STARTUPINFO));
		b = CreateProcess(NULL, const_cast<LPTSTR>(lpCmdLind), NULL, NULL, TRUE,
							NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	}

	if (!b) return FALSE;

	WaitForSingleObject(pi.hProcess, INFINITE);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int CWinAtmKillDlg::DeleteFiles(LPCTSTR DirName, int DeleteDays)
{
	CFileFind	finder;

	CString dir, file;
	dir.Format("%s\\*.*",DirName);

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);

	BOOL bWorking = finder.FindFile(dir);
	CFileStatus	fileStatus;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		dir  = finder.GetFilePath();
		file = finder.GetFileName();

		if(	file == _T(".") ||
			file == _T(".."))	continue;

		if(finder.IsDirectory())
		{
			if(!DeleteFiles(dir))
			{
				return FALSE;
			}

			RemoveDirectory(dir);
			continue;
		}

		if (DeleteDays < 0)										// Delete
			DeleteFile(dir);
		else
		if (!CFile::GetStatus(dir, fileStatus))	
			continue;
		else
		{
			CTime t1( fileStatus.m_mtime.GetYear(), fileStatus.m_mtime.GetMonth(), 
					  fileStatus.m_mtime.GetDay(), 0, 0, 0 );
			CTime t2( lTime.wYear, lTime.wMonth, lTime.wDay, 0, 0, 0 );
			CTimeSpan ts;
			if (t2 > t1)										// Time Adj
				ts = t2 - t1;  
			else
				ts = t2 - t2;  

			if (ts.GetDays() > DeleteDays)						// Day Over Delete
				DeleteFile(dir);
		}
	}

	finder.Close();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int CWinAtmKillDlg::RegQueryValueExt(LPCTSTR hKeyName, LPCTSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData)
{
	LONG	lResult;
	HKEY	hKeyResult = 0;
	DWORD	dwDisposition = 0;

	if (dwType == REG_INT)
		dwLen = sizeof(int);
	else
	if (dwType == REG_STR)
	{
		if (dwLen<1) return FALSE;
	}

	DWORD	dwTypeSave = dwType;
	DWORD	dwLenSave = dwLen;

	// Open Key
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,KEY_ALL_ACCESS,&hKeyResult);

	if (lResult == ERROR_SUCCESS) 
	{
		dwType = dwTypeSave;
		dwLen = dwLenSave;
		lResult = RegQueryValueEx(hKeyResult,lpszValueName,0,(LPDWORD)&dwType,(LPBYTE)lpszData,(LPDWORD)&dwLen);

		if (lResult != ERROR_SUCCESS)
		{
			dwType = dwTypeSave;
			dwLen = dwLenSave;
			if (dwType == REG_STR)
				dwLen = strlen((char*)lpszData) + 1;
			lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);

			if (lResult == ERROR_SUCCESS) 
			{
				dwType = dwTypeSave;
				dwLen = dwLenSave;
				lResult = RegQueryValueEx(hKeyResult,lpszValueName,0,(LPDWORD)&dwType,(LPBYTE)lpszData,(LPDWORD)&dwLen);
			}
		}
	}
	else 
	{
		// Create New Key 
		lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,"",
			  			         REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,
								 NULL, &hKeyResult, &dwDisposition);

		if (lResult == ERROR_SUCCESS)
		{
			dwType = dwTypeSave;
			dwLen = dwLenSave;
			if (dwType == REG_STR)
				dwLen = strlen((char*)lpszData) + 1;
			lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);
		}

		if (lResult == ERROR_SUCCESS) 
		{
			dwType = dwTypeSave;
			dwLen = dwLenSave;
			lResult = RegQueryValueEx(hKeyResult,lpszValueName,0,(LPDWORD)&dwType,(LPBYTE)lpszData,(LPDWORD)&dwLen);
		}
	}

	// Close Key
	RegCloseKey(hKeyResult);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;
    else 
		return FALSE;
}

int CWinAtmKillDlg::RegSetValueExt(LPCTSTR hKeyName, LPCTSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData)
{
	HKEY	hKeyResult = 0;
	DWORD	dwDisposition = 0;
	LONG	lResult;

	if (dwType == REG_INT)
		dwLen = sizeof(int);

	// Open Key 
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,KEY_ALL_ACCESS,&hKeyResult);

	if (lResult == ERROR_SUCCESS) 
	{
		// Set Value On Open Key 
		lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);
	}
	else 
	{
		// Create New Key 
		lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,"",
			  			         REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,
								 NULL, &hKeyResult, &dwDisposition);

		if (lResult == ERROR_SUCCESS)
			lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);
	}

	// Close Key 
	RegCloseKey(hKeyResult);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;
    else 
		return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
int	CWinAtmKillDlg::RegGetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int InitValue)
{
	RegQueryValueExt(hKeyName, lpszValueName, REG_INT, sizeof(int), &InitValue);
	return InitValue;
}

CString	CWinAtmKillDlg::RegGetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, LPCTSTR InitValue)
{
	char	szData[STRBUFFSIZE] = {0};
	CString	strRet("");

	memset(szData, 0, sizeof(szData));
	if (InitValue)
		sprintf(szData, "%s", InitValue);

	RegQueryValueExt(hKeyName, lpszValueName, REG_STR, sizeof(szData), szData);
	strRet = szData;
	return strRet;
}

int	CWinAtmKillDlg::RegSetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int SetValue)
{
	return RegSetValueExt(hKeyName, lpszValueName, REG_INT, sizeof(int), &SetValue);
}

int	CWinAtmKillDlg::RegSetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, CString SetValue)
{
	char	szData[STRBUFFSIZE] = {0};

	memset(szData, 0, sizeof(szData));
	sprintf(szData, "%s", SetValue);

	return RegSetValueExt(hKeyName, lpszValueName, REG_STR, SetValue.GetLength()+1, szData);
}

