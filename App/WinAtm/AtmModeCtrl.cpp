/////////////////////////////////////////////////////////////////////////////
// AtmModeCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "sys/timeb.h"

#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"

#include "..\H\Dll\DevDefine.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmnDefine.h"
#include "..\H\Tran\TranCmn.h"

#include "..\H\Dll\DevEtc.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	Atm Status
/////////////////////////////////////////////////////////////////////////////
//	Mode :	ATM_INIT			: Initial		Mode
//			ATM_ERROR			: Error			Mode
//			ATM_CLERK			: Clerk			Mode
//			ATM_READY			: Ready			Mode
//			ATM_CUSTOM			: Custom		Mode 
//			ATM_TRAN			: Transaction	Mode
//			ATM_DOWN			: Down			Mode
/////////////////////////////////////////////////////////////////////////////
// Atm Mode Ctrl
int CWinAtmCtrl::AtmModeCtrl()
{
/////////////////////////////////////////////////////////////////////////////
	if (!m_pDevCmn->fnAPM_AtmModeCtrl())						// Atm Mode Ctrl
		return TRUE;
/////////////////////////////////////////////////////////////////////////////

	switch (m_pDevCmn->AtmStatus) 
	{
		case ATM_INIT:											// Initial Mode
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
// Mode Procedure
			m_pTranCmn->ErrorSendFlag = FALSE;					// 2008.11.14
			if ((GetTime() > m_pTranCmn->strRebootTime) &&	// Rebooting Check
				(GetTime() < m_pTranCmn->strRebootTimeEnd))		// 060411 重启时间调整
				m_pTranCmn->ErrorSendFlag = TRUE;
			
			m_pDevCmn->AtmStatusSave = ATM_INIT;
			m_pDevCmn->fnAPL_ClearError();						// 2004.11.05
			m_pDevCmn->fnAPL_CheckPowerOffError();
			m_pDevCmn->RecoveryType = 0;						// Auto Recovery
			m_pDevCmn->DeviceStatus = m_pDevCmn->fnAPL_GetDefineDevice(DEV_MAIN);	
			m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus), TRUE);
			
// Before Procedure
			m_pDevCmn->fnAPL_SetSensorInfo();
			m_pDevCmn->fnAPL_GetAvailTrans();
			m_pDevCmn->fnAPL_CheckDevice();
			m_pDevCmn->fnAPL_CheckMaterial();

// Mode Setting(Condition Procedure)
			if (!m_pDevCmn->fnAPL_CheckError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else												// Next Mode
			{
				m_pDevCmn->AtmStatus = ATM_CLERK;
			}

// Mode Setting(Non Condition Procedure)
			if (!m_pDevCmn->fnAPL_DisplayPrintError(TRUE))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationLed(TRUE))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationRpl(TRUE))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationInform(TRUE))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkInformation(TRUE))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkExecCheck())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!ClerkProc())
				m_pDevCmn->AtmStatus = ATM_DOWN;

// After Procedure
			m_pTranCmn->fnAPP_NmsProc(TRUE);					// Nms Procedure
			if (m_pDevCmn->ATMFirstStart == 1)
			{// 
//////////////////////////////////////////////////////////////////////////////////////////////
//  				m_pDevCmn->fnAPL_AgentCreateFile();

				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo("-------------- ATM START ---------------");
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TIME:%4.4s/%2.2s/%2.2s %2.2s:%2.2s:%2.2s", GetDate().GetBuffer(0),
													&GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
													GetTime().GetBuffer(0), &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4]));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("BANK:%9.9s  ATMID:%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("VER:%s(%s)", VERSION,m_pTranCmn->strVersion));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo("-------------- ATM 开机 ---------------");
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("时间:%4.4s/%2.2s/%2.2s %2.2s:%2.2s:%2.2s", GetDate().GetBuffer(0),
													&GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
													GetTime().GetBuffer(0), &GetTime().GetBuffer(0)[2], &GetTime().GetBuffer(0)[4]));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("银行机构号:%9.9s  ATM号:%8.8s", m_pProfile->NETWORK.BranchNum, m_pProfile->NETWORK.AtmSerialNum));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("软件版本号:%s(%s)", VERSION,m_pTranCmn->strVersion));
				}

				if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)// 060411 修改密钥丢失
				{
					m_pDevCmn->fnPIN_SetPinKeyName(TPINKEY);
					m_pDevCmn->fnPIN_SetMacKeyName(TMACKEY);
 				}
				
				if(m_pDevCmn->bAgentMode == 1)
				{
					m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_ATM_OPEN);
				}
