/////////////////////////////////////////////////////////////////////////////
//	TranMenu.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	MUNU FUNCTION
/////////////////////////////////////////////////////////////////////////////
// Main Menu Display
int CTranCmn::fnAPP_MainMenuDisplay(int InitFlag)
{
	static	int		TranAvailSave = 0;							// Transaction Avail Save
	int		i;
	int		ScrNum = 0;
	int		PinInputMode = PIN_DISABLE_MODE;

	if (!InitFlag)
	{
		if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	// Speed Up
			return T_OK;

		if ((m_pDevCmn->fnSCR_GetCurrentScreenNo() == MenuTblTemp[MenuIndex].MenuScrNum) &&
			(m_pDevCmn->TranAvail == TranAvailSave))			// Transaction Avail Check
			return T_OK;
	}

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->McuReadMSOnlyFlag = FALSE;						// Mcu Read Only MS Flag:2010.10.20
/////////////////////////////////////////////////////////////////////////////
	TranAvailSave = m_pDevCmn->TranAvail;						// Transaction Avail Save
	fnAPP_InitializeDS();										// Initialize DS
	memcpy(MenuTblTemp, MenuTbl, sizeof(MenuTblTemp));			// Menu Button Initial
	switch (m_pDevCmn->AtmDefine.MachineType)					// Main Screen Adjust
	{
		case ATM:
			MenuTblTemp[0].MenuScrNum = 1;						// ATM
			MenuTblTemp[1].MenuScrNum = 2;						// 
			break;
		case CDP:
			MenuTblTemp[0].MenuScrNum = 3;						// CDP
			MenuTblTemp[1].MenuScrNum = 4;						// 
			break;
		case CD:
			MenuTblTemp[0].MenuScrNum = 5;						// CD
			MenuTblTemp[1].MenuScrNum = 6;						// 
			break;
	}
	TranAvail = m_pDevCmn->TranAvail;							// Transaction Avail
	WithAvail = m_pDevCmn->WithAvail;							// Withdraw Avail
	DepAvail = m_pDevCmn->DepAvail;								// Deposit Avail
	PbmAvail = m_pDevCmn->PbmAvail;								// Passbook Avail
MsgDump(TRACE_CODE_MODE, "log", __FILE__, __LINE__, "fnAPP_MainMenuDisplay:"					\
													"m_pDevCmn->AtmDefine.MachineType[%d], "	\
													"m_pDevCmn->AtmDefine.MachineKind[%d], "	\
													"TranAvail[%x], "							\
													"WithAvail[%x], "							\
													"DepAvail[%x], "							\
													"PbmAvail[%x]",
													m_pDevCmn->AtmDefine.MachineType,
													m_pDevCmn->AtmDefine.MachineKind,
													TranAvail, 
													WithAvail, 
													DepAvail, 
													PbmAvail);

/////////////////////////////////////////////////////////////////////////////

	switch (WithAvail)											// Withdraw Avail Status 	
	{
		case TRAN_WITH_100:
		case TRAN_WITH_50:
//		case TRAN_WITH_20:										// 2004.11.05
//		case TRAN_WITH_10:										// 2004.11.05
		case TRAN_WITH_BOTH:
			break;

		case TRAN_WITH_NOT:
		default: 
			fnAPP_MenuClearTranProc(0, TRAN_WITH, MENU_NULL);	// Delete Withdraw
			break;
	}

	switch (DepAvail)											// Deposit Avail Status
	{
		case TRAN_DEP_CASH:
			break;

		case TRAN_DEP_NOT:
		default: 
			fnAPP_MenuClearTranProc(0, TRAN_DEP, MENU_NULL);	// Delete Deposit
			break;
	}

	switch (PbmAvail)											// Passbook Avail Status
	{
		case TRAN_PBM_OK:										
			break;

		case TRAN_PBM_NOT:
		default:												// Delete Passbook
			fnAPP_MenuClearTranProc(0, TRAN_PB, MENU_NULL);
			fnAPP_MenuClearMenuCode(0, MENU_PB, ~MENU_PB);
			break;
	}
	
/////////////////////////////////////////////////////////////////////////////
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 1Step)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 2Step)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 3Step)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 4Step)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 5Step)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 6Step)
	fnAPP_MenuMake(MenuTblTemp);								// Menu Make(Sub Menu 7Step)

	// Menu Adjust Section(INI Setting)
	if (m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET)			// IC Card Not Used
	{
		fnAPP_MenuClearTranProc(0, TRAN_IC, MENU_NULL);
		fnAPP_MenuClearMenuCode(0, MENU_IC, ~MENU_IC);
	}

	// Menu Adjust Section(Host Request)						// 2005.09.09
	//增加101硬加密模式 by hyl 20070902
	if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
	{
		MenuTblTemp[2].MenuPinInputMode = PIN_PASSWORD_MODE4;
		MenuTblTemp[5].MenuPinInputMode = PIN_PASSWORD_MODE4;
	}
	
/////////////////////////////////////////////////////////////////////////////
	memcpy(&MenuTblSave, &MenuTblTemp, sizeof(MenuTblSave));	// Menu Table Save
	MenuBeforeInputFlag	= FALSE;								// Menu Before Input Flag
	MenuDoingFlag = TRUE;										// Menu Doing Flag
	MenuContinueFlag = FALSE;									// Menu Continue Flag

/////////////////////////////////////////////////////////////////////////////



	fnAPP_MenuAvailCheck(MenuIndex);							// Menu Avali Check
	if (MenuAvail & MENU_MCU)									// Card Enable
		fnAPD_CardEnDisable(ENABLE, TRUE);
	else fnAPD_CardEnDisable(DISABLE, TRUE);

	if (MenuAvail & MENU_PB)									// Passbook Enable
		fnAPD_PbEnDisable(ENABLE, TRUE);
	else fnAPD_PbEnDisable(DISABLE, TRUE);

	fnAPP_MenuBtnCheck(MenuIndex);								// Menu Button Check

	ScrNum = MenuTblTemp[MenuIndex].MenuScrNum;
	PinInputMode = MenuTblTemp[MenuIndex].MenuPinInputMode;

	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++) 
	{															// Delete Button
		if ((MenuTblTemp[MenuIndex].MenuInfo[i][0] == MENU_NULL) && 
			(MenuTbl[MenuIndex].MenuInfo[i][0] != MENU_NULL))
			m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
	}

	switch (m_pDevCmn->AtmDefine.MachineType)
	{
		case ATM:												// Display Message
		case CDP:
		case CD:
			break;
	}

//////////////////////////////////////////////////////////////////////////////
	if (ScrNum == 5)              //by liyi 20111024
	{
		CString strAdNameList = "";
		CString strAdName = "";
		int ADENDCOUNT = 0;
		int	ADSetNum = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM);
		int ADPicNum = 0;			// added by liuxl 20120208

		for(int nADCOUNT = 1;nADCOUNT <= ADSetNum;nADCOUNT++)		// added by liuxl 20120208
		{   
			if(	IniGetStr(_TRANSACTION_INI,_INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",nADCOUNT))!="" && 
				IniGetStr(_TRANSACTION_INI,_INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",nADCOUNT))=="")
			{
				strAdName = GetSprintf("%d",nADCOUNT);
				strAdNameList = strAdNameList + strAdName + "|";
				ADPicNum++;
			}

		}
		
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AdNameList:[%s]",strAdNameList); 

//		int ADPicNum = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM);
		int ADTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADTIME);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADPicNum:[%d]",ADPicNum); 
		int iNoCardTransFlag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_NOCARDTRANSFLAG,0);
		int iEChannelFlag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELFLAG, 1);
		int iEChannelResult = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 0);
		CString strEChannelNum = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPARM, "ECHANNELNUM", "0");
		m_pDevCmn->fnSCR_DisplayNumeric(1, ADPicNum);
        m_pDevCmn->fnSCR_DisplayNumeric(2, ADTime);
		if (iNoCardTransFlag == 0)
			m_pDevCmn->fnSCR_DisplayNumeric(3, 1);
		else
			m_pDevCmn->fnSCR_DisplayNumeric(3, 0);
		m_pDevCmn->fnSCR_DisplayString(4,strAdNameList);
		if (iEChannelFlag == 0	||
			Asc2Int(strEChannelNum) == 0	||
			iEChannelResult == 0)
			m_pDevCmn->fnSCR_DisplayNumeric(5, 1);
		else
			m_pDevCmn->fnSCR_DisplayNumeric(5, 0);
// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADPicNum:[]",ADPicNum); 
	}
