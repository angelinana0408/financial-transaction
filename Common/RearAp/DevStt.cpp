/////////////////////////////////////////////////////////////////////////////
//	DevStt.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "CmnLibIn.h"
#include "ConstDef.h"

#include "nhmwiop.h"

#include "RearApCtl.h"
#include "RearApScr.h"
#include "RearApScrChn.h"
#include "RearApEtc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	STT FUNCTION(STT) : STT SET MODE
/////////////////////////////////////////////////////////////////////////////
// Clear Error Code
int	CRearApCtrl::fnSTT_ClearErrorCode()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_ClearErrorCode()");

	int		nReturn = FALSE;

	nReturn = fnTTU_ClearErrorCode();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_ClearErrorCode():return(%d)", nReturn);
	return nReturn;
}

// Clear Key Data
int	CRearApCtrl::fnSTT_ClearKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_ClearKeyData()");

	int		nReturn = FALSE;

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_ClearKeyData():return(%d)", nReturn);
	return nReturn;
}

// Set Screen Data(Screen Data Name, Screen Data Value)
int	CRearApCtrl::fnSTT_SetScreenData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_SetScreenData(szSetDataName[%s], szSetDataValue[%s])", szSetDataName, szSetDataValue);

	int		nReturn = FALSE;

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_SetScreenData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Display Data(Display Data Name, Display Data Value)
int	CRearApCtrl::fnSTT_SetDisplayData(LPCTSTR szSetDisplayDataName, LPCTSTR szSetDisplayDataValue)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_SetDisplayData(szSetDisplayDataName[%s], szSetDisplayDataValue[%s])", szSetDisplayDataName, szSetDisplayDataValue);

	int		nReturn = FALSE;
	CString	strSetDisplayDataName(szSetDisplayDataName);
	CString	strSetDisplayDataValue(szSetDisplayDataValue);
	CString	strTemp("");
	int		i = 0;

	if (strSetDisplayDataName == SOP_SCREEN)
	{
		for (i = 0; i < Ttu.APValueArray.GetSize(); i++)
		{
			Ttu.APValueArray[i] = Ttu.APValueArrayTemp[i];
			Ttu.APStateArray[i] = Ttu.APStateArrayTemp[i];
		}

		if (Asc2Int(strSetDisplayDataValue) == 916)				// CASHVALUE : 2005.12.01
		{
			Ttu.APValueArray[5] = "000";
			Ttu.APValueArray[6] = "000";
			Ttu.APValueArray[7] = "000";
			Ttu.APValueArray[8] = "000";
			Ttu.APValueArray[9] = "000";
			Ttu.APValueArray[10] = "000";
			Ttu.APValueArray[11] = "000";
			Ttu.APValueArray[12] = "000";
		}

		Ttu.APCenterMovie = strSetDisplayDataValue;
		fnSTT_APCenterMovie();									// AP Center Movie

		for (i = 0; i < Ttu.APValueArrayTemp.GetSize(); i++)
		{
			Ttu.APValueArrayTemp[i] = "";
			Ttu.APStateArrayTemp[i] = "";
		}
	}
	else
	if (strSetDisplayDataName == SOP_TIME)
	{
		Ttu.APMaxTime = strSetDisplayDataValue;
	}
	else
	if (strSetDisplayDataName.Left(strlen(SOP_STRING)) == SOP_STRING)
	{
		strTemp = strSetDisplayDataName;
		strTemp.Replace(SOP_STRING, "");
		Ttu.APValueArrayTemp[Asc2Int(strTemp)] = strSetDisplayDataValue;
	}
	else
	if (strSetDisplayDataName.Left(strlen(SOP_BTNONOFF)) == SOP_BTNONOFF)
	{
		strTemp = strSetDisplayDataName;
		strTemp.Replace(SOP_BTNONOFF, "");
		Ttu.APStateArrayTemp[Asc2Int(strTemp)] = strSetDisplayDataValue;
	}
	else
	if (strSetDisplayDataName == SOP_UPDATE)
	{
		if (strSetDisplayDataValue == SOP_UPDATE_SCREEN)
		{
			for (i = 0; i < Ttu.APValueArray.GetSize(); i++)
			{
				Ttu.APValueArray[i] = Ttu.APValueArrayTemp[i];
				Ttu.APStateArray[i] = Ttu.APStateArrayTemp[i];
			}

			if (Asc2Int(Ttu.APCenterMovie) == 916)				// CASHVALUE : 2005.12.01
			{
				Ttu.APValueArray[5] = "000";
				Ttu.APValueArray[6] = "000";
				Ttu.APValueArray[7] = "000";
				Ttu.APValueArray[8] = "000";
				Ttu.APValueArray[9] = "000";
				Ttu.APValueArray[10] = "000";
				Ttu.APValueArray[11] = "000";
				Ttu.APValueArray[12] = "000";
			}

			Ttu.APUpdate = strSetDisplayDataValue;
			fnSTT_APUpdate();									// AP Update

			for (i = 0; i < Ttu.APValueArrayTemp.GetSize(); i++)
			{
				Ttu.APValueArrayTemp[i] = "";
				Ttu.APStateArrayTemp[i] = "";
			}
		}
		else
		if (strSetDisplayDataValue == SOP_UPDATE_MESSAGE)
		{
			Ttu.APValueArray[48] = Ttu.APValueArrayTemp[48];

			Ttu.APUpdate = strSetDisplayDataValue;
			fnSTT_APUpdate();									// AP Update

			Ttu.APValueArrayTemp[48] = "";
		}
	}
	else
	if (strSetDisplayDataName == SOP_PROGRESS)
	{
		Ttu.APProgress = strSetDisplayDataName;
		fnSTT_APProgress();										// AP Progress
	}


MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_SetDisplayData(...):return(%d)", nReturn);
	return nReturn;
}

// Set Current Language Mode
int	CRearApCtrl::fnSTT_SetCurrentLangMode(int nLangMode)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_SetCurrentLangMode(nLangMode[%d])", nLangMode);

	int		nReturn = FALSE;

	Ttu.CurrentLangMode = nLangMode;
	nReturn = nLangMode;

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_SetCurrentLangMode(...):return(%d)", nReturn);
	return nReturn;
}
													
/////////////////////////////////////////////////////////////////////////////
//	STT FUNCTION(STT) : STT GET MODE
/////////////////////////////////////////////////////////////////////////////
// Get Error Code
CString CRearApCtrl::fstrSTT_GetErrorCode()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetErrorCode()");

	CString strReturn("");

	strReturn = fstrTTU_GetErrorCode();		

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetErrorCode():return(%s)", strReturn);
	return strReturn;
}

// Get Device Status
int	CRearApCtrl::fnSTT_GetDeviceStatus()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_GetDeviceStatus()");

	static	int		nSaveReturn = FALSE;
	int		nReturn = FALSE;

	nReturn = fnTTU_GetDeviceStatus();

if (nReturn != nSaveReturn)
{
	nSaveReturn = nReturn;
	MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_GetDeviceStatus():return(%d)", nReturn);
}
	return nReturn;
}

// Get Key Data
CString	CRearApCtrl::fstrSTT_GetKeyData()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetKeyData()");

	CString strReturn("");

	strReturn = fstrSTT_APGetReadDataSet();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetKeyData():return(%s)", strReturn);
	return strReturn;
}

// Get Key String(Init Flag)
CString	CRearApCtrl::fstrSTT_GetKeyString(int nInitFlag)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetKeyString(nInitFlag[%d])", nInitFlag);

	CString strReturn("");

	if (nInitFlag)
		fnSTT_ClearKeyData();									

	strReturn = fstrSTT_GetKeyData();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetKeyString(...):return(%s)", strReturn);
	return strReturn;
}

