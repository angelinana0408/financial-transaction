//  Exam53.cpp
//  Created: 4/4/2001  (rk)
//  Last modified: 4/4/2001  (rk)
//  Get MS Internet Explorer version number by reading 
//  the Registry

#include <windows.h>
#include <stdio.h>

int GET_MSIE_VERSION(int *,int *);

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	int iRes,iMajor=0,iMinor=0;
	char szVer[50];

	iRes=GET_MSIE_VERSION(&iMajor,&iMinor);

	if(iRes==0)
	{
		sprintf(szVer,"IE Major Version=%i Minor Version=%i",
			iMajor,iMinor);
		MessageBox(NULL,(LPTSTR)szVer,"Success",
			MB_OK|MB_ICONINFORMATION|MB_TOPMOST);
	}
	if(iRes!=0)
         MessageBox(NULL,LPTSTR("Unable to get MSIE version"),
		  "Error",MB_OK|MB_ICONERROR|MB_TOPMOST);

	return 0;
}

int GET_MSIE_VERSION(int *iMajor, int *iMinor)
// Created: 3/23/2001  (rk)
// Last modified: 4/4/2001  (rk)
// Please send any bugs or suggestions to kochhar@physiology.wisc.edu
// Get MS Internet Explorer version by reading the registry
// Return major version in iMajor, minor version in iMinor
// Return code is 0 for success, error otherwise
// Should work with Windows 95/98/ME/NT/2000
// Return codes are as follows:
//    0    : Success
//    644  : Unable to open Registry Key
//    645  : Unable to read key value
//    650  : Unable to decipher IE version number
{
	LONG lResult;
	int iPos,iPos2;
    HKEY hKey;
	DWORD dwSize=100,dwType;
	char szVAL[100],szTemp[5];
	char *pDec,*pDec2;

    // Open the key for query access
	lResult = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                    LPCTSTR("SOFTWARE\\Microsoft\\Internet Explorer"),
					0,KEY_QUERY_VALUE,&hKey);

	if(lResult != ERROR_SUCCESS)   // Unable to open Key
	{
		return 644;
	}

    // OK, read the value
	lResult=::RegQueryValueEx(hKey,LPTSTR("Version"),NULL,
			&dwType, LPBYTE(szVAL),&dwSize);

	if(lResult != ERROR_SUCCESS)    // Unable to get value
	{
	  // Close the key before quitting
		lResult=::RegCloseKey(hKey);
	  return 645;
	}

	// Close the key
    lResult=::RegCloseKey(hKey);

	// Extract major version by looking for the first decimal
	pDec=strstr(szVAL,".");
	if(pDec==NULL)
		return 650;         // Unable to decipher version number
	iPos=pDec-szVAL+1;
	ZeroMemory(szTemp,5);
	strncpy(szTemp,szVAL,iPos-1);
	*iMajor=atoi(szTemp);

	// Find the Minor version number, look for second decimal
	pDec++;
	pDec2=strstr(pDec,".");
	if(pDec2==NULL)
	{
		*iMinor=0;          // Minor version not found
		return 0;
	}
	iPos2=pDec2-szVAL+1;
	ZeroMemory(szTemp,5);
	strncpy(szTemp,pDec,iPos2-iPos-1);
	*iMinor=atoi(szTemp);

	return 0;
}