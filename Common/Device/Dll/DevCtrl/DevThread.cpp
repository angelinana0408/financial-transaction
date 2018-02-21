/////////////////////////////////////////////////////////////////////////////
//	DevThread.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"
#include <wininet.h>

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Profile.h"
#include "..\..\H\Dll\Network.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	THREAD
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Vfd Control Thread
/////////////////////////////////////////////////////////////////////////////
int CDevCmn::fnAPT_VfdControlThread()
{
/////////////////////////////////////////////////////////////////////////////
	VfdThreadUse = TRUE;										// 2005.04.19
/////////////////////////////////////////////////////////////////////////////

	static	CString		strCmdPosition(_INIKEY_VFDINFO_CMD);
	static	int			nSendDelayCnt = 5;

	CString				strTempCmd("");
	CStringArray		strTempCmdArray;
	int					i = 0;

/////////////////////////////////////////////////////////////////////////////
	while (VfdDoing)
	{
		if (!fnAPL_GetDefineDevice(DEV_VFD))
			break;

		if (nSendDelayCnt > 0)									// Send Delay Check
		{
			nSendDelayCnt--;
			break;
		}

		if (fnAPL_GetDownErrorDevice(DEV_VFD))					// Vfd Error Check
		{
			nSendDelayCnt = 5;
			break;
		}

/////////////////////////////////////////////////////////////////////////////
		// NEXT CMD POSITION, DELAY TIME(SEC), CMD, OPTION1, OPTION2...
		strTempCmd = IniGetStr(_VFDINFO_INI, _INIKEY_VFDINFO, strCmdPosition);
		SplitString(strTempCmd, ",", strTempCmdArray);
		if (strTempCmdArray.GetSize() >= 3)
			;
		else
		{
			VfdDoing = FALSE;									// 2005.04.19
			break;
		}

		for (i = 0; i < strTempCmdArray.GetSize(); i++)			// Trim
		{
			strTempCmdArray[i].TrimLeft();
			strTempCmdArray[i].TrimRight();
		}
		
/////////////////////////////////////////////////////////////////////////////
		if (strTempCmdArray[2].CompareNoCase("SetBrightnessLevel") == 0)
		{
			if (strTempCmdArray.GetSize() >= 4)
			{	
				// fnVFD_SetBrightnessLevel(int nLevel);
				fnVFD_SetBrightnessLevel(Asc2Int(strTempCmdArray[3]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("SetDisplayAtPowerOn") == 0)
		{
			if (strTempCmdArray.GetSize() >= 7)
			{	
				// fnVFD_SetDisplayAtPowerOn(int nSMemIndex, 
				//							 int nEMemIndex, 
				//							 int nScrolled, 
				//							 int nInterval);
				fnVFD_SetDisplayAtPowerOn(Asc2Int(strTempCmdArray[3]),
										  Asc2Int(strTempCmdArray[4]),
										  Asc2Int(strTempCmdArray[5]),
										  Asc2Int(strTempCmdArray[6]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("InitializeDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 3)
			{	
				// fnVFD_InitializeDisplay();
				fnVFD_InitializeDisplay();
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("ClearDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 3)
			{	
				// fnVFD_ClearDisplay();
				fnVFD_ClearDisplay();
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("ScrollDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 6)
			{	
				// fnVFD_ScrollDisplay(int nShiftNum, 
				//					   int nRepeatNum, 
				//					   int nInterval);
				fnVFD_ScrollDisplay(Asc2Int(strTempCmdArray[3]),
									Asc2Int(strTempCmdArray[4]),
									Asc2Int(strTempCmdArray[5]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("BlinkDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 7)
			{	
				// fnVFD_BlinkDisplay(int nPattern, 
				//					  int nNormalTime, 
				//					  int nReverseTime, 
				//					  int nRepeatNumber);
				fnVFD_BlinkDisplay(Asc2Int(strTempCmdArray[3]),
								   Asc2Int(strTempCmdArray[4]),
								   Asc2Int(strTempCmdArray[5]),
								   Asc2Int(strTempCmdArray[6]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("CurtainDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 6)
			{	
				// fnVFD_CurtainDisplay(int nDirection, 
				//						int nSpeed, 
				//						int nPattern);
				fnVFD_CurtainDisplay(Asc2Int(strTempCmdArray[3]),
								     Asc2Int(strTempCmdArray[4]),
									 Asc2Int(strTempCmdArray[5]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("RealTimeTextDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 11)
			{	
				// fnVFD_RealTimeTextDisplay(LPCTSTR szTextData, 
				//							 int nOffsetX, 
				//							 int nOffsetY, 
				//							 int nCharSzX, 
				//							 int nCharSzY, 
				//							 int nBoldStyle, 
				//							 int nItalic, 
				//							 LPCTSTR szFontName);
				fnVFD_RealTimeTextDisplay(strTempCmdArray[3],
										  Asc2Int(strTempCmdArray[4]),
										  Asc2Int(strTempCmdArray[5]),
										  Asc2Int(strTempCmdArray[6]),
										  Asc2Int(strTempCmdArray[7]),
										  Asc2Int(strTempCmdArray[8]),
										  Asc2Int(strTempCmdArray[9]),
										  strTempCmdArray[10]);
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("RealTimeBitImageDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 6)
			{	
				// fnVFD_RealTimeBitImageDisplay(LPCTSTR szFileName, 
				//								 int nOffsetX, 
				//								 int nOffsetY);
				fnVFD_RealTimeBitImageDisplay(strTempCmdArray[3],
											  Asc2Int(strTempCmdArray[4]),
											  Asc2Int(strTempCmdArray[5]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("RealTimeTextBitImageDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 14)
			{	
				// fnVFD_RealTimeTextBitImageDisplay(LPCTSTR szTextData, 
				//									 int nOffsetX, 
				//									 int nOffsetY, 
				//									 int nCharSzX, 
				//									 int nCharSzY, 
				//									 int nBoldStyle, 
				//									 int nItalic, 
				//									 LPCTSTR szFontName, 
				//									 LPCTSTR szFileName, 
				//									 int nImgOffsetX, 
				//									 int nImgOffsetY);
				fnVFD_RealTimeTextBitImageDisplay(strTempCmdArray[3],
												  Asc2Int(strTempCmdArray[4]),
												  Asc2Int(strTempCmdArray[5]),
												  Asc2Int(strTempCmdArray[6]),
												  Asc2Int(strTempCmdArray[7]),
												  Asc2Int(strTempCmdArray[8]),
												  Asc2Int(strTempCmdArray[9]),
												  strTempCmdArray[10],
												  strTempCmdArray[11],
												  Asc2Int(strTempCmdArray[12]),
												  Asc2Int(strTempCmdArray[13]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("fnVFD_FROMTextSave") == 0)
		{
			if (strTempCmdArray.GetSize() >= 12)
			{	
				// fnVFD_FROMTextSave(int nMemIndex, 
				//					  LPCTSTR szTextData, 
				//					  int nOffsetX, 
				//					  int nOffsetY, 
				//					  int nCharSzX, 
				//					  int nCharSzY, 
				//					  int nBoldStyle, 
				//					  int nItalic, 
				//					  LPCTSTR szFontName);
				fnVFD_FROMTextSave(Asc2Int(strTempCmdArray[3]),
								   strTempCmdArray[4],
								   Asc2Int(strTempCmdArray[5]),
								   Asc2Int(strTempCmdArray[6]),
								   Asc2Int(strTempCmdArray[7]),
								   Asc2Int(strTempCmdArray[8]),
								   Asc2Int(strTempCmdArray[9]),
								   Asc2Int(strTempCmdArray[10]),
								   strTempCmdArray[11]);
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("fnVFD_FROMBitImageSave") == 0)
		{
			if (strTempCmdArray.GetSize() >= 7)
			{	
				// fnVFD_FROMBitImageSave(int nMemIndex, 
				//						  LPCTSTR szFileName, 
				//						  int nOffsetX, 
				//						  int nOffsetY);
				fnVFD_FROMBitImageSave(Asc2Int(strTempCmdArray[3]),
									   strTempCmdArray[4],
									   Asc2Int(strTempCmdArray[5]),
									   Asc2Int(strTempCmdArray[6]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("fnVFD_FROMTextBitImageSave") == 0)
		{
			if (strTempCmdArray.GetSize() >= 15)
			{	
				// fnVFD_FROMTextBitImageSave(int nMemIndex, 
				//							  LPCTSTR szTextData, 
				//							  int nOffsetX, 
				//							  int nOffsetY, 
				//							  int nCharSzX, 
				//							  int nCharSzY, 
				//							  int nBolStyle, 
				//							  int nItalic, 
				//							  LPCTSTR szFontName, 
				//							  LPCTSTR szFileName, 
				//							  int nImgOffsetX, 
				//							  int nImgOffsetY);
				fnVFD_FROMTextBitImageSave(Asc2Int(strTempCmdArray[3]),
										   strTempCmdArray[4],
										   Asc2Int(strTempCmdArray[5]),
										   Asc2Int(strTempCmdArray[6]),
										   Asc2Int(strTempCmdArray[7]),
										   Asc2Int(strTempCmdArray[8]),
										   Asc2Int(strTempCmdArray[9]),
										   Asc2Int(strTempCmdArray[10]),
										   strTempCmdArray[11],
										   strTempCmdArray[12],
										   Asc2Int(strTempCmdArray[13]),
										   Asc2Int(strTempCmdArray[14]));
			}
		}
		else
		if (strTempCmdArray[2].CompareNoCase("fnVFD_FROMDisplay") == 0)
		{
			if (strTempCmdArray.GetSize() >= 4)
			{	
				// fnVFD_FROMDisplay(int nMemIndex);
				fnVFD_FROMDisplay(Asc2Int(strTempCmdArray[3]));
			}
		}
		else
		{
			VfdDoing = FALSE;									// 2005.04.19
			break;
		}

/////////////////////////////////////////////////////////////////////////////
		strCmdPosition = strTempCmdArray[0];					// Next Command Position

		nSendDelayCnt = Asc2Int(strTempCmdArray[1]) + 5;		// Delay Time(Sec + 5)
		if (nSendDelayCnt < 5)									// Adjust
			nSendDelayCnt = 5;
/////////////////////////////////////////////////////////////////////////////
		break;
	}
	
/////////////////////////////////////////////////////////////////////////////
	VfdThreadUse = FALSE;										// 2005.04.19
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Elec Journal Send Thread
/////////////////////////////////////////////////////////////////////////////
UINT fnAPT_EjrSendThread(LPVOID pParam)
{
	CDevCmn*			m_pDevCmn = (CDevCmn*)pParam;
	
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->EjrThreadUse = TRUE;								// 2005.04.19
/////////////////////////////////////////////////////////////////////////////

	CString				strEjrWriteSerialNo("");
	CString				strEjrSendSerialNo("");
	char				szTempEjrSerialNo[256];										

	char				szTempFileName[256];
	CFileStatus			TempFileStatus;
	FILE*				TempFp;
	EjrTbl				TempEjr;
	int					nTempFReadNum = 0;
	CString				strTempPrintData("");
	CString				strTempSendPrintData("");
	int					i = 0;
	int					nSendDelayCount = 1;					// 2005.03.16

/////////////////////////////////////////////////////////////////////////////
	while (m_pDevCmn->EjrDoing)
	{
		if ((memcmp(m_pDevCmn->NetDefine.BpIpAddress2, "000", 3) == 0)	||
			(memcmp(m_pDevCmn->NetDefine.BpPortNo2, "00000", 5) == 0))
			break;

		for (i = 0; i < nSendDelayCount; i++)
		{
			Delay_Msg(1000);
			if (!m_pDevCmn->EjrDoing)
				continue;
		}
		nSendDelayCount = 1;									// 2005.03.16

		strEjrWriteSerialNo = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRWRITEP);
		strEjrSendSerialNo = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP);

		if (strEjrWriteSerialNo != strEjrSendSerialNo)
		{
			memcpy(szTempEjrSerialNo, strEjrSendSerialNo.GetBuffer(0), 4);
			if (memcmp(szTempEjrSerialNo, "9999", 4) == 0)		// 2005.04.19
				memset(szTempEjrSerialNo, '0', 4);
			else
				AddString(szTempEjrSerialNo, "0001", 4);
			strEjrSendSerialNo.Format("%4.4s", szTempEjrSerialNo);
			
			sprintf(szTempFileName, "%s\\%4.4s.TXT", 
									_EJR_SEND, 
									strEjrSendSerialNo.GetBuffer(0));
			TempFp = fopen(szTempFileName, "rb");				// Journaling Send
			if (TempFp)
			{
				nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, TempFp);
				fclose(TempFp);
				if (nTempFReadNum)
				{
					strTempSendPrintData = "";
					for (i = 0; i < JPR_MAX_ROW; i++)
					{
						strTempPrintData = GetString(TempEjr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
						strTempPrintData.TrimRight();
						if (strTempPrintData.GetLength())
							strTempSendPrintData += strTempPrintData + "|";
					}

					switch (m_pDevCmn->NetDefine.InterfaceKind)
					{
						case 0:									// 0:CUP(WH)		// 2004.05.08
						case 13:								// 13:CUP(WH8583)	// 2005.10.20
							strTempPrintData.Format("%4.4d %2.2s %4.4s %2.2s %2.2s %4.4s %2.2s %2.2s %2.2s ",
														strTempSendPrintData.GetLength() + 33,
														"T1",
														&TempEjr.SerialNo[4],
														&TempEjr.Date[4],
														&TempEjr.Date[6],
														TempEjr.Date,
														TempEjr.Time,
														&TempEjr.Time[2],
														&TempEjr.Time[4]);
							break;

						default:								// OTHER		: 2004.08.26
							strTempPrintData.Format("%4.4d %2.2s %9.9s %8.8s %8.8s %2.2s %2.2s %4.4s %2.2s %2.2s %2.2s ",
														strTempSendPrintData.GetLength() + 55,
														"T1",
														&m_pDevCmn->NetDefine.BranchNum[0],
														&m_pDevCmn->NetDefine.AtmSerialNum[0],
														&TempEjr.SerialNo[0],
														&TempEjr.Date[4],
														&TempEjr.Date[6],
														TempEjr.Date,
														TempEjr.Time,
														&TempEjr.Time[2],
														&TempEjr.Time[4]);
							break;								// 2004.08.26
					}

					strTempSendPrintData = strTempPrintData + strTempSendPrintData;
					
					if ((memcmp(m_pDevCmn->NetDefine.BpIpAddress2, "000", 3) == 0)	||
						(memcmp(m_pDevCmn->NetDefine.BpPortNo2, "00000", 5) == 0))
					{
						RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP, strEjrSendSerialNo);
						DeleteFile(szTempFileName);
					}
					else
					if ((m_pDevCmn->fnNET_Initialize2(LINE_EJR_OPCL_TIME))	&&
						(m_pDevCmn->fnNET_SendData2(MakeUnPack(strTempSendPrintData.GetBuffer(0), strTempSendPrintData.GetLength()), LINE_EJR_SEND_TIME)))
					{
						RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP, strEjrSendSerialNo);
						DeleteFile(szTempFileName);
						m_pDevCmn->fnNET_Deinitialize2(LINE_EJR_OPCL_TIME);
					}
					else										// 2004.08.23
					{
						m_pDevCmn->fnNET_Deinitialize2(LINE_EJR_OPCL_TIME);
						nSendDelayCount = 60;					// 2005.04.19
					}
				}
			}
			else
			{
				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP, strEjrSendSerialNo);
			}
		}
	}
	
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->EjrThreadUse = FALSE;							// 2005.04.19
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Ftp Send Thread
/////////////////////////////////////////////////////////////////////////////
UINT fnAPT_FtpSendThread(LPVOID pParam)
{
	CDevCmn*	m_pDevCmn = (CDevCmn*)pParam;

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->FtpThreadUse = TRUE;								// 2005.06.15
/////////////////////////////////////////////////////////////////////////////

	HINTERNET		hInternet, hFtp;
	CString			strFtpSendFile("");
	CString			strUserName("");
	CString			strPassword("");
	int				nFtpKind = 0;
	CString			strServerDir("");
	CString			strPutFtpFile("");
	CString			strPutAtmFile("");
	CString			strSrcFtpFile("");
	CString			strZipFileExp("");
	CString			strTemp("");
	char			szIpAddress[256];
	int				nFtpPort = 0;

	CFileStatus		TempFileStatus;
	CStringArray	strTempArray;
	int				i = 0;
	int				nSendDelayCount = 1;

	strUserName		= RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPUSERNAME);
	strPassword		= RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPPASSWORD);
	RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND, 0);
	RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND1, 0);
	RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND2, 0);
	RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND3, 0);
	RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND4, 0);
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME1, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME2, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME3, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME4, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3, "");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4, "");

	memset(szIpAddress, 0, sizeof(szIpAddress));
	sprintf(szIpAddress, "%d.%d.%d.%d", Asc2Int(&m_pDevCmn->NetDefine.BpIpAddress4[0], 3),
										Asc2Int(&m_pDevCmn->NetDefine.BpIpAddress4[4], 3),
										Asc2Int(&m_pDevCmn->NetDefine.BpIpAddress4[8], 3),
										Asc2Int(&m_pDevCmn->NetDefine.BpIpAddress4[12], 3));
	nFtpPort = Asc2Int(m_pDevCmn->NetDefine.BpPortNo4, 5);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPT_FtpSendThread() szIpAddress[%s] nFtpPort[%d]", szIpAddress, nFtpPort);

/////////////////////////////////////////////////////////////////////////////
	while (m_pDevCmn->FtpDoing)
	{
		for (i = 0; i < nSendDelayCount; i++)
		{
			Delay_Msg(1000);
			if (!m_pDevCmn->FtpDoing)
				continue;

			if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME) == "")
			{
				if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1) != "")
				{
					RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND, RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND1));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME1));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME1, "");
				}
				else
				if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2) != "")
				{
					RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND, RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND2));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME2));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME2, "");
				}
				else
				if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3) != "")
				{
					RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND, RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND3));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME3));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME3, "");
				}
				else
				if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4) != "")
				{
					RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND, RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND4));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME4));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4));
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME4, "");
				}
			}
		}
		nSendDelayCount = 1;

		if ((strUserName == "")		||							// User/Pass Check
			(strPassword == ""))
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPT_FtpSendThread() UserName Or Password has not set");
			break;
		}
																// Ip/Port Check
		if ((memcmp(m_pDevCmn->NetDefine.BpIpAddress4, "000", 3) == 0)	||
			(memcmp(m_pDevCmn->NetDefine.BpPortNo4, "00000", 5) == 0))
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPT_FtpSendThread() BpIpAddress4 or BpPortNo4 has not set");
			break;
		}

		strFtpSendFile = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME);
		if (strFtpSendFile == "")								// FileName Check
		{
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, "");
			continue;
		}

		nFtpKind = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPKIND);
		strServerDir = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME);
		
		strPutFtpFile = "";
		if (CFile::GetStatus(strFtpSendFile, TempFileStatus))
		{
			if (SplitString(strFtpSendFile, "\\", strTempArray) == 1)
				strPutFtpFile = strFtpSendFile;
			else
			if (SplitString(strFtpSendFile, "\\", strTempArray) >= 2)
				strPutFtpFile = strTempArray[strTempArray.GetSize() - 1];
			else
			{
				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, "");
				continue;
			}
		}
		else													// File Not Found
		{
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, "");
			continue;
		}
		