//////////////////////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->ATMFirstStart = 0;
			}
			break;
		
		case ATM_ERROR: 										// Error Mode
// Mode Procedure
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(m_pDevCmn->AtmStatusReEntryFlag))
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
				m_pDevCmn->AtmStatusReEntryFlag = FALSE;		// Can Not ReEntry
				m_pDevCmn->fnAPL_UnUsed();
				m_pDevCmn->fnAPL_DisplayPrintError(TRUE);

				if (m_pDevCmn->AtmStatusSave != ATM_ERROR)		// 2004.07.05
				{
					if (m_pDevCmn->OpenKey)						// Only Check in Transaction Mode
					{											// 2005.02.28
						if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & DEVICE_NO_RECOVERY)
						{
							if (m_pDevCmn->AtmStatusSave == ATM_CUSTOM)
							{
//								m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [AUTO RECOVERY]",
//																			GetTime().GetBuffer(0),
//																			&(GetTime().GetBuffer(0)[2]),
//																			&(GetTime().GetBuffer(0)[4])));
								m_pDevCmn->fnAPL_ClearError();
								m_pDevCmn->RecoveryType = 0;	// Auto Recovery
								m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));
							}
							else
							if (m_pDevCmn->AtmStatusSave == ATM_TRAN)
							{
								if (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "82160", 5) == 0)
								{
//									m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [AUTO RECOVERY]",
//																				GetTime().GetBuffer(0),
//																				&(GetTime().GetBuffer(0)[2]),
//																				&(GetTime().GetBuffer(0)[4])));
									m_pDevCmn->fnAPL_ClearError();
									m_pDevCmn->RecoveryType = 0;// Auto Recovery
									m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus));
								}
							}
						}
						else									// Except Cash
						{
//							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [AUTO RECOVERY]",
//																		GetTime().GetBuffer(0),
//																		&(GetTime().GetBuffer(0)[2]),
//																		&(GetTime().GetBuffer(0)[4])));
							m_pDevCmn->fnAPL_ClearError();
							m_pDevCmn->RecoveryType = 0;		// Auto Recovery
							m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus) & (~(DEVICE_NO_RECOVERY)));
						}										// 2005.11.07
					}
				}
			}
			m_pDevCmn->AtmStatusSave = ATM_ERROR;

// Before Procedure
			m_pDevCmn->fnAPL_AutoOffDevice();					// Auto Off
			m_pDevCmn->fnAPL_SetSensorInfo();
			m_pDevCmn->fnAPL_GetAvailTrans();
			m_pDevCmn->fnAPL_CheckDevice();
			m_pDevCmn->fnAPL_CheckMaterial();
			
// Mode Setting(Condition Procedure)
			if (!m_pDevCmn->fnAPL_CheckError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->CshChangeLoc == CHANGE_POS)			&&
				(m_pDevCmn->CshLoc == NORMAL_POS)				&&
				(!m_pDevCmn->fnAPL_AutoResetCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)			&&
				(!m_pDevCmn->fnAPL_AutoReadyCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else												// Next Mode
			{
				m_pDevCmn->AtmStatus = ATM_CLERK;
			}

// Mode Setting(Non Condition Procedure)
			if (!m_pDevCmn->fnAPL_DisplayPrintError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationLed())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationIndicator())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationRpl())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationInform())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkInformation())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkExecCheck())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!ClerkProc())
				m_pDevCmn->AtmStatus = ATM_DOWN;

// After Procedure
			m_pTranCmn->fnAPP_NmsProc();						// Nms Procedure

			break;

		case ATM_CLERK: 										// Cleark Mode
// Mode Procedure
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(m_pDevCmn->AtmStatusReEntryFlag))
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
				m_pDevCmn->AtmStatusReEntryFlag = FALSE;		// Can Not ReEntry
				m_pDevCmn->fnAPL_UnUsed();
				m_pDevCmn->fnAPL_DisplayNormalSegment();
				m_pDevCmn->fnAPL_DisplayPrintError(TRUE);

