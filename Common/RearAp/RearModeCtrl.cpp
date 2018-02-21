/////////////////////////////////////////////////////////////////////////////
// RearModeCtrl.cpp : Implementation of the CRearApCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include <wininet.h>

#include "CmnLibIn.h"

#include "RearAp.h"
#include "RearApCtl.h"
#include "RearApPpg.h"

#include "RearApScr.h"
#include "RearApEtc.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Clerk Mode Control
int CRearApCtrl::ClerkModeCtrl()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkModeCtrl()");

/////////////////////////////////////////////////////////////////////////////
	MwPatchVersion		= 0;									// MiddleWare Patch Version
	MwPatchVersion		= Asc2Int(RegGetStr(_REGKEY_PATCHVERSION, "VERSION", Int2Asc(MwPatchVersion, 3)));
	ApDeviceLangMode	= CHN_MODE;								// Ap Device Language Mode
	OpDeviceLangMode	= CHN_MODE;								// Op Device Language Mode

/////////////////////////////////////////////////////////////////////////////
	Scr.CurrentScreenNo = 0;									// Current Screen No
	Scr.CurrentLangMode = CHN_MODE;								// Current Language Mode

/////////////////////////////////////////////////////////////////////////////
	Ttu.APCenterMovie = "";										// AP Center Movie
	Ttu.APMaxTime = "";											// AP Max Time
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.APValueArray);
																// AP Value Array
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.APStateArray);
																// AP State Array
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.APValueArrayTemp);
																// AP Value Array Temp
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.APStateArrayTemp);
																// AP State Array Temp

	Ttu.APUpdate = "";											// AP Update
	Ttu.APProgress = "";										// AP Progress
	
	Ttu.Index = 0;												// Index
	memset(&Ttu.Data, 0, sizeof(Ttu.Data));						// Data
	memset(&Ttu.Temp, 0, sizeof(Ttu.Temp));						// Temp
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.ScreenDisplayArray);
																// Screen Display Array
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.ScreenAcceptArray);
																// Screen Accept Array

	Ttu.GetAuto = 0;											// Get Autometor
	Ttu.GetPosition = 0;										// Get Position
	memset(Ttu.GetPositionIndex, 0, sizeof(Ttu.GetPositionIndex));	
																// Get Position Index
	Ttu.GetPositionSave = 0;									// Get Position Save
	Ttu.GetIndex = 0;											// Get Index

	Ttu.GetAPValueSave = "";									// Get AP Value Save
	Ttu.GetData = "";											// Get Data
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.GetAcceptArray);
																// Get Accept Array
	SplitString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", ":", Ttu.GetDisplayArray);
																// Get Display Array
	
	Ttu.CurrentScreenNo = 0;									// Current Screen No
	Ttu.CurrentLangMode = CHN_MODE;								// Current Language Mode

/////////////////////////////////////////////////////////////////////////////
	Clerk.Doing = TRUE;											// Doing
	Clerk.PowerOffFlag = FALSE;									// Power Off Flag
	Clerk.ReadyFlag = FALSE;									// Ready Flag
	Clerk.DoorOpen11Flag = FALSE;								// Door Open 11 Flag

	Clerk.Index = 0;											// Index
	Clerk.TempIndex = 0;										// Temp Index
	Clerk.SaveIndex = 0;										// Save Index
	Clerk.AtmOpStatus = "";										// Atm Op Status
	Clerk.AtmOpMessage = "";									// Atm Op Message

	Clerk.UpdateScreen = FALSE;									// Update Screen
	Clerk.UpdateButton = FALSE;									// Update Button
	Clerk.UpdateMessage = FALSE;								// Update Message

	Clerk.AtmStatus = "";										// Atm Status
	Clerk.OpenStatus = "";										// Open Status
	Clerk.SendRecvStatus = "";									// Send Recv Status
	Clerk.TransactionStaus = "";								// Transaction Status
	Clerk.PowerStaus = "";										// Power Status
	Clerk.AsPassWord = "";										// As PassWord
	Clerk.OpPassWord = "";										// Op PassWord
	Clerk.ScrBankName = "";										// Screen Bank Name
	Clerk.MachineKind = "";										// Machine Kind
	Clerk.OpSwitchStatus = "";									// Op Switch Status
	Clerk.FtpIp = "";											// Ftp Ip
	Clerk.FtpPort = "";											// Ftp Port
	Clerk.AtmSerialNumber = "";									// AtmSerial Number
	Clerk.BranchNumber = "";									// Branch Number
	Clerk.ApVersion = "";										// Ap Version
	Clerk.TransactionDate = "";									// Transaction Date
	Clerk.InterfaceKind = "";									// Interface Kind
	Clerk.OpDevice = "";										// Op Device : 2005.11.11
	Clerk.OpLed = "";											// Op Led : 2005.12.01
	Clerk.SplDevice = "";										// Spl Device : 2006.06.02

/////////////////////////////////////////////////////////////////////////////
	Ejm.Date = "";												// Date
	Ejm.TranSerialNo = "";										// TranSerialNo
	Ejm.AccountNum = "";										// AccountNum
	
	Ejm.Count = 0;												// Count
	Ejm.FindOkFlag = FALSE;										// Find Ok Flag
	Ejm.Position = 0;											// Position
	memset(&Ejm.EjrData, 0, sizeof(Ejm.EjrData));				// Ejr Data
	
/////////////////////////////////////////////////////////////////////////////
	int iAddCashCamNo = IniGetInt(_MONITOR_INI_DAT, "CAMERA", "ADDCASH_NO", 2);
	int iMonitorCS_AddCash = -1;

/////////////////////////////////////////////////////////////////////////////
	while (Clerk.Doing)
	{
		Delay_Msg(100);											// Performance Adjust
		Clerk.TempIndex = Clerk.Index;							// Temp Index
		switch (Clerk.Index)
		{
			case 0:		
				if (ClerkStatusUpdate())						// Wait for ATM Initial End
					Clerk.Index = 1;
				break;

/////////////////////////////////////////////////////////////////////////////
			case 1:												// Clerk Select Mode Procedure
				ClerkSelectModeProc();
				break;

/////////////////////////////////////////////////////////////////////////////
			case 11:											// Clerk Op Input Pin Procedure
				ClerkOpInputPinProc();
				break;

			case 12:											// Clerk Op Mode Procedure
				ClerkOpModeProc();
				break;

			case 13:											// Clerk Op Input Change Pin Procedure
				ClerkOpInputChangePinProc();
				break;

			case 14:											// Clerk Add Cash Procedure
/////////////////////////开始添钞的拍摄   by zjd
//				int iAddCashCamNo = IniGetInt(_MONITOR_INI_DAT, "CAMERA", "ADDCASH_NO", 2);
//				int iMonitorCS_AddCash = -1;
				if (iMonitorCS_AddCash != -1)
				{
					hy_HCMonitor2_CustomerServiceEnd(iMonitorCS_AddCash);
					iMonitorCS_AddCash = -1;
				}
				iMonitorCS_AddCash = hy_HCMonitor2_CustomerServiceBegin();
				hy_HCMonitor2_StartCap(iAddCashCamNo, iMonitorCS_AddCash,
					"----", "ADD CASH", "NO");

//				if(IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA,_INIKEY_TRAN_DVRMODE,0) == 1)
// 			      	 fnAPP_DvrDataComm(CMDCODE_STARTDVR,DVRCHAN_IDX9,999,"ADD CASH");
////////////////////////////////////////////////////////
				ClerkAddCashProc();
////////////////////////////////添钞结束摄像头拍摄   by zjd
				hy_HCMonitor2_StopCap(iAddCashCamNo);

//				if(IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA,_INIKEY_TRAN_DVRMODE,0) == 1)
// 				     fnAPP_DvrDataComm(CMDCODE_ENDDVR,DVRCHAN_IDX9,5,"ADD CASH");
				break;

			case 15:											// Clerk Cash Value Procedure
				ClerkCashValueProc();							// 2005.02.24
				break;

			case 16:											// Clerk Total Procedure
				ClerkTotalProc();								// 2005.08.31
				break;

			case 17:											// Clerk Special Mode Procedure
				ClerkSpecialModeProc();							// 2005.10.25
				break;

			case 18:											// Clerk Set Param Procedure
				ClerkSetParamProc();							// 2005.10.25
				break;

			case 19:											// Clerk Set Master Key Procedure
				ClerkSetMasterKeyProc();						// 2005.10.25
				break;

			case 191:											// Clerk Code Inquiry Procedure
				ClerkCodeInquiryProc();							// 2006.08.22
				break;

			case 192:											// Clerk Input Master Key AB Procedure
				ClerkInputMasterKeyABProc();					// 2007.04.20
				break;

			case 20:											// Clerk Input Auth Code Procedure
				ClerkInputAuthCodeProc();						// 2006.07.10
				break;

/////////////////////////////////////////////////////////////////////////////
			case 21:											// Clerk As Input Pin Procedure
				ClerkAsInputPinProc();
				break;

			case 22:											// Clerk As Mode Procedure
				ClerkAsModeProc();
				break;

			case 23:											// Clerk As Input Change Pin Procedure
				ClerkAsInputChangePinProc();
				break;

			case 24:											// Clerk Ap Update Procedure
				ClerkApUpdateProc();
				break;

			case 25:											// Clerk Trace Copy Procedure
				ClerkTraceCopyProc();
				break;

			case 26:											// Clerk Version Procedure
				ClerkVersionProc();
				break;

			case 27:											// Clerk Work Parameter Procedure
				ClerkWorkParamProc();
				break;

			case 28:											// Clerk DB Initialization Procedure
				ClerkDBInitialProc();
				break;

			case 29:											// Clerk Other Operation Procedure
				ClerkOtherOperationProc();
				break;

			case 291:											// Clerk Read Ic Master Key Procedure
				ClerkReadIcMasterKeyProc();
				break;

/////////////////////////////////////////////////////////////////////////////
			case 31:											// Clerk Administrator Login Procedure
				ClerkAdminLoginProc();
				break;

			case 32:											// Clerk Super Administrator Mode Procedure
				ClerkSuperAdminModeProc();
				break;

			case 33:											// Clerk Add Administrator Procedure
				ClerkAddAdminProc();
				break;

			case 34:											// Clerk Delete Administrator Procedure
				ClerkDelAdminProc();
				break;

			case 35:											// Clerk Change Administrator Pin Procedure
				ClerkChangeAdminPinProc();
				break;

/////////////////////////////////////////////////////////////////////////////
			case 41:											// Clerk Ejm Procedure
				ClerkEjmProc();
				break;

			case 42:											// Clerk Ejm View Procedure
				ClerkEjmViewProc();
				break;

			case 43:											// Clerk Ejm Result Procedure
				ClerkEjmResultProc();
				break;

			case 44:											// Clerk Ejm Copy Procedure
				ClerkEjmCopyProc();
				break;

			case 45:											// Clerk Ejm Send Procedure
				ClerkEjmSendProc();
				break;

			case 46:											// Clerk Ejm Print Procedure
				ClerkEjmPrintProc();
				break;

			case 60:											// Clerk Post Count Menu
				ClerkPostCountMenuProc();
				break;

			case 61:
				ClerkTotalInqureProc();
				break;

			case 62:
				ClerkCounterOtherProc();
				break;

			case 63:
				ClerkPostEJCopyProc();
				break;

			case 64:
				ClerkModifyAdmPinProc();
				break;
			case 65:											// yaokq add 20081029
				ClerkPostViewAtmcTotalProc();
				break;

			case 69:
				ClerkOpAdmInputPinProc();
				break;

			case 70:											// Clerk Post TECH Menu
				ClerkPostTechMenuProc();
				break;

			case 71:
				ClerkKeyManageProc();
				break;

			case 72:
				ClerkOtherFunctionProc();
				break;

			case 73:
				ClerkDownloadParamProc();
				break;

			case 74:
				ClerkTechModifyAdmPinProc();
				break;

			case 75:
				ClerkTechAddCashModeProc();
				break;

			case 76:								// added by yusy 08.09.09
				ClerkChangeMaterial();
				break;

			case 77:
				ClerkInitEPP();						// added by yusy 2010.03.22
				break;

			case 78:
				ClerkTotalKindProc();
				break;

			case 79:
				ClerkTechAdmInputPinProc();
				break;

/////////////////////////////////////////////////////////////////////////////
			default:
				break;
		}
		Clerk.SaveIndex = Clerk.TempIndex;						// Save Index
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkModeCtrl():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk Select Mode Procedure
int CRearApCtrl::ClerkSelectModeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSelectModeProc()");

	int				nScrNum = 0;
	int				nFirstFlag = TRUE;
	int				nInitFlag = TRUE;
	int				nFirstUpdateBtnFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	
	Clerk.DoorOpen11Flag = FALSE;								// 2006.07.31

//	if (RegGetStr(_REGKEY_ADMINPIN, ZERO2).GetLength() >= 1)
//		;
//	else
//	if ((Clerk.OpPassWord == ZERO4)		||						// 2004.11.05
//		(Clerk.OpPassWord == ZERO5)		||		
//		(Clerk.OpPassWord == ZERO6)		||		
//		(Clerk.OpPassWord == ZERO7)		||		
//		(Clerk.OpPassWord == ZERO8))
//	{
		Clerk.Index = 12;										// Clerk Op Mode Procedure
		return TRUE;
//	}
	
	nScrNum = 901;												// Display Select Mode

	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();	
			if (Clerk.SplDevice == "SPL_ONOFFSW")				// 2006.06.02
			{
				if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
				{
					if ((Clerk.OpSwitchStatus == "RUN") ||
						(Clerk.OpSwitchStatus == "RUNCHANGED"))
					{
						Clerk.ReadyFlag = FALSE;				// 2005.05.26
						SetShareData("OP_COMMAND", "OP_RUN");	// OP_RUN
						SetShareData("OP_RESULT", OP_DO);
						ClerkWaitResult();						// Clerk Wait Result
						return TRUE;
					}
					else										// 2009.03.27 (A)
					{
						if (Clerk.OpDevice == "OP_TOUCH")
						{
							Clerk.ReadyFlag = TRUE;
							if (nFirstUpdateBtnFlag)
							{
								nFirstUpdateBtnFlag = FALSE;
								Clerk.UpdateButton = TRUE;
							}
						}
					}
				}
			}

			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateButton)
			{
				Clerk.UpdateButton = FALSE;
				nInitFlag = TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nFirstFlag == TRUE)									||
				(nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				ClerkSetButton(Clerk.AtmOpStatus);
				fnSCR_DisplayImage(1, FALSE);					// F1 Disabled
				fnSCR_DisplayImage(2, FALSE);					// F2 Disabled
				fnSCR_DisplayImage(3, FALSE);					// F3 Disabled
				if ((Clerk.OpDevice == "OP_TSPL")		||		// 2006.01.10
					(Clerk.OpDevice == "OP_TSPLCHN")	||		// 2007.04.10
					(Clerk.OpDevice == "OP_HSPL"))
				{
					if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
						nScrNum = 901;							// OP_MODE
					else
						nScrNum = 900;							// OP_STATUS

//					if (Ttu.APStateArrayTemp[7] == "on")
//						nScrNum = 901;							// OP_MODE
//					else
//						nScrNum = 900;							// OP_STATUS
//					if ((Clerk.OpSwitchStatus == "RUN") ||
//						(Clerk.OpSwitchStatus == "RUNCHANGED"))
//						nScrNum = 900;							// OP_STATUS
//					else
//						nScrNum = 901;							// OP_MODE
				}
				if (nFirstFlag)
				{
					nFirstFlag = FALSE;
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
				}
				else
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			if (Clerk.SplDevice == "SPL_ONOFFSW")				// 2006.06.02
				;
			else
			{
				Clerk.ReadyFlag = FALSE;						// 2005.05.26
				SetShareData("OP_COMMAND", "OP_RUN");			// OP_RUN
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
		}
		else
		if (strReturn == "ALLRESET")
		{														// Reset Message
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_ALLRESET");			// OP_ALLRESET
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "RESET")
		{														// Reset Message
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_RESET");				// OP_RESET
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "ASMODE")
		{
			Clerk.Index = 21;									// Clerk As Input Pin Procedure
			break;
		}
		else
		if (strReturn == "OPMODE")
		{
			Clerk.Index = 11;									// Clerk Op Input Pin Procedure
			break;
		}
		else
		if (strReturn == "RUNMODE")
		{
			Clerk.ReadyFlag = TRUE;								// 2005.05.26
			SetShareData("OP_COMMAND", "OP_READY");				// OP_READY
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "APMODE")
		{
			if (Clerk.SplDevice == "SPL_ONOFFSW")				// 2006.06.02
				;
			else
			{
				Clerk.ReadyFlag = FALSE;						// 2005.05.26
				SetShareData("OP_COMMAND", "OP_RUN");			// OP_RUN
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
		}
		else
		if (strReturn == "POWEROFF")
		{
			SetShareData("OP_COMMAND", "OP_POWEROFF");			// OP_POWEROFF
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSelectModeProc():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk Op Input Pin Procedure
int	CRearApCtrl::ClerkOpInputPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpInputPinProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	if (RegGetStr(_REGKEY_ADMINPIN, ZERO2).GetLength() >= 1)
	{
		Clerk.Index = 31;										// Clerk Administrator Login Procedure
		return TRUE;
	}

	if ((Clerk.OpPassWord == ZERO4)		||						// 2004.11.05
		(Clerk.OpPassWord == ZERO5)		||		
		(Clerk.OpPassWord == ZERO6)		||		
		(Clerk.OpPassWord == ZERO7)		||		
		(Clerk.OpPassWord == ZERO8))
	{
		Clerk.Index = 12;										// Clerk Op Mode Procedure
		return TRUE;
	}

	nScrNum = 902;												// Display Input Pin
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 1;									// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == Clerk.OpPassWord))
			{
				Clerk.Index = 12;								// Clerk Op Mode Procedure
				break;
			}
			else
			{
				nScrNum = 903;									// Display Input Pin Again
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}													// PassWord Error Message
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpInputPinProc():return");
	return TRUE;
}

// Clerk Op Mode Procedure
int	CRearApCtrl::ClerkOpModeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpModeProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	int				nFirstUpdateBtnFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	
	nScrNum = 901;												// Display Op Mode
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.SplDevice == "SPL_ONOFFSW")				// 2006.06.02
			{
				if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
				{
					if ((Clerk.OpSwitchStatus == "RUN") ||
						(Clerk.OpSwitchStatus == "RUNCHANGED"))
					{
						Clerk.ReadyFlag = FALSE;				// 2005.05.26
						SetShareData("OP_COMMAND", "OP_RUN");	// OP_RUN
						SetShareData("OP_RESULT", OP_DO);
						ClerkWaitResult();						// Clerk Wait Result

						Clerk.Index = 1;						// Clerk Select Mode Procedure
						return TRUE;
					}
					else										// 2009.03.27 (A)
					{
						if (Clerk.OpDevice == "OP_TOUCH")
						{
							Clerk.ReadyFlag = TRUE;
							if (nFirstUpdateBtnFlag)
							{
								nFirstUpdateBtnFlag = FALSE;
								Clerk.UpdateButton = TRUE;
							}
						}
					}
				}
			}

			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateButton)								// 2005.05.31
			{
				Clerk.UpdateButton = FALSE;
				nInitFlag = TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}
			
			if (!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TOTALKINDSET", 1))
				strTempClerkMessage = GuideMsgTbl[63][OpDeviceLangMode];

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				ClerkSetButton(Clerk.AtmOpStatus);
				if ((Clerk.OpDevice == "OP_TSPL")		||		// 2006.01.10
					(Clerk.OpDevice == "OP_TSPLCHN")	||		// 2007.04.10
					(Clerk.OpDevice == "OP_HSPL"))
				{
					if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
						nScrNum = 901;							// OP_MODE
					else
						nScrNum = 900;							// OP_STATUS

//					if (Ttu.APStateArrayTemp[7] == "on")
//						nScrNum = 901;							// OP_MODE
//					else
//						nScrNum = 900;							// OP_STATUS
//					if ((Clerk.OpSwitchStatus == "RUN") ||
//						(Clerk.OpSwitchStatus == "RUNCHANGED"))
//						nScrNum = 900;							// OP_STATUS
//					else
//						nScrNum = 901;							// OP_MODE
				}
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			if (Clerk.SplDevice == "SPL_ONOFFSW")				// 2006.06.02
				;
			else
			{
				Clerk.ReadyFlag = FALSE;						// 2005.05.26
				SetShareData("OP_COMMAND", "OP_RUN");			// OP_RUN
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result

				Clerk.Index = 1;								// Clerk Select Mode Procedure
			}
			break;
		}
		else
		if (strReturn == "OPADMIN")
		{														// Processing Message
				Clerk.Index = 69;								// Clerk Total Procedure
				break;
		}
		else
		if (strReturn == "TECHADMIN")
		{
				Clerk.Index = 79;								// Clerk Total Procedure
				break;
		}
		else
		if (strReturn == "ADDCASH")
		{
			if (Asc2Int(Clerk.InterfaceKind) == 18)				// 18:CUP(BJGD)	ONLY : 2006.07.31
			{
				if (Clerk.DoorOpen11Flag == TRUE)
				{
					Clerk.Index = 14;							// Clerk Add Cash Procedure
					break;
				}
				else
				{												// Door Open Message : 2006.07.31
					strTempClerkMessage = GuideMsgTbl[53][OpDeviceLangMode];
				}
			}
			else
			{
				Clerk.Index = 14;								// Clerk Add Cash Procedure
				break;
			}
		}
		else
		if (strReturn == "PRINTCASH")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_PRINTCASH");			// OP_PRINTCASH
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "CASHVALUE")							// 2005.02.24
		{
			Clerk.Index = 15;									// Clerk Cash Value Procedure
			break;
		}
		else
		if (strReturn == "RQK")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_RQK");				// OP_RQK
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "DOWNFIT")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNFIT");			// OP_DOWNFIT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result : 2004.11.05
		}
		else
		if (strReturn == "SPECIAL")
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			break;
		}
		else
		if (strReturn == "CHANGEPIN")
		{
			if (RegGetStr(_REGKEY_ADMINPIN, ZERO2).GetLength() >= 1)
				;
			else
			{
				Clerk.Index = 13;								// Clerk Op Input Change Pin Procedure
				break;
			}
		}
		else
		if (strReturn == "EJM")
		{
			Clerk.Index = 41;									// Clerk Ejm Result Procedure
			break;
		}
		else
		if (strReturn == "OPENSHUTTER")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_OPENSHUTTER");		// OP_OPENSHUTTER
			SetShareData("OP_RESULT", OP_DO);

			if (ClerkWaitResult() == TRUE)						// Clerk Wait Result
				Clerk.DoorOpen11Flag = TRUE;					// 2006.07.31
		}
		else
		if (strReturn == "CLOSESHUTTER")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_CLOSESHUTTER");		// OP_CLOSESHUTTER
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "ALLRESET")
		{														// Reset Message
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_ALLRESET");			// OP_ALLRESET
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "RESET")
		{														// Reset Message
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_RESET");				// OP_RESET
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "ASMODE")
		{
			Clerk.Index = 21;									// Clerk As Input Pin Procedure
			break;
		}
		else
		if (strReturn == "OPMODE")
		{
			Clerk.Index = 11;									// Clerk Op Input Pin Procedure
			break;
		}
		else
		if (strReturn == "RUNMODE")
		{
			Clerk.ReadyFlag = TRUE;								// 2005.05.26
			SetShareData("OP_COMMAND", "OP_READY");				// OP_READY
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "APMODE")
		{
			if (Clerk.SplDevice == "SPL_ONOFFSW")				// 2006.06.02
				;
			else
			{
				Clerk.ReadyFlag = FALSE;						// 2005.05.26
				SetShareData("OP_COMMAND", "OP_RUN");			// OP_RUN
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result

				Clerk.Index = 1;								// Clerk Select Mode Procedure
			}
			break;
		}
		else
		if (strReturn == "POWEROFF")
		{
			SetShareData("OP_COMMAND", "OP_POWEROFF");			// OP_POWEROFF
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// 2006.08.22
		if (strReturn == "CODEINQUIRY")
		{
			Clerk.Index = 191;									// Clerk Code Inquiry Procedure
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpModeProc():return");
	return TRUE;
}

// Clerk Op Input Change Pin Procedure
int	CRearApCtrl::ClerkOpInputChangePinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpInputChangePinProc()");

	int				nScrNum = 0;
	int				RetryCnt = 3;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strFirstPassWord("");
	CString			strSecondPassWord("");

	while (RetryCnt)
	{
		nScrNum = 913;											// Display Input Pin
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 12;									// Clerk Op Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strFirstPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 12;								// Clerk Op Mode Procedure
				break;
			}
		}

