/////////////////////////////////////////////////////////////////////////////
//	DevScr.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\nhmwi.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR) : SCR SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CDevCmn::fnSCR_ClearErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = RegSetStr(_REGKEY_DEVERROR, DEVNM_SCR, "");

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Key Data
int	CDevCmn::fnSCR_ClearKeyData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearKeyData()");

	int		nReturn = FALSE;

	nReturn = m_pMwi->ScrClearKeyData();

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_ClearKeyData():return(%d)", nReturn);
	return nReturn;
}

// Set Screen Data(Screen Data Name, Screen Data Value)
int	CDevCmn::fnSCR_SetScreenData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue)
{
	CString strSetDataName(szSetDataName);

if ((strSetDataName.Find("PIN") >= 0)			||
	(strSetDataName.Find("PASSWORD") >= 0)		||
	(strSetDataName.Find("PASS") >= 0)			||
	(strSetDataName.Find("PWD") >= 0))
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, "*");
else
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, szSetDataValue);

	int		nReturn = FALSE;

	nReturn = m_pMwi->ScrSetScreenData(szSetDataName, szSetDataValue);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Display Data(Display Data Name, Display Data Value)
int	CDevCmn::fnSCR_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetDisplayData(szSetDisplayDataName[%s], szSetDisplayDataValue[%s])", szSetDisplayDataName, szSetDisplayDataValue);

	int		nReturn = FALSE;

	nReturn = m_pMwi->ScrSetDisplayData(szSetDisplayDataName, szSetDisplayDataValue);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetDisplayData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Current Language Mode
int	CDevCmn::fnSCR_SetCurrentLangMode(int nLangMode)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetCurrentLangMode(nLangMode[%d])", nLangMode);

	int		nReturn = FALSE;

	Scr.CurrentLangMode = nLangMode;
	nReturn = nLangMode;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_SetCurrentLangMode(...):return(%d)", nReturn);
	return nReturn;
}
													
/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR) : SCR GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CDevCmn::fstrSCR_GetErrorCode()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetErrorCode()");

	CString strReturn("");

	strReturn = RegGetStr(_REGKEY_DEVERROR, DEVNM_SCR, "0000000");		
	strReturn += "0000000";										// Error Code Adjust
	strReturn = strReturn.Left(7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CDevCmn::fnSCR_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	nReturn = NORMAL;

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Key Data
CString	CDevCmn::fstrSCR_GetKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyData()");

	CString strReturn("");

	strReturn = m_pMwi->ScrGetKeyData(MID_DEVRSP_TIME);

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyData():return(%s)", strReturn);
	return strReturn;
}

// Get Key String(Init Flag)
CString	CDevCmn::fstrSCR_GetKeyString(int nInitFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyString(nInitFlag[%d])", nInitFlag);

	CString strReturn("");

	if (nInitFlag)
		fnSCR_ClearKeyData();									

	strReturn = fstrSCR_GetKeyData();

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyString(...):return(%s)", strReturn);
	return strReturn;
}

// Get Check Screen(Screen No, Language Mode)
CString	CDevCmn::fstrSCR_GetCheckScreen(int nScreenNo, int nLangMode)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetCheckScreen(nScreenNo[%d], nLangMode[%d])", nScreenNo, nLangMode);

	CString	strReturn("");
	CFileStatus	FileStatus;

	if (nScreenNo > 0)
	{
		if (nLangMode == CHN_MODE)	
			strReturn.Format("%sT%3.3sC%3.3d.swf", _SCR_DIR, &TranDefine.ApVersion[10], nScreenNo);
		else							
		if (nLangMode == ENG_MODE)	
			strReturn.Format("%sT%3.3sE%3.3d.swf", _SCR_DIR, &TranDefine.ApVersion[10], nScreenNo);
		else 
		if (nLangMode == KOR_MODE)	
			strReturn.Format("%sT%3.3sK%3.3d.swf", _SCR_DIR, &TranDefine.ApVersion[10], nScreenNo);
		else 
		if (nLangMode == JPN_MODE)	
			strReturn.Format("%sT%3.3sJ%3.3d.swf", _SCR_DIR, &TranDefine.ApVersion[10], nScreenNo);
		else 
			strReturn.Format("%sT%3.3sC%3.3d.swf", _SCR_DIR, &TranDefine.ApVersion[10], nScreenNo);

		if (!CFile::GetStatus(strReturn.GetBuffer(0), FileStatus))	
			strReturn = "";										// File Not Found
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetCheckScreen(...):return(%s)", strReturn);
	return strReturn;
}