//				m_pDevCmn->fnAPL_CloseHost();					// 2005.09.11 : DO NOT MODIFY
			}
			m_pDevCmn->AtmStatusSave = ATM_CLERK;

			if (!m_pTranCmn->setAtmStatus)
				m_pTranCmn->setAtmStatus = 1;

// Before Procedure
			m_pDevCmn->fnAPL_SetSensorInfo();
			m_pDevCmn->fnAPL_GetAvailTrans();
			m_pDevCmn->fnAPL_CheckDevice();
			m_pDevCmn->fnAPL_CheckMaterial();
			
// Mode Setting(Condition Procedure)
			if (!m_pDevCmn->fnAPL_CheckError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->CshChangeLoc == CHANGE_POS)			&&
				(m_pDevCmn->CshLoc == NORMAL_POS)				&&
				(!m_pDevCmn->fnAPL_AutoResetCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)			&&
				(!m_pDevCmn->fnAPL_AutoReadyCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else												// Next Mode
			{
				m_pDevCmn->AtmStatus = ATM_READY;
			}

// Mode Setting(Non Condition Procedure)
			if (!m_pDevCmn->fnAPL_DisplayPrintError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationLed())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationIndicator())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationRpl())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationInform())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkInformation())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkExecCheck())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!ClerkProc())
				m_pDevCmn->AtmStatus = ATM_DOWN;

// After Procedure
			m_pTranCmn->fnAPP_NmsProc();						// Nms Procedure
			break;

		case ATM_READY:											// Ready Mode
// Mode Procedure
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(m_pDevCmn->AtmStatusReEntryFlag))
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
				m_pDevCmn->AtmStatusReEntryFlag = FALSE;		// Can Not ReEntry
				m_pDevCmn->fnAPL_UnUsed();
				m_pDevCmn->fnAPL_DisplayNormalSegment();
				m_pDevCmn->fnAPL_DisplayPrintError(TRUE);
			}
			m_pDevCmn->AtmStatusSave = ATM_READY;
			
// Before Procedure
			m_pDevCmn->fnAPL_SetSensorInfo();
			m_pDevCmn->fnAPL_GetAvailTrans();
			m_pDevCmn->fnAPL_CheckDevice();
			m_pDevCmn->fnAPL_CheckMaterial();

			m_pTranCmn->RQKCode2 = 1;

// Mode Setting(Condition Procedure)
			if (!m_pDevCmn->fnAPL_CheckError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->CshChangeLoc == CHANGE_POS)			&&
				(m_pDevCmn->CshLoc == NORMAL_POS)				&&
				(!m_pDevCmn->fnAPL_AutoResetCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)			&&
				(!m_pDevCmn->fnAPL_AutoReadyCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else												// 2005.11.07
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_SPR))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_SPR))			&&
				((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
				 (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_JPR))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_JPR))			&&
				((m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	||
				 (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else												// 2006.08.22
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MCU))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_MCU))			&&
				(m_pDevCmn->CardMaxRetractCnt > 0)					&&
				(m_pDevCmn->CardRetractCnt >= m_pDevCmn->CardMaxRetractCnt))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_CSH))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_CSH))			&&
				((m_pDevCmn->CshLoc != NORMAL_POS)				||
				 (m_pDevCmn->RejectCSTLoc != NORMAL_POS)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->DoorStatus == DOOR_OPENED)
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->InternalTamperStatus == ABNORMAL_POS)// 2012.08.24
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TOTALKINDSET", 1))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->fnAPL_GetDownDevice((m_pDevCmn->fnAPL_GetUseDevice(DEV_MAIN) & (~(m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MAIN)))) | DEVICE_YES_CHECK))
				m_pDevCmn->AtmStatus = ATM_READY;				// 2005.11.07
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// Check Line
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pTranCmn->fnAPP_MainOpenProc())				// Open Procedure
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH)) &&
				(!m_pDevCmn->fnAPL_AutoRefillCsh(m_pDevCmn->fnCSH_GetAutoloadCondition())))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else												// Next Mode
			{
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			}

// Mode Setting(Non Condition Procedure)
			if (!m_pDevCmn->fnAPL_DisplayPrintError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationLed())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationIndicator())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationRpl())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationInform())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkInformation())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkExecCheck())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!ClerkProc())
				m_pDevCmn->AtmStatus = ATM_DOWN;