/////////////////////////////////////////////////////////////////////////////
		nScrNum = 914;											// Display Input Pin Again
		nInitFlag = TRUE;
		strTempClerkInformation = "";
		strSaveClerkInformation = "";
		strTempClerkMessage = "";
		strSaveClerkMessage = "";

		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 12;									// Clerk Op Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strSecondPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 12;								// Clerk Op Mode Procedure
				break;
			}
		}

		if(strFirstPassWord == strSecondPassWord)
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			strTemp = "OP_CHANGEOPPIN||" + strFirstPassWord;
			SetShareData("OP_COMMAND", strTemp);				// OP_CHANGEOPPIN
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
			
			fnSCR_DisplayString(48, Clerk.AtmOpMessage);
			fnSCR_DisplayMessage();
			Delay_Msg(2000);

			Clerk.Index = 12;									// Clerk Op Mode Procedure
			break;
		}
		else
		{
			if (!--RetryCnt)
			{													// Operation Miss Error Message
				strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 12;								// Clerk Op Mode Procedure
				break;
			}
			else
			{													// PassWord Missmatch
				strTempClerkMessage = GuideMsgTbl[29][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpInputChangePinProc():return");
	return TRUE;
}

// Clerk Add Cash Procedure
int	CRearApCtrl::ClerkAddCashProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAddCashProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strCounterNum("");

	nScrNum = 943;												// Input Addcash Counter Number  by zjd
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}
			
			if ((nInitFlag == TRUE)										|| //2008.4.20 add
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
 				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
 					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}
		
		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Op Mode Procedure or Post Menu
			return TRUE;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
			{
				strCounterNum.Format("%s", strTempArray[1]);
				break;
			}
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);
				
				Clerk.Index = Clerk.SaveIndex;					// Clerk Op Mode Procedure
				return TRUE;
			}
		}
	}
			
			
	nScrNum = 910;												// Display Add Cash
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (Asc2Int(Clerk.InterfaceKind) == 1005)		// 1005:CB(XA) ONLY : 2006.08.22
					fnSCR_DisplayImage(3, FALSE);				// F3 Disabled(SubCash)
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Op Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 5) &&
				(strTempArray[0] == "ADDCASH"))
			{													// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strTemp =  "OP_ADDCASH||";
				strTemp += strCounterNum + "||";
				strTemp += strTempArray[1] + "||";
				strTemp += strTempArray[2] + "||";
				strTemp += strTempArray[3] + "||";
				strTemp += strTempArray[4];

				SetShareData("OP_COMMAND", strTemp);			// OP_ADDCASH
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				if (Asc2Int(Clerk.InterfaceKind) == 4001)		// 4001:BOCOM(BJ)	: 2007.02.02
				{
					fnSCR_DisplayString(48, Clerk.AtmOpMessage);
					fnSCR_DisplayMessage();
					Delay_Msg(2000);
					Clerk.Index = Clerk.SaveIndex;				// Clerk Op Mode Procedure
					break;
				}
			}
			else
			if ((strTempArray.GetSize() >= 5) &&				// 2005.05.26
				(strTempArray[0] == "CLEARCASH"))
			{													// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strTemp =  "OP_CLEARCASH||";
				strTemp += strCounterNum + "||";
				strTemp += strTempArray[1] + "||";
				strTemp += strTempArray[2] + "||";
				strTemp += strTempArray[3] + "||";
				strTemp += strTempArray[4];

				SetShareData("OP_COMMAND", strTemp);			// OP_CLEARCASH
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
			else
			if ((strTempArray.GetSize() >= 5) &&				// 2005.08.31
				(strTempArray[0] == "SUBCASH"))
			{													// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strTemp =  "OP_SUBCASH||";
				strTemp += strCounterNum + "||";
				strTemp += strTempArray[1] + "||";
				strTemp += strTempArray[2] + "||";
				strTemp += strTempArray[3] + "||";
				strTemp += strTempArray[4];

				SetShareData("OP_COMMAND", strTemp);			// OP_SUBCASH
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
			else												// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAddCashProc():return");
	return TRUE;
}

// Clerk Total Kind Procedure
int	CRearApCtrl::ClerkTotalKindProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalKindProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strCounterNum("");

	nScrNum = 943;												// Input Addcash Counter Number  by zjd
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}
			
			if ((nInitFlag == TRUE)										|| //2008.4.20 add
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
 				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
 					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}
		
		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Op Mode Procedure or Post Menu
			return TRUE;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
			{
				strCounterNum.Format("%s", strTempArray[1]);
				break;
			}
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);
				
				Clerk.Index = Clerk.SaveIndex;					// Clerk Op Mode Procedure
				return TRUE;
			}
		}
	}
			
			
	nScrNum = 977;												// Display Input Pin
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}
			
			strTempClerkInformation = IniGetStr(_TRANSACTION_INI, "TRANSACTION", "ATMPTOTALKIND", "1");
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}
			
			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}
		
		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
			{
				if ((strTempArray[1] == "0") || (strTempArray[1] == "1"))
				{
					if (RegGetStr(_REGKEY_ADDCASHMODE, ZERO2, "0") == "1"	&&
						strTempArray[1] == "0"								&&
						Asc2Int(IniGetStr(_TRANSACTION_INI,"TRANSACTION","TOTALADDRULESET","1")))
					{
//						strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
						strTempClerkMessage = "请先修改加钞方式!";
						fnSCR_DisplayString(48, strTempClerkMessage);
						fnSCR_DisplayMessage();
						Delay_Msg(2000);
						
						Clerk.Index = Clerk.SaveIndex;					// Clerk Special Mode Procedure
						break;
					}
					strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
					strSaveClerkMessage = strTempClerkMessage;
					fnSCR_DisplayString(48, strTempClerkMessage);
					fnSCR_DisplayMessage();
					strTempClerkMessage = "";
					
					strTemp =  "OP_TOTALKIND||";
					strTemp += strCounterNum + "||";
					strTemp += strTempArray[1];
					
					SetShareData("OP_COMMAND", strTemp);			// OP_ADDCASH
					SetShareData("OP_RESULT", OP_DO);
					ClerkWaitResult();								// Clerk Wait Result

					fnSCR_DisplayString(48, Clerk.AtmOpMessage);
					fnSCR_DisplayMessage();
					Delay_Msg(5000);
					
					Clerk.Index = Clerk.SaveIndex;						// Clerk Special Mode Procedure
					break;
				}
			}
			else
			{
				strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);
				
				Clerk.Index = Clerk.SaveIndex;					// Clerk Special Mode Procedure
				break;
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalKindProc():return");
	return TRUE;
}

// Clerk Add Cash Procedure
int	CRearApCtrl::ClerkCashValueProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkCashValueProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");

	nScrNum = 916;												// Display Cash Value
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)								// 2006.07.07
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_REBOOT");		// OP_REBOOT
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
			Clerk.Index = Clerk.SaveIndex;						// Clerk As Mode Procedure or Clerk Op Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 5)
			{													// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strTemp =  "OP_CASHVALUE||";
				strTemp += strTempArray[1] + "||";
				strTemp += strTempArray[2] + "||";
				strTemp += strTempArray[3] + "||";
				strTemp += strTempArray[4];

				SetShareData("OP_COMMAND", strTemp);			// OP_CASHVALUE
				SetShareData("OP_RESULT", OP_DO);
				if (ClerkWaitResult() == TRUE)					// Clerk Wait Result : 2008.04.10
					Clerk.PowerOffFlag = TRUE;
			}
			else												// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkCashValueProc():return");
	return TRUE;
}

// Clerk Total Procedure
int	CRearApCtrl::ClerkTotalProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");

	nScrNum = 926;												// Display Total
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 12;									// Clerk Op Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 3) &&				// 2005.08.31
				(strTempArray[0] == "TOTALCB"))
			{													// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strTemp =  "OP_TOTALCB||";
				strTemp += strTempArray[1] + "||";
				strTemp += strTempArray[2];

				SetShareData("OP_COMMAND", strTemp);			// OP_TOTALCB
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
			else
			if ((strTempArray.GetSize() >= 3) &&				// 2005.08.31
				(strTempArray[0] == "TOTALIB"))
			{													// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strTemp =  "OP_TOTALIB||";
				strTemp += strTempArray[1] + "||";
				strTemp += strTempArray[2];

				SetShareData("OP_COMMAND", strTemp);			// OP_TOTALIB
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
			else												// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalProc():return");
	return TRUE;
}

// Clerk Special Mode Procedure
int	CRearApCtrl::ClerkSpecialModeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSpecialModeProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;
	FILE*			TempFp;
	char			szBuff[256];

	nScrNum = 927;												// Display Special Mode
	if (Asc2Int(Clerk.InterfaceKind) == 18)						// 18:CUP(BJGD)	 : 2006.07.07
		nScrNum = 927;
	else
	if (Asc2Int(Clerk.InterfaceKind) == 1005)					// 1005:CB(XA)	 : 2006.08.22
		nScrNum = 934;

	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (Asc2Int(Clerk.InterfaceKind) == 4001)		// 4001:BOCOM(BJ)	: 2007.02.02
					;
				else
					fnSCR_DisplayImage(4, FALSE);				// F4 Disabled(DownParam)

				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				//SetShareData("OP_COMMAND", "OP_POWEROFF");	// OP_POWEROFF				
				SetShareData("OP_COMMAND", "OP_REBOOT");		// 修改单次存款最大张数，要重启系统 change by liyi 2008.05.14
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				if ((Asc2Int(Clerk.InterfaceKind) == 18)	||	// 18:CUP(BJGD)	 : 2006.07.07
					(Asc2Int(Clerk.InterfaceKind) == 1005))		// 1005:CB(XA)	 : 2006.08.22
					Clerk.Index = 29;							// Clerk Other Operation Procedure
				else
					Clerk.Index = 12;							// Clerk Op Mode Procedure
			}
			break;
		}
		else
		if (strReturn == "DOWNFIT")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNFIT");			// OP_DOWNFIT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result : 2004.11.05
		}
		else
		if (strReturn == "SETPARAM")
		{
			Clerk.Index = 18;									// Clerk Set Param Procedure
			break;
		}
		else
		if (strReturn == "SETMASTERKEY")
		{
			if (Asc2Int(Clerk.InterfaceKind) == 20)				// 20:CUP(SHGD)	 : 2006.09.22
			{
				strDriveNum = ClerkCheckDriveIsInstall(8);
				if (strDriveNum == "")
				{
					strTempClerkMessage = GuideMsgTbl[31][OpDeviceLangMode];
				}
				else
				if (!ClerkCheckDiskIsInsert(strDriveNum))
				{
					strTempClerkMessage = GuideMsgTbl[32][OpDeviceLangMode];
				}
				else
				{
					strKeyFilePathName = strDriveNum + GetSprintf("%8.8s.TXT", Clerk.AtmSerialNumber);
					if (CFile::GetStatus(strKeyFilePathName, TempFileStatus))
					{											// File Check

						TempFp = fopen(strKeyFilePathName, "r");
						if (TempFp)
						{
							memset(szBuff, 0, sizeof(szBuff));
							if (fgets(szBuff, 250, TempFp))
							{
								strMasterKey.Format("%s", szBuff);
								strMasterKey.TrimLeft();
								strMasterKey.TrimRight();
							}
						}
						
						if ((strMasterKey.GetLength() == 16)	||
							(strMasterKey.GetLength() == 32))
						{
							Clerk.PowerOffFlag = TRUE;			// 2007.03.27
							strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
							strSaveClerkMessage = strTempClerkMessage;
							fnSCR_DisplayString(48, strTempClerkMessage);
							fnSCR_DisplayMessage();
							strTempClerkMessage = "";
							
							strTemp = "OP_USBSETMASTERKEY||" + strMasterKey;
							SetShareData("OP_COMMAND", strTemp);
							SetShareData("OP_RESULT", OP_DO);
							ClerkWaitResult();					// Clerk Wait Result : 2006.09.22
						}
						else
						{
							strTempClerkMessage = GuideMsgTbl[54][OpDeviceLangMode];
						}
					}
					else
					{
						strTempClerkMessage = GuideMsgTbl[37][OpDeviceLangMode];
					}
				}
			}
			else
			if ((Asc2Int(Clerk.InterfaceKind) == 4002)	||		// 4002:ICBC(SY)	 : 2007.04.20
				(Asc2Int(Clerk.InterfaceKind) == 2500)	||		// 2500:POST-TY(HNC)		 2007.05.15 : ATMU   长连接
				(Asc2Int(Clerk.InterfaceKind) == 2501))			// 2501:POST-TY(HN)			 2007.05.15 : ATMU   短连接
			{
				Clerk.Index = 192;								// Clerk Input Master Key Procedure
				break;
			}
			else
			{
				Clerk.Index = 19;								// Clerk Set Master Key Procedure
				break;
			}
		}
//		else													// 2006.08.22
//		if (strReturn == "INPUTAUTHCODE")
//		{
//			Clerk.Index = 20;									// Clerk Input Auth Code Procedure
//			break;
//		}
		else													// 2006.08.22
		if (strReturn == "READICMASTERKEY")
		{
			Clerk.Index = 291;									// Clerk Read Ic Master Key Procedure
			break;
		}
		else													// 2006.08.22
		if (strReturn == "DOWNPARAM")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNPARAM");			// OP_DOWNPARAM
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result : 2004.11.05
		}
//		else
//		if (strReturn == "UPDATEMASTERKEY")
//		{
//			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
//			strSaveClerkMessage = strTempClerkMessage;
//			fnSCR_DisplayString(48, strTempClerkMessage);
//			fnSCR_DisplayMessage();
//			strTempClerkMessage = "";
//			
//			SetShareData("OP_COMMAND", "OP_UPDATEMASTERKEY");	// OP_UPDATEMASTERKEY
//			SetShareData("OP_RESULT", OP_DO);
//			ClerkWaitResult();									// Clerk Wait Result : 2004.11.05
//		}
//		else
//		if (strReturn == "INITMASTERKEY")
//		{
//			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
//			strSaveClerkMessage = strTempClerkMessage;
//			fnSCR_DisplayString(48, strTempClerkMessage);
//			fnSCR_DisplayMessage();
//			strTempClerkMessage = "";
//			
//			SetShareData("OP_COMMAND", "OP_INITMASTERKEY");		// OP_INITMASTERKEY
//			SetShareData("OP_RESULT", OP_DO);
//			ClerkWaitResult();									// Clerk Wait Result : 2004.11.05
//		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSpecialModeProc():return");
	return TRUE;
}

// Clerk Set Param Procedure
int	CRearApCtrl::ClerkSetParamProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSetParamProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;
	CString			strPrintData("");

	nScrNum = 928;												// Display Parameter
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Special Mode Procedure
			break;
		}
		else 
		if ((strTempArray.GetSize() >= 31) &&					// liyi add 2008.09.23
			(strTempArray[0] == "SAVE"))						
		{
//			Clerk.PowerOffFlag = TRUE;							// 2005.10.25   先判断是否需要重新启动，然后再置该值  by zjd

																// Processing Message			
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

/////////////////////////////////////////////////////////////////////////////
			strTemp =	"OP_SETPARAM||";
			strTemp +=	strTempArray[1] + "||";
			strTemp +=	strTempArray[2] + "||";
			strTemp +=	strTempArray[3] + "||";
			strTemp +=	strTempArray[4] + "||";
			strTemp +=	strTempArray[5] + "||";
			strTemp +=	strTempArray[6] + "||";
			strTemp +=	strTempArray[7] + "||";
			strTemp +=	strTempArray[8] + "||";
			strTemp +=	strTempArray[9] + "||";
			strTemp +=	strTempArray[10] + "||";
			strTemp +=	strTempArray[11] + "||";
			strTemp +=	strTempArray[12] + "||";
			strTemp +=	strTempArray[13] + "||";
			strTemp +=	strTempArray[14] + "||";
			strTemp +=	strTempArray[15] + "||";
			strTemp +=	strTempArray[16] + "||";
			strTemp +=	strTempArray[17] + "||";
			strTemp +=	strTempArray[18] + "||";
			strTemp +=	strTempArray[19] + "||";
			strTemp +=	strTempArray[20] + "||";
			strTemp +=	strTempArray[21] + "||";
			strTemp +=	strTempArray[22] + "||";
			strTemp +=	strTempArray[23] + "||";
			strTemp +=	strTempArray[24] + "||";
			strTemp +=	strTempArray[25] + "||";
			strTemp +=	strTempArray[26] + "||";
			strTemp +=	strTempArray[27] + "||";
			strTemp +=	strTempArray[28] + "||";
			strTemp +=	strTempArray[29] + "||";
			strTemp +=	strTempArray[30];
			
			SetShareData("OP_COMMAND", strTemp);				// OP_SETPARAM
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else 
		if ((strTempArray.GetSize() >= 15) &&
			(strTempArray[0] == "PRINT"))
		{
/////////////////////////////////////////////////////////////////////////////
			strPrintData = "OP_SPRJPRPRINT||";
			// Line1 Edit
			strTemp = "========================================";
			strPrintData += strTemp + "||";
			// Line2 Edit
			strTemp = "       * USER DEFINE PARAMETER *";
			strPrintData += strTemp + "||";
			// Line3 Edit
			strTemp = "========================================";
			strPrintData += strTemp + "||";
			// Line4 Edit
			strTemp.Format("DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
												GetDate().GetBuffer(0),
												&(GetDate().GetBuffer(0)[4]),
												&(GetDate().GetBuffer(0)[6]),
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4]));
			strPrintData += strTemp + "||";
			// Line5 Edit
			strTemp.Format("ADV NUMBER         : %4.4s", strTempArray[1]);
			strPrintData += strTemp + "||";
			// Line6 Edit
			strTemp.Format("ADV INTERVAL       : %4.4s", strTempArray[2]);
			strPrintData += strTemp + "||";
			// Line7 Edit
			strTemp.Format("TRANS TIMEOUT      : %4.4s", strTempArray[3]);
			strPrintData += strTemp + "||";
			// Line8 Edit
			strTemp.Format("OP TIMEOUT         : %4.4s", strTempArray[4]);
			strPrintData += strTemp + "||";
			// Line9 Edit
			strTemp.Format("RETRACT CARD       : %4.4s", strTempArray[5]);
			strPrintData += strTemp + "||";
			// Line10 Edit
			strTemp.Format("RETRACT CASH       : %4.4s", strTempArray[6]);
			strPrintData += strTemp + "||";
			// Line11 Edit
			strTemp.Format("MSR TIME           : %4.4s", strTempArray[7]);
			strPrintData += strTemp + "||";
			// Line12 Edit
			strTemp.Format("DETAIL NUMBER      : %4.4s", strTempArray[8]);
			strPrintData += strTemp + "||";
			// Line13 Edit
			strTemp.Format("EX TIMES           : %4.4s", strTempArray[9]);
			strPrintData += strTemp + "||";
			// Line14 Edit
			strTemp.Format("RESEND TIMES       : %4.4s", strTempArray[10]);
			strPrintData += strTemp + "||";
			// Line15 Edit
			strTemp.Format("CWC TIMES          : %4.4s", strTempArray[11]);
			strPrintData += strTemp + "||";
			// Line16 Edit
			strTemp.Format("CWD MAX AMOUNT     : %4.4sRMB", strTempArray[12]);
			strPrintData += strTemp + "||";
			// Line17 Edit
			strTemp.Format("CWD MIN AMOUNT     : %4.4sRMB", strTempArray[13]);
			strPrintData += strTemp + "||";
			// Line18 Edit
			strTemp.Format("REMIT MAX AMOUNT   : %6.6sRMB", strTempArray[14]);
			strPrintData += strTemp + "||";
			// Line19 Edit
			strTemp.Format("REMIT MIN AMOUNT   : %6.6sRMB", strTempArray[15]);
			strPrintData += strTemp + "||";
			// Line20 Edit
			strTemp.Format("TFR MAX AMOUNT     : %6.6sRMB", strTempArray[16]);
			strPrintData += strTemp + "||";
			// Line21 Edit
			strTemp.Format("TFR MIN AMOUNT     : %4.4sRMB", strTempArray[17]);
			strPrintData += strTemp + "||";
			// Line22 Edit
			strTemp = "========================================";
			strPrintData += strTemp;

			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SetShareData("OP_COMMAND", strPrintData);			// OP_SPRJPRPRINT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// Flash Error Message
			strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSetParamProc():return");
	return TRUE;
}

// Clerk Set Master Key Procedure
int	CRearApCtrl::ClerkSetMasterKeyProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSetMasterKeyProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;

	nScrNum = 929;												// Display Parameter
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			break;
		}
		else 
		if ((strTempArray.GetSize() >= 2) &&					// 2005.10.25
			(strTempArray[0] == "SETMASTERKEY"))
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

/////////////////////////////////////////////////////////////////////////////
			strTemp =	"OP_SETMASTERKEY||";
			strTemp +=	strTempArray[1];
			
			SetShareData("OP_COMMAND", strTemp);				// OP_SETMASTERKEY
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// Flash Error Message
			strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSetMasterKeyProc():return");
	return TRUE;
}

// Clerk Input Auth Code Procedure   060710  by zjd
int	CRearApCtrl::ClerkInputAuthCodeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInputAuthCodeProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;
	CString			strAuthCode("");
	char			szIdenCode[128];
	char			szAuthCode[64];

	memset(szIdenCode, 0, sizeof(szIdenCode));
	memset(szAuthCode, 0, sizeof(szAuthCode));
	nScrNum = 932;												// Display Parameter
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			hy_GetIdentifyCode3(szIdenCode, sizeof(szIdenCode));				//返回出来为20位  by  zjd
			strTempClerkInformation = GetSprintf("%20s", szIdenCode);
			fnSCR_DisplayString(1, strTempClerkInformation);
			fnSCR_DisplayString(48, strTempClerkMessage);
			if (fnSCR_GetCurrentScreenNo() == nScrNum)			
				fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			else 
				fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			break;
		}
		else 
		if ((strTempArray.GetSize() >= 2) &&					// 2005.10.25
			(strTempArray[0] == "INPUTAUTHCODE"))
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			strAuthCode = strTempArray[1];

//			ClerkWaitResult();									// Clerk Wait Result
			memcpy(szAuthCode, strAuthCode.GetBuffer(0), strAuthCode.GetLength());
			hy_SaveAuthorithCode3((unsigned char *)szIdenCode, (unsigned char *)szAuthCode);

			if (hy_CheckAuthorithCodeFromFile3() == AUTH_OK)
			{
/////////////////////////////////////////////////////////////////////////////
				strTempClerkMessage = GuideMsgTbl[57][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTemp =	"OP_INPUTAUTHCODE||";
				strTemp +=	strAuthCode;
				
				SetShareData("OP_COMMAND", strTemp);			// OP_INPUTAUTHCODE
				SetShareData("OP_RESULT", OP_DO);
				Delay_Msg(3000);
			}
			else
			{
/////////////////////////////////////////////////////////////////////////////
				strTempClerkMessage = GuideMsgTbl[58][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTemp =	"OP_INPUTAUTHCODE||";
				strTemp +=	strAuthCode;
				
				SetShareData("OP_COMMAND", strTemp);			// OP_INPUTAUTHCODE
				SetShareData("OP_RESULT", OP_ERROR);
				Delay_Msg(3000);
			}
		}
		else													// Flash Error Message
			strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
	}
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInputAuthCodeProc():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk Code Procedure
int	CRearApCtrl::ClerkCodeInquiryProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkCodeInquiryProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;

	nScrNum = 935;												// Display Input Response Code
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 12;									// Clerk Op Mode Procedure
			break;
		}
		else 
		if ((strTempArray.GetSize() >= 2) &&					// 2006.08.22
			(strTempArray[0] == "OPERRORCODEINQ"))
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
//			fnSCR_DisplayString(48, strTempClerkMessage);		// 2006.08.22
//			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			strTempClerkMessage = IniGetStr(_OPERRORCODE_INI, _INIKEY_OPERRORCODE, strTempArray[1]);
			if (strTempClerkMessage == "")
				strTempClerkMessage = GuideMsgTbl[54][OpDeviceLangMode];
		}
		else 
		if ((strTempArray.GetSize() >= 2) &&					// 2006.08.22
			(strTempArray[0] == "OPHOSTNGCODEINQ"))
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
//			fnSCR_DisplayString(48, strTempClerkMessage);		// 2006.08.22
//			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			strTempClerkMessage = IniGetStr(_OPHOSTNGCODE_INI, _INIKEY_OPHOSTNGCODE, strTempArray[1]);
			if (strTempClerkMessage == "")
				strTempClerkMessage = GuideMsgTbl[54][OpDeviceLangMode];
		}
		else													// Flash Error Message
			strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkCodeInquiryProc():return");
	return TRUE;
}

// Clerk Input Master Key AB Procedure
int	CRearApCtrl::ClerkInputMasterKeyABProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInputMasterKeyABProc()");

	int				nScrNum = 0;
	int				RetryCnt = 3;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strPartA("");
	CString			strPartB("");
	CString			strCheckValue("");

	while (RetryCnt)
	{
		nScrNum = 941;											// Display Input Master Key Part A
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Special Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strPartA.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = Clerk.SaveIndex;					// Clerk Special Mode Procedure
				break;
			}
		}

/////////////////////////////////////////////////////////////////////////////
		nScrNum = 942;											// Display Input Master Key Part B
		nInitFlag = TRUE;
		strTempClerkInformation = "";
		strSaveClerkInformation = "";
		strTempClerkMessage = "";
		strSaveClerkMessage = "";

		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Special Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strPartB.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = Clerk.SaveIndex;					// Clerk Special Mode Procedure
				break;
			}
		}

		char	szPartA[256];
		char	szPartB[256];
		char	szResult[256];

		memset(szPartA, 0, sizeof(szPartA));
		memset(szPartB, 0, sizeof(szPartB));
		memset(szResult, 0, sizeof(szResult));

		MakePack(strPartA.GetBuffer(0), szPartA, strPartA.GetLength());
		MakePack(strPartB.GetBuffer(0), szPartB, strPartB.GetLength());
		for (int i = 0; i < 16; i++)
		{
			szResult[i] = szPartA[i] ^ szPartB[i];
		}

		strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
		strSaveClerkMessage = strTempClerkMessage;
		fnSCR_DisplayString(48, strTempClerkMessage);
		fnSCR_DisplayMessage();
		strTempClerkMessage = "";
		strCheckValue = "";

		strTemp =	"OP_SETMASTERKEY||";
		strTemp +=	MakeUnPack(szResult, 16);

		SetShareData("OP_MASTERKEY", MakeUnPack(szResult, 16));
		SetShareData("OP_COMMAND", strTemp);				// OP_SETMASTERKEY
		SetShareData("OP_RESULT", OP_DO);
		ClerkWaitResult();								// Clerk Wait Result
		strCheckValue = "校验:[" + GetShareData("OP_CHECKVALUE") + "]";
		fnSCR_DisplayString(24, strCheckValue);
		fnSCR_DisplayString(48, Clerk.AtmOpMessage);
		fnSCR_DisplayMessage();
		if (fnSCR_GetCurrentScreenNo() == nScrNum)			
			fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
		Delay_Msg(5000);

		Clerk.Index = Clerk.SaveIndex;						// Clerk Special Mode Procedure
		break;
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInputMasterKeyABProc():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk As Input Pin Procedure
int	CRearApCtrl::ClerkAsInputPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsInputPinProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	nScrNum = 902;												// Display Input Pin
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 1;									// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == Clerk.AsPassWord))
			{
				Clerk.Index = 22;								// Clerk As Mode Procedure
				break;
			}
			else
			{
				nScrNum = 903;									// Display Input Pin Again
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}													// PassWord Error Message
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsInputPinProc():return");
	return TRUE;
}

