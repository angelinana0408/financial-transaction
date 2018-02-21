// WinAtmKill.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinAtmKill.h"
#include "WinAtmKillDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillApp

BEGIN_MESSAGE_MAP(CWinAtmKillApp, CWinApp)
	//{{AFX_MSG_MAP(CWinAtmKillApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillApp construction

CWinAtmKillApp::CWinAtmKillApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinAtmKillApp object

CWinAtmKillApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinAtmKillApp initialization

BOOL CWinAtmKillApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CWinAtmKillDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is


		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CWinAtmKillApp::Run() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinApp::Run();
}

BOOL CWinAtmKillApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_USER)
	{
		Sleep(100);
		PostMessage(NULL, WM_USER + 1, 0, 0);
	}
	else
	if (pMsg->message == WM_USER + 2)
	{
		NTShutdownProcess();
	}
	
	return CWinApp::PreTranslateMessage(pMsg);
}


void CWinAtmKillApp::NTShutdownProcess() 
{
	HANDLE				hToken;
	LUID				Luid;
	BOOL				bReturn = FALSE;
	DWORD				dwError;
	TOKEN_PRIVILEGES	NewPrivileges;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
		dwError = GetLastError();
		if (dwError == ERROR_CALL_NOT_IMPLEMENTED) 
			return;
	}

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Luid)) 
	{
		CloseHandle(hToken);
		return;
	}

	NewPrivileges.PrivilegeCount			= 1;
	NewPrivileges.Privileges[0].Luid		= Luid;
	NewPrivileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;

	bReturn = AdjustTokenPrivileges(hToken,
									FALSE,
									&NewPrivileges,
									0,
									NULL,
									NULL);

	CloseHandle(hToken);

	::ExitWindowsEx(EWX_SHUTDOWN, (DWORD)-1);
}
