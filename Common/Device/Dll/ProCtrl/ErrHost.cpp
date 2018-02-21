/////////////////////////////////////////////////////////////////////////////
//	ErrHost.cpp
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

HErrName Main[]={
	"Loop"				,
	"Count"		
};

DErrName ErrName[]={
     "Remark"
};

/////////////////////////////////////////////////////////////////////////////
//	Set Error Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec)
{
	int		i = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	memcpy(&ERR.Remark[0], &DErrInfo->Remark[0], sizeof(ERR.Remark) - 1);
	ERR.Remark[sizeof(ERR.Remark)] = NULL;

	i = 0;
	MAIN.Loop		= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	MAIN.Count		= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);

	sprintf(SUB_SECT, "%s %d", SubSec, MAIN.Count);

	i = 0;
	PutPStr(SUB_SECT, ErrName[i++].name, ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);

	MAIN.Count++;												// Count
	if (MAIN.Count >= ErrHost_MaxNumber) 
	{
		MAIN.Loop = 1;											// Loop
		PutPInt(MainSec, Main[0].name, MAIN.Loop,	_ERRHOST_INI);
		MAIN.Count = 0;
	}

	PutPInt(MainSec, Main[1].name, MAIN.Count,	_ERRHOST_INI);
}

/////////////////////////////////////////////////////////////////////////////
//	Clear Error
/////////////////////////////////////////////////////////////////////////////
void CProfile::ClearAllErrSub(char* MainSec, char* SubSec)
{
	int		i = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	memset(&MAIN, 0, sizeof(HERRINFO));
	i = 0;
	PutPInt(MainSec, Main[0].name, MAIN.Loop,	_ERRHOST_INI);
	PutPInt(MainSec, Main[1].name, MAIN.Count,	_ERRHOST_INI);

	memset(&ERR,  0, sizeof(DERRINFO));
	StrAllSpace(ERR.Remark);

	for (int j = 0; j < ErrHost_MaxNumber; j++) 
	{
		sprintf(SUB_SECT, "%s %d", SubSec, j);

		i = 0;
		PutPStr(SUB_SECT, ErrName[i++].name, ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI, FALSE);
	}

	IniFlush(_ERRHOST_INI);										// 2005.07.21 : Speed Up
}

/////////////////////////////////////////////////////////////////////////////
//	Current Error Info
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetCrtErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	i = 0;
	MAIN.Loop	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	MAIN.Count	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	
	if (MAIN.Loop || MAIN.Count)								// Error Occured
	{
		if (MAIN.Count == 0)
			MAIN.Count = ErrHost_MaxNumber - 1;
		else MAIN.Count = MAIN.Count - 1;

		sprintf(SUB_SECT, "%s %d", SubSec, MAIN.Count);

		i = 0;
		GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);
		return 1;
	}
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	All Error Info
/////////////////////////////////////////////////////////////////////////////
int CProfile::GetAllErrSub(DERRINFO* DErrInfo, char* MainSec, char* SubSec)
{
	int		i = 0, ii = 0;
	char	SUB_SECT[81];
	HERRINFO	MAIN;
	DERRINFO	ERR;

	i = 0;
	MAIN.Loop	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	MAIN.Count	= GetPInt(MainSec, Main[i++].name, 0, _ERRHOST_INI);
	
	if (MAIN.Loop || MAIN.Count)								// Error Occured
	{
		if (MAIN.Count)
		{
			for (int j = (MAIN.Count - 1); j >= 0; j--)			// From Recent Error
			{
				sprintf(SUB_SECT, "%s %d", SubSec, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);

				memcpy(&DErrInfo[ii++], &ERR, sizeof(DERRINFO));
			}
		}

		if (MAIN.Loop)
		{
			for (int j = (ErrHost_MaxNumber - 1); j >= MAIN.Count; j--) 
			{
				sprintf(SUB_SECT, "%s %d", SubSec, j);

				i = 0;
				GetPStr(SUB_SECT, ErrName[i++].name, '?', ERR.Remark, sizeof(ERR.Remark), _ERRHOST_INI);

				memcpy(&DErrInfo[ii++], &ERR, sizeof(DERRINFO));
			}
			return ErrHost_MaxNumber;
		}
		else return MAIN.Count;
	}
	else return 0;
}

/////////////////////////////////////////////////////////////////////////////
//	Transactrion Error Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrTran(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, TRANMAIN_SEC, TRANERR_SEC);
	return;
}
void CProfile::ClearAllErrTran()
{
	ClearAllErrSub(TRANMAIN_SEC, TRANERR_SEC);
	return;
}
int CProfile::GetCrtErrTran(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, TRANMAIN_SEC, TRANERR_SEC));
}
int CProfile::GetAllErrTran(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, TRANMAIN_SEC, TRANERR_SEC));
}
	
/////////////////////////////////////////////////////////////////////////////
//	Cancel Error Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrCancel(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, CANCELMAIN_SEC, CANCELERR_SEC);
	return;
}
void CProfile::ClearAllErrCancel()
{
	ClearAllErrSub(CANCELMAIN_SEC, CANCELERR_SEC);
	return;
}
int CProfile::GetCrtErrCancel(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, CANCELMAIN_SEC, CANCELERR_SEC));
}
int CProfile::GetAllErrCancel(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, CANCELMAIN_SEC, CANCELERR_SEC));
}

/////////////////////////////////////////////////////////////////////////////
//	Host Error Info
/////////////////////////////////////////////////////////////////////////////
void CProfile::ErrHost(DERRINFO* DErrInfo)
{
	ErrSub(DErrInfo, HOSTMAIN_SEC, HOSTERR_SEC);
	return;
}
void CProfile::ClearAllErrHost()
{
	ClearAllErrSub(HOSTMAIN_SEC, HOSTERR_SEC);
	return;
}
int CProfile::GetCrtErrHost(DERRINFO* DErrInfo)
{
	return (GetCrtErrSub(DErrInfo, HOSTMAIN_SEC, HOSTERR_SEC));
}
int CProfile::GetAllErrHost(DERRINFO* DErrInfo)
{
	return (GetAllErrSub(DErrInfo, HOSTMAIN_SEC, HOSTERR_SEC));
}