///////////////////////////////////////////////////////////////////////////////
	
	if ((PinInputMode == PIN_PASSWORD_MODE)			||			// 2005.04.19
		(PinInputMode == PIN_PASSWORD_MODE2)		||
		(PinInputMode == PIN_PASSWORD_MODE3)		||
		(PinInputMode == PIN_PASSWORD_MODE4)		||
		(PinInputMode == PIN_PASSWORD_MODE5)		||
		(PinInputMode == PIN_PASSWORD_MODE6)		||
		(PinInputMode == PIN_PASSWORD_MODE7))
		//m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_NO_WAIT, PinInputMode, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_TRUE, PIN_PASSWORD_TERM);
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_NO_WAIT, PinInputMode, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
	else
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, K_NO_WAIT, PinInputMode);
																// Display Screen
	return T_OK;
}

// Main Menu Device Event
int CTranCmn::fnAPP_MainMenuDeviceEvent(int CheckTime)
{
	ULONG  WaitTime = TimerSet(CheckTime);
	int	   CheckDev = DEV_NONE;
	
	while (TRUE)
	{
		Delay_Msg(100);											// Performance Adjust

		CheckDev |= DEV_SCR;									// Mouse Or Touch

		if ((!CardTran)									&&		// No Card
			(DevEnDisable & DEV_MCU))
			CheckDev |= DEV_MCU;

		if ((!PbTran)									&&		// No Passbook
			(DevEnDisable & DEV_PBM))
			CheckDev |= DEV_PBM;

		if (m_pDevCmn->fnAPL_GetDeviceEvent(CheckDev))
		{
			if ((m_pDevCmn->DeviceTran & CheckDev) == m_pDevCmn->DeviceTran)
				break;
		}

		if ((!CheckTime) ||
			(CheckTimer(WaitTime)))
			return T_TIMEOVER;									// AtmModeCtrl Return
	}
	
	return T_OK;
}

// Main Menu Continue Check
int CTranCmn::fnAPP_MainMenuContinueCheck()
{
	int		SlipStatusSave = m_pDevCmn->SlipStatus;
	
// Before Procedure
	m_pDevCmn->fnAPL_SetSensorInfo();
	m_pDevCmn->fnAPL_GetAvailTrans();
	m_pDevCmn->fnAPL_CheckDevice();
	m_pDevCmn->fnAPL_CheckMaterial(DEV_CSH);

// Mode Setting(Condition Procedure)
//	if ((TranAvail != m_pDevCmn->TranAvail)		||
//		(WithAvail != m_pDevCmn->WithAvail)		||
//		(DepAvail != m_pDevCmn->DepAvail)		||
//		(PbmAvail != m_pDevCmn->PbmAvail))
//		MenuContinueFlag = FALSE;
//	else
	if ((TranAvail & TRAN_ALL_OK)	&&							// 2004.08.23
		(m_pDevCmn->TranAvail == TRAN_ALL_NOT))
		MenuContinueFlag = FALSE;
	else
	if ((WithAvail & TRAN_WITH_BOTH)	&&
		(m_pDevCmn->WithAvail == TRAN_WITH_NOT))
		MenuContinueFlag = FALSE;
	else
	if ((DepAvail & TRAN_DEP_CASH)	&&
		(m_pDevCmn->DepAvail == TRAN_DEP_NOT))
		MenuContinueFlag = FALSE;
	else
	if ((PbmAvail & TRAN_PBM_OK)	&&
		(m_pDevCmn->PbmAvail == TRAN_PBM_NOT))
		MenuContinueFlag = FALSE;
	else
	if ((SlipStatusSave != SLIP_EMPTY_PAPER) &&					// 2005.02.28
		(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
		MenuContinueFlag = FALSE;
	else
	if (!m_pDevCmn->fnAPL_CheckError())
		MenuContinueFlag = FALSE;
	else
	if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
		MenuContinueFlag = FALSE;
	else
	if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
		(m_pDevCmn->CshChangeLoc == CHANGE_POS)			&&
		(m_pDevCmn->CshLoc == NORMAL_POS)				&&
		(!m_pDevCmn->fnAPL_AutoResetCsh()))
		MenuContinueFlag = FALSE;
	else
	if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
		(m_pDevCmn->RejectCSTLoc == NORMAL_POS)			&&
		(!m_pDevCmn->fnAPL_AutoReadyCsh()))
		MenuContinueFlag = FALSE;
	else
	if (!m_pDevCmn->OpenKey)
		MenuContinueFlag = FALSE;
	else														// 2005.11.07
	if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_SPR))	&&
		(m_pDevCmn->fnAPL_GetUseDevice(DEV_SPR))			&&
		((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
		 (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)))
		MenuContinueFlag = FALSE;
	else
	if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_JPR))	&&
		(m_pDevCmn->fnAPL_GetUseDevice(DEV_JPR))			&&
		((m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	||
		 (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)))
		MenuContinueFlag = FALSE;
	else														// 2006.08.22
	if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MCU))	&&
		(m_pDevCmn->fnAPL_GetUseDevice(DEV_MCU))			&&
		(m_pDevCmn->CardMaxRetractCnt > 0)					&&
		(m_pDevCmn->CardRetractCnt >= m_pDevCmn->CardMaxRetractCnt))
		MenuContinueFlag = FALSE;
	else
	if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_CSH))	&&
		(m_pDevCmn->fnAPL_GetUseDevice(DEV_CSH))			&&
		((m_pDevCmn->CshLoc != NORMAL_POS)				||
		 (m_pDevCmn->RejectCSTLoc != NORMAL_POS)))
		MenuContinueFlag = FALSE;
	else
	if (m_pDevCmn->DoorStatus == DOOR_OPENED)
		MenuContinueFlag = FALSE;
	else
	if (m_pDevCmn->fnAPL_GetDownDevice((m_pDevCmn->fnAPL_GetUseDevice(DEV_MAIN) & (~(m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MAIN)))) | DEVICE_YES_CHECK))
		MenuContinueFlag = FALSE;								// 2005.11.07
	else
	if (!m_pDevCmn->fnAPL_CheckHostLine())						// Check Line
		MenuContinueFlag = FALSE;
	else
	if (!m_pDevCmn->fnAPL_CheckHostOpen())						// Check Open
		MenuContinueFlag = FALSE;
	else
	if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))	&&
		(m_pDevCmn->fnCSH_GetAutoloadCondition() != NORMAL))
		MenuContinueFlag = FALSE;
	else														// Next Mode
	{
		MenuContinueFlag = TRUE;
	}

// Mode Setting(Non Condition Procedure)
	if (!m_pDevCmn->fnAPL_DisplayPrintError())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_DisplayInformationLed())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_DisplayInformationIndicator())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_SendInformationRpl())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_SendInformationInform())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
		MenuContinueFlag = FALSE;
	if (!m_pDevCmn->fnAPL_CheckPowerStatus(TRUE))				// 2004.08.23
		MenuContinueFlag = FALSE;
																// 2004.08.23
	TranAvail = m_pDevCmn->TranAvail;							// Transaction Avail
	WithAvail = m_pDevCmn->WithAvail;							// Withdraw Avail
	DepAvail = m_pDevCmn->DepAvail;								// Deposit Avail
	PbmAvail = m_pDevCmn->PbmAvail;								// Passbook Avail
	TranAvail = (WithAvail | DepAvail | PbmAvail);
	
	if (MenuContinueFlag)
		return T_OK;
	else
		return T_CANCEL;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Make
int	CTranCmn::fnAPP_MenuMake(void* MenuTblMake)
{
	MENUHeader*	MenuTblWork = (MENUHeader*)MenuTblMake;
	int		i, j, k;

	for (i = 0; i < itemof(MenuTbl); i++)
	{
		j = 0;													// 0Step Menu Make

		if (MenuTblWork[i].MenuInfo[j][0])						// Exist Skip : 2004.07.08
			;
		else
		{
			MenuTblWork[i].MenuInfo[j][0] = MENU_NULL;			// Initial

			if (!MenuTblWork[i].MenuInfo[j][1])
				MenuTblWork[i].MenuInfo[j][1] = i;
			for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)
			{
				MenuTblWork[i].MenuInfo[j][0] |=				// Menu Card
					MenuTblWork[MenuTblWork[i].MenuInfo[j][1]].MenuInfo[k][0];
			}

			if (!MenuTblWork[i].MenuInfo[j][2])
				MenuTblWork[i].MenuInfo[j][2] = i;
			for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	
			{
				MenuTblWork[i].MenuInfo[j][0] |=				// Menu Passbook
					MenuTblWork[MenuTblWork[i].MenuInfo[j][2]].MenuInfo[k][0];
			}

			if (!MenuTblWork[i].MenuInfo[j][3])
				MenuTblWork[i].MenuInfo[j][3] = i;
			for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)	
			{
				MenuTblWork[i].MenuInfo[j][0] |=				// Menu Input
					MenuTblWork[MenuTblWork[i].MenuInfo[j][3]].MenuInfo[k][0];
			}
		}

		for (j = 1; j < itemof(MenuTblWork[0].MenuInfo); j++)	
		{														// 1-nStep Menu Make
			if (MenuTblWork[i].MenuInfo[j][0])					// Exist Skip : 2004.07.08
				;
			else
			{
				if (MenuTblWork[i].MenuInfo[j][3])				// Menu Button
				{
					MenuTblWork[i].MenuInfo[j][0] = MENU_NULL;	// Initial
					for (k = 0; k < itemof(MenuTblWork[0].MenuInfo); k++)
					{
						MenuTblWork[i].MenuInfo[j][0] |= 
						MenuTblWork[MenuTblWork[i].MenuInfo[j][3]].MenuInfo[k][0];
					}
				}
			}
		}
	}
	return T_OK;
}

