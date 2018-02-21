// LoaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\Addins\tagConst.h"
#include "Loader.h"
#include "LoaderDlg.h"

#define COMPILE_MULTIMON_STUBS									// multimon.h Define
#include ".\Addins\MultiMon.h"									// multi-monitor

#include "io.h"
#include "direct.h"
#include "Shlwapi.h"

#include "CmnLibIn.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);			// DDX/DDV support
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
// CLoaderDlg dialog

CLoaderDlg::CLoaderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoaderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoaderDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLoaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoaderDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoaderDlg, CDialog)
	//{{AFX_MSG_MAP(CLoaderDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoaderDlg message handlers

BOOL CLoaderDlg::OnInitDialog()
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

	// OPEN SHAREDATA
	OpenShareData();											// 2005.10.07

/////////////////////////////////////////////////////////////////////////////
	// CREATE HTMLVIEW
	m_HtmlView.CreateFromStatic(IDC_HTML_VIEWER, this);

	// CURSOR
	ShowCursor(FALSE);

	// REPOSITION WINDOW
	OnResizeWindow();

	// FRONTAP START
	CString strStart;
	strStart.Format("FILE://%s\\%s", _DEF_EXERELEPATH, "MAIN.HTM");
	m_HtmlView.Navigate(strStart);

	OnStartTimer();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLoaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CLoaderDlg::OnPaint() 
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
HCURSOR CLoaderDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

/////////////////////////////////////////////////////////////////////////////
#define		MAX_MONITOR		16

struct		structMonitorInfo
{
	HMONITOR	hMonitor;
	char		szDevice[256];
	RECT		rcMonitor;
	RECT		rcWork;
	DWORD		dwFlag;
};

structMonitorInfo MonitorInfo[MAX_MONITOR];

static int MaxMonitorIndex = 0;
static int CurMonitorIndex = 0;
static int PriMonitorIndex = 0;

/////////////////////////////////////////////////////////////////////////////
void CLoaderDlg::OnResizeWindow()
{
	CWnd	*pWnd;
	CRect	rect;
	
	rect.left = 0 - 4;											// 2005.04.19
	rect.top = 0 - 50;
	rect.right = 800 + 4;										// 2005.04.19
	rect.bottom = 600 + 100;
	
	SetWindowPos(&wndNoTopMost, 
		rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	
	MoveWindow(rect);
	
	rect.top = 0;
	rect.bottom = 600;
	for(pWnd=GetTopWindow(); pWnd!= NULL;pWnd = pWnd->GetNextWindow())
	{
		ScreenToClient(rect);
		pWnd->MoveWindow(rect);
	}
	
	return;
}

/////////////////////////////////////////////////////////////////////////////
BOOL CALLBACK CallbackMonitorInfo(HMONITOR	hMonitor,
								  HDC		hdc,
								  LPRECT	prc,
								  LPARAM	lParam)
{
	MONITORINFOEX	MIX;
	MONITORINFO		MI;

	MI.cbSize = sizeof(MI);
	GetMonitorInfo(hMonitor, &MI);

	MIX.cbSize = sizeof(MIX);
	GetMonitorInfo(hMonitor, &MIX);

	MonitorInfo[CurMonitorIndex].hMonitor	= hMonitor;
	strcpy(MonitorInfo[CurMonitorIndex].szDevice, (LPSTR)MIX.szDevice);
	MonitorInfo[CurMonitorIndex].rcMonitor	= MI.rcMonitor;
	MonitorInfo[CurMonitorIndex].rcWork		= MI.rcWork;
	MonitorInfo[CurMonitorIndex].dwFlag		= MI.dwFlags;

	if (MonitorInfo[CurMonitorIndex].dwFlag == MONITORINFOF_PRIMARY)
		PriMonitorIndex = CurMonitorIndex;

	CurMonitorIndex++;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
void CLoaderDlg::OnStartTimer()
{
	m_Timer = SetTimer(1, 500, NULL);							// 2004.11.05
}

void CLoaderDlg::OnTimer(UINT nIDEvent) 
{
	HWND	hMyWnd = NULL;
	CString strStatus = GetShareData("AP_STATUS");

	if (strStatus.CompareNoCase("ON") == 0)
	{
		hMyWnd = ::FindWindow(NULL, AfxGetAppName());
		::SetForegroundWindow(hMyWnd);
		::ShowWindow(hMyWnd, SW_SHOW);
	}
	else
	if (strStatus.CompareNoCase("OFF") == 0)
	{
		hMyWnd = ::FindWindow(NULL, AfxGetAppName());
		::ShowWindow(hMyWnd, SW_HIDE);
	}

	CDialog::OnTimer(nIDEvent);
}

void CLoaderDlg::OnStopTimer()
{
	KillTimer(m_Timer);
}

void CLoaderDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnStopTimer();

	// CLOSE SHAREDATA
	CloseShareData();											// 2005.10.07
}
