/////////////////////////////////////////////////////////////////////////////
//	ErrStack.cpp
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Profile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

MainName Main[]={
	"Loop"				,
	"CrtCount"			,
	"StackCount"
};

ErrorName ErrName[] = {
	"Date"				,
	"Time"				,
	"ProcCount" 	    ,
	"ErrorCode"			,
	"ErrorString"			
};

static	ERRSTACK	FIRSTERRSAVE;								// First Error Save
static	ERRSTACK	LASTERRSAVE;								// Last Error Save

/////////////////////////////////////////////////////////////////////////////
//	Error Stack
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrStack(char pProcCount, char* pErrorCode, char* pErrorString)
{
	int		i = 0;
	char	gDate[9], gTime[7];
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	memset(&ERR, 0, sizeof(ERRSTACK));							// Initialize
	GetDateTime(gDate, gTime);									// YYYYMMDD,HHMMSS
	sprintf(ERR.Date, "%4.4s/%2.2s/%2.2s", &gDate[0], &gDate[4], &gDate[6]);
	sprintf(ERR.Time, "%2.2s:%2.2s:%2.2s", &gTime[0], &gTime[2], &gTime[4]);

	ERR.ProcCount[0] = pProcCount;
	ERR.ProcCount[1] = NULL;

	if (FindChar(pErrorCode, sizeof(ERR.ErrorCode), NULL) < sizeof(ERR.ErrorCode))
		memcpy(&ERR.ErrorCode[0], pErrorCode, FindChar(pErrorCode, sizeof(ERR.ErrorCode), NULL));
	else
		memcpy(&ERR.ErrorCode[0], pErrorCode, sizeof(ERR.ErrorCode) - 1);
	ERR.ErrorCode[sizeof(ERR.ErrorCode)] = NULL;

	if (FindChar(pErrorString, sizeof(ERR.ErrorString), NULL) < sizeof(ERR.ErrorString))
		memcpy(&ERR.ErrorString[0], pErrorString, FindChar(pErrorString, sizeof(ERR.ErrorString), NULL));
	else
		memcpy(&ERR.ErrorString[0], pErrorString, sizeof(ERR.ErrorString) - 1);
	ERR.ErrorString[sizeof(ERR.ErrorString)] = NULL;	

	i = 0;
	MAIN.Loop		= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);

	if (MAIN.StackCount)										// First Error Stack
	{
		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// Last Error Save
		return;
	}
	else
	{
		memcpy(&FIRSTERRSAVE, &ERR, sizeof(FIRSTERRSAVE));		// First Error Save
		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// Last Error Save
	}

//	if (MAIN.StackCount)										// Last Error Stack
//	{												
//		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// Last Error Save
//		if (MAIN.CrtCount == 0)
//			MAIN.CrtCount = ErrStack_MaxNumber - 1;
//		else MAIN.CrtCount = MAIN.CrtCount - 1;
//	}
//	else
//	{
//		memcpy(&FIRSTERRSAVE, &ERR, sizeof(FIRSTERRSAVE));		// Last Error Save
//		memcpy(&LASTERRSAVE, &ERR, sizeof(LASTERRSAVE));		// Last Error Save
//	}

	sprintf(SUB_SECT, "%s %d", ERR_SEC, MAIN.CrtCount);

	i = 0;
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.Date,			sizeof(ERR.Date),		_ERRSTACK_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
 	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ProcCount,		sizeof(ERR.ProcCount),	_ERRSTACK_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorCode,		sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

	MAIN.StackCount++;											// Stack Count
	if (MAIN.StackCount > ErrStack_MaxNumber)
		MAIN.StackCount = ErrStack_MaxNumber;

	MAIN.CrtCount++;											// Current Count
	if (MAIN.CrtCount >= ErrStack_MaxNumber) 
	{
		MAIN.Loop = 1;											// Loop
		PutPInt(MAIN_SEC, Main[0].name, MAIN.Loop, _ERRSTACK_INI);
		MAIN.CrtCount = 0;
	}

	PutPInt(MAIN_SEC, Main[1].name, MAIN.CrtCount,		_ERRSTACK_INI);
	PutPInt(MAIN_SEC, Main[2].name, MAIN.StackCount,	_ERRSTACK_INI);
}