// Menu Clear Menu Code
int CTranCmn::fnAPP_MenuClearMenuCode(int Index, int MenuCode, int MenuValue)
{
	int		i, j;
	
	if (Index)
	{
		for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)	
		{
			if (MenuTblTemp[Index].MenuInfo[j][0] & MenuCode)
				MenuTblTemp[Index].MenuInfo[j][0] &= MenuValue;
		}
	}
	else
	{
		for (i = 0; i < itemof(MenuTblTemp); i++)
			for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
			{
				if (MenuTblTemp[i].MenuInfo[j][0] & MenuCode)
					MenuTblTemp[i].MenuInfo[j][0] &= MenuValue;
			}
	}
	
	return T_OK;
}

// Menu Clear Transaction Procedure
int CTranCmn::fnAPP_MenuClearTranProc(int Index, int TranValue, int MenuValue)
{
	int		i, j;

	if (Index)
	{
		for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
		{
			if (MenuTblTemp[Index].MenuInfo[j][4] == TranValue)
				MenuTblTemp[Index].MenuInfo[j][0] &= MenuValue;
		}
	}
	else
	{
		for (i = 0; i < itemof(MenuTblTemp); i++)
			for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
			{
				if (MenuTblTemp[i].MenuInfo[j][4] == TranValue)
					MenuTblTemp[i].MenuInfo[j][0] &= MenuValue;
			}
	}

	return T_OK;
}
	
// Menu Clear Transaction Code
int CTranCmn::fnAPP_MenuClearTranCode(int Index, int TranValue, int MenuValue)
{
	int		i, j;
	
	if (Index)
	{
		for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
		{
			if (MenuTblTemp[Index].MenuInfo[j][5] == TranValue)
				MenuTblTemp[Index].MenuInfo[j][0] &= MenuValue;
		}
	}
	else
	{
		for (i = 0; i < itemof(MenuTblTemp); i++)
			for (j = 0; j < itemof(MenuTblTemp[0].MenuInfo); j++)
			{
				if (MenuTblTemp[i].MenuInfo[j][5] == TranValue)
					MenuTblTemp[i].MenuInfo[j][0] &= MenuValue;
			}
	}
	
	return T_OK;
}

// Menu Avail Check
int CTranCmn::fnAPP_MenuAvailCheck(int Index, int SubIndex)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuAvailCheck:Index[%d], SubIndex[%d]", Index, SubIndex);
	int		i;

	MenuAvail = MENU_NULL;
	if (SubIndex)												// Choice
		MenuAvail |= MenuTblTemp[Index].MenuInfo[SubIndex][0];
	else														// Process & Menu All
	{
		for (i = 0; i < itemof(MenuTblTemp[0].MenuInfo); i++)
			MenuAvail |= MenuTblTemp[Index].MenuInfo[i][0];
	}

	if (MenuAvail)
		return T_OK;
	else 
		return T_CANCEL;
}
	
// Menu Button Check
int CTranCmn::fnAPP_MenuBtnCheck(int Index)
{
	int		i;

	MenuBtnCnt = 0;												// Menu Button Count
	MenuBtnIndex = 0;											// Menu Button Index
	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++)
	{
		if (MenuTblTemp[Index].MenuInfo[i][0] != MENU_NULL)
		{
			MenuBtnCnt++;
			if (MenuTblTemp[Index].MenuInfo[i][2] == ON)
				MenuBtnIndex = i;
		}
	}

	if (MenuBtnCnt)
		return T_OK;
	else 
		return T_CANCEL;
}