// Clerk As Mode Procedure
int	CRearApCtrl::ClerkAsModeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsModeProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strPrintData("");
	CString			strTemp("");
	CStringArray	strTempArray;
	
	nScrNum = 904;												// Display As Mode
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			SplitString(strTempClerkInformation, "||", strTempArray);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if ((Asc2Int(Clerk.InterfaceKind) == 18)	||	// 18:CUP(BJGD)	 : 2006.07.07
					(Asc2Int(Clerk.InterfaceKind) == 1005))		// 1005:CB(XA)	 : 2006.08.22
					;
				else
					fnSCR_DisplayImage(10, FALSE);				// F10 Disabled
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}

			Clerk.Index = 1;									// Goto Select Mode
			break;
		}
		else
		if (strReturn == "CHANGEPIN")
		{
			Clerk.Index = 23;									// Clerk As Input Change Pin Procedure
			break;
		}
		else
		if (strReturn == "APUPDATE")
		{
			Clerk.Index = 24;									// Clerk Ap Update Procedure
			break;
		}
		else
		if (strReturn == "TRACECOPY")
		{
			Clerk.Index = 25;									// Clerk Trace Copy Procedure
			break;
		}
		else
		if (strReturn == "EJMCOPY")								// 2004.07.05
		{
			Clerk.Index = 44;									// Clerk Ejm Copy Procedure
			break;
		}
		else
		if (strReturn == "BACKUPEJ")								// 2004.07.05
		{
			Clerk.Index = 63;									// Clerk Ejm Copy Procedure
			break;
		}
		else
		if (strReturn == "VDM")									// 2004.06.30
		{
			Clerk.Doing = FALSE;								// Power Off
			Clerk.Index = 1;
			Clerk.AtmOpStatus = "DOWN";
			Clerk.AtmOpMessage = GuideMsgTbl[36][OpDeviceLangMode];
			Clerk.UpdateScreen = TRUE;
			Clerk.UpdateButton = TRUE;
			Clerk.UpdateMessage = TRUE;
			
			SetShareData("OP_COMMAND", "OP_VDM");				// OP_VDM
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
			break;
		}
		else
		if (strReturn == "VERSION")
		{
			Clerk.Index = 26;									// Clerk Version Procedure
			break;
		}
		else
		if (strReturn == "WORKPARAM")
		{
			Clerk.Index = 27;									// Clerk Work Parameter Procedure
			break;
		}
		else
		if (strReturn == "DBINITIAL")
		{
			Clerk.Index = 28;									// Clerk DB Initialization Procedure
			break;
		}
		else
		if (strReturn == "DBPRINT")
		{
			CString	strTranDate("");
			strTranDate = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE);

			strPrintData = "OP_SPRJPRPRINT||";

			// Line1 Edit
			strTemp = "========================================";
			strPrintData += strTemp + "||";
			// Line2 Edit
			strTemp = "       * TRANSACITON STATISTICS *";
			strPrintData += strTemp + "||";
			// Line3 Edit
			strTemp = "========================================";
			strPrintData += strTemp + "||";
			// Line4 Edit
			strTemp.Format("DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
												GetDate().GetBuffer(0),
												&(GetDate().GetBuffer(0)[4]),
												&(GetDate().GetBuffer(0)[6]),
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4]));
			strPrintData += strTemp + "||";
			// Line5 Edit
			strTemp.Format("ATM ID             : %8.8s", Clerk.AtmSerialNumber);
			strPrintData += strTemp + "||";
			// Line6 Edit
			strTemp.Format("BRANCH NUMBER      : %9.9s", Clerk.BranchNumber);
			strPrintData += strTemp + "||";
			// Line7 Edit
			strTemp.Format("BEGIN DATE & TIME  : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
												strTranDate.Mid(0,4),
												strTranDate.Mid(4,2),
												strTranDate.Mid(6,2),
												strTranDate.Mid(9,2),
												strTranDate.Mid(11,2),
												strTranDate.Mid(13,2));
			strPrintData += strTemp + "||";
			// Line8 Edit
			strTemp.Format("END   DATE & TIME  : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
												strTranDate.Mid(16,4),
												strTranDate.Mid(20,2),
												strTranDate.Mid(22,2),
												strTranDate.Mid(25,2),
												strTranDate.Mid(27,2),
												strTranDate.Mid(29,2));
			strPrintData += strTemp + "||";
			// Line9 Edit
			strTemp.Empty();
			strPrintData += strTemp + "||";
			// Line10 Edit
			strTemp.Format("WITHDRAW     COUNT : %10.10s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANWITHDRAW).Mid(10, 10));
			strPrintData += strTemp + "||";
			// Line11 Edit
			strTemp.Format("WITHDRAW    AMOUNT : %16.16s.%2.2s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANWITHDRAW).Mid(23, 16),
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANWITHDRAW).Mid(39, 2));
			strPrintData += strTemp + "||";
			// Line12 Edit
			strTemp.Empty();
			strPrintData += strTemp + "||";
			// Line13 Edit
			strTemp.Format("INQUIRY      COUNT : %10.10s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANINQUIRY).Mid(10, 10));
			strPrintData += strTemp + "||";
			// Line14 Edit
			strTemp.Empty();
			strPrintData += strTemp + "||";
			// Line15 Edit
			strTemp.Format("TRANSFER     COUNT : %10.10s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANTRANSFER).Mid(10, 10));
			strPrintData += strTemp + "||";
			// Line16 Edit
			strTemp.Format("TRANSFER    AMOUNT : %16.16s.%2.2s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANTRANSFER).Mid(23, 16),
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANTRANSFER).Mid(39, 2));
			strPrintData += strTemp + "||";
			// Line17 Edit
			strTemp.Empty();
			strPrintData += strTemp + "||";
			// Line18 Edit
			strTemp.Format("DEPOSIT      COUNT : %10.10s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDEPOSIT).Mid(10, 10));
			strPrintData += strTemp + "||";
			// Line19 Edit
			strTemp.Format("DEPOSIT     AMOUNT : %16.16s.%2.2s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDEPOSIT).Mid(23, 16),
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDEPOSIT).Mid(39, 2));
			strPrintData += strTemp + "||";
			// Line20 Edit
			strTemp.Empty();
			strPrintData += strTemp + "||";
			// Line21 Edit
			strTemp.Format("PASSBOOK     COUNT : %10.10s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANPASSBOOK).Mid(10, 10));
			strPrintData += strTemp + "||";
			// Line22 Edit
			strTemp.Empty();
			strPrintData += strTemp + "||";
			// Line23 Edit
			strTemp.Format("IC           COUNT : %10.10s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANIC).Mid(10, 10));
			strPrintData += strTemp + "||";
			// Line24 Edit
			strTemp.Format("IC          AMOUNT : %16.16s.%2.2s",
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANIC).Mid(23, 16),
												RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANIC).Mid(39, 2));
			strPrintData += strTemp + "||";
			// Line25 Edit
			strTemp = "========================================";
			strPrintData += strTemp;

			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", strPrintData);			// OP_SPRJPRPRINT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result

//////////////////////////////////////////////////////////////////////////
			CString	strErrorDate("");
			CString	strErrorDbData("");
			int		nPrintingIndex = 0, i =0, j = 0;
			strErrorDate = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDATE);
			strErrorDbData = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDB);

			while (nPrintingIndex < ERROR_DB_MAX_COUNT)
			{
				strPrintData = "OP_SPRJPRPRINT||";
				// Line1 Edit
				strTemp = "========================================";
				strPrintData += strTemp + "||";
				// Line2 Edit
				strTemp = "          * ERROR STATISTICS *";
				strPrintData += strTemp + "||";
				// Line3 Edit
				strTemp = "========================================";
				strPrintData += strTemp + "||";
				// Line4 Edit
				strTemp.Format("DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
													GetDate().GetBuffer(0),
													&(GetDate().GetBuffer(0)[4]),
													&(GetDate().GetBuffer(0)[6]),
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]));
				strPrintData += strTemp + "||";
				// Line5 Edit
				strTemp.Format("ATM ID             : %8.8s", Clerk.AtmSerialNumber);
				strPrintData += strTemp + "||";
				// Line6 Edit
				strTemp.Format("BRANCH NUMBER      : %9.9s", Clerk.BranchNumber);
				strPrintData += strTemp + "||";
				// Line7 Edit
				strTemp.Format("BEGIN DATE & TIME  : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
													strErrorDate.Mid(0,4),
													strErrorDate.Mid(4,2),
													strErrorDate.Mid(6,2),
													strErrorDate.Mid(9,2),
													strErrorDate.Mid(11,2),
													strErrorDate.Mid(13,2));
				strPrintData += strTemp + "||";
				// Line8 Edit
				strTemp.Format("END   DATE & TIME  : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
													strErrorDate.Mid(16,4),
													strErrorDate.Mid(20,2),
													strErrorDate.Mid(22,2),
													strErrorDate.Mid(25,2),
													strErrorDate.Mid(27,2),
													strErrorDate.Mid(29,2));
				strPrintData += strTemp + "||";
				// Line9 Edit
				strTemp.Empty();
				strPrintData += strTemp + "||";

				for (i = 0; i < 16; i++)
				{
					if (strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 2, 7) == SPACE7)
					{
						if (i == 0)
						{
							if (nPrintingIndex == 0)
							{
								// Line10 Edit
								strTemp.Format("# NO DATA #");
								strPrintData += strTemp;
							}
							else
							{									// 2007.09.06
								nPrintingIndex = ERROR_DB_MAX_COUNT;
								break;
							}
						}

						SetShareData("OP_COMMAND", strPrintData);// OP_SPRJPRPRINT
						SetShareData("OP_RESULT", OP_DO);
						ClerkWaitResult();						// Clerk Wait Result
						nPrintingIndex = ERROR_DB_MAX_COUNT;	// End
						break;
					}

					strTemp.Format("%1.1s-%5.5s(%2.2s) %6.6s %4.4s.%2.2s.%2.2s %4.4s.%2.2s.%2.2s",
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 0, 1),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 2, 5),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 7, 2),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 10, 6),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 17, 4),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 21, 2),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 23, 2),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 26, 4),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 30, 2),
									strErrorDbData.Mid(nPrintingIndex * sizeof(StatErrorDbTbl) + 32, 2));
					if (i == 0)
						strPrintData += strTemp;
					else
						strPrintData += "||" + strTemp;

					nPrintingIndex++;
					if (i == 15)
					{
						strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
						strSaveClerkMessage = strTempClerkMessage;
						fnSCR_DisplayString(48, strTempClerkMessage);
						fnSCR_DisplayMessage();
						strTempClerkMessage = "";
			
						SetShareData("OP_COMMAND", strPrintData);// OP_SPRJPRPRINT
						SetShareData("OP_RESULT", OP_DO);
						if (ClerkWaitResult() == FALSE)			// Clerk Wait Result
							nPrintingIndex = ERROR_DB_MAX_COUNT;// End
					}
				}
			}
		}
		else
		if (strReturn == "OTHER")								// 2006.07.07
		{
			Clerk.Index = 29;									// Clerk Other Procedure
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsModeProc():return");
	return TRUE;
}

// Clerk As Input Change Pin Procedure
int	CRearApCtrl::ClerkAsInputChangePinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsInputChangePinProc()");

	int				nScrNum = 0;
	int				RetryCnt = 3;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strFirstPassWord("");
	CString			strSecondPassWord("");

	while (RetryCnt)
	{
		nScrNum = 913;											// Display Input Pin
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strFirstPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 22;								// Clerk As Mode Procedure
				break;
			}
		}

/////////////////////////////////////////////////////////////////////////////
		nScrNum = 914;											// Display Input Pin Again
		nInitFlag = TRUE;
		strTempClerkInformation = "";
		strSaveClerkInformation = "";
		strTempClerkMessage = "";
		strSaveClerkMessage = "";

		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strSecondPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 22;								// Clerk As Mode Procedure
				break;
			}
		}

		if(strFirstPassWord == strSecondPassWord)
		{
			strTemp = "OP_CHANGEASPIN||" + strFirstPassWord;
			SetShareData("OP_COMMAND", strTemp);				// OP_CHANGEASPIN
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
			
			fnSCR_DisplayString(48, Clerk.AtmOpMessage);
			fnSCR_DisplayMessage();
			Delay_Msg(2000);

			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		{
			if (!--RetryCnt)
			{													// Operation Miss Error Message
				strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 22;								// Clerk As Mode Procedure
				break;
			}
			else
			{													// PassWord Missmatch
				strTempClerkMessage = GuideMsgTbl[29][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsInputChangePinProc():return");
	return TRUE;
}

// Clerk Ap Update Procedure
int	CRearApCtrl::ClerkApUpdateProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkApUpdateProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");

	unsigned		nDriveType;
	CString			strDriveNum("");
	CString			strUpdateFileName = "V*.ZIP";
	CString			strUpdateFilePathName("");
	CString			strDirSeperator("\\");
	CFileFind		finder;
	BOOL			bIsFind;
	int				nCopyOk = 0;
	BOOL			bHaveUpdate = FALSE;
	CFileStatus		TempFileStatus;
	CString			strCheckFileName("");

	nScrNum = 906;												// Display Ap Update
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		if (strReturn == "FDUPDATE")
		{
			Clerk.PowerOffFlag = TRUE;							// 2004.07.05

			nDriveType = 7;
			bHaveUpdate = TRUE;
		}
		else
		if (strReturn == "CDROMUPDATE")
		{
			nDriveType = DRIVE_CDROM;
			bHaveUpdate = TRUE;
		}
		else
		if (strReturn == "USBUPDATE")
		{
			nDriveType = 8;
			bHaveUpdate = TRUE;
		}

		if (bHaveUpdate)
		{
			bHaveUpdate = FALSE;
																// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			strDriveNum = ClerkCheckDriveIsInstall(nDriveType);
			if ( strDriveNum == "" )
			{
				strTempClerkMessage = GuideMsgTbl[31][OpDeviceLangMode];			
			}
			else
			if ( !ClerkCheckDiskIsInsert( strDriveNum ) )
			{
				strTempClerkMessage = GuideMsgTbl[32][OpDeviceLangMode];
			}
			else
			{
				strUpdateFilePathName = strDriveNum + strUpdateFileName;
				bIsFind = finder.FindFile(strUpdateFilePathName);
				if ( !bIsFind )
				{
					finder.Close();
					strTempClerkMessage = GuideMsgTbl[33][OpDeviceLangMode];
				}
				else
				{
					while (bIsFind)
					{
						bIsFind = finder.FindNextFile();
						strUpdateFilePathName = finder.GetFilePath();
						strUpdateFileName = finder.GetFileName();
						
						nCopyOk = CopyFile( strUpdateFilePathName, _AP_UPDATE_DIR + strDirSeperator + strUpdateFileName, 0 );
						if ( nCopyOk == 0 )
						{
							strTempClerkMessage = GuideMsgTbl[34][OpDeviceLangMode];
							break;
						}
					}
					finder.Close();
					if ( nCopyOk != 0 )
					{											// Successfully Message
						Clerk.PowerOffFlag = TRUE;				// 2007.04.10
						strTempClerkMessage = GuideMsgTbl[23][OpDeviceLangMode];
					}
				}
			}
		}
	}

	strCheckFileName.Format("%s\\%s", _AP_UPDATE_DIR, strUpdateFileName);

	if (CFile::GetStatus(strCheckFileName, TempFileStatus))		// File Check
	{
		TempFileStatus.m_attribute = 0x00;						// 2007.07.18
		CFile::SetStatus(strCheckFileName, TempFileStatus);
	}
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkApUpdateProc():return");
	return TRUE;
}

// Clerk Trace Copy Procedure
int	CRearApCtrl::ClerkTraceCopyProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTraceCopyProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	unsigned		nDriveType;
	CString			strDriveNum("");
	CString			strSrcTraceFile("");
	CString			strObjTraceFile("");
	CString			strInputDate("");
	BOOL			bHaveCopy = FALSE;
	CString			strZipFileExp;

	nScrNum = 907;												// Display Trace Copy
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SplitString(strReturn, ":", strTempArray);
			if(strTempArray.GetSize() < 2)
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
			else
			if (strTempArray[0] == "FDCOPY")
			{
				nDriveType = 7;
				bHaveCopy = TRUE;
			}
			else
			if (strTempArray[0] == "USBCOPY")
			{
				nDriveType = 8;
				bHaveCopy = TRUE;
			}

			if (bHaveCopy)
			{
				bHaveCopy = FALSE;
																// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strInputDate = ZERO8 + strTempArray[1];
				strInputDate = strInputDate.Right(8);

				strDriveNum = ClerkCheckDriveIsInstall(nDriveType);
				if ( strDriveNum == "" )
				{
					strTempClerkMessage = GuideMsgTbl[31][OpDeviceLangMode];
				}
				else
				if ( !ClerkCheckDiskIsInsert( strDriveNum ) )
				{
					strTempClerkMessage = GuideMsgTbl[32][OpDeviceLangMode];
				}
				else
				{
					CreateDirectory( _AP_TEMPZIP_DIR, NULL );
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );
					ClerkCopyTmpFile( _AP_TRACE_DIR, ".KXLOG", strInputDate );
					ClerkCopyTmpFile( _AP_TRACE_DIR, ".TXT", strInputDate );
					ClerkCopyTmpFile( _EJR_DIR, ".TXT", strInputDate );
					ClerkCopyTmpFile( _INI_DIR, ".INI", ZERO8 );
					
					Delay_Msg(100);
					strSrcTraceFile.Format("%s\\TRACE.ZIP", _AP_TEMPZIP_DIR);
					strZipFileExp.Format( "pkzip25 -add %s %s\\*.*", strSrcTraceFile, _AP_TEMPZIP_DIR );
					WinExecWaitEnd( strZipFileExp, SW_HIDE );

					strObjTraceFile.Format("%s\\TRACE.ZIP", strDriveNum);
					if ( CopyFile( strSrcTraceFile, strObjTraceFile, 0 ) != 0 )
					{
						strTempClerkMessage = GuideMsgTbl[23][OpDeviceLangMode];
					}											// Successfully Message
					else
					{
						strTempClerkMessage = GuideMsgTbl[34][OpDeviceLangMode];
					}
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTraceCopyProc() return");
	return TRUE;
}

// Clerk Version Procedure
int	CRearApCtrl::ClerkVersionProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkVersionProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;
	CString			strPrintData("");

	nScrNum = 908;												// Display Version
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else 
		if (strReturn == "PRINT")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

/////////////////////////////////////////////////////////////////////////////
			strTemp = GetShareData("OP_VERSION");				// OP_VERSION
			SplitString(strTemp, "||", strTempArray);
			
			if (strTempArray.GetSize() < 22)
				strTempClerkMessage = "VERSION DATA ERROR !";
			else
			{
				strPrintData = "OP_SPRJPRPRINT||";
				// Line1										// 2007.11.01
				strTemp.Format("   * VERSION INFORMATION *(%8.8s)", Clerk.AtmSerialNumber);
				strPrintData += strTemp + "||";
				// Line2
				strTemp.Format("DATE & TIME : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s", 
												GetDate().GetBuffer(0),
												&(GetDate().GetBuffer(0)[4]),
												&(GetDate().GetBuffer(0)[6]),
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4]));
				strPrintData += strTemp + "||";
				// Line3										// 2006.02.23
				strTemp.Format("AP VERSION  : %s (%s:%d)", strTempArray[0], Clerk.MachineKind.GetBuffer(0), Asc2Int(Clerk.InterfaceKind));
				strPrintData += strTemp + "||";
				// Line4
				strTemp.Format("=== SP VERSION ===");
				strPrintData += strTemp + "||";
				// Line5
				strTemp.Format("CDU : %s", strTempArray[2]);
				strPrintData += strTemp + "||";
				// Line6
				strTemp.Format("JPR : %s", strTempArray[3]);
				strPrintData += strTemp + "||";
				// Line7
				strTemp.Format("SPR : %s", strTempArray[4]);
				strPrintData += strTemp + "||";
				// Line8
				strTemp.Format("MCU : %s", strTempArray[5]);
				strPrintData += strTemp + "||";
				// Line9
				strTemp.Format("PIN : %s", strTempArray[6]);
				strPrintData += strTemp + "||";
				// Line10
				strTemp.Format("CAM : %s", strTempArray[7]);
				strPrintData += strTemp + "||";
				// Line11
				strTemp.Format("SIU : %s", strTempArray[8]);
				strPrintData += strTemp + "||";
				// Line12
				strTemp.Format("UPS : %s", strTempArray[9]);
				strPrintData += strTemp + "||";
				// Line13
				strTemp.Format("TTU : %s", strTempArray[10]);
				strPrintData += strTemp + "||";
				// Line14
				strTemp.Format("MWI : %s", strTempArray[11]);
				strPrintData += strTemp + "||";
				// Line15
				strTemp.Format("COMMONITOR : %s", strTempArray[12]);
				strPrintData += strTemp + "||";
				// Line16
				strTemp.Format("=== EP VERSION ===");
				strPrintData += strTemp + "||";
				// Line17
				strTemp.Format("CDU : %s", strTempArray[13]);
				strPrintData += strTemp + "||";
				// Line18
				strTemp.Format("JPR : %s", strTempArray[14]);
				strPrintData += strTemp + "||";
				// Line19
				strTemp.Format("SPR : %s", strTempArray[15]);
				strPrintData += strTemp + "||";
				// Line20
				strTemp.Format("MCU : %s", strTempArray[16]);
				strPrintData += strTemp + "||";
				// Line21
				strTemp.Format("PIN : %s", strTempArray[17]);
				strPrintData += strTemp + "||";
				// Line22
				strTemp.Format("CAM : %s", strTempArray[18]);
				strPrintData += strTemp + "||";
				// Line23
				strTemp.Format("SIU : %s", strTempArray[19]);
				strPrintData += strTemp + "||";
				// Line24
				strTemp.Format("UPS : %s", strTempArray[20]);
				strPrintData += strTemp + "||";
				// Line25
				strTemp.Format("TTU : %s", strTempArray[21]);
				strPrintData += strTemp;
				
				SetShareData("OP_COMMAND", strPrintData);		// OP_SPRJPRPRINT
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkVersionProc():return");
	return TRUE;
}

// Clerk Work Parameter Procedure
int	CRearApCtrl::ClerkWorkParamProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkWorkParamProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;
	CStringArray	strOldArray;
	CString			strPrintData("");

	nScrNum = 909;												// Display Work Parameter
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk As Mode Procedure
			break;
		}
		else 
		if ((strTempArray.GetSize() >= 16) &&					// 2005.05.26
			(strTempArray[0] == "SAVE"))
		{
			if (!Asc2Int(IniGetStr(_TRANSACTION_INI, "TRANSACTION", "ATMPTOTALKIND", "1"))	&&
				strTempArray[15] == "1"														&&
				Asc2Int(IniGetStr(_TRANSACTION_INI,"TRANSACTION","TOTALADDRULESET","1")))
			{
//				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strTempClerkMessage = "请先修改轧账方式!";
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);
				
				Clerk.Index = Clerk.SaveIndex;						// Clerk As Mode Procedure
				break;
			}
			SplitString(strTempClerkInformation, "\n", strOldArray);
			for (int i=1; i < 14; i++)
			{
				MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "strOldArray[%s]",strOldArray[i]);
				MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "strTempArray[%s]",strTempArray[i+1]);
				if (strOldArray[i] != strTempArray[i+1])           
					Clerk.PowerOffFlag = TRUE;					// 2004.07.05   先判断是否需要重新启动，然后再置该值  by zjd
			}

			if ((strTempArray[15] == "0") || (strTempArray[15] == "1"))	
			{													// 立即设置加钞方式的值  by zjd
				MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "addcashmode:[%s]",strTempArray);
				RegSetStr(_REGKEY_ADDCASHMODE, ZERO2, strTempArray[15]);
			}
																// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

/////////////////////////////////////////////////////////////////////////////
			strTemp =	"OP_WORKPARAM||";
			strTemp +=	strTempArray[ 1] + "||";
			strTemp +=	strTempArray[ 2] + "||";
			strTemp +=	strTempArray[ 3] + "||";
			strTemp +=	strTempArray[ 4] + "||";
			strTemp +=	strTempArray[ 5] + "||";
			strTemp +=	strTempArray[ 6] + "||";
			strTemp +=	strTempArray[ 7] + "||";
			strTemp +=	strTempArray[ 8] + "||";
			strTemp +=	strTempArray[ 9] + "||";
			strTemp +=	strTempArray[10] + "||";
			strTemp +=	strTempArray[11] + "||";
			strTemp +=	strTempArray[12] + "||";
			strTemp +=	strTempArray[13] + "||";
			strTemp +=	strTempArray[14] + "||";
			strTemp +=	strTempArray[15];
			
			SetShareData("OP_COMMAND", strTemp);				// OP_WORKPARAM
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else 
		if ((strTempArray.GetSize() >= 11) &&
			(strTempArray[0] == "PRINT"))
		{
/////////////////////////////////////////////////////////////////////////////
			strPrintData = "OP_SPRJPRPRINT||";
			// Line1 Edit
			strTemp = "========================================";
			strPrintData += strTemp + "||";
			// Line2 Edit
			strTemp = "          * WORK PARAMETER *";
			strPrintData += strTemp + "||";
			// Line3 Edit
			strTemp = "========================================";
			strPrintData += strTemp + "||";
			// Line4 Edit
			strTemp.Format("DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
												GetDate().GetBuffer(0),
												&(GetDate().GetBuffer(0)[4]),
												&(GetDate().GetBuffer(0)[6]),
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4]));
			strPrintData += strTemp + "||";
			// Line5 Edit
			strTemp.Format("ATM ID             : %8.8s", strTempArray[1]);
			strPrintData += strTemp + "||";
			// Line6 Edit
			strTemp.Format("BRANCH NUMBER      : %9.9s", strTempArray[2]);
			strPrintData += strTemp + "||";
			// Line7 Edit
			strTemp.Format("RESERVED NUMBER1   : %8.8s", strTempArray[3]);
			strPrintData += strTemp + "||";
			// Line8 Edit
			strTemp.Format("RESERVED NUMBER2   : %8.8s", strTempArray[4]);
			strPrintData += strTemp + "||";
			// Line9 Edit
			strTemp.Format("HOST IP  (TRAN)    : %3.3s.%3.3s.%3.3s.%3.3s",
												strTempArray[5].Mid(0,3),
												strTempArray[5].Mid(3,3),
												strTempArray[5].Mid(6,3),
												strTempArray[5].Mid(9,3));
			strPrintData += strTemp + "||";
			// Line10 Edit
			strTemp.Format("HOST PORT(TRAN)    : %5.5s", strTempArray[6]);
			strPrintData += strTemp + "||";
			// Line11 Edit
			strTemp.Format("HOST IP  (JNL)     : %3.3s.%3.3s.%3.3s.%3.3s",
												strTempArray[7].Mid(0,3),
												strTempArray[7].Mid(3,3),
												strTempArray[7].Mid(6,3),
												strTempArray[7].Mid(9,3));
			strPrintData += strTemp + "||";
			// Line12 Edit
			strTemp.Format("HOST PORT(JNL)     : %5.5s", strTempArray[8]);
			strPrintData += strTemp + "||";
			// Line13 Edit
			strTemp.Format("ATM IP ADDRESS     : %3.3s.%3.3s.%3.3s.%3.3s",
												strTempArray[9].Mid(0,3),
												strTempArray[9].Mid(3,3),
												strTempArray[9].Mid(6,3),
												strTempArray[9].Mid(9,3));
			strPrintData += strTemp + "||";
			// Line14 Edit
			strTemp.Format("ATM SUBNET MASK    : %3.3s.%3.3s.%3.3s.%3.3s",
												strTempArray[10].Mid(0,3),
												strTempArray[10].Mid(3,3),
												strTempArray[10].Mid(6,3),
												strTempArray[10].Mid(9,3));
			strPrintData += strTemp + "||";
			// Line15 Edit
			strTemp.Format("ATM GATE WAY       : %3.3s.%3.3s.%3.3s.%3.3s",
												strTempArray[11].Mid(0,3),
												strTempArray[11].Mid(3,3),
												strTempArray[11].Mid(6,3),
												strTempArray[11].Mid(9,3));
			strPrintData += strTemp + "||";
			// Line16 Edit
			strTemp.Format("ATM PORT           : %5.5s", strTempArray[12]);
			strPrintData += strTemp + "||";
			// Line17 Edit
			strTemp.Format("HOST IP  (BID)     : %3.3s.%3.3s.%3.3s.%3.3s",
												strTempArray[13].Mid(0,3),
												strTempArray[13].Mid(3,3),
												strTempArray[13].Mid(6,3),
												strTempArray[13].Mid(9,3));
			strPrintData += strTemp + "||";
			// Line18 Edit
			strTemp.Format("HOST PORT(BID)     : %5.5s", strTempArray[14]);
			strPrintData += strTemp + "||";
			// Line19 Edit
			strTemp.Format("ADDCASH MODE       : %3.3s", strTempArray[15]);
			strPrintData += strTemp + "||";
			// Line20 Edit
			strTemp = "========================================";
			strPrintData += strTemp;

			SetShareData("OP_COMMAND", strPrintData);			// OP_SPRJPRPRINT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// Flash Error Message
			strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkWorkParamProc():return");
	return TRUE;
}

// Clerk DB Initialization Procedure
int CRearApCtrl::ClerkDBInitialProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkDBInitialProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");

	nScrNum = 919;												// Display DB Initialization
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		if (strReturn == "INITIAL")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SetShareData("OP_COMMAND", "OP_DBINITIAL");			// OP_DBINITIAL
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkDBInitialProc():return");
	return TRUE;
}