/////////////////////////////////////////////////////////////////////////////
		if (nFtpKind == 1)										// Skip : 2005.10.25
		{
			_mkdir(_AP_TEMPFTP_DIR);
			DeleteFiles(_AP_TEMPFTP_DIR, -1);
			strSrcFtpFile.Format("%s\\%s", _AP_TEMPFTP_DIR, strPutFtpFile);
			if (CopyFile(strFtpSendFile, strSrcFtpFile, 0) == 0)
			{
				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, "");
				continue;
			}

			strPutAtmFile = strSrcFtpFile;
		}
		else
		if (nFtpKind == 2)										// No Add Atm Serial Num : 2010.04.01
		{
			_mkdir(_AP_TEMPFTP_DIR);
			DeleteFiles(_AP_TEMPFTP_DIR, -1);
			strSrcFtpFile.Format("%s\\%s.ZIP", _AP_TEMPFTP_DIR, strPutFtpFile);
			strZipFileExp.Format("C:\\T1ATM\\APP\\EXERELE\\PKZIP25 -add %s %s", strSrcFtpFile, strFtpSendFile);
			WinExecWaitEnd(strZipFileExp, SW_HIDE);

			strPutFtpFile = strPutFtpFile + ".ZIP";
			strPutAtmFile = strSrcFtpFile;
		}
		else													// Zip
		{
			_mkdir(_AP_TEMPFTP_DIR);
			DeleteFiles(_AP_TEMPFTP_DIR, -1);
			strSrcFtpFile.Format("%s\\%s.ZIP", _AP_TEMPFTP_DIR, strPutFtpFile);
			strZipFileExp.Format("C:\\T1ATM\\APP\\EXERELE\\PKZIP25 -add %s %s", strSrcFtpFile, strFtpSendFile);
			WinExecWaitEnd(strZipFileExp, SW_HIDE);

			strTemp.Format("%8.8s", m_pDevCmn->m_pProfile->NETWORK.AtmSerialNum);
			if (strPutFtpFile.Find(strTemp) >= 0)				// 2006.06.01
				strPutFtpFile = strPutFtpFile + ".ZIP";
			else
				strPutFtpFile = strTemp + strPutFtpFile + ".ZIP";
			strPutAtmFile = strSrcFtpFile;
		}

