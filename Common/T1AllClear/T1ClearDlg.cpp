// T1ClearDlg.cpp : implementation file
//

#include "stdafx.h"
#include "T1Clear.h"
#include "T1ClearDlg.h"

#include "Psapi.h"
#pragma comment( lib, "psapi.lib")

#include "ProcessList.h"

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
// CT1ClearDlg dialog

CT1ClearDlg::CT1ClearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CT1ClearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CT1ClearDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CT1ClearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CT1ClearDlg)
	DDX_Control(pDX, IDC_TITLE, m_stTitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CT1ClearDlg, CDialog)
	//{{AFX_MSG_MAP(CT1ClearDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CT1ClearDlg message handlers

BOOL CT1ClearDlg::OnInitDialog()
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
	SetTitle();
	SetTimer(1, 1000, 0);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CT1ClearDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CT1ClearDlg::OnPaint() 
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
HCURSOR CT1ClearDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// ----------------------------------------------------------------------------
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
// ----------------------------------------------------------------------------
int CT1ClearDlg::KillProcess(LPCTSTR szAtmName)
{
	DWORD dwProcesses[1024];
	DWORD dwNeeded, dwProcessCount;
	unsigned int i;
	HANDLE hProcess = NULL;
	char	szProcessName[MAX_PATH] = "unknown";
	DWORD	dwExitCode = 0;
	CString szKillName;
		
	if( !EnumProcesses( dwProcesses, sizeof(dwProcesses), &dwNeeded ) )
		return FALSE;

	dwProcessCount = dwNeeded / sizeof(DWORD);
	for( i = 0; i < dwProcessCount; i++ )
	{
		hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
								PROCESS_VM_READ |									
							    PROCESS_TERMINATE,
							    FALSE, dwProcesses[i] );

		if( hProcess )
		{
			HMODULE hModule;
			if( EnumProcessModules( hProcess, &hModule, sizeof(hModule),
									&dwNeeded) )
			{
				GetModuleBaseName( hProcess, hModule, szProcessName,
								   sizeof( szProcessName ) );
				szKillName.Format(szProcessName);
				szKillName.MakeLower();
				if (szKillName.CompareNoCase(szAtmName) == 0)
					break;
			}
		}
	}

	if ( i >= dwProcessCount ) 
	{
		return FALSE;
	}

	if( GetExitCodeProcess( hProcess, &dwExitCode ) )
	{
		TerminateProcess( hProcess, dwExitCode );
	}

	return TRUE;
}

// ----------------------------------------------------------------------------
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
// ----------------------------------------------------------------------------
void CT1ClearDlg::OnTimer(UINT nIDEvent) 
{
	KillTimer(nIDEvent);
	
	switch(nIDEvent)
	{
	case 1 :
		AtmClear();
		break;

	default:break;
	}

	CDialog::OnTimer(nIDEvent);
}

// ----------------------------------------------------------------------------
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
// ----------------------------------------------------------------------------
void CT1ClearDlg::AtmClear()
{
	int nProcCount = itemof(KillList);							// Get ProcessCount that need to delete
	int bAllClear  = TRUE;
	
	for (int i = 0 ; i < nProcCount ; i++)
	{
		if ( KillProcess(KillList[i]) == FALSE)
			bAllClear = FALSE;
	}

	if (!bAllClear)												// retry
	{
		for (int i = 0 ; i < nProcCount ; i++)
		{
			if ( KillProcess(KillList[i]) == FALSE)
				bAllClear = FALSE;
		}
	}
}

// ----------------------------------------------------------------------------
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
// ----------------------------------------------------------------------------
void CT1ClearDlg::SetTitle()
{
	// 메인타이틀을 설정한다.
	m_stTitle.SetForeColor(RGB(255,255,0));
	m_stTitle.SetBackColor(RGB(0,0,0));
	m_stTitle.SetFitText(FALSE);
	m_stTitle.SetFontSize(CSize(12,6));
	m_stTitle.SetWindowText("### WAIT : TERMINATE M/W BASED ATM/CD APPLICATION... ###");
	m_stTitle.SetBlinkColor(RGB(255,0,0));
	m_stTitle.SetInterval(300);
}