// Clerk Other Operation Procedure
int CRearApCtrl::ClerkOtherOperationProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOtherOperationProc()");

	int				nScrNum = 0;
	int				nFirstFlag = TRUE;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	
	nScrNum = 931;												// Display Select Mode
	if (Asc2Int(Clerk.InterfaceKind) == 18)						// 18:CUP(BJGD)	 : 2006.07.07
		nScrNum = 931;
	else
	if (Asc2Int(Clerk.InterfaceKind) == 1005)					// 1005:CB(XA)	 : 2006.08.22
		nScrNum = 933;

	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();	

			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			SplitString(strTempClerkInformation, "||", strTempArray);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 22;									// Clerk As Mode Procedure
			break;
		}
		else
		if (strReturn == "PRINTCASH")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_PRINTCASH");			// OP_PRINTCASH
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "CASHVALUE")							// 2005.02.24
		{
			Clerk.Index = 15;									// Clerk Cash Value Procedure
			break;
		}
		else
		if (strReturn == "RQK")
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_RQK");				// OP_RQK
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "SPECIAL")
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			break;
		}
		else
		if (strReturn == "EJM")
		{
			Clerk.Index = 41;									// Clerk Ejm Result Procedure
			break;
		}
		else
		if (strReturn == "TFRONMOTHERCARD")						// 2006.08.22
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_SETTRAN||TFRON||MOTHERCARD");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "TFROFFMOTHERCARD")					// 2006.08.22
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_SETTRAN||TFROFF||MOTHERCARD");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "TFRONOTHERCARD")						// 2006.08.22
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_SETTRAN||TFRON||OTHERCARD");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "TFROFFOTHERCARD")						// 2006.08.22
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_SETTRAN||TFROFF||OTHERCARD");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOtherOperationProc():return");
	return TRUE;
}

// Clerk Read Ic Master Key Procedure
int CRearApCtrl::ClerkReadIcMasterKeyProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkReadIcMasterKeyProc()");
	int				nScrNum = 0;
	int				RetryCnt = 3;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strFirstPassWord("");
	CString			strSecondPassWord("");

	nScrNum = 936;												// Display Insert Card A

	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_YES)
			break;
		else
		if (strReturn == S_NO)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			return TRUE;
		}
	}

	nScrNum = 902;												// Display Input Pin
	nInitFlag = TRUE;											// Display | Update
	strTempClerkMessage = "";
	strSaveClerkMessage = "";
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			return TRUE;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == "123456"))
			{
				fnSCR_DisplayScreen(939);
				Delay_Msg(3000);
				fnSCR_DisplayScreen(940);
				Delay_Msg(3000);
				break;
			}
			else
			{													// PassWord Error Message
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}
		}
	}

	nScrNum = 937;												// Display Insert Card B
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_YES)
			break;
		else
		if (strReturn == S_NO)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			return TRUE;
		}
	}

	nScrNum = 902;												// Display Input Pin
	nInitFlag = TRUE;											// Display | Update
	strTempClerkMessage = "";
	strSaveClerkMessage = "";
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			return TRUE;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == "123456"))
			{
				fnSCR_DisplayScreen(939);
				Delay_Msg(3000);
				fnSCR_DisplayScreen(940);
				Delay_Msg(3000);
				break;
			}
			else
			{													// PassWord Error Message
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}
		}
	}

	nScrNum = 938;												// Display Insert Card T
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_YES)
			break;
		else
		if (strReturn == S_NO)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			return TRUE;
		}
	}

	nScrNum = 902;												// Display Input Pin
	nInitFlag = TRUE;											// Display | Update
	strTempClerkMessage = "";
	strSaveClerkMessage = "";
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 17;									// Clerk Special Mode Procedure
			return TRUE;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == "123456"))
			{
				fnSCR_DisplayScreen(939);
				Delay_Msg(3000);
				fnSCR_DisplayScreen(940);
				Delay_Msg(3000);
				Clerk.Index = 17;								// Clerk Special Mode Procedure
				break;
			}
			else
			{													// PassWord Error Message
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkReadIcMasterKeyProc():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk Administrator Login Procedure							// 2005.03.24
int CRearApCtrl::ClerkAdminLoginProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAdminLoginProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strPrintData("");

	nScrNum = 921;												// Display Administrator Login
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 1;									// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 3)		&&
				(strTempArray[1].GetLength() >= 1)	&&
				(strTempArray[2].GetLength() >= 1))
			{
				strTempArray[1] = ZERO2 + strTempArray[1];
				strTempArray[1] = strTempArray[1].Right(2);
				if (strTempArray[2] == RegGetStr(_REGKEY_ADMINPIN, strTempArray[1]))
				{
					if (strTempArray[1] == ZERO2)
					{											// Processing Message
						strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
						strSaveClerkMessage = strTempClerkMessage;
						fnSCR_DisplayString(48, strTempClerkMessage);
						fnSCR_DisplayMessage();
						strTempClerkMessage = "";

						strPrintData = "OP_JPRPRINT||";
						strTemp.Format("%2.2s:%2.2s:%2.2s [LOGIN ID] [%s]",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]),
													strTempArray[1]);
						strPrintData += strTemp;
						
						SetShareData("OP_COMMAND", strPrintData);
						SetShareData("OP_RESULT", OP_DO);		// OP_JPRPRINT
						ClerkWaitResult();

						Clerk.Index = 32;						// Clerk Super Administrator Mode Procedure
						break;
					}
					else
					{											// Processing Message
						strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
						strSaveClerkMessage = strTempClerkMessage;
						fnSCR_DisplayString(48, strTempClerkMessage);
						fnSCR_DisplayMessage();
						strTempClerkMessage = "";

						strPrintData = "OP_JPRPRINT||";
						strTemp.Format("%2.2s:%2.2s:%2.2s [LOGIN ID] [%s]",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]),
													strTempArray[1]);
						strPrintData += strTemp;
						
						SetShareData("OP_COMMAND", strPrintData);
						SetShareData("OP_RESULT", OP_DO);		// OP_JPRPRINT
						ClerkWaitResult();

						Clerk.Index = 12;						// Clerk Op Mode Procedure
						break;
					}
				}
				else
				if (RegGetStr(_REGKEY_ADMINPIN, strTempArray[1]).GetLength() < 1)
				{												// No Such Administrator
					strTempClerkMessage = GuideMsgTbl[40][OpDeviceLangMode];
				}
				else
				{												// PassWord Error Message
					strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
				}
			}
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAdminLoginProc():return");
	return TRUE;
}

// Clerk Super Administrator Mode Procedure						// 2005.03.24
int CRearApCtrl::ClerkSuperAdminModeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSuperAdminModeProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	nScrNum = 922;												// Display Super Administrator Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 1;									// Clerk Select Mode Procedure
			break;
		}
		else
		if (strReturn == "ADDADMIN")
		{
			Clerk.Index = 33;									// Clerk Add Administrator Procedure
			break;
		}
		else
		if (strReturn == "DELADMIN")
		{
			Clerk.Index = 34;									// Clerk Delete Administrator Procedure
			break;
		}
		else
		if (strReturn == "CHANGEADMINPIN")
		{
			Clerk.Index = 35;									// Clerk Change Administrator Pin Procedure
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSuperAdminModeProc():return");
	return TRUE;
}

// Clerk Add Administrator Procedure							// 2005.03.24
int CRearApCtrl::ClerkAddAdminProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAddAdminProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strPrintData("");

	nScrNum = 923;												// Display Add Administrator Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 32;									// Clerk Super Administrator Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 3)		&&
				(strTempArray[1].GetLength() >= 1)	&&
				(strTempArray[2].GetLength() >= 1))
			{
				strTempArray[1] = ZERO2 + strTempArray[1];
				strTempArray[1] = strTempArray[1].Right(2);
				if (RegGetStr(_REGKEY_ADMINPIN, strTempArray[1]).GetLength() >= 1)
				{												// Administrator is already existed
					strTempClerkMessage = GuideMsgTbl[39][OpDeviceLangMode];
				}
				else
				{												// Processing Message
					strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
					strSaveClerkMessage = strTempClerkMessage;
					fnSCR_DisplayString(48, strTempClerkMessage);
					fnSCR_DisplayMessage();
					strTempClerkMessage = "";

					strPrintData = "OP_JPRPRINT||";
					strTemp.Format("%2.2s:%2.2s:%2.2s [ADD ID] [%s]",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4]),
												strTempArray[1]);
					strPrintData += strTemp;
					
					SetShareData("OP_COMMAND", strPrintData);	// OP_JPRPRINT
					SetShareData("OP_RESULT", OP_DO);		
					ClerkWaitResult();

					RegSetStr(_REGKEY_ADMINPIN, strTempArray[1], strTempArray[2]);
					Clerk.AtmOpMessage = GuideMsgTbl[23][OpDeviceLangMode];
					Clerk.UpdateMessage = TRUE;					// Successfully Message
				}
			}
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAddAdminProc():return");
	return TRUE;
}

// Clerk Delete Administrator Procedure							// 2005.03.24
int CRearApCtrl::ClerkDelAdminProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkDelAdminProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strPrintData("");

	nScrNum = 924;												// Display Add Administrator Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 32;									// Clerk Super Administrator Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)		&&
				(strTempArray[1].GetLength() >= 1))
			{
				strTempArray[1] = ZERO2 + strTempArray[1];
				strTempArray[1] = strTempArray[1].Right(2);
				if ((RegGetStr(_REGKEY_ADMINPIN, strTempArray[1]).GetLength() >= 1) &&
					(strTempArray[1] != ZERO2))
				{												// Processing Message
					strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
					strSaveClerkMessage = strTempClerkMessage;
					fnSCR_DisplayString(48, strTempClerkMessage);
					fnSCR_DisplayMessage();
					strTempClerkMessage = "";

					strPrintData = "OP_JPRPRINT||";
					strTemp.Format("%2.2s:%2.2s:%2.2s [DELETE ID] [%s]",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4]),
												strTempArray[1]);
					strPrintData += strTemp;
					
					SetShareData("OP_COMMAND", strPrintData);	// OP_JPRPRINT
					SetShareData("OP_RESULT", OP_DO);		
					ClerkWaitResult();

					RegSetStr(_REGKEY_ADMINPIN, strTempArray[1], "");
					Clerk.AtmOpMessage = GuideMsgTbl[23][OpDeviceLangMode];
					Clerk.UpdateMessage = TRUE;					// Successfully Message
				}
				else
				{												// No Such Administrator
					strTempClerkMessage = GuideMsgTbl[40][OpDeviceLangMode];
				}
			}
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkDelAdminProc():return");
	return TRUE;
}

// Clerk Change Administrator Pin Procedure						// 2005.03.24
int CRearApCtrl::ClerkChangeAdminPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkChangeAdminPinProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strPrintData("");

	nScrNum = 925;												// Display Add Administrator Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 32;									// Clerk Super Administrator Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 4)		&&
				(strTempArray[1].GetLength() >= 1)	&&
				(strTempArray[2].GetLength() >= 1)	&&
				(strTempArray[3].GetLength() >= 1))
			{
				strTempArray[1] = ZERO2 + strTempArray[1];
				strTempArray[1] = strTempArray[1].Right(2);
				if (RegGetStr(_REGKEY_ADMINPIN, strTempArray[1]).GetLength() >= 1)
				{
					if (strTempArray[2] == RegGetStr(_REGKEY_ADMINPIN, strTempArray[1]))
					{											// Processing Message
						strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
						strSaveClerkMessage = strTempClerkMessage;
						fnSCR_DisplayString(48, strTempClerkMessage);
						fnSCR_DisplayMessage();
						strTempClerkMessage = "";

						strPrintData = "OP_JPRPRINT||";
						strTemp.Format("%2.2s:%2.2s:%2.2s OPERATOR change PIN",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]));
						strPrintData += strTemp;
						
						SetShareData("OP_COMMAND", strPrintData);	
						SetShareData("OP_RESULT", OP_DO);		// OP_JPRPRINT
						ClerkWaitResult();

						RegSetStr(_REGKEY_ADMINPIN, strTempArray[1], strTempArray[3]);
						Clerk.AtmOpMessage = GuideMsgTbl[23][OpDeviceLangMode];
						Clerk.UpdateMessage = TRUE;				// Successfully Message
					}
					else
					{											// Old password is incorrect
						strTempClerkMessage = GuideMsgTbl[41][OpDeviceLangMode];
					}
				}
				else
				{												// No Such Administrator
					strTempClerkMessage = GuideMsgTbl[40][OpDeviceLangMode];
				}
			}
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkChangeAdminPinProc():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk Ejm Procedure
int CRearApCtrl::ClerkEjmProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	
	nScrNum = 905;												// Display Ejm Mode
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_NUMERIC_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)								// 2006.07.07
		{
			if ((Asc2Int(Clerk.InterfaceKind) == 18)	||		// 18:CUP(BJGD)	 : 2006.07.07
				(Asc2Int(Clerk.InterfaceKind) == 1005))			// 1005:CB(XA)	 : 2006.08.22
				Clerk.Index = 22;								// Clerk Other Operation Procedure
			else
				Clerk.Index = 12;								// Clerk Op Mode Procedure
			break;
		}
		else
		if (strReturn == "EJMVIEW")
		{
			Clerk.Index = 43;									// Clerk Ejm Result Procedure
			break;
		}
		else
		if (strReturn == "EJMCOPY")
		{
			Clerk.Index = 44;									// Clerk Ejm Copy Procedure
			break;
		}
		else
		if (strReturn == "EJMSEND")
		{
			Clerk.Index = 45;									// Clerk Ejm Send Procedure
			break;
		}
		else
		if (strReturn == "EJMPRINT")
		{
			Clerk.Index = 46;									// Clerk Ejm Print Procedure
			break;
		}
		else
		if (strReturn == "EJMJPRDIRECTPRINT")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			ClerkEjmJprDirectPrintProc();						// Clerk Ejm Jpr Direct Print Procedure
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmProc():return");
	return TRUE;
}

// Clerk Ejm View Procedure
int CRearApCtrl::ClerkEjmViewProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmViewProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");

	nScrNum = 911;												// Display Ejr View
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Ejm.Date = "";										// Date
			Ejm.TranSerialNo = "";								// TranSerialNo
			Ejm.AccountNum = "";								// AccountNum
			Clerk.Index = 43;									// Clerk Ejm Result Procedure
			break;
		}
		else
		{
			if (strTempArray[0] == "VIEW")
			{
				if (strTempArray.GetSize() >= 4)
				{												// Processing Message
					strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
					strSaveClerkMessage = strTempClerkMessage;
					fnSCR_DisplayString(48, strTempClerkMessage);
					fnSCR_DisplayMessage();
					strTempClerkMessage = "";
																// 2005.12.01
					Ejm.Date = strTempArray[1];					// Date
					Ejm.Date.TrimLeft();
					Ejm.Date.TrimRight();
					Ejm.TranSerialNo = strTempArray[2];			// TranSerialNo
					Ejm.TranSerialNo.TrimLeft();
					Ejm.TranSerialNo.TrimRight();
					Ejm.AccountNum = strTempArray[3];			// AccountNum
					Ejm.AccountNum.TrimLeft();
					Ejm.AccountNum.TrimRight();

					if ((Ejm.TranSerialNo.GetLength())	||
						(Ejm.AccountNum.GetLength()))
					{
						ClerkEjmRecordFind(0);					// Count Find
						ClerkEjmRecordFind(2);					// Data Find(TranSerialNo & AccountNum)
					}
					else
					{
						ClerkEjmRecordFind(0);					// Count Find
						if (Ejm.Count)
						{
							Ejm.Position = Ejm.Count - 1;
							ClerkEjmRecordFind(1);				// Data Find(Position)
						}
						else
						{
							Ejm.Position = 0;
							ClerkEjmRecordFind(1);				// Data Find(Position)
						}
					}

					Clerk.Index = 43;							// Clerk Ejm Result Procedure
					break;
				}
				else											// Flash Error Message
					strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
			else												// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmViewProc():return");
	return TRUE;
}

// Clerk Ejm Result Procedure
int CRearApCtrl::ClerkEjmResultProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmResultProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	int				i = 0;

	if ((Clerk.SaveIndex == 42)		&&							// Clerk Ejm View Procedure
		(Ejm.Date.GetLength() == 8))
		;
	else
	{
		strTempClerkInformation = ClerkInformationMake(42);		// Clerk Ejm View Procedure
		SplitString(strTempClerkInformation, "\n", strTempArray);
		if (strTempArray.GetSize() >= 1)
			Ejm.Date = strTempArray[0];
		else
			Ejm.Date = GetDate();
		Ejm.TranSerialNo = "";
		Ejm.AccountNum = "";

		ClerkEjmRecordFind(0);									// Count Find
		if (Ejm.Count)
		{
			Ejm.Position = Ejm.Count - 1;
			ClerkEjmRecordFind(1);								// Data Find(Position)
		}
		else
		{
			Ejm.Position = 0;
			ClerkEjmRecordFind(1);								// Data Find(Position)
		}
	}

	nScrNum = 912;												// Display Ejm Result
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}
		
			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if (Ejm.Count)
			{
				if (Ejm.FindOkFlag)
					strTempClerkMessage.Format("(%d / %d)", Ejm.Position + 1, Ejm.Count);
				else											// No Data Message
					strTempClerkMessage = GuideMsgTbl[37][OpDeviceLangMode];
			}
			else												// No Data Message
				strTempClerkMessage = GuideMsgTbl[37][OpDeviceLangMode];
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 41;									// Clerk Ejm Procedure
			break;
		}
		else
		if (strReturn == "SEARCH") 
		{
			Clerk.Index = 42;									// Clerk Ejm View Procedure
			break;
		}
		else
		if (strReturn == "VIEW+1") 
		{
			Ejm.Position++;
			if (Ejm.Position >= Ejm.Count)						// Return First Record
				Ejm.Position = 0;

			ClerkEjmRecordFind(1);								// Data Find(Position)
		}
		else
		if (strReturn == "VIEW-1") 
		{
			Ejm.Position--;
			if (Ejm.Position < 0)								// Return Last Record
			{
				if (Ejm.Count)
					Ejm.Position = Ejm.Count - 1;
				else
					Ejm.Position = 0;
			}

			ClerkEjmRecordFind(1);								// Data Find(Position)
		}
		else
		if (strReturn == "VIEW-10") 
		{
			if (Ejm.Position == 0)								// Return Last Record				
			{
				if (Ejm.Count)
					Ejm.Position = Ejm.Count - 1;
				else
					Ejm.Position = 0;
			}
			else												// Return First Record
			{
				Ejm.Position -= 10;
				if (Ejm.Position < 0)							
					Ejm.Position = 0;
			}

			ClerkEjmRecordFind(1);								// Data Find(Position)
		}
		else
		if (strReturn == "PRINTEJM")							// Print Ejm
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SplitString(strTempClerkInformation, "\n", strTempArray);
			strTemp =  "OP_SPRPRINT||";
			for (i = 0; i <= 23; i++)
				strTemp += strTempArray[i] + "||";
			strTemp += strTempArray[24];

			SetShareData("OP_COMMAND", strTemp);				// OP_SPRPRINT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmResultProc():return");
	return TRUE;
}