/////////////////////////////////////////////////////////////////////////////
// Menu Procedure
int CTranCmn::fnAPP_MenuProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc()");
	
	CString strReturn = "";

	if ((MenuContinueFlag)	||									// Menu Continue Flag
		(MainTranReturnFlag))									// Main Transaction Return Flag : 2006.08.22
	{
		MainTranReturnFlag = FALSE;								// Main Transaction Return Flag : 2006.08.22
		
		TranProc = TRAN_NULL;									// Clear
		TranCode = 0;
		TranCode2 = 0;

		memcpy(&MenuTblTemp, &MenuTblSave, sizeof(MenuTblTemp));// Menu Table Copy

		if (LowAmountFlag == 0)
		{
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
			{
				MenuIndex = 3;											// SELECT SERVICE
				MenuIndexSave = 2;										// INPUT PIN
				MenuIndexSub = 0;										// Menu Avail
			}
			else
			{
				MenuIndex = 9;											// SELECT SERVICE
				MenuIndexSave = 5;										// INPUT PIN
				MenuIndexSub = 0;										// Menu Avail
			}
		}
		else
		{
			if (LocalCardFlag == 3)										// added by liuxl 20110507
			{
				int nlen = 0;
				CString str;
				nlen = MakeUnPack(PbcAppDisp[PbcAppIndex].AID, str, PbcAppDisp[PbcAppIndex].AidLen);

				if (memcmp(str, "A000000333010101", nlen) == 0 ||
					memcmp(str, "A000000333010102", nlen) == 0)
				{
					MenuIndex = 7;											// SELECT SERVICE
					MenuIndexSave = 5;										// INPUT PIN
					MenuIndexSub = 0;										// Menu Avail
				}
				else
				{
					MenuIndex = 8;											// SELECT SERVICE
					MenuIndexSave = 0;										// INPUT PIN
					MenuIndexSub = 0;										// Menu Avail
				}
			}
			else
			if (POSTICType == 2)
			{
				MenuIndex = 8;											// SELECT SERVICE
				MenuIndexSave = 0;										// INPUT PIN
				MenuIndexSub = 0;										// Menu Avail
			}
			else
			{
				MenuIndex = 7;											// SELECT SERVICE
				MenuIndexSave = 5;										// INPUT PIN
				MenuIndexSub = 0;										// Menu Avail
			}
		}

		if (m_pDevCmn->TranResult)								// Host Ok
			;
		else
		{
			if ((memcmp(HostResp.RejectCode, "26", 2) == 0))
			{
				MenuIndex = 2;									// INPUT PIN
				MenuIndexSave = 1;								// PRPCESS INFORMATION
				MenuIndexSub = 0;								// Menu Avail
			}
		}

		m_pDevCmn->DeviceTran = DEV_MCU;						// Device Event Mcu Select
	}
		
	while (TRUE)
	{
		fnAPP_MenuDeviceProc();
		if (TranProc && TranCode)								// Menu Complete
			break;

//		if (strVersion == VERSIONHUNAN)							// 060411 湖南版本
		{
			if (!MenuIndexNext)
			{													// Reserved Transaction
				fnAPP_CancelProc(T_MSG, TranMsgTbl[4][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}
			else
			if ((MenuIndexNext == 3 && MenuIndex == 2)	||
				(MenuIndexNext == 6 && MenuIndex == 5))
			//	&& LocalCardFlag != 3)								// yaokq   //jl 2014.03.07
			{
				TranCode = TC_INQUIRY_CheckPin;
				TranCode2 = 0;
				int iNGCnt = 0;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "yaheiheio_TC_INQUIRY_CheckPin");
				while(TRUE)
				{
					iFstPIN = 1;
					fnAPP_SendHost();
					fnAPP_RecvHost();
					if(m_pDevCmn->bAgentMode == 1)
					{
						fnAPP_AgentIdxGetData(AGENT_INQUIRY_CheckPin);
					}
					
					if(m_pDevCmn->TranResult)
					{
						if (//strVersion == VERSIONHUNAN	&&			//jl 2014.03.07
							MenuIndexSave != 10)
							fnAPP_PMEAcceptInquiryConfirm();
							if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||// 2010.10.20
								(m_pDevCmn->McuReadMSOnlyFlag))
							{
								if (LocalCardFlag == 3)								// liyi chang 2008.10.20 
								{
									while (TRUE)
									{												// 显示选择交易帐户类型画面  by zjd
										m_pDevCmn->fnSCR_DisplayScreen(15, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
										strReturn = m_pDevCmn->fstrSCR_GetKeyString();
										if (strReturn == S_TIMEOVER)
											fnAPP_CancelProc(T_TIMEOVER);
										else
										if (strReturn == S_EXIT)
											fnAPP_CancelProc(T_EXIT);
										else
										if (strReturn == S_CANCEL)
											fnAPP_CancelProc(T_CANCEL);
										else
										{
											m_pTranCmn->AccountType = strReturn;		//Save AccountType   by zjd
											break;
										}
									}
								}
							}							
						break;
					}
					else if(memcmp(HostResp.RejectCode, "25", 2) == 0)		// liyi 
					{
						break;
					}
					else if(memcmp(HostResp.RejectCode, "26", 2) == 0)
					{
						if (iNGCnt < 3)
						{
							iNGCnt ++;
							fnAPP_PMEAcceptPassWord();						// Accept PassWord
							continue;
						}
						else
							fnAPP_CancelProc(T_CANCEL);
					}
					else												// 显示错误信息，进行相应处理  by zjd
					{
						if (HostResp.CardRetractFlag)							// Card Retract Flag
						{
							fnAPP_PMDProc();									// Message Display
							fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
							// Card Out
							fnAPD_MaterialOutCheckRetract(DEV_MCU, TRUE);		// Card Out Check & Retract
							fnAPP_PSPProc(DEV_SPR);								// Spr Print
							fnAPD_CheckDeviceAction(DEV_SPR);
							fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);// Spr Out
							fnAPD_MaterialOutCheckRetract(DEV_SPR);				// Spr Out Check & Retract
							fnAPP_CancelProc(T_CANCEL);
						}
						else if (HostResp.CardEjectFlag)								// Card Eject Flag
						{
							fnAPP_PMDProc();									// Message Display
							fnAPD_CheckDeviceAction(DEV_MCU);
							fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
							// Card Out
							fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
							fnAPP_CancelProc(T_CANCEL);
						}	
						else
						{
							if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
								(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
							{
								fnAPP_PMDProc();							// Message Display
								fnAPD_CheckDeviceAction(DEV_MCU);
								fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
								// Card Out
								fnAPD_MaterialOutCheckRetract(DEV_MCU);	
								// Card Out Check & Retract
								fnAPP_CancelProc(T_CANCEL);
							}
							else if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
							{
								fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
								MainTranReturnFlag = TRUE;
								break;
							}
							else
								fnAPP_CancelProc(T_CANCEL);
						}
					}
				}
				iFstPIN = 0;
			}

			if (MenuIndexSave == 10		&&
				MenuIndexNext != MenuIndex)
			{
				TranProc = TRAN_TRANS;	// Tran Procedure
				TranCode = TC_ECHANNEL;	// Tran Code
				TranCode2 = 0;			// Tran Code2
				break;
			}
			else
			if (MenuIndexSave == 10		&&
				MenuIndexNext == MenuIndex)
				;
			else
			{
				MenuIndexSave = MenuIndex;							// Menu Index Save
				MenuIndex = MenuIndexNext;
				MenuIndexSub = 0;
			}
		}
		//		else
		//		{
		//			if (!MenuIndexNext)
		//			{														// Reserved Transaction
		//				fnAPP_CancelProc(T_MSG, TranMsgTbl[4][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
		//			}
		//			else
		//			{
		//				MenuIndexSave = MenuIndex;							// Menu Index Save
		//				MenuIndex = MenuIndexNext;
		//				MenuIndexSub = 0;
		//			}
		//		}

		if (CardTran)											// Exist Card
		{
			m_pDevCmn->DeviceTran = DEV_MCU;
			fnAPP_MenuDeviceProc();
			if (TranProc && TranCode)							// Menu Complete
				break;
			if (MenuIndex != MenuIndexNext)						// Next Screen : 2002.04.23
				continue;					
		}

		if (PbTran)												// Exist Passbook
		{
			m_pDevCmn->DeviceTran = DEV_PBM;
			fnAPP_MenuDeviceProc();
			if (TranProc && TranCode)							// Menu Complete
				break;
			if (MenuIndex != MenuIndexNext)						// Next Screen : 2002.04.23
				continue;					
		}

		if (!MenuBeforeInputFlag	&&
			MenuIndex != 10)								// 2005.02.28
		{
			MenuBeforeInputFlag = TRUE;
			fnAPP_MenuBeforeInput();
		}
		fnAPP_MenuDisplay();
		fnAPP_MenuDeviceEvent();
	}

	// Menu Card Pb No Card Accept Section
	// Tran Code Chage Section
	TranProcSave = TranProc;									// Transaction Procedure Save
	TranCodeSave = TranCode;									// Transaction Code Save
	TranCode2Save = TranCode2;									// Transaction Code2 Save

	// New Card Transaction Section

	fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);

	// Transaction Check Section
	if (MenuAvail & MENU_NC)
	{
	}
	
	if (MenuAvail & MENU_TT)
	{
	}
	
	TranAvail = (WithAvail | DepAvail | PbmAvail);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc:"
													"CardTran[%x], "
													"PbTran[%x], "
													"MenuAvail[%x], "
													"TranProc[%d], "
													"TranCode[%d], "
													"TranCode2[%d], "
													"WithAvail[%x], "
													"DepAvail[%x], "
													"PbmAvail[%x], "
													"TranAvail[%x]",
													CardTran, 
													PbTran,
													MenuAvail, 
													TranProc, 
													TranCode, 
													TranCode2,
													WithAvail, 
													DepAvail, 
													PbmAvail, 
													TranAvail);

	if ((CardTran) && (PbTran))									// Two Media
	{															// Ivalid Media
		fnAPP_CancelProc(T_MSG, TranMsgTbl[5][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuProc():return");
	return T_OK;
}

// Menu Before Input
int CTranCmn::fnAPP_MenuBeforeInput()
{
	int		ScrNum = 26;
	int		iniScrNum = 1;
	CString strReturn("");

	//安全提示画面   by zjd
//	int CautionPicNum = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURENUM);
	int PoundageFlag = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_POUNDAGESETNUM, 2);
	
	while (TRUE)
	{
		m_pDevCmn->fnSCR_DisplayNumeric(1, 1);

		if (PoundageFlag)
			m_pDevCmn->fnSCR_DisplayNumeric(2, 1);
		else
			m_pDevCmn->fnSCR_DisplayNumeric(2, 0);

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);

		if (TranCode == TC_CASHTRANS	&&
			iniScrNum)
		{
			iniScrNum = 0;
			
			fnAPD_CardEnDisable(DISABLE);							// 关闭读卡器
			fnAPD_PbEnDisable(DISABLE);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_OFF);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_OFF);
			
			m_pDevCmn->McuReadMSOnlyFlag = TRUE;


			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfoDirect(GetSprintf("DATE:%4.4s/%2.2s/%2.2s ATMID:%8.8s",
														GetDate().GetBuffer(0), &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
														m_pProfile->NETWORK.AtmSerialNum));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ==> Cash Transfer >",
							GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfoDirect(GetSprintf("日期:%4.4s/%2.2s/%2.2s ATM号:%8.8s",
														GetDate().GetBuffer(0), &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
														m_pProfile->NETWORK.AtmSerialNum));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 插卡 卡号:现金转账 >",
							GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4])));
			}

		}

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONTINUE)
			break;
		else
		if (strReturn == S_EXIT)
			fnAPP_CancelProc(T_EXIT);
		else
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
		if (strReturn == "POUNDAGE")
			fnAPP_PMEAcceptChargeConfirm();
		else
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM) != 0)
	{	//需要在参数返回时置该位置的值  by zjd
		ScrNum = 25;
		//重要通知提示画面   by zjd                       需要确定显示内容和方式

		int NoticePicNum = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM);