// Get Current Screen No
int	CDevCmn::fnSCR_GetCurrentScreenNo()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentScreenNo()");

	int		nReturn = FALSE;

	nReturn = Scr.CurrentScreenNo;								

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentScreenNo():return(%d)", nReturn);
	return nReturn;
}

// Get Current Language Mode
int	CDevCmn::fnSCR_GetCurrentLangMode()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentLangMode()");

	int		nReturn = FALSE;

	nReturn = Scr.CurrentLangMode;								

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_GetCurrentLangMode():return(%d)", nReturn);
	return nReturn;
}

// Get Key Numeric(Ret Length:0=Real Length)
CString	CDevCmn::fstrSCR_GetKeyNumeric(int nRetLength, int nInitFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyNumeric(nRetLength[%d], nInitFlag)", nRetLength, nInitFlag);

	CString strReturn("");
	char	szTemp[256];
	CString strTemp("");

	if (nInitFlag)
		fnSCR_ClearKeyData();									

	strReturn = fstrSCR_GetKeyData();
	if (!IsNum(strReturn.GetBuffer(0), strReturn.GetLength()))	// Character Skip
		;
	else
	if (nRetLength > 0)											
	{
		sprintf(szTemp, "%%%d.%dd%%s", nRetLength, nRetLength);	// Input Data Adjust
		strTemp.Format(szTemp, 0, strReturn);
		strReturn = strTemp.Right(nRetLength);
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fstrSCR_GetKeyNumeric(...):return(%s)", strReturn);
	return strReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	SCR FUNCTION(SCR) : SCR OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CDevCmn::fnSCR_Initialize()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_Initialize()");

	int		nReturn = FALSE;

	nReturn = NORMAL;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CDevCmn::fnSCR_Deinitialize()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = NORMAL;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// Display Screen(Screen No, Input Time, Pin Input Mode, Card Data) 2008.05.09
int	CDevCmn::fnSCR_DisplayScreen(int nScreenNo, int nInputSec, int nPinInputMode, LPCTSTR szCardData, int nPinPassWordMin, int nPinPassWordMax, int nPinPassWordAuto, LPCTSTR szPinPassWordTerm)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayScreen(nScreenNo[%d], nInputSec[%d], nPinInputMode[%d], szCardData[%s], nPinPassWordMin[%d], nPinPassWordMax[%d], nPinPassWordAuto[%d], szPinPassWordTerm[%s])", nScreenNo, nInputSec, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);

	int		nReturn = FALSE;
	CString strTempTopName("");
	CString	strTempCheckName = fstrSCR_GetCheckScreen(nScreenNo, Scr.CurrentLangMode);
	static int iLastScrNo = 0;
	static long lDispTimer = 0;
	long lCurrTimer = 0;

	BYTE    startMV[] = {0x24, 0x3d, 0x30, 0x91, 0x0d, 0x0a};
	BYTE    stopMV[] = {0x24, 0x3e, 0x30, 0x92, 0x0d, 0x0a};

	/////////////////////////////////////////////处理005-等待插卡，027-暂停服务，611-正在读卡三个页面
	
	if(IniGetInt(_ADMACHINE_INI, _INIKEY_ADMACHINE, _INIKEY_ADMACHINE_ADMODE, 0) == 1)
	{
		if (nScreenNo == 5)
		{
			//发送开始循环播放信号
			nReturn = fnAPP_WriteADComm(startMV,6);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm=%d,startMV", nReturn);
			if (nReturn > 0)
			{
				IdleADFlag = 1;
			}
		}
		if (nScreenNo == 27 || nScreenNo == 611)
		{
			//发送停止循环播放信号
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EnterCriticalSection");
			EnterCriticalSection(&ADMutex);
			IdleADFlag = 0;
			nReturn = fnAPP_WriteADComm(stopMV,6);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm=%d,stopMV", nReturn);
			LeaveCriticalSection(&ADMutex);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LeaveCriticalSection");
		}

		////////////////////////////////////////////sleep，等待广告播放结束
		if (nScreenNo == 711)
		{
			iLastScrNo = 711;
			lDispTimer = time(0) + nInputSec;
			nInputSec = 0;
		}
		else
		{
			if(iLastScrNo == 711)
			{
///////////////////////////////////////////////////////////////////////////////////////// liyi 2008.11.04  广告机新方案 start
				if (nScreenNo == 641)
				{
					iLastScrNo = 641;
				}
				else
				{		
					lCurrTimer = time(0);
					if (lCurrTimer < lDispTimer)
						Delay_Msg((lDispTimer - lCurrTimer + 1) * 1000);
					iLastScrNo = 0;
				}
			}
			else if (iLastScrNo == 641)
			{
///////////////////////////////////////////////////////////////////////////////////////// end
//				lCurrTimer = time(0);
//				if (lCurrTimer < lDispTimer)
//					Delay_Msg((lDispTimer - lCurrTimer + 1) * 1000);
// 				iLastScrNo = 0;
				nReturn = fnAPP_WriteADComm(stopMV,6);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm=%d,stopMV", nReturn);
				iLastScrNo = 0;
			}
		}
	}
	
	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode
	{
		if (CashCashMode == CSH_TESTCASH_MODE)					// Test Cash
			strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 3);
		else													// Real Cash
			strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 2);
	}
	else														// Online Mode
	{
		if (CashCashMode == CSH_TESTCASH_MODE)					// Test Cash
			strTempTopName.Format("%sTOP/TOP%d.swf", _SCR_DIR, 1);
		else													// Real Cash
			strTempTopName = "";
	}

	if (strTempTopName != "")									// Top Image
	{
		nReturn = fnSCR_SetDisplayData(SCR_TOP, strTempTopName);
	}

	if (nInputSec > 0)											// Input Time
	{
		nReturn = fnSCR_SetDisplayData(SCR_TIME, Int2Asc(nInputSec));
	}
	
	switch (AtmDefine.MachineKind)								// Numeric Pad & Button Adjust : 2005.11.11
	{
		case NANOCASH:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, TRUE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MCD3:
			fnSCR_DisplayImage(13, TRUE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, TRUE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case TTW:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, TRUE);
			fnSCR_DisplayImage(16, FALSE);
			break;
		case MCD4:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case TTW2:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, FALSE);
			break;
		case EATMGBA1:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case EATMULL1:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case EATMTTW:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case TTW2E:												// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MCD4E:												// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX8000T:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX8000L:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX5600T:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX5600L:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
	}
	
	Scr.CurrentScreenNo = nScreenNo;							// Save
	if (strTempCheckName == "")									// Screen Not Exist? => China Screen
		strTempCheckName = fstrSCR_GetCheckScreen(nScreenNo, CHN_MODE);
	if (strTempCheckName != "")									// Screen Exist? => Display
		nReturn = fnSCR_SetDisplayData(SCR_SCREEN, strTempCheckName);

	if (HostLineMode == HOST_LOCAL_MODE)						// Local Mode
	{
		// FLASH Modify?
		// 300ms Waiting : 2003.10.10
		// Delay_Msg(500);
	}

	fnSCR_ClearKeyData();

	switch (AtmDefine.OpDevice)									// 2005.11.11
	{
		case NOT_SET:
			if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
				;
			else
				fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
		case OP_TOUCH:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
		case OP_TSPL:
		case OP_TSPLCHN:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
		case OP_HSPL:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayScreen(nScreenNo[%d], nInputSec[%d]):return(%d)", nScreenNo, nInputSec, nReturn);
	return nReturn;
}