// check if Auth, if Fail Then ERROR Return  by zjd
//			if (hy_CheckAuthorithCodeFromFile3() == AUTH_FAIL)
//				m_pDevCmn->AtmStatus = ATM_ERROR;

// After Procedure
			m_pTranCmn->fnAPP_NmsProc();						// Nms Procedure
			break;

		case ATM_CUSTOM:										// Custom Mode
// Mode Procedure
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave	||
				m_pDevCmn->AtmStatusReEntryFlag)					&&
				m_pTranCmn->setAtmStatus)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
				m_pDevCmn->AtmStatusReEntryFlag = FALSE;		// Can Not ReEntry
				m_pDevCmn->fnAPL_DisplayInformationLed(TRUE);
				m_pDevCmn->fnAPL_DisplayInformationIndicator(TRUE);	

				m_pDevCmn->fnAPL_SetOnTransaction(OFF);
				m_pDevCmn->fnAPL_SetProcCount('0');
				m_pDevCmn->fnSCR_SetCurrentLangMode(CHN_MODE);
				m_pTranCmn->fnAPP_MainMenuDisplay(TRUE);
			}
			if (!m_pTranCmn->setAtmStatus)
				m_pTranCmn->setAtmStatus = 1;

			if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)// 2008.11.14
				m_pTranCmn->ErrorSendFlag = TRUE;

			m_pDevCmn->AtmStatusSave = ATM_CUSTOM;
			m_pTranCmn->SeriesTran = 0;							// added by yusy 20110526

// Before Procedure
			m_pDevCmn->fnAPL_SetSensorInfo();
			m_pDevCmn->fnAPL_GetAvailTrans();
			m_pDevCmn->fnAPL_CheckDevice();
			m_pDevCmn->fnAPL_CheckMaterial(DEV_CSH);
			
// Mode Setting(Condition Procedure)
			if (!m_pDevCmn->fnAPL_CheckError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->CshChangeLoc == CHANGE_POS)			&&
				(m_pDevCmn->CshLoc == NORMAL_POS)				&&
				(!m_pDevCmn->fnAPL_AutoResetCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)			&&
				(!m_pDevCmn->fnAPL_AutoReadyCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else												// 2005.11.07
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_SPR))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_SPR))			&&
				((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
				 (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_JPR))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_JPR))			&&
				((m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	||
				 (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else												// 2006.08.22
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MCU))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_MCU))			&&
				(m_pDevCmn->CardMaxRetractCnt > 0)					&&
				(m_pDevCmn->CardRetractCnt >= m_pDevCmn->CardMaxRetractCnt))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_CSH))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_CSH))			&&
				((m_pDevCmn->CshLoc != NORMAL_POS)				||
				 (m_pDevCmn->RejectCSTLoc != NORMAL_POS)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->DoorStatus == DOOR_OPENED)
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->InternalTamperStatus == ABNORMAL_POS)// 2012.08.24
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TOTALKINDSET", 1))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pTranCmn->fnAPP_MainMenuDeviceEvent() != T_OK)	&&
				(m_pDevCmn->fnAPL_GetDownDevice((m_pDevCmn->fnAPL_GetUseDevice(DEV_MAIN) & (~(m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MAIN)))) | DEVICE_YES_CHECK)))
				m_pDevCmn->AtmStatus = ATM_READY;				// 2005.11.07
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// Check Line : 2004.08.30
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())				// Check Open : 2004.08.30
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pTranCmn->fnAPP_MainMenuDeviceEvent() != T_OK)// Check Event
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			else
			if (!m_pDevCmn->fnAPL_AutoRecoverCheck())			// Check Recover : 2004.11.05
			{
				if (m_pDevCmn->fnAPL_AutoRecoverDevice())
					m_pDevCmn->AtmStatus = ATM_CUSTOM;
				else
					m_pDevCmn->AtmStatus = ATM_ERROR;
			}
			else												// Next Mode
			{
				m_pDevCmn->AtmStatus = ATM_TRAN;
			}

// Mode Setting(Non Condition Procedure)
			if (!m_pDevCmn->fnAPL_DisplayPrintError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationLed())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationIndicator())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationRpl())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationInform())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkInformation())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkExecCheck())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!ClerkProc())
				m_pDevCmn->AtmStatus = ATM_DOWN;

