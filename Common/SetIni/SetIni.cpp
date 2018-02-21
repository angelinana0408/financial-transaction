// SetIni.cpp : Defines the entry point for the console application.
//

#include "io.h"
#include "direct.h"
#include "stdio.h"
#include "stdlib.h"

#include <windows.h>
#include <tlhelp32.h>
#include <iostream.h>

/////////////////////////////////////////////////////////////////////////////
int WINAPI IniSetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nValue);
int WINAPI IniSetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szValue);
/////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
    char	szPara[5][256];										// Max Support 5 Parameters
	
	if (argc < 2)												// No Parameter
	{
		cout << "USAGE: SetIni FileName SectionName EntryName Value i/s";
		exit(1);
	}

	if (argc < 6)												// Not Enough Parameters
	{
		cout << "CAUTION: NOT Enough Parameter!!!";
		exit(1);
	}

	if (argc > 6)												// Redundant Parameters
	{
		cout << "CAUTION: Redundant Parameter!!!";
		exit(1);
	}

	for (int i = 1; i < argc; i++)								// Get Parameters
	{
		memset(szPara[i-1], 0x00, sizeof(szPara[i-1]));
		memcpy(szPara[i-1], argv[i], strlen(argv[i]));
	}

	if ((strcmp(szPara[4], "i") == 0)	||
		(strcmp(szPara[4], "I") == 0))
		IniSetInt(	szPara[0],									// Set Integer in INI File
					szPara[1],
					szPara[2],
					atoi(szPara[3]));
	else
	if ((strcmp(szPara[4], "s") == 0)	||
		(strcmp(szPara[4], "S") == 0))
		IniSetStr(	szPara[0],									// Set String in INI File
					szPara[1],
					szPara[2],
					szPara[3]);
	else
	{
		cout << "CAUTION: Unrecognized Parameter!!!";
		exit(1);
	}
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
int WINAPI GetIniName(LPTSTR IniName, LPCTSTR strIniFileName)
{
	if(strlen(strIniFileName)==0)						return FALSE;
	
	sprintf(IniName, "%s", strIniFileName);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
int WINAPI IniSetInt(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, int nValue)
{
	char	IniName[MAX_PATH];
	char	szStrValue[256] = {0};
	
	memset(szStrValue, 0, sizeof(szStrValue));
	::itoa(nValue, szStrValue, 10);
	
	if (GetIniName(IniName, szFileName))
		return ::WritePrivateProfileString(szSectionName, szEntryName, szStrValue, IniName);
	else 
		return NULL;
}

/////////////////////////////////////////////////////////////////////////////
int WINAPI IniSetStr(LPCTSTR szFileName, LPCTSTR szSectionName, LPCTSTR szEntryName, LPCTSTR szValue)
{
	char	IniName[MAX_PATH];
	
	if (GetIniName(IniName, szFileName))
		return ::WritePrivateProfileString(szSectionName, szEntryName, szValue, IniName);
	else 
		return NULL;
}