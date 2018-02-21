#include "stdafx.h"
#include "io.h"
#include "CmnLibEx.h"

//////////////////////////////////////////////////////////////////////
int WINAPI RunExecProgram(char* prog,BOOL bWait,char* cmd)
{
	STARTUPINFO			si;
	PROCESS_INFORMATION	pi;

	memset(&si,0x00,sizeof(STARTUPINFO));
	memset(&pi,0x00,sizeof(PROCESS_INFORMATION));

	si.cb = sizeof(STARTUPINFO);
	//si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	if(!CreateProcess(prog,cmd,NULL,NULL,
					FALSE,0,NULL,NULL,&si,&pi))	return FALSE;

	if(!bWait)		return TRUE;

	WaitForInputIdle(GetCurrentProcess(),INFINITE);
	if(pi.hProcess)
	{
		DWORD	dwExitCode = STILL_ACTIVE;
		while(dwExitCode ==STILL_ACTIVE)
		{
			WaitForSingleObject(pi.hProcess,100);
			GetExitCodeProcess( pi.hProcess, &dwExitCode);
		}
	}
	return TRUE;
}

// WinExecWaitEnd
int WINAPI WinExecWaitEnd(LPCTSTR lpCmdLind, UINT uCmdShow)
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

