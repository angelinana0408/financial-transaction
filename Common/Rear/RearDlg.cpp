// RearDlg.cpp : implementation file
//

#include "stdafx.h"
#include ".\Addins\tagConst.h"
#include "Rear.h"
#include "RearDlg.h"

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
// CRearDlg dialog

CRearDlg::CRearDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRearDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRearDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRearDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRearDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRearDlg, CDialog)
	//{{AFX_MSG_MAP(CRearDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRearDlg message handlers

BOOL CRearDlg::OnInitDialog()
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
	
/////////////////////////////////////////////////////////////////////////////
	// OPEN SHAREDATA
	OpenShareData();											// 2005.10.07

	// CREATE HTMLVIEW
	m_HtmlView.CreateFromStatic(IDC_HTML_REARVIEW, this);

	// CURSOR
	ShowCursor(FALSE);

	// REPOSITION WINDOW
	OnResizeWindow();

	// REARAP START
	CString strStart;
	strStart.Format("FILE://%s\\%s", _DEF_EXERELEPATH, "REARAP.HTM");
	m_HtmlView.Navigate(strStart);
	
	OnStartTimer();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRearDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
void CRearDlg::OnPaint() 
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
HCURSOR CRearDlg::OnQueryDragIcon()
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
void CRearDlg::OnResizeWindow()
{
	CWnd	*pWnd;
	CRect	rect;
	
//	InitializeDisplayDevice();
	
//	if (MaxMonitorIndex >= 1)									// Support 2Monitor Machine
	if (IniGetInt("D:\\INI\\DEVICE.INI", "DEVICE", "ETCDevice2") == 1)
	{															// 2005.04.19
		rect.left = 0 - 4;										// 2005.04.19
		rect.top = 0 - 50;										// Title Bar Move
		rect.right = 800 + 0;									// 2005.04.19
		rect.bottom = 600 + 100;
	}
	else
	{
		rect.left = 0 - 4;										// 2005.04.19
		rect.top = 0 - 50;
		rect.right = 800 + 4;									// 2005.04.19
		rect.bottom = 600 + 100;
	}
	
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
void CRearDlg::InitializeDisplayDevice()
{
	HDC		hDc;

	hDc = ::GetDC(NULL);		
	EnumDisplayMonitors(hDc, NULL, CallbackMonitorInfo, 0);
	::ReleaseDC(NULL, hDc);

	MaxMonitorIndex = CurMonitorIndex - 1;

	for (int i = 0; i<=MaxMonitorIndex;i++)
	{
		if (i != PriMonitorIndex) 
		{
			CurMonitorIndex = i;
			break;
		}
	}

	m_rOplRect.left   = MonitorInfo[PriMonitorIndex].rcWork.left;
	m_rOplRect.top    = MonitorInfo[PriMonitorIndex].rcWork.top;
	m_rOplRect.right  = MonitorInfo[PriMonitorIndex].rcWork.right;
	m_rOplRect.bottom = MonitorInfo[PriMonitorIndex].rcWork.bottom; 

	m_rSplRect.left   = MonitorInfo[CurMonitorIndex].rcWork.left;
	m_rSplRect.top    = MonitorInfo[CurMonitorIndex].rcWork.top;
	m_rSplRect.right  = MonitorInfo[CurMonitorIndex].rcWork.right;
	m_rSplRect.bottom = MonitorInfo[CurMonitorIndex].rcWork.bottom; 

	return;
}

/////////////////////////////////////////////////////////////////////////////
void CRearDlg::OnStartTimer()
{
	m_Timer = SetTimer(1, 500, NULL);							// 2004.11.05
}

void CRearDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if (GetShareData("OP_MODE").CompareNoCase("OP") == 0)		// 204.06.24
	{
		HWND hRearWnd = ::FindWindow(NULL, "Loader");			// 2005.04.19
		::ShowWindow(hRearWnd, SW_HIDE);

		::SetWindowPos(this->m_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
						SWP_SHOWWINDOW|SWP_NOMOVE|SWP_NOSIZE);

		mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						65535 / 64 * 63,						// Focus x(Mouse)
						65535 / 64,								// Focus y(Mouse)
						NULL, 
						NULL);
		
		SetShareData("OP_MODE", "OPOK");
	}

	CDialog::OnTimer(nIDEvent);
}

void CRearDlg::OnStopTimer()
{
	KillTimer(m_Timer);
}

void CRearDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	OnStopTimer();
	
	// CLOSE SHAREDATA
	CloseShareData();											// 2005.10.07
}
