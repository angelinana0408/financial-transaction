#include <string>
#include <vector>
#include <winsock2.h>

#include "stdafx.h"
#include "CmnLibEx.h"

#pragma comment(lib, "ws2_32.lib")

///////////////////////////////////////////////////////////////////////////////
int WINAPI RegQueryValueExt(LPCTSTR hKeyName, LPCTSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData)
{
	LONG	lResult;
	HKEY	hKeyResult = 0;
	DWORD	dwDisposition = 0;

	if (dwType == REG_INT)
		dwLen = sizeof(int);
	else
	if (dwType == REG_STR)
	{
		if (dwLen<1) return FALSE;
//		memset(lpszData,NULL,dwLen);							// 초기화 무처리 : 2003.05.18
	}

	DWORD	dwTypeSave = dwType;								// Query시 변경됨 : 2003.05.23
	DWORD	dwLenSave = dwLen;

	// Open Key
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,KEY_ALL_ACCESS,&hKeyResult);

	if (lResult == ERROR_SUCCESS) 
	{
		dwType = dwTypeSave;
		dwLen = dwLenSave;
		lResult = RegQueryValueEx(hKeyResult,lpszValueName,0,(LPDWORD)&dwType,(LPBYTE)lpszData,(LPDWORD)&dwLen);

		if (lResult != ERROR_SUCCESS)
		{
			dwType = dwTypeSave;
			dwLen = dwLenSave;
			if (dwType == REG_STR)
				dwLen = strlen((char*)lpszData) + 1;
			lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);

			if (lResult == ERROR_SUCCESS) 
			{
				dwType = dwTypeSave;
				dwLen = dwLenSave;
				lResult = RegQueryValueEx(hKeyResult,lpszValueName,0,(LPDWORD)&dwType,(LPBYTE)lpszData,(LPDWORD)&dwLen);
			}
		}
	}
	else 
	{
		// Create New Key 
		lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,"",
			  			         REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,
								 NULL, &hKeyResult, &dwDisposition);

		if (lResult == ERROR_SUCCESS)
		{
			dwType = dwTypeSave;
			dwLen = dwLenSave;
			if (dwType == REG_STR)
				dwLen = strlen((char*)lpszData) + 1;
			lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);
		}

		if (lResult == ERROR_SUCCESS) 
		{
			dwType = dwTypeSave;
			dwLen = dwLenSave;
			lResult = RegQueryValueEx(hKeyResult,lpszValueName,0,(LPDWORD)&dwType,(LPBYTE)lpszData,(LPDWORD)&dwLen);
		}
	}

	// Close Key
	RegCloseKey(hKeyResult);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;
    else 
		return FALSE;
}

int WINAPI RegSetValueExt(LPCTSTR hKeyName, LPCTSTR lpszValueName, DWORD dwType, DWORD dwLen, LPVOID lpszData)
{
	HKEY	hKeyResult = 0;
	DWORD	dwDisposition = 0;
	LONG	lResult;

	if (dwType == REG_INT)
		dwLen = sizeof(int);

	// Open Key 
	lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,KEY_ALL_ACCESS,&hKeyResult);

	if (lResult == ERROR_SUCCESS) 
	{
		// Set Value On Open Key 
		lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);
	}
	else 
	{
		// Create New Key 
		lResult = RegCreateKeyEx(HKEY_LOCAL_MACHINE,hKeyName,0,"",
			  			         REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,
								 NULL, &hKeyResult, &dwDisposition);

		if (lResult == ERROR_SUCCESS)
			lResult = RegSetValueEx(hKeyResult,lpszValueName,0,dwType,(CONST BYTE*)lpszData,dwLen);
	}

	// Close Key 
	RegCloseKey(hKeyResult);

	if (lResult == ERROR_SUCCESS) 
		return TRUE;
    else 
		return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
int	WINAPI RegGetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int InitValue)
{
	RegQueryValueExt(hKeyName, lpszValueName, REG_INT, sizeof(int), &InitValue);
	return InitValue;
}