//		m_pDevCmn->fnSCR_DisplayNumeric(1, NoticePicNum);
		m_pDevCmn->fnSCR_DisplayNumeric(1, 1);
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONTINUE)
			;
		else
		if (strReturn == S_EXIT)
			fnAPP_CancelProc(T_EXIT);
		else
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);		
	}

	if (MenuIndexSave == 10)
		return T_OK;

	if (!m_pDevCmn->fnAPL_GetAvailWithdraw())
	{
		ScrNum = 20;											//无取款提示   by zjd
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONTINUE)
			;
		else
		if (strReturn == S_EXIT)
			fnAPP_CancelProc(T_EXIT);
		else
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);		
	}
	
	if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||		// 2005.04.19
		(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
		(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
		(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
	{
		ScrNum = 21;											//无收条提示   by zjd
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONTINUE)
			;
		else
		if (strReturn == S_EXIT)
			fnAPP_CancelProc(T_EXIT);
		else
		if (strReturn == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);		
	}
		

	return T_OK;
}

// Menu Display
int CTranCmn::fnAPP_MenuDisplay()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDisplay()");

	int		i;
	int		ScrNum = 0;
	int		PinInputMode = PIN_DISABLE_MODE;
	char	selaid[2];

	int		icwdmaxshutter = 0;									// added by liuxl 20110601

	memset(selaid, 0, sizeof(selaid));
	fnAPP_MenuAvailCheck(MenuIndex);
	if (MenuAvail & MENU_MCU)									// Card Enable
		fnAPD_CardEnDisable(ENABLE);
	else fnAPD_CardEnDisable(DISABLE);

	if (MenuAvail & MENU_PB)									// Passbook Enable
		fnAPD_PbEnDisable(ENABLE);
	else fnAPD_PbEnDisable(DISABLE);

	fnAPP_MenuBtnCheck(MenuIndex);								// Menu Button Check
	if (TranProc && TranCode)									// Menu Complete
		;
	else
	if ((MenuBtnCnt == 1)	&&									// 1 Button
		(MenuBtnIndex))											// Button Exist
	{
		return T_OK;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "MenuIndex[%d]", MenuIndex);
	ScrNum = MenuTblTemp[MenuIndex].MenuScrNum;
	PinInputMode = MenuTblTemp[MenuIndex].MenuPinInputMode;

	if (ScrNum == 59)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDisplay()[%d]", CurrTypes);

		if (CurrTypes & 1)
			;
		else
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);

		if (CurrTypes & 2)
			;
		else
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);

		if (CurrTypes & 4)
			;
		else
			m_pDevCmn->fnSCR_DisplayImage(4, FALSE);
	}

	if (ScrNum == 12 || ScrNum == 66)
	{
	/////////////////////////////////////////////////////////////////////////////
		if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
		{
			memset(CurrTranCanDO, '1', sizeof(CurrTranCanDO));
		}
	/////////////////////////////////////////////////////////////////////////////

	// Menu Adjust Section(Device Status)
	// Menu Adjust Section(Host Request)
	/////////////////////////////////////////////////////////////////////////////
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDisplay()[%s]", CurrTranCanDO);
		CurrTranCanDO[4] = '0';										// 存款固定不可做   by zjd
//		CurrTranCanDO[5] = '0';									// 汇款不可做   by zjd
//		CurrTranCanDO[6] = '0';									// 代缴费暂不可做    

        if(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM,_INIKEY_TRAN_ACMODE,0) == 0) // 无账户管家,无其他业务
		{   
			m_pDevCmn->fnSCR_DisplayNumeric(1, 0);
            //fnAPP_MenuClearTranCode(0, TC_ACMANAGER, MENU_NULL);
		}
		else
        if(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM,_INIKEY_TRAN_ACMODE,0) == 1) // 有账户管家,无其他业务
		{
			if ((LocalCardFlag == 1 
				|| LocalCardFlag == 5 
				|| LocalCardFlag == 7))									//本省卡开通管家婆业务
			{
				m_pDevCmn->fnSCR_DisplayNumeric(1, 1);			 
 			}
			else
			{
				m_pDevCmn->fnSCR_DisplayNumeric(1, 0);
			}
		}
		else
        if(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM,_INIKEY_TRAN_ACMODE,0) == 2) // 无账户管家,有其他业务
		{
			if (ScrNum == 12)
			{
				MenuTblTemp[3].MenuInfo[3][4] = TRAN_TRANS;
				MenuTblTemp[3].MenuInfo[3][5] = TC_OTHER;
				memset(MenuTblTemp[3].MenuKind[3], 0, 81);
				memcpy(MenuTblTemp[3].MenuKind[3], "OTHER", 5);
			}
			else 
			if (ScrNum == 66)
			{
				MenuTblTemp[9].MenuInfo[3][4] = TRAN_TRANS;
				MenuTblTemp[9].MenuInfo[3][5] = TC_OTHER;
				memset(MenuTblTemp[9].MenuKind[3], 0, 81);
				memcpy(MenuTblTemp[9].MenuKind[3], "OTHER", 5);
			}

			m_pDevCmn->fnSCR_DisplayNumeric(1, 2);
		}
//		else													//帐户管家可用，存款不可用
//		{
//			MenuTblTemp[3].MenuInfo[8][4] = TRAN_TRANS;
//			MenuTblTemp[3].MenuInfo[8][5] = TC_ACMANAGER;
//			memset(MenuTblTemp[3].MenuKind[8], 0, 81);
//			memcpy(MenuTblTemp[3].MenuKind[8], "ACMANAGER", 9);
//			m_pDevCmn->fnSCR_DisplayNumeric(1, 1);				//送显示管家婆按钮标志
// 		}
		
		if (memcmp(CurrTranCanDO, "0000000", 7) == 0)
		{
			fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);		// 没有可做交易
		}
		
		if (memcmp(&CurrTranCanDO[0],"1",1) != 0)
		{
			fnAPP_MenuClearTranCode(0, TC_INQUIRY, MENU_NULL);		// Delete Inquiry
		}
		
		if (memcmp(&CurrTranCanDO[1],"1",1) != 0)
		{
			fnAPP_MenuClearTranCode(0, TC_WITHDRAWAL, MENU_NULL);	// Delete Withdraw
		}
		
		if (memcmp(&CurrTranCanDO[2],"1",1) != 0)
		{
			fnAPP_MenuClearTranCode(0, TC_TRANSFER, MENU_NULL);		// Delete Transfer
		}
		
		if (memcmp(&CurrTranCanDO[3],"1",1) != 0)
		{
			fnAPP_MenuClearTranCode(0, TC_CHANGEPIN, MENU_NULL);	// Delete Change Pin
		}

		if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDRAWALFLAG, 1) == 0)
		{
			fnAPP_MenuClearTranCode(0, TC_WITHDRAWAL, MENU_NULL);	// Delete Withdrawal  added by liuxl 20110601
		}

		if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDRAWALFLAG1, 1) == 0)
		{
			fnAPP_MenuClearTranCode(0, TC_WITHDRAWAL, MENU_NULL);	// Delete Withdrawal
		}

		//锁定取款 added by liuxl 20110601
		icwdmaxshutter = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM, 1);
		if ( icwdmaxshutter > 0 &&
			IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, 0) == icwdmaxshutter )
		{
			fnAPP_MenuClearTranCode(0, TC_WITHDRAWAL, MENU_NULL);	// Delete Withdrawal

			// 转账锁定 added by liuxl 20111008
			if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANSFERFLAG, 0) == 0)
				fnAPP_MenuClearTranCode(0, TC_TRANSFER, MENU_NULL);		// Delete Transfer
		}