// Display Update(Screen No, Input Time, Pin Input Mode, Card Data)
int	CDevCmn::fnSCR_DisplayUpdate(int nScreenNo, int nInputSec, int nPinInputMode, LPCTSTR szCardData, int nPinPassWordMin, int nPinPassWordMax, int nPinPassWordAuto, LPCTSTR szPinPassWordTerm)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayUpdate(nScreenNo[%d], nInputSec[%d], nPinInputMode[%d], szCardData[%s], nPinPassWordMin[%d], nPinPassWordMax[%d], nPinPassWordAuto[%d], szPinPassWordTerm[%s])", nScreenNo, nInputSec, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);

	int		nReturn = FALSE;

	switch (AtmDefine.MachineKind)								// Numeric Pad & Button Adjust : 2005.11.11
	{
		case NANOCASH:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, TRUE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MCD3:
			fnSCR_DisplayImage(13, TRUE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, TRUE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case TTW:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, TRUE);
			fnSCR_DisplayImage(16, FALSE);
			break;
		case MCD4:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case TTW2:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, FALSE);
			break;
		case EATMGBA1:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case EATMULL1:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case EATMTTW:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case TTW2E:												// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MCD4E:												// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX8000T:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX8000L:											// 2005.11.11
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX5600T:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
		case MX5600L:
			fnSCR_DisplayImage(13, FALSE);

			fnSCR_DisplayImage(14, TRUE);
			fnSCR_DisplayImage(15, FALSE);
			fnSCR_DisplayImage(16, TRUE);
			break;
	}
	
	nReturn = fnSCR_SetDisplayData(SCR_UPDATE, SCR_UPDATE_SCREEN);
																// Screen Update
	fnSCR_ClearKeyData();

	switch (AtmDefine.OpDevice)									// 2005.11.11
	{
		case NOT_SET:
			if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
				;
			else
				fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
		case OP_TOUCH:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
		case OP_TSPL:
		case OP_TSPLCHN:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
		case OP_HSPL:
			fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm);
			break;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayUpdate(nScreenNo[%d], nInputSec[%d]):return(%d)", nScreenNo, nInputSec, nReturn);
	return nReturn;
}