// After Procedure
			m_pTranCmn->fnAPP_NmsProc();						// Nms Procedure
			if (!m_pDevCmn->AtmStatusReEntryFlag)				// 2004.11.05
				m_pTranCmn->fnAPP_MainMenuDisplay();			// Main Menu Display
			break;

		case ATM_TRAN:											// Transaction Mode
// Mode Procedure
			if ((m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)	||
				(m_pDevCmn->AtmStatusReEntryFlag))
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
				m_pDevCmn->AtmStatusReEntryFlag = FALSE;		// Can Not ReEntry
				m_pDevCmn->TranType = 1;						// Customer Use

				m_pDevCmn->fnAPL_SetTranStart();				// Transaction Start
				m_pTranCmn->fnAPP_MainTranProc(TRAN_TRAN);		// Transaction 
				m_pDevCmn->fnAPL_SetOnTransaction(OFF);			// Transaction End
				m_pDevCmn->fnSCR_SetCurrentLangMode(CHN_MODE);
				m_pDevCmn->fnAPL_CashRetract(m_pTranCmn->CashRetractFlag);
				m_pTranCmn->CashRetractFlag = FALSE;			// 2011.01.04
			}
			m_pDevCmn->AtmStatusSave = ATM_TRAN;

// Before Procedure
			m_pDevCmn->fnAPL_SetSensorInfo();
			m_pDevCmn->fnAPL_GetAvailTrans();
			m_pDevCmn->fnAPL_CheckDevice();
			m_pDevCmn->fnAPL_CheckMaterial();

// Mode Setting(Condition Procedure)
			if (!m_pDevCmn->fnAPL_CheckError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(m_pDevCmn->DeviceStatus))		
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->CshChangeLoc == CHANGE_POS)			&&
				(m_pDevCmn->CshLoc == NORMAL_POS)				&&
				(!m_pDevCmn->fnAPL_AutoResetCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))		&&
				(m_pDevCmn->RejectCSTLoc == NORMAL_POS)			&&
				(!m_pDevCmn->fnAPL_AutoReadyCsh()))
				m_pDevCmn->AtmStatus = ATM_ERROR;
			else
			if (!m_pDevCmn->OpenKey)
				m_pDevCmn->AtmStatus = ATM_CLERK;
			else												// 2005.11.07
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_SPR))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_SPR))			&&
				((m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
				 (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_JPR))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_JPR))			&&
				((m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)	||
				 (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else												// 2006.08.22
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MCU))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_MCU))			&&
				(m_pDevCmn->CardMaxRetractCnt > 0)					&&
				(m_pDevCmn->CardRetractCnt >= m_pDevCmn->CardMaxRetractCnt))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((!m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_CSH))	&&
				(m_pDevCmn->fnAPL_GetUseDevice(DEV_CSH))			&&
				((m_pDevCmn->CshLoc != NORMAL_POS)				||
				 (m_pDevCmn->RejectCSTLoc != NORMAL_POS)))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->DoorStatus == DOOR_OPENED)
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->InternalTamperStatus == ABNORMAL_POS)// 2012.08.24
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TOTALKINDSET", 1))
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (m_pDevCmn->fnAPL_GetDownDevice((m_pDevCmn->fnAPL_GetUseDevice(DEV_MAIN) & (~(m_pDevCmn->fnAPL_GetAutoOffBankDevice(DEV_MAIN)))) | DEVICE_YES_CHECK))
				m_pDevCmn->AtmStatus = ATM_READY;				// 2005.11.07
			else
			if (!m_pDevCmn->fnAPL_CheckHostLine())				// Check Line
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if (!m_pDevCmn->fnAPL_CheckHostOpen())				// Check Open
				m_pDevCmn->AtmStatus = ATM_READY;
			else
			if ((m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))	&&
				(m_pDevCmn->fnCSH_GetAutoloadCondition() != NORMAL))
				m_pDevCmn->AtmStatus = ATM_READY;
			else												// Next Mode
			{
				m_pDevCmn->AtmStatus = ATM_CUSTOM;
			}

// Mode Setting(Non Condition Procedure)
			if (!m_pDevCmn->fnAPL_DisplayPrintError())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationLed())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_DisplayInformationIndicator())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationRpl())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_SendInformationInform())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkInformation())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClerkExecCheck())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_ClearDeviceExecOffInfo())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!m_pDevCmn->fnAPL_CheckPrintDevicePosition())
				m_pDevCmn->AtmStatus = ATM_ERROR;
			if (!ClerkProc())
				m_pDevCmn->AtmStatus = ATM_DOWN;

