#include "stdafx.h"
#include "io.h"
#include "direct.h"
#include "stdio.h"
#include "stdlib.h"
#include "CmnLibEx.h"

//////////////////////////////////////////////////////////////////////
//	Ini Define
//////////////////////////////////////////////////////////////////////
#define	_REGKEY_DEFAULT			_T("SOFTWARE\\ATM\\APP\\DEFAULTSETTING")
#define	_DEFVAL_INIDIR			_T("D:\\INI")

CString	m_strIniDir("");										// INI颇老DIR

//////////////////////////////////////////////////////////////////////
int WINAPI GetIniName(LPTSTR IniName, LPCTSTR strIniFileName)
{
	if(strlen(strIniFileName)==0)						return FALSE;

//	if (m_strIniDir.IsEmpty())									// 烹钦包府
//	{
//		m_strIniDir = RegGetStr(_REGKEY_DEFAULT, "INIDIR", _DEFVAL_INIDIR);
//		if (m_strIniDir.IsEmpty())	
//			m_strIniDir = _DEFVAL_INIDIR;
//		_mkdir(m_strIniDir);
//	}
//	sprintf(IniName, "%s\\%s", m_strIniDir, strIniFileName);

	sprintf(IniName, "%s", strIniFileName);						// 俺喊包府
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
int WINAPI IniGetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nDefault)
{
	char	IniName[MAX_PATH];
	CString strResult("");
	char	szStrValue[256] = {0};

	memset(szStrValue, 0, sizeof(szStrValue));
	::itoa(nDefault, szStrValue, 10);

	if (GetIniName(IniName, szFileName))
	{
		strResult = IniGetStr(szFileName, szSectionName, szEntryName, szStrValue);
		return ::atoi(strResult.GetBuffer(0));
	}
	else
		return NULL;
}

///////////////////////////////////////////////////////////////////////////////
CString WINAPI IniGetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szDefault)
{
	char	IniName[MAX_PATH];
	CString strResult("");
	char	szValue[STRBUFFSIZE] = {0};
	char	szDefValue[STRBUFFSIZE] = {0};
	UINT	lResult;

	memset(szValue, 0, sizeof(szValue));

	if (GetIniName(IniName, szFileName))
	{
		lResult = ::GetPrivateProfileString(szSectionName, szEntryName, "",
											  szValue, sizeof(szValue), IniName);
		strResult = szValue;
		if (!lResult)
		{
			memset(szDefValue, 0, sizeof(szDefValue));
			if (szDefault)
				sprintf(szDefValue, "%s", szDefault);
			lResult = IniSetStr(szFileName, szSectionName, szEntryName, szDefValue);
			strResult = szDefValue;
		}
		return strResult;
	}
	return strResult;
}

///////////////////////////////////////////////////////////////////////////////
int WINAPI IniSetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nValue)
{
	char	IniName[MAX_PATH];
	char	szStrValue[256] = {0};
	int		nRet = NULL;

	memset(szStrValue, 0, sizeof(szStrValue));
	::itoa(nValue, szStrValue, 10);

	if (GetIniName(IniName, szFileName))
	{
		nRet = ::WritePrivateProfileString(szSectionName, szEntryName, szStrValue, IniName);
		IniFlush(IniName);
	}

	return nRet;
}

///////////////////////////////////////////////////////////////////////////////
int WINAPI IniSetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szValue)
{
	char	IniName[MAX_PATH];
	int		nRet = NULL;

	if (GetIniName(IniName, szFileName))
	{
		nRet = ::WritePrivateProfileString(szSectionName, szEntryName, szValue, IniName);
		IniFlush(IniName);
	}
	
	return nRet;
}

///////////////////////////////////////////////////////////////////////////////
int WINAPI IniFlush(LPCTSTR szFileName)
{
	FILE*	TempFp;

	TempFp = fopen(szFileName, "ac");
	if (TempFp)
	{
		fflush(TempFp);
		fclose(TempFp);
	}
	
	return TRUE;
}