// Display Message()
int	CDevCmn::fnSCR_DisplayMessage()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayMessage()");

	int		nReturn = FALSE;

	nReturn = fnSCR_SetDisplayData(SCR_UPDATE, SCR_UPDATE_MESSAGE);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayMessage():return(%d)", nReturn);
	return nReturn;
}


// Display Image(Image No, OnOff Flag)
int	CDevCmn::fnSCR_DisplayImage(int nImageNo, int nOnOffFlag)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayImage(nImageNo[%d], nOnOffFlag[%d])", nImageNo, nOnOffFlag);

	int		nReturn = FALSE;
	CString strTemp("");

	if (nImageNo > 0)
	{
		strTemp.Format("%s%d", SCR_BTNONOFF, nImageNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), ((nOnOffFlag) ? "on" : "off"));
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayImage(...):return(%d)", nReturn);
	return nReturn;
}

// Display String(String No, String, Length)
int	CDevCmn::fnSCR_DisplayString(int nStringNo, LPCTSTR szString, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szString);
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display String(String No, String)
int	CDevCmn::fnSCR_DisplayString(int nStringNo, LPCTSTR szString)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s])", nStringNo, szString);

	int		nReturn = FALSE;
	CString strTemp("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), szString);
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display String Array(String No, String, Length)
int	CDevCmn::fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");
	CStringArray strTempArray;

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szString);
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if (nStringNo > 0)
	{
		SplitString(strTemp2, "\n", strTempArray);
		for (int i = 0; i < strTempArray.GetSize(); i++)
		{
			strTemp.Format("%s%d", SCR_STRING, nStringNo + i);
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTempArray[i].GetBuffer(0));
		}
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display String (String No, String)
int	CDevCmn::fnSCR_DisplayStringArray(int nStringNo, LPCTSTR szString)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(nStringNo[%d], szString[%s])", nStringNo, szString);

	int		nReturn = FALSE;
	CString strTemp("");
	CStringArray strTempArray;

	if (nStringNo > 0)
	{
		strTemp.Format("%s", szString);							// 2004.11.05
		SplitString(strTemp, "\n", strTempArray);				// 2004.11.05
		for (int i = 0; i < strTempArray.GetSize(); i++)
		{
			strTemp.Format("%s%d", SCR_STRING, nStringNo + i);
			nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTempArray[i].GetBuffer(0));
		}
	}

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayString(...):return(%d)", nReturn);
	return nReturn;
}

// Display Numeric(String No, Numeric, Length)
int	CDevCmn::fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric, int nInLength)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szNumeric);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], szNumeric[%s], nInLength[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength);

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		strTemp2.Format("%d", Asc2Int(szNumeric, nInLength));
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// Display Numeric(String No, Numeric)
int	CDevCmn::fnSCR_DisplayNumeric(int nStringNo, LPCTSTR szNumeric)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], szNumeric[%s])", nStringNo, szNumeric);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		strTemp2.Format("%d", Asc2Int(szNumeric, strlen(szNumeric)));
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// Display Numeric(String No, Numeric)
int	CDevCmn::fnSCR_DisplayNumeric(int nStringNo, int nNumeric)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(nStringNo[%d], nNumeric[%d])", nStringNo, nNumeric);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);
		strTemp2.Format("%d", nNumeric);
		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayNumeric(...):return(%d)", nReturn);
	return nReturn;
}

// Display Amount(String No, Amount, Length, Mark, IsMinus)
int	CDevCmn::fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, int nInLength, char cMarkChar, int nIsMinus)
{
	int		nReturn = FALSE;
	char	szTemp[256];
	CString strTemp("");
	CString strTemp2("");

	sprintf(szTemp, "%%%d.%ds", nInLength, nInLength);			// Output Data Adjust
	strTemp2.Format(szTemp, szAmount);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], szAmount[%s], nInLength[%d], cMarkChar[%1.1c], nIsMinus[%d])", nStringNo, strTemp2.GetBuffer(0), nInLength, cMarkChar, nIsMinus);

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);

		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// Display Amount(String No, Amount, Mark, IsMinus)