// After Procedure
			m_pTranCmn->fnAPP_NmsProc();						// Nms Procedure
			break;

		case ATM_DOWN:											// Down Mode
// Mode Procedure
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatus[%s]", AtmStatusName[m_pDevCmn->AtmStatus]);
			m_pDevCmn->AtmStatusSave = ATM_DOWN;

// Before Procedure
// Mode Setting(Condition Procedure)
// Mode Setting(Non Condition Procedure)

// After Procedure
			TerminateProcess();
			break;

		default:
			break;
	}

	m_pDevCmn->Op.SendRecvStatus	= "OFF";					// SendRecvStatus(OFF)
	m_pDevCmn->Op.TransationStatus	= "OFF";					// TransationStatus(OFF)
	return TRUE;
}

// Terminate Process
int	CWinAtmCtrl::TerminateProcess()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminateProcess()");

/////////////////////////////////////////////////////////////////////////////
	m_pTranCmn->HostNms.PowerOffFlag = TRUE;
/////////////////////////////////////////////////////////////////////////////
	m_pTranCmn->fnAPP_NmsProc();

/////////////////////////////////////////////////////////////////////////////
//	memcpy(&GuideMsgTbl[4][m_pDevCmn->ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
//	m_pDevCmn->fnSCR_DisplayStringArray(1, GuideMsgTbl[4][m_pDevCmn->ApDeviceLangMode]);
//	m_pDevCmn->fnSCR_DisplayScreen(702);						// Stop Service
/////////////////////////////////////////////////////////////////////////////暂停服务画面应银行要求进行更改  by zjd
	m_pDevCmn->fnSCR_DisplayScreen(27);							// Stop Service
	m_pDevCmn->fnAPL_UnUsed();
	
	if (m_pProfile->DEVICE.NmsDevice)							// 2006.07.03
	{
		m_pTranCmn->fnAPL_EndProcess();
		while (m_pTranCmn->fnAPL_GetUseStatus())
			Delay_Msg(1000);
	}
	
/////////////////////////////////////////////////////////////////////////////
	if (m_pTranCmn->HostNms.RebootFlag)							// Rebooting Check
		;														// 2004.07.08
	else
		m_pDevCmn->fnUPS_PowerOff(POWER_OFF_TIME);

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_ClerkInitModeSet();						// Clerk Initialize Mode Set
	KillTimer(TIMER_THREAD);									// Timer Stop
	KillTimer(TIMER_VFD_THREAD);								// Vfd Stop
	m_pDevCmn->fnNET_Deinitialize3(LINE_OPCL_TIME);
	m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);
	m_pDevCmn->fnAPL_UnLoadDevice();
	m_pDevCmn->AtmDoing = FALSE;								// Atm Stop

/////////////////////////////////////////////////////////////////////////////
	if (m_pTranCmn->HostNms.RebootFlag)							// Rebooting Check
		WindowsShutdownProcess(TRUE);							// Windows Reboot
	else
		WindowsShutdownProcess(FALSE);							// Windows Shutdown

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TerminateProcess():return");
	return TRUE;
}

// Windows Shutdown Process
int	CWinAtmCtrl::WindowsShutdownProcess(int nRebootFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "WindowsShutdownProcess(nRebootFlag[%d])", nRebootFlag);

	HANDLE				hToken;
	LUID				Luid;
	BOOL				bReturn = FALSE;
	DWORD				dwError;
	TOKEN_PRIVILEGES	NewPrivileges;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken))
	{
		dwError = GetLastError();
		if (dwError == ERROR_CALL_NOT_IMPLEMENTED) 
			return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &Luid)) 
	{
		CloseHandle(hToken);
		return FALSE;
	}

	NewPrivileges.PrivilegeCount			= 1;
	NewPrivileges.Privileges[0].Luid		= Luid;
	NewPrivileges.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;

	bReturn = AdjustTokenPrivileges(hToken,
									FALSE,
									&NewPrivileges,
									0,
									NULL,
									NULL);

	CloseHandle(hToken);

	if (nRebootFlag)
		::ExitWindowsEx(EWX_REBOOT, (DWORD)-1);
	else
		::ExitWindowsEx(EWX_SHUTDOWN, (DWORD)-1);

	return TRUE;
}