// 		if (memcmp(&CurrTranCanDO[5],"1",1) != 0)
// 		{
// 			MenuTblTemp[3].MenuInfo[3][0] = MENU_NULL;				// Delete Remit
// 			MenuTblTemp[9].MenuInfo[3][0] = MENU_NULL;				// Delete Remit
// 	//		fnAPP_MenuClearTranCode(0, TC_REMIT_ADD, MENU_NULL);	// Delete Remit
// 	//		fnAPP_MenuClearTranCode(0, TC_REMIT_IN, MENU_NULL);		// Delete Remit
// 	//		fnAPP_MenuClearTranCode(0, TC_REMIT_PWD, MENU_NULL);	// Delete Remit
// 		}
		
		if (memcmp(&CurrTranCanDO[6],"1",1) != 0)
		{
			fnAPP_MenuClearTranCode(0, TC_PAY, MENU_NULL);			// Delete Pay
 		}
		else
		{
			int			iPayDataLen1 = 0;
			int			iPayDataLen2 = 0;
			CString		PayData("");
			CString		TmpStr("");

			iPayMenuNumCZ = 0;
			iPayMenuNumJF = 0;
			memset(stPayCZ, 0, sizeof(stPayCZ));
			memset(stPayJF, 0, sizeof(stPayJF));
			iPayDataLen1 = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYFLAG1", "000"));
			iPayDataLen2 = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYFLAG2", "000"));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iPayDataLen1[%d]",iPayDataLen1);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iPayDataLen2[%d]",iPayDataLen2);
			memset(stPayMenuCZ,0,sizeof(stPayMenuCZ));			// liyi add 2009.04.09
			memset(stPayMenuJF,0,sizeof(stPayMenuJF));			// liyi add 2009.04.09

			
			if (iPayDataLen1 == 0 && iPayDataLen2 == 0)
				fnAPP_MenuClearTranCode(0, TC_PAY, MENU_NULL);		// Delete Pay
			else
			{
				if (iPayDataLen1 != 0)
				{
					PayData = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYDATA1", "");
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYDATA1[%s]",PayData);
                 	
					if (PayData.GetLength() == iPayDataLen1)
					{
						for (int j = 0; j < (iPayDataLen1 / 7); j++)
						{
							memcpy(stPayCZ[j].szPayContral, &PayData.GetBuffer(0)[j * 7 + 3], 4);
							stPayCZ[j].iPayNum = Asc2Int(&PayData.GetBuffer(0)[j * 7], 3);
						}
						int iNum = j;
						int iMin;
						int m,n;
						PayStruct stPayTemp;
						CString strTmp("");

						for (m=0; m<iNum-1; m++)
						{
							iMin = m;
							for (n=m+1; n<iNum; n++)
							{
								if (stPayCZ[iMin].iPayNum > stPayCZ[n].iPayNum)
								{
									iMin = n;
								}
							}
							memcpy(stPayTemp.szPayContral,stPayCZ[m].szPayContral,4);
							stPayTemp.iPayNum = stPayCZ[m].iPayNum;

							memcpy(stPayCZ[m].szPayContral,stPayCZ[iMin].szPayContral,4);
							stPayCZ[m].iPayNum = stPayCZ[iMin].iPayNum;

							memcpy(stPayCZ[iMin].szPayContral,stPayTemp.szPayContral,4);
							stPayCZ[iMin].iPayNum = stPayTemp.iPayNum;
						}
						PayData = "";
						for (int t=0; t<iNum; t++)
						{
							strTmp.Format("%03d%4.4s",stPayCZ[t].iPayNum,stPayCZ[t].szPayContral);	// added by liuxl 20111012
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayCZ[%d][%s]",t,strTmp);
							PayData = PayData + strTmp;
						}
					}
 					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYDATA1_FORMAT[%s]",PayData);
					memset(stPayCZ, 0, sizeof(stPayCZ));                	
					if (PayData.GetLength() == iPayDataLen1)
					{
						for (int j = 0; j < (iPayDataLen1 / 7); j++)
						{
							memcpy(stPayCZ[j].szPayContral, &PayData.GetBuffer(0)[j * 7 + 3], 4);
							stPayCZ[j].iPayNum = Asc2Int(&PayData.GetBuffer(0)[j * 7], 3);
							TmpStr = IniGetStr("C:\\T1ATM\\APP\\EXERELE\\PayList.INI", "PAYLIST",GetSprintf("PAY%03d", stPayCZ[j].iPayNum));
							memcpy(stPayCZ[j].szPayName, TmpStr.GetBuffer(0), (TmpStr.GetLength() > 10 ? 10 : TmpStr.GetLength()));		
							if (PayData.GetBuffer(0)[j * 7 + 3] == '1')
							{
								iPayMenuNumCZ ++;
								stPayMenuCZ[j].iPayNum = stPayCZ[j].iPayNum;
								memcpy(stPayMenuCZ[j].szPayName, stPayCZ[j].szPayName, sizeof(stPayCZ[j].szPayName));
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayMenuCZ[%d]iPayNum[%03d]szPayName[%10s]",j,stPayMenuCZ[j].iPayNum,stPayMenuCZ[j].szPayName);
							}
						}
					}
				}
				if (iPayDataLen2 != 0)
				{
					PayData = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYDATA2", "");
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYDATA2[%s]",PayData);
					if (PayData.GetLength() == iPayDataLen2)	// V060413 缴费项目排序
					{
						for (int j = 0; j < (iPayDataLen2 / 7); j++)
						{
							memcpy(stPayJF[j].szPayContral, &PayData.GetBuffer(0)[j * 7 + 3], 4);
							stPayJF[j].iPayNum = Asc2Int(&PayData.GetBuffer(0)[j * 7], 3);
							
						}

						int iNum = j;
						int iMin;
						int m,n;
						PayStruct stPayTemp;
						CString strTmp("");

						for (m=0; m<iNum-1; m++)
						{
							iMin = m;
							for (n=m+1; n<iNum; n++)
							{
								if (stPayJF[iMin].iPayNum > stPayJF[n].iPayNum)
								{
									iMin = n;
								}
							}
							memcpy(stPayTemp.szPayContral,stPayJF[m].szPayContral,4);
							stPayTemp.iPayNum = stPayJF[m].iPayNum;

							memcpy(stPayJF[m].szPayContral,stPayJF[iMin].szPayContral,4);
							stPayJF[m].iPayNum = stPayJF[iMin].iPayNum;

							memcpy(stPayJF[iMin].szPayContral,stPayTemp.szPayContral,4);
							stPayJF[iMin].iPayNum = stPayTemp.iPayNum;
						}
						PayData = "";
						for (int t=0; t<iNum; t++)
						{
//							strTmp.Format("%03d%s",stPayJF[t].iPayNum,stPayJF[t].szPayContral);
							strTmp.Format("%03d%4.4s",stPayJF[t].iPayNum,stPayJF[t].szPayContral);	// added by liuxl 20111012
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayJF[%d][%s]",t,strTmp);
							PayData = PayData + strTmp;
						}
					}
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYDATA2_FORMAT[%s]",PayData);
					memset(stPayJF, 0, sizeof(stPayJF));
					if (PayData.GetLength() == iPayDataLen2)
					{   
						for (int j = 0; j < (iPayDataLen2 / 7); j++)
						{
							memcpy(stPayJF[j].szPayContral, &PayData.GetBuffer(0)[j * 7 + 3], 4);
							stPayJF[j].iPayNum = Asc2Int(&PayData.GetBuffer(0)[j * 7], 3);
							TmpStr = IniGetStr("C:\\T1ATM\\APP\\EXERELE\\PayList.INI", "PAYLIST",GetSprintf("PAY%03d", stPayJF[j].iPayNum));
							memcpy(stPayJF[j].szPayName, TmpStr.GetBuffer(0), (TmpStr.GetLength() > 10 ? 10 : TmpStr.GetLength()));
							if (PayData.GetBuffer(0)[j * 7 + 3] == '1')
							{
								iPayMenuNumJF ++;
								stPayMenuJF[j].iPayNum = stPayJF[j].iPayNum;
								memcpy(stPayMenuJF[j].szPayName, stPayJF[j].szPayName, sizeof(stPayJF[j].szPayName));
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayMenuJF[%d]iPayNum[%03d]szPayName[%10s]",j,stPayMenuJF[j].iPayNum,stPayMenuJF[j].szPayName);
							}
						}
					}
				}
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iPayMenuNumCZ[%d]",iPayMenuNumCZ);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iPayMenuNumJF[%d]",iPayMenuNumJF);
				if (iPayMenuNumCZ == 0 && iPayMenuNumJF == 0)
					fnAPP_MenuClearTranCode(0, TC_PAY, MENU_NULL);	// Delete Pay
			}
		}
		

	////////////////////////////////////////////////////////////////////////////////////////////
	}
    
	if (ScrNum == 13)             //by hyl
	{   
		if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
		{
			memset(CurrTranCanDO, '1', sizeof(CurrTranCanDO));
		}
		
		if(iPayMenuNumCZ == 0)
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);
		else
		if(iPayMenuNumJF == 0)
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);		
	}

	if (ScrNum == 60 || ScrNum == 64)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDisplay()[%s]", CurrTranCanDO);
		if (ScrNum == 64)
		{
			m_pTranCmn->AccountType = "1";
			LowAmountFlag = 1;
			CurrTypeFlag = 4;
		}

		if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
		{
			memset(CurrTranCanDO, '1', sizeof(CurrTranCanDO));
		}

		if (memcmp(&CurrTranCanDO[10],"4",1) != 0)
			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);

		if (memcmp(&CurrTranCanDO[11],"4",1) != 0)
			m_pDevCmn->fnSCR_DisplayImage(3, FALSE);

		if ((memcmp(&CurrTranCanDO[8],"4",1) != 0)	&&
			(memcmp(&CurrTranCanDO[12],"4",1) != 0))
			m_pDevCmn->fnSCR_DisplayImage(2, FALSE);

		if (memcmp(&CurrTranCanDO[9],"4",1) != 0)
			m_pDevCmn->fnSCR_DisplayImage(4, FALSE);
	}
	