int	CDevCmn::fnSCR_DisplayAmount(int nStringNo, LPCTSTR szAmount, char cMarkChar, int nIsMinus)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], szAmount[%s], cMarkChar[%1.1c], nIsMinus[%d])", nStringNo, szAmount, cMarkChar, nIsMinus);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);

		strTemp2 = szAmount;
		
		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// Display Amount(String No, Amount, Mark, IsMinus)
int	CDevCmn::fnSCR_DisplayAmount(int nStringNo, int nAmount, char cMarkChar, int nIsMinus)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(nStringNo[%d], nAmount[%d], cMarkChar[%1.1c], nIsMinus[%d])", nStringNo, nAmount, cMarkChar, nIsMinus);

	int		nReturn = FALSE;
	CString strTemp("");
	CString strTemp2("");

	if (nStringNo > 0)
	{
		strTemp.Format("%s%d", SCR_STRING, nStringNo);

		strTemp2.Format("%d", nAmount);

		strTemp2 = Asc2Amt(strTemp2, strTemp2.GetLength(), 256, cMarkChar);
		strTemp2.TrimLeft();

		if (nIsMinus)
			strTemp2 = "-" + strTemp2;							// 2003.11.27

		nReturn = fnSCR_SetDisplayData(strTemp.GetBuffer(0), strTemp2.GetBuffer(0));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnSCR_DisplayAmount(...):return(%d)", nReturn);
	return nReturn;
}

// 打开串口 2008.05.09
// 输入: pPort - 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者
//       nBaudRate - 波特率       9600
//       nParity - 奇偶校验       0
//       nByteSize - 数据字节宽度 8
//       nStopBits - 停止位       1
BOOL CDevCmn::fnAPP_OpenADComm(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// 串口控制块
	int timeout1 = 100;
	int timeout2 = 1;
	int timeout3 = 500;
	int timeout4 = 1;
	int timeout5 = 100;
	
	COMMTIMEOUTS timeouts = {	// 串口超时控制参数
	    	timeout1,			// 读字符间隔超时时间: 100 ms
			timeout2,			// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
			timeout3,			// 基本的(额外的)读超时时间: 500 ms
			timeout4,			// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
 			timeout5};			// 基本的(额外的)写超时时间: 100 ms

	if(hADComm == INVALID_HANDLE_VALUE)
	{
		hADComm = CreateFile(pPort,	// 串口名称或设备路径
			GENERIC_READ | GENERIC_WRITE,	// 读写方式
			0,				// 共享方式：独占
			NULL,			// 默认的安全描述符
			OPEN_EXISTING,	// 创建方式
			0,				// 不需设置文件属性
			NULL);			// 不需参照模板文件
		
		if(hADComm == INVALID_HANDLE_VALUE) return FALSE;		// 打开串口失败
		
		GetCommState(hADComm, &dcb);		// 取DCB
		dcb.BaudRate = nBaudRate;
		dcb.ByteSize = nByteSize;
		dcb.Parity = nParity;
		dcb.StopBits = nStopBits;
		SetCommState(hADComm, &dcb);		// 设置DCB
		SetupComm(hADComm, 4096, 1024);	// 设置输入输出缓冲区大小
		SetCommTimeouts(hADComm, &timeouts);	// 设置超时
	}
	return TRUE;
}

// 关闭串口
BOOL CDevCmn::fnAPP_CloseADComm()
{
	return CloseHandle(hADComm);
}

// 写串口
// 输入: pData - 待写的数据缓冲区指针
//       nLength - 待写的数据长度
// 返回: 实际写入的数据长度
int CDevCmn::fnAPP_WriteADComm(void* pData, int nLength)
{
	DWORD dwNumWrite;	// 串口发出的数据长度
	DWORD errcode;

	CString m_strPort =GetSprintf("\\\\.\\%s",IniGetStr(_ADMACHINE_INI, _INIKEY_ADMACHINE, _INIKEY_ADMACHINE_OPENADCOMM,"COM2"));
	//CString m_strPort = "COM1";
	if (fnAPP_OpenADComm(m_strPort,9600,NOPARITY,8,ONESTOPBIT) != FALSE)
	{
		if (WriteFile(hADComm, pData, (DWORD)nLength, &dwNumWrite, NULL))
		{
			return (int)dwNumWrite;
		}
		else
		{
			errcode=GetLastError();
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm() WriteFile GetLastError=%d", errcode);
			return -1;
		}
	}
	else
	{
		errcode=GetLastError();
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm() fnAPP_OpenADComm GetLastError=%d",errcode);
		return -2;
	}
}