/////////////////////////////////////////////////////////////////////////////
//	Clear All Error Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::ClearAllErrStack()
{
	int		i = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	memset(&MAIN, 0, sizeof(MAININFO));
	i = 0;
	PutPInt(MAIN_SEC, Main[0].name, MAIN.Loop,			_ERRSTACK_INI);
	PutPInt(MAIN_SEC, Main[1].name, MAIN.CrtCount,		_ERRSTACK_INI);
	PutPInt(MAIN_SEC, Main[2].name, MAIN.StackCount,	_ERRSTACK_INI);

	memset(&ERR,  0, sizeof(ERRSTACK));
	StrAllSpace(ERR.Date);
	StrAllSpace(ERR.Time);
	StrAllSpace(ERR.ProcCount);
	StrAllSpace(ERR.ErrorCode);
	StrAllSpace(ERR.ErrorString);

	for (int j = 0; j < ErrStack_MaxNumber; j++) 
	{
		sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

		i = 0;
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.Date,			sizeof(ERR.Date),		_ERRSTACK_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
	 	PutPStr(SUB_SECT, ErrName[i++].name, ERR.ProcCount,		sizeof(ERR.ProcCount),	_ERRSTACK_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorCode,		sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Current Error 
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetCrtErrStack(ERRSTACK* pEditBuff, bool pClearCommand)
{
	int		i = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.StackCount)										// Error Occured
	{
		if (MAIN.CrtCount == 0)
			MAIN.CrtCount = ErrStack_MaxNumber - 1;
		else MAIN.CrtCount = MAIN.CrtCount - 1;
		
		sprintf(SUB_SECT, "%s %d", ERR_SEC, MAIN.CrtCount);

		i = 0;
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

		if (pClearCommand)										// Clear Count
		{
			MAIN.StackCount = 0;
			PutPInt(MAIN_SEC, Main[2].name, MAIN.StackCount,	_ERRSTACK_INI);
		}

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 1;
	}
	else														// No Error
	{
		StrAllSpace(ERR.Date);
		StrAllSpace(ERR.Time);
		StrAllSpace(ERR.ProcCount);
		StrAllSpace(ERR.ErrorCode);
		StrAllSpace(ERR.ErrorString);

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Get First Error Stack
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetFirstErrStack(ERRSTACK* pEditBuff)
{
	int		i = 0;
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.StackCount)										// Error Occured
	{
		memcpy(pEditBuff, &FIRSTERRSAVE, sizeof(FIRSTERRSAVE));	// First Error Save
		return 1;
	}
	else														// No Error
	{
		StrAllSpace(ERR.Date);
		StrAllSpace(ERR.Time);
		StrAllSpace(ERR.ProcCount);
		StrAllSpace(ERR.ErrorCode);
		StrAllSpace(ERR.ErrorString);

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Get Last Error Stack
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetLastErrStack(ERRSTACK* pEditBuff)
{
	int		i = 0;
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 1;
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.StackCount = GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.StackCount)										// Error Occured
	{
		memcpy(pEditBuff, &LASTERRSAVE, sizeof(LASTERRSAVE));	// Last Error Save
		return 1;
	}
	else														// No Error
	{
		StrAllSpace(ERR.Date);
		StrAllSpace(ERR.Time);
		StrAllSpace(ERR.ProcCount);
		StrAllSpace(ERR.ErrorCode);
		StrAllSpace(ERR.ErrorString);

		memcpy(pEditBuff, &ERR, sizeof(ERRSTACK));
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	All Error Stack
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetAllErrStack(ERRSTACK* pEditBuff)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	MAININFO	MAIN;
	ERRSTACK	ERR;

	i = 0;
	MAIN.Loop		= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	MAIN.CrtCount	= GetPInt(MAIN_SEC, Main[i++].name, 0, _ERRSTACK_INI);
	
	if (MAIN.Loop || MAIN.CrtCount)								// Error Occured
	{	
		if (MAIN.CrtCount)
		{
			for (int j = (MAIN.CrtCount - 1); j >= 0; j--)		// From Recent Error
			{
				sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

				memcpy(&pEditBuff[ii++], &ERR, sizeof(ERRSTACK));
			}
		}

		if (MAIN.Loop)
		{
			for (int j = (ErrStack_MaxNumber - 1); j >= MAIN.CrtCount; j--) 
			{
				sprintf(SUB_SECT, "%s %d", ERR_SEC, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Date,			sizeof(ERR.Date), 		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Time,			sizeof(ERR.Time),		_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ProcCount,	sizeof(ERR.ProcCount),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorCode,	sizeof(ERR.ErrorCode),	_ERRSTACK_INI);
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.ErrorString,	sizeof(ERR.ErrorString),_ERRSTACK_INI);

				memcpy(&pEditBuff[ii++], &ERR, sizeof(ERRSTACK));
			}
			return ErrStack_MaxNumber;
		}
		else return MAIN.CrtCount;
	}
	else return 0;
}

