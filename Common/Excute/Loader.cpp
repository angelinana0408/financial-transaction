// Loader.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Loader.h"
#include "LoaderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLoaderApp

BEGIN_MESSAGE_MAP(CLoaderApp, CWinApp)
	//{{AFX_MSG_MAP(CLoaderApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoaderApp construction

CLoaderApp::CLoaderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CLoaderApp object

CLoaderApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CLoaderApp initialization

BOOL CLoaderApp::InitInstance()
{
	// ------------------------------------------------------------------------
	// 중복실행 방지
	// ------------------------------------------------------------------------
	// 1. MUTEX를 하나 생성한다
	HANDLE hMutexAP = ::CreateMutex(NULL, TRUE, _T("T1AtmExcute"));
	
	// 2. 존재 Flag를 설정한다
	BOOL bMutexFound = FALSE; 
 
	// 3. 이미 동일한 이름의 뮤텍스가 존재한다면....
	//    새로 생성되지는 않는다.
	if(::GetLastError() == ERROR_ALREADY_EXISTS) 
	{
        bMutexFound = TRUE; 
    }

	// 4. 뮤텍스를 해제한다
	if(hMutexAP) 
    {
        ::ReleaseMutex(hMutexAP); 
    }

    // 5. 이미 하나의 Instance가 존재하면 프로그램 종료
    if(bMutexFound)
    {
//		AfxMessageBox("이미 T1AtmFrontAP 프로그램이 실행중입니다");
        return FALSE;
    }

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

	CLoaderDlg dlg;
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