//	if (ScrNum == 403)             //by hyl
//	{   
//		//MAC PIN ERROR CARD RETURN
//		if (memcmp(HostResp.RejectCode, "Z5", 2) == 0 ||            //Mac/Pin error return card by hyl
//			memcmp(HostResp.RejectCode, "2Q", 2) == 0 ||
//			memcmp(HostResp.RejectCode, "2R", 2) == 0 ||
//			memcmp(HostResp.RejectCode, "ZI", 2) == 0 )	
//		{
//			m_pDevCmn->fnSCR_DisplayImage(1, FALSE);      //MAC,PIN ERROR 提示并退卡 by hyl
// 			m_pDevCmn->fnSCR_DisplayImage(2, FALSE); 
// 			Delay_Msg(1000);
// 			fnAPP_CancelProc(T_CANCEL);
//		}			
// 	}


	//20070816 add by hyl 
	if (ScrNum == 101)
	{
		 if(CardBank == 1)
		{
			m_pDevCmn->fnSCR_DisplayString(1, "1");
		}
		 m_pDevCmn->fnSCR_DisplayNumeric(2, i101Error);
	}

	for (i = 1; i < itemof(MenuTblTemp[0].MenuInfo); i++) 
	{															// Delete Button
		if ((MenuTblTemp[MenuIndex].MenuInfo[i][0] == MENU_NULL) && 
			(MenuTbl[MenuIndex].MenuInfo[i][0] != MENU_NULL))
		{
			m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
		}
	}


	

	if ((MenuIndex == 0) ||										// Main Screen
		(MenuIndex == 1) )										// Main Screen(Cancel)
	{
		switch (m_pDevCmn->AtmDefine.MachineType)
		{
			case ATM:											// Display Message
			case CDP:
			case CD:
				break;
		}
	}

	if (PbcAppTotal == 0	&&
		ScrNum == 12)
		m_pDevCmn->fnSCR_DisplayString(1, "");
	else
	if (ScrNum == 12	||
		ScrNum == 60	||
		ScrNum == 64	||
		ScrNum == 66)
		m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("当前应用 : %s", PbcAppDisp[PbcAppIndex].AppName));
	
	// Menu Adjust Section(Host Request)

	if ((PinInputMode == PIN_PASSWORD_MODE)			||			// 2005.04.19
		(PinInputMode == PIN_PASSWORD_MODE2)		||
		(PinInputMode == PIN_PASSWORD_MODE3)		||
		(PinInputMode == PIN_PASSWORD_MODE4)		||
		(PinInputMode == PIN_PASSWORD_MODE5)		||
		(PinInputMode == PIN_PASSWORD_MODE6)		||
		(PinInputMode == PIN_PASSWORD_MODE7))
			//m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PinInputMode, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_TRUE, PIN_PASSWORD_TERM);
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PinInputMode, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
	else
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PinInputMode);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDisplay():return");
	return T_OK;
}

// Menu Device Event
int CTranCmn::fnAPP_MenuDeviceEvent(int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceEvent()");

	ULONG  WaitTime = TimerSet(CheckTime);
	int	   CheckDev = DEV_NONE;
	
	if (TranProc && TranCode)									// Menu Complete
		;
	else
	if ((MenuBtnCnt == 1)	&&									// 1 Button
		(MenuBtnIndex))											// Button Exist
	{
		m_pDevCmn->DeviceTran = DEV_SCR;						// Menu Auto Choice
		return T_OK;
	}

	while (TRUE)
	{
		Delay_Msg(100);											// Performance Adjust

		CheckDev |= DEV_SCR;									// Mouse Or Touch

		if ((!CardTran)									&&		// No Card
			(DevEnDisable & DEV_MCU))
			CheckDev |= DEV_MCU;

		if ((!PbTran)									&&		// No Passbook
			(DevEnDisable & DEV_PBM))
			CheckDev |= DEV_PBM;

		if (m_pDevCmn->fnAPL_GetDeviceEvent(CheckDev))
		{
			if ((m_pDevCmn->DeviceTran & CheckDev) == m_pDevCmn->DeviceTran)
				break;
		}

		fnAPP_MenuDeviceCheck();								// Menu Device Check

		if ((!CheckTime) ||
			(CheckTimer(WaitTime)))
			fnAPP_CancelProc(T_TIMEOVER);						// TranCmn Cancel Procedure
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceEvent():return");
	return T_OK;
}

// Menu Device Procedure
int CTranCmn::fnAPP_MenuDeviceProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc()");

	CString GetKeyStr("");
	int		GetKeyID = 0;
	int		ScrNum = 0;
	CString strReturn = "";
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:m_pDevCmn->DeviceTran[%x],MenuIndex[%d]", m_pDevCmn->DeviceTran, MenuIndex);
	switch (m_pDevCmn->DeviceTran)
	{
		case DEV_MCU:
			if (!CardTran)
			{
				fnAPD_PbEnDisable(DISABLE);
				m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_OFF);
				m_pDevCmn->fnSCR_DisplayScreen(611);			// Card Reading
				if(m_pDevCmn->bAgentMode == 1)					// 删除ATMUNET流水临时文件
				{
					BOOL rc = DeleteFile("D:\\ATMS\\TEMP\\Temp.jnl");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc():DeleteFile(D:\\ATMS\\TEMP\\Temp.jnl)rc=%d",rc);
					
				}										 
				fnAPD_CardReadCheck();
/////////////////////////////////////////////////////////////////////////////
				if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||// 2010.10.20
					(m_pDevCmn->McuReadMSOnlyFlag))
					;
				else
				{
					if (!CardTran)								// IC Card Fall Back
					{
						fnAPD_CardFallBack();
//						FallBack = 1;							// added by liuxl 20110505
//						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_fall[%d]", FallBack);
						fnAPD_CardReadCheck();
					}
				}
/////////////////////////////////////////////////////////////////////////////
			}

			// Menu Adjust Section(Card)

			fnAPP_MenuClearMenuCode(0, MENU_ALL, CardTran);		// Menu Clear CardTran
			if (TranProc && TranCode)							// Menu Complete
				break;

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CardTran[%x]", CardTran);
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[1][1];
			else
			{
				if (LocalCardFlag == 3)								// added by liuxl 20110507
				{
					int nlen = 0;
					CString str;
					nlen = MakeUnPack(PbcAppDisp[PbcAppIndex].AID, str, PbcAppDisp[PbcAppIndex].AidLen);
					
					if (memcmp(str, "A000000333010101", nlen) == 0 ||
						memcmp(str, "A000000333010102", nlen) == 0)
						MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][1];
					else
						MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[2][1];
				}
				else
				if (POSTICType == 2)
					MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[2][1];
				else
					MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][1];
			}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "LocalCardFlag[%d], POSTICType[%d], MenuIndexSave[%d]", LocalCardFlag, POSTICType, MenuIndexSave);
			if ((LocalCardFlag == 3	||
				POSTICType == 2)	&&
				MenuIndexSave == 10)
				fnAPP_CancelProc(T_MSG, TranMsgTbl[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);

			if (!MenuIndexNext)
				MenuIndexNext = MenuIndex;
			TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// Tran Procedure
			TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// Tran Code
			TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// Tran Code2
			break;

		case DEV_PBM:
			if (!PbTran)
			{
				fnAPD_CardEnDisable(DISABLE);
				m_pDevCmn->fnSCR_DisplayScreen(0);				// Passbook Reading
				m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_OFF);
				fnAPD_PbReadCheck();
			}

			// Menu Adjust Section(Passbook)

			fnAPP_MenuClearMenuCode(0, MENU_ALL, PbTran);		// Menu Clear PbTran
			if (TranProc && TranCode)							// Menu Complete
				break;

			MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][2];
			if (!MenuIndexNext)
				MenuIndexNext = MenuIndex;
			TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// Tran Procedure
			TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// Tran Code
			TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// Tran Code2
			break;

		case DEV_SCR:											// Mouse or Touch
			if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 101)							// 密码输入过短按了ENTER键  by zjd
			{
				GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();

				if (GetKeyStr == "ENTER")
					i101Error ++;
				else
				if (GetKeyStr == S_CLEAR)
					i101Error = 0;
			}
			
			
			if (i101Error >= 3)
				GetKeyStr = S_INPUTOVER;
			else
			{
				if (TranProc && TranCode)							// Menu Complete
					;
				else
				if ((MenuBtnCnt == 1)	&&							// 1 Button
					(MenuBtnIndex))									// Button Exist
				{
					GetKeyID = MenuBtnIndex;
					MenuIndexSub = GetKeyID;

					MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][3];
					if (!MenuIndexNext)
						MenuIndexNext = MenuIndex;
					TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// Tran Procedure
					TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// Tran Code
					TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// Tran Code2
					break;
				}
				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 101)							// 密码输入过短按了ENTER键  by zjd
				{
					GetKeyStr = m_pDevCmn->fstrSCR_GetKeyString();
					if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 59)
					{
						LowAmountFlag = 0;
						if (GetKeyStr == "CreditAccout")
						{
							m_pTranCmn->AccountType = "4";
							CurrTypeFlag = 2;
						}
						else
						if (GetKeyStr == "SavingAccount")
						{
							m_pTranCmn->AccountType = "2";
							CurrTypeFlag = 1;
						}
						else
						if (GetKeyStr == "ICAccount")
						{
							m_pTranCmn->AccountType = "1";
							LowAmountFlag = 1;
							CurrTypeFlag = 4;
						}
						
						if (CurrTypeFlag != 4)
						{
							for(int i = 0; i < 7; i++)
							{
								if(TempTranCanDO[i] & CurrTypeFlag)
									CurrTranCanDO[i] = '1';
								else
									CurrTranCanDO[i] = '0';
							}
						}
					}
				}
			}

			GetKeyID = 0;
			if (TranProc && TranCode)							// Menu Complete
				;
			else
			if ((m_pDevCmn->fnSCR_GetCurrentScreenNo() == MenuTblTemp[MenuIndex].MenuScrNum) &&
				(GetKeyStr.GetLength()))						// Button Exist
			{
				for (int i = 1; i < itemof(MenuTblTemp[0].MenuKind); i++)
				{												// Button Find
					if (!GetKeyStr.CompareNoCase(MenuTblTemp[MenuIndex].MenuKind[i]))
						GetKeyID = i;
				}
			}
																// 2004.08.19
			if ((GetKeyID >= 1) && (GetKeyID <= 16))			// Button Value
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"			\
													"GetKeyStr[%s], GetKeyID[%d]",
													GetKeyStr, GetKeyID);
																// 2005.03.30
				MenuIndexSub = GetKeyID;
				MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][3];
				if (!MenuIndexNext)
					MenuIndexNext = MenuIndex;
				TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// Tran Proc
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// Tran Code
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// Tran Code2

				// Other Transaction Section
				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() == 101)// INPUT PIN SCREEN : 2004.11.05
				{
					if (GetKeyStr == "LANGUAGE")
					{
						if (m_pDevCmn->fnSCR_GetCurrentLangMode() == CHN_MODE)
							m_pDevCmn->fnSCR_SetCurrentLangMode(ENG_MODE);
						else
							m_pDevCmn->fnSCR_SetCurrentLangMode(CHN_MODE);
					}
				}
			}
			else 
			if (GetKeyStr == S_RETURN)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"			\
													"GetKeyStr[%s], GetKeyID[%d]",
													GetKeyStr, GetKeyID);
																// 2005.03.30
				MenuIndexNext = MenuIndexSave;					// Menu Index Save Return
				if (!MenuIndexNext)
					MenuIndexNext = MenuIndex;
				TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// Tran Proc
				TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// Tran Code
				TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// Tran Code2
			}
			else 
			if (GetKeyStr == S_EXIT)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"			\
													"GetKeyStr[%s], GetKeyID[%d]",
													GetKeyStr, GetKeyID);
																// 2005.03.30
				fnAPP_CancelProc(T_EXIT);
			}
			else 
			if (GetKeyStr == S_CANCEL)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"			\
													"GetKeyStr[%s], GetKeyID[%d]",
													GetKeyStr, GetKeyID);
																// 2005.03.30
				fnAPP_CancelProc(T_CANCEL);
			}
			else 
			if (GetKeyStr == S_TIMEOVER)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"			\
													"GetKeyStr[%s], GetKeyID[%d]",
													GetKeyStr, GetKeyID);
																// 2005.03.30
				fnAPP_CancelProc(T_TIMEOVER);
			}
			else 
			if (GetKeyStr == S_INPUTOVER)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"			\
													"GetKeyStr[%s], GetKeyID[%d]",
													GetKeyStr, GetKeyID);
																// 2005.03.30
				fnAPP_CancelProc(T_INPUTOVER);
			}
			else 
			{
				ScrNum = m_pDevCmn->fnSCR_GetCurrentScreenNo();
				if (ScrNum == 101)								// INPUT PIN SCREEN
				{
					CString	PinKeyData;
					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
					{
					    PinKeyData = m_pDevCmn->fstrPIN_GetPinKeyData();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData3[%s]", PinKeyData);
						if  (PinKeyData.GetLength() == 0)
							fnAPP_CancelProc(T_ERROR);
						else if (PinKeyData.GetLength() >= sizeof(Accept.PassWord))
							memcpy(Accept.PassWord, PinKeyData.GetBuffer(0), sizeof(Accept.PassWord));
						else
							memcpy(Accept.PassWord, PinKeyData.GetBuffer(0), PinKeyData.GetLength());
					}
					else
					{
    					PinKeyData = GetKeyStr;
						memset(Accept.PassWord, 'F', sizeof(Accept.PassWord));
						
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData[%s]", PinKeyData);
						if (PinKeyData.GetLength() >= 6)
						{
							memcpy(Accept.PassWord, "06", 2);
							memcpy(Accept.PassWord + 2, PinKeyData.GetBuffer(0), 6);
						}
						else
						{
							memcpy(Accept.PassWord, GetSprintf("02d",PinKeyData.GetLength(),2), 2);
							memcpy(Accept.PassWord + 2, PinKeyData.GetBuffer(0), PinKeyData.GetLength());
						}
					}
/*
					if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||// 2010.10.20 // 移动到插卡验密之后
						(m_pDevCmn->McuReadMSOnlyFlag))
					{
						if (LocalCardFlag == 3)								// liyi chang 2008.10.20 
						{
							while (TRUE)
							{												// 显示选择交易帐户类型画面  by zjd
								m_pDevCmn->fnSCR_DisplayScreen(15, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
								strReturn = m_pDevCmn->fstrSCR_GetKeyString();
								if (strReturn == S_TIMEOVER)
									fnAPP_CancelProc(T_TIMEOVER);
								else
								if (strReturn == S_EXIT)
									fnAPP_CancelProc(T_EXIT);
								else
								if (strReturn == S_CANCEL)
									fnAPP_CancelProc(T_CANCEL);
								else
								{
									m_pTranCmn->AccountType = strReturn;		//Save AccountType   by zjd
									break;
								}
							}
						}
					}
*/			
					MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][3];
					if (!MenuIndexNext)
						MenuIndexNext = MenuIndex;
					TranProc = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][4];	// Tran Proc
					TranCode = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][5];	// Tran Code
					TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[MenuIndexSub][6];	// Tran Code2
				}
