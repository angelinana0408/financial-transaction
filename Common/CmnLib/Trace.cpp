#include "stdafx.h"
#include "direct.h"
#include "stdio.h"
#include "stdlib.h"
#include "CmnLibEx.h"
#include "Cryptogram.h"

//////////////////////////////////////////////////////////////////////
//	Trace Define
//////////////////////////////////////////////////////////////////////
#define	_REGKEY_DEFAULT			_T("SOFTWARE\\ATM\\APP\\DEFAULTSETTING")
#define	_DEFVAL_TRACEDIR		_T("D:\\TRACE")

int		m_nTraceMode = TRACE_CODE_MODE;							// 개발(분석운영)
CString	m_strTraceDir("");										// 트레이스를 남길 디렉토리
int		m_nCryptoMode = 0;										// 로그의 암호화 여부
CString m_szCryptoPWD("");										// 암호화 키(패스워드)

//////////////////////////////////////////////////////////////////////
int WINAPI GetDumpName(LPTSTR DumpName, LPCTSTR strTraceFileName)
{
	if(strlen(strTraceFileName)==0)						return FALSE;

	if (m_strTraceDir.IsEmpty())
	{
		m_nTraceMode = RegGetInt(_REGKEY_DEFAULT, "TRACELEVEL", TRACE_CODE_MODE);
		m_strTraceDir = RegGetStr(_REGKEY_DEFAULT, "TRACEDIR", _DEFVAL_TRACEDIR);
		m_nCryptoMode = RegGetInt(_REGKEY_DEFAULT, "CRYPTOMODE", 0);
		m_szCryptoPWD = RegGetStr(_REGKEY_DEFAULT, "CRYPTOPWD", _T(""));

		if (m_strTraceDir.IsEmpty())	
			m_strTraceDir = _DEFVAL_TRACEDIR;
		_mkdir(m_strTraceDir);
	}

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);
	sprintf(DumpName,"%s\\%s%02d%02d.Txt",m_strTraceDir,strTraceFileName,lTime.wMonth,lTime.wDay); // modify by yusy 2008.09.09

	CFileStatus	fileStatus;
	if(!CFile::GetStatus(DumpName,fileStatus))			return TRUE;
	if(fileStatus.m_mtime.GetYear()==lTime.wYear)		return TRUE; // modify by yusy 2008.09.09

	DeleteFile(DumpName);
	return TRUE;
}

int WINAPI HexDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR FileName, int LineNum, char* title, void* lpData, int len, int Code)
{
	char*	FindFileName;
	char	filebuf[256];
    char	buf[256];

	FindFileName = strrchr(FileName,'\\');
	sprintf(buf,"%5.5d",LineNum);

	if (FindFileName)
	{
		sprintf(filebuf, "%15.15s", FindFileName + 1);
		return HexDump(nLevel, strTraceFileName, filebuf, buf, title, lpData, len, Code);
	}
	else
		return HexDump(nLevel, strTraceFileName, FileName, buf, title, lpData, len, Code);
}

int WINAPI HexDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR strSubSystem, LPCTSTR strOperation, char* title, void* lpData, int len, int Code)
{
///////////////////////// Memory Information /////////////////////////////
	MEMORYSTATUS memStatus;										// 2004.11.05
	memStatus.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memStatus);
	DWORD uvm;
	uvm = memStatus.dwTotalVirtual - memStatus.dwAvailVirtual;