/////////////////////////////////////////////////////////////////////////////
		hInternet = InternetOpen("FTPSERVER", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
		if (!hInternet)
		{
			nSendDelayCount = 60;
			continue;
		}
		
		hFtp = InternetConnect(hInternet, 
							   szIpAddress,
							   nFtpPort, 
							   strUserName, 
							   strPassword, 
							   INTERNET_SERVICE_FTP, 
							   0, 
							   0);
		if (!hFtp) 
		{
			nSendDelayCount = 60;
			InternetCloseHandle(hInternet);
			continue;
		}

		if (strServerDir.GetLength())							// 2009.07.01
		{
			if (!FtpSetCurrentDirectory(hFtp, strServerDir))
			{
				if (!FtpCreateDirectory(hFtp, strServerDir))
				{
					nSendDelayCount = 60;
					InternetCloseHandle(hFtp);
					InternetCloseHandle(hInternet);
					continue;
				}
				else
				if (!FtpSetCurrentDirectory(hFtp, strServerDir))
				{
					nSendDelayCount = 60;
					InternetCloseHandle(hFtp);
					InternetCloseHandle(hInternet);
					continue;
				}
			}
		}

		if (!FtpPutFile(hFtp, 
						strPutAtmFile, 
						strPutFtpFile, 
						FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,
						0)) 
		{
			nSendDelayCount = 60;
			InternetCloseHandle(hFtp);
			InternetCloseHandle(hInternet);
			continue;
		}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPT_FtpSendThread(): [FtpPut] LocalFileName[%s] FtpFileName[%s]",
											 strPutAtmFile, 
											 strPutFtpFile);

		InternetCloseHandle(hFtp);
		InternetCloseHandle(hInternet);

		DeleteFiles(_AP_TEMPFTP_DIR, -1);

		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPFILENAME, "");
	}

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->FtpThreadUse = FALSE;							// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