//				else
//				// No Card Input Section
//				if (ScrNum == NNN)
//				{
//					fnAPD_CardEnDisable(DISABLE);
//					fnAPD_PbEnDisable(DISABLE);
//					
//					MenuNcTran = MENU_NC;						// No Card Setting
//					fnAPP_MenuClearMenuCode(0, MENU_ALL, MenuNcTran);
//																// Menu No Card Transaction
//					if (TranProc && TranCode)					// Menu Complete
//						break;
//
//					MenuIndexNext = MenuTblTemp[MenuIndex].MenuInfo[0][3];
//					if (!MenuIndexNext)
//						MenuIndexNext = MenuIndex;
//					TranProc = MenuTblTemp[MenuIndex].MenuInfo[0][4];	// Tran Proc
//					TranCode = MenuTblTemp[MenuIndex].MenuInfo[0][5];	// Tran Code
//					TranCode2 = MenuTblTemp[MenuIndex].MenuInfo[0][6];	// Tran Code2
//				}
				else
				{
					fnAPP_CancelProc(T_PROGRAM);
				}
			}
			break;

		default: 
			break;
	}

	fnAPP_MenuAvailCheck(MenuIndex, MenuIndexSub);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:"		\
													"MenuAvail[%x], "			\
													"MenuIndex[%d], "			\
													"MenuIndexSave[%d], "		\
													"MenuIndexSub[%d]", 
													MenuAvail, 
													MenuIndex, 
													MenuIndexSave, 
													MenuIndexSub);

	if (!MenuAvail)
	{
		if (MenuIndexSub)										// Mouse or Touch
		{
			if ((CardTran) && (MenuTbl[MenuIndex].MenuInfo[MenuIndexSub][0]))
			{													// Ivalid Transaction(Card)
				fnAPP_CancelProc(T_MSG, TranMsgTbl[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
			}
			if ((PbTran) && (MenuTbl[MenuIndex].MenuInfo[MenuIndexSub][0]))
			{													// Ivalid Transaction(Passbook)
				fnAPP_CancelProc(T_MSG, TranMsgTbl[7][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
			}
			
		}
		else													// MCU, PBM
		{
			if (CardTran)
			{													// Ivalid Transaction(Card)
				fnAPP_CancelProc(T_MSG, TranMsgTbl[6][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
			}
			if (PbTran)
			{													// Ivalid Transaction(Passbook)
				fnAPP_CancelProc(T_MSG, TranMsgTbl[7][m_pDevCmn->fnSCR_GetCurrentLangMode()]);	
			}
		}
																// Reserved Transaction
		fnAPP_CancelProc(T_MSG, TranMsgTbl[4][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	}

	fnAPP_MenuDeviceCheck();									// Menu Device Check

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc():return");
	return T_OK;
}
		
// Menu Device Check
int CTranCmn::fnAPP_MenuDeviceCheck()
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceCheck()");

//	if ((m_pDevCmn->fnAPL_GetDeviceEvent(DEV_MCU))	&&			// Two Media
//		(m_pDevCmn->fnAPL_GetDeviceEvent(DEV_PBM)))				// Ivalid Media
//		fnAPP_CancelProc(T_MSG, TranMsgTbl[5][m_pDevCmn->fnSCR_GetCurrentLangMode()]);		

//	if ((m_pDevCmn->fnMCU_GetMaterialInfo() & ST_SENSOR2)	&&	// Two Media
//		(m_pDevCmn->fnPBM_GetMaterialInfo() & ST_SENSOR2))		// Ivalid Media
//		fnAPP_CancelProc(T_MSG, TranMsgTbl[5][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceCheck():return");
	return T_OK;
}

