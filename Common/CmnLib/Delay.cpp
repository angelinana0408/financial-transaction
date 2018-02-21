#include "stdafx.h"
#include "CmnLibEx.h"

int WINAPI Delay_Msg(DWORD time)
{
	DWORD	Ttime = 0;
	MSG		msg;

	while (TRUE)
	{
/*
		if(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(!AfxGetApp()->PreTranslateMessage(&msg)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		if (time <= 10)
		{
			Ttime += 10;
			Sleep(10);
		}
		else
		{
			Ttime += 25;
			Sleep(25);
		}
*/

		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(!AfxGetApp()->PreTranslateMessage(&msg)) {
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}

		// 5ms이하의 딜레이시간은 5ms로 처리한다.
		// 그 이상은 10ms로 처리한다.
		if (time <= 5)
		{
			Ttime += 5;
			Sleep(5);
		}
		else
		{
			Ttime += 10;
			Sleep(10);
		}

		if (Ttime >= time)
			break;
	}

	return TRUE;
}

ULONG WINAPI TimerSet(int nSecond)
{
	return (GetTickCount() + (nSecond * 1000));
}

int WINAPI CheckTimer(ULONG nSetTime)
{
	if (GetTickCount() > nSetTime)
 		return TRUE;
	else 
		return FALSE;
}