//////////////////////////////////////////////////////////////////////////

	char	DumpName[MAX_PATH];
    int		i=0,j=0;
    BYTE	ascii[256],ch;
	CString	strEvent, strTemp;

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);
    strEvent.Format("[%04d/%02d/%02d %02d:%02d:%02d:%03d %s(%s)] VM(%10d):Name : %s , Length : %d\n",
					lTime.wYear,lTime.wMonth,lTime.wDay,
					lTime.wHour,lTime.wMinute,lTime.wSecond,lTime.wMilliseconds,
					strSubSystem,strOperation,
					uvm,
					title,len);

	if(len==0 || lpData==NULL);
	else
	{
		BYTE *pBuf = (BYTE *)lpData;

		memset(ascii,0x00,sizeof(ascii));
		for(i=0;i<len;i++)
		{
			if((i%16)==0)   
			{
				strTemp.Format("%05d:%05d ",i,i+16);
				strEvent += strTemp;
			}

			strTemp.Format("%02x ",pBuf[i]);
			strEvent += strTemp;

			if(j==7)	strEvent += " ";

			if (Code == EBCASC)
				ch = E2A(pBuf[i]);
			else ch = pBuf[i];

			if(ch>=0x20 && ch <= 0x7e)
					ascii[j] = ch;
			else    ascii[j] = '.';

			j++;
			if(j==16)
			{
				strTemp.Format("%-16.16s\n",ascii);
				strEvent += strTemp;
				memset(ascii,0x00,sizeof(ascii));
				j = 0;
			}
		}

		if(j)
		{
			for(;j<16;j++)
			{
				strEvent += "   ";
				if(j && (j%8)==0)
					strEvent += " ";
			}
			strTemp.Format("%-16.16s\n",ascii);
			strEvent += strTemp;
		}

	}
	
	strEvent += "-----------------------------------------------------------------------------\n";

	if (GetDumpName(DumpName,strTraceFileName))
	{
		if (m_nTraceMode >= nLevel)								// TRACE LEVEL 비교
		{
			FILE *fp = fopen(DumpName,"a");
			if(fp==NULL)	return FALSE;

			if ( !m_nCryptoMode )
			{
				fprintf(fp,"%s",(LPCTSTR)strEvent);
			}
			else
			{
				if ( -1 == nLevel )								// Test 용
				{
					fprintf(fp,"%s",(LPCTSTR)strEvent);
				}
				else
				{
					BYTE bDest[STRBUFFSIZE] = { 0, };
					DWORD dwDest = sizeof(bDest);
					
					int nRet = LogEncrypt((LPCSTR)(LPCTSTR)strEvent, (DWORD)strEvent.GetLength(), 
						(LPSTR)bDest, dwDest, (LPCSTR)(LPCTSTR)m_szCryptoPWD);
					
					if ( 0 == nRet )
						fwrite(bDest, sizeof(BYTE), dwDest, fp);
					else
					{
						fprintf(fp,"Crypto ERROR : 0x%08X  ", GetLastError());
						fprintf(fp,"%s",(LPCTSTR)strEvent);
					}
				}
			}

			fclose(fp);
			return TRUE;
		}
	}
	return FALSE;
}

int WINAPI HexDumpPack(int nLevel, LPCTSTR strTraceFileName, LPCTSTR FileName, int LineNum, char* title, void* lpData, int len, int Code)
{
    char	buf[STRBUFFSIZE];
	int		Length = MakePack(lpData, buf, len);

	return HexDump(nLevel, strTraceFileName, FileName, LineNum, title, buf, Length, Code);
}

int WINAPI HexDumpPack(int nLevel, LPCTSTR strTraceFileName, LPCTSTR strSubSystem, LPCTSTR strOperation, char* title, void* lpData, int len, int Code)
{
    char	buf[STRBUFFSIZE];
	int		Length = MakePack(lpData, buf, len);

	return HexDump(nLevel, strTraceFileName, strSubSystem, strOperation, title, buf, Length, Code);
}

int WINAPI MsgDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR FileName, int LineNum, LPCTSTR fmt,...)
{
///////////////////////// Memory Information /////////////////////////////
	MEMORYSTATUS memStatus;										// 2004.11.05
	memStatus.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memStatus);
	DWORD uvm;
	uvm = memStatus.dwTotalVirtual - memStatus.dwAvailVirtual;
//////////////////////////////////////////////////////////////////////////

	char*	FindFileName;

	FindFileName = strrchr(FileName,'\\');
	if (FindFileName)
		FindFileName++;
	else 
		FindFileName = (char*)FileName;


	char	DumpName[MAX_PATH] = {0};
    char	buf[STRBUFFSIZE];
	CString	strEvent;

    memset(buf, 0x00, sizeof(buf));
    vsprintf((LPSTR)buf, (LPCSTR)fmt, (va_list)(&fmt+1));

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);
    strEvent.Format("[%04d/%02d/%02d %02d:%02d:%02d:%03d %15.15s(%5.5d)] VM(%10d):%s\n",
					lTime.wYear,lTime.wMonth,lTime.wDay,
					lTime.wHour,lTime.wMinute,lTime.wSecond,lTime.wMilliseconds,
					FindFileName,LineNum,
					uvm,
					buf);

	if (GetDumpName(DumpName,strTraceFileName))
	{
		if (m_nTraceMode >= nLevel)								// TRACE LEVEL 비교
		{
			FILE *fp = fopen(DumpName,"a");
			if(fp==NULL)	return FALSE;
			
			if ( !m_nCryptoMode )
			{
				fprintf(fp,"%s",(LPCTSTR)strEvent);
			}
			else
			{
				if ( -1 == nLevel )
				{
					fprintf(fp,"%s",(LPCTSTR)strEvent);
				}
				else
				{
					BYTE bDest[STRBUFFSIZE] = { 0, };
					DWORD dwDest = sizeof(bDest);
					
					int nRet = LogEncrypt((LPCSTR)(LPCTSTR)strEvent, (DWORD)strEvent.GetLength(), 
						(LPSTR)bDest, dwDest, (LPCSTR)(LPCTSTR)m_szCryptoPWD);
					
					if ( 0 == nRet )
						fwrite(bDest, sizeof(BYTE), dwDest, fp);
					else
					{
						fprintf(fp,"Crypto ERROR : 0x%08X  ", GetLastError());
						fprintf(fp,"%s",(LPCTSTR)strEvent);
					}
				}
			}

			fclose(fp);
			return TRUE;
		}
	}
	return FALSE;
}