// Clerk Ejm Copy Procedure
int	CRearApCtrl::ClerkEjmCopyProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmCopyProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	unsigned		nDriveType;
	CString			strDriveNum("");
	CString			strSrcTraceFile("");						
	CString			strObjTraceFile("");
	CString			strInputStartDate("");						// 2005.02.24
	CString			strInputEndDate("");
	CString			strInputDate("");
	int				nInputStartDate = 0;
	int				nInputEndDate = 0;
	int				nInputDate = 0;
	BOOL			bHaveCopy = FALSE;
	CString			strZipFileExp;

	nScrNum = 915;												// Display Ejm Send
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Ejm Procedure or Clerk AS Procedure
			break;
		}
		else
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SplitString(strReturn, ":", strTempArray);
			if(strTempArray.GetSize() < 2)
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
			else
			if (strTempArray[0] == "FDCOPY")
			{
				nDriveType = 7;
				bHaveCopy = TRUE;
			}
			else
			if (strTempArray[0] == "USBCOPY")
			{
				nDriveType = 8;
				bHaveCopy = TRUE;
			}

			if (bHaveCopy)
			{
				bHaveCopy = FALSE;
																// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strInputStartDate = ZERO8 + strTempArray[1];
				strInputStartDate = strInputStartDate.Right(8);
				nInputStartDate = Asc2Int(strInputStartDate);
				strInputEndDate = ZERO8 + strTempArray[2];
				strInputEndDate = strInputEndDate.Right(8);
				nInputEndDate = Asc2Int(strInputEndDate);

				strDriveNum = ClerkCheckDriveIsInstall(nDriveType);
				if ( strDriveNum == "" )
				{
					strTempClerkMessage = GuideMsgTbl[31][OpDeviceLangMode];
				}
				else
				if ( !ClerkCheckDiskIsInsert( strDriveNum ) )
				{
					strTempClerkMessage = GuideMsgTbl[32][OpDeviceLangMode];
				}
				else
				if ( nInputStartDate > nInputEndDate )
				{
					strTempClerkMessage = GuideMsgTbl[38][OpDeviceLangMode];
				}
				else
				{
					CreateDirectory( _AP_TEMPZIP_DIR, NULL );
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );

					for (nInputDate = nInputStartDate; nInputDate <= nInputEndDate; nInputDate++)
					{
						if (((nInputDate % 100) > 31)		||
							(((nInputDate % 10000) / 100) > 12))
							continue;
						strInputDate = Int2Asc(nInputDate);
						ClerkMergeTmpFile(_EJR_DIR, ".TXT", strInputDate, 2);
						Delay_Msg(100);							// ALL COPY
					}

					strSrcTraceFile.Format("%s\\EJRALL%s(%s-%s).ZIP", _AP_TEMPZIP_DIR, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
					strZipFileExp.Format( "pkzip25 -add %s %s\\*.*", strSrcTraceFile, _AP_TEMPZIP_DIR );
					WinExecWaitEnd( strZipFileExp, SW_HIDE );

					strObjTraceFile.Format("%s\\EJRALL%s(%s-%s).ZIP", strDriveNum, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
					if ( CopyFile( strSrcTraceFile, strObjTraceFile, 0 ) != 0 )
					{
						strTempClerkMessage = GuideMsgTbl[23][OpDeviceLangMode];
					}											// Successfully Message
					else
					{
						strTempClerkMessage = GuideMsgTbl[34][OpDeviceLangMode];
					}
					
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmCopyProc() return");
	return TRUE;
}

// Clerk Ejm Send Procedure
int CRearApCtrl::ClerkEjmSendProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmSendProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	unsigned		nEjmType;
	CString			strSrcTraceFile("");						
	CString			strObjTraceFile("");
	CString			strInputStartDate("");						// 2005.02.24
	CString			strInputEndDate("");
	CString			strInputDate("");
	int				nInputStartDate = 0;
	int				nInputEndDate = 0;
	int				nInputDate = 0;
	BOOL			bHaveSend = FALSE;
	CString			strZipFileExp("");
	CString			strFtpFimeName("");

	nScrNum = 917;												// Display Ejm Send
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 41;									// Clerk Ejm Procedure
			break;
		}
		else
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SplitString(strReturn, ":", strTempArray);
			if(strTempArray.GetSize() < 2)
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
			else
			if (strTempArray[0] == "EJMSENDWITH")				// Send Withdrawal Ejm
			{
				nEjmType = 1;
				bHaveSend = TRUE;
			}
			else
			if (strTempArray[0] == "EJMSENDALL")				// Send All Ejm
			{
				nEjmType = 2;
				bHaveSend = TRUE;
			}
			else
			if (strTempArray[0] == "EJMSENDUSER")				// Send User Ejm
			{
				nEjmType = 3;
				bHaveSend = TRUE;
			}

			if (bHaveSend)
			{
				bHaveSend = FALSE;
																// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strInputStartDate = ZERO8 + strTempArray[1];
				strInputStartDate = strInputStartDate.Right(8);
				nInputStartDate = Asc2Int(strInputStartDate);
				strInputEndDate = ZERO8 + strTempArray[2];
				strInputEndDate = strInputEndDate.Right(8);
				nInputEndDate = Asc2Int(strInputEndDate);

				if (( Clerk.FtpIp == "" )		||
					( Clerk.FtpIp == ZERO12 ))
				{
					strTempClerkMessage = GuideMsgTbl[42][OpDeviceLangMode];
				}
				else
				if (( Clerk.FtpPort == "" )		||
					( Clerk.FtpPort == ZERO5 ))
				{
					strTempClerkMessage = GuideMsgTbl[43][OpDeviceLangMode];
				}
				else
				if ( nInputStartDate > nInputEndDate )
				{
					strTempClerkMessage = GuideMsgTbl[38][OpDeviceLangMode];
				}
				else
				{
					CreateDirectory( _AP_TEMPZIP_DIR, NULL );
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );

					for (nInputDate = nInputStartDate; nInputDate <= nInputEndDate; nInputDate++)
					{
						if (((nInputDate % 100) > 31)		||
							(((nInputDate % 10000) / 100) > 12))
							continue;
						strInputDate = Int2Asc(nInputDate);
						ClerkMergeTmpFile(_EJR_DIR, ".TXT", strInputDate, nEjmType);
						Delay_Msg(100);
					}

					if (nEjmType == 1)
					{
						strSrcTraceFile.Format("%s\\EJRWITH%s(%s-%s).ZIP", _AP_TEMPZIP_DIR, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
						strFtpFimeName.Format("EJRWITH%s(%s-%s).ZIP", Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
					}
					else
					if (nEjmType == 2)
					{
						strSrcTraceFile.Format("%s\\EJRALL%s(%s-%s).ZIP", _AP_TEMPZIP_DIR, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
						strFtpFimeName.Format("EJRALL%s(%s-%s).ZIP", Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
					}
					else
					if (nEjmType == 3)
					{
						strSrcTraceFile.Format("%s\\EJRUSER%s(%s-%s).ZIP", _AP_TEMPZIP_DIR, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
						strFtpFimeName.Format("EJRUSER%s(%s-%s).ZIP", Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
					}

					strZipFileExp.Format( "pkzip25 -add %s %s\\*.*", strSrcTraceFile, _AP_TEMPZIP_DIR );
					WinExecWaitEnd( strZipFileExp, SW_HIDE );

					if (ClerkFtpRun(Clerk.FtpIp, Clerk.FtpPort, 
									RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPUSERNAME),
									RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPPASSWORD),
									RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME),
									strSrcTraceFile, 
									strFtpFimeName,
									0))
					{
						strTempClerkMessage = GuideMsgTbl[23][OpDeviceLangMode];
					}											// Successfully Message
					else
					{
						strTempClerkMessage = GuideMsgTbl[44][OpDeviceLangMode];
					}											// Error Message

					DeleteFiles( _AP_TEMPZIP_DIR, -1 );
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmSendProc():return");
	return TRUE;
}

// Clerk Ejm Print Procedure
int CRearApCtrl::ClerkEjmPrintProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmPrintProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	unsigned		nEjmType;
	CString			strSrcTraceFile("");						
	CString			strObjTraceFile("");
	CString			strInputStartDate("");						// 2005.02.24
	CString			strInputEndDate("");
	CString			strInputDate("");
	int				nInputStartDate = 0;
	int				nInputEndDate = 0;
	int				nInputDate = 0;
	BOOL			bHavePrint = FALSE;
	CString			strZipFileExp;

	CString			strPrintFileName("");
	FILE*			TempFp;
	int				nCount = 0;
	char			szBuff[256];
	CString			strPrintData("");

	nScrNum = 918;												// Display Ejm Print
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 41;									// Clerk Ejm Procedure
			break;
		}
		else
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SplitString(strReturn, ":", strTempArray);
			if(strTempArray.GetSize() < 2)
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
			else
			if (strTempArray[0] == "EJMPRINTWITH")				// Print Withdrawal Ejm
			{
				nEjmType = 11;
				bHavePrint = TRUE;
			}
			else
			if (strTempArray[0] == "EJMPRINTALL")				// Print All Ejm
			{
				nEjmType = 12;
				bHavePrint = TRUE;
			}
			else
			if (strTempArray[0] == "EJMPRINTUSER")				// Print User Ejm
			{
				nEjmType = 13;
				bHavePrint = TRUE;
			}

			if (bHavePrint)
			{
				bHavePrint = FALSE;
																// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strInputStartDate = ZERO8 + strTempArray[1];
				strInputStartDate = strInputStartDate.Right(8);
				nInputStartDate = Asc2Int(strInputStartDate);
				strInputEndDate = ZERO8 + strTempArray[2];
				strInputEndDate = strInputEndDate.Right(8);
				nInputEndDate = Asc2Int(strInputEndDate);

				if ( nInputStartDate > nInputEndDate )
				{
					strTempClerkMessage = GuideMsgTbl[38][OpDeviceLangMode];
				}
				else
				{
					CreateDirectory( _AP_TEMPZIP_DIR, NULL );
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );

					for (nInputDate = nInputStartDate; nInputDate <= nInputEndDate; nInputDate++)
					{
						if (((nInputDate % 100) > 31)		||
							(((nInputDate % 10000) / 100) > 12))
							continue;
						strInputDate = Int2Asc(nInputDate);
						ClerkMergeTmpFile(_EJR_DIR, ".TXT", strInputDate, nEjmType);
						Delay_Msg(100);
					}

					strPrintFileName.Format("%s\\TEMPEJM.txt", _AP_TEMPZIP_DIR);
					TempFp = fopen(strPrintFileName, "r");
					if (TempFp)
					{
						nCount = 0;
						while (TRUE)
						{
							memset(szBuff, 0, sizeof(szBuff));
							if (fgets(szBuff, 250, TempFp))
							{
								if (nCount == 0)				// 2006.12.11
									strPrintData = "OP_JPRDIRECTPRINT";
								strPrintData += "||" + GetSprintf("%-40.40s", szBuff);

								nCount++;
								if (nCount == 25)
								{
									SetShareData("OP_COMMAND", strPrintData);
									SetShareData("OP_RESULT", OP_DO);
									if (ClerkWaitResult() == FALSE)
										break;

									nCount = 0;
								}
							}
							else
							{
								if (nCount)
								{
									SetShareData("OP_COMMAND", strPrintData);
									SetShareData("OP_RESULT", OP_DO);
									ClerkWaitResult();
								}
								break;
							}
						}
						fclose(TempFp);
					}
					
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmPrintProc():return");
	return TRUE;
}

// Clerk Ejm Jpr Direct Print Procedure
int CRearApCtrl::ClerkEjmJprDirectPrintProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmJprDirectPrintProc()");

	CString			strPrintFileName("");
	FILE*			TempFp;
	int				nCount = 0;
	int				nTempPosition = 0;
	char			szBuff[256];
	CString			strPrintData("");
	CString			strTempData("");
	CFileStatus		TempFileStatus;

	strPrintFileName.Format("%s\\%s", _EJR_JNL, _EJR_JNL_HISTORY);
	
	if (CFile::GetStatus(strPrintFileName, TempFileStatus))		// File Check
	{
		TempFp = fopen(strPrintFileName, "rb");
		if (TempFp)
		{
			nCount = 0;
			nTempPosition = RegGetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_JNLPOSITION);
			fseek(TempFp, nTempPosition * 42, SEEK_SET);

			while (TRUE)
			{
				memset(szBuff, 0, sizeof(szBuff));
				if (fgets(szBuff, 250, TempFp))					// 1 Line Read
				{
					if (nCount == 0)
						strPrintData = "OP_JPRDIRECTPRINT";
					strTempData.Format("%-40.40s", szBuff);
					strTempData.TrimRight();
					strPrintData += "||" + strTempData;

					nCount++;
					if (nCount == 25)
					{
						SetShareData("OP_COMMAND", strPrintData);
						SetShareData("OP_RESULT", OP_DO);
						if (ClerkWaitResult() == FALSE)
							break;

						nTempPosition += nCount;
						RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_JNLPOSITION, nTempPosition);
						nCount = 0;

						Delay_Msg(2000);						// 2006.05.20
					}
				}
				else
				{
					if (nCount)
					{
						SetShareData("OP_COMMAND", strPrintData);
						SetShareData("OP_RESULT", OP_DO);
						if (ClerkWaitResult() == FALSE)
							break;

						nTempPosition += nCount;
						RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_JNLPOSITION, nTempPosition);
						nCount = 0;
					}
					break;
				}
			}
			
			fclose(TempFp);
			if (ClerkWaitResult())
			{
				DeleteFile(strPrintFileName);
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_JNLPOSITION, 0);
			}
		}
	}
	else														// File Not Found
	{
		strPrintData = "OP_JPRDIRECTPRINT";
		strPrintData += "||";
		SetShareData("OP_COMMAND", strPrintData);
		SetShareData("OP_RESULT", OP_DO);
		ClerkWaitResult();

		RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_JNLPOSITION, 0);
	}
	
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmJprDirectPrintProc():return");
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	PRINT FORMAT
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:   * VERSION INFORMATION *(NNNNNNNN)
// 1:DATE & TIME : 2005.05.19 09:10:12
// 2:AP VERSION  : V04-01-08 (ATM:00000)
// 3:=== SP VERSION ===
// 4:CDU : V 01.00.06
// 5:JPR : V 02.00.08
// 6:SPR : V 02.01.05
// 7:MCU : V 01.00.11
// 8:PIN : V 03.01.03
// 9:CAM : V 02.02.02
//10:SIU : V 02.00.06
//11:UPS : V00.00.00
//12:TTU :
//13:MWI : V04.01.08
//14:COMMONITOR :
//15:=== EP VERSION ===
//16:CDU : V010202
//17:JPR : V00.01.06
//18:SPR : V010001
//19:MCU : 2567-01J
//20:PIN : V03.01.06
//21:CAM : V 01.00.00
//22:SIU : V00.00.00
//23:UPS : V00.00.00
//24:TTU :
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          * WORK PARAMETER *
// 2:========================================
// 3:DATE & TIME        : 2005.05.19 15:53:12
// 4:ATM ID             : 13000002
// 5:BRANCH NUMBER      : 87654321
// 6:HOST IP  (TRAN)    : 192.168.001.21)
// 7:HOST PORT(TRAN)    : 05005
// 8:HOST IP  (JNL)     : 000.000.000.000
// 9:HOST PORT(JNL)     : 00000
//10:ATM IP ADDRESS     : 192.168.001.032
//11:ATM SUBNET MASK    : 255.255.255.000
//12:ATM GATE WAY       : 192.168.001.001
//13:ATM PORT           : 00000
//14:HOST IP  (BID)     : 000.000.000.000
//15:HOST PORT(BID)     : 00000
//16:HOST IP  (FTP)     : 000.000.000.000
//17:HOST PORT(FTP)     : 00000
//18:========================================
//19:
//20:
//21:
//22:
//23:
//24:
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:       * TRANSACITON STATISTICS *
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:ATM ID             : NNNNNNNN
// 5:BRANCH NUMBER      : NNNNNNNN
// 6:BEGIN DATE & TIME  : 2004.05.01 12:12:12 
// 7:END   DATE & TIME  : 2004.05.01 12:12:12
// 8:
// 9:WITHDRAW     COUNT : NNNNNNNNNN
//10:WITHDRAW    AMOUNT : NNNNNNNNNNNNNNNN.NN
//11:
//12:INQUIRY      COUNT : NNNNNNNNNN
//13:
//14:TRANSFER     COUNT : NNNNNNNNNN
//15:TRANSFER    AMOUNT : NNNNNNNNNNNNNNNN.NN
//16:
//17:DEPOSIT      COUNT : NNNNNNNNNN
//18:DEPOSIT     AMOUNT : NNNNNNNNNNNNNNNN.NN
//19:
//20:PASSBOOK     COUNT : NNNNNNNNNN
//21:
//22:IC           COUNT : NNNNNNNNNN
//23:IC          AMOUNT : NNNNNNNNNNNNNNNN.NN
//24:========================================
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          * ERROR STATISTICS *
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:BRANCH NUMBER      : NNNNNNNN
// 5:ATM ID             : NNNNNNNN
// 6:BEGIN DATE & TIME  : 2004.05.01 12:12:12
// 7:END   DATE & TIME  : 2004.05.01 12:12:12
// 8:
// 9:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//10:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//11:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//12:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//13:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//14:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//15:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//16:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//17:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//18:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//19:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//20:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//21:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//22:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//23:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
//24:P-EEEEE(SS) CCCCCC YYYY.MM.DD YYYY.MM.DD
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Clerk Check Driver Is Install
CString CRearApCtrl::ClerkCheckDriveIsInstall(unsigned nDriveType)
{
	DWORD			dwNumBytesForDriveStrings;
	HANDLE			hHeap;
	LPSTR			lp;
	CString			strResult = "";
	unsigned		nCurrDriveType;

	dwNumBytesForDriveStrings = GetLogicalDriveStrings( 0, NULL );
	if ( dwNumBytesForDriveStrings != 0 )
	{
		hHeap = GetProcessHeap();
		lp = (LPSTR)HeapAlloc( hHeap, HEAP_ZERO_MEMORY, dwNumBytesForDriveStrings );
		GetLogicalDriveStrings( HeapSize( hHeap, 0, lp ), lp );
		while ( *lp != 0 )
		{
			nCurrDriveType = GetDriveType( lp );
			if ( nDriveType == 7 )
			{
				if ( ( nCurrDriveType == DRIVE_REMOVABLE ) && ( !strcmp( lp, "A:\\" ) ) )
				{
					strResult = lp;
					break;
				}
			}
			else if ( nDriveType == 8 )
			{
				if ( ( nCurrDriveType == DRIVE_REMOVABLE ) && ( strcmp( lp, "A:\\" ) ) )
				{
					strResult = lp;
					break;
				}
			}
			else if ( nDriveType == nCurrDriveType )
			{
				strResult = lp;
				break;
			}
			lp = _tcschr( lp, 0 ) + 1;
		}
	}

	return strResult;
}

// Clerk Check Disk Is Insert
int CRearApCtrl::ClerkCheckDiskIsInsert(CString strDriveNum)
{
    DWORD			SectorsPerCluster;
	DWORD			BytesPerSector;
	DWORD			NumberOfFreeClusters;
	DWORD			TotalNumberOfClusters;

	int nDiskFreeSpace = GetDiskFreeSpace( strDriveNum, 
		&SectorsPerCluster, &BytesPerSector, 
		&NumberOfFreeClusters, &TotalNumberOfClusters );

	return nDiskFreeSpace;
}

// Clerk Check Disk Free Space
DWORD CRearApCtrl::ClerkCheckDiskFreeSpace(CString strDriveNum)
{
	DWORD			SectorsPerCluster;
	DWORD			BytesPerSector;
	DWORD			NumberOfFreeClusters;
	DWORD			TotalNumberOfClusters;
	DWORD			FreeBytes;

	int nDiskFreeSpace = GetDiskFreeSpace( strDriveNum, 
		&SectorsPerCluster, &BytesPerSector, 
		&NumberOfFreeClusters, &TotalNumberOfClusters );

	FreeBytes = SectorsPerCluster * BytesPerSector * NumberOfFreeClusters;

	return FreeBytes;
}

// Clerk Copy File To Temp Directory
int CRearApCtrl::ClerkCopyTmpFile(CString strPath, CString strFileType, CString strInputDate)
{
	CFileFind		finder;
	CString			dir, file;
	int				nCopyOk;
	CFileStatus		fileStatus;
	CString			strDirSeperator("\\");

	dir.Format("%s\\%s", strPath, "*.*");
	BOOL bWorking = finder.FindFile(dir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		dir  = finder.GetFilePath();
		file = finder.GetFileName();
		
		if(	file == _T(".") ||
			file == _T(".."))	continue;
		
		if(finder.IsDirectory())
		{
			if(!ClerkCopyTmpFile(dir, strFileType, strInputDate))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!CFile::GetStatus(dir, fileStatus))
			{
				finder.Close();
				return FALSE;
			}

			CTime t1(fileStatus.m_mtime.GetYear(), fileStatus.m_mtime.GetMonth(), 
				fileStatus.m_mtime.GetDay(), 0, 0, 0);

			if ((strFileType.CompareNoCase(file.Right(strFileType.GetLength())) == 0) && 
				((t1.Format("%Y%m%d") == strInputDate) || (ZERO8 == strInputDate)))
			{
				nCopyOk = CopyFile( dir, _AP_TEMPZIP_DIR + strDirSeperator + file, 0 );
				if ( nCopyOk == 0 )
				{
					finder.Close();
					return FALSE;
				}
			}
		}
	}
	finder.Close();

	return TRUE;
}

// Clerk Merge File In Temp Directory
int CRearApCtrl::ClerkMergeTmpFile(CString strPath, CString strFileType, CString strInputDate, int nEjmType)
{
	CFileFind		finder;
	CString			dir, file;
	CFileStatus		fileStatus;
	CString			strDirSeperator("\\");
	CString			strSrcFileName("");
	CString			strDesFileName("");
	CString			strDes2FileName("");
	FILE*			SrcFileNameFp;
	FILE*			DesFileNameFp;
	FILE*			Des2FileNameFp;
	CString			strTemp("");

	EjrTbl			TempEjr;
	int				nTempFReadNum = 0;
	char			szTemp[256];
	BOOL			bHaveWrite = FALSE;
	BOOL			bHaveWriteLine = FALSE;

	dir.Format("%s\\%s", strPath, "*.*");
	BOOL bWorking = finder.FindFile(dir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		dir  = finder.GetFilePath();
		file = finder.GetFileName();
		
		if(	file == _T(".") ||
			file == _T(".."))	continue;
		
		if(finder.IsDirectory())
		{
			if(!ClerkMergeTmpFile(dir, strFileType, strInputDate, nEjmType))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!CFile::GetStatus(dir, fileStatus))
			{
				finder.Close();
				return FALSE;
			}
			
			CTime t1(fileStatus.m_mtime.GetYear(), fileStatus.m_mtime.GetMonth(), 
				fileStatus.m_mtime.GetDay(), 0, 0, 0);

			if ((strFileType.CompareNoCase(file.Right(strFileType.GetLength())) == 0) && 
				(t1.Format("%Y%m%d") == strInputDate))			// 2006.01.04
			{
				strSrcFileName = dir;
				SrcFileNameFp = fopen(strSrcFileName, "rb");
				if (!SrcFileNameFp)
					break;

				strDesFileName.Format("%s\\TEMPEJM.txt", _AP_TEMPZIP_DIR);
				DesFileNameFp = fopen(strDesFileName, "a");
				if (!DesFileNameFp)
				{
					fclose(SrcFileNameFp);
					break;
				}

				strDes2FileName.Format("%s\\%s", _AP_TEMPZIP_DIR, file);
				Des2FileNameFp = fopen(strDes2FileName, "a");
				if (!Des2FileNameFp)
				{
					fclose(SrcFileNameFp);
					fclose(DesFileNameFp);
					break;
				}

				while (TRUE)
				{				
					memset(&TempEjr, 0, sizeof(TempEjr));
					nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, SrcFileNameFp);
					sprintf(szTemp, "%%%d.%ds", sizeof(TempEjr), sizeof(TempEjr));
					if (nTempFReadNum)
						strTemp.Format(szTemp, (LPSTR)&TempEjr);
					else
						break;
					
					bHaveWrite = FALSE;
					bHaveWriteLine = FALSE;
					switch (nEjmType)							// 2006.08.22
					{
						case 1:									// Withdrawal Copy & Send
						case 11:								// Withdrawal Print
							if ((strTemp.Find("TRAN TYPE     : WITHDRAWAL")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : CWD")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRANSFER")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : TFR")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRF")		>= 0))
								bHaveWrite = TRUE;
							if (strTemp.Find("TRAN TYPE     : ") >= 0)
								bHaveWriteLine = TRUE;
							break;

						case 2:									// All Copy & Send
						case 12:								// All Print
							bHaveWrite = TRUE;
							if (strTemp.Find("TRAN TYPE     : ") >= 0)
								bHaveWriteLine = TRUE;
							break;

						case 3:									// User Copy & Send
						case 13:								// User Print
							if ((strTemp.Find("TRAN TYPE     : WITHDRAWAL")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : CWD")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : INQUIRY")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : INQ")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : DETAIL")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRANSFER")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : TFR")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : TRF")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : CHANGEPIN")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : PIN")		>= 0)	||
								(strTemp.Find("TRAN TYPE     : CHECKPIN")	>= 0)	||
								(strTemp.Find("TRAN TYPE     : CWC")		>= 0))
								bHaveWrite = TRUE;
							if (strTemp.Find("TRAN TYPE     : ") >= 0)
								bHaveWriteLine = TRUE;
							break;
					}

					if (bHaveWrite)
					{
						for (int i = 0; i < JPR_MAX_ROW; i++)
						{
							strTemp = GetString(TempEjr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
							strTemp.TrimRight();
							if (strTemp.GetLength())
							{
								if ((nEjmType == 1)	||			// Copy & Send
									(nEjmType == 2)	||
									(nEjmType == 3))
									fprintf(Des2FileNameFp, "%s\n", strTemp);

								if ((nEjmType == 11)	||		// Print
									(nEjmType == 12)	||
									(nEjmType == 13))
									fprintf(DesFileNameFp, "%s\n", strTemp);
							}
						}
						
						if (bHaveWriteLine)
						{
							if ((nEjmType == 1)	||				// Copy & Send
								(nEjmType == 2)	||
								(nEjmType == 3))
								fprintf(Des2FileNameFp, "********************************\n");

							if ((nEjmType == 11)	||			// Print
								(nEjmType == 12)	||
								(nEjmType == 13))
								fprintf(DesFileNameFp, "********************************\n");
						}
					}
				}
				
				fclose(SrcFileNameFp);
				fclose(DesFileNameFp);
				fclose(Des2FileNameFp);

				if ((nEjmType == 1)	||							// Copy & Send
					(nEjmType == 2)	||
					(nEjmType == 3))
					DeleteFile(strDesFileName);
			}
		}
	}
	finder.Close();

	return TRUE;
}

// Clerk Copy  Ej File To Temp Directory
int CRearApCtrl::ClerkCopyEJFile(CString strPath, CString strFileType, CString strInputDate)
{
	CFileFind		finder;
	CString			dir, file;
	int				nCopyOk;
	CFileStatus		fileStatus;
	CString			strDirSeperator("\\");

	dir.Format("%s\\%s", strPath, "*.*");
	BOOL bWorking = finder.FindFile(dir);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		
		dir  = finder.GetFilePath();
		file = finder.GetFileName();
		
		if(	file == _T(".") ||
			file == _T(".."))	continue;
		
		if(finder.IsDirectory())
		{
			if(!ClerkCopyEJFile(dir, strFileType, strInputDate))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!CFile::GetStatus(dir, fileStatus))
			{
				finder.Close();
				return FALSE;
			}

			CTime t1(fileStatus.m_mtime.GetYear(), fileStatus.m_mtime.GetMonth(), 
				fileStatus.m_mtime.GetDay(), 0, 0, 0);

			if ((strFileType.CompareNoCase(file.Right(strFileType.GetLength())) == 0) && 
				((t1.Format("%Y%m%d").Right(6) == strInputDate.Left(6)) || (ZERO8 == strInputDate)))
			{
				nCopyOk = CopyFile( dir, _AP_TEMPZIP_DIR + strDirSeperator + file, 0 );
				if ( nCopyOk == 0 )
				{
					finder.Close();
					return FALSE;
				}
			}
		}
	}
	finder.Close();

	return TRUE;
}

// Clerk Ftp Procedure
int CRearApCtrl::ClerkFtpRun(CString strIP, CString strPort, CString strUserName, CString strPassword, CString strServerDir, CString strLocalFileName, CString strFtpFileName, int nRunType)
{
	HINTERNET	hInternet, hFtp;
	CString		strPutFtpFile("");
	CString		strPutAtmFile("");
	CString		strGetFtpFile("");
	CString		strGetAtmFile("");
	char		szIpAddress[256];
	int			nFtpPort;

	memset(szIpAddress, 0, sizeof(szIpAddress));
	sprintf(szIpAddress, "%d.%d.%d.%d", Asc2Int(strIP.Mid(0,3), 3),
										Asc2Int(strIP.Mid(3,3), 3),
										Asc2Int(strIP.Mid(6,3), 3),
										Asc2Int(strIP.Mid(9,3), 3));
	strIP.Format("%s", szIpAddress);

	nFtpPort = Asc2Int(strPort);

	strPutFtpFile	= strFtpFileName;
	strPutAtmFile	= strLocalFileName;

	strGetFtpFile	= "";
	strGetAtmFile	= "";

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun(): IpAddress[%s] Port[%d] UserName[%s], PassWord[%s]", 
													  strIP, 
													  nFtpPort, 
													  strUserName, 
													  strPassword);

	hInternet = InternetOpen("FTPTEST", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternet)
	{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun():FTP Internet Open Fail");
		return FALSE;
	}

	hFtp = InternetConnect(hInternet, 
						   strIP,
						   nFtpPort, 
						   strUserName, 
						   strPassword, 
						   INTERNET_SERVICE_FTP, 
						   0, 
						   0);
	if (!hFtp) 
	{
		InternetCloseHandle(hInternet);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun():FTP Connect Fail");
		return FALSE;
	}
	
	if (!FtpSetCurrentDirectory(hFtp, strServerDir))
	{
		if (!FtpCreateDirectory(hFtp, strServerDir))
		{
			InternetCloseHandle(hFtp);
			InternetCloseHandle(hInternet);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun():FtpSetCurrentDirectory Error");
			return FALSE;
		}
	}

	switch (nRunType)
	{
		case 0:													// Upload
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun(): [FtpPut] FtpFileName[%s] LocalFileName[%s]",
													  strPutFtpFile, 
													  strPutAtmFile);
			if (!FtpPutFile(hFtp, 
							strPutAtmFile, 
							strPutFtpFile, 
							FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD,
							0)) 
			{
				InternetCloseHandle(hFtp);
				InternetCloseHandle(hInternet);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun():FtpPutFile Error");
				return FALSE;
			}
			break;
			
		case 1:													// Download
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun(): [FtpGet] GetFtpFileName[%s] GetAtmLocalFilaName[%s]",
													  strGetFtpFile, 
													  strGetAtmFile);
			if (!FtpGetFile(hFtp, 
							strGetFtpFile, 
							strGetAtmFile, 
							TRUE, 
							FILE_ATTRIBUTE_NORMAL, 
							FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 
							1)) 
			{
				InternetCloseHandle(hFtp);
				InternetCloseHandle(hInternet);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkFtpRun():FtpGetFile Error");
				return FALSE;
			}
			break;
	}
		
	InternetCloseHandle(hFtp);
	InternetCloseHandle(hInternet);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Clerk Information Make
CString	CRearApCtrl::ClerkInformationMake(int nIndex)
{
	CString			strTempClerkInformation("");
	CString			strTempClerkInformation2("");
	CStringArray	strTempClerkInformationArray;
	static CString	strSaveClerkInformation("");
	CString			strClerkInformation("");
	int				i = 0;
//	int				SprSet = Asc2Int(IniGetStr(_TRANSACTION_INI,"TRANSACTION","SPRSET","0"));	// added by liuxl 20110407
//	CString			strVersion = IniGetStr(_TRANSACTION_INI,"TRANSACTION","SPROPTMODE","0");

	switch	(nIndex)
	{
		case 0:													// Wait for ATM Initial End
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1:													// Clerk Select Mode Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 0; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		case 11:												// Clerk Op Input Pin Procedure
			break;

		case 12:												// Clerk Op Mode Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 0; i <= 4; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				// check Auth and Display   by zjd
				int iAuthResult = hy_CheckAuthorithCodeFromFile3();
				if (iAuthResult == AUTH_OK)
					strClerkInformation += strTempClerkInformationArray[5] + "\n";
				else if(iAuthResult == AUTH_FAIL)
					strClerkInformation += "[License: FAIL! ]\n";
				else
					strClerkInformation += "[License: TRYOUT]\n";

				for (i = 6; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

		case 13:												// Clerk Op Input Change Pin Procedure
			break;

		case 14:												// Clerk Add Cash Procedure
			strTempClerkInformation = GetShareData("OP_ADDCASH");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 20)
			{
				for (i = 0; i <= 18; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[19];
			}
			break;

		case 15:												// Clerk Cash Value Procedure
			strTempClerkInformation = GetShareData("OP_CASHVALUE");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 4)
			{
				for (i = 0; i <= 2; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[3];
			}
			break;

		case 16:												// Clerk Total Procedure
			strTempClerkInformation = GetShareData("OP_TOTAL");	// 2005.08.31
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 2)
			{
				for (i = 0; i <= 0; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[1];
			}
			break;

		case 17:												// Clerk Special Mode Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 6; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

		case 18:												// Clerk Set Param Procedure 
			strTempClerkInformation = GetShareData("OP_SETPARAM");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 30)	// liyi add 2008.09.23
			{			
				for (i = 0; i <= 28; i++)						
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[29];
			}
			break;

		case 19:												// Clerk Set Master Key Procedure
			break;

/////////////////////////////////////////////////////////////////////////////
		case 21:												// Clerk As Input Pin Procedure
			break;

		case 22:												// Clerk As Mode Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 6; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

		case 23:												// Clerk As Input Change Pin Procedure
			break;

		case 24:												// Clerk Ap Update Procedure
			strTempClerkInformation = GetShareData("OP_APUPDATE");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 2)
			{
				for (i = 0; i <= 0; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[1];
			}
			break;

		case 25:												// Clerk Trace Copy Procedure
			strTempClerkInformation = GetShareData("OP_TRACECOPY");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 3)
			{
				for (i = 0; i <= 1; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[2];
			}
			break;

		case 26:												// Clerk Version Procedure
			strTempClerkInformation = GetShareData("OP_VERSION");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 22)
			{
				for (i = 0; i <= 20; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[21];
			}
			break;

		case 27:												// Clerk Work Parameter Procedure
			strTempClerkInformation = GetShareData("OP_WORKPARAM");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 15)
			{
				for (i = 0; i <= 13; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[14];
			}
			break;

		case 28:												// Clerk DB Initialization Procedure
			strTempClerkInformation = GetShareData("OP_DBINITIAL");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 2)
			{
				for (i = 0; i <= 0; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[1];
			}
			break;

		case 29:												// Clerk Other Operation Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 6; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		case 31:												// Clerk Administrator Login Procedure
			break;

		case 32:												// Clerk Super Administrator Mode Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 6; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

		case 33:												// Clerk Add Administrator Procedure
			break;

		case 34:												// Clerk Delete Administrator Procedure
			break;

		case 35:												// Clerk Change Administrator Pin Procedure
			break;

/////////////////////////////////////////////////////////////////////////////
		case 41:												// Clerk Ejm Procedure
			strTempClerkInformation = GetShareData("OP_HEADER");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 26)
			{
				for (i = 6; i <= 24; i++)
// 					if (strVersion == "1" && SprSet)
// 						if ((i == 12 && strTempClerkInformationArray[i] == "故障") 
// 							|| (i == 20 && strTempClerkInformationArray[i] == "无纸"))
// 						{
// 							strClerkInformation += "正常";
// 							strClerkInformation += "\n";
// 						}
// 						else
// 							strClerkInformation += strTempClerkInformationArray[i] + "\n";
// 					else
						strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[25];
			}
			break;

		case 42:												// Clerk Ejm View Procedure
			strTempClerkInformation = GetShareData("OP_EJMVIEW");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 3)
			{
				for (i = 0; i <= 1; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[2];
			}
			break;

		case 43:												// Clerk Ejm Result Procedure
			if (Ejm.Count == 0)
			{
				for (i = 0; i <= 30; i++)						// 2005.11.11
					strClerkInformation += "\n";
				break;
			}

			for (i = 0; i <= 24; i++)
			{
				strTempClerkInformation.Format("%40.40s", Ejm.EjrData.Data[i]);
				strTempClerkInformation.TrimRight();
				if (strTempClerkInformation.GetLength())
					strClerkInformation += strTempClerkInformation + "\n";
				else
					strTempClerkInformation2 += "\n";
			}
			strClerkInformation += strTempClerkInformation2 + "\n";
																// 2004.07.05
			if (!IsSpace(Ejm.EjrData.AccountNum, sizeof(Ejm.EjrData.AccountNum)))
			{
				strClerkInformation += GetSprintf("%s%4.4s\\%4.4s%6.6s1.JPG", 
										_EJR_IMG,				// Pic 1
										&Ejm.EjrData.Date[4],
										&Ejm.EjrData.Date[4],
										Ejm.EjrData.TranSerialNo) + "\n";
				strClerkInformation += GetSprintf("%s%4.4s\\%4.4s%6.6s2.JPG", 
										_EJR_IMG,				// Pic 2
										&Ejm.EjrData.Date[4],
										&Ejm.EjrData.Date[4],
										Ejm.EjrData.TranSerialNo) + "\n";
				strClerkInformation += GetSprintf("%s%4.4s\\%4.4s%6.6s3.JPG",
										_EJR_IMG,				// Pic 3
										&Ejm.EjrData.Date[4],
										&Ejm.EjrData.Date[4],
										Ejm.EjrData.TranSerialNo) + "\n";
				strClerkInformation += GetSprintf("%s%4.4s\\%4.4s%6.6s4.JPG",
										_EJR_IMG,				// Pic 4
										&Ejm.EjrData.Date[4],
										&Ejm.EjrData.Date[4],
										Ejm.EjrData.TranSerialNo) + "\n";
				strClerkInformation += GetSprintf("%s%4.4s\\%4.4s%6.6s5.JPG",
										_EJR_IMG,				// Pic 5 : 2005.11.11
										&Ejm.EjrData.Date[4],
										&Ejm.EjrData.Date[4],
										Ejm.EjrData.TranSerialNo) + "\n";
				strClerkInformation += GetSprintf("%s%4.4s\\%4.4s%6.6s6.JPG",
										_EJR_IMG,				// Pic 6 : 2005.11.11
										&Ejm.EjrData.Date[4],
										&Ejm.EjrData.Date[4],
										Ejm.EjrData.TranSerialNo);
			}
			break;

		case 44:												// Clerk Ejm Copy Procedure
			strTempClerkInformation = GetShareData("OP_EJMCOPY");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 4)
			{
				for (i = 0; i <= 2; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[3];
			}
			break;

		case 45:												// Clerk Ejm Send Procedure
			strTempClerkInformation = GetShareData("OP_EJMSEND");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 4)
			{
				for (i = 0; i <= 2; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[3];
			}
			break;

		case 46:												// Clerk Ejm Print Procedure
			strTempClerkInformation = GetShareData("OP_EJMPRINT");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 4)
			{
				for (i = 0; i <= 2; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[3];
			}
			break;

		case 65:               // yaokq add 20081029
			strTempClerkInformation = GetShareData("OP_VIEW_ATMCTOTALS");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 32)	// yaokq add 20081029
			{			
				for (i = 0; i <= 30; i++)						
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[31];
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		case 100:												// Led Display
			strTempClerkInformation = GetShareData("OP_LED");
			SplitString(strTempClerkInformation, "||", strTempClerkInformationArray);
			if (strTempClerkInformationArray.GetSize() >= 8)
			{
				for (i = 0; i <= 6; i++)
					strClerkInformation += strTempClerkInformationArray[i] + "\n";
				strClerkInformation += strTempClerkInformationArray[7];
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		default:
			break;
	}

	if (strSaveClerkInformation != strClerkInformation)			// 2005.03.30
	{
		strSaveClerkInformation = strClerkInformation;
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInformationMake:nIndex(%d), strClerkInformation(%s)", nIndex, strClerkInformation);
	}

	return strClerkInformation;
}

