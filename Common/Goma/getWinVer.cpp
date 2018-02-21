//  Exam65.cpp
//  11/8/2001  (rk)
//  Modified: 11/24/2001  (rk)
//  Test the routine GET_WIN_VERSION_EX to check system version
//  and service pack number

#include <windows.h>
#include <iostream.h>

#ifndef VER_SUITE_PERSONAL              // To allow compile on
#define VER_SUITE_PERSONAL 0x00000200   // older systems
#define VER_SUITE_DATACENTER 0x00000080
#define VER_SUITE_ENTERPRISE 0x00000002
#define VER_SUITE_BACKOFFICE 0x00000004
#endif

int GET_WIN_VERSION_EX(int *,int *,int *,int *,int *,int*);

int main()
{
	int iRes,iPack,iVerMajor,iVerMinor,iSysType,iSubSys,iBuild;

	iRes=GET_WIN_VERSION_EX(&iSysType,&iSubSys,&iVerMajor,
		   &iVerMinor,&iPack,&iBuild);

    if(iRes != 0)
	{
	    cout << "Failed: Result code=" << iRes << endl;
		return 0;
	}

	if(iSysType==2) cout << "Windows/95";
	if(iSysType==3) cout << "Windows/98";
	if(iSysType==4) cout << "Windows/ME";
	if(iSysType==5) cout << "Windows/NT";
	if(iSysType==6) cout << "Windows/2000";
	if(iSysType==7) cout << "Windows/NT";

	if(iSubSys==1) cout << " Home Edition" << endl;
	if(iSubSys==2) cout << " Professional" << endl;
	if(iSubSys==3) cout << " Server" << endl;
	if(iSubSys==4) cout << " Advanced Server" << endl;
	if(iSubSys==5) cout << " Data Center Server" << endl;

	cout << "Version#=" << iVerMajor << "." << iVerMinor
		  << "  Service Pack#=" << iPack << " Build no.="
		  << iBuild << endl;
	return 0;
}
int GET_WIN_VERSION_EX(int *iSysType,int *iSubSys,int *iVerMajor,
					int *iVerMinor,int *iPack,int *iBuild)