// Get Check Screen(Screen No, Language Mode)
CString	CRearApCtrl::fstrSTT_GetCheckScreen(int nScreenNo, int nLangMode)
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetCheckScreen(nScreenNo[%d], nLangMode[%d])", nScreenNo, nLangMode);

	CString	strReturn("");

	if (nScreenNo > 0)
	{
		if ((Clerk.OpDevice == "OP_TSPL")	||
			(Clerk.OpDevice == "OP_HSPL"))
		{
			for (int i = 0; i < itemof(GuideScrTbl); i++)
			{
				if (GuideScrTbl[i].GuideScrNum == nScreenNo)
				{
					strReturn.Format("%d", nScreenNo);
					break;
				}
			}
		}
		else
		if (Clerk.OpDevice == "OP_TSPLCHN")
		{
			for (int i = 0; i < itemof(GuideScrTblChn); i++)
			{
				if (GuideScrTblChn[i].GuideScrNum == nScreenNo)
				{
					strReturn.Format("%d", nScreenNo);
					break;
				}
			}
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_GetCheckScreen(...):return(%s)", strReturn);
	return strReturn;
}

// Get Current Screen No
int	CRearApCtrl::fnSTT_GetCurrentScreenNo()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_GetCurrentScreenNo()");

	int		nReturn = FALSE;

	nReturn = Ttu.CurrentScreenNo;								

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_GetCurrentScreenNo():return(%d)", nReturn);
	return nReturn;
}

// Get Current Language Mode
int	CRearApCtrl::fnSTT_GetCurrentLangMode()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_GetCurrentLangMode()");

	int		nReturn = FALSE;

	nReturn = Ttu.CurrentLangMode;								

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_GetCurrentLangMode():return(%d)", nReturn);
	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	STT FUNCTION(STT) : STT OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize
int	CRearApCtrl::fnSTT_Initialize()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_Initialize()");

	int		nReturn = FALSE;

	nReturn = fnTTU_Initialize();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_Initialize():return(%d)", nReturn);
	return nReturn;
}

// Deinitialize
int	CRearApCtrl::fnSTT_Deinitialize()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_Deinitialize()");

	int		nReturn = FALSE;

	nReturn = fnTTU_Deinitialize();

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_Deinitialize():return(%d)", nReturn);
	return nReturn;
}

// AP Center Movie
int	CRearApCtrl::fnSTT_APCenterMovie()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APCenterMovie()");

	int		nReturn = FALSE;
	int		i = 0;

/////////////////////////////////////////////////////////////////////////////
	// FIND SCREEN NUM
	if ((Clerk.OpDevice == "OP_TSPL")	||
		(Clerk.OpDevice == "OP_HSPL"))
	{
		for (i = 0; i < itemof(GuideScrTbl); i++)
		{
			if (GuideScrTbl[i].GuideScrNum == Asc2Int(Ttu.APCenterMovie))
			{
				Ttu.Index = i;
				break;
			}
		}

		if ((Ttu.Index < 0)	||
			(Ttu.Index >= itemof(GuideScrTbl)))
			return nReturn;

/////////////////////////////////////////////////////////////////////////////
		// SCREEN DATA
		nReturn = fnTTU_ClearScreenSync();						// Clear Screen Sync
		memset(&Ttu.Data, 0, sizeof(Ttu.Data));
		memcpy(&Ttu.Temp, &GuideScrTbl[Ttu.Index], sizeof(Ttu.Temp));

/////////////////////////////////////////////////////////////////////////////
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		SplitString(GuideScrTbl[Ttu.Index].GuideScrDisplay, ":", Ttu.ScreenDisplayArray);
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		SplitString(GuideScrTbl[Ttu.Index].GuideScrAccept, ":", Ttu.ScreenAcceptArray);
		SplitString(GuideScrTbl[Ttu.Index].GuideScrAccept, ":", Ttu.GetAcceptArray);
		SplitString(GuideScrTbl[Ttu.Index].GuideScrAccept, ":", Ttu.GetDisplayArray);
		for (i = 0; i < (Ttu.GetAcceptArray.GetSize() - 1); i++)// Initialize
			Ttu.GetAcceptArray[i] = "";
		for (i = 0; i < (Ttu.GetDisplayArray.GetSize() - 1); i++)// Initialize
			Ttu.GetDisplayArray[i] = "";

/////////////////////////////////////////////////////////////////////////////
		memset(Ttu.GetPositionIndex, 0, sizeof(Ttu.GetPositionIndex));	
																// Get Position Index
		fnSTT_APScreenDataMakeGetAcceptArray();					// AP Screen Data Make Get Accept Array
		fnSTT_APScreenDataMakeGuideScrData();					// AP Screen Data Make Guide Screen Data
		Ttu.GetAuto = 0;										// Get Autometor
		if (Asc2Int(Clerk.InterfaceKind) == 4001)				// 4001:BOCOM(BJ)	: 2007.02.02
		{
			if (Asc2Int(Ttu.APCenterMovie) == 910)
				Ttu.GetPosition = Ttu.GetPositionIndex[1];		// Make Get Position
			else
				Ttu.GetPosition = Ttu.GetPositionIndex[0];		// Make Get Position
		}
		else
			Ttu.GetPosition = Ttu.GetPositionIndex[0];			// Make Get Position
		Ttu.GetPositionSave = 0;								// Get Position Save
		Ttu.GetIndex = 0;										// Get Index

		Ttu.GetAPValueSave = "";								// Get AP Value Save
		Ttu.GetData = "";										// Get Data

/////////////////////////////////////////////////////////////////////////////
		fnSTT_APNormalDisplay();								// AP Normal Display
		memcpy(&Ttu.Data, &Ttu.Temp, sizeof(Ttu.Data));			// Save

/////////////////////////////////////////////////////////////////////////////
		fnSTT_APInvertedDisplay();								// AP Inverted Display
	}
	else
	if (Clerk.OpDevice == "OP_TSPLCHN")
	{
		for (i = 0; i < itemof(GuideScrTblChn); i++)
		{
			if (GuideScrTblChn[i].GuideScrNum == Asc2Int(Ttu.APCenterMovie))
			{
				Ttu.Index = i;
				break;
			}
		}

		if ((Ttu.Index < 0)	||
			(Ttu.Index >= itemof(GuideScrTblChn)))
			return nReturn;

/////////////////////////////////////////////////////////////////////////////
		// SCREEN DATA
		nReturn = fnTTU_ClearScreenSync();						// Clear Screen Sync
		memset(&Ttu.Data, 0, sizeof(Ttu.Data));
		memcpy(&Ttu.Temp, &GuideScrTblChn[Ttu.Index], sizeof(Ttu.Temp));

/////////////////////////////////////////////////////////////////////////////
		// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
		SplitString(GuideScrTblChn[Ttu.Index].GuideScrDisplay, ":", Ttu.ScreenDisplayArray);
		// SCREEN ACCEPT(INDEX,NO,XX,YY,LENGTH,ECHOMODE,ATTRIBUTE,INPUTTYPE,CURSORON,FLUSH,AUTOEND,TIMEOUT)
		SplitString(GuideScrTblChn[Ttu.Index].GuideScrAccept, ":", Ttu.ScreenAcceptArray);
		SplitString(GuideScrTblChn[Ttu.Index].GuideScrAccept, ":", Ttu.GetAcceptArray);
		SplitString(GuideScrTblChn[Ttu.Index].GuideScrAccept, ":", Ttu.GetDisplayArray);
		for (i = 0; i < (Ttu.GetAcceptArray.GetSize() - 1); i++)// Initialize
			Ttu.GetAcceptArray[i] = "";
		for (i = 0; i < (Ttu.GetDisplayArray.GetSize() - 1); i++)// Initialize
			Ttu.GetDisplayArray[i] = "";

/////////////////////////////////////////////////////////////////////////////
		memset(Ttu.GetPositionIndex, 0, sizeof(Ttu.GetPositionIndex));	
																// Get Position Index
		fnSTT_APScreenDataMakeGetAcceptArray();					// AP Screen Data Make Get Accept Array
		fnSTT_APScreenDataMakeGuideScrData();					// AP Screen Data Make Guide Screen Data
		Ttu.GetAuto = 0;										// Get Autometor
		if (Asc2Int(Clerk.InterfaceKind) == 4001)				// 4001:BOCOM(BJ)	: 2007.02.02
		{
			if (Asc2Int(Ttu.APCenterMovie) == 910)
				Ttu.GetPosition = Ttu.GetPositionIndex[1];		// Make Get Position
			else
				Ttu.GetPosition = Ttu.GetPositionIndex[0];		// Make Get Position
		}
		else
			Ttu.GetPosition = Ttu.GetPositionIndex[0];			// Make Get Position
		Ttu.GetPositionSave = 0;								// Get Position Save
		Ttu.GetIndex = 0;										// Get Index

		Ttu.GetAPValueSave = "";								// Get AP Value Save
		Ttu.GetData = "";										// Get Data

/////////////////////////////////////////////////////////////////////////////
		fnSTT_APNormalDisplay();								// AP Normal Display
		memcpy(&Ttu.Data, &Ttu.Temp, sizeof(Ttu.Data));			// Save

/////////////////////////////////////////////////////////////////////////////
		fnSTT_APInvertedDisplay();								// AP Inverted Display
	}
	
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APCenterMovie():return(%d)", nReturn);
	return nReturn;
}