// Clerk Ejm Record Find
int CRearApCtrl::ClerkEjmRecordFind(int nFindKind)	
{
	char		szTempFileName[256];
	CFileStatus	TempFileStatus;
	FILE*		TempFp;
	EjrTbl		TempEjr;
	int			nTempFReadNum = 0;
	CString		strTempTranSerialNo("");
	CString		strTempAccountNum("");

/////////////////////////////////////////////////////////////////////////////
	memset(&Ejm.EjrData, 0, sizeof(Ejm.EjrData));
	sprintf(szTempFileName, "%s%4.4s\\%4.4s.TXT", 
							_EJR_TXT, 
							&(Ejm.Date.GetBuffer(0)[4]), 
							&(Ejm.Date.GetBuffer(0)[4]));

/////////////////////////////////////////////////////////////////////////////
	if (nFindKind == 0)											// Count Find 
	{
		Ejm.Count = 0;
		if (CFile::GetStatus(szTempFileName, TempFileStatus))
		{
			TempFp = fopen(szTempFileName, "rb");
			if (TempFp)
			{
				nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, TempFp);
				if ((nTempFReadNum)		&&						
					(memcmp(TempEjr.Date, Ejm.Date.GetBuffer(0), sizeof(TempEjr.Date)) == 0))
				{												// Delete(1 Year Ago Data)
					fseek(TempFp, 0, SEEK_END);
					Ejm.Count = ftell(TempFp) / sizeof(TempEjr);
				}
				fclose(TempFp);
			}
		}
	}
	else
	if (nFindKind == 1)											// Data Find(Position)
	{
		Ejm.FindOkFlag = FALSE;
		if (CFile::GetStatus(szTempFileName, TempFileStatus))
		{
			TempFp = fopen(szTempFileName, "rb");
			if (TempFp)
			{
				fseek(TempFp, Ejm.Position * sizeof(TempEjr), SEEK_SET);
				nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, TempFp);
				if (nTempFReadNum)
				{
					memcpy(&Ejm.EjrData, &TempEjr, sizeof(Ejm.EjrData));
					Ejm.FindOkFlag = TRUE;
				}
				fclose(TempFp);
			}
		}
	}
	else
	if (nFindKind == 2)											// Data Find(TranSerialNo & AccountNum)
	{
		Ejm.FindOkFlag = FALSE;
		Ejm.Position = 0;
		if (CFile::GetStatus(szTempFileName, TempFileStatus))
		{
			TempFp = fopen(szTempFileName, "rb");
			if (TempFp)
			{
				nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, TempFp);
				if ((nTempFReadNum)		&&						
					(memcmp(TempEjr.Date, Ejm.Date.GetBuffer(0), sizeof(TempEjr.Date)) == 0))
				{												// Delete(1 Year Ago Data)
					fseek(TempFp, 0, SEEK_SET);
					while (TRUE)
					{
						nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, TempFp);
						if (nTempFReadNum)
						{										// 2005.12.01
							strTempTranSerialNo.Format("%8.8s", TempEjr.TranSerialNo);
							strTempAccountNum.Format("%24.24s", TempEjr.AccountNum);
																// 2005.12.23
							if ((Ejm.TranSerialNo.GetLength())	&&
								(Ejm.AccountNum.GetLength()))
							{
								if ((strTempTranSerialNo.Find(Ejm.TranSerialNo)	>= 0)	&&
									(strTempAccountNum.Find(Ejm.AccountNum) >= 0))
								{
									memcpy(&Ejm.EjrData, &TempEjr, sizeof(Ejm.EjrData));
									Ejm.FindOkFlag = TRUE;
									break;
								}
							}
							else
							if (Ejm.TranSerialNo.GetLength())
							{
								if (strTempTranSerialNo.Find(Ejm.TranSerialNo)	>= 0)
								{
									memcpy(&Ejm.EjrData, &TempEjr, sizeof(Ejm.EjrData));
									Ejm.FindOkFlag = TRUE;
									break;
								}
							}
							else
							if (Ejm.AccountNum.GetLength())
							{
								if (strTempAccountNum.Find(Ejm.AccountNum) >= 0)
								{
									memcpy(&Ejm.EjrData, &TempEjr, sizeof(Ejm.EjrData));
									Ejm.FindOkFlag = TRUE;
									break;
								}
							}
							Ejm.Position++;
						}
						else
						{
							if (Ejm.Position)
							{
								Ejm.Position--;
								memcpy(&Ejm.EjrData, &TempEjr, sizeof(Ejm.EjrData));
							}
							break;
						}
					}
				}
				fclose(TempFp);
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkEjmRecordFind:return(nFindKind[%d], Ejm.Count[%d], Ejm.FindOkFlag[%d], Ejm.Position[%d])", nFindKind, Ejm.Count, Ejm.FindOkFlag, Ejm.Position);
	return TRUE;
}