int WINAPI MsgDump(int nLevel, LPCTSTR strTraceFileName, LPCTSTR strSubSystem, LPCTSTR strOperation, LPCTSTR fmt,...)
{
///////////////////////// Memory Information /////////////////////////////
	MEMORYSTATUS memStatus;										// 2004.11.05
	memStatus.dwLength = sizeof(MEMORYSTATUS);
	GlobalMemoryStatus(&memStatus);
	DWORD uvm;
	uvm = memStatus.dwTotalVirtual - memStatus.dwAvailVirtual;
//////////////////////////////////////////////////////////////////////////

	char	DumpName[MAX_PATH] = {0};
    char	buf[STRBUFFSIZE];
	CString	strEvent;

    memset(buf, 0x00, sizeof(buf));
    vsprintf((LPSTR)buf, (LPCSTR)fmt, (va_list)(&fmt+1));

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);
    strEvent.Format("[%04d/%02d/%02d %02d:%02d:%02d:%03d %s(%s)] VM(%10d):%s\n",
					lTime.wYear,lTime.wMonth,lTime.wDay,
					lTime.wHour,lTime.wMinute,lTime.wSecond,lTime.wMilliseconds,
					strSubSystem,strOperation,
					uvm,
					buf);

	if (GetDumpName(DumpName,strTraceFileName))
	{
		if (m_nTraceMode >= nLevel)								// TRACE LEVEL비교
		{
			FILE *fp = fopen(DumpName,"a");
			if(fp==NULL)	return FALSE;
			
			if ( !m_nCryptoMode )
			{
				fprintf(fp,"%s",(LPCTSTR)strEvent);
			}
			else
			{
				if ( -1 == nLevel )
				{
					fprintf(fp,"%s",(LPCTSTR)strEvent);
				}
				else
				{
					BYTE bDest[STRBUFFSIZE] = { 0, };
					DWORD dwDest = sizeof(bDest);
					
					int nRet = LogEncrypt((LPCSTR)(LPCTSTR)strEvent, (DWORD)strEvent.GetLength(), 
						(LPSTR)bDest, dwDest, (LPCSTR)(LPCTSTR)m_szCryptoPWD);
					
					if ( 0 == nRet )
						fwrite(bDest, sizeof(BYTE), dwDest, fp);
					else
					{
						fprintf(fp,"Crypto ERROR : 0x%08X  ", GetLastError());
						fprintf(fp,"%s",(LPCTSTR)strEvent);
					}
				}
			}

			fclose(fp);
			return TRUE;
		}
	}
	return FALSE;
}

int WINAPI DeleteFiles(LPCTSTR DirName, int DeleteDays)
{
	CFileFind	finder;

	CString dir, file;
	dir.Format("%s\\*.*",DirName);

    SYSTEMTIME  lTime;
    GetLocalTime(&lTime);

	BOOL bWorking = finder.FindFile(dir);
	CFileStatus	fileStatus;
	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		dir  = finder.GetFilePath();
		file = finder.GetFileName();

		if(	file == _T(".") ||
			file == _T(".."))	continue;

		if(finder.IsDirectory())
		{
			if(!DeleteFiles(dir))
			{
				return FALSE;
			}

			RemoveDirectory(dir);
			continue;
		}

		if (DeleteDays < 0)										// 무조건 삭제처리
			DeleteFile(dir);
		else
		if (!CFile::GetStatus(dir, fileStatus))	
			continue;
		else
		{
			CTime t1( fileStatus.m_mtime.GetYear(), fileStatus.m_mtime.GetMonth(), 
					  fileStatus.m_mtime.GetDay(), 0, 0, 0 );
			CTime t2( lTime.wYear, lTime.wMonth, lTime.wDay, 0, 0, 0 );
			CTimeSpan ts;
			if (t2 > t1)										// 시간보정
				ts = t2 - t1;  
			else
				ts = t2 - t2;  

			if (ts.GetDays() > DeleteDays)						// 일초과시 삭제처리
				DeleteFile(dir);
		}
	}

	finder.Close();
	return TRUE;
}