// AP Update
int	CRearApCtrl::fnSTT_APUpdate()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APUpdate()");

	int		nReturn = FALSE;

/////////////////////////////////////////////////////////////////////////////
	memset(Ttu.GetPositionIndex, 0, sizeof(Ttu.GetPositionIndex));	
																// Get Position Index
	fnSTT_APScreenDataMakeGetAcceptArray();						// AP Screen Data Make Get Accept Array
	fnSTT_APScreenDataMakeGuideScrData();						// AP Screen Data Make Guide Screen Data
	Ttu.GetAuto = 0;											// Get Autometor
	Ttu.GetPosition = Ttu.GetPositionIndex[0];					// Make Get Position
	Ttu.GetPositionSave = 0;									// Get Position Save
	Ttu.GetIndex = 0;											// Get Index

	Ttu.GetAPValueSave = "";									// Get AP Value Save
	Ttu.GetData = "";											// Get Data

/////////////////////////////////////////////////////////////////////////////
	fnSTT_APNormalDisplay();									// AP Normal Display
	memcpy(&Ttu.Data, &Ttu.Temp, sizeof(Ttu.Data));				// Save

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APUpdate():return(%d)", nReturn);
	return nReturn;
}

// AP Progress
int	CRearApCtrl::fnSTT_APProgress()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APCenterMovie()");

	int		nReturn = FALSE;

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APCenterMovie():return(%d)", nReturn);
	return nReturn;
}