// Clerk Status Update
int CRearApCtrl::ClerkStatusUpdate(int InitFlag)
{
/////////////////////////////////////////////////////////////////////////////
// OP_STATUS
// 0:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
// 1:OPEN STATUS(ON,OFF)
// 2:SEND RECV STATUS(ON,OFF)
// 3:TRANSACTION STATUS(ON,OFF)
// 4:POWER STATUS(ON,OFF)
// 5:AS PASSWORD
// 6:OP PASSWORD
// 7:SCR BANK NAME
// 8:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
// 9:OPERATOR SWITCH STATUS(NONE,SUPERVISOR,RUN,SUPERVISORCHANGED,RUNCHANGED)
// 10:FTP IP(NNNNNNNNNNNN)
// 11:FTP PORT(NNNNN)
// 12:ATM SERIAL NUMBER(NNNNNNNN)
// 13:BRANCH NUMBER(NNNNNNNN)
// 14:AP VERSION(V00-00-00)
// 15:TRANSACTION DATE(YYYYMMDD)
// 16:INTERFACE KIND(N)
// 17:OP DEVICE(NOT_SET, OP_TOUCH, OP_TSPL, OP_HSPL)
// 18:OP LED(POWERLED,COMMLED,INSERVICELED,SUPERVISORLED,ERRORLED,CARDLED,REPORTLED,NOTELED)
// 19:SPL DEVICE(NOT_SET, SPL_TOGGLESW, SPL_ONOFFSW)
/////////////////////////////////////////////////////////////////////////////
	static int		nFirstFlag = TRUE;
	static int		nFirstCount = 0;
	CString			strStatusInfo("");
	static CString	strSaveStatusInfo("");
	CStringArray	strStatusArray;
	
/////////////////////////////////////////////////////////////////////////////
	if (!nFirstFlag)											// 2005.03.30
		;
	else
	if ((nFirstFlag)	&&
		(GetShareData("OP_APSTATUS").CompareNoCase("APSTART") == 0))
	{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_APSTATUS(%s)", GetShareData("OP_APSTATUS"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_MODE(%s)", GetShareData("OP_MODE"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", GetShareData("OP_STATUS"));
		Delay_Msg(2000);										// 2005.04.03
		nFirstFlag = FALSE;
		SetShareData("OP_APSTATUS", "APRUN");
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_APSTATUS(%s)", GetShareData("OP_APSTATUS"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_MODE(%s)", GetShareData("OP_MODE"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", GetShareData("OP_STATUS"));
	}
	else														// 2005.03.30
	{
		Delay_Msg(2000);										// 2005.04.03
		nFirstCount++;
		if (nFirstCount > 15)									// 2005.04.03 : 30 SEC
		{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_APSTATUS(%s)", GetShareData("OP_APSTATUS"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_MODE(%s)", GetShareData("OP_MODE"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", GetShareData("OP_STATUS"));
			nFirstFlag = FALSE;
			SetShareData("OP_APSTATUS", "APRUN");
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_APSTATUS(%s)", GetShareData("OP_APSTATUS"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_MODE(%s)", GetShareData("OP_MODE"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", GetShareData("OP_STATUS"));
		}
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_APSTATUS(%s), nFirstCount(%d)", GetShareData("OP_APSTATUS"), nFirstCount);
		return FALSE;
	}

/////////////////////////////////////////////////////////////////////////////
	if (GetShareData("OP_MODE").CompareNoCase("OPOK") == 0)
	{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_APSTATUS(%s)", GetShareData("OP_APSTATUS"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_MODE(%s)", GetShareData("OP_MODE"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", GetShareData("OP_STATUS"));
		SetShareData("OP_MODE", "OPDO");
		SetShareData("OP_OPSTATUS", "OPDO");					// 2006.01.10
		Clerk.Index = 1;										// Main Screen
		Clerk.UpdateScreen = TRUE;
	}
	else
	if (GetShareData("OP_OPSTATUS").CompareNoCase("OP") == 0)
	{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_OPSTATUS(%s)", GetShareData("OP_OPSTATUS"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_MODE(%s)", GetShareData("OP_MODE"));
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", GetShareData("OP_STATUS"));
		SetShareData("OP_OPSTATUS", "OPDO");					// 2006.01.10
		if ((Clerk.OpDevice == "OP_TSPL")		||				// 2006.01.10
			(Clerk.OpDevice == "OP_TSPLCHN")	||				// 2007.04.10
			(Clerk.OpDevice == "OP_HSPL"))
			Clerk.UpdateScreen = TRUE;
	}

/////////////////////////////////////////////////////////////////////////////
	strStatusInfo = GetShareData("OP_STATUS");
	if (strStatusInfo == "")
		return FALSE;
	
	if ((InitFlag) ||
		(strStatusInfo != strSaveStatusInfo))
		;
	else
		return FALSE;

	SplitString(strStatusInfo, "||", strStatusArray);			// Split String

	if (strStatusArray.GetSize() < 20)							// 2005.11.11
		return FALSE;

/////////////////////////////////////////////////////////////////////////////
	strSaveStatusInfo		= strStatusInfo;					// Status Save
	Clerk.AtmStatus			= strStatusArray[0];				// 0:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
	Clerk.OpenStatus		= strStatusArray[1];				// 1:OPEN STATUS(ON,OFF)
	Clerk.SendRecvStatus	= strStatusArray[2];				// 2:SEND RECV STATUS(ON,OFF)
	Clerk.TransactionStaus	= strStatusArray[3];				// 3:TRANSACTION STATUS(ON,OFF)
	Clerk.PowerStaus		= strStatusArray[4];				// 4:POWER STATUS(ON,OFF)
	Clerk.AsPassWord		= strStatusArray[5];				// 5:AS PASSWORD
	Clerk.OpPassWord		= strStatusArray[6];				// 6:OP PASSWORD
	Clerk.ScrBankName		= strStatusArray[7];				// 7:SCR BANK NAME
	Clerk.MachineKind		= strStatusArray[8];				// 8:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
	Clerk.OpSwitchStatus	= strStatusArray[9];				// 9:OPERATOR SWITCH STATUS(NONE,SUPERVISOR,RUN,SUPERVISORCHANGED,RUNCHANGED)
	Clerk.FtpIp				= strStatusArray[10];				// 10:FTP IP(NNNNNNNNNNNN)
	Clerk.FtpPort			= strStatusArray[11];				// 11:FTP PORT(NNNNN)
	Clerk.AtmSerialNumber	= strStatusArray[12];				// 12:ATM SERIAL NUMBER(NNNNNNNN)
	Clerk.BranchNumber		= strStatusArray[13];				// 13:BRANCH NUMBER(NNNNNNNN)
	Clerk.ApVersion			= strStatusArray[14];				// 14:AP VERSION(V00-00-00)
	Clerk.TransactionDate	= strStatusArray[15];				// 15:TRANSACTION DATE(YYYYMMDD)
	Clerk.InterfaceKind		= strStatusArray[16];				// 16:INTERFACE KIND(N)
	Clerk.OpDevice			= strStatusArray[17];				// 17:OP DEVICE(NOT_SET, OP_TOUCH, OP_TSPL, OP_HSPL)
	Clerk.OpLed				= strStatusArray[18];				// 18:OP LED(POWERLED,COMMLED,INSERVICELED,SUPERVISORLED,ERRORLED,CARDLED,REPORTLED,NOTELED)
	Clerk.SplDevice			= strStatusArray[19];				// 19:SPL DEVICE(NOT_SET, SPL_TOGGLESW, SPL_ONOFFSW)

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkStatusUpdate:OP_STATUS(%s)", strStatusInfo);

/////////////////////////////////////////////////////////////////////////////
	if (Clerk.OpDevice == "NOT_SET")							// 2005.11.11
	{
		OpDeviceLangMode = CHN_MODE;
	}
	else
	if (Clerk.OpDevice == "OP_TOUCH")
	{
		OpDeviceLangMode = CHN_MODE;
	}
	else
	if (Clerk.OpDevice == "OP_TSPL")
	{
		OpDeviceLangMode = ENG_MODE;
	}
	else
	if (Clerk.OpDevice == "OP_TSPLCHN")
	{
		OpDeviceLangMode = CHN_MODE;
	}
	else
	if (Clerk.OpDevice == "OP_HSPL")
	{
		OpDeviceLangMode = ENG_MODE;
	}

/////////////////////////////////////////////////////////////////////////////
	if ((Clerk.AtmStatus == "ERROR")		||
		(Clerk.AtmStatus == "CLERK"))
		;
	else
	{
		if ((fnSCR_GetCurrentScreenNo() == 900)	||
			(fnSCR_GetCurrentScreenNo() == 901))
			;
		else
		{
			Clerk.Index = 1;									// Return to Main Screen
			Clerk.UpdateScreen = TRUE;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	Clerk.AtmOpStatus = Clerk.AtmStatus;
	Clerk.UpdateButton = TRUE;

	if ((Clerk.SendRecvStatus == "ON")		||
		(Clerk.TransactionStaus == "ON"))
	{
		Clerk.AtmOpStatus = "TRAN";
		Clerk.UpdateButton = TRUE;
	}
	
/////////////////////////////////////////////////////////////////////////////
	if (Clerk.PowerStaus == "OFF")
	{
		Clerk.Doing = FALSE;									// Power Off
		Clerk.Index = 1;
		Clerk.AtmOpStatus = "DOWN";
		Clerk.AtmOpMessage = GuideMsgTbl[36][OpDeviceLangMode];
		Clerk.UpdateScreen = TRUE;
		Clerk.UpdateButton = TRUE;
		Clerk.UpdateMessage = TRUE;
																// Power Off Message
		fnSCR_DisplayString(48, GuideMsgTbl[36][OpDeviceLangMode]);
		ClerkSetButton("DOWN");
		fnSCR_DisplayScreen(901);

		SetShareData("OP_COMMAND", "OP_POWEROFFSTATUS");		// OP_POWEROFFSTATUS
		SetShareData("OP_RESULT", OP_DO);
		ClerkWaitResult();										// Clerk Wait Result
	}
	
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// Clerk Set Button
int CRearApCtrl::ClerkSetButton(CString strAtmStatus)
{
//		1			2			3			4			5			6
//		ASMODE,		POWEROFF,	READY,		OPMODE,		RESET,		RUN
//
//		OFF,		OFF,		OFF,		OFF,		OFF,		OFF,	// INIT		(AP)
//		ON,			ON,			OFF,		ON,			ON,			OFF,	// ERROR	(OP)
//		ON,			ON,			OFF,		ON,			ON,			ON,		// CLERK	(OP)
//		OFF,		OFF,		ON,			OFF,		OFF,		OFF,	// READY	(AP)
//		OFF,		OFF,		ON,			OFF,		OFF,		OFF,	// CUSTOM	(AP)
//		OFF,		OFF,		OFF,		OFF,		OFF,		OFF,	// TRAN		(AP)
//		OFF,		OFF,		OFF,		OFF,		OFF,		OFF,	// DOWN		(AP)
//
//		1
//		HELP/SUBTOTAL/TOTAL/ADDCASH/PRINTCASH/RQK/DOWNFIT/CHANGEPIN/TRANSTOTAL/EJMVIEW
//		2           3            4        5     6      7      8       9      10
//		OPENSHUTTER/CLOSESHUTTER/ALLRESET/RESET/ASMODE/OPMODE/RUNMODE/APMODE/POWEROFF
//
//		OFF														// INIT		(AP)
//		OFF        /OFF         /OFF     /OFF  /OFF   /OFF   /OFF    /OFF   /OFF
//		ON														// ERROR	(OP)
//		ON         /ON          /ON      /ON   /ON    /ON    /OFF    /ON    /ON
//		ON														// CLERK	(OP)
//		ON         /ON          /ON      /ON   /ON    /ON    /OFF    /ON    /ON
//		OFF														// READY	(AP)
//		OFF        /OFF         /OFF     /OFF  /OFF   /OFF   /ON     /OFF   /OFF
//		OFF														// CUSTOM	(AP)
//		OFF        /OFF         /OFF     /OFF  /OFF   /OFF   /ON     /OFF   /OFF
//		OFF														// TRAN		(AP)
//		OFF        /OFF         /OFF     /OFF  /OFF   /OFF   /OFF    /OFF   /OFF
//		OFF														// DOWN		(AP)
//		OFF        /OFF         /OFF     /OFF  /OFF   /OFF   /OFF    /OFF   /OFF


	if (strAtmStatus == "INIT")
	{
		fnSCR_DisplayImage(1,	FALSE);							// F1
		fnSCR_DisplayImage(2,	FALSE);							// F2
		fnSCR_DisplayImage(3,	FALSE);							// F3
		fnSCR_DisplayImage(4,	FALSE);							// F4
		fnSCR_DisplayImage(5,	FALSE);							// F5
		fnSCR_DisplayImage(6,	FALSE);							// F6
		fnSCR_DisplayImage(7,	FALSE);							// F7
		fnSCR_DisplayImage(8,	FALSE);							// F8
		fnSCR_DisplayImage(9,	FALSE);							// F9
		fnSCR_DisplayImage(10,	FALSE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}
	else
	if (strAtmStatus == "ERROR")
	{
		if ((Clerk.OpDevice == "NOT_SET")		||				// 2005.12.01
			(Clerk.OpDevice == "OP_TSPL")		||
			(Clerk.OpDevice == "OP_TSPLCHN")	||
			(Clerk.OpDevice == "OP_HSPL"))
		{
			fnSCR_DisplayImage(1,	TRUE);						// F1
			fnSCR_DisplayImage(2,	TRUE);						// F2
			fnSCR_DisplayImage(3,	TRUE);						// F3
			fnSCR_DisplayImage(4,	TRUE);						// F4
			fnSCR_DisplayImage(5,	TRUE);						// F5
			fnSCR_DisplayImage(6,	TRUE);						// F6
			fnSCR_DisplayImage(7,	TRUE);						// F7
			fnSCR_DisplayImage(8,	FALSE);						// F8
			fnSCR_DisplayImage(9,	(Clerk.SplDevice == "SPL_ONOFFSW") ? FALSE : TRUE);
																// F9 : 2006.06.02
			fnSCR_DisplayImage(10,	TRUE);						// F10
//			fnSCR_DisplayImage(11,	FALSE);						// F11 (Reserved)
//			fnSCR_DisplayImage(12,	FALSE);						// F12 (Reserved)
		}
		else
		{
			if (Clerk.ReadyFlag == TRUE)						// OP_READY
			{
				fnSCR_DisplayImage(1,	TRUE);					// F1
				fnSCR_DisplayImage(2,	TRUE);					// F2
				fnSCR_DisplayImage(3,	TRUE);					// F3
				fnSCR_DisplayImage(4,	TRUE);					// F4
				fnSCR_DisplayImage(5,	TRUE);					// F5
				fnSCR_DisplayImage(6,	TRUE);					// F6
				fnSCR_DisplayImage(7,	TRUE);					// F7
				fnSCR_DisplayImage(8,	FALSE);					// F8
				fnSCR_DisplayImage(9,	(Clerk.SplDevice == "SPL_ONOFFSW") ? FALSE : TRUE);
																// F9 : 2006.06.02
				fnSCR_DisplayImage(10,	TRUE);					// F10
//				fnSCR_DisplayImage(11,	FALSE);					// F11 (Reserved)
//				fnSCR_DisplayImage(12,	FALSE);					// F12 (Reserved)
			}
			else												// OP_RUN
			{
				fnSCR_DisplayImage(1,	FALSE);					// F1
				fnSCR_DisplayImage(2,	FALSE);					// F2
				fnSCR_DisplayImage(3,	FALSE);					// F3
				fnSCR_DisplayImage(4,	FALSE);					// F4
				fnSCR_DisplayImage(5,	FALSE);					// F5
				fnSCR_DisplayImage(6,	FALSE);					// F6
				fnSCR_DisplayImage(7,	FALSE);					// F7
//				fnSCR_DisplayImage(8,	TRUE);					// F8
				fnSCR_DisplayImage(8,	(Clerk.SplDevice == "SPL_ONOFFSW") ? FALSE : TRUE);
																// F8	added by liuxl 20111103
				fnSCR_DisplayImage(9,	FALSE);					// F9
				fnSCR_DisplayImage(10,	FALSE);					// F10
//				fnSCR_DisplayImage(11,	FALSE);					// F11 (Reserved)
//				fnSCR_DisplayImage(12,	FALSE);					// F12 (Reserved)
			}
		}
	}
	else
	if (strAtmStatus == "CLERK")
	{
		fnSCR_DisplayImage(1,	TRUE);							// F1
		fnSCR_DisplayImage(2,	TRUE);							// F2
		fnSCR_DisplayImage(3,	TRUE);							// F3
		fnSCR_DisplayImage(4,	TRUE);							// F4
		fnSCR_DisplayImage(5,	TRUE);							// F5
		fnSCR_DisplayImage(6,	TRUE);							// F6
		fnSCR_DisplayImage(7,	TRUE);							// F7
		fnSCR_DisplayImage(8,	FALSE);							// F8
		fnSCR_DisplayImage(9,	(Clerk.SplDevice == "SPL_ONOFFSW") ? FALSE : TRUE);
																// F9 : 2006.06.02
		fnSCR_DisplayImage(10,	TRUE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}
	else
	if (strAtmStatus == "READY")
	{
		fnSCR_DisplayImage(1,	FALSE);							// F1
		fnSCR_DisplayImage(2,	FALSE);							// F2
		fnSCR_DisplayImage(3,	FALSE);							// F3
		fnSCR_DisplayImage(4,	FALSE);							// F4
		fnSCR_DisplayImage(5,	FALSE);							// F5
		fnSCR_DisplayImage(6,	FALSE);							// F6
		fnSCR_DisplayImage(7,	FALSE);							// F7
//		fnSCR_DisplayImage(8,	TRUE);							// F8
		fnSCR_DisplayImage(8,	(Clerk.SplDevice == "SPL_ONOFFSW") ? FALSE : TRUE);
																// F8	added by liuxl 20111103
		fnSCR_DisplayImage(9,	FALSE);							// F9
		fnSCR_DisplayImage(10,	FALSE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}
	else
	if (strAtmStatus == "CUSTOM")
	{
		fnSCR_DisplayImage(1,	FALSE);							// F1
		fnSCR_DisplayImage(2,	FALSE);							// F2
		fnSCR_DisplayImage(3,	FALSE);							// F3
		fnSCR_DisplayImage(4,	FALSE);							// F4
		fnSCR_DisplayImage(5,	FALSE);							// F5
		fnSCR_DisplayImage(6,	FALSE);							// F6
		fnSCR_DisplayImage(7,	FALSE);							// F7
//		fnSCR_DisplayImage(8,	TRUE);							// F8
		fnSCR_DisplayImage(8,	(Clerk.SplDevice == "SPL_ONOFFSW") ? FALSE : TRUE);
																// F8	added by liuxl 20111103
		fnSCR_DisplayImage(9,	FALSE);							// F9
		fnSCR_DisplayImage(10,	FALSE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}
	else
	if (strAtmStatus == "TRAN")
	{
		fnSCR_DisplayImage(1,	FALSE);							// F1
		fnSCR_DisplayImage(2,	FALSE);							// F2
		fnSCR_DisplayImage(3,	FALSE);							// F3
		fnSCR_DisplayImage(4,	FALSE);							// F4
		fnSCR_DisplayImage(5,	FALSE);							// F5
		fnSCR_DisplayImage(6,	FALSE);							// F6
		fnSCR_DisplayImage(7,	FALSE);							// F7
		fnSCR_DisplayImage(8,	FALSE);							// F8
		fnSCR_DisplayImage(9,	FALSE);							// F9
		fnSCR_DisplayImage(10,	FALSE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}
	else
	if (strAtmStatus == "DOWN")
	{
		fnSCR_DisplayImage(1,	FALSE);							// F1
		fnSCR_DisplayImage(2,	FALSE);							// F2
		fnSCR_DisplayImage(3,	FALSE);							// F3
		fnSCR_DisplayImage(4,	FALSE);							// F4
		fnSCR_DisplayImage(5,	FALSE);							// F5
		fnSCR_DisplayImage(6,	FALSE);							// F6
		fnSCR_DisplayImage(7,	FALSE);							// F7
		fnSCR_DisplayImage(8,	FALSE);							// F8
		fnSCR_DisplayImage(9,	FALSE);							// F9
		fnSCR_DisplayImage(10,	FALSE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}
	else
	{
		fnSCR_DisplayImage(1,	FALSE);							// F1
		fnSCR_DisplayImage(2,	FALSE);							// F2
		fnSCR_DisplayImage(3,	FALSE);							// F3
		fnSCR_DisplayImage(4,	FALSE);							// F4
		fnSCR_DisplayImage(5,	FALSE);							// F5
		fnSCR_DisplayImage(6,	FALSE);							// F6
		fnSCR_DisplayImage(7,	FALSE);							// F7
		fnSCR_DisplayImage(8,	FALSE);							// F8
		fnSCR_DisplayImage(9,	FALSE);							// F9
		fnSCR_DisplayImage(10,	FALSE);							// F10
//		fnSCR_DisplayImage(11,	FALSE);							// F11 (Reserved)
//		fnSCR_DisplayImage(12,	FALSE);							// F12 (Reserved)
	}

	return TRUE;
}

// Clerk Wait Result
int CRearApCtrl::ClerkWaitResult(int nWaitSec)
{
	int				nReturn = FALSE;
	CString			strResult("");
	CStringArray	strResultArray;
	CString			strTemp("");

	strTemp = GetShareData("OP_COMMAND");

	if (strTemp.Find("MASTERKEY") >= 0)
	{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkWaitResult():OP_COMMAND[]");
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkWaitResult():OP_COMMAND[%s]", strTemp);
	}
	
	Clerk.AtmOpMessage = "";									// Atm Op Message
	Clerk.UpdateMessage = FALSE;								// Update Message

	ULONG WiatTime = TimerSet(nWaitSec);
	ULONG WiatTime2 = TimerSet(MID_DEVRSP_TIME);				// 125 Sec : 2007.11.01
	while (TRUE)
	{
		Delay_Msg(100);											// Performance Adjust
		strResult = GetShareData("OP_RESULT");
		SplitString(strResult, "||", strResultArray);
		
		if (strResultArray[0] == OP_NORMAL)						// Command Result Success
		{
			if (strResultArray.GetSize() >= 2)
			{
				Clerk.AtmOpMessage = strResultArray[1];			// Error Message
				Clerk.UpdateMessage = TRUE;
			}
			nReturn = TRUE;										// 2005.11.11
			break;
		}
		else
		if (strResultArray[0] == OP_ERROR)						// Command Result Error
		{
			if (strResultArray.GetSize() >= 2)
			{
				Clerk.AtmOpMessage = strResultArray[1];			// Error Message
				Clerk.UpdateMessage = TRUE;
			}
			nReturn = FALSE;									// 2005.11.11
			break;
		}
		else
		if (strResultArray[0] == OP_DO)							// Command Result Doing : 2007.11.01
		{
			if (CheckTimer(WiatTime))							// Time Out
			{
				Clerk.AtmOpMessage = "OP TIMEOUT !";
				Clerk.UpdateMessage = TRUE;

				SetShareData("OP_RESULT", "ERROR||" + Clerk.AtmOpMessage);
				nReturn = FALSE;								// 2005.11.11
				break;
			}
		}
		else
		{
			if (CheckTimer(WiatTime2))							// Time Out
			{
				Clerk.AtmOpMessage = "OP TIMEOUT2 !";
				Clerk.UpdateMessage = TRUE;

				SetShareData("OP_RESULT", "ERROR||" + Clerk.AtmOpMessage);
				nReturn = FALSE;								// 2005.11.11
				break;
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkWaitResult():OP_RESULT[%s]", GetShareData("OP_RESULT"));
	return nReturn;
}

/////////////////////////////////////////////////////以下为邮储管理菜单使用  by zjd
// Clerk Post Count Menu
int	CRearApCtrl::ClerkPostCountMenuProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostCountMenuProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;

	nScrNum = 960;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				Clerk.Index = 12;								// Clerk Op Mode Procedure
			}
			break;
		}
		else
		if (strReturn == "TOTALINQURE")
		{
			Clerk.Index = 61;
			break;
		}
		else
		if (strReturn == "CASHREPN")
		{
			//Clerk.Index = 14;									// Clerk Add Cash Procedure
			strTempClerkMessage = GuideMsgTbl[59][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			Delay_Msg(4000);
			strTempClerkMessage = "";
			break;
		}
		else
		if (strReturn == "CLEARRETAINEDCARD")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_CLEARCARD");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// 2006.08.22
		if (strReturn == "COUNTEROTHERFUNCTION")
		{
			Clerk.Index = 62;
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostCountMenuProc():return");
	return TRUE;
}

// Clerk Total Inqure
int	CRearApCtrl::ClerkTotalInqureProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalInqureProc(),clerk.index:%d",Clerk.Index);

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	int				mTotalResult;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;
	CString			strInputAmount("");
	CString			strCounterNum("");

	if (Asc2Int(IniGetStr(_TRANSACTION_INI, "TRANSACTION", "ATMPTOTALKIND", "1")))
		nScrNum = 961;												// Display Special Mode
	else
		nScrNum = 959;
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				Clerk.Index = 60;								// Clerk Post Count Menu
			}
			break;
		}
// 		else
// 		if (strReturn == "ATMCTOTALS")
// 		{
// 			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
// 			strSaveClerkMessage = strTempClerkMessage;
// 			fnSCR_DisplayString(48, strTempClerkMessage);
// 			fnSCR_DisplayMessage();
// 			strTempClerkMessage = "";
// 			
// 			SetShareData("OP_COMMAND", "OP_SUBTOTAL");			// OP_SUBTOTAL
// 			SetShareData("OP_RESULT", OP_DO);
// 			ClerkWaitResult();									// Clerk Wait Result
// 		}
		else
		if (strReturn == "ATMPTOTALS")
		{
			if(!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TOTALKINDSET", 1))
			{
				strTempClerkMessage = GuideMsgTbl[63][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(4000);
				strTempClerkMessage = "";
			}
			else
			if (Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, "ATMPTOTALKIND", "1")))
			{
				nScrNum = 943;												// Input Addcash Counter Number  by zjd
				
				strTempClerkMessage = "";									//	liyi add 2008.05.22		
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();

				while (TRUE)
				{	
					while (TRUE)
					{
						Delay_Msg(100);										// Performance Adjust
						ClerkStatusUpdate();
						if (Clerk.UpdateScreen)
						{
							Clerk.UpdateScreen = FALSE;
							return TRUE;
						}
						if (Clerk.UpdateMessage)
						{
							Clerk.UpdateMessage = FALSE;
							strTempClerkMessage = Clerk.AtmOpMessage;
							nInitFlag = TRUE;
						}
						
						if ((nInitFlag == TRUE)										|| //2008.4.20 add
							(strTempClerkInformation != strSaveClerkInformation)	||
							(strTempClerkMessage != strSaveClerkMessage))
						{
							nInitFlag = FALSE;
							strSaveClerkInformation = strTempClerkInformation;
							strSaveClerkMessage = strTempClerkMessage;
							fnSCR_DisplayString(48, strTempClerkMessage);
							if (fnSCR_GetCurrentScreenNo() == nScrNum)			
								fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
							else 
								fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
							
						}
						
						if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
							break;
					}
					
					nInitFlag = TRUE;										// Display | Update
					strReturn = fstrSCR_GetKeyString();
					SplitString(strReturn, ":", strTempArray);
					if (strReturn == S_EXIT)
					{
						Clerk.Index = 61;						// Clerk Op Mode Procedure or Post Menu
						return TRUE;
					}
					else
					{
						if (strTempArray.GetSize() >= 2)
						{
							strCounterNum.Format("%s", strTempArray[1]);
							break;
						}
						else
						{													// Flash Error Message
							strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
							fnSCR_DisplayString(48, strTempClerkMessage);
							fnSCR_DisplayMessage();
							Delay_Msg(2000);
							
							Clerk.Index = Clerk.SaveIndex;					// Clerk Op Mode Procedure
							return TRUE;
						}
					}
				}

				nScrNum = 944;												// Input Cash Remain Amount  by zjd

				while (TRUE)
				{	
					while (TRUE)
					{
						Delay_Msg(100);										// Performance Adjust
						ClerkStatusUpdate();
						if (Clerk.UpdateScreen)
						{
							Clerk.UpdateScreen = FALSE;
							return TRUE;
						}
						if (Clerk.UpdateMessage)
						{
							Clerk.UpdateMessage = FALSE;
							strTempClerkMessage = Clerk.AtmOpMessage;
							nInitFlag = TRUE;
						}
						
						if ((nInitFlag == TRUE)										||  //2008.4.20 add
							(strTempClerkInformation != strSaveClerkInformation)	||
							(strTempClerkMessage != strSaveClerkMessage))
						{
							nInitFlag = FALSE;
							strSaveClerkInformation = strTempClerkInformation;
							strSaveClerkMessage = strTempClerkMessage;
 							fnSCR_DisplayString(48, strTempClerkMessage);
							if (fnSCR_GetCurrentScreenNo() == nScrNum)			
								fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
							else 
 								fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
						}
						
						if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
							break;
					}
					
					nInitFlag = TRUE;										// Display | Update
					strReturn = fstrSCR_GetKeyString();
					SplitString(strReturn, ":", strTempArray);
					if (strReturn == S_EXIT)
					{
						Clerk.Index = 61;									// Clerk Op Mode Procedure or Post Menu
						return TRUE;
					}
					else
					{
						if (strTempArray.GetSize() >= 2)
						{
							strInputAmount.Format("%s", strTempArray[1]);
							break;
						}
						else
						{													// Flash Error Message
							strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
							fnSCR_DisplayString(48, strTempClerkMessage);
							fnSCR_DisplayMessage();
							Delay_Msg(2000);
							
							Clerk.Index = 61;								// Clerk Op Mode Procedure
							return TRUE;
						}
					}
				}
				
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";
//				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "1");	// added by yusy 2010.03.22

				strTemp =	"OP_TOTAL||";
				strTemp +=	strCounterNum + "||";
				strTemp +=	strInputAmount;
				SetShareData("OP_COMMAND", strTemp);				// OP_TOTAL
				SetShareData("OP_RESULT", OP_DO);
				mTotalResult = ClerkWaitResult();									// Clerk Wait Result
				
//				nScrNum = 961;  //by hyl 
			}
			else
			{
				if (RegGetStr(_REGKEY_ADDCASHMODE, ZERO2, "0") == "1")
				{
					strTempClerkMessage = "请先修改加钞方式!";
					fnSCR_DisplayString(48, strTempClerkMessage);
					fnSCR_DisplayMessage();
					Delay_Msg(4000);
					strTempClerkMessage = "";
					break;
				}

				nScrNum = 943;												// Input Addcash Counter Number  by zjd

				strTempClerkMessage = "";									//	liyi add 2008.05.22		
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();

				while (TRUE)
				{	
					while (TRUE)
					{
						Delay_Msg(100);										// Performance Adjust
						ClerkStatusUpdate();
						if (Clerk.UpdateScreen)
						{
							Clerk.UpdateScreen = FALSE;
							return TRUE;
						}
						if (Clerk.UpdateMessage)
						{
							Clerk.UpdateMessage = FALSE;
							strTempClerkMessage = Clerk.AtmOpMessage;
							nInitFlag = TRUE;
						}
						
						if ((nInitFlag == TRUE)										|| //2008.4.20 add
							(strTempClerkInformation != strSaveClerkInformation)	||
							(strTempClerkMessage != strSaveClerkMessage))
						{
							nInitFlag = FALSE;
							strSaveClerkInformation = strTempClerkInformation;
							strSaveClerkMessage = strTempClerkMessage;
							fnSCR_DisplayString(48, strTempClerkMessage);
							if (fnSCR_GetCurrentScreenNo() == nScrNum)			
								fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
							else 
								fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
							
						}
						
						if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
							break;
					}
					
					nInitFlag = TRUE;										// Display | Update
					strReturn = fstrSCR_GetKeyString();
					SplitString(strReturn, ":", strTempArray);
					if (strReturn == S_EXIT)
					{
						Clerk.Index = 61;						// Clerk Op Mode Procedure or Post Menu
						return TRUE;
					}
					else
					{
						if (strTempArray.GetSize() >= 2)
						{
							strCounterNum.Format("%s", strTempArray[1]);
							strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
							strSaveClerkMessage = strTempClerkMessage;
							fnSCR_DisplayString(48, strTempClerkMessage);
							fnSCR_DisplayMessage();
							strTempClerkMessage = "";
//							RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "1");
							break;
						}
						else
						{													// Flash Error Message
							strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
							fnSCR_DisplayString(48, strTempClerkMessage);
							fnSCR_DisplayMessage();
							Delay_Msg(2000);
							
							Clerk.Index = Clerk.SaveIndex;					// Clerk Op Mode Procedure
							return TRUE;
						}
					}
				}
				
				strTemp =  "OP_TOTALPRE||";
				strTemp += strCounterNum;
				
				SetShareData("OP_COMMAND", strTemp);			// OP_ADDCASH
				SetShareData("OP_RESULT", OP_DO);
				mTotalResult = ClerkWaitResult();

//				nScrNum = 959;
			}

// 			if (mTotalResult)
// 			{
// 				nScrNum = 920;
// 				while (TRUE)
// 				{
// 					Delay_Msg(100);										// Performance Adjust
// 					ClerkStatusUpdate();
// 					if (Clerk.UpdateScreen)
// 					{
// 						Clerk.UpdateScreen = FALSE;
// 						return TRUE;
// 					}
// 					if (Clerk.UpdateMessage)
// 					{
// 						Clerk.UpdateMessage = FALSE;
// 						strTempClerkMessage = Clerk.AtmOpMessage;
// 						nInitFlag = TRUE;
// 					}
// 					
// 					strTempClerkInformation = ClerkInformationMake(Clerk.Index);
// 					if ((nInitFlag == TRUE)										||
// 						(strTempClerkInformation != strSaveClerkInformation)	||
// 						(strTempClerkMessage != strSaveClerkMessage))
// 					{
// 						nInitFlag = FALSE;
// 						strSaveClerkInformation = strTempClerkInformation;
// 						strSaveClerkMessage = strTempClerkMessage;
// 						fnSCR_DisplayStringArray(1, strTempClerkInformation);
// 						fnSCR_DisplayString(48, strTempClerkMessage);
// 						if (fnSCR_GetCurrentScreenNo() == nScrNum)			
// 							fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
// 						else 
// 							fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_MENU_MODE);
// 					}
// 					
// 					if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
// 						break;
// 				}
// 				nInitFlag = TRUE;
// 				strReturn = fstrSCR_GetKeyString();
// 				if (strReturn == "ENTER");
// 			}

			if (Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, "ATMPTOTALKIND", "1")))
				nScrNum = 961;
			else
				nScrNum = 959;
		}
		else
		if (strReturn == "UPLOADJOUR")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_UPLOADJOUR");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// 2006.08.22
		if (strReturn == "BACKUPEJ")
		{
			Clerk.Index = 63;
			break;
		}
// 		else
// 		if(strReturn == "VIEWATMCTOTALS")						// yaokq add 20081028
// 		{
// 			Clerk.Index = 65;
// 			break;
// 		}
		else
		if(strReturn == "CASHREPN")								// yaokq add 20081031
		{
			if(!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TOTALKINDSET", 1))	// added by yusy 20100322
			{
				strTempClerkMessage = GuideMsgTbl[63][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(4000);
				strTempClerkMessage = "";
			}
			else
			if(RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "0") == "0")	// added by yusy 20100322
			{
				strTempClerkMessage = GuideMsgTbl[60][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(4000);
				strTempClerkMessage = "";
			}
			else
			{
				Clerk.Index = 14;									// Clerk Add Cash Procedure
				break;
			}
		}
		else
		if(strReturn == "ATMPTOTALSKIND")
		{
			Clerk.Index = 78;
			break;
		}
	}
   
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalInqureProc():return");
	return TRUE;
}

//Clerk Post Atmc Totals Procedure   
// yaokq add 20081029
int CRearApCtrl::ClerkPostViewAtmcTotalProc()
{
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostViewAtmcTotalProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CString			strTemp("");
	CStringArray	strTempArray;
	CString			strPrintData("");

	nScrNum = 963;												// Display Parameter
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
				else
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_ALL_MODE);
			}
			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Special Mode Procedure
			break;
		}
		else if ((strTempArray.GetSize() >= 32) &&
			(strTempArray[0] == "PRINT"))
		{

			strPrintData = "OP_SPRJPRPRINT||";

			// Add 6 Lines 200811101
//			for(int i = 0;i < 7;i++)
//			{
//				strTemp = "                                        ";
//				strPrintData += strTemp + "||";
//			}

			// Line1 Edit
			strTemp.Format("%2.2s:%2.2s:%2.2s Operator ATMC TOTAL",
					GetTime().GetBuffer(0),&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]));
			strPrintData += strTemp + "||";
			// Line2 Edit 
//			char	AtmId[20];
//			memset(AtmId,0,sizeof(AtmId));
			
//			AtmId = IniGetStr(_NETWORK_INI,"NETWORK","AtmSerialNum",NULL);

			strTemp.Format("DATE:%4.4s-%2.2s-%2.2s   ATM ID:%8.8s",
					GetDate().GetBuffer(0),&(GetDate().GetBuffer(0)[4]),
					&(GetDate().GetBuffer(0)[6]),Clerk.AtmSerialNumber);
			strPrintData += strTemp + "||";
			// Line3 Edit
			strTemp = "               ATMC TOTAL";
			strPrintData += strTemp + "||";
			// Line4 Edit
			strTemp.Format("ATM Cassettes Total Number:%4.4s",strTempArray[1]);
												
			strPrintData += strTemp + "||";
			// Line5 Edit
			strTemp = "             Cas.1  Cas.2   Cas.3  Cas.4";
			strPrintData += strTemp + "||";
			// Line6 Edit
			strTemp = "Notes       :CNY    CNY     CNY   CNY";
			strPrintData += strTemp + "||";
			// Line7 Edit
			strTemp.Format("Value       :%4.4s   %4.4s    %4.4s  %4.4s",
					strTempArray[2],strTempArray[3],strTempArray[4],strTempArray[5]);
			strPrintData += strTemp + "||";
			// Line8 Edit
			strTemp.Format("Last        :%4.4s   %4.4s    %4.4s  %4.4s",
					strTempArray[6],strTempArray[7],strTempArray[8],strTempArray[9]);
			strPrintData += strTemp + "||";
			// Line9 Edit
			strTemp.Format("Left        :%4.4s   %4.4s    %4.4s  %4.4s",
					strTempArray[10],strTempArray[11],strTempArray[12],strTempArray[13]);
			strPrintData += strTemp + "||";
			// Line10 Edit
			strTemp.Format("Disp        :%4.4s   %4.4s    %4.4s  %4.4s",
					strTempArray[14],strTempArray[15],strTempArray[16],strTempArray[17]);
			strPrintData += strTemp + "||";
			// Line11 Edit
			strTemp.Format("PURG        :%4.4s   %4.4s    %4.4s  %4.4s",
					strTempArray[18],strTempArray[19],strTempArray[20],strTempArray[21]);
			strPrintData += strTemp + "||";
			// Line12 Edit
			strTemp.Format("RMB Cash Init   Amount      :%10s", strTempArray[22]);
			strPrintData += strTemp + "||";
			// Line13 Edit
			strTemp.Format("RMB Cash Remain Amount      :%10s", strTempArray[23]);
			strPrintData += strTemp + "||";
			// Line14 Edit
			strTemp.Format("RMB CWD Times               :%4s", strTempArray[24]);
			strPrintData += strTemp + "||";
			// Line15 Edit
			strTemp.Format("RMB CWD Amount              :%10s", strTempArray[25]);
			strPrintData += strTemp + "||";
			// Line16 Edit
			strTemp.Format("RMB TFR Times               :%4s", strTempArray[26]);
			strPrintData += strTemp + "||";
			// Line17 Edit
			strTemp.Format("RMB TFR Amount              :%10s", strTempArray[27]);
			strPrintData += strTemp + "||";
			// Line18 Edit
			strTemp.Format("RMB CWC Times               :%4s", strTempArray[28]);
			strPrintData += strTemp + "||";
			// Line19 Edit
			strTemp.Format("RMB CWC Amount              :%10s", strTempArray[29]);
			strPrintData += strTemp + "||";
			// Line20 Edit
			strTemp.Format("Momeny Retact Times         :%4s", strTempArray[30]);
			strPrintData += strTemp + "||";
			// Line21 Edit
			strTemp.Format("Momeny Retact Amount        :%10s", strTempArray[31]);
			strPrintData += strTemp + "||";
			// Line22 Edit
			strTemp.Format("Card Retained               :%4s", strTempArray[32]);
			strPrintData += strTemp;

			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SetShareData("OP_COMMAND", strPrintData);			// OP_SPRJPRPRINT
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else													// Flash Error Message
			strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkSetParamProc():return");
	return TRUE;
}

// Clerk Post EJ Copy Procedure
int	CRearApCtrl::ClerkPostEJCopyProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostEJCopyProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	unsigned		nDriveType;
	CString			strDriveNum("");
	CString			strSrcTraceFile("");						
	CString			strObjTraceFile("");
	CString			strInputStartDate("");						// 2005.02.24
	CString			strInputEndDate("");
	CString			strInputDate("");
	int				nInputStartDate = 0;
	int				nInputEndDate = 0;
	int				nInputDate = 0;
	BOOL			bHaveCopy = FALSE;
	CString			strZipFileExp;

	nScrNum = 915;												// Display Ejm Send
	while (TRUE)
	{
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(44);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;
		strReturn = fstrSCR_GetKeyString();
		if (strReturn == S_EXIT)
		{
			Clerk.Index = Clerk.SaveIndex;						// Clerk Ejm Procedure or Clerk AS Procedure
			break;
		}
		else
		{														// Processing Message
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			SplitString(strReturn, ":", strTempArray);
			if(strTempArray.GetSize() < 2)
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
			}
			else
			if (strTempArray[0] == "FDCOPY")
			{
				nDriveType = 7;
				bHaveCopy = TRUE;
			}
			else
			if (strTempArray[0] == "USBCOPY")
			{
				nDriveType = 8;
				bHaveCopy = TRUE;
			}

			if (bHaveCopy)
			{
				bHaveCopy = FALSE;
																// Processing Message
				strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				strTempClerkMessage = "";

				strInputStartDate = ZERO8 + strTempArray[1];
				strInputStartDate = strInputStartDate.Right(8);
				nInputStartDate = Asc2Int(strInputStartDate);
				strInputEndDate = ZERO8 + strTempArray[2];
				strInputEndDate = strInputEndDate.Right(8);
				nInputEndDate = Asc2Int(strInputEndDate);

				strDriveNum = ClerkCheckDriveIsInstall(nDriveType);
				if ( strDriveNum == "" )
				{
					strTempClerkMessage = GuideMsgTbl[31][OpDeviceLangMode];
				}
				else
				if ( !ClerkCheckDiskIsInsert( strDriveNum ) )
				{
					strTempClerkMessage = GuideMsgTbl[32][OpDeviceLangMode];
				}
				else
				if ( nInputStartDate > nInputEndDate )
				{
					strTempClerkMessage = GuideMsgTbl[38][OpDeviceLangMode];
				}
				else
				{
					CreateDirectory( _AP_TEMPZIP_DIR, NULL );
					DeleteFiles( _AP_TEMPZIP_DIR, -1 );

					for (nInputDate = nInputStartDate; nInputDate <= nInputEndDate; nInputDate++)
					{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "nInputDate(%d)", nInputDate);
						if (((nInputDate % 100) > 31)		||
							(((nInputDate % 10000) / 100) > 12))
							continue;
						strInputDate = GetSprintf("%06.6d", nInputDate % 1000000);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "strInputDate(%s)", strInputDate);
						strInputDate += Clerk.AtmSerialNumber;
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "strInputDate1(%s)", strInputDate);
						ClerkCopyEJFile( _EJR_DETAILSEND, ".LOG", strInputDate );
						ClerkCopyEJFile( _EJR_DETAILSEND, ".SPR.LOG", strInputDate );
						ClerkCopyEJFile( _EJR_DETAILSEND, ".LINE.LOG", strInputDate );
						Delay_Msg(100);							// ALL COPY
					}

					strSrcTraceFile.Format("%s\\EJALL%s(%s-%s).ZIP", _AP_TEMPZIP_DIR, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
					strZipFileExp.Format( "pkzip25 -add %s %s\\*.*", strSrcTraceFile, _AP_TEMPZIP_DIR );
					WinExecWaitEnd( strZipFileExp, SW_HIDE );

					strObjTraceFile.Format("%s\\EJALL%s(%s-%s).ZIP", strDriveNum, Clerk.AtmSerialNumber, strInputStartDate, strInputEndDate);
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "EJCopyOBJ(%s)", strObjTraceFile);
					if ( CopyFile( strSrcTraceFile, strObjTraceFile, 0 ) != 0 )
					{
						strTempClerkMessage = GuideMsgTbl[23][OpDeviceLangMode];
					}											// Successfully Message
					else
					{
						strTempClerkMessage = GuideMsgTbl[34][OpDeviceLangMode];
					}
					
//					DeleteFiles( _AP_TEMPZIP_DIR, -1 );
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostEJCopyProc() return");
	return TRUE;
}

// Clerk Total Inqure
int	CRearApCtrl::ClerkCounterOtherProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalInqureProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;

	CString			SprOptMode = IniGetStr(_TRANSACTION_INI,"TRANSACTION","SPROPTMODE","0");
	int				SprSet, TotalAddSet;
	if (SprOptMode == "1")
		nScrNum = 964;
	else
		nScrNum = 962;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			
			SprSet = Asc2Int(IniGetStr(_TRANSACTION_INI,"TRANSACTION","SPRSET","0"));		// added by liuxl 20110420
			TotalAddSet = Asc2Int(IniGetStr(_TRANSACTION_INI,"TRANSACTION","TOTALADDRULESET","1"));
			if (nScrNum == 964)
			{
				if (SprSet)
				{
					strTempClerkInformation += "\n";
					strTempClerkInformation += "开启";
				}
				else
				{
					strTempClerkInformation += "\n";
					strTempClerkInformation += "关闭";
				}
			}
			else
			{
				if (TotalAddSet)
				{
					strTempClerkInformation += "\n";
					strTempClerkInformation += "开启";
				}
				else
				{
					strTempClerkInformation += "\n";
					strTempClerkInformation += "关闭";
				}
			}

			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				Clerk.Index = 60;								// Clerk Post Count Menu
			}
			break;
		}
		else
		if (strReturn == "MODIFYADMPIN")
		{
			Clerk.Index = 64;
			break;
		}
		else
		if (strReturn == "SHUTDOWN")
		{
			SetShareData("OP_COMMAND", "OP_POWEROFF");			// OP_POWEROFF
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "REBOOT")
		{
			SetShareData("OP_COMMAND", "OP_REBOOT");			// OP_POWEROFF
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITJOURNALPTR")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITJOURNALPTR");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITRECEIPTPTR")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITRECEIPTPTR");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITCARDREADER")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITCARDREADER");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "RECEIPTPTRSET")						// added by liuxl 20110407
		{
			int	SprSet = Asc2Int(IniGetStr(_TRANSACTION_INI,"TRANSACTION","SPRSET","0"));
			if (SprSet)
				strTempClerkMessage = GuideMsgTbl[62][OpDeviceLangMode];
			else
				strTempClerkMessage = GuideMsgTbl[61][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_RECEIPTPTRSET");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();
		}
		else
		if (strReturn == "TOTALADDRULE")
		{
			if (TotalAddSet)
				strTempClerkMessage = GuideMsgTbl[65][OpDeviceLangMode];
			else
				strTempClerkMessage = GuideMsgTbl[64][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_TOTALADDRULE");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTotalInqureProc():return");
	return TRUE;
}

// Clerk Modify Admin Pin Procedure						// 2005.03.24
int CRearApCtrl::ClerkModifyAdmPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkModifyAdmPinProc()");

	int				nScrNum = 0;
	int				RetryCnt = 3;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strFirstPassWord("");
	CString			strSecondPassWord("");
	CString			strPrintData("");

	while (RetryCnt)
	{
		nScrNum = 913;											// Display Input Pin
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 62;
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strFirstPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 62;
				break;
			}
		}

/////////////////////////////////////////////////////////////////////////////
		nScrNum = 914;											// Display Input Pin Again
		nInitFlag = TRUE;
		strTempClerkInformation = "";
		strSaveClerkInformation = "";
		strTempClerkMessage = "";
		strSaveClerkMessage = "";

		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 62;
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strSecondPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 62;
				break;
			}
		}

		if(strFirstPassWord == strSecondPassWord)
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			strPrintData = "OP_JPRPRINT||";						// 打印流水   by zjd
			strTemp.Format("%2.2s:%2.2s:%2.2s OPERATOR Counter change PIN",
										GetTime().GetBuffer(0),
										&(GetTime().GetBuffer(0)[2]),
										&(GetTime().GetBuffer(0)[4]));
			strPrintData += strTemp;
			
			SetShareData("OP_COMMAND", strPrintData);	
			SetShareData("OP_RESULT", OP_DO);		// OP_JPRPRINT
			ClerkWaitResult();

			RegSetStr(_REGKEY_POSTADMINPIN, "00", strFirstPassWord);
			Clerk.AtmOpMessage = GuideMsgTbl[23][OpDeviceLangMode];
			Clerk.UpdateMessage = TRUE;				// Successfully Message
			Clerk.Index = 62;
			break;
		}
		else
		{
			if (!--RetryCnt)
			{													// Operation Miss Error Message
				strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 62;
				break;
			}
			else
			{													// PassWord Missmatch
				strTempClerkMessage = GuideMsgTbl[29][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkModifyAdmPinProc():return");
	return TRUE;
}

// Clerk Post Tech Menu
int	CRearApCtrl::ClerkPostTechMenuProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostTechMenuProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;

	nScrNum = 970;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_REBOOT");		// 修改参数后重启 by hyl
				SetShareData("OP_RESULT", OP_DO);
			    ClerkWaitResult();	
//		 		SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
//				SetShareData("OP_RESULT", OP_DO);
//				ClerkWaitResult();								// Clerk Wait Result
//				Clerk.Index = 1;								// Goto Select Mode

			}
			else												// 2006.07.07
			{
				Clerk.Index = 12;								// Clerk Op Mode Procedure
			}
			break;
		}
		else
		if (strReturn == "PARAMETERSETTING")					// AS 中的参数设置   by zjd
		{
			Clerk.Index = 27;
			break;
		}
		else
		if (strReturn == "KEYMANAGE")
		{
			Clerk.Index = 71;									// Clerk Add Cash Procedure
			break;
		}
		else
		if (strReturn == "OTHERFUNCTION")
		{
			Clerk.Index = 72;
			break;
		}
		else													// 2006.08.22
		if (strReturn == "DOWNLOADPARAM")
		{
			Clerk.Index = 73;
			break;
		}
		else
		if (strReturn == "CHANGEMATERIAL")						// added by yusy 08.09.09
		{
			Clerk.Index = 76;
			break;
		}
		else
		if (strReturn == "UNLOCKCWD")							// added by liuxl 20110609
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_UNLOCKCWD");			// OP_UNLOCKATM
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();
		}