CString	WINAPI RegGetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, LPCTSTR InitValue)
{
	char	szData[STRBUFFSIZE] = {0};
	CString	strRet("");

	memset(szData, 0, sizeof(szData));
	if (InitValue)
		sprintf(szData, "%s", InitValue);

	RegQueryValueExt(hKeyName, lpszValueName, REG_STR, sizeof(szData), szData);
	strRet = szData;
	return strRet;
}

int	WINAPI RegSetInt(LPCTSTR hKeyName, LPCTSTR lpszValueName, int SetValue)
{
	return RegSetValueExt(hKeyName, lpszValueName, REG_INT, sizeof(int), &SetValue);
}

int	WINAPI RegSetStr(LPCTSTR hKeyName, LPCTSTR lpszValueName, CString SetValue)
{
	char	szData[STRBUFFSIZE] = {0};

	memset(szData, 0, sizeof(szData));
	sprintf(szData, "%s", SetValue);

	return RegSetValueExt(hKeyName, lpszValueName, REG_STR, SetValue.GetLength()+1, szData);
}

///////////////////////////////////////////////////////////////////////////////
CStringArray strAdapterId;										// Adapter Id

int WINAPI AtmGetAdapterId()
{
	// The following code is designed for Windows 2K.
	// If you want it to work on Windows NT, 
	//   you should read HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\NetworkCards

	HKEY hKey, hSubKey, hNdiIntKey;
	
	strAdapterId.RemoveAll();									// Clear

	if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"System\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}",
		0,
		KEY_READ,
		&hKey) != ERROR_SUCCESS)
		return FALSE;											// Register Open Failed
	
	DWORD			dwIndex		= 0;
	DWORD			dwBufSize	= 256;
	DWORD			dwDataType;
	char			szSubKey[256];
	unsigned char	szData[256];
	
	while(RegEnumKeyEx(hKey, dwIndex++, szSubKey, &dwBufSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(hKey, szSubKey, 0, KEY_READ, &hSubKey) == ERROR_SUCCESS)
		{		
			if(RegOpenKeyEx(hSubKey, "Ndi\\Interfaces", 0, KEY_READ, &hNdiIntKey) == ERROR_SUCCESS)
			{
				dwBufSize = 256;
				if(RegQueryValueEx(hNdiIntKey, "LowerRange", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
				{
					if(strcmp((char*)szData, "ethernet") == 0)	// Identify netcard
					{
						dwBufSize = 256;
						if(RegQueryValueEx(hSubKey, "DriverDesc", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
						{
							dwBufSize = 256;
							if(RegQueryValueEx(hSubKey, "NetCfgInstanceID", 0, &dwDataType, szData, &dwBufSize) == ERROR_SUCCESS)
								strAdapterId.Add((LPCTSTR)szData);
						}
					}
				}
				RegCloseKey(hNdiIntKey);
			}
			RegCloseKey(hSubKey);
		}
		dwBufSize = 256;
	}	/* end of while */
	
	RegCloseKey(hKey);

	if(!strAdapterId.GetSize())									// No Netcard
		return FALSE;
	else
		return TRUE;
}

int WINAPI AtmGetIpAddress(char* szIpAddress)
{
	int		ret;
	char    ServiceName[STRBUFFSIZE] = {0};
	char    pIPAddress[STRBUFFSIZE] = {0};
	CString	RegNetwork("");
	CString	RegSrvcName("");

	RegSrvcName =  (LPCTSTR)"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\1";
	ret = RegQueryValueExt(RegSrvcName, "ServiceName", REG_STR, sizeof(ServiceName), ServiceName);
	if(ret == 0)	return FALSE;

	RegNetwork  = (LPCTSTR)"SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters\\Interfaces\\";
	RegNetwork += (LPCTSTR)(ServiceName);

	ret = RegQueryValueExt(RegNetwork, "IPAddress", REG_BIN, sizeof(pIPAddress), pIPAddress);
	if(ret == 0)	return FALSE;

	strcpy(szIpAddress, pIPAddress);
	return TRUE;
}

int WINAPI AtmGetGateWay(char* szGateWay)
{
	int		ret;
	char    ServiceName[STRBUFFSIZE] = {0};
	char    pGateWay[STRBUFFSIZE] = {0};
	CString	RegNetwork("");
	CString	RegSrvcName("");

	RegSrvcName =  (LPCTSTR)"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\1";
	ret = RegQueryValueExt(RegSrvcName, "ServiceName", REG_STR, sizeof(ServiceName), ServiceName);
	if(ret == 0)	return FALSE;

	RegNetwork  = (LPCTSTR)"SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters\\Interfaces\\";
	RegNetwork += (LPCTSTR)(ServiceName);

	ret = RegQueryValueExt(RegNetwork, "DefaultGateway", REG_BIN, sizeof(pGateWay), pGateWay);
	if(ret == 0)	return FALSE;

	strcpy(szGateWay, pGateWay);
	return TRUE;
}

int WINAPI AtmGetSubnetMask(char* szSubnetMask)
{
	int		ret;
	char    ServiceName[STRBUFFSIZE] = {0};
	char    pSubnetMask[STRBUFFSIZE] = {0};
	CString	RegNetwork("");
	CString	RegSrvcName("");

	RegSrvcName = (LPCTSTR)"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\NetworkCards\\1";
	ret = RegQueryValueExt(RegSrvcName, "ServiceName", REG_STR, sizeof(ServiceName), ServiceName);
	if(!ret)	return FALSE;

	RegNetwork  = (LPCTSTR)"SYSTEM\\ControlSet001\\Services\\Tcpip\\Parameters\\Interfaces\\";
	RegNetwork += (LPCTSTR)(ServiceName);

	ret = RegQueryValueExt(RegNetwork, "SubnetMask", REG_BIN, sizeof(pSubnetMask), pSubnetMask);
	if(!ret)	return FALSE;

	strcpy(szSubnetMask, pSubnetMask);
	return TRUE;
}

//--------------------------------------------------------------------------------------
int WINAPI AtmSetIpAddress(char* szIpAddress)					// 2004.06.18
{
	BOOL	bRet = TRUE;
	int		iCardIndex;
	CString	RegNetwork("");

	if (!AtmGetAdapterId())	return FALSE;						// No NetCard

	for (iCardIndex = 0; iCardIndex < strAdapterId.GetSize(); iCardIndex++)
	{
		RegNetwork  = (LPCTSTR)"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\";
		RegNetwork += (LPCTSTR)(strAdapterId[iCardIndex]);
		
		bRet = bRet && RegSetValueExt(RegNetwork, "IPAddress", REG_MULTI_SZ, strlen(szIpAddress)+2, (BYTE*)szIpAddress);
	}															// Change IP address one by one

	return bRet;
}

int WINAPI AtmSetGateWay(char* szGateWay)
{
	BOOL	bRet = TRUE;
	int		iCardIndex;
	CString	RegNetwork("");

	if (!AtmGetAdapterId())	return FALSE;						// No NetCard

	for (iCardIndex = 0; iCardIndex < strAdapterId.GetSize(); iCardIndex++)
	{
		RegNetwork  = (LPCTSTR)"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\";
		RegNetwork += (LPCTSTR)(strAdapterId[iCardIndex]);
		
		bRet = bRet && RegSetValueExt(RegNetwork, "DefaultGateway", REG_MULTI_SZ, strlen(szGateWay)+1, (BYTE*)szGateWay);
	}															// Change Gateway one by one

	return bRet;
}

int WINAPI AtmSetSubnetMask(char* szSubnetMask)
{
	BOOL	bRet = TRUE;
	int		iCardIndex;
	CString	RegNetwork("");

	if (!AtmGetAdapterId())	return FALSE;						// No NetCard

	for (iCardIndex = 0; iCardIndex < strAdapterId.GetSize(); iCardIndex++)
	{
		RegNetwork  = (LPCTSTR)"SYSTEM\\CurrentControlSet\\Services\\Tcpip\\Parameters\\Interfaces\\";
		RegNetwork += (LPCTSTR)(strAdapterId[iCardIndex]);
		
		bRet = bRet && RegSetValueExt(RegNetwork, "SubnetMask", REG_MULTI_SZ, lstrlen(szSubnetMask)+1, (BYTE*)szSubnetMask);
	}															// Change Subnet Mask one by one

	return bRet;
}