// Created: 11/8/2001  (RK)
// Last modified: 11/24/2001  (RK)
// Please report any problems or bugs to kochhar@physiology.wisc.edu
// The latest version of this routine can be found at:
//     http://www.neurophys.wisc.edu/ravi/software/winver/
// Get the system version number and service pack number
// This works for Win/95/98/ME and also Win/NT/2000
// Return codes are as follows:
//   0   = Normal termination
//   606 = Unable to identify system type
// iSysType has the following values:
//   0=Unknown, 2=Win/95, 3=Win/98 4=Win/ME, 5=Win/NT
//   6=Win/2000 7=Win/XP
// iSubSys has the following possible values:
//   For Win/95, it is as follows:
//    0=Original,    1=OSR2
//   For Win/98, it is as follows:
//    0=Original,    1=Second Edition
//   For NT/2000/XP, it is as follows:
//    0=Unknown,  1=Home/Personal Edition,  2=Professional
//    3=Server,   4=Advanced Server,   5=Data Center Server
//    6=BackOffice 
// iVerMajor is (e.g.) 4 for Win/NT, 5 for Win/2000
// iVerMinor is (e.g.) 51 for NT v3.51, 0 for NT v 4.00
// iPack (service pack #) is meaningful for NT or 2000 or XP only
// iBuild is the build number
{
	BOOL bResult;
	OSVERSIONINFOEX osvi;
	char szPack[4]="-1";

	*iBuild=0;

    // Try calling GetVersionEx using the OSVERSIONINFOEX structure.
    // If that fails, try using the OSVERSIONINFO structure.

    ZeroMemory(&osvi,sizeof(OSVERSIONINFOEX));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

    if(!(bResult = GetVersionEx((OSVERSIONINFO*)&osvi)) )
    {
       // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.
       osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
       if (!GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
          return 606;
    }

	// The system Type (2=Win95, 3=Win98/ME, 
	//    4=Win/ME, 5=Win/NT  6=Win/2000)
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32s)
        *iSysType=1;
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		*iSysType=2;
	if(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
        *iSysType=5;

	// Major version number
	*iVerMajor=osvi.dwMajorVersion;
	if(*iVerMajor >= 5) *iSysType=6;

	// Minor version number
	*iVerMinor=osvi.dwMinorVersion;

	// For Win/98, minor version is 10
	if((*iSysType==2) && (*iVerMinor==10)) *iSysType=3;
	// For Win/ME, minor version is 90 or greater
	if((*iSysType==3) && (*iVerMinor==90)) *iSysType=4;
	// For Win/XP, minor version is 1
	if((*iSysType==6) && (*iVerMinor==1)) *iSysType=7;

	// Service Pack info
	// For NT/2000, extract the service pack as a number
	*iPack=-1;
	if((*iSysType>=5) && (strcmp(osvi.szCSDVersion,"Service Pack")))
	{
       szPack[0]=osvi.szCSDVersion[12];
       szPack[1]=osvi.szCSDVersion[13];
       szPack[2]=osvi.szCSDVersion[14];
	   *iPack=atoi(szPack);
	}

	// For Win/95, extract OSR info and return in iSubSys
	if(*iSysType==2)
	{
		*iSubSys=0;
        if(osvi.szCSDVersion[1]=='C' || osvi.szCSDVersion[1]=='B' )
			*iSubSys=1;                  // OSR2 (4.00.950B)
	}

	// For Win/98, extract OSR info and return in iSubSys
	if(*iSysType==3)
	{
		*iSubSys=0;
        if(osvi.szCSDVersion[1]=='A')
			*iSubSys=1;                  // Second Edition
	}

	// For NT/2000/XP, determine whether Home, Professional or Server
	// or Advanced Server
	if(*iSysType>=5)
	{
		if(bResult)
		{
            // See if Workstation or Server
            if(osvi.wProductType == VER_NT_WORKSTATION)
			{
                // See if personal edition
                if(osvi.wSuiteMask & VER_SUITE_PERSONAL)
				{
				    *iSubSys=1;
				}
			    else
				{
				     *iSubSys=2;  // Must be Professional
				}
			}

	        if(osvi.wProductType == VER_NT_SERVER ||
				osvi.wProductType == VER_NT_DOMAIN_CONTROLLER)
			{
                // See if data center edition
                if(osvi.wSuiteMask & VER_SUITE_DATACENTER)
				{
                    *iSubSys=5;  // Datacenter Server
				}
			    else if(osvi.wSuiteMask & VER_SUITE_ENTERPRISE)
				{
				    *iSubSys=4;  // Advanced Server
				}
			    else if(osvi.wSuiteMask & VER_SUITE_BACKOFFICE)
				{
				    *iSubSys=6;  // BackOffice Server
				}
			    else
				{
                    *iSubSys=3;  // Must be Server
				}
			}
		}
		else
		{
			// Unable to retrieve OSVERSIONINFOEX, get from 
			// registry (for older versions of NT) instead
            HKEY hKey;
            char szProductType[80];
            DWORD dwBufLen;

            RegOpenKeyEx( HKEY_LOCAL_MACHINE,
               "SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
               0, KEY_QUERY_VALUE, &hKey );
            RegQueryValueEx( hKey, "ProductType", NULL, NULL,
               (LPBYTE)szProductType, &dwBufLen);

            RegCloseKey( hKey );
            if(lstrcmpi( "WINNT", szProductType) == 0 )
               *iSubSys=2;
            if(lstrcmpi( "LANMANNT", szProductType) == 0 )
               *iSubSys=3;
            if(lstrcmpi( "SERVERNT", szProductType) == 0 )
               *iSubSys=4;
		}

		// Build number
		*iBuild=osvi.dwBuildNumber & 0xFFFF;
	}

	return 0;
}