//		else													// 2006.08.22
//		if (strReturn == "ADDCASHMODE")
//		{
//			Clerk.Index = 75;
//			break;
//		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkPostTechMenuProc():return");
	return TRUE;
}

// Clerk Key Manage Proc
int	CRearApCtrl::ClerkKeyManageProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkKeyManageProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;

	nScrNum = 971;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				Clerk.Index = 70;								// Clerk Post Tech Menu
			}
			break;
		}
		else
		if (strReturn == "INITCDK")
		{
			Clerk.Index = 192;
			break;
		}
		else
		if (strReturn == "REQUESTCDK")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_REQUESTCDK");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();
		}
		else
		if (strReturn == "REQUESTMAC")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_REQUESTMAC");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "REQUESTPIN")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_REQUESTPIN");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITEPP")
		{
			Clerk.Index = 77;
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkKeyManageProc():return");
	return TRUE;
}

// Clerk Change Material Proc		
int CRearApCtrl::ClerkChangeMaterial()  // added by yusy 08.09.09
{
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkChangeMaterial()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	nScrNum = 975;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								//
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												//
			{
				Clerk.Index = 70;								// Clerk Post Tech Menu
			}
			break;
		}
		else
		if (strReturn == "CHANGESPRPAPER")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			SetShareData("OP_COMMAND", "OP_CHANGESPRPAPER");	// OP_CHANGESPRPAPER
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "CHANGEJPRPAPER")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			SetShareData("OP_COMMAND", "OP_CHANGEJPRPAPER");	// OP_CHANGEJPRPAPER
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();
		}
		else
		if (strReturn == "CHANGEINK")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			SetShareData("OP_COMMAND", "OP_CHANGEINK");			// OP_CHANGEINK
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();
		}
	}

	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkChangeMaterial():return");
	return TRUE;
}

// Clerk Init EPP Proc		
int CRearApCtrl::ClerkInitEPP()  // added by yusy 2010.03.22
{
	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInitEPP()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	nScrNum = 976;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								//
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												//
			{
				Clerk.Index = 71;								// Clerk Post Tech Menu
			}
			break;
		}
		else
		if (strReturn == "INITEPP")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITEPP");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
	}

	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkInitEPP():return");
	return TRUE;
}

// Clerk Tech Other Function Proc
int	CRearApCtrl::ClerkOtherFunctionProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOtherFunctionProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;

	nScrNum = 972;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				SetShareData("OP_COMMAND", "OP_POWEROFF");		// OP_POWEROFF
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				Clerk.Index = 70;								// Clerk Post Tech Menu
			}
			break;
		}
		else
		if (strReturn == "SHUTDOWN")
		{
			SetShareData("OP_COMMAND", "OP_POWEROFF");			// OP_POWEROFF
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "REBOOT")
		{
			SetShareData("OP_COMMAND", "OP_REBOOT");			// OP_POWEROFF
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITJOURNALPTR")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITJOURNALPTR");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITRECEIPTPTR")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITRECEIPTPTR");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITDISPENCE")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITDISPENCE");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "INITCARDREADER")
		{
			strTempClerkMessage = GuideMsgTbl[22][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_INITCARDREADER");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "HARDWAREDOCSET")
		{
			Clerk.Doing = FALSE;								// Power Off
			Clerk.Index = 1;
			Clerk.AtmOpStatus = "DOWN";
			Clerk.AtmOpMessage = GuideMsgTbl[36][OpDeviceLangMode];
			Clerk.UpdateScreen = TRUE;
			Clerk.UpdateButton = TRUE;
			Clerk.UpdateMessage = TRUE;
			
			SetShareData("OP_COMMAND", "OP_VDM");				// OP_VDM
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
			break;
		}
		else
		if (strReturn == "MODIFYTECHPIN")
		{
			Clerk.Index = 74;
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOtherFunctionProc():return");
	return TRUE;
}

// Clerk Download Param Proc
int	CRearApCtrl::ClerkDownloadParamProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkDownloadParamProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strMasterKey("");
	CString			strDriveNum("");
	CString			strKeyFilePathName("");
	CString			strDirSeperator("\\");
	CFileStatus		TempFileStatus;

	nScrNum = 973;												// Display Special Mode
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

//			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			strTempClerkInformation = ClerkInformationMake(12);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			if (Clerk.PowerOffFlag)								// 2004.07.05
			{
				//SetShareData("OP_COMMAND", "OP_POWEROFF");	// OP_POWEROFF
				SetShareData("OP_COMMAND", "OP_REBOOT");		// 修改单次存款最大张数，要重启系统 change by liyi 2008.05.14
				SetShareData("OP_RESULT", OP_DO);
				ClerkWaitResult();								// Clerk Wait Result
				Clerk.Index = 1;								// Goto Select Mode
			}
			else												// 2006.07.07
			{
				Clerk.Index = 70;								// Clerk Post Tech Menu
			}
			break;
		}
		else
		if (strReturn == "DOWNLOADCONTROLPARA")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNLOADCONTROLPARA");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "DOWNLOADCARDTABEL")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNLOADCARDTABEL");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "DOWNLOADEXTPARA")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNLOADEXTPARA");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "CHANGECONTROLPARA")
		{
			Clerk.Index = 18;									// Clerk Set Param Procedure
			break;
		}
		else
		if (strReturn == "DOWNLOADICCARDTABLE")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
				
			SetShareData("OP_COMMAND", "OP_DOWNLOADICCARDTABLE");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "DOWNLOADPBOCPARA")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
				
			SetShareData("OP_COMMAND", "OP_DOWNLOADPBOCPARA");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "DOWNLOADAIDLIST")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
				
			SetShareData("OP_COMMAND", "OP_DOWNLOADAIDLIST");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
		else
		if (strReturn == "DOWNLOADECHANNEL")
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";
			
			SetShareData("OP_COMMAND", "OP_DOWNLOADECHANNEL");
			SetShareData("OP_RESULT", OP_DO);
			ClerkWaitResult();									// Clerk Wait Result
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkDownloadParamProc():return");
	return TRUE;
}

// Clerk Tech Modify Adm Pin Procedure						// 2005.03.24
int CRearApCtrl::ClerkTechModifyAdmPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTechModifyAdmPinProc()");

	int				nScrNum = 0;
	int				RetryCnt = 3;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strTemp("");
	CString			strFirstPassWord("");
	CString			strSecondPassWord("");
	CString			strPrintData("");

	while (RetryCnt)
	{
		nScrNum = 913;											// Display Input Pin
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 72;
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strFirstPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 72;
				break;
			}
		}

/////////////////////////////////////////////////////////////////////////////
		nScrNum = 914;											// Display Input Pin Again
		nInitFlag = TRUE;
		strTempClerkInformation = "";
		strSaveClerkInformation = "";
		strTempClerkMessage = "";
		strSaveClerkMessage = "";

		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)			
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 72;
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
				strSecondPassWord.Format("%s", strTempArray[1]);
			else
			{													// Flash Error Message
				strTempClerkMessage = GuideMsgTbl[28][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 72;
				break;
			}
		}

		if(strFirstPassWord == strSecondPassWord)
		{
			strTempClerkMessage = GuideMsgTbl[26][OpDeviceLangMode];
			strSaveClerkMessage = strTempClerkMessage;
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
			strTempClerkMessage = "";

			strPrintData = "OP_JPRPRINT||";						// 打印流水   by zjd
			strTemp.Format("%2.2s:%2.2s:%2.2s OPERATOR TECHer change PIN",
										GetTime().GetBuffer(0),
										&(GetTime().GetBuffer(0)[2]),
										&(GetTime().GetBuffer(0)[4]));
			strPrintData += strTemp;
			
			SetShareData("OP_COMMAND", strPrintData);	
			SetShareData("OP_RESULT", OP_DO);		// OP_JPRPRINT
			ClerkWaitResult();

			RegSetStr(_REGKEY_POSTTECHADMINPIN, "00", strFirstPassWord);
			Clerk.AtmOpMessage = GuideMsgTbl[23][OpDeviceLangMode];
			Clerk.UpdateMessage = TRUE;				// Successfully Message
			Clerk.Index = 72;
			break;
		}
		else
		{
			if (!--RetryCnt)
			{													// Operation Miss Error Message
				strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
				fnSCR_DisplayString(48, strTempClerkMessage);
				fnSCR_DisplayMessage();
				Delay_Msg(2000);

				Clerk.Index = 72;
				break;
			}
			else
			{													// PassWord Missmatch
				strTempClerkMessage = GuideMsgTbl[29][OpDeviceLangMode];
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTechModifyAdmPinProc():return");
	return TRUE;
}

// Clerk Op Adm Input Pin Procedure
int	CRearApCtrl::ClerkOpAdmInputPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkOpAdmInputPinProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strPassWord("");

	strPassWord = RegGetStr(_REGKEY_POSTADMINPIN, ZERO2, "");
	if (strPassWord == "")
	{
		strPassWord = GetSprintf("000000");                     //by hyl 
//		Clerk.Index = 60;
//		return TRUE;
	}
	
	nScrNum = 902;												// Display Input Pin
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 1;									// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == strPassWord))
			{
				Clerk.Index = 60;
				break;
			}
			else
			{
				nScrNum = 903;									// Display Input Pin Again
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}													// PassWord Error Message
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkAsInputPinProc():return");
	return TRUE;
}

// Clerk Op Adm Input Pin Procedure
int	CRearApCtrl::ClerkTechAdmInputPinProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTechAdmInputPinProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;
	CString			strPassWord("");

	strPassWord = RegGetStr(_REGKEY_POSTTECHADMINPIN, ZERO2, "");
	if (strPassWord == "")
	{
		strPassWord = GetSprintf("111111");
//		Clerk.Index = 70;
// 		return TRUE;
	}
	
	nScrNum = 902;												// Display Input Pin
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 1;									// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if ((strTempArray.GetSize() >= 2)	&&
				(strTempArray[1] == strPassWord))
			{
				Clerk.Index = 70;
				break;
			}
			else
			{
				nScrNum = 903;									// Display Input Pin Again
				strTempClerkMessage = GuideMsgTbl[25][OpDeviceLangMode];
			}													// PassWord Error Message
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTechAdmInputPinProc():return");
	return TRUE;
}

// Clerk Tech Input AddCash Mode Procedure
int	CRearApCtrl::ClerkTechAddCashModeProc()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTechAddCashModeProc()");

	int				nScrNum = 0;
	int				nInitFlag = TRUE;
	CString			strTempClerkInformation("");
	CString			strSaveClerkInformation("");
	CString			strTempClerkMessage("");
	CString			strSaveClerkMessage("");
	CString			strReturn("");
	CStringArray	strTempArray;

	nScrNum = 974;												// Display Input Pin
	while (TRUE)
	{	
		while (TRUE)
		{
			Delay_Msg(100);										// Performance Adjust
			ClerkStatusUpdate();
			if (Clerk.UpdateScreen)
			{
				Clerk.UpdateScreen = FALSE;
				return TRUE;
			}
			if (Clerk.UpdateMessage)
			{
				Clerk.UpdateMessage = FALSE;
				strTempClerkMessage = Clerk.AtmOpMessage;
				nInitFlag = TRUE;
			}

			strTempClerkInformation = ClerkInformationMake(Clerk.Index);
			if ((nInitFlag == TRUE)										||
				(strTempClerkInformation != strSaveClerkInformation)	||
				(strTempClerkMessage != strSaveClerkMessage))
			{
				nInitFlag = FALSE;
				strSaveClerkInformation = strTempClerkInformation;
				strSaveClerkMessage = strTempClerkMessage;
				fnSCR_DisplayStringArray(1, strTempClerkInformation);
				fnSCR_DisplayString(48, strTempClerkMessage);
				if (fnSCR_GetCurrentScreenNo() == nScrNum)
					fnSCR_DisplayUpdate(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
				else 
					fnSCR_DisplayScreen(nScrNum, K_NO_WAIT, PIN_INPUT_MODE);
			}

			if (fnSCR_ScanDeviceAction(DEV_SOP, 0, EVENT_IN) == DEV_SOP)
				break;
		}

		nInitFlag = TRUE;										// Display | Update
		strReturn = fstrSCR_GetKeyString();
		SplitString(strReturn, ":", strTempArray);
		if (strReturn == S_EXIT)
		{
			Clerk.Index = 70;									// Clerk Select Mode Procedure
			break;
		}
		else
		{
			if (strTempArray.GetSize() >= 2)
			{
				if ((strTempArray[1] == "0") || (strTempArray[1] == "1"))
				{
					strTempClerkMessage = GuideMsgTbl[23][OpDeviceLangMode];
					RegSetStr(_REGKEY_ADDCASHMODE, ZERO2, strTempArray[1]);
				}
				else
					strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
			}
			else
			{
					strTempClerkMessage = GuideMsgTbl[24][OpDeviceLangMode];
			}
			fnSCR_DisplayString(48, strTempClerkMessage);
			fnSCR_DisplayMessage();
		}
	}

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "ClerkTechAddCashModeProc():return");
	return TRUE;
}


// 串口设备句柄
HANDLE hComm;

// 打开串口
// 输入: pPort - 串口名称或设备路径，可用"COM1"或"\\.\COM1"两种方式，建议用后者
//       nBaudRate - 波特率       9600
//       nParity - 奇偶校验       0
//       nByteSize - 数据字节宽度 8
//       nStopBits - 停止位       1
BOOL CRearApCtrl::fnAPP_OpenComm(const char* pPort, int nBaudRate, int nParity, int nByteSize, int nStopBits)
{
	DCB dcb;		// 串口控制块
//	int timeout1 = IniGetInt(_TIMEDEFINE_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TIMEDEFINE_READINTERVALTIMEOUT,100);
//	int timeout2 = IniGetInt(_TIMEDEFINE_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TIMEDEFINE_READTOTALTIMEOUTMULTIPLIER,1);
//	int timeout3 = IniGetInt(_TIMEDEFINE_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TIMEDEFINE_READTOTALTIMEOUTCONSTANT,3000);
//	int timeout4 = IniGetInt(_TIMEDEFINE_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TIMEDEFINE_WRITETOTALTIMEOUTMULTIPLIER,1);
//	int timeout5 = IniGetInt(_TIMEDEFINE_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TIMEDEFINE_WRITETOTALTIMEOUTCONSTANT,3000);
	int timeout1 = 100;
	int timeout2 = 1;
	int timeout3 = 3000;
	int timeout4 = 1;
	int timeout5 = 3000;
	
	COMMTIMEOUTS timeouts = {	// 串口超时控制参数
	    	timeout1,				// 读字符间隔超时时间: 100 ms
			timeout2,					// 读操作时每字符的时间: 1 ms (n个字符总共为n ms)
			timeout3,				// 基本的(额外的)读超时时间: 500 ms
			timeout4,					// 写操作时每字符的时间: 1 ms (n个字符总共为n ms)
			timeout5};				// 基本的(额外的)写超时时间: 100 ms
		
		hComm = CreateFile(pPort,	// 串口名称或设备路径
			GENERIC_READ | GENERIC_WRITE,	// 读写方式
			0,				// 共享方式：独占
			NULL,			// 默认的安全描述符
			OPEN_EXISTING,	// 创建方式
			0,				// 不需设置文件属性
			NULL);			// 不需参照模板文件
		
		if(hComm == INVALID_HANDLE_VALUE) return FALSE;		// 打开串口失败
		
		GetCommState(hComm, &dcb);		// 取DCB
		
		dcb.BaudRate = nBaudRate;
		dcb.ByteSize = nByteSize;
		dcb.Parity = nParity;
		dcb.StopBits = nStopBits;
		
		SetCommState(hComm, &dcb);		// 设置DCB
		
		SetupComm(hComm, 4096, 1024);	// 设置输入输出缓冲区大小
		
		SetCommTimeouts(hComm, &timeouts);	// 设置超时
		
		return TRUE;
}

// 关闭串口
BOOL CRearApCtrl::fnAPP_CloseComm()
{
	return CloseHandle(hComm);
}

// 写串口
// 输入: pData - 待写的数据缓冲区指针
//       nLength - 待写的数据长度
// 返回: 实际写入的数据长度
int CRearApCtrl::fnAPP_WriteComm(void* pData, int nLength)
{
	DWORD dwNumWrite;	// 串口发出的数据长度
	
	WriteFile(hComm, pData, (DWORD)nLength, &dwNumWrite, NULL);
	
	return (int)dwNumWrite;
}

// 读串口
// 输入: pData - 待读的数据缓冲区指针
//       nLength - 待读的最大数据长度
// 返回: 实际读出的数据长度
int CRearApCtrl::fnAPP_DvrDataComm(int DvrCmdCode,int DvrChanIdx,int DvrTimeOut,char UseStat[8])
{	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CRearApCtrl::fnAPP_DvrDataComm()");
    CString TranType;
//	char AtmSerialNumber[9];
// 	char BranchNumber[10];
	BYTE TempSum;
	BYTE TempSumRe[2];
	BYTE TempDataSend[120];
	BYTE TempDataReceive[120];
	CString m_strPort;
	m_strPort = GetSprintf("\\\\.\\%s",IniGetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_OPENCOMM,"COM10"));
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm()CommPort[%s]",m_strPort);
     if(!fnAPP_OpenComm(m_strPort,9600,NOPARITY,8,ONESTOPBIT))   
	{ 
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_OpenComm() openfail");
	    fnAPP_CloseComm();
        fnAPP_OpenComm(m_strPort,9600,NOPARITY,8,ONESTOPBIT);	
	}
	memset(&DvrCmdDataSend,0x00, sizeof(DvrCmdDataSend));
	memset(&DvrCmdDataReceive,0x00, sizeof(DvrCmdDataReceive));
	memset(&TempSum,0x00, sizeof(TempSum));
	memset(&TempSumRe,0x00, sizeof(TempSumRe));
	memset(&TempDataSend,0x00, sizeof(TempDataSend));
	memset(&TempDataReceive,0x00, sizeof(TempDataReceive));
	TranType = "ADDCASH";		
	DvrCmdDataSend.StartCode[0] = 0x02;
	DvrCmdDataSend.StartCode[1] = 'A';
	DvrCmdDataSend.StartCode[2] = 'T';
	DvrCmdDataSend.StartCode[3] = 'M';
	
	memcpy(DvrCmdDataSend.CmdCode,GetSprintf("%03d",DvrCmdCode),3);          //命令码
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.CmdCode[%s]",DvrCmdDataSend.CmdCode);
	memcpy(DvrCmdDataSend.Time, GetSprintf("00%4.4s%2.2s%2.2s%2.2s%2.2s%2.2s", GetDate().GetBuffer(0),
		&GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
		GetTime().GetBuffer(0), &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4]),16);	 //时间日期
//	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.Time[%s]",DvrCmdDataSend.Time);
	memcpy(DvrCmdDataSend.ChanIdx,GetSprintf("%04d",DvrChanIdx),4);          //叠加画面
	memcpy(DvrCmdDataSend.TimeOut,GetSprintf("%03d",DvrTimeOut),3);          //叠加时间
	memcpy(DvrCmdDataSend.Line1Data,GetSprintf("%8.8s", 
		Clerk.AtmSerialNumber),8);
	memcpy(DvrCmdDataSend.Line2Data,GetSprintf("%012.12s%08.8s%8.8s00%4.4s%2.2s%2.2s%2.2s%2.2s%2.2s", 
		Clerk.BranchNumber,
		TranType, 
		UseStat,
		GetDate().GetBuffer(0),
		&GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
		GetTime().GetBuffer(0), &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4]),44);    
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.CmdCode[%s]",DvrCmdDataSend.CmdCode);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.Time[%s]",DvrCmdDataSend.Time);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.ChanIdx[%s]",DvrCmdDataSend.ChanIdx);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.TimeOut[%s]",DvrCmdDataSend.TimeOut);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.Line1Data[%s]",DvrCmdDataSend.Line1Data);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DvrCmdDataSend.Line2Data[%s]",DvrCmdDataSend.Line2Data);


	memcpy(&TempDataSend,&DvrCmdDataSend,120);
 //    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDataSend[%s]",TempDataSend);
	TempSum = TempDataSend[0];
	for (int j=0; j <118; j++ )
	{
		TempSum = TempSum ^ TempDataSend[j+1];
	} 
    memcpy(DvrCmdDataSend.Checksum, GetSprintf("%02x", TempSum, 1),2);
    if(DvrCmdDataSend.Checksum[0] >= 'a'&&DvrCmdDataSend.Checksum[0]<='f')
		DvrCmdDataSend.Checksum[0] =DvrCmdDataSend.Checksum[0] -32;
	if(DvrCmdDataSend.Checksum[1] >= 'a'&&DvrCmdDataSend.Checksum[1]<='f')
		DvrCmdDataSend.Checksum[1] =DvrCmdDataSend.Checksum[1] -32;

	fnAPP_WriteComm(&DvrCmdDataSend,120);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn::fnAPP_DvrDataComm():DvrCmdDataSend.Checksum[%2s]",DvrCmdDataSend.Checksum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CRearApCtrl::fnAPP_DvrDataComm():DataSend[%s]",GetSprintf("%120.120s",&DvrCmdDataSend,120));

	fnAPP_ReadComm(&TempDataReceive,16);
	fnAPP_CloseComm();
	TempSum = TempDataReceive[0];
 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CRearApCtrl::fnAPP_DvrDataComm():TempDataReceive[%16.16s]",TempDataReceive);
	for (int i=0; i <11; i++ )
	{
		TempSum = TempSum ^ TempDataReceive[i+1];
	} 
	memcpy(TempSumRe,GetSprintf("%02x", TempSum, 1),2);
    if(TempSumRe[0] >= 'a'&& TempSumRe[0]<='f')
         TempSumRe[0] = TempSumRe[0] - 0x20;
    if(TempSumRe[1] >= 'a'&& TempSumRe[0]<='f')
		 TempSumRe[1] = TempSumRe[1] - 0x20;
		
	if(TempDataReceive[12]==TempSumRe[0]&& TempDataReceive[13]==TempSumRe[1])
	{
		memcpy(&DvrCmdDataReceive,GetSprintf("%-16.16s",&TempDataReceive),16);
		 IniSetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRSTATUS,GetSprintf("%4.4s",DvrCmdDataReceive.DvrStatus));
	     IniSetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_CHANSTATUS,GetSprintf("%4.4s",DvrCmdDataReceive.ChanStatus));
         MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CRearApCtrl::fnAPP_DvrDataComm():DvrCmdDataReceive[%16.16s]",GetSprintf("%16.16s",&DvrCmdDataReceive),16);
		 return TRUE;
	}
	else 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CRearApCtrl::fnAPP_DvrDataComm():return FALSE");
		return FALSE;		
	}
}

int CRearApCtrl::fnAPP_ReadComm(void* pData, int nLength)
{
	DWORD dwNumRead;	// 串口收到的数据长度
	
	ReadFile(hComm, pData, (DWORD)nLength, &dwNumRead, NULL);
	
	return (int)dwNumRead;
}