// AP Screen Data Make Get Accept Array
int	CRearApCtrl::fnSTT_APScreenDataMakeGetAcceptArray()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APScreenDataMakeGetAcceptArray()");

	int		nReturn = FALSE;
	CStringArray	ScreenAcceptLineArray;
	CStringArray	ScreenDisplayLineArray;
	int				i = 0, j = 0;

	for (i = 0; i < (Ttu.ScreenAcceptArray.GetSize() - 1); i++)	// Make Get Accept Array
	{
		SplitString(Ttu.ScreenAcceptArray[i], ",", ScreenAcceptLineArray);
		if ((IsNum(ScreenAcceptLineArray[0].GetBuffer(0), ScreenAcceptLineArray[0].GetLength()))	&&
			(Asc2Int(ScreenAcceptLineArray[0]) < itemof(Ttu.GetPositionIndex)))
			Ttu.GetPositionIndex[Asc2Int(ScreenAcceptLineArray[0])] = i;
																// Make Get Position Index
		Ttu.GetAcceptArray[i] = Ttu.ScreenAcceptArray[i];
		for (j = 0; j < (Ttu.ScreenDisplayArray.GetSize() - 1); j++)
		{														// Make Get Display Array
			SplitString(Ttu.ScreenDisplayArray[j], ",", ScreenDisplayLineArray);
			if (ScreenAcceptLineArray[1] == ScreenDisplayLineArray[0])
			{
				Ttu.GetDisplayArray[i] = Ttu.ScreenDisplayArray[j];
				break;
			}
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APScreenDataMakeGetAcceptArray():return(%d)", nReturn);
	return nReturn;
}

// AP Screen Data Make Guide Screen Data
int	CRearApCtrl::fnSTT_APScreenDataMakeGuideScrData()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APScreenDataMakeGuideScrData()");

	int		nReturn = FALSE;
	CStringArray	ScreenDisplayLineArray;
	int				i = 0;
	char			szTemp[256];
	CString			strTemp("");

	for (i = 0; i < (Ttu.ScreenDisplayArray.GetSize() - 1); i++)// Make Guide Screen Data
	{
		SplitString(Ttu.ScreenDisplayArray[i], ",", ScreenDisplayLineArray);
		if (ScreenDisplayLineArray.GetSize() >= 7)
		{
			if (Asc2Int(ScreenDisplayLineArray[4]))				// LENGTH
			{
				if (Asc2Int(ScreenDisplayLineArray[0]) == 0)	// NO == "00"
				{
					sprintf(szTemp, "%%-%d.%ds", Asc2Int(ScreenDisplayLineArray[4]), Asc2Int(ScreenDisplayLineArray[4]));
					strTemp.Format(szTemp, ScreenDisplayLineArray[6]);
				}
				else											// NO != "00"
				{
					sprintf(szTemp, "%%-%d.%ds", Asc2Int(ScreenDisplayLineArray[4]), Asc2Int(ScreenDisplayLineArray[4]));
					if (ScreenDisplayLineArray[6].GetLength())	// FORMAT
						strTemp.Format(szTemp, EditString(Ttu.APValueArray[Asc2Int(ScreenDisplayLineArray[0])], ScreenDisplayLineArray[6].GetBuffer(0)));
					else
						strTemp.Format(szTemp, Ttu.APValueArray[Asc2Int(ScreenDisplayLineArray[0])]);
				}

				memcpy(&Ttu.Temp.GuideScrData[Asc2Int(ScreenDisplayLineArray[2])][Asc2Int(ScreenDisplayLineArray[1])],
							strTemp, Asc2Int(ScreenDisplayLineArray[4]));
			}
		}
	}

	if (Asc2Int(Ttu.APCenterMovie) == 901)						// OP_MODE
	{
		if (Ttu.APStateArray[1] == "off")						// STATUS UPDATE ~ VIEW JOURNAL
		{
			memset(&Ttu.Temp.GuideScrData[6][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[7][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[8][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[9][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[10][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[11][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[12][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[13][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[14][0], ' ', 20);
			memset(&Ttu.Temp.GuideScrData[6][20], ' ', 20);
		}
		if (Ttu.APStateArray[2] == "off")						// OPEN SHUTTER
		{
			memset(&Ttu.Temp.GuideScrData[7][20], ' ', 20);
		}
		if (Ttu.APStateArray[3] == "off")						// CLOSE SHUTTER
		{
			memset(&Ttu.Temp.GuideScrData[8][20], ' ', 20);
		}
		if (Ttu.APStateArray[4] == "off")						// RESET ALL
		{
			memset(&Ttu.Temp.GuideScrData[10][20], ' ', 20);
		}
		if (Ttu.APStateArray[5] == "off")						// RESET
		{
			memset(&Ttu.Temp.GuideScrData[11][20], ' ', 20);
		}
		if (Ttu.APStateArray[6] == "off")						// AS MODE
		{
			memset(&Ttu.Temp.GuideScrData[12][20], ' ', 20);
		}
		if (Ttu.APStateArray[7] == "off")						// OP MODE
		{
			memset(&Ttu.Temp.GuideScrData[13][20], ' ', 20);
		}
		if (Ttu.APStateArray[10] == "off")						// POWER OFF
		{
			memset(&Ttu.Temp.GuideScrData[14][20], ' ', 20);
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APScreenDataMakeGuideScrData():return(%d)", nReturn);
	return nReturn;
}

// AP Normal Display
int	CRearApCtrl::fnSTT_APNormalDisplay()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APNormalDisplay()");

	int		nReturn = FALSE;
	int		i = 0, nWriteY = -1;
	CString	strTemp("");

	for (i = 0; i < itemof(Ttu.Temp.GuideScrData); i++)
	{
		if (memcmp(Ttu.Data.GuideScrData[i], Ttu.Temp.GuideScrData[i], 40) == 0)
		{
			if ((nWriteY >= 0)	&&
				(strTemp != ""))
				nReturn = fnTTU_WriteAtSync(0, nWriteY, "", strTemp);
																// Write At Sync
			nWriteY = -1;
			strTemp = "";
		}
		else
		{
			if (nWriteY == -1)
				nWriteY = i;
			strTemp += GetString(Ttu.Temp.GuideScrData[i], 40, 40);
		}
	}

	if ((nWriteY >= 0)	&&
		(strTemp != ""))
		nReturn = fnTTU_WriteAtSync(0, nWriteY, "", strTemp);	// Write At Sync

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APNormalDisplay():return(%d)", nReturn);
	return nReturn;
}

// AP Inverted Display
int	CRearApCtrl::fnSTT_APInvertedDisplay()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APInvertedDisplay()");

	int		nReturn = FALSE;
	CStringArray	ScreenDisplayLineArray;
	int				i = 0;
	char			szTemp[256];
	CString			strTemp("");
	
	// SCREEN DISPLAY(NO,XX,YY,ATTRIBUTE,LENGTH,JUSTIFIER,FORMAT)
	for (i = 0; i < (Ttu.ScreenDisplayArray.GetSize() - 1); i++)
	{
		SplitString(Ttu.ScreenDisplayArray[i], ",", ScreenDisplayLineArray);
		if (ScreenDisplayLineArray.GetSize() >= 7)
		{
			if ((Asc2Int(ScreenDisplayLineArray[4]))	&&		// LENGTH
				(ScreenDisplayLineArray[3] == "INVERTED"))		// INVERTED
			{
				if (Asc2Int(ScreenDisplayLineArray[0]) == 0)	// NO == "00"
				{
					sprintf(szTemp, "%%-%d.%ds", Asc2Int(ScreenDisplayLineArray[4]), Asc2Int(ScreenDisplayLineArray[4]));
					strTemp.Format(szTemp, ScreenDisplayLineArray[6]);
				}
				else											// NO != "00"
				{
					sprintf(szTemp, "%%-%d.%ds", Asc2Int(ScreenDisplayLineArray[4]), Asc2Int(ScreenDisplayLineArray[4]));
					if (ScreenDisplayLineArray[6].GetLength())	// FORMAT
						strTemp.Format(szTemp, EditString(Ttu.APValueArray[Asc2Int(ScreenDisplayLineArray[0])], ScreenDisplayLineArray[6].GetBuffer(0)));
					else
						strTemp.Format(szTemp, Ttu.APValueArray[Asc2Int(ScreenDisplayLineArray[0])]);
				}

				if (ScreenDisplayLineArray[3] == "NORMAL")
					ScreenDisplayLineArray[3] = "";
				nReturn = fnTTU_WriteAtSync(Asc2Int(ScreenDisplayLineArray[1]), 
											Asc2Int(ScreenDisplayLineArray[2]),
											ScreenDisplayLineArray[3],
											strTemp);			// Write At Sync
			}
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APInvertedDisplay():return(%d)", nReturn);
	return nReturn;
}

// AP Get Read Data
int	CRearApCtrl::fnSTT_APGetReadAuto()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetReadAuto()");

	int		nReturn = FALSE;
	static	CString	strOpLedSave("");
	CStringArray	strOpLedArray;

	if (Clerk.OpLed != strOpLedSave)							// Op Led Control
	{
		strOpLedSave = Clerk.OpLed;
		SplitString(Clerk.OpLed, ",", strOpLedArray);
		if (MwPatchVersion > 5)
		{
			for (int i = 0; i <= 7; i++)
			{
				if (strOpLedArray.GetSize() > i)
				{
					if (strOpLedArray[i] == "ON")
						fnTTU_SetLEDSync(i, "CONTINUOUS");		// 2007.03.27
					else
						fnTTU_SetLEDSync(i, "OFF");
				}
			}
		}
		else
		{
			for (int i = 0; i <= 7; i++)
			{
				if (strOpLedArray.GetSize() > i)
				{
					if (strOpLedArray[i] == "ON")
						fnTTU_SetLEDSync(i + 1, "CONTINUOUS");	// 2007.03.27
					else
						fnTTU_SetLEDSync(i + 1, "OFF");
				}
			}
		}
	}

	if ((Ttu.GetAcceptArray.GetSize() - 1) < 1)					// Accept Not Found
	{
		Ttu.GetAuto = 9;
		nReturn = FALSE;
	}

	while (Ttu.GetAuto <= 2)
	{
		Delay_Msg(100);											// Performance Adjust
		ClerkStatusUpdate();
		if ((fnSCR_GetCurrentScreenNo() == 900)	||
			(fnSCR_GetCurrentScreenNo() == 901))
		{
			if ((Clerk.OpDevice == "OP_TSPL")		||			// 2006.01.10
				(Clerk.OpDevice == "OP_TSPLCHN")	||			// 2007.04.10
				(Clerk.OpDevice == "OP_HSPL"))
			{
				if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
				{
					if ((Clerk.OpSwitchStatus == "RUN") ||		// Run MOde Changed
						(Clerk.OpSwitchStatus == "RUNCHANGED"))
					{
						Ttu.GetAuto = 9;
						nReturn = FALSE;
					}
				}
			}
		}

		switch (Ttu.GetAuto)
		{
			case 0:
				fnSTT_APGetClear();
				fnSTT_APGetSendCmd();
				Ttu.GetAuto = 1;
				break;

			case 1:
				if (fnCMN_ScanDeviceAction(DEV_TTU, 0, EVENT_IN) == DEV_TTU)
					Ttu.GetAuto = 2;
				break;

			case 2:
				fnSTT_APGetDataSet();
				fnSTT_APGetDataDisplay();
				if (fnSTT_APGetNextCmdSet())
				{
					Ttu.GetAuto = 0;
				}
				else
				{
					Ttu.GetAuto = 9;
					nReturn = TRUE;
				}
				break;

			default:
				break;
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetReadAuto():return(%d)", nReturn);
	return nReturn;
}

// AP Get Clear
int	CRearApCtrl::fnSTT_APGetClear()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetClear()");

	int		nReturn = FALSE;
	CStringArray	ScreenAcceptLineArray;
	
	SplitString(Ttu.GetAcceptArray[Ttu.GetPosition], ",", ScreenAcceptLineArray);
	
	Ttu.GetPositionSave = Ttu.GetPosition;
	Ttu.GetIndex = Asc2Int(ScreenAcceptLineArray[1]);
	Ttu.GetAPValueSave = Ttu.APValueArray[Ttu.GetIndex];

	Ttu.APValueArray[Ttu.GetIndex] = "";

/////////////////////////////////////////////////////////////////////////////
	fnSTT_APScreenDataMakeGuideScrData();						// AP Screen Data Make Guide Screen Data

/////////////////////////////////////////////////////////////////////////////
	fnSTT_APNormalDisplay();									// AP Normal Display
	memcpy(&Ttu.Data, &Ttu.Temp, sizeof(Ttu.Data));				// Save

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetClear():return(%d)", nReturn);
	return nReturn;
}

// AP Get Send Command
int	CRearApCtrl::fnSTT_APGetSendCmd()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetSendCmd()");

	int		nReturn = FALSE;
	CStringArray	ScreenAcceptLineArray;

	SplitString(Ttu.GetAcceptArray[Ttu.GetPosition], ",", ScreenAcceptLineArray);
	if (ScreenAcceptLineArray[6] == "NORMAL")
		ScreenAcceptLineArray[6] = "";
	if ((ScreenAcceptLineArray[7] == "REALNUMERIC")		||
		(ScreenAcceptLineArray[7] == "REALNUMERIC2"))
		ScreenAcceptLineArray[7] = "NUMERIC";
	if ((ScreenAcceptLineArray[7] == "REALALPHANUMERIC")	||
		(ScreenAcceptLineArray[7] == "REALALPHANUMERIC2"))
		ScreenAcceptLineArray[7] = "ALPHANUMERIC";

	if (Asc2Int(ScreenAcceptLineArray[4]))
	{															// Read At Sync
		nReturn = fnTTU_ReadAt(Asc2Int(ScreenAcceptLineArray[2]), 
								Asc2Int(ScreenAcceptLineArray[3]),
								Asc2Int(ScreenAcceptLineArray[4]),
								ScreenAcceptLineArray[5],
								ScreenAcceptLineArray[6],
								ScreenAcceptLineArray[7],
								Asc2Int(ScreenAcceptLineArray[8]),
								Asc2Int(ScreenAcceptLineArray[9]),
								Asc2Int(ScreenAcceptLineArray[10]),
								Asc2Int(ScreenAcceptLineArray[11]));

		memset(&Ttu.Data.GuideScrData[Asc2Int(ScreenAcceptLineArray[3])][Asc2Int(ScreenAcceptLineArray[2])],
					'?', Asc2Int(ScreenAcceptLineArray[4]));
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetSendCmd():return(%d)", nReturn);
	return nReturn;
}

// AP Get Data Set
int	CRearApCtrl::fnSTT_APGetDataSet()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetDataSet()");

	int		nReturn = FALSE;
	CStringArray	ScreenAcceptLineArray;
	CString			strTemp("");

	SplitString(Ttu.GetAcceptArray[Ttu.GetPosition], ",", ScreenAcceptLineArray);

	Ttu.GetData = fstrTTU_GetReadData();						// Get Read Data
	if ((Ttu.GetData == T_CANCEL)	||
		(Ttu.GetData == T_TIMEOVER))
	{
		Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetAPValueSave;
	}
	else
	if (Ttu.GetData.GetLength() == 0)
	{
		if (ScreenAcceptLineArray[7] == "REALNUMERIC")
			Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetData;
		else
		if (ScreenAcceptLineArray[7] == "REALALPHANUMERIC")
			Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetData;
		else
		if (Ttu.GetPosition == Ttu.GetPositionIndex[0])
		{
			strTemp = "0000000000000000000000000000000000000000" + Ttu.GetData;
			strTemp = strTemp.Right(Asc2Int(ScreenAcceptLineArray[4]));
			Ttu.APValueArray[Ttu.GetIndex] = strTemp;
		}
		else
			Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetAPValueSave;
	}
	else
	{
		if (Asc2Int(ScreenAcceptLineArray[4]) == Ttu.GetData.GetLength())
			Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetData;
		else
		if ((ScreenAcceptLineArray[7] == "REALNUMERIC")		||
			(ScreenAcceptLineArray[7] == "REALNUMERIC2"))
			Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetData;
		else
		if ((ScreenAcceptLineArray[7] == "REALALPHANUMERIC")	||
			(ScreenAcceptLineArray[7] == "REALALPHANUMERIC2"))
			Ttu.APValueArray[Ttu.GetIndex] = Ttu.GetData;
		else
		{
			strTemp = "0000000000000000000000000000000000000000" + Ttu.GetData;
			strTemp = strTemp.Right(Asc2Int(ScreenAcceptLineArray[4]));
			Ttu.APValueArray[Ttu.GetIndex] = strTemp;
		}
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetDataSet():return(%d)", nReturn);
	return nReturn;
}

// AP Get Data Display
int	CRearApCtrl::fnSTT_APGetDataDisplay()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetDataDisplay()");

	int		nReturn = FALSE;

/////////////////////////////////////////////////////////////////////////////
	fnSTT_APScreenDataMakeGuideScrData();						// AP Screen Data Make Guide Screen Data

/////////////////////////////////////////////////////////////////////////////
	fnSTT_APNormalDisplay();									// AP Normal Display
	memcpy(&Ttu.Data, &Ttu.Temp, sizeof(Ttu.Data));				// Save

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetDataDisplay():return(%d)", nReturn);
	return nReturn;
}

// AP Get Next Command Set
int	CRearApCtrl::fnSTT_APGetNextCmdSet()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetNextCmdSet()");

	int		nReturn = FALSE;

	// Input Data
	if (Ttu.GetPosition != Ttu.GetPositionIndex[0])
	{
		if ((Ttu.GetData == T_CANCEL)	||
			(Ttu.GetData == T_TIMEOVER))
			Ttu.GetPosition = Ttu.GetPositionIndex[0];
		else
		{
			Ttu.GetPosition++;
			if (Ttu.GetPosition >= (Ttu.GetAcceptArray.GetSize() - 1))
				Ttu.GetPosition = 0;
		}

		nReturn = TRUE;
		return nReturn;
	}

	// Menu Choice
	if (fstrSTT_APGetReadDataSet().GetLength())
	{
		nReturn = FALSE;
	}
	else
	{
		if ((Asc2Int(Ttu.APValueArray[Ttu.GetIndex]) >= 0)	&&
			(Asc2Int(Ttu.APValueArray[Ttu.GetIndex]) <= 9))
		{
			Ttu.GetPosition = Ttu.GetPositionIndex[Asc2Int(Ttu.APValueArray[Ttu.GetIndex])];
		}
		
		nReturn = TRUE;
		return nReturn;
	}

	switch (Asc2Int(Ttu.APCenterMovie))
	{
		case 900:												// OP_STATUS
			break;

		case 901:												// OP_MODE
			if ((Ttu.GetData == T_CANCEL)	||					// 2006.01.10
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "01"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "02"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "03"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "04"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "05"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "06"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "07"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "08"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
				(Ttu.APValueArray[Ttu.GetIndex] == "09"))
				nReturn = TRUE;
//			else
//			if ((Ttu.APStateArray[1] == "off")				&&	// STATUS UPDATE ~ VIEW JOURNAL
//				(Ttu.APValueArray[Ttu.GetIndex] == "10"))
//				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[2] == "off")				&&	// OPEN SHUTTER
				(Ttu.APValueArray[Ttu.GetIndex] == "11"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[3] == "off")				&&	// CLOSE SHUTTER
				(Ttu.APValueArray[Ttu.GetIndex] == "12"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[4] == "off")				&&	// RESET ALL
				(Ttu.APValueArray[Ttu.GetIndex] == "33"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[5] == "off")				&&	// RESET
				(Ttu.APValueArray[Ttu.GetIndex] == "44"))
				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[6] == "off")				&&	// AS MODE
				(Ttu.APValueArray[Ttu.GetIndex] == "55"))
				nReturn = TRUE;
//			else
//			if ((Ttu.APStateArray[7] == "off")				&&	// OP MODE
//				(Ttu.APValueArray[Ttu.GetIndex] == "66"))
//				nReturn = TRUE;
			else
			if ((Ttu.APStateArray[10] == "off")				&&	// POWER OFF
				(Ttu.APValueArray[Ttu.GetIndex] == "99"))
				nReturn = TRUE;
			break;

		case 902:												// INPUT_PASSWORD
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[1].GetLength() >= 4)	&&		// 4-6
				(Ttu.APValueArray[1].GetLength() <= 6))
				;
			else
			{
				Ttu.APValueArray[48] = GuideMsgTbl[25][OpDeviceLangMode];
				nReturn = TRUE;
			}
			break;

		case 903:												// INPUT_PASSWORD_AGAIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[1].GetLength() >= 4)	&&		// 4-6
				(Ttu.APValueArray[1].GetLength() <= 6))
				;
			else
			{
				Ttu.APValueArray[48] = GuideMsgTbl[25][OpDeviceLangMode];
				nReturn = TRUE;
			}
			break;

		case 904:												// AS_MODE
			break;

		case 905:												// EJM
			break;

		case 906:												// APUDATE
			break;

		case 907:												// TRACECOPY
			break;

		case 908:												// VERSION
			break;

		case 909:												// WORKPARAM
			break;

		case 910:												// ADDCASH
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[Ttu.GetIndex] == "11")	||	// ADDCASH
				(Ttu.APValueArray[Ttu.GetIndex] == "13"))		// SUBCASH
			{
				// yaokq add 20081028加钞张数不可大于3000张
				int CashNumMax = 3000;
				if((Asc2Int(Ttu.APValueArray[10]) > CashNumMax)||(Asc2Int(Ttu.APValueArray[13]) >CashNumMax)||
				(Asc2Int(Ttu.APValueArray[16]) >CashNumMax)||(Asc2Int(Ttu.APValueArray[19]) >CashNumMax))
				{	
					Ttu.APValueArray[48] = GuideMsgTbl[30][OpDeviceLangMode];
					nReturn = TRUE;	
				}
				else
				if ((Asc2Int(Ttu.APValueArray[9]) * Asc2Int(Ttu.APValueArray[10])) != Asc2Int(Ttu.APValueArray[11]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[46][OpDeviceLangMode];
					nReturn = TRUE;
				}
				else
				if ((Asc2Int(Ttu.APValueArray[12]) * Asc2Int(Ttu.APValueArray[13])) != Asc2Int(Ttu.APValueArray[14]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[47][OpDeviceLangMode];
					nReturn = TRUE;
				}
				else
				if ((Asc2Int(Ttu.APValueArray[15]) * Asc2Int(Ttu.APValueArray[16])) != Asc2Int(Ttu.APValueArray[17]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[48][OpDeviceLangMode];
					nReturn = TRUE;
				}
				else
				if ((Asc2Int(Ttu.APValueArray[18]) * Asc2Int(Ttu.APValueArray[19])) != Asc2Int(Ttu.APValueArray[20]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[49][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 911:												// EJMVIEW
			break;

		case 912:												// EJMRESULT
			break;

		case 913:												// INPUT_NEW_PIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[1].GetLength() >= 4)	&&		// 4-6
				(Ttu.APValueArray[1].GetLength() <= 6))
				;
			else
			{
				Ttu.APValueArray[48] = GuideMsgTbl[55][OpDeviceLangMode];
				nReturn = TRUE;
			}
			break;

		case 914:												// INPUT_NEW_PIN_AGAIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[1].GetLength() >= 4)	&&		// 4-6
				(Ttu.APValueArray[1].GetLength() <= 6))
				;
			else
			{
				Ttu.APValueArray[48] = GuideMsgTbl[55][OpDeviceLangMode];
				nReturn = TRUE;
			}
			break;

		case 915:												// EJMCOPY
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[Ttu.GetIndex] == "11")	||	// FDCOPY
				(Ttu.APValueArray[Ttu.GetIndex] == "12"))		// USBCOPY
			{
				if (Asc2Int(Ttu.APValueArray[3]) > Asc2Int(Ttu.APValueArray[4]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[38][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 916:												// CASHVALUE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")			// CASHVALUE
			{
				if ((Asc2Int(Ttu.APValueArray[5]) != Asc2Int(Ttu.APValueArray[9]))	||
					(Asc2Int(Ttu.APValueArray[6]) != Asc2Int(Ttu.APValueArray[10]))	||
					(Asc2Int(Ttu.APValueArray[7]) != Asc2Int(Ttu.APValueArray[11]))	||
					(Asc2Int(Ttu.APValueArray[8]) != Asc2Int(Ttu.APValueArray[12])))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[50][OpDeviceLangMode];
					nReturn = TRUE;
				}

				if (Asc2Int(Ttu.APValueArray[1]))
				{
					if ((Asc2Int(Ttu.APValueArray[5]) == 10)	||
						(Asc2Int(Ttu.APValueArray[5]) == 20)	||
						(Asc2Int(Ttu.APValueArray[5]) == 50)	||
						(Asc2Int(Ttu.APValueArray[5]) == 100))
						;
					else
					{
						Ttu.APValueArray[48] = GuideMsgTbl[51][OpDeviceLangMode];
						nReturn = TRUE;
					}
				}

				if (Asc2Int(Ttu.APValueArray[2]))
				{
					if ((Asc2Int(Ttu.APValueArray[6]) == 10)	||
						(Asc2Int(Ttu.APValueArray[6]) == 20)	||
						(Asc2Int(Ttu.APValueArray[6]) == 50)	||
						(Asc2Int(Ttu.APValueArray[6]) == 100))
						;
					else
					{
						Ttu.APValueArray[48] = GuideMsgTbl[51][OpDeviceLangMode];
						nReturn = TRUE;
					}
				}

				if (Asc2Int(Ttu.APValueArray[3]))
				{
					if ((Asc2Int(Ttu.APValueArray[7]) == 10)	||
						(Asc2Int(Ttu.APValueArray[7]) == 20)	||
						(Asc2Int(Ttu.APValueArray[7]) == 50)	||
						(Asc2Int(Ttu.APValueArray[7]) == 100))
						;
					else
					{
						Ttu.APValueArray[48] = GuideMsgTbl[51][OpDeviceLangMode];
						nReturn = TRUE;
					}
				}

				if (Asc2Int(Ttu.APValueArray[4]))
				{
					if ((Asc2Int(Ttu.APValueArray[8]) == 10)	||
						(Asc2Int(Ttu.APValueArray[8]) == 20)	||
						(Asc2Int(Ttu.APValueArray[8]) == 50)	||
						(Asc2Int(Ttu.APValueArray[8]) == 100))
						;
					else
					{
						Ttu.APValueArray[48] = GuideMsgTbl[51][OpDeviceLangMode];
						nReturn = TRUE;
					}
				}
			
				int		nValue10 = 0, nValue20 = 0, nValue50 = 0, nValue100 = 0;
				if (Asc2Int(Ttu.APValueArray[5]) == 10) nValue10++;
				if (Asc2Int(Ttu.APValueArray[5]) == 20) nValue20++;
				if (Asc2Int(Ttu.APValueArray[5]) == 50) nValue50++;
				if (Asc2Int(Ttu.APValueArray[5]) == 100) nValue100++;
				if (Asc2Int(Ttu.APValueArray[6]) == 10) nValue10++;
				if (Asc2Int(Ttu.APValueArray[6]) == 20) nValue20++;
				if (Asc2Int(Ttu.APValueArray[6]) == 50) nValue50++;
				if (Asc2Int(Ttu.APValueArray[6]) == 100) nValue100++;
				if (Asc2Int(Ttu.APValueArray[7]) == 10) nValue10++;
				if (Asc2Int(Ttu.APValueArray[7]) == 20) nValue20++;
				if (Asc2Int(Ttu.APValueArray[7]) == 50) nValue50++;
				if (Asc2Int(Ttu.APValueArray[7]) == 100) nValue100++;
				if (Asc2Int(Ttu.APValueArray[8]) == 10) nValue10++;
				if (Asc2Int(Ttu.APValueArray[8]) == 20) nValue20++;
				if (Asc2Int(Ttu.APValueArray[8]) == 50) nValue50++;
				if (Asc2Int(Ttu.APValueArray[8]) == 100) nValue100++;

				if (((nValue10) && (nValue20) && (nValue50) && (nValue100))		||
					((nValue10) && (nValue20) && (nValue50))					||
					((nValue10) && (nValue20) && (nValue100))					||
					((nValue10) && (nValue50) && (nValue100))					||
					((nValue20) && (nValue50) && (nValue100))					||
					((nValue10) && (nValue50))									||
					((nValue10) && (nValue100))									||
					((nValue20) && (nValue50))									||
					((nValue20) && (nValue100)))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[51][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 917:												// EJMSEND
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[Ttu.GetIndex] == "11")	||	// EJMSENDWITH
				(Ttu.APValueArray[Ttu.GetIndex] == "12")	||	// EJMSENDALL
				(Ttu.APValueArray[Ttu.GetIndex] == "13"))		// EJMSENDUSER
			{
				if (Asc2Int(Ttu.APValueArray[3]) > Asc2Int(Ttu.APValueArray[4]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[38][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 918:												// EJMPRINT
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[Ttu.GetIndex] == "11")	||	// EJMPRINTWITH
				(Ttu.APValueArray[Ttu.GetIndex] == "12")	||	// EJMPRINTALL
				(Ttu.APValueArray[Ttu.GetIndex] == "13"))		// EJMPRINTUSER
			{
				if (Asc2Int(Ttu.APValueArray[3]) > Asc2Int(Ttu.APValueArray[4]))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[38][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 919:												// DB_INITIAL
			break;

		case 921:												// LOGIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")			// LOGIN
			{
				if (Ttu.APValueArray[2].GetLength() == 6)		// 6
					;
				else
				{
					Ttu.APValueArray[48] = GuideMsgTbl[25][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 922:												// ADMIN
			break;

		case 923:												// ADDADMIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")			// ADDADMIN
			{
				if (Ttu.APValueArray[2].GetLength() == 6)		// 6
				{
					if ((Asc2Int(Ttu.APValueArray[2]) % 111111 == 0)	||
						 (Ttu.APValueArray[2] == "123456") || (Ttu.APValueArray[2] == "654321") || 
						 (Ttu.APValueArray[2] == "234567") || (Ttu.APValueArray[2] == "765432") || 
						 (Ttu.APValueArray[2] == "345678") || (Ttu.APValueArray[2] == "876543") || 
						 (Ttu.APValueArray[2] == "456789") || (Ttu.APValueArray[2] == "987654"))
					{
						Ttu.APValueArray[48] = GuideMsgTbl[56][OpDeviceLangMode];
						nReturn = TRUE;
					}
				}
				else
				{
					Ttu.APValueArray[48] = GuideMsgTbl[55][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 924:												// DELADMIN
			break;

		case 925:												// CHANGEADMINPIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")			// CHANGEADMINPIN
			{
				if ((Ttu.APValueArray[2].GetLength() != 6)	||	// 6
					(Ttu.APValueArray[3].GetLength() != 6)	||	// 6
					(Ttu.APValueArray[4].GetLength() != 6))		// 6
				{
					Ttu.APValueArray[48] = GuideMsgTbl[55][OpDeviceLangMode];
					nReturn = TRUE;
				}
				else
				if (Ttu.APValueArray[3] != Ttu.APValueArray[4])
				{
					Ttu.APValueArray[48] = GuideMsgTbl[29][OpDeviceLangMode];
					nReturn = TRUE;
				}
				else
				if ((Asc2Int(Ttu.APValueArray[3]) % 111111 == 0)	||
					 (Ttu.APValueArray[3] == "123456") || (Ttu.APValueArray[2] == "654321") || 
					 (Ttu.APValueArray[3] == "234567") || (Ttu.APValueArray[2] == "765432") || 
					 (Ttu.APValueArray[3] == "345678") || (Ttu.APValueArray[2] == "876543") || 
					 (Ttu.APValueArray[3] == "456789") || (Ttu.APValueArray[2] == "987654"))
				{
					Ttu.APValueArray[48] = GuideMsgTbl[56][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 926:												// BATCHTOTAL
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				;
			else
			if ((Ttu.APValueArray[Ttu.GetIndex] == "11")	||	// TOTALCB
				(Ttu.APValueArray[Ttu.GetIndex] == "12"))		// TOTALIB
			{
				if ((Ttu.APValueArray[2].GetLength() >= 0)	&&	// 0-14
					(Ttu.APValueArray[2].GetLength() <= 14))
					;
				else
				{
					Ttu.APValueArray[48] = GuideMsgTbl[52][OpDeviceLangMode];
					nReturn = TRUE;
				}
			}
			break;

		case 927:												// USERDEFINE
			break;

		case 928:												// SETPARAM
			break;

		case 929:												// SETMASTERKEY
			break;

		case 932:												// INPUTAUTHCODE
			break;
			
		case 963:												//yaokq add 20081029
			break;

		default:
			break;
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fnSTT_APGetNextCmdSet():return(%d)", nReturn);
	return nReturn;
}

// AP Get Read Data Set
CString	CRearApCtrl::fstrSTT_APGetReadDataSet()
{
MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_APGetReadDataSet()");

	CString strReturn("");

	// Menu Choice
	switch (Asc2Int(Ttu.APCenterMovie)) 
	{
		case 900:												// OP_STATUS
			break;

		case 901:												// OP_MODE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "STATUSUPDATE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "OPADMIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "TECHADMIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "08")
				strReturn = "PRINTCASH";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "09")
				strReturn = "CASHVALUE";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
//				strReturn = "SPECIAL";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "OPENSHUTTER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "CLOSESHUTTER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "33")
				strReturn = "ALLRESET";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "44")
				strReturn = "RESET";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "55")
				strReturn = "ASMODE";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "66")
//				strReturn = "OPMODE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "99")
				strReturn = "POWEROFF";
			break;

		case 902:												// INPUT_PASSWORD
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
				strReturn = "CONFIRM:" + Ttu.APValueArray[1];
			break;

		case 903:												// INPUT_PASSWORD_AGAIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
				strReturn = "CONFIRM:" + Ttu.APValueArray[1];
			break;

		case 904:												// AS_MODE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "CHANGEPIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "APUPDATE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "TRACECOPY";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
                strReturn = "BACKUPEJ";                // 20080516 by hyl
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "VDM";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "VERSION";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
				strReturn = "WORKPARAM";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "08")
				strReturn = "DBINITIAL";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "09")
				strReturn = "DBPRINT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 905:												// EJM
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "EJMVIEW";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "EJMCOPY";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "EJMSEND";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "EJMPRINT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 906:												// APUDATE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "FDUPDATE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "CDROMUPDATE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "13")
				strReturn = "USBUPDATE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 907:												// TRACECOPY
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "FDCOPY:" + Ttu.APValueArray[3];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "USBCOPY:" + Ttu.APValueArray[3];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 908:												// VERSION
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "PRINT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 909:												// WORKPARAM
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "SAVE:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2] + ":"
									+ Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4] + ":"
									+ Ttu.APValueArray[5] + ":"
									+ Ttu.APValueArray[6] + ":"
									+ Ttu.APValueArray[7] + ":"
									+ Ttu.APValueArray[8] + ":"
									+ Ttu.APValueArray[9] + ":"
									+ Ttu.APValueArray[10] + ":"
									+ Ttu.APValueArray[11] + ":"
									+ Ttu.APValueArray[12] + ":"
									+ Ttu.APValueArray[13] + ":"
									+ Ttu.APValueArray[14] + ":"
									+ Ttu.APValueArray[15];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "PRINT:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2] + ":"
									+ Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4] + ":"
									+ Ttu.APValueArray[5] + ":"
									+ Ttu.APValueArray[6] + ":"
									+ Ttu.APValueArray[7] + ":"
									+ Ttu.APValueArray[8] + ":"
									+ Ttu.APValueArray[9] + ":"
									+ Ttu.APValueArray[10] + ":"
									+ Ttu.APValueArray[11] + ":"
									+ Ttu.APValueArray[12] + ":"
									+ Ttu.APValueArray[13] + ":"
									+ Ttu.APValueArray[14] + ":"
									+ Ttu.APValueArray[15];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 910:												// ADDCASH
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "ADDCASH:" + Ttu.APValueArray[10] + ":"
									+ Ttu.APValueArray[13] + ":"
									+ Ttu.APValueArray[16] + ":"
									+ Ttu.APValueArray[19];
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
//				strReturn = "CLEARCASH:" + Ttu.APValueArray[10] + ":"
//									+ Ttu.APValueArray[13] + ":"
//									+ Ttu.APValueArray[16] + ":"
//									+ Ttu.APValueArray[19];
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "13")
//				strReturn = "SUBCASH:" + Ttu.APValueArray[10] + ":"
//									+ Ttu.APValueArray[13] + ":"
//									+ Ttu.APValueArray[16] + ":"
// 									+ Ttu.APValueArray[19];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 911:												// EJMVIEW
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "VIEW:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2] + ":"
									+ Ttu.APValueArray[3];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 912:												// EJMRESULT
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "VIEW-10";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "PRINTEJM";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "13")
				strReturn = "SEARCH";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "14")
				strReturn = "VIEW-1";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "15")
				strReturn = "VIEW+1";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 913:												// INPUT_NEW_PIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
				strReturn = "CONFIRM:" + Ttu.APValueArray[1];
			break;

		case 914:												// INPUT_NEW_PIN_AGAIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
				strReturn = "CONFIRM:" + Ttu.APValueArray[1];
			break;

		case 915:												// EJMCOPY
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "FDCOPY:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "USBCOPY:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 916:												// CASHVALUE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "CASHVALUE:" + Ttu.APValueArray[5] + ":"
									+ Ttu.APValueArray[6] + ":"
									+ Ttu.APValueArray[7] + ":"
									+ Ttu.APValueArray[8];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 917:												// EJMSEND
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "EJMSENDWITH:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "EJMSENDALL:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "13")
				strReturn = "EJMSENDUSER:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 918:												// EJMPRINT
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "EJMPRINTWITH:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "EJMPRINTALL:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "13")
				strReturn = "EJMPRINTUSER:" + Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 919:												// DB_INITIAL
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "INITIAL";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 920:												// TOTALSTIP
//			if ((Ttu.GetData == T_CANCEL)	||
//				(Ttu.GetData == T_TIMEOVER))
//				strReturn = S_EXIT;
//			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "ENTER";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
//				strReturn = S_EXIT;
			break;

		case 921:												// LOGIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "LOGIN:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 922:												// ADMIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "CHANGEADMINPIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "ADDADMIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "DELADMIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 923:												// ADDADMIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "ADDADMIN:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 924:												// DELADMIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "DELADMIN:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 925:												// CHANGEADMINPIN
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "CHANGEADMINPIN:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2] + ":"
									+ Ttu.APValueArray[3];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 926:												// BATCHTOTAL
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "TOTALCB:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "TOTALIB:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 927:												// USERDEFINE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "DOWNFIT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "SETPARAM";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "SETMASTERKEY";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
//				strReturn = "DOWNPARAM";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
//				strReturn = "UPDATEMASTERKEY";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
//				strReturn = "INITMASTERKEY";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
//				strReturn = "INPUTAUTHCODE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 928:												// SETPARAM //liyi add 2008.09.23
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "SAVE:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2] + ":"
									+ Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4] + ":"
									+ Ttu.APValueArray[5] + ":"
									+ Ttu.APValueArray[6] + ":"
									+ Ttu.APValueArray[7] + ":"
									+ Ttu.APValueArray[8] + ":"
									+ Ttu.APValueArray[9] + ":"
									+ Ttu.APValueArray[10] +":"
									+ Ttu.APValueArray[11] +":"
									+ Ttu.APValueArray[12] +":"
									+ Ttu.APValueArray[13] +":"
									+ Ttu.APValueArray[14] +":"
									+ Ttu.APValueArray[15] +":"
									+ Ttu.APValueArray[16] +":"
									+ Ttu.APValueArray[17] +":"
									+ Ttu.APValueArray[18] +":"
									+ Ttu.APValueArray[19] +":"
									+ Ttu.APValueArray[20] +":"
									+ Ttu.APValueArray[21] +":"
									+ Ttu.APValueArray[22] +":"
									+ Ttu.APValueArray[23] +":"
									+ Ttu.APValueArray[24] +":"
									+ Ttu.APValueArray[25] +":"
									+ Ttu.APValueArray[26] +":"
									+ Ttu.APValueArray[27] +":"
									+ Ttu.APValueArray[28] +":"
									+ Ttu.APValueArray[29] +":"
									+ Ttu.APValueArray[30];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "12")
				strReturn = "PRINT:" + Ttu.APValueArray[1] + ":"
									+ Ttu.APValueArray[2] + ":"
									+ Ttu.APValueArray[3] + ":"
									+ Ttu.APValueArray[4] + ":"
									+ Ttu.APValueArray[5] + ":"
									+ Ttu.APValueArray[6] + ":"
									+ Ttu.APValueArray[7] + ":"
									+ Ttu.APValueArray[8] + ":"
									+ Ttu.APValueArray[9] + ":"
									+ Ttu.APValueArray[10] +":"
									+ Ttu.APValueArray[11] +":"
									+ Ttu.APValueArray[12] +":"
									+ Ttu.APValueArray[13] +":"
									+ Ttu.APValueArray[14] +":"
									+ Ttu.APValueArray[15] +":"
									+ Ttu.APValueArray[16] +":"
									+ Ttu.APValueArray[17];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 929:												// SETMASTERKEY
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "SETMASTERKEY:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 932:												// INPUTAUTHCODE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "INPUTAUTHCODE:" + Ttu.APValueArray[2];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 941:												// INPUTMASTERKEY_A
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "INPUTMASTERKEY_A:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 942:												// INPUTMASTERKEY_B
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "INPUTMASTERKEY_B:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 943:												// ADDCASHINPUTCOUNTER
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "ADDCASHINPUTCOUNTER:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 944:												// ADDCASHINPUTCOUNTER
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "TOTALINPUTAMOUNT:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;
			
		case 959:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "VIEWATMCTOTALS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "ATMCTOTALS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "ATMPTOTALS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")    //清机加钞
				strReturn = "CASHREPN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "UPLOADJOUR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "BACKUPEJ";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
				strReturn = "ATMPTOTALSKIND";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 960:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "TOTALINQURE";
//			else
//			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
//				strReturn = "CASHREPN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "CLEARRETAINEDCARD";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "CHECKSTATUS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "COUNTEROTHERFUNCTION";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 961:      //yaokq add 20081031
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "VIEWATMCTOTALS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "ATMCTOTALS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "ATMPTOTALS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")    //清机加钞
				strReturn = "CASHREPN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "UPLOADJOUR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "BACKUPEJ";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
				strReturn = "ATMPTOTALSKIND";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 962:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "MODIFYADMPIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "SHUTDOWN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "REBOOT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "INITJOURNALPTR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "INITRECEIPTPTR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "INITCARDREADER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
				strReturn = "TOTALADDRULE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 963:												//yaokq add 20081029
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
				if (Ttu.APValueArray[Ttu.GetIndex] == "12")
						strReturn = "PRINT:" + Ttu.APValueArray[1] + ":"
						+ Ttu.APValueArray[2] + ":"
						+ Ttu.APValueArray[3] + ":"
						+ Ttu.APValueArray[4] + ":"
						+ Ttu.APValueArray[5] + ":"
						+ Ttu.APValueArray[6] + ":"
						+ Ttu.APValueArray[7] + ":"
						+ Ttu.APValueArray[8] + ":"
						+ Ttu.APValueArray[9] + ":"
						+ Ttu.APValueArray[10] +":"
						+ Ttu.APValueArray[11] +":"
						+ Ttu.APValueArray[12] +":"
						+ Ttu.APValueArray[13] +":"
						+ Ttu.APValueArray[14] +":"
						+ Ttu.APValueArray[15] +":"
						+ Ttu.APValueArray[16] +":"
						+ Ttu.APValueArray[17] + ":"
						+ Ttu.APValueArray[18] + ":"
						+ Ttu.APValueArray[19] + ":"
						+ Ttu.APValueArray[20] + ":"
						+ Ttu.APValueArray[21] + ":"
						+ Ttu.APValueArray[22] + ":"
						+ Ttu.APValueArray[23] + ":"
						+ Ttu.APValueArray[24] + ":"
						+ Ttu.APValueArray[25] +":"
						+ Ttu.APValueArray[26] +":"
						+ Ttu.APValueArray[27] +":"
						+ Ttu.APValueArray[28] +":"
						+ Ttu.APValueArray[29] +":"
						+ Ttu.APValueArray[30] +":"
						+ Ttu.APValueArray[31] +":"
						+ Ttu.APValueArray[32];
					else
						if (Ttu.APValueArray[Ttu.GetIndex] == "88")
							strReturn = "EXIT";
			break;

		case 964:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "MODIFYADMPIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "SHUTDOWN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "REBOOT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "INITJOURNALPTR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "INITRECEIPTPTR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "INITCARDREADER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")	// added by liuxl 20110609
				strReturn = "RECEIPTPTRSET";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 970:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "VIEWSTATUS";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "PARAMETERSETTING";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "KEYMANAGE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "OTHERFUNCTION";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "DOWNLOADPARAM";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06") // added by yusy 08.09.09
				strReturn = "CHANGEMATERIAL";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")	// added by liuxl 20110609
				strReturn = "UNLOCKCWD";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 971:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "INITCDK";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "REQUESTCDK";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "REQUESTMAC";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "REQUESTPIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "INITEPP";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 972:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "INITJOURNALPTR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "INITRECEIPTPTR";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "INITDISPENCE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "INITCARDREADER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "HARDWAREDOCSET";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "MODIFYTECHPIN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
				strReturn = "REBOOT";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "08")
				strReturn = "SHUTDOWN";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 973:
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "DOWNLOADCONTROLPARA";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "DOWNLOADCARDTABEL";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "DOWNLOADEXTPARA";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "04")
				strReturn = "CHANGECONTROLPARA";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "05")
				strReturn = "DOWNLOADICCARDTABLE";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "06")
				strReturn = "DOWNLOADPBOCPARA";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "07")
				strReturn = "DOWNLOADAIDLIST";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "08")
				strReturn = "DOWNLOADECHANNEL";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;

		case 974:												// ADDCASH MODE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "ADDCASHMODE:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		case 975:												// added by yusy 08.09.09
			if (Ttu.APValueArray[Ttu.GetIndex] == "01")
				strReturn = "CHANGESPRPAPER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "02")
				strReturn = "CHANGEJPRPAPER";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "03")
				strReturn = "CHANGEINK";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;
		case 976:
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "INITEPP";
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "10")
				strReturn = S_EXIT;
			break;
			
		case 977:												// ADDCASH MODE
			if ((Ttu.GetData == T_CANCEL)	||
				(Ttu.GetData == T_TIMEOVER))
				strReturn = S_EXIT;
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "11")
				strReturn = "TOTALKINDSEL:" + Ttu.APValueArray[1];
			else
			if (Ttu.APValueArray[Ttu.GetIndex] == "88")
				strReturn = S_EXIT;
			break;

		default:
			break;
	}

MsgDump(TRACE_DUMP_MODE, "Rear", __FILE__, __LINE__, "fstrSTT_APGetReadDataSet():return(%s)", strReturn);
	return strReturn;
}

