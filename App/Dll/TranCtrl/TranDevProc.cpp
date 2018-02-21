/////////////////////////////////////////////////////////////////////////////
// TranDev.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "sys/timeb.h"

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
//	MEDIA FUNCTION
/////////////////////////////////////////////////////////////////////////////
// Check Device Action
int CTranCmn::fnAPD_CheckDeviceAction(int ChkID, int ErrChkFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(ChkID[%x], ErrChkFlag[%d])", ChkID, ErrChkFlag);

	ChkID &= m_pDevCmn->fnAPL_GetAvailDevice(ChkID);
	m_pDevCmn->fnAPL_CheckDeviceAction(ChkID);
	if (ErrChkFlag)
	{
		if (m_pDevCmn->fnAPL_GetAvailErrorDevice(ChkID))
		{
			if ((ChkID & DEV_CSH)	&&
				(CashCountEjrFlag))
			{
				CashCountEjrFlag = FALSE;						// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
//				if ((IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1) ||
//					(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3))		// 2008.10.13
				if (CwcInfo.CwcFlag != 0)						//	added by liuxl 20110714
					fnAPL_WriteTransactonEjr(11);				// 2005.05.26
				else
					fnAPL_WriteTransactonEjr(12);				// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
																// added by liuxl 20110715
				CString	strErrorCode("");						// 2005.05.26
				strErrorCode = m_pDevCmn->fstrCSH_GetErrorCode();
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck():[%s]return", strErrorCode);
																// Count Error
				if (strErrorCode.Left(5) == "40082")
				{
					int iTempCwdShutter = 0;
					if (IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM, 1) > 0)
					{
						iTempCwdShutter =  IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, 0);
						iTempCwdShutter ++;
						IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, iTempCwdShutter);
					}
				}
				
//				if (CashDispErrorFlag == 1)
				if (CashCountErrorFlag == 1)
					fnAPD_CwcInfoSet(1);
				fnAPP_CancelProc(T_ERROR);
			}
			
			if ((ChkID & DEV_MCU)		&&
				(m_pDevCmn->fnMCU_ICGetChipPowerStatus() == IC_POWER_HWERROR))
			{
				m_pDevCmn->Pbc.POSEntryCode = 0x02;				// Set Ic Mode
				if (m_pDevCmn->fnAPL_AutoRecoverDeviceMcu())
					fnAPD_CardFallBack();
				else
					fnAPP_CancelProc(T_ERROR);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceActionMcu(ChkID[%x], ErrChkFlag[%d])", ChkID, ErrChkFlag);
			}
			else
				fnAPP_CancelProc(T_ERROR);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CheckDeviceAction(...):return");
	return T_OK;
}

// CWC Record
int CTranCmn::fnAPD_CwcInfoSet(int TranCodeFlag)				// added by liuxl 20110714
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CwcInfoSet(TranCodeFlag[%d])", TranCodeFlag);
	
	if (!TranCodeFlag)
	{
		memset(&CwcInfo, 0, sizeof(CwcInfo));
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CwcInfoSet(...):return");
		return T_OK;
	}

	CwcInfo.CwcFlag = TranCodeFlag;
	CwcInfo.CwcRetryCnt = 0;

	memcpy(CwcInfo.TranKind, HMESend.Data[3], HMESend.DataLength[3]);
	memcpy(CwcInfo.TranDate, HMESend.Data[13], HMESend.DataLength[13]);
	memcpy(CwcInfo.TranTime, HMESend.Data[12], HMESend.DataLength[12]);
	memcpy(CwcInfo.TranSerialNo, HMESend.Data[11], HMESend.DataLength[11]);
	memcpy(CwcInfo.TranAmount, HMESend.Data[4], HMESend.DataLength[4]);
	if (HMESend.DataLength[2] != 0)
	{
		memcpy(CwcInfo.TranAccount, HMESend.Data[2]+2, HMESend.DataLength[2]-2);
		CwcInfo.TranAccountLen = HMESend.DataLength[2]-2;
	}
	if (HMESend.DataLength[59] != 0)
	{
		memcpy(CwcInfo.TranExpInfo, HMESend.Data[59]+3, HMESend.DataLength[59]-3);
		CwcInfo.TranExpInfoLen = HMESend.DataLength[59]-3;
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranKind: %6.6s", CwcInfo.TranKind);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranDate: %8.8s", CwcInfo.TranDate);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranTime: %6.6s", CwcInfo.TranTime);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranSerialNo: %8.8s", CwcInfo.TranSerialNo);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranAmount: %12.12s", CwcInfo.TranAmount);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranAccount: %s", CwcInfo.TranAccount);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranExpInfo: %s", CwcInfo.TranExpInfo);

	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||
		(m_pDevCmn->McuReadMSOnlyFlag))
		;
	else
	{
		memcpy(CwcInfo.ICSerialNo, HMESend.Data[23], HMESend.DataLength[23]);
		fnAPD_PbcReadIcDataSmp(CwcInfo.ICCwcData, &CwcInfo.ICCwcDataLen);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ICSerialNo: %3.3s", CwcInfo.ICSerialNo);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TRANFIELD55: %s", MakeUnPack(CwcInfo.ICCwcData, CwcInfo.ICCwcDataLen));
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CwcInfoSet(...):return");
	
	return T_OK;
}

int CTranCmn::fnAPD_CardRetainData(int cardRetainFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardRetainData[%d])", cardRetainFlag);
	
	if (!cardRetainFlag)
	{
		memset(&CardRetainData, 0, sizeof(CardRetainData));
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardRetainData(...):return");
		return T_OK;
	}
	
	CardRetainData.CardRetFlag = cardRetainFlag;
	CardRetainData.CardRetCnt = 0;
	
	memcpy(CardRetainData.TranDateTime, &GetDate().GetBuffer(0)[0], 8);
	memcpy(&CardRetainData.TranDateTime[8], &GetTime().GetBuffer(0)[0], 6);
	memcpy(&CardRetainData.CardRetainKind, "1", 1);
	switch (CardRetainData.CardRetFlag)
	{
		case 1:
			memcpy(CardRetainData.CardRetainInfo, "00", sizeof(CardRetainData.CardRetainInfo));
			break;
		case 2:
			memcpy(CardRetainData.CardRetainInfo, "01", sizeof(CardRetainData.CardRetainInfo));
			break;
		case 3:
			memcpy(CardRetainData.CardRetainInfo, HostResp.RejectCode, sizeof(HostResp.RejectCode));
			break;
		default:
			break;

	}

	if (strlen(CardAccountNo) <= 19)
	{
		CardRetainData.TranAccountLen = strlen(CardAccountNo);
		memcpy(CardRetainData.TranAccount, CardAccountNo, strlen(CardAccountNo));
	}
	else
	{
		CardRetainData.TranAccountLen = 19;
		memcpy(CardRetainData.TranAccount, CardAccountNo, 19);
	}

	if (FallBack)
		memcpy(&CardRetainData.TranType, "1", 1);
	else
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		memcpy(&CardRetainData.TranType, "2", 1);
	else
		memcpy(&CardRetainData.TranType, "1", 1);
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranDateTime: %14.14s", CardRetainData.TranDateTime);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CardRetainKind: %c", CardRetainData.CardRetainKind);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CardRetainInfo: %2.2s", CardRetainData.CardRetainInfo);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranAccount: %s", CardRetainData.TranAccount);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranType: %c", CardRetainData.TranType);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardRetainData(...):return");
	
	return T_OK;
}

// Card EnDisable
int CTranCmn::fnAPD_CardEnDisable(int Action, int InitFlag, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEnDisable(Action[%d], InitFlag[%d], WaitFlag[%d])", Action, InitFlag, WaitFlag);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (Action == ENABLE)
	{
		if ((InitFlag) ||
			(!(DevEnDisable & DEV_MCU)))
		{
			DevEnDisable |= DEV_MCU;
			fnAPD_CheckDeviceAction(DEV_MCU);
//			m_pDevCmn->fnMCU_ClearErrorCode();					// 2008.11.14
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU, ENABLE);
			if (WaitFlag)
				fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_ON);
		}
	}
	else
	{
		if ((InitFlag) ||
			(DevEnDisable & DEV_MCU))
		{
			DevEnDisable &= (~DEV_MCU);
			fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU, DISABLE);
			if (WaitFlag)
				fnAPD_CheckDeviceAction(DEV_MCU);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_OFF);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardEnDisable(...):return");
	return T_OK;
}

// Pb EnDisable
int CTranCmn::fnAPD_PbEnDisable(int Action, int InitFlag, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbEnDisable(Action[%d], InitFlag[%d], WaitFlag[%d])", Action, InitFlag, WaitFlag);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if (Action == ENABLE)
	{
		if ((InitFlag) ||
			(!(DevEnDisable & DEV_PBM)))
		{
			DevEnDisable |= DEV_PBM;
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnPBM_ClearErrorCode();					// 2005.09.11
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, ENABLE);
			if (WaitFlag)
				fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_ON);
		}
	}
	else
	{
		if ((InitFlag) ||
			(DevEnDisable & DEV_PBM))
		{
			DevEnDisable &= (~DEV_PBM);
			fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PBM, DISABLE);
			if (WaitFlag)
				fnAPD_CheckDeviceAction(DEV_PBM);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_OFF);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbEnDisable(...):return");
	return T_OK;
}

// Pin EnDisable
int CTranCmn::fnAPD_PinEnDisable(int Action, int InitFlag, int nPinInputMode, LPCTSTR szCardData, int nPinPassWordMin, int nPinPassWordMax, int nPinPassWordAuto, LPCTSTR szPinPassWordTerm, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PinEnDisable(Action[%d], InitFlag[%d], nPinInputMode[%d], szCardData[%s], nPinPassWordMin[%d], nPinPassWordMax[%d], nPinPassWordAuto[%d], szPinPassWordTerm[%s], WaitFlag[%d])", Action, InitFlag, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm, WaitFlag);

	if (Action == ENABLE)
	{
		if ((InitFlag) ||
			(!(DevEnDisable & DEV_PIN)))
		{
			DevEnDisable |= DEV_PIN;
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm, WaitFlag);
		}
	}
	else
	{
		if ((InitFlag) ||
			(DevEnDisable & DEV_PIN))
		{
			DevEnDisable &= (~DEV_PIN);
			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PIN, DISABLE, nPinInputMode, szCardData, nPinPassWordMin, nPinPassWordMax, nPinPassWordAuto, szPinPassWordTerm, WaitFlag);
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PinEnDisable(...):return");
	return T_OK;
}

// Card Read Check
int CTranCmn::fnAPD_CardReadCheck(LPCTSTR strInCardData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck()");

	CString			strCardData("");
	CStringArray	strCardDataArray;
	CString			strReturn("");
	int				i = 0, Result = 0;
	int				nPbcTempIndex = 0;
	int				nCurrIndex = 0, nPageIndex = 0;
	char			szBuff[256];
	int				nResult;

	if (!strInCardData)											// No Data
	{
		if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
			return T_OK;
	}

	do 
	{
		CardTran = MENU_NULL;									// Card Transaction
		CardBank = 0;											// Card Bank
		i101Error = 0;
		CardService = 0;										// Card Service
		memset(&CardData, 0, sizeof(CardData));					// Card Data 
		memset(CardAccountNo, 0, sizeof(CardAccountNo));		// Card Account No
		memset(CardPan, 0, sizeof(CardPan));					// Card Pan
		strCardDataArray.RemoveAll();

		if (!strInCardData)										// No Data
		{
/////////////////////////////////////////////////////////////////////////////
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
			{
				fnAPD_CheckDeviceAction(DEV_MCU);
				m_pDevCmn->fnMCU_Read();
				fnAPD_CheckDeviceAction(DEV_MCU);
				strCardData = m_pDevCmn->fstrMCU_GetCardData();
			}
			else
			{
// 				m_pDevCmn->Pbc.POSEntryCode = 0x02;				// Set Ic Mode
				fnAPD_CheckDeviceAction(DEV_MCU);
				m_pDevCmn->fnMCU_Read();
				fnAPD_CheckDeviceAction(DEV_MCU);
//				if (m_pDevCmn->Pbc.POSEntryCode == 0x02)
//				{
				if (m_pDevCmn->Pbc.POSEntryCode == 0x92)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "### enter 0x92 ###");
					m_pDevCmn->fnMCU_Read();
					strCardData = m_pDevCmn->fstrMCU_GetCardData();
				}
				else
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "### enter normal ###");
					strCardData = m_pDevCmn->fstrMCU_GetICData();
					if ((strCardData.GetLength() >= 2)		&&		// IC Chip Power Ok
						((strCardData.Left(2) == "3B")	||
						 (strCardData.Left(2) == "3F")))
						;
					else											// IC Chip Power Fail
						return T_OK;

					m_pDevCmn->Pbc.POSEntryCode = 0x02;				// Set Ic Mode
					fnAPD_PbcInitialData();							// Pboc Initial Data
					
					if (fnAPD_PbcSelApp(SEL_FOR_TRAN, 0) != T_OK)	// Pboc Sel App
						return T_OK;

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcSelApp Len[%d]", PbcAppTotal);
					PbcAppIndex = 0;								// Pboc Tran AppIndex Set : 2010.10.20

					while (TRUE)
					{												// 显示选择应用类型
						if ((PbcAppDisp[0].Priority & 0x80) == FALSE && PbcAppTotal == 1)
							;
						else
						if (PbcAppDispTotal > 0)
						{
							if (PbcAppDispTotal <= 6)
								nPageIndex = 0;
							if (nPageIndex == 0)
							{
								nCurrIndex = 1;
								for (int i = 1; i < 7; i++)
								{
									if (PbcAppDispTotal >= nCurrIndex)
										m_pDevCmn->fnSCR_DisplayString(i, GetSprintf("%s", PbcAppDisp[nCurrIndex-1].AppName));
									else
										m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
									nCurrIndex++;
								}
								if (PbcAppDispTotal <= 6)
									m_pDevCmn->fnSCR_DisplayImage(8, FALSE);
								m_pDevCmn->fnSCR_DisplayScreen(61, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
							}
							else
							{
								nCurrIndex = 7;
								for (int i = 1; i < 7; i++)
								{
									if (PbcAppDispTotal >= nCurrIndex)
										m_pDevCmn->fnSCR_DisplayString(i, GetSprintf("%s", PbcAppDisp[nCurrIndex-1].AppName));
									else
										m_pDevCmn->fnSCR_DisplayImage(i, FALSE);
									nCurrIndex++;
								}
								m_pDevCmn->fnSCR_DisplayScreen(70, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
							}
							
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
							if (strReturn == S_NEXT)
							{
								nPageIndex = 1;
								continue;
							}
							else
							if (strReturn == S_Previous)
							{
								nPageIndex = 0;
								continue;
							}
							else
							{
								nPbcTempIndex = atoi(strReturn);
								if (nPbcTempIndex > PbcAppDispTotal)
									continue;

								for (int j = 0; j < PbcAppDispTotal; j++)
								{
									if((PbcAppDisp[nPbcTempIndex].AidLen == PbcAppList[j].AidLen) &&
										(memcmp(PbcAppDisp[nPbcTempIndex].AID, PbcAppList[j].AID, PbcAppDisp[nPbcTempIndex].AidLen) == 0))
									{
										PbcAppIndex = j;
									}
								}
							}
							
							m_pDevCmn->fnSCR_DisplayScreen(711);
						}

						nResult = fnAPD_PbcFinalApp(SEL_FOR_TRAN);
						if (nResult != T_OK)	// Pboc Final App
						{
							if (nResult == T_CANCEL)
							{
								// Pboc Make App Display
								if (fnAPD_PbcSelApp(SEL_FOR_TRAN, 1) != T_OK)
									return T_OK;
								m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[26][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
								m_pDevCmn->fnSCR_DisplayScreen(702);
								Delay_Msg(1000);
							}
							else
								return T_OK;
						}
						else
							break;
					}

	// 				if (fnAPD_PbcReadPan() != T_OK)					// Pboc Read Pan
	// 					return T_OK;

					if (fnAPD_PbcReadApp() != T_OK)
					{
						if (FallBack)
							return T_OK;
						else
							fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
					}

					strCardData = fstrAPD_PbcMakeCardData();		// Pboc Make Card Data
					CardTran |= MENU_IC;	
				}
										// IC CARD
			}
/////////////////////////////////////////////////////////////////////////////
		}
		else
		{	
			strCardData = strInCardData;
		}
		SplitString(strCardData, ",", strCardDataArray);

		if (strCardDataArray.GetSize() >= 1)
		{
			if (strCardDataArray[0].GetLength())
			{
				CardData.TrInfo |= ISO1_TRACK;
				CardData.ISO1size = __min(sizeof(CardData.ISO1Buff) * 2, strCardDataArray[0].GetLength()) / 2;
				MakePack(strCardDataArray[0].GetBuffer(0), CardData.ISO1Buff, __min(sizeof(CardData.ISO1Buff) * 2, strCardDataArray[0].GetLength()));
			}
		}
		if (strCardDataArray.GetSize() >= 2)
		{
			if (strCardDataArray[1].GetLength())
			{
				CardData.TrInfo |= ISO2_TRACK;
				CardData.ISO2size = __min(sizeof(CardData.ISO2Buff) * 2, strCardDataArray[1].GetLength()) / 2;
				MakePack(strCardDataArray[1].GetBuffer(0), CardData.ISO2Buff, __min(sizeof(CardData.ISO2Buff) * 2, strCardDataArray[1].GetLength()));
			}
		}
		if (strCardDataArray.GetSize() >= 3)
		{
			if (strCardDataArray[2] != "")
			{
				CardData.TrInfo |= ISO3_TRACK;
				CardData.ISO3size = __min(sizeof(CardData.ISO3Buff) * 2, strCardDataArray[2].GetLength()) / 2;
				MakePack(strCardDataArray[2].GetBuffer(0), CardData.ISO3Buff, __min(sizeof(CardData.ISO3Buff) * 2, strCardDataArray[2].GetLength()));
			}
		}
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:CardData", &CardData, sizeof(CardData));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:TrInfo[%d]", CardData.TrInfo);
		if (!strInCardData)										// No Data
		{
			if (!(CardData.TrInfo & (ISO2_TRACK | ISO3_TRACK)))
				break;
		}
		else
		{
			if (!(CardData.TrInfo & (ISO2_TRACK | ISO3_TRACK)))
				break;
		}

		if (CardData.TrInfo & ISO3_TRACK)						// ISO3 Track 
		{
			if ((CardData.ISO3size < 7)								||
				(CardData.ISO3size > sizeof(CardData.ISO3Buff))		||
				(FindChar(CardData.ISO3Buff, CardData.ISO3size, NULL) != CardData.ISO3size)
																	||
				(!IsUnPack(CardData.ISO3Buff, CardData.ISO3size)))
//				(!IsNum(pCardData3->BankNo, 3))						||
//				(IsZero(pCardData3->BankNo, 3))						||
//				(IsZero(pCardData3->AccountNo, 16))					||
//				(IsZero(CardData.ISO3Buff, 7))						||
//				(IsChar(CardData.ISO3Buff, 7, '=')))
				;
			else
			{													
				CardTran |= MENU_MCU3;							// CARD(3TR)
			}
		}

		if (CardData.TrInfo & ISO2_TRACK)						// ISO2 Track
		{			
			if ((CardData.ISO2size < 2)								||
				(CardData.ISO2size > sizeof(CardData.ISO2Buff))		||
				(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size)
																	||
				(!IsUnPack(CardData.ISO2Buff, CardData.ISO2size)))
//				(!IsNum(CardData.ISO2Buff, 2))						||
//				(IsZero(CardData.ISO2Buff, 2))						||
//				(IsChar(CardData.ISO2Buff, 2, '=')))
				;				
			else
			{			
				CardTran |= MENU_MCU2;							// CARD(2TR)
			}
		}


		if (CardTran & MENU_MCU)
		{
			memset(szBuff, 0, sizeof(szBuff));
//			nResult = fnAPL_GetCardAvailData((char*)&CardData.ISO2Buff, (char*)&CardData.ISO3Buff, szBuff, &CardBank);
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
			{
				nResult = fnAPL_GetPOSTCardAvailData((char*)&CardData.ISO2Buff, (char*)&CardData.ISO3Buff, szBuff, &CardBank);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:szBuff[%s], CardBank[%d], nResult[%d]", szBuff, CardBank, nResult);
			}
			else
			{
				nResult = fnAPL_GetICCardAvailData((char*)&CardData.ISO2Buff, szBuff, &CardBank);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:szBuff[%s], CardBank[%d], nResult[%d]", szBuff, CardBank, nResult);
			}

			if ((nResult == T_OK) &&							// Find Ok
				(CardBank == 1))								// Mother Card
			{
				if ((CardTran & MENU_MCU23) == MENU_MCU23)		// 2005.03.16
					CardTran |= MENU_MCUM2M3;
				else
				if ((CardTran & MENU_MCU2) == MENU_MCU2)
					CardTran |= MENU_M2;
				else
				if ((CardTran & MENU_MCU3) == MENU_MCU3)
					CardTran |= MENU_M3;
				else
					CardTran |= MENU_NULL;

				for (i = 0; i < 19; i++)						// Make Card Account No
				{
					if (IsNum(&szBuff[i], 1))
						CardAccountNo[i] = szBuff[i];
					else
						break;
				}

				Result = FindNotNum(szBuff, 19);				// Make Card Pan
				if (Result <= 12)
				{
					memset(&CardPan[0], '0', 1);
					memcpy(&CardPan[1], szBuff, 11);
				}
				else
				if (Result <= 18)
					memcpy(&CardPan[0], &szBuff[Result - 13], 12);
				else 
					memcpy(&CardPan[0], &szBuff[6], 12);
			}
			else 
			{
				if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	// Host Local Mode
				{
					CardBank = 1;
					if ((CardTran & MENU_MCU23) == MENU_MCU23)	// 2005.03.16
						CardTran |= MENU_MCUM2M3;
					else
					if ((CardTran & MENU_MCU2) == MENU_MCU2)
						CardTran |= MENU_M2;
					else
					if ((CardTran & MENU_MCU3) == MENU_MCU3)
						CardTran |= MENU_M3;
					else
						CardTran |= MENU_NULL;
				}
				else
				{
					CardBank = 0;
					if ((CardTran & MENU_MCU23) == MENU_MCU23)	// 2005.03.16
						CardTran |= MENU_MCUT2T3;
					else
					if ((CardTran & MENU_MCU2) == MENU_MCU2)
						CardTran |= MENU_T2;
					else
					if ((CardTran & MENU_MCU3) == MENU_MCU3)
						CardTran |= MENU_T3;
					else
						CardTran |= MENU_NULL;
				}

				if (CardData.TrInfo & ISO2_TRACK)				// ISO2 Track
				{
					for (i = 0; i < 19; i++)					// Make Card Account No
					{
						if (IsNum(&CardData.ISO2Buff[i], 1))
						{
							CardAccountNo[i] = CardData.ISO2Buff[i];
						}
						else
							break;
					}
					Result = FindNotNum(CardData.ISO2Buff, 19);	// Make Card Pan
					if (Result <= 12)
					{
						memset(&CardPan[0], '0', 1);
						memcpy(&CardPan[1], CardData.ISO2Buff, 11);
					}
					else
					if (Result <= 18)
						memcpy(&CardPan[0], &CardData.ISO2Buff[Result - 13], 12);
					else 
						memcpy(&CardPan[0], &CardData.ISO2Buff[6], 12);
				}
				
				if (CardData.TrInfo & ISO3_TRACK && 			// ISO3 Track
					strlen(CardAccountNo) <= 10)				// 060411 二磁道错误
				{
					for (int i = 0; i < 19; i++)				// Make Card Account No
					{
						if (IsNum(&CardData.ISO3Buff[i + 2], 1))
						{
							CardAccountNo[i] = CardData.ISO3Buff[i + 2];
						}
						else
							break;
					}
					Result = FindNotNum(&CardData.ISO3Buff[2], 19);				// Make Card Pan
					if (Result <= 12)
					{
						memset(&CardPan[0], '0', 1);
						memcpy(&CardPan[1], &CardData.ISO3Buff[2], 11);
					}
					else
					if (Result <= 18)
						memcpy(&CardPan[0], &CardData.ISO3Buff[2 + Result - 13], 12);
					else 
						memcpy(&CardPan[0], &CardData.ISO3Buff[2 + Result - 13], 12);
				}

/*//20070802
									for (i = 0; i < 19; i++)						// Make Card Account No
									{
										if (IsNum(&szBuff[i], 1))
											CardAccountNo[i] = szBuff[i];
										else
											break;
									}
					
									Result = FindNotNum(szBuff, 19);				// Make Card Pan
									if (Result <= 12)
									{
										memset(&CardPan[0], '0', 1);
										memcpy(&CardPan[1], szBuff, 11);
									}
									else
									if (Result <= 18)
										memcpy(&CardPan[0], &szBuff[Result - 13], 12);
									else 
										memcpy(&CardPan[0], &szBuff[6], 12);*/
					
			}
/////////////////////////////////////////////////////////////////////////////
		if (m_pDevCmn->AtmDefine.MwDevice == MW_KAL)			// 2006.03.21 //晓星代码拷贝,判别EPP的SP类型 by hyl
			CardPan[12] = '0';									// Kal Bug : 2004.06.30
/////////////////////////////////////////////////////////////////////////////
		if (strlen(CardAccountNo) <= 10)						// 2005.09.11
			CardTran = MENU_NULL;								// 2005.08.22

/*    // 当无三磁时为不可用,根据邮储需求无二磁有三磁时卡仍为可用 by hyl             
					if (CardTran & MENU_MCU2)								// CARD(2TR)
						;
					else
						CardTran = MENU_NULL;								// 2005.09.11*/
			
		}
	} while(FALSE);
	
/////////////////////////////////////////////////////////////////////////////
	fnAPL_DvrSendDataProc("CI");								// 2005.06.15
/////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////////
	transInqCnt = 0;											// added by yusy
	if (LangMode)
	{
		m_pDevCmn->fnAPL_PrintJnlInfoDirect(GetSprintf("DATE:%4.4s/%2.2s/%2.2s ATMID:%8.8s",
													GetDate().GetBuffer(0), &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
													m_pProfile->NETWORK.AtmSerialNum));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ==> %s>",
			GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfoDirect(GetSprintf("日期:%4.4s/%2.2s/%2.2s ATM号:%8.8s",
													GetDate().GetBuffer(0), &GetDate().GetBuffer(0)[4], &GetDate().GetBuffer(0)[6],
													m_pProfile->NETWORK.AtmSerialNum));
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 插卡 卡号:%s>",
			GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
	}

/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck:"
													"CardTran[%x], "		\
													"CardBank[%d], "		\
													"CardService[%d], "		\
													"CardAccountNo[%s], "	\
													"CardPan[%s], ",		\
													CardTran,
													CardBank,
													CardService,
													CardAccountNo,
													CardPan);

/////////////////////////////////////////////////////////////////////////////
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCARD, 
				MakeUnPack(&CardTran, sizeof(CardTran))				+	","		+		
				MakeUnPack(&CardBank, sizeof(CardBank))				+	","		+
				MakeUnPack(&CardService, sizeof(CardService))		+	","		+
				MakeUnPack(&CardData, sizeof(CardData))				+	","		+
				MakeUnPack(CardAccountNo, sizeof(CardAccountNo))	+	","		+
				MakeUnPack(CardPan, sizeof(CardPan)));
/////////////////////////////////////////////////////////////////////////////

	if (!(CardTran & (~MENU_IC)))								// 2010.10.20
	{
		if (!strInCardData)										// No Data
		{
			if (strlen(CardAccountNo))							// 2005.09.11
				fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			else
				fnAPP_CancelProc(T_MSG, TranMsgTbl[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
		}
		else
			fnAPP_CancelProc(T_MSG, TranMsgTbl[2][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	}
		
/////////////////////////////////////////////////////////////////////////////
	// Capture Order : Face, Hand
	if (m_pDevCmn->TranStatus == TRAN_TRAN)				// Transaction
	{													// Delete & Inc & Capture
//				m_pDevCmn->fnAPL_CaptureFaceSave(1, TRUE, TRUE);// 2005.11.11
//				m_pDevCmn->fnAPL_CaptureHandSave(2);			// Speed Up
		hy_HCMonitor2_Register();								// 为了预防MONITOR2在AP后启动，增加此处授权，以免监控授权失败  by zjd
		if (m_pDevCmn->iMonitorCS_Custom != -1)
		{
			hy_HCMonitor2_CustomerServiceEnd(m_pDevCmn->iMonitorCS_Custom);
			m_pDevCmn->iMonitorCS_Custom = -1;
		}

        //Dvr Send by hyl
		if(InitInfo.TranDvrMode == 1)
		     fnAPP_DvrDataComm(CMDCODE_INCARD,DVRCHAN_IDX12,5,"INSERCARD");

		m_pDevCmn->iMonitorCS_Custom = hy_HCMonitor2_CustomerServiceBegin();
		hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 5,
			m_pDevCmn->iMonitorCS_Custom, CardAccountNo, "INSERT CARD", "NO");
		memset(CardAccNoForMonitor, 0, sizeof(CardAccNoForMonitor));
		memcpy(CardAccNoForMonitor, CardAccountNo, strlen(CardAccountNo));
	}
/////////////////////////////////////////////////////////////////////////////

	if (!CardTran)	
	{
		if (!strInCardData)										// No Data
		{
			if (strlen(CardAccountNo) < 13)							// 2005.09.11
//				fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				fnAPP_CancelProc(T_MSG, "", 416);
			else
				fnAPP_CancelProc(T_MSG, TranMsgTbl[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
		}
		else
			fnAPP_CancelProc(T_MSG, TranMsgTbl[2][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	}
	else
	if (strlen(CardAccountNo) < 13)							// 2005.09.11
		fnAPP_CancelProc(T_MSG, "", 416);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardReadCheck():return");
	return T_OK;
}

// Card Ic Check
int CTranCmn::fnAPD_CardIcCheck(LPCTSTR strInCardData)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardIcCheck()");

	CString			strCardData("");
	CStringArray	strCardDataArray;
	int				i = 0, Result = 0;

	do															// 2010.10.20
	{
		memset(&CardData, 0, sizeof(CardData));					// Card Data 
		strCardDataArray.RemoveAll();

		strCardData = strInCardData;
		SplitString(strCardData, ",", strCardDataArray);

		if (strCardDataArray.GetSize() >= 2)
		{
			if (strCardDataArray[1].GetLength())
			{
				CardData.TrInfo |= ISO2_TRACK;
				CardData.ISO2size = __min(sizeof(CardData.ISO2Buff) * 2, strCardDataArray[1].GetLength()) / 2;
				MakePack(strCardDataArray[1].GetBuffer(0), CardData.ISO2Buff, __min(sizeof(CardData.ISO2Buff) * 2, strCardDataArray[1].GetLength()));
			}
		}
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardIcCheck:CardData", &CardData, sizeof(CardData));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardIcCheck:TrInfo[%d]", CardData.TrInfo);
		if (!(CardData.TrInfo & (ISO2_TRACK | ISO3_TRACK)))
			break;

		if (CardData.TrInfo & ISO2_TRACK)						// ISO2 Track
		{			
			if ((CardData.ISO2size < 2)								||
				(CardData.ISO2size > sizeof(CardData.ISO2Buff))		||
				(FindChar(CardData.ISO2Buff, CardData.ISO2size, NULL) != CardData.ISO2size)
																	||
				(!IsUnPack(CardData.ISO2Buff, CardData.ISO2size)))
//				(!IsNum(CardData.ISO2Buff, 2))						||
//				(IsZero(CardData.ISO2Buff, 2))						||
//				(IsChar(CardData.ISO2Buff, 2, '=')))
			{
				CardData.TrInfo &= (~ISO2_TRACK);				// 2010.10.20
			}
		}

	} while(FALSE);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardIcCheck:"
													"CardTran[%x], "		\
													"CardBank[%d], "		\
													"CardService[%d], "		\
													"CardAccountNo[%s], "	\
													"CardPan[%s], ",		\
													CardTran,
													CardBank,
													CardService,
													CardAccountNo,
													CardPan);

/////////////////////////////////////////////////////////////////////////////
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCARD, 
				MakeUnPack(&CardTran, sizeof(CardTran))				+	","		+		
				MakeUnPack(&CardBank, sizeof(CardBank))				+	","		+
				MakeUnPack(&CardService, sizeof(CardService))		+	","		+
				MakeUnPack(&CardData, sizeof(CardData))				+	","		+
				MakeUnPack(CardAccountNo, sizeof(CardAccountNo))	+	","		+
				MakeUnPack(CardPan, sizeof(CardPan)));
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardIcCheck():return");
	return T_OK;
}

// Card Fall Back
int	CTranCmn::fnAPD_CardFallBack()
{																// 2010.10.20
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardFallBack()");

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->McuReadMSOnlyFlag = TRUE;						// Read MS Only Setting
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->fnMCU_ICGetChipPowerStatus() == IC_POWER_ONLINE)		||// Get IC Chip Power Status
		(m_pDevCmn->fnMCU_ICGetChipPowerStatus() == IC_POWER_HWERROR))
		m_pDevCmn->fnMCU_ICChipPower(IC_ACT_POWER_OFF);			// IC Chip Power Off
	
	if (m_pDevCmn->Pbc.POSEntryCode == 0x02)
	{
		m_pDevCmn->Pbc.POSEntryCode = 0x92;						// Set Fall Back Mode
	}

	fnAPD_CheckDeviceAction(DEV_MCU);
	m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU, ENABLE);
	
	ULONG	WaitTime = TimerSet(K_30_WAIT);						// Mcu Wait Event 
	while (TRUE)
	{
		if (m_pDevCmn->fnAPL_GetDeviceEvent(DEV_MCU))
			break;
			
		if (CheckTimer(WaitTime))
			break;

		Delay_Msg(100);											// Performance Adjust
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardFallBack():return");
	return T_OK;
}

// Pb Read Check
int CTranCmn::fnAPD_PbReadCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck()");

	CString	strPbData("");

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	do 
	{
		PbTran = MENU_NULL;										// Pb Transaction
		memset(&PbMsData, 0, sizeof(PbMsData));					// Pb Ms Data

		fnAPD_CheckDeviceAction(DEV_PBM);
		m_pDevCmn->fnPBM_Read();
		fnAPD_CheckDeviceAction(DEV_PBM);
		strPbData = m_pDevCmn->fstrPBM_GetMsData();

		PbMsData.Len = __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()) / 2;
		MakePack(strPbData.GetBuffer(0), PbMsData.sBuf, __min(sizeof(PbMsData.sBuf) * 2, strPbData.GetLength()));
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck:PbMsData", &PbMsData, sizeof(PbMsData));

		if ((PbMsData.Len >= 1)	&&
		    (PbMsData.sBuf[0] == 'S'))							// Read Fail
			break;

		if ((PbMsData.Len < 2)								||
			(PbMsData.Len > sizeof(PbMsData.sBuf))			||
			(FindChar(PbMsData.sBuf, PbMsData.Len, NULL) != PbMsData.Len)
															||
			(!IsNum(pPbMsData->AccountNo, sizeof(pPbMsData->AccountNo))))
//			(!IsNum(PbMsData.sBuf, 2))						||
//			(IsZero(PbMsData.sBuf, 2))						||
//			(IsChar(PbMsData.sBuf, 2, '=')))
			;				
		else
		{			
			PbTran |= MENU_PB;									// PASSBOOK
		}
	} while(FALSE);
	
/////////////////////////////////////////////////////////////////////////////
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCARD, 
				MakeUnPack(&PbTran, sizeof(PbTran))					+	","		+
				MakeUnPack(&PbMsData, sizeof(PbMsData)));
/////////////////////////////////////////////////////////////////////////////

	if (!PbTran)	
	{
		fnAPP_CancelProc(T_MSG, TranMsgTbl[3][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbReadCheck():return");
	return T_OK;
}

// Card Write
int CTranCmn::fnAPD_CardWrite(MCAP* WriteCardData, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite(WriteCardData, WaitFlag[%d])", WaitFlag);
HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite:WriteCardData", WriteCardData, sizeof(MCAP));

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		return T_OK;

	if (!CardTran)
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_MCU);
	m_pDevCmn->fnMCU_Write(MakeUnPack(WriteCardData->ISO3Buff, WriteCardData->ISO3size));

	if (WaitFlag)
		fnAPD_CheckDeviceAction(DEV_MCU);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_CardWrite(...):return");
	return T_OK;
}

// Pb Print
int	CTranCmn::fnAPD_PbPrint(int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbPrint(WaitFlag[%d])", WaitFlag);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		return T_OK;

	if (!PbTran)
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_PBM);
	m_pDevCmn->fnPBM_Print(MakeUnPack(PbData, PbDataCnt));

	if (WaitFlag)
		fnAPD_CheckDeviceAction(DEV_PBM);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbPrint(...):return");
	return T_OK;
}

// Elec journal Index Set
int	CTranCmn::fnAPD_EjrIndexSet()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EjrIndexSet()");

	EjrIndexFlag = TRUE;										// Elec journal Index Flag
	memset(&EjrData, ' ', sizeof(EjrData));						// Elec journal Data

	memcpy(EjrData.Date, m_pProfile->TRANS.YYYYMMDD, 8);		// Date
	memcpy(EjrData.Time, GetTime().GetBuffer(0), 6);			// Time
	if (m_pDevCmn->TranStatus == TRAN_TRAN)						// AccountNum
		memcpy(EjrData.AccountNum, fstrAPP_EditAcnt(FALSE), strlen(fstrAPP_EditAcnt(FALSE)));
	memcpy(EjrData.TranSerialNo, m_pProfile->TRANS.SerialNo, 8);// TranSerialNo
	memcpy(EjrData.TranAmount, Accept.Money, 12);				// TranAmount
	memcpy(EjrData.KeyNum, m_pProfile->TRANS.SerialNo, 8);		// KeyNum
	if (CardTran)												// TranMaterial
		memcpy(EjrData.TranMaterial, "CARD", 4);
	else
	if (PbTran)
		memcpy(EjrData.TranMaterial, "PB", 2);
	else
		memcpy(EjrData.TranMaterial, "NO CARD", 7);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_EjrIndexSet():return");
	return T_OK;
}

// Shu Print
int	CTranCmn::fnAPD_ShuPrint(int PrintDevice, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ShuPrint(PrintDevice[%x], WaitFlag[%d])", PrintDevice, WaitFlag);
	
	

	int		i, js = 0, jj =0, Len;

	if ((PrintDevice & (DEV_SPR | DEV_JPR)) == (DEV_SPR | DEV_JPR))
	{
		if ((m_pDevCmn->TranStatus == TRAN_TOTAL)		||		// No Screen
			(m_pDevCmn->TranStatus == TRAN_TOTALPRE)		||
			(m_pDevCmn->TranStatus == TRAN_TOTALKIND)		||
			(m_pDevCmn->TranStatus == TRAN_CARDRETAIN)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)||
			(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT)||
			(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||		// 2004.07.05
			(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
			(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
			(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
			(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||		// added by yusy ic
			(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNPBOCPARA)||
			(m_pDevCmn->TranStatus == TRAN_DOWNPARM)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNPAY)		||
			(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
			(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
			(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
			(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||		// 2006.07.03
			(m_pDevCmn->TranStatus == TRAN_DCEJREND)	||
			(m_pDevCmn->fnSCR_GetCurrentScreenNo() == 403))
			;
		else
		if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||	// added by liuxl 20110926
			(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
			(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
			(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
			;
		else
			m_pDevCmn->fnSCR_DisplayScreen(612);				// Spr & Jpr Print Screen
	}
	else
	if (PrintDevice & DEV_SPR)
	{
		if ((m_pDevCmn->TranStatus == TRAN_TOTAL)		||		// No Screen
			(m_pDevCmn->TranStatus == TRAN_TOTALPRE)		||
			(m_pDevCmn->TranStatus == TRAN_TOTALKIND)		||
			(m_pDevCmn->TranStatus == TRAN_CARDRETAIN)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)||
			(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT)||
			(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||		// 2004.07.05
			(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
			(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
			(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
			(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||		// added by yusy ic
			(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNPBOCPARA)||
			(m_pDevCmn->TranStatus == TRAN_DOWNPARM)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNPAY)		||
			(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
			(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
			(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
			(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||		// 2006.07.03
			(m_pDevCmn->TranStatus == TRAN_DCEJREND)	||
			(m_pDevCmn->fnSCR_GetCurrentScreenNo() == 403))
			;
		else
		if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||	// added by liuxl 20110926
			(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
			(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
			(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
			;
		else
			m_pDevCmn->fnSCR_DisplayScreen(612);				// Spr Print Screen
	}
	else
	if (PrintDevice & DEV_JPR)
		;														// Jpr Print Screen
	else 
		return T_OK;

	fnAPD_EjrIndexSet();
	m_pDevCmn->fnAPL_EjrSpoolPrintStart(PrintDevice, &EjrData);

	// yaokq add 20081101 
	if(((m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||
		(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
		(m_pDevCmn->TranStatus == TRAN_TOTAL))		&& 
		 (PrintDevice & DEV_SPR))
	{
		js = 7;
	}

	int tempCnt = itemof(ShuData);

	for (i = 0; i < itemof(ShuData); i++)
	{
		Len = strlen(ShuData[i]);
		if (PrintDevice & DEV_SPR) 
			m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, js++, Len, GetString(ShuData[i], Len, Len));
// 		else
		if (PrintDevice & DEV_JPR) 
			m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, jj++, Len, GetString(ShuData[i], Len, Len));
	}

	if (PrintDevice & DEV_SPR)
		SlipPrintFlag = TRUE;
	if (PrintDevice & DEV_JPR)
		JnlPrintFlag = TRUE;

	m_pDevCmn->fnAPL_EjrSpoolEnd(PrintDevice);

	if(fnAPD_CheckDeviceAction(PrintDevice) == T_OK)					// added by yusy 08.09.09
	{
		if (PrintDevice & DEV_SPR)
		{
			m_pProfile->GetTransProfile();
			m_pProfile->TRANS.SPRPrintCnt += 1;
			m_pProfile->PutTransProfile();
		}
		if (PrintDevice & DEV_JPR)
		{
// 			m_pProfile->GetTransProfile();
// 			m_pProfile->TRANS.JPRPrintCnt += tempCnt;
// 			m_pProfile->TRANS.INKPrintCnt += tempCnt;
// 			m_pProfile->PutTransProfile();
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_ShuPrint(...):return");
	return T_OK;
}

// JPR Print(Internal Procedure)
int	CTranCmn::fnAPD_JnlPrint(int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_JnlPrint(WaitFlag[%d])", WaitFlag);

	int		i, jj =0, Len;

	fnAPD_EjrIndexSet();
	m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_JPR, &EjrData);

	for (i = 0; i < itemof(ShuData); i++)
	{
		Len = strlen(ShuData[i]);
		m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, jj++, Len, GetString(ShuData[i], Len, Len));
	}

	m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_JPR);

	if (WaitFlag)
		fnAPD_CheckDeviceAction(DEV_JPR);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_JnlPrint(...):return");
	return T_OK;
}

// Material Out
int CTranCmn::fnAPD_MaterialOut(int Device, int CheckTime, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut(Device[%x], CheckTime[%d], WaitFlag[%d])", Device, CheckTime, WaitFlag);

	int		ScrNum = 0;
	int		ActDevice = Device;

/////////////////////////////////////////////////////////////////////////////
	MaterialOutFlag = 0;										// Material Out Flag
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))
		ActDevice &= ~DEV_CSH;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:ActDevice[%x]", ActDevice);

/////////////////////////////////////////////////////////////////////////////
	if (CardTran)
	{
		if (HostResp.CardRetractFlag)							// Card Retract Flag
			ActDevice &= ~DEV_MCU;
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:"			\
													"ActDevice[%x], "				\
													"HostResp.CardRetractFlag[%d]", 
													ActDevice, 
													HostResp.CardRetractFlag);

/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_SPR)									// Spr Out(Order)
	{
		if (!SlipPrintFlag)
			ActDevice &= ~DEV_SPR;
	}

	if (ActDevice & DEV_CSH)									// Cash Out & Reject(Order)
	{
		if ((TranProc == TRAN_WITH) &&
			(!CashCountFlag))									// Cash Count Flag
			ActDevice &= ~DEV_CSH;
		else
		if ((TranProc == TRAN_DEP) &&
			(!CashInOpenFlag))									// Cash In Open Flag
			ActDevice &= ~DEV_CSH;
		else
		{
			fnAPD_CheckDeviceAction(DEV_CSH);
			Delay_Msg(200);										// Sensor Wait(200ms)
			fnAPD_CheckDeviceAction(DEV_CSH);
		
			if (!m_pDevCmn->fnCSH_GetMaterialInfo())
				ActDevice &= ~DEV_CSH;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:ActDevice[%x], "	\
													"TranProc[%d], "					\
													"CashCountFlag[%d], "				\
													"CashInOpenFlag[%d], ",
													ActDevice,
													TranProc,
													CashCountFlag,
													CashInOpenFlag);

/////////////////////////////////////////////////////////////////////////////
	fnAPD_CheckDeviceAction(ActDevice);

/////////////////////////////////////////////////////////////////////////////
	if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo()) &&
		((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())
							&&
		(!((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())))
	{
		MaterialOutFlag |= DEV_SPR;
		MaterialOutFlag |= DEV_MCU;
		
		ScrNum = 622;											// 2008.12.08 liyi add 先退凭条再退卡
		m_pDevCmn->DeviceTimeout &= ~DEV_SPR;
		m_pDevCmn->fnSPR_Eject(InitInfo.TakeSlipTimeOut);
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_SPR, FLICKER_ON);
		if (ScrNum)
		{
			if ((m_pDevCmn->TranStatus == TRAN_TOTAL)		||
				(m_pDevCmn->TranStatus == TRAN_TOTALPRE)		||
				(m_pDevCmn->TranStatus == TRAN_TOTALKIND)		||
				(m_pDevCmn->TranStatus == TRAN_CARDRETAIN)		||
				(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)||
				(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT)||
				(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||
				(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
				(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
				(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
				(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
				(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
				(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||		// added by yusy ic
				(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
				(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
				(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
				(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
				(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||
				(m_pDevCmn->TranStatus == TRAN_DCEJREND)	||
				(m_pDevCmn->fnSCR_GetCurrentScreenNo() == 403))
				;
			else
			{
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(3000);
			}
		}
		
		ScrNum = 621;

		if (LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <== %s>",	// VM60413 完善流水
				GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
			m_pDevCmn->fnAPL_PrintJnlInfo("*******************************");
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 退卡 卡号:%s>",	// VM60413 完善流水
				GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
			m_pDevCmn->fnAPL_PrintJnlInfo("*******************************");
		}
		
		m_pDevCmn->DeviceTimeout &= ~DEV_MCU;

		hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 5, m_pDevCmn->iMonitorCS_Custom,
			CardAccNoForMonitor, "TAKE CARD", "NO");			// 打开取卡拍摄脸的摄像头  by zjd
		m_pDevCmn->fnMCU_Eject(CheckTime);
														// 2005.11.11
		if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
			(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
			ScrNum = 0;									// Screen
		else
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_ON);

	}
	else 
	if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo()) &&
		((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())
							&&
		(!((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())))
	{
		MaterialOutFlag |= DEV_SPR;
		MaterialOutFlag |= DEV_PBM;

		ScrNum = 622;											// Slip & Pb Screen
		if (TranProc == TRAN_WITH)
		{
			if (ActDevice & DEV_CSH)
				ScrNum = 622;									// Slip & Pb & Cash Screen
		}
		if (TranProc == TRAN_DEP)
		{
			if (ActDevice & DEV_CSH)
				ScrNum = 622;									// Slip & Pb & Cash Screen
		}

		m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_PBM);
		m_pDevCmn->fnSPR_Eject(CheckTime);
		m_pDevCmn->fnPBM_Eject(CheckTime);	
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_SPR, FLICKER_ON);
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_ON);
	}
	else 
	if (((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo()) &&
		((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())
							&&
		(!((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())))
	{
		MaterialOutFlag |= DEV_MCU;
		MaterialOutFlag |= DEV_PBM;

		ScrNum = 621;											// Card & Pb Screen
		if (TranProc == TRAN_WITH)
		{
			if (ActDevice & DEV_CSH)
				ScrNum = 621;									// Card & Pb & Cash Screen
		}
		if (TranProc == TRAN_DEP)
		{
			if (ActDevice & DEV_CSH)
				ScrNum = 621;									// Card & Pb & Cash Screen
		}

		m_pDevCmn->DeviceTimeout &= ~(DEV_MCU | DEV_PBM);

		hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 5, m_pDevCmn->iMonitorCS_Custom,
			CardAccNoForMonitor, "TAKE CARD", "NO");					// 打开取卡拍摄脸的摄像头  by zjd
		m_pDevCmn->fnMCU_Eject(CheckTime);
		m_pDevCmn->fnPBM_Eject(CheckTime);
		if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||	// 2005.11.11
			(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
			ScrNum = 0;											// Pb Screen
		else
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_ON);
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_ON);
	}
	else 
	{
		if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())	||
			((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())	||
			((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()))
			;
		else
		{
			if (TranProc == TRAN_WITH)
			{
				if (ActDevice & DEV_CSH)
					ScrNum = 651;								// Cash Screen
			}
			if (TranProc == TRAN_DEP)
			{
				if (ActDevice & DEV_CSH)
					ScrNum = 651;								// Cash Screen
			}
		}

		if (((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())	&&
			((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())	&&
			((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo()))
		{
			MaterialOutFlag |= DEV_SPR;
			MaterialOutFlag |= DEV_MCU;
			MaterialOutFlag |= DEV_PBM;

			ScrNum = 623;										// Spr & Card & Pb Screen
			if (TranProc == TRAN_WITH)
			{
				if (ActDevice & DEV_CSH)
					ScrNum = 623;								// Spr & Card & Pb & Cash Screen
			}
			if (TranProc == TRAN_DEP)
			{
				if (ActDevice & DEV_CSH)	
					ScrNum = 623;								// Spr & Card & Pb & Cash Screen
			}

			m_pDevCmn->DeviceTimeout &= ~(DEV_SPR | DEV_MCU | DEV_PBM);

			hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 5, m_pDevCmn->iMonitorCS_Custom,
				CardAccNoForMonitor, "TAKE CARD", "NO");				// 打开取卡拍摄脸的摄像头  by zjd
			m_pDevCmn->fnMCU_Eject(CheckTime);
			m_pDevCmn->fnSPR_Eject(CheckTime);
			m_pDevCmn->fnPBM_Eject(CheckTime);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_SPR, FLICKER_ON);
																// 2005.11.11
			if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
				(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				ScrNum = 622;									// Spr & Pb Screen
			else
				m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_ON);
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_ON);
		}
		else
		{
			if ((ActDevice & DEV_SPR) && m_pDevCmn->fnSPR_GetMaterialInfo())
			{
				MaterialOutFlag |= DEV_SPR;

				ScrNum = 622;									// Slip Screen
				if (TranProc == TRAN_WITH)
				{
					if (ActDevice & DEV_CSH)
						ScrNum = 622;							// Slip & Cash Screen
				}
				if (TranProc == TRAN_DEP)
				{
					if (ActDevice & DEV_CSH)
						ScrNum = 622;							// Slip & Cash Screen
				}

				m_pDevCmn->DeviceTimeout &= ~DEV_SPR;
				m_pDevCmn->fnSPR_Eject(CheckTime);
				m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_SPR, FLICKER_ON);
			}

			if ((ActDevice & DEV_MCU) && m_pDevCmn->fnMCU_GetMaterialInfo())
			{
				MaterialOutFlag |= DEV_MCU;

				ScrNum = 621;									// Card Screen
				if (TranProc == TRAN_WITH)
				{
					if (ActDevice & DEV_CSH)
						ScrNum = 621;							// Card & Cash Screen
				}
				if (TranProc == TRAN_DEP)
				{
					if (ActDevice & DEV_CSH)
						ScrNum = 621;							// Card & Cash Screen
				}
/////////////////////////////////////////////////////////////////////////////
				
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <== %s>",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
					m_pDevCmn->fnAPL_PrintJnlInfo("*******************************");
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 退卡 卡号:%s>",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
					m_pDevCmn->fnAPL_PrintJnlInfo("*******************************");
				}

				m_pDevCmn->DeviceTimeout &= ~DEV_MCU;

				hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 5, m_pDevCmn->iMonitorCS_Custom,
					CardAccNoForMonitor, "TAKE CARD", "NO");			// 打开取卡拍摄脸的摄像头  by zjd
				m_pDevCmn->fnMCU_Eject(CheckTime);
																// 2005.11.11
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					ScrNum = 0;									// Screen
				else
					m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_ON);
			}

			if ((ActDevice & DEV_PBM) && m_pDevCmn->fnPBM_GetMaterialInfo())
			{
				MaterialOutFlag |= DEV_PBM;

				ScrNum = 0;										// Pb Screen
				if (TranProc == TRAN_WITH)
				{
					if (ActDevice & DEV_CSH)
						ScrNum = 0;								// Pb & Cash Screen
				}
				if (TranProc == TRAN_DEP)
				{
					if (ActDevice & DEV_CSH)
						ScrNum = 0;								// Pb & Cash Screen
				}

				m_pDevCmn->DeviceTimeout &= ~DEV_PBM;
				m_pDevCmn->fnPBM_Eject(CheckTime);	
				m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_ON);
			}
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut:MaterialOutFlag[%x]", MaterialOutFlag);

/////////////////////////////////////////////////////////////////////////////
	if (WaitFlag)
	{
		fnAPD_CheckDeviceAction(ActDevice);

		if (ActDevice & DEV_PBM)								// Print Waiting
		{
			ULONG  WaitTime = TimerSet(10);						// 10Sec
			while (m_pDevCmn->fnPBM_GetMaterialInfo() & ST_SENSOR2)
			{
				Delay_Msg(100);
				if (CheckTimer(WaitTime))
					break;
			}
		}
	}

	// Pb Guide Section
	if (MaterialOutFlag & DEV_PBM)
	{
																// Pb Turn Message
																// Pb Change Message
	}

	if (ScrNum)
	{
		if ((m_pDevCmn->TranStatus == TRAN_TOTAL)		||		// No Screen
			(m_pDevCmn->TranStatus == TRAN_TOTALPRE)		||
			(m_pDevCmn->TranStatus == TRAN_TOTALKIND)		||
			(m_pDevCmn->TranStatus == TRAN_CARDRETAIN)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)||
			(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT)||
			(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||		// 2004.07.05
			(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
			(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
			(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
			(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
			(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
			(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||		// added by yusy ic
			(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
			(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
			(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
			(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
			(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||		// 2006.07.03
			(m_pDevCmn->TranStatus == TRAN_DCEJREND)	||
			(m_pDevCmn->fnSCR_GetCurrentScreenNo() == 403 && ScrNum != 621))
			;
		else
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, CheckTime);	// 2006.05.20
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOut(...):return");
	return T_OK;
}

// Material Out Check Retract
int CTranCmn::fnAPD_MaterialOutCheckRetract(int Device, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract(Device[%x], WaitFlag[%d])", Device, WaitFlag);

	int		ActDevice = Device;
	int		RetractDevice = Device;
	int		ErrDevice = DEV_NONE;
	int		SprRetractCnt = -1;									// Default -1
	int		McuRetractCnt = -1;
	int		PbmRetractCnt = -1;

	if (m_pDevCmn->fnAPL_GetYesDevice(DEV_SPR))					// 2004.11.05
		SprRetractCnt = Asc2Int(m_pDevCmn->fstrSPR_GetRetractCnt());
	if (m_pDevCmn->fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(m_pDevCmn->fstrMCU_GetRetractCnt());
	if (m_pDevCmn->fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(m_pDevCmn->fstrPBM_GetRetractCnt());

/////////////////////////////////////////////////////////////////////////////
	MaterialOutCheckFlag = 0;									// Material Out Check Flag
/////////////////////////////////////////////////////////////////////////////

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;

	RetractDevice = ActDevice;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x]", ActDevice);

	if (!(MaterialOutFlag & DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!(MaterialOutFlag & DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!(MaterialOutFlag & DEV_PBM))
		ActDevice &= ~DEV_PBM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], MaterialOutFlag[%x]", ActDevice, MaterialOutFlag);

/////////////////////////////////////////////////////////////////////////////
	fnAPD_CheckDeviceAction(ActDevice);
	if (ActDevice)
	{
		if (ActDevice & DEV_SPR)
			m_pDevCmn->fnSPR_WaitTaken();
		if (ActDevice & DEV_MCU)
		{
			m_pDevCmn->fnMCU_WaitTaken();
//			hy_HCMonitor2_StopCap(m_pDevCmn->iFaceCamNo);	// 关闭取卡拍脸的摄像头  by zjd
			hy_HCMonitor2_RestartAllowed();
		}
		if (ActDevice & DEV_PBM)
			m_pDevCmn->fnPBM_WaitTaken();
		fnAPD_CheckDeviceAction(ActDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:m_pDevCmn->DeviceTimeout[%x]", m_pDevCmn->DeviceTimeout);

	    if(InitInfo.TranDvrMode == 1)
		{
			if ((ActDevice & DEV_MCU) && (!(m_pDevCmn->DeviceTimeout & DEV_MCU)))
			{
				fnAPP_DvrDataComm(CMDCODE_TAKECARD,DVRCHAN_IDX12,5,"TAKECARD");
			}
		}		

		// Sp Event Only : 2003.12.06
		if ((ActDevice & DEV_SPR) && (!(m_pDevCmn->DeviceTimeout & DEV_SPR)))
		{
				ActDevice &= ~DEV_SPR;
				MaterialOutCheckFlag |= DEV_SPR;
		}
		if ((ActDevice & DEV_MCU) && (!(m_pDevCmn->DeviceTimeout & DEV_MCU)))
		{
				ActDevice &= ~DEV_MCU;
				MaterialOutCheckFlag |= DEV_MCU;
		}
		if ((ActDevice & DEV_PBM) && (!(m_pDevCmn->DeviceTimeout & DEV_PBM)))
		{
				ActDevice &= ~DEV_PBM;
				MaterialOutCheckFlag |= DEV_PBM;
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], MaterialOutCheckFlag[%x], RetractDevice[%x], DevEnDisable[%x]", ActDevice, MaterialOutCheckFlag, RetractDevice, DevEnDisable);
		
//	if (!(DevEnDisable & DEV_MCU))								// Shutter Type Not Used : 2004.07.05
//	{
//		ActDevice &= ~DEV_MCU;
//		RetractDevice &= ~DEV_MCU;
//	}
//	if (!(DevEnDisable & DEV_PBM))								// Shutter Type Not Used : 2004.07.05
//	{
//		ActDevice &= ~DEV_PBM;
//		RetractDevice &= ~DEV_PBM;
//	}

	if (RetractDevice & DEV_SPR)
	{
		if (m_pDevCmn->fnSPR_GetMaterialInfo())
			fnAPD_MaterialRetract(DEV_SPR);
	}
	if (MaterialOutFlag & DEV_SPR)
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_SPR, FLICKER_OFF);

	if (RetractDevice & DEV_MCU)
	{
		if (m_pDevCmn->fnMCU_GetMaterialInfo())
			fnAPD_MaterialRetract(DEV_MCU);
	}
	if (MaterialOutFlag & DEV_MCU)
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MCU, FLICKER_OFF);

	if (RetractDevice & DEV_PBM) 
	{
		if (m_pDevCmn->fnPBM_GetMaterialInfo())
			fnAPD_MaterialRetract(DEV_PBM);
	}
	if (MaterialOutFlag & DEV_PBM)
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_PBM, FLICKER_OFF);

	fnAPD_CheckDeviceAction(RetractDevice);						// 2004.11.05

/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_SPR)									// Spr Out
	{
		if (m_pDevCmn->fnAPL_GetYesDevice(DEV_SPR))				// 2004.11.05
		{
			if ((SprRetractCnt >= 0)	&&
				(SprRetractCnt < Asc2Int(m_pDevCmn->fstrSPR_GetRetractCnt())))
				;
			else
			{
				ActDevice &= ~DEV_SPR;
				MaterialOutCheckFlag |= DEV_SPR;
			}
		}
	}

	if (ActDevice & DEV_MCU)									// Mcu Out
	{
		if (m_pDevCmn->fnAPL_GetYesDevice(DEV_MCU))
		{
			if ((McuRetractCnt >= 0)	&&
				(McuRetractCnt < Asc2Int(m_pDevCmn->fstrMCU_GetRetractCnt())))
				;
			else
			{
				ActDevice &= ~DEV_MCU;
				MaterialOutCheckFlag |= DEV_MCU;
			}
		}
	}

	if (ActDevice & DEV_PBM)									// Pbm Out
	{
		if (m_pDevCmn->fnAPL_GetYesDevice(DEV_PBM))
		{
			if ((PbmRetractCnt >= 0)	&&
				(PbmRetractCnt < Asc2Int(m_pDevCmn->fstrPBM_GetRetractCnt())))
				;
			else
			{
				ActDevice &= ~DEV_PBM;
				MaterialOutCheckFlag |= DEV_PBM;
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract:ActDevice[%x], MaterialOutCheckFlag[%x], RetractDevice[%x], DevEnDisable[%x]", ActDevice, MaterialOutCheckFlag, RetractDevice, DevEnDisable);

	if (MaterialOutCheckFlag & DEV_MCU)							// 2004.06.30
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_DvrSendDataProc("CO");							// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
	}

	if (ActDevice)												// Lost
	{
		// 2003.11.07
		if ((ActDevice & (DEV_MCU | DEV_SPR)) == (DEV_MCU | DEV_SPR))
		{
/////////////////////////////////////////////////////////////////////////////
			fnAPL_DvrSendDataProc("CO");						// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
			m_pDevCmn->fnAPL_StackError("0110391", "LOST CARD & SLIP", DEV_MCU | DEV_SPR);
			ErrDevice |= DEV_MCU;
			ErrDevice |= DEV_SPR;
		}
		else
		if ((ActDevice & (DEV_PBM | DEV_SPR)) == (DEV_PBM | DEV_SPR))
		{
			m_pDevCmn->fnAPL_StackError("0110691", "LOST PB & SLIP", DEV_PBM | DEV_SPR);
			ErrDevice |= DEV_PBM;
			ErrDevice |= DEV_SPR;
		}
		else
		if (ActDevice & DEV_MCU)
		{
/////////////////////////////////////////////////////////////////////////////
			fnAPL_DvrSendDataProc("CO");						// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
			m_pDevCmn->fnAPL_StackError("0110191", "LOST CARD", DEV_MCU);
			ErrDevice |= DEV_MCU;
		}
		else
		if (ActDevice & DEV_PBM)
		{
			m_pDevCmn->fnAPL_StackError("0110591", "LOST PB", DEV_PBM);
			ErrDevice |= DEV_PBM;
		}
		else
		if (ActDevice & DEV_SPR)
		{
//			m_pDevCmn->fnAPL_StackError("0110091", "LOST SLIP", DEV_SPR);
//			ErrDevice |= DEV_SPR;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	if (ErrDevice)
	{
		m_pDevCmn->DeviceLost |= ErrDevice;
		fnAPP_CancelProc(T_ERROR);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialOutCheckRetract(...):return");
	return T_OK;
}

// Material Retract
int CTranCmn::fnAPD_MaterialRetract(int Device, int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract(Device[%x], WaitFlag[%d])", Device, WaitFlag);

	int		ActDevice = Device;

/////////////////////////////////////////////////////////////////////////////
	MaterialRetractFlag = 0;									// Material Retract Flag : 2004.07.05
/////////////////////////////////////////////////////////////////////////////
	
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))
		ActDevice &= ~DEV_SPR;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU))
		ActDevice &= ~DEV_MCU;
	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM))
		ActDevice &= ~DEV_PBM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x]", ActDevice);

	if (MaterialOutCheckFlag & DEV_MCU)
		ActDevice &= ~DEV_MCU;
	if (MaterialOutCheckFlag & DEV_PBM)
		ActDevice &= ~DEV_PBM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x], MaterialOutCheckFlag[%x]", ActDevice, MaterialOutCheckFlag);

//	if (!(DevEnDisable & DEV_MCU))								// Shutter Type Not Used : 2004.07.05
//		ActDevice &= ~DEV_MCU;
//	if (!(DevEnDisable & DEV_PBM))								// Shutter Type Not Used : 2004.07.05
//		ActDevice &= ~DEV_PBM;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:ActDevice[%x], DevEnDisable[%x]", ActDevice, DevEnDisable);

	fnAPD_CheckDeviceAction(ActDevice);

	if (ActDevice & DEV_SPR)
	{
		if (m_pDevCmn->fnSPR_GetMaterialInfo())
		{
			MaterialRetractFlag |= DEV_SPR;						// 2004.07.05
			if (MaterialOutFlag & DEV_SPR)
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);		// Lost
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_SPR);		// Order
		}
	}

	if (ActDevice & DEV_MCU)
	{
		if (m_pDevCmn->fnMCU_GetMaterialInfo())
		{
			MaterialRetractFlag |= DEV_MCU;						// 2004.07.05
			if (MaterialOutFlag & DEV_MCU)
				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// Lost
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// Order

			int iRetractCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);	// added by liuxl 20120606
			int iMcuCnt = Asc2Int(m_pDevCmn->fstrMCU_GetRetractCnt());
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract:McuCnt[%d], CardNum[%d]", iMcuCnt, iRetractCnt);
			if (iMcuCnt > iRetractCnt)
			{
				if(m_pDevCmn->bAgentMode == 1)						// 060411 只在此处上送ATMUnet吞卡报文
					fnAPP_AgentIdxGetData(AGENT_CARD_RETRACT);
			
				if(InitInfo.TranDvrMode == 1)
				{
					fnAPP_DvrDataComm(CMDCODE_RETAINCARD,DVRCHAN_IDX12,5,"RETCARD");
					fnAPP_DvrDataComm(CMDCODE_TAKECARD,DVRCHAN_IDX12,5,"TAKECARD");
				}

				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <=>  %s>",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
					m_pDevCmn->fnAPL_PrintJnlInfo("*****CARD RETAIND*****");
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 卡号:%s>",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]), CardAccountNo));
					m_pDevCmn->fnAPL_PrintJnlInfo("*****吞卡*****");
				}

				if (HostResp.CardRetractFlag)
					fnAPD_CardRetainData(3);
				else
					fnAPD_CardRetainData(1);
				
				if (HostResp.CardEjectFlag != TRUE)
					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, 1);
				else
					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, 3);

//				int iRetractCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);

				if (LangMode)
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST1%02.2d",iRetractCnt),
									GetSprintf("%03.3d    TIME : %2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s",
									iRetractCnt + 1,
									&CardRetainData.TranDateTime[2],
									&CardRetainData.TranDateTime[4],
									&CardRetainData.TranDateTime[6],
									&CardRetainData.TranDateTime[8],
									&CardRetainData.TranDateTime[10],
									&CardRetainData.TranDateTime[12]));
					IniSetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST2%02.2d",iRetractCnt),
									GetSprintf("Card No : %s", CardRetainData.TranAccount));
					IniSetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST3%02.2d",iRetractCnt),
									GetSprintf("Card Type : %1.1s    ATM : %2.2s",
									&CardRetainData.TranType,
									CardRetainData.CardRetainInfo));
				}
				else
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST1%02.2d",iRetractCnt),
									GetSprintf("%03.3d    时间 : %2.2s-%2.2s-%2.2s %2.2s:%2.2s:%2.2s",
									iRetractCnt + 1,
									&CardRetainData.TranDateTime[2],
									&CardRetainData.TranDateTime[4],
									&CardRetainData.TranDateTime[6],
									&CardRetainData.TranDateTime[8],
									&CardRetainData.TranDateTime[10],
									&CardRetainData.TranDateTime[12]));
					IniSetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST2%02.2d",iRetractCnt),
									GetSprintf("卡号 : %s", CardRetainData.TranAccount));
					IniSetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST3%02.2d",iRetractCnt),
									GetSprintf("输入方式 : %1.1s    ATM吞卡原因 : %2.2s",
									&CardRetainData.TranType,
									CardRetainData.CardRetainInfo));
				}
				IniSetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", iRetractCnt + 1);
			}

//			MaterialRetractFlag |= DEV_MCU;						// 2004.07.05
//			if (MaterialOutFlag & DEV_MCU)
//				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// Lost
//			else
//				m_pDevCmn->fnAPL_RetractMaterial(DEV_MCU);		// Order
		}
	}

	if (ActDevice & DEV_PBM) 
	{
		if (m_pDevCmn->fnPBM_GetMaterialInfo())
		{
			MaterialRetractFlag |= DEV_PBM;						// 2004.07.05
			if (MaterialOutFlag & DEV_PBM)
				m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);		// Lost
			else
				m_pDevCmn->fnAPL_RetractMaterial(DEV_PBM);		// Order
		}
	}

	if (WaitFlag)
		fnAPD_CheckDeviceAction(ActDevice);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MaterialRetract(...):return");
	return T_OK;
}

// Money Count Initialize
int CTranCmn::fnAPD_MoneyCountInitialize()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountInitialize()");
																// 2006.02.09
/////////////////////////////////////////////////////////////////////////////
	CashRejCSTBeforeCnt = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT);		
																// Cash Reject CST Before Count
	CashFstCSTBeforeCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1);		
																// Cash First CST Before Count
	CashSndCSTBeforeCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2);		
																// Cash Second CST Before Count
	CashTrdCSTBeforeCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3);		
																// Cash Third CST Before Count
	CashFthCSTBeforeCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4);		
																// Cash Fourth CST Before Count
	
	CashRejCSTAfterCnt = CashRejCSTBeforeCnt;					// Cash Reject CST After Count
	CashFstCSTAfterCnt = CashFstCSTBeforeCnt;					// Cash First CST After Count
	CashSndCSTAfterCnt = CashSndCSTBeforeCnt;					// Cash Second CST After Count
	CashTrdCSTAfterCnt = CashTrdCSTBeforeCnt;					// Cash Third CST After Count
	CashFthCSTAfterCnt = CashFthCSTBeforeCnt;					// Cash Fourth CST After Count

	CashRejCSTRejCnt = 0;										// Cash Reject CST Reject Count
	CashFstCSTDispRejCnt = 0;									// Cash First CST Dispense Reject Count
	CashSndCSTDispRejCnt = 0;									// Cash Second CST Dispense Reject Count
	CashTrdCSTDispRejCnt = 0;									// Cash Third CST Dispense Reject Count
	CashFthCSTDispRejCnt = 0;									// Cash Fourth CST Dispense Reject Count
	
	CashDispPrintFlag = FALSE;									// Cash Dispense Print Flag
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountInitialize():return");
	return T_OK;
}

// Money Count
int CTranCmn::fnAPD_MoneyCount(int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount(WaitFlag[%d])", WaitFlag);

////////////////////////////////////////////////////////////Stream Capture by zjd
//	m_pDevCmn->fnAPL_CaptureFaceSave(1, TRUE);					// 2005.11.11
//	m_pDevCmn->fnAPL_CaptureHandSave(2);						// Speed Up

	hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 3,
		m_pDevCmn->iMonitorCS_Custom, CardAccNoForMonitor, "COUNT FACE", "NO");
	hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iHandCamNo, 3,
		m_pDevCmn->iMonitorCS_Custom, CardAccNoForMonitor, "COUNT HAND","NO");
/////////////////////////////////////////////////////////////////////////////
	fnAPD_MoneyCount10050(WaitFlag);
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount(...):return");
	return T_OK;
}

// Money Count 100 50
int CTranCmn::fnAPD_MoneyCount10050(int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount10050(WaitFlag[%d])", WaitFlag);

	int				CashMoney = Asc2Int(&Accept.Money[4], 6);	// 2004.11.05
	int				CashRemain100 = Asc2Int(&Accept.Money[4], 6) % CASH_CHINA_100;
	int				CashRemain50 = Asc2Int(&Accept.Money[4], 6) % CASH_CHINA_50;
	CString			strTemp("");
	CStringArray	strTempArray;

/////////////////////////////////////////////////////////////////////////////
	CashFstCSTDispCnt = 0;										// Cash First CST Dispence Count
	CashSndCSTDispCnt = 0;										// Cash Second CST Dispence Count
	CashTrdCSTDispCnt = 0;										// Cash Third CST Dispence Count
	CashFthCSTDispCnt = 0;										// Cash Fourth CST Dispence Count
	
	CashCountFlag = FALSE;										// Cash Count Flag
	CashCountEjrFlag = FALSE;									// 2005.05.26
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount:"				\
													"CashMoney[%d], "				\
													"CashRemain100[%d], "			\
													"CashRemain50[%d], "			\
													"PMDRecvData.ParaLength[%d]",
													CashMoney,
													CashRemain100,
													CashRemain50,
													PMDRecvData.ParaLength);

	CashCountEjrFlag = TRUE;									// 2005.11.04
/////////////////////////////////////////////////////////////////////////////
	if ((!CashMoney)	||										// Zero Count
		(CashRemain50))
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(10);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(10);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		return T_OK;
	}

	if ((m_pDevCmn->TranStatus == TRAN_TOTAL)		||			// No Screen
		(m_pDevCmn->TranStatus == TRAN_TOTALPRE)		||
		(m_pDevCmn->TranStatus == TRAN_TOTALKIND)		||
		(m_pDevCmn->TranStatus == TRAN_CARDRETAIN)		||
		(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)||
		(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT)||
		(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||			// 2004.07.05
		(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
		(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
		(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
		(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
		(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
		(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||		// added by yusy ic
		(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
		(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
		(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
		(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
		(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||			// 2006.07.03
		(m_pDevCmn->TranStatus == TRAN_DCEJREND))
		;
	else
		m_pDevCmn->fnSCR_DisplayScreen(641);					// Cash Counting Screen
	
/////////////////////////////////////////////////////////////////////////////
	fnAPD_CheckDeviceAction(DEV_CSH);
	strTemp = m_pDevCmn->fstrCSH_GetDispenseOfCST(CashMoney);
	fnAPD_CheckDeviceAction(DEV_CSH);
	SplitString(strTemp, ",", strTempArray);					// 2004.06.22
	if (strTempArray.GetSize() >= 1)
		CashFstCSTDispCnt = Asc2Int(strTempArray[0]);
	if (strTempArray.GetSize() >= 2)
		CashSndCSTDispCnt = Asc2Int(strTempArray[1]);
	if (strTempArray.GetSize() >= 3)
		CashTrdCSTDispCnt = Asc2Int(strTempArray[2]);
	if (strTempArray.GetSize() >= 4)
		CashFthCSTDispCnt = Asc2Int(strTempArray[3]);
/////////////////////////////////////////////////////////////////////////////

	fnAPD_CheckDeviceAction(DEV_CSH);
// 	m_pDevCmn->fnCSH_ReadyDispense();							// 2011.01.04
// 	fnAPD_CheckDeviceAction(DEV_CSH);

	if ((m_pDevCmn->CSTCnt >= 1) &&								// CST Adjust
		(m_pDevCmn->CSTCnt <= 4))
	{
		switch (WithAvail) 
		{
			case TRAN_WITH_100:
				if (CashRemain100)
				{
/////////////////////////////////////////////////////////////////////////////
					fnAPL_WriteTransactonEjr(10);				// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
					fnAPP_CancelProc(T_PROGRAM);
				}
/////////////////////////////////////////////////////////////////////////////
				fnAPL_DvrSendDataProc("CB");					// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(3);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnCSH_Dispense(CashMoney);
				break;
			case TRAN_WITH_50:
/////////////////////////////////////////////////////////////////////////////
				fnAPL_DvrSendDataProc("CB");					// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(3);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnCSH_Dispense(CashMoney);
				break;
			case TRAN_WITH_BOTH:
/////////////////////////////////////////////////////////////////////////////
				fnAPL_DvrSendDataProc("CB");					// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(3);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnCSH_Dispense(CashMoney);
				break;

			case TRAN_WITH_NOT:
			default:
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(10);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				fnAPP_CancelProc(T_PROGRAM);
				break;
		}
	}
	else
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(10);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		fnAPP_CancelProc(T_PROGRAM);
	}

	CashCountFlag = TRUE;										// Cash Count Flag
	CashCountEjrFlag = TRUE;									// 2005.05.26

	if (WaitFlag)
		fnAPD_CheckDeviceAction(DEV_CSH);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCount10050(...):return");
	return T_OK;
}

// Money Count Check
int CTranCmn::fnAPD_MoneyCountCheck()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountCheck()");

	switch (m_pDevCmn->AtmDefine.CashHandler)					// 2006.06.02
	{
		case BRM_SET:
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
			break;
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);
			CashDispErrorFlag = 0;
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_CSH))
			{													// 2005.04.19
				CashDispErrorFlag = 1;
				CString	strErrorCode("");						// 2005.05.26
				strErrorCode = m_pDevCmn->fstrCSH_GetErrorCode();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountCheck():[%s]return", strErrorCode);
																// Count Error
				if ((strErrorCode.Left(5) == "40045")	||	
					(strErrorCode.Left(5) == "40089")	||
					(strErrorCode.Left(5) == "97197")	||
																// Sensor Error
					(strErrorCode.Left(4) == "4001")	||
					(strErrorCode.Left(5) == "40021")	||
					(strErrorCode.Left(5) == "40022")	||
					(strErrorCode.Left(5) == "40023")	||
					(strErrorCode.Left(5) == "40028")	||
					(strErrorCode.Left(5) == "40029")	||
					(strErrorCode.Left(5) == "4002A")	||
					(strErrorCode.Left(5) == "4002B")	||
					(strErrorCode.Left(5) == "40039")	||
					(strErrorCode.Left(5) == "4003B")	||
					(strErrorCode.Left(5) == "4003E")	||
					(strErrorCode.Left(5) == "4003F")	||
					(strErrorCode.Left(5) == "4004E")	||
					(strErrorCode.Left(5) == "4009B")	||
					(strErrorCode.Left(5) == "4009C")	||
					(strErrorCode.Left(5) == "400A1")	||
					(strErrorCode.Left(5) == "400AB")	||
					(strErrorCode.Left(5) == "400C7")	||
					(strErrorCode.Left(5) == "400C8")	||
					(strErrorCode.Left(5) == "400D8")	||
					(strErrorCode.Left(5) == "400DB")	||
					(strErrorCode.Left(5) == "400DC")	||
					(strErrorCode.Left(5) == "40084")	||
					(strErrorCode.Left(5) == "40087")	||
					(strErrorCode.Left(5) == "4008A")	||
					(strErrorCode.Left(5) == "4008B")	||
					(strErrorCode.Left(5) == "4008C")	||
					(strErrorCode.Left(5) == "4008D")	||
					(strErrorCode.Left(5) == "4008E")	||
					(strErrorCode.Left(5) == "4008F")	||
																// Set Error
					(strErrorCode.Left(5) == "4003C")	||
					(strErrorCode.Left(5) == "40059")	||
					(strErrorCode.Left(5) == "4009D")	||
					(strErrorCode.Left(5) == "400DD")	||
					(strErrorCode.Left(5) == "4003D")	||
					(strErrorCode.Left(5) == "4005A")	||
					(strErrorCode.Left(5) == "4009E")	||
					(strErrorCode.Left(5) == "400DE")	||
																// Missfeed Error
					(strErrorCode.Left(5) == "40047")	||
					(strErrorCode.Left(5) == "4005B")	||
					(strErrorCode.Left(5) == "4009F")	||
					(strErrorCode.Left(5) == "400DF")	||
																// Cst Jam
					(strErrorCode.Left(5) == "400C1")	||
					(strErrorCode.Left(5) == "400C2")	||
					(strErrorCode.Left(5) == "400C3")	||
					(strErrorCode.Left(5) == "400C4")	||
																// Reject Error
					(strErrorCode.Left(5) == "40041")	||
					(strErrorCode.Left(5) == "40043")	||
					(strErrorCode.Left(5) == "40044")	||
					(strErrorCode.Left(5) == "4004B")	||
					(strErrorCode.Left(5) == "4005D")	||

//					(strErrorCode.Left(5) == "400D0")	||		// NH Request : 2008.11.26
//					(strErrorCode.Left(5) == "400D2")	||		// 2010.10.20
//					(strErrorCode.Left(5) == "400D4")	||
//					(strErrorCode.Left(5) == "400A2")	||
//					(strErrorCode.Left(5) == "400AD")	||
					(strErrorCode.Left(5) == "400CC")	||
					(strErrorCode.Left(5) == "40011")	||
					(strErrorCode.Left(5) == "400F2")	||
					(strErrorCode.Left(5) == "400F3")	||
					(strErrorCode.Left(5) == "400F7")	||
					(strErrorCode.Left(5) == "400F8")	||
					(strErrorCode.Left(5) == "400F9")	||
					(strErrorCode.Left(5) == "400FA")	||
					(strErrorCode.Left(5) == "400FB")	||
					(strErrorCode.Left(5) == "400FC")	||
					(strErrorCode.Left(5) == "400FD")	||
					(strErrorCode.Left(5) == "4005C")	||

					(strErrorCode.Left(5) == "400BA")	||		// NH Request : 2009.01.16
					(strErrorCode.Left(5) == "400BB")	||

					(strErrorCode.Left(5) == "400FF")	||		// 2010.10.20
					(strErrorCode.Left(7) == "9792401"))
				{
					m_pDevCmn->DeviceStatus &= ~DEV_CSH;

					// added by yusy 08.09.09
					memcpy(&szAgentErrorCode[0], GetSprintf("%c",m_pDevCmn->CurErrBuff.ProcCount[0]), 1);
					memcpy(&szAgentErrorCode[1], GetSprintf("%-10.10s",GetSprintf("%7.7s",m_pDevCmn->CurErrBuff.ErrorCode)),10);
					
					HostNms.SaveErrCnt = m_pDevCmn->CurErrCnt;
					memcpy(&HostNms.SaveErrBuff, &m_pDevCmn->CurErrBuff, sizeof(HostNms.SaveErrBuff));

					m_pDevCmn->fnAPL_ClearError();
					Delay_Msg(200);								// Sensor Wait(200ms)
					if (LangMode)
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****CDM ERROR****",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("              ERRCODE: %s", strErrorCode.Left(5)));
					}
					else
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****出钞模块故障****",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("              故障代码: %s", strErrorCode.Left(5)));
					}

					m_pDevCmn->fnCSH_Initialize();
					fnAPD_CheckDeviceAction(DEV_CSH);
					m_pDevCmn->fnCSH_Retract();
					fnAPD_CheckDeviceAction(DEV_CSH);
					fnAPD_MoneyCount();							// Money Count
					fnAPD_CheckDeviceAction(DEV_CSH);			// 2008.11.03
					memset(&szAgentErrorCode[0], 'X', 11);
					CashDispErrorFlag = 0;
					HostNms.SaveErrCnt = 0;
					memset(&HostNms.SaveErrBuff, 0, sizeof(HostNms.SaveErrBuff));
				}
				else
				{
					fnAPD_CwcInfoSet(1);						// added by liuxl 20110714
					fnAPP_CancelProc(T_ERROR);
				}
			}
			break;
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountCheck():return");
	return T_OK;
}

// Money Out Check
int CTranCmn::fnAPD_MoneyOutCheck(int ScrNum, int CheckTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck(ScrNum[%d], CheckTime[%d])", ScrNum, CheckTime);

	int		ActDevice = DEV_NONE;
	int		CashRejectMethod = 0;
	int		TimeOverCshFlag = FALSE;
	int		iTmpCnt = 0;
	int 	iTmpAmt = 0;

/////////////////////////////////////////////////////////////////////////////
	MoneyOutFlag = 0;											// Money Out Flag
	MoneyOutCheckFlag = 0;										// Money Out Check Flag
	MoneyRetractFlag = 0;										// Money Retract Flag
/////////////////////////////////////////////////////////////////////////////

	if (TranProc == TRAN_WITH)
	{
		if (CashCountFlag)
		{
			ActDevice |= DEV_CSH;
			CashRejectMethod = 0;								// Reject(Withdraw)
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:"		\
													"ActDevice[%x], "			\
													"CashCountFlag[%d]",
													ActDevice, 
													CashCountFlag);

	if (TranProc == TRAN_DEP)
	{
		if (CashInOpenFlag)										// Cash In Open Flag
			;
		else 
			return T_OK;

		if (CashRejectFlag)
		{
			ActDevice |= DEV_CSH;
			CashRejectMethod = 1;								// Reject(Cancel)
		}
		if (CashRejectAFlag)
		{
			ActDevice |= DEV_CSH;
			CashRejectMethod = 2;								// Reject(Excess)
		}
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:"		\
													"ActDevice[%x], "			\
													"CashRejectFlag[%d], "		\
													"CashRejectAFlag[%d]",
													ActDevice, 
													CashRejectFlag, 
													CashRejectAFlag);

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))
		ActDevice &= ~ DEV_CSH;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:ActDevice[%x]", ActDevice);

	if (ActDevice & DEV_CSH)
		fnAPD_CheckDeviceAction(DEV_CSH);
	
/////////////////////////////////////////////////////////////////////////////
//	Delay_Msg(200);												// Sensor Wait(200ms)
	if (ActDevice & DEV_CSH)
	{
/////////////////////////////////////////////////////////////////////////////
		CashCountFlag = FALSE;
		CashRejectFlag = FALSE;
		CashRejectAFlag = FALSE;
/////////////////////////////////////////////////////////////////////////////

		if (ScrNum);
		else
		if (TranProc == TRAN_WITH)
		{
			if (Asc2Int(&Accept.Money[4], 6))
			{
				ScrNum = 651;									// Cash Take Screen
			}
		}
		else
		if (TranProc == TRAN_DEP)
		{
			if (ActDevice & DEV_CSH)
				ScrNum = 651;									// Reject Screen(Cancel)
		}

//		if (ScrNum == NNN)										// Reject Screen(Excess)
//		{
//			m_pDevCmn->fnSCR_DisplayNumeric(1, fnAPP_CalcCashMaxIn());
//		}

		if (ScrNum)
		{
			if ((m_pDevCmn->TranStatus == TRAN_TOTAL)		||	// No Screen
				(m_pDevCmn->TranStatus == TRAN_TOTALPRE)		||
				(m_pDevCmn->TranStatus == TRAN_TOTALKIND)		||
				(m_pDevCmn->TranStatus == TRAN_CARDRETAIN)		||
				(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)||
				(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT)||
				(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||	// 2004.07.05
				(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
				(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
				(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
				(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
				(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
				(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||	// added by yusy ic
				(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
				(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
				(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
				(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
				(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||	// 2006.07.03
				(m_pDevCmn->TranStatus == TRAN_DCEJREND))
				;
			else
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, CheckTime);
		}

/////////////////////////////////////////////////////////////////////////////
		if (ActDevice & DEV_CSH)
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_CSH, FLICKER_ON);

/////////////////////////////////////////////////////////////////////////////
		while (TRUE)
		{
			if (ActDevice & DEV_CSH)
			{
/////////////////////////////////////////////////////////////////////////////
				if (m_pProfile->NETWORK.InterfaceKind == 18)	// Cup(BJGD) :2006.07.07
					;
				else
				{
//					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [CASH : PRESENT]",
//																GetTime().GetBuffer(0),
//																&(GetTime().GetBuffer(0)[2]),
//																&(GetTime().GetBuffer(0)[4])));
//																// BOC(SD) SPEC : 2004.07.05
				}
/////////////////////////////////////////////////////////////////////////////
				MoneyOutFlag |= DEV_CSH;
				m_pDevCmn->DeviceTimeout &= ~DEV_CSH;
/////////////////////////////////////////////////////////////////////////////
				CashOutOpenFlag = TRUE;							// Cash Out Open Flag(TRUE)
/////////////////////////////////////////////////////////////////////////////
				if (TranProc == TRAN_WITH)
				{
					hy_HCMonitor2_StartCap(m_pDevCmn->iHandCamNo, m_pDevCmn->iMonitorCS_Custom,
						CardAccNoForMonitor, "TAKE CASH", "NO");	// 打开拍摄手的摄像头  by zjd
					m_pDevCmn->fnCSH_Present(CheckTime);
				}
				else
				{
					if (CashRejectMethod == 1)
						m_pDevCmn->fnCSH_Reject(CheckTime);
					else
						m_pDevCmn->fnCSH_Present(CheckTime);
				}
			}

/////////////////////////////////////////////////////////////////////////////
			if (ActDevice & DEV_CSH)							// 2006.06.02
			{
				fnAPD_CheckDeviceAction(DEV_CSH);
				m_pDevCmn->fnAPL_CaptureHandSave(2);
				m_pDevCmn->fnCSH_WaitLift();
				fnAPD_CheckDeviceAction(DEV_CSH);
			}

/////////////////////////////////////////////////////////////////////////////
			if (ActDevice & DEV_CSH)
			{
				if (!(m_pDevCmn->DeviceTimeout & DEV_CSH))		// 2005.11.11
				{
					int		CaptureIndex = 3;
					ULONG	WaitTime = TimerSet(CheckTime);
					while (TRUE)
					{
						if (CaptureIndex >= 7)
							CaptureIndex = 3;

						m_pDevCmn->fnAPL_CaptureHandSave(CaptureIndex++);

						if (m_pDevCmn->fnAPL_GetDeviceEvent(DEV_CSH, EVENT_OUT))
							break;
							
						if ((!CheckTime) ||
							(CheckTimer(WaitTime)))
							break;

						Delay_Msg(300);							// Performance Adjust
					}
											
					fnAPD_CheckDeviceAction(DEV_CSH);
					m_pDevCmn->fnCSH_WaitTaken();
					fnAPD_CheckDeviceAction(DEV_CSH);
					Delay_Msg(200);								// Sensor Wait(200ms)
				}
				if (m_pDevCmn->DeviceTimeout & DEV_CSH)			// Not Taken
					TimeOverCshFlag = TRUE;
				else											// Taken
					TimeOverCshFlag = FALSE;
			}

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:"				\
													"ActDevice[%x], "					\
													"m_pDevCmn->DeviceTimeout[%x], "	\
													"TimeOverCshFlag[%d], ",
													ActDevice, 
													m_pDevCmn->DeviceTimeout, 
													TimeOverCshFlag);

/////////////////////////////////////////////////////////////////////////////
			if (ActDevice & DEV_CSH)
			{
				if (!TimeOverCshFlag)							// Taken : 2006.06.02
				{
					CashOutOpenFlag = FALSE;					// Cash Out Open Flag(FALSE)
					m_pDevCmn->fnCSH_CloseShutter();			// Close Shutter
					fnAPD_CheckDeviceAction(DEV_CSH);
					Delay_Msg(200);								// Sensor Wait(200ms)

					if (!(m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM))
						break;
				}

				if (TimeOverCshFlag)
				{
					if(InitInfo.TranDvrMode == 1)
						 fnAPP_DvrDataComm(CMDCODE_RETAINCASH,DVRCHAN_IDX12,5,"RETCASH");
					
					m_pDevCmn->fnSCR_DisplayScreen(417);		// 显示钞票被回收  by zjd
					Delay_Msg(500);
					break;
				}
			}
		} 
	}

	hy_HCMonitor2_StopCap(m_pDevCmn->iHandCamNo);	// 关闭拍手的摄像头  by zjd

/////////////////////////////////////////////////////////////////////////////
	switch (m_pDevCmn->AtmDefine.CashHandler)					// 2006.06.02
	{
		case BRM_SET:
			if (TimeOverCshFlag)
			{
/////////////////////////////////////////////////////////////////////////////
// 				if (LangMode)
// 				{
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****CASH TAKEN TIME OUT****",
// 															GetTime().GetBuffer(0),
// 															&(GetTime().GetBuffer(0)[2]),
// 															&(GetTime().GetBuffer(0)[4])));
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CASH RETAIN  AMT:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
// 				}
// 				else
// 				{
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****客户取钞超时****",
// 															GetTime().GetBuffer(0),
// 															&(GetTime().GetBuffer(0)[2]),
// 															&(GetTime().GetBuffer(0)[4])));
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("回收钞票  金额:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
// 				}
// 
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, 2);
// /////////////////////////////////////////////////////////////////////////////
// 				MoneyRetractFlag |= DEV_CSH;					// Money Retract Flag
// ////////////////////// 记录统计值   by zjd 
// 				iTmpCnt =  IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
// 				iTmpCnt ++;
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, iTmpCnt);
// 				iTmpAmt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);
// 				iTmpAmt += Asc2Int(Accept.Money, sizeof(Accept.Money));
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, iTmpAmt);

				m_pDevCmn->fnCSH_Retract();						// Retract : 2005.04.07
				fnAPD_CheckDeviceAction(DEV_CSH);
				Delay_Msg(200);									// Sensor Wait(200ms)
			}

			if (CashOutOpenFlag)								// 2005.12.16
			{
				CashOutOpenFlag = FALSE;						// Cash Out Open Flag(FALSE)
				m_pDevCmn->fnCSH_CloseShutter();				// Close Shutter
				fnAPD_CheckDeviceAction(DEV_CSH);
				Delay_Msg(200);									// Sensor Wait(200ms)
			}
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
			if (CashOutOpenFlag)								// 2005.12.16
			{
				CashOutOpenFlag = FALSE;						// Cash Out Open Flag(FALSE)
				m_pDevCmn->fnCSH_CloseShutter();				// Close Shutter
				fnAPD_CheckDeviceAction(DEV_CSH);
				Delay_Msg(200);									// Sensor Wait(200ms)
			}
			break;
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
			if (TimeOverCshFlag)
			{
/////////////////////////////////////////////////////////////////////////////
// 				if (LangMode)
// 				{
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****CASH TAKEN TIME OUT****",
// 															GetTime().GetBuffer(0),
// 															&(GetTime().GetBuffer(0)[2]),
// 															&(GetTime().GetBuffer(0)[4])));
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CASH RETAIN  AMT:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
// 				}
// 				else
// 				{
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****客户取钞超时****",
// 															GetTime().GetBuffer(0),
// 															&(GetTime().GetBuffer(0)[2]),
// 															&(GetTime().GetBuffer(0)[4])));
// 					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("回收钞票金额:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
// 				}
// 
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, 2);
// /////////////////////////////////////////////////////////////////////////////
// 				MoneyRetractFlag |= DEV_CSH;					// Money Retract Flag
// ////////////////////// 记录统计值   by zjd 
// 				iTmpCnt =  IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
// 				iTmpCnt ++;
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, iTmpCnt);
// 				iTmpAmt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);
// 				iTmpAmt += Asc2Int(Accept.Money, sizeof(Accept.Money));
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, iTmpAmt);

				m_pDevCmn->fnCSH_Retract();						// Retract : 2005.04.07
				fnAPD_CheckDeviceAction(DEV_CSH);
				Delay_Msg(200);									// Sensor Wait(200ms)
			}

			if (CashOutOpenFlag)								// 2005.12.16
			{
				CashOutOpenFlag = FALSE;						// Cash Out Open Flag(FALSE)
				m_pDevCmn->fnCSH_CloseShutter();				// Close Shutter
				fnAPD_CheckDeviceAction(DEV_CSH);
				Delay_Msg(200);									// Sensor Wait(200ms)
			}
			break;
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}
	
/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_CSH)
		m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_CSH, FLICKER_OFF);
	
/////////////////////////////////////////////////////////////////////////////
	if (ActDevice & DEV_CSH)									// 2004.06.30
	{
		if (TimeOverCshFlag && 
			fnAPD_MoneyRetractPrint())							// added by liuxl 20120606
		{
			m_pDevCmn->DeviceTimeout &= ~DEV_CSH;
			TimeOverCshFlag = FALSE;
		}

		switch (m_pDevCmn->AtmDefine.CashHandler)				// 2006.06.02
		{
			case BRM_SET:
				if (TimeOverCshFlag)
				{
					if (TranProc == TRAN_WITH)
					{
/////////////////////////////////////////////////////////////////////////////
						fnAPL_DvrSendDataProc("CE");			// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
						fnAPL_WriteTransactonEjr(6);			// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
						m_pDevCmn->fnAPL_StackError("0110291", "LOST WITHDRAW CASH");
						m_pDevCmn->DeviceLost	|= DEV_CSH;
					}
					else
					{
/////////////////////////////////////////////////////////////////////////////
						fnAPL_DvrSendDataProc("CE");			// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
						fnAPL_WriteTransactonEjr(6);			// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
						m_pDevCmn->fnAPL_StackError("0110292", "LOST DEPOSIT CASH");
						m_pDevCmn->DeviceLost	|= DEV_CSH;
					}
					fnAPP_CancelProc(T_CANCEL);					// LY T_CANCEL
				}
				else
				{
/////////////////////////////////////////////////////////////////////////////
					fnAPL_DvrSendDataProc("CE");				// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
					MoneyOutCheckFlag |= DEV_CSH;
/////////////////////////////////////////////////////////////////////////////
					if (m_pProfile->NETWORK.InterfaceKind == 18)// Cup(BJGD) :2006.07.07
						;
					else
					{
						if (LangMode)
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s CASH TAKEN",
																	GetTime().GetBuffer(0),
																	&(GetTime().GetBuffer(0)[2]),
																	&(GetTime().GetBuffer(0)[4])));
															// BOC(SD) SPEC : 2004.07.05
						}
						else
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 客户取钞成功",
																	GetTime().GetBuffer(0),
																	&(GetTime().GetBuffer(0)[2]),
																	&(GetTime().GetBuffer(0)[4])));
						}
					}
/////////////////////////////////////////////////////////////////////////////
				}
				break;
			case CDU_SHUTTER1510_SET:
			case CDU_SHUTTER20_SET:
			case CDU_SHUTTER30_SET:
				if (m_pDevCmn->fnCSH_GetMaterialInfo() & BRM_IN_CSM)
				{
					if (TranProc == TRAN_WITH)
					{
/////////////////////////////////////////////////////////////////////////////
						fnAPL_DvrSendDataProc("CE");			// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
						fnAPL_WriteTransactonEjr(6);			// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
						m_pDevCmn->fnAPL_StackError("0110291", "LOST WITHDRAW CASH", DEV_CSH);
						m_pDevCmn->DeviceStatus |= DEV_CSH;
						m_pDevCmn->DeviceLost	|= DEV_CSH;
					}
					else
					{
/////////////////////////////////////////////////////////////////////////////
						fnAPL_DvrSendDataProc("CE");			// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
						fnAPL_WriteTransactonEjr(6);			// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
						m_pDevCmn->fnAPL_StackError("0110292", "LOST DEPOSIT CASH", DEV_CSH);
						m_pDevCmn->DeviceStatus |= DEV_CSH;
						m_pDevCmn->DeviceLost	|= DEV_CSH;
					}
					fnAPP_CancelProc(T_CANCEL);					// LY T_CANCEL
				}
				else
				{
/////////////////////////////////////////////////////////////////////////////
					fnAPL_DvrSendDataProc("CE");				// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
					MoneyOutCheckFlag |= DEV_CSH;
/////////////////////////////////////////////////////////////////////////////
					if (m_pProfile->NETWORK.InterfaceKind == 18)// Cup(BJGD) :2006.07.07
						;
					else
					{
						if (LangMode)
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s CASH TAKEN",
																	GetTime().GetBuffer(0),
																	&(GetTime().GetBuffer(0)[2]),
																	&(GetTime().GetBuffer(0)[4])));
						
																// BOC(SD) SPEC : 2004.07.05
						}
						else
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 客户取钞成功",
																	GetTime().GetBuffer(0),
																	&(GetTime().GetBuffer(0)[2]),
																	&(GetTime().GetBuffer(0)[4])));
						}
					}
/////////////////////////////////////////////////////////////////////////////
				}
				break;
			case CDU_RETRACT20_SET:
			case CDU_RETRACT30_SET:
			case CDU_PRESENT20_SET:
			case CDU_PRESENT30_SET:
			case CDU_GPRESENT20_SET:
			case CDU_GPRESENT30_SET:
				if (TimeOverCshFlag)
				{
					if (TranProc == TRAN_WITH)
					{
/////////////////////////////////////////////////////////////////////////////
						fnAPL_DvrSendDataProc("CE");			// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
						fnAPL_WriteTransactonEjr(6);			// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
						m_pDevCmn->fnAPL_StackError("0110291", "LOST WITHDRAW CASH");
						m_pDevCmn->DeviceLost	|= DEV_CSH;
					}
					else
					{
/////////////////////////////////////////////////////////////////////////////
						fnAPL_DvrSendDataProc("CE");			// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
						fnAPL_WriteTransactonEjr(6);			// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
						m_pDevCmn->fnAPL_StackError("0110292", "LOST DEPOSIT CASH");
						m_pDevCmn->DeviceLost	|= DEV_CSH;
					}
					fnAPP_CancelProc(T_CANCEL);					// LY T_CANCEL
				}
				else
				{
/////////////////////////////////////////////////////////////////////////////
					fnAPL_DvrSendDataProc("CE");				// 2005.06.15
/////////////////////////////////////////////////////////////////////////////
					MoneyOutCheckFlag |= DEV_CSH;
/////////////////////////////////////////////////////////////////////////////
					if (m_pProfile->NETWORK.InterfaceKind == 18)// Cup(BJGD) :2006.07.07
						;
					else
					{
						int nRejectCnt = 0;						
						nRejectCnt  = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT);
						
						int nFstCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1);
						int nSndCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2);
						int nTrdCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3);
						int nFthCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4);
						
						if (LangMode)
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s CASH TAKEN",
																	GetTime().GetBuffer(0),
																	&(GetTime().GetBuffer(0)[2]),
																	&(GetTime().GetBuffer(0)[4])));
																// BOC(SD) SPEC : 2004.07.05
						}
						else
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 客户取钞成功",
																	GetTime().GetBuffer(0),
																	&(GetTime().GetBuffer(0)[2]),
																	&(GetTime().GetBuffer(0)[4])));
						}
						
						if (LangMode)
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf(" REMAIN:%4.4d-%4.4d-%4.4d-%4.4d REJECT:%4.4d",
																	nFstCnt, nSndCnt, nTrdCnt, nFthCnt,
																	nRejectCnt));
						}
						else
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf(" 钞票剩余:%4.4d-%4.4d-%4.4d-%4.4d废钞箱:%4.4d",
																	nFstCnt, nSndCnt, nTrdCnt, nFthCnt,
																	nRejectCnt));
						}
					}
/////////////////////////////////////////////////////////////////////////////
				}
				break;
			case CDU_TRAY20_SET:
			case CDU_TRAY30_SET:
				break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck:MoneyOutCheckFlag[%d]", MoneyOutCheckFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyOutCheck(...):return");
	return T_OK;
}

// Money Retract
int CTranCmn::fnAPD_MoneyRetract()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract()");

	if (!m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))
		return T_OK;

	fnAPD_CheckDeviceAction(DEV_CSH);

	Delay_Msg(200);												// Sensor Wait(200ms)
	if (m_pDevCmn->fnCSH_GetMaterialInfo())
		m_pDevCmn->fnAPL_RetractMaterial(DEV_CSH);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetract():return");
	return T_OK;
}

// Money Count Print
int CTranCmn::fnAPD_MoneyCountPrint(int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountPrint(WaitFlag[%d])", WaitFlag);

	if (!CashCountFlag)											// 2005.02.24
		return T_OK;
	if (CashDispPrintFlag)										// 2005.02.24
		return T_OK;

	CashDispPrintFlag = TRUE;									// Cash Dispense Print Flag
																// 2006.02.09
	CashRejCSTAfterCnt = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT);		
																// Cash Reject CST After Count
	CashFstCSTAfterCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1);		
																// Cash First CST After Count
	CashSndCSTAfterCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2);		
																// Cash Second CST After Count
	CashTrdCSTAfterCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3);		
																// Cash Third CST After Count
	CashFthCSTAfterCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4);		
																// Cash Fourth CST After Count

	CashRejCSTRejCnt = CashRejCSTAfterCnt - CashRejCSTBeforeCnt;
	if (CashRejCSTRejCnt < 0)
		CashRejCSTRejCnt = 0;
	CashFstCSTDispRejCnt = CashFstCSTBeforeCnt - CashFstCSTAfterCnt;
	if (CashFstCSTDispRejCnt < 0)
		CashFstCSTDispRejCnt = 0;
	CashSndCSTDispRejCnt = CashSndCSTBeforeCnt - CashSndCSTAfterCnt;
	if (CashSndCSTDispRejCnt < 0)
		CashSndCSTDispRejCnt = 0;
	CashTrdCSTDispRejCnt = CashTrdCSTBeforeCnt - CashTrdCSTAfterCnt;
	if (CashTrdCSTDispRejCnt < 0)
		CashTrdCSTDispRejCnt = 0;
	CashFthCSTDispRejCnt = CashFthCSTBeforeCnt - CashFthCSTAfterCnt;
	if (CashFthCSTDispRejCnt < 0)
		CashFthCSTDispRejCnt = 0;

/////////////////////////////////////////////////////////////////////////////
																
	if (m_pProfile->NETWORK.InterfaceKind == 18)				// Cup(BJGD) :2006.07.07
		;
	else
	{															// 2005.03.22  hh:mm:ss CASSETTE:XX-XX-XX-XX
		if (LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s CASSETTE:%2.2d-%2.2d-%2.2d-%2.2d",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]),
													CashFstCSTDispRejCnt,
													CashSndCSTDispRejCnt,
													CashTrdCSTDispRejCnt,
													CashFthCSTDispRejCnt));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 各钞箱出钞张数:%2.2d-%2.2d-%2.2d-%2.2d",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]),
													CashFstCSTDispRejCnt,
													CashSndCSTDispRejCnt,
													CashTrdCSTDispRejCnt,
													CashFthCSTDispRejCnt));
		}

//		if (CashRejCSTRejCnt > 0)								// 2006.02.09
//		{
//			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [CASH : REJECT %2.2d]",
//														GetTime().GetBuffer(0),
//														&(GetTime().GetBuffer(0)[2]),
//														&(GetTime().GetBuffer(0)[4]),
//														CashRejCSTRejCnt));
//		}
	}

/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyCountPrint(...):return");
	return T_OK;
}

// Money Retract Print
int CTranCmn::fnAPD_MoneyRetractPrint(int WaitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetractPrint(WaitFlag[%d])", WaitFlag);

	int nRetractAction = RETRACT_NOTACTIVE;
	CStringArray strTempRetractArray;
	int		iTmpCnt = 0;
	int 	iTmpAmt = 0;
	
	nRetractAction = m_pDevCmn->fnCSH_GetRetractAction();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetractPrint(nRetractAction[%d])", nRetractAction);

	SplitString(m_pDevCmn->fstrCSH_GetRetractResult(), ",", strTempRetractArray);
	
	if (nRetractAction == RETRACT_RETRACTED)
	{
		switch (m_pDevCmn->AtmDefine.CashHandler)				// 2006.06.02
		{
			case BRM_SET:
				/////////////////////////////////////////////////////////////////////////////
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****CASH TAKEN TIME OUT****",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4])));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CASH RETAIN  AMT:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****客户取钞超时****",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4])));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("回收钞票  金额:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
				}
				
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, 2);
				/////////////////////////////////////////////////////////////////////////////
				MoneyRetractFlag |= DEV_CSH;					// Money Retract Flag
				////////////////////// 记录统计值   by zjd 
				iTmpCnt =  IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
				iTmpCnt ++;
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, iTmpCnt);
				iTmpAmt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);
				iTmpAmt += Asc2Int(Accept.Money, sizeof(Accept.Money));
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, iTmpAmt);

				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RETRACT SUCCESS]",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CASH100: %d CASH50: %d OTHER: %d",
															Asc2Int(strTempRetractArray[1]),
															Asc2Int(strTempRetractArray[2]), 
															Asc2Int(strTempRetractArray[5])));  //liyi change 2008.04.20
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 钞票回收成功",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("100面额张数:%d 50面额张数:%d 其它:%d",
															Asc2Int(strTempRetractArray[1]),
															Asc2Int(strTempRetractArray[2]),
															Asc2Int(strTempRetractArray[5])));
				}
				break;
			case CDU_SHUTTER1510_SET:
			case CDU_SHUTTER20_SET:
			case CDU_SHUTTER30_SET:
				break;
			case CDU_RETRACT20_SET:
			case CDU_RETRACT30_SET:
			case CDU_PRESENT20_SET:
			case CDU_PRESENT30_SET:
			case CDU_GPRESENT20_SET:
			case CDU_GPRESENT30_SET:
				/////////////////////////////////////////////////////////////////////////////
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****CASH TAKEN TIME OUT****",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
//					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CASH RETAIN  AMT:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****客户取钞超时****",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
//					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("回收钞票金额:%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
				}

				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, 2);
				/////////////////////////////////////////////////////////////////////////////
				MoneyRetractFlag |= DEV_CSH;					// Money Retract Flag
				////////////////////// 记录统计值   by zjd 
				iTmpCnt =  IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
				iTmpCnt ++;
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, iTmpCnt);
				iTmpAmt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);
				iTmpAmt += Asc2Int(Accept.Money, sizeof(Accept.Money));
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, iTmpAmt);

				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RETRACT SUCCESS]",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 钞票回收成功",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				break;
			case CDU_TRAY20_SET:
			case CDU_TRAY30_SET:
				break;
		}
	}
	else
	if (nRetractAction == RETRACT_CASHTAKEN)
	{
		if (LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RETRACT CASHTAKEN]",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 回收钞票已取走",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetractPrint:RetractAction[%d]", nRetractAction);
		return T_EXIT;											// added by liuxl 20120606
	}
	else
	if (nRetractAction == RETRACT_NOTACTIVE)
	{
		switch (m_pDevCmn->AtmDefine.CashHandler)				// 2006.06.02
		{
			case BRM_SET:
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RETRACT FAIL]",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CASH100: %d CASH50: %d OTHER: %d",
															Asc2Int(strTempRetractArray[1]),
															Asc2Int(strTempRetractArray[2]), 
															Asc2Int(strTempRetractArray[5])));  //liyi change 2008.04.20
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 钞票回收失败",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("100面额张数:%d 50面额张数:%d 其它:%d",
															Asc2Int(strTempRetractArray[1]),
															Asc2Int(strTempRetractArray[2]),
															Asc2Int(strTempRetractArray[5])));
				}
				break;
			case CDU_SHUTTER1510_SET:
			case CDU_SHUTTER20_SET:
			case CDU_SHUTTER30_SET:
				break;
			case CDU_RETRACT20_SET:
			case CDU_RETRACT30_SET:
			case CDU_PRESENT20_SET:
			case CDU_PRESENT30_SET:
			case CDU_GPRESENT20_SET:
			case CDU_GPRESENT30_SET:
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RETRACT FAIL]",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 钞票回收失败",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				break;
			case CDU_TRAY20_SET:
			case CDU_TRAY30_SET:
				break;
		}
	}

	int nRejectCnt = 0;	
	nRejectCnt  = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT);
	
	int nFstCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1);
	int nSndCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2);
	int nTrdCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3);
	int nFthCnt = m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4);
	if (LangMode)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf(" REMAIN:%4.4d-%4.4d-%4.4d-%4.4d REJECT:%4.4d",
												nFstCnt, nSndCnt, nTrdCnt, nFthCnt,
												nRejectCnt));
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf(" 钞票剩余:%4.4d-%4.4d-%4.4d-%4.4d废钞箱:%4.4d",
												nFstCnt, nSndCnt, nTrdCnt, nFthCnt,
												nRejectCnt));
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_MoneyRetractPrint(...):return");
	return T_OK;
}

int CTranCmn::fnAPD_PbcReadIcDataSmp(char *icdatalist, int *nlen)				// added by liuxl 20110516
{
	BYTE icdata[256];
	char tagbuffer[3];
	char lenbuffer[3];
	int len = 0;
	*nlen = 0;
	
	memset(tagbuffer, 0, sizeof(tagbuffer));
	memset(icdata, 0, sizeof(icdata));
	memset(lenbuffer, 0, sizeof(lenbuffer));

	//9F10
	m_pDevCmn->fnPBC_TlvGetValue(0x9F10, icdata, &len);
	if (len > 32) len = 32;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_IsuAppData(9F10):len[%d]data[%s]", len, MakeUnPack(icdata, len));
	MakePack("9F10", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", len).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, len);
	*nlen += 3 + len;

	//9F36
	m_pDevCmn->fnPBC_TlvGetValue(0x9F36, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_ATC(9F36):len[%d]data[%s]", len, MakeUnPack(icdata, 2));
	MakePack("9F36", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", 2).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, 2);
	*nlen += 5;

	//95
	m_pDevCmn->fnPBC_TlvGetValue(0x95, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TVR(95):len[%d]data[%s]", len, MakeUnPack(icdata, 5));
	MakePack("95", tagbuffer, 2);
	memcpy(&icdatalist[*nlen], tagbuffer, 1);
	MakePack(GetSprintf("%02X", 5).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+1], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+2], icdata, 5);
//	icdatalist[*nlen+2]	|= 0x80;								// added by liuxl 20110523
	*nlen += 7;

// 	//9F1E
// 	m_pDevCmn->fnPBC_TlvGetValue(0x9F1E, icdata, &len);
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_IFD(9F1E):len[%d]data[%s]", len, MakeUnPack(icdata, 8));
// 	MakePack("9F1E", tagbuffer, 4);
// 	memcpy(&icdatalist[*nlen], tagbuffer, 2);
// 	MakePack(GetSprintf("%02X", 8).GetBuffer(0), lenbuffer, 2);
// 	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
// 	memcpy(&icdatalist[*nlen+3], icdata, 8);
// 	*nlen += 11;

	//DF31
	if (ICScriptWriteFlag)
	{
		BYTE	btRet[1024];
		int		nLen = 0;

		memset(btRet, 0, sizeof(btRet));
		m_pDevCmn->fnPBC_TlvGetScriptResult(btRet, &nLen);
		nLen = nLen/2;
		MakePack("DF31", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", nLen).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], btRet, nLen);
		*nlen += 3 + nLen;
	}

	return T_OK;
}

int CTranCmn::fnAPD_PbcChangeScript()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcChangeScript()");

	BYTE icdatalist[1024];
	BYTE icdata[256];
	BYTE tag9F10[3];
	BYTE tag9F36[3];
	char szTmpData[1024];
	int nScriptLen, len, tempLen;
	int flag = 0;
	char lenbuffer[3];
	CString strScript, strDest;
	CStringArray strScriptArray;

	if (TranCode != TC_LOAD)
		return FALSE;

	nScriptLen = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM"));
	strScript = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScriptLen));
	if (strScript != "")
	{
		SplitString(strScript, ",", strScriptArray);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SrcScript[%s]",strScriptArray[6].GetBuffer(0));
		len = MakePack(strScriptArray[6].GetBuffer(0), szTmpData, strScriptArray[6].GetLength());
	}
	else
		return FALSE;

	if (m_pDevCmn->fnPBC_SelFinalAppSelection(PbcAppIndex, SEL_FOR_TRAN) == EMV_OK)
	{
		if (fnAPD_PbcReadApp() == T_OK)
		{
			if (fnAPD_PbcStoreTlv() == T_OK)
			{
				memset(icdata, 0, sizeof(icdata));
				m_pDevCmn->fnPBC_TlvGetValue(0x95, icdata, &tempLen);
				memset(icdata, 0xFF, tempLen);
				m_pDevCmn->fnPBC_TlvStoreValue(0x95, icdata, tempLen);

				fnAPD_PbcReadyTrans();
				PbcTermFlag = TRUE;
				flag = 1;
			}
		}
	}

	int i = 0;
	int j = 0;
	int k = 0;
	int nlen = 0;
	memset(icdata, 0, sizeof(icdata));
	memset(icdatalist, 0, sizeof(icdatalist));
	memset(tag9F10, 0, sizeof(tag9F10));
	memset(tag9F36, 0, sizeof(tag9F36));
	tag9F10[0] = 0x9F;
	tag9F10[1] = 0x10;
	tag9F36[0] = 0x9F;
	tag9F36[1] = 0x36;
	while (i < len)
	{
		strScript = "";

		if ((szTmpData[i++] & 0x1F) == 0x1F)
			i++;
		if (szTmpData[i] & 0x80)
		{
			if (szTmpData[i++] & 0x01)
			{
				nlen = int(szTmpData[i++]);
			}
			else
			{
				nlen = int(MAKEWORD(szTmpData[i + 1], szTmpData[i]));
				i += 2;
			}
		}
		else
			nlen = int(szTmpData[i++]);
		i += nlen;
		
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strTempArrayLen[%d]", i - j);
		if (memcmp(&szTmpData[j], tag9F10, 2) == 0)
		{
			if (flag)
			{
				m_pDevCmn->fnPBC_TlvGetValue(0x9F10, icdata, &nlen);
				memcpy(&icdatalist[k], tag9F10, 2);
				k += 2;
				MakePack(GetSprintf("%02X", nlen).GetBuffer(0), lenbuffer, 2);
				memcpy(&icdatalist[k++], lenbuffer, 1);
				memcpy(&icdatalist[k], icdata, nlen);
				k += nlen;
			}
			else
			{
				memcpy(&icdatalist[k], &szTmpData[j], i - j - nlen);
				k += i - j - nlen;
				memset(&icdatalist[k], 0x00, nlen);
				k += nlen;
			}
		}
		else
		if (memcmp(&szTmpData[j], tag9F36, 2) == 0)
		{
			if (flag)
			{
				m_pDevCmn->fnPBC_TlvGetValue(0x9F36, icdata, &nlen);
				memcpy(&icdatalist[k], tag9F36, 2);
				k += 2;
				MakePack(GetSprintf("%02X", nlen).GetBuffer(0), lenbuffer, 2);
				memcpy(&icdatalist[k++], lenbuffer, 1);
				memcpy(&icdatalist[k], icdata, nlen);
				k += nlen;
			}
			else
			{
				memcpy(&icdatalist[k], &szTmpData[j], i - j - nlen);
				k += i - j - nlen;
				memset(&icdatalist[k], 0x00, nlen);
				k += nlen;
			}
		}
		else
		{
			memcpy(&icdatalist[k], &szTmpData[j], i - j);
			k += i - j;
		}
		j = i;
	}

	strDest = MakeUnPack(icdatalist, k);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DestScript[%s]", strDest.GetBuffer(0));

	CString strTmp = GetSprintf("%6.6s,%8.8s,%6.6s,%8.8s,%12.12s,%s,%s,%3.3s", 
								strScriptArray[0].GetBuffer(0),
								strScriptArray[1].GetBuffer(0),
								strScriptArray[2].GetBuffer(0),
								strScriptArray[3].GetBuffer(0),
								strScriptArray[4].GetBuffer(0),
								strScriptArray[5].GetBuffer(0),
								strDest,
								strScriptArray[7].GetBuffer(0));
	
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScriptLen), "");
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScriptLen), strTmp);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcChangeScript():return");
	return TRUE;
}

int CTranCmn::fnAPD_PbcReadIcData(char *icdatalist, int *nlen)
{
	BYTE icdata[256];
	char tagbuffer[3];
	char lenbuffer[3];
	int len = 0;
	*nlen = 0;

	memset(tagbuffer, 0, sizeof(tagbuffer));
	memset(icdata, 0, sizeof(icdata));
	memset(lenbuffer, 0, sizeof(lenbuffer));

	//9F26
	m_pDevCmn->fnPBC_TlvGetValue(0x9F26, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_AC(9F26):len[%d]data[%s]", len, MakeUnPack(icdata, 8));
	MakePack("9F26", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", 8).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, 8);
	*nlen += 11;

	//9F10
	m_pDevCmn->fnPBC_TlvGetValue(0x9F10, icdata, &len);
	if (len > 32) len = 32;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_IsuAppData(9F10):len[%d]data[%s]", len, MakeUnPack(icdata, len));
	MakePack("9F10", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", len).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, len);
	*nlen += 3 + len;

	//9F37
	m_pDevCmn->fnPBC_TlvGetValue(0x9F37, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_UnpredictNo(9F37):len[%d]data[%s]", len, MakeUnPack(icdata, 4));
	MakePack("9F37", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", 4).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, 4);
	*nlen += 7;

	//9F36
	m_pDevCmn->fnPBC_TlvGetValue(0x9F36, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_ATC(9F36):len[%d]data[%s]", len, MakeUnPack(icdata, 2));
	MakePack("9F36", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", 2).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, 2);
	*nlen += 5;

	//95
	m_pDevCmn->fnPBC_TlvGetValue(0x95, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TVR(95):len[%d]data[%s]", len, MakeUnPack(icdata, 5));
	MakePack("95", tagbuffer, 2);
	memcpy(&icdatalist[*nlen], tagbuffer, 1);
	MakePack(GetSprintf("%02X", 5).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+1], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+2], icdata, 5);
//	icdatalist[*nlen+2]	|= 0x80;								// added by liuxl 20110523
	*nlen += 7;

	//9A
	m_pDevCmn->fnPBC_TlvGetValue(0x9A, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TrDate(9A):len[%d]data[%s]", len, MakeUnPack(icdata, 3));
	MakePack("9A", tagbuffer, 2);
	memcpy(&icdatalist[*nlen], tagbuffer, 1);
	MakePack(GetSprintf("%02X", 3).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+1], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+2], icdata, 3);
	*nlen += 5;

	//82
	m_pDevCmn->fnPBC_TlvGetValue(0x82, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_AIP(82):len[%d]data[%s]", len, MakeUnPack(icdata, 2));
	MakePack("82", tagbuffer, 2);
	memcpy(&icdatalist[*nlen], tagbuffer, 1);
	MakePack(GetSprintf("%02X", 2).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+1], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+2], icdata, 2);
	*nlen += 4;

	//9F1A
	m_pDevCmn->fnPBC_TlvGetValue(0x9F1A, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TCountryCod(9F1A):len[%d]data[%s]", len, MakeUnPack(icdata, 2));
	MakePack("9F1A", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", 2).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, 2);
	*nlen += 5;

	//9F33
	m_pDevCmn->fnPBC_TlvGetValue(0x9F33, icdata, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TerminalCapa(9F33):len[%d]data[%s]", len, MakeUnPack(icdata, 3));
	MakePack("9F33", tagbuffer, 4);
	memcpy(&icdatalist[*nlen], tagbuffer, 2);
	MakePack(GetSprintf("%02X", 3).GetBuffer(0), lenbuffer, 2);
	memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
	memcpy(&icdatalist[*nlen+3], icdata, 3);
	*nlen += 6;

//	if (TranCode != TC_SCRIPT)
	if (ICScriptWriteFlag != 1)								// added by liuxl 20110519
	{
		//9F27
		m_pDevCmn->fnPBC_TlvGetValue(0x9F27, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_CryptInfData(9F27):len[%d]data[%s]", len, MakeUnPack(icdata, 1));
		MakePack("9F27", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 1).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 1);
		*nlen += 4;

		//9C
		m_pDevCmn->fnPBC_TlvGetValue(0x9C, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TrType(9C):len[%d]data[%s]", len, MakeUnPack(icdata, 1));
		MakePack("9C", tagbuffer, 2);
		memcpy(&icdatalist[*nlen], tagbuffer, 1);
		MakePack(GetSprintf("%02X", 1).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+1], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+2], icdata, 1);
		*nlen += 3;

		//9F02
		m_pDevCmn->fnPBC_TlvGetValue(0x9F02, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_AmountAuthN(9F02):len[%d]data[%s]", len, MakeUnPack(icdata, 6));
		MakePack("9F02", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 6).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 6);
		*nlen += 9;

		//5F2A
		m_pDevCmn->fnPBC_TlvGetValue(0x5F2A, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TrCurCod(5F2A):len[%d]data[%s]", len, MakeUnPack(icdata, 2));
		MakePack("5F2A", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 2).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 2);
		*nlen += 5;

		//9F03
		memset(icdata, 0, sizeof(icdata));
		m_pDevCmn->fnPBC_TlvGetValue(0x9F03, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_AmountOtherN(9F03):len[%d]data[%s]", len, MakeUnPack(icdata, 6));
		MakePack("9F03", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 6).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 6);
		*nlen += 9;
		
		//9F34											// added by liuxl 20110508
		m_pDevCmn->fnPBC_TlvGetValue(0x9F34, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_CVMR(9F34):len[%d]data[%s]", len, MakeUnPack(icdata, 3));
		MakePack("9F34", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 3).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 3);
		*nlen += 6;

		//9F35
		m_pDevCmn->fnPBC_TlvGetValue(0x9F35, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TerminalType(9F35):len[%d]data[%s]", len, MakeUnPack(icdata, 1));
		MakePack("9F35", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 1).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 1);
		*nlen += 4;

		//84
		m_pDevCmn->fnPBC_TlvGetValue(0x84, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_DFName(84):len[%d]data[%s]", len, MakeUnPack(icdata, len));
		MakePack("84", tagbuffer, 2);
		memcpy(&icdatalist[*nlen], tagbuffer, 1);
		MakePack(GetSprintf("%02X", len).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+1], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+2], icdata, len);
		*nlen += 2 + len;

		//9F09
		m_pDevCmn->fnPBC_TlvGetValue(0x9F09, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TrAppVerNo(9F09):len[%d]data[%s]", len, MakeUnPack(icdata, 2));
		MakePack("9F09", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", 2).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, 2);
		*nlen += 5;

		//9F41
		m_pDevCmn->fnPBC_TlvGetValue(0x9F41, icdata, &len);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EMVTid_TrSeqCnt(9F41):len[%d]data[%s]", len, MakeUnPack(icdata, len));
		MakePack("9F41", tagbuffer, 4);
		memcpy(&icdatalist[*nlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", len).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[*nlen+2], lenbuffer, 1);
		memcpy(&icdatalist[*nlen+3], icdata, len);
		*nlen += 3 + len;
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Pboc Initial Data
int	CTranCmn::fnAPD_PbcInitialData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcInitialData()");

	int				nResult = FALSE;
	int				nAppCnt = 0;
	int				i = 0, nlen = 0;
	PBCst_APPLIST	stTempAppList[MAX_APP_NUM];

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	memset(PbcAppList, 0, sizeof(PbcAppList));					// Pboc App List
	memset(&PbcTerm, 0, sizeof(PbcTerm));						// Pboc Term
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	nAppCnt = 0;
	memset(stTempAppList, 0, sizeof(stTempAppList));
	for (i = 0; i < MAX_APP_NUM; i++)							// Get Pbc Kernel App List
	{
		nResult = m_pDevCmn->fnPBC_InitGetApplication(i, &stTempAppList[i]);
		if (nResult == EMV_OK)
		{
			nAppCnt += 1;
			continue;
		}
		else
			break;
	}

	if (nAppCnt > 0)											// Del Pbc Kernel App List
	{
		for (i = 0; i < nAppCnt; i++)
			m_pDevCmn->fnPBC_InitDelApplication(stTempAppList[i].AID, stTempAppList[i].AidLen);
	}

	//memcpy(stTempAppList, PbcAppTable, sizeof(PbcAppTable));	// Load ATM App List
	fnAPL_GetAIDListFileContent(stTempAppList, &nlen);
	for (i = 0; i < nlen; i++)
	{															// Store App List to Kernel
		if (stTempAppList[i].AidLen != 0)
			nResult = m_pDevCmn->fnPBC_InitAddApplication(&stTempAppList[i]);
	}

/////////////////////////////////////////////////////////////////////////////
	memcpy(PbcTerm.MerchName, "PSBC-ATM", 8);				// Load ATM Term Data
	MakePack("6011", PbcTerm.MerchCateCode, 4);
	memcpy(PbcTerm.MerchId, "000000000000001", 15);	
	memcpy(PbcTerm.TermId, m_pProfile->NETWORK.AtmSerialNum, 8);
	PbcTerm.TerminalType = 0x14;
	MakePack("604020", PbcTerm.Capability, 6);
	MakePack("E280B05001", PbcTerm.ExCapability, 10);
	PbcTerm.TransCurrExp = 0x02;
	PbcTerm.ReferCurrExp = 0x02;
	MakePack("0156", PbcTerm.ReferCurrCode, 4);
	MakePack("0156", PbcTerm.CountryCode, 4);
	MakePack("0156", PbcTerm.TransCurrCode, 4);
	PbcTerm.ReferCurrCon = 1000;
	PbcTerm.TransType = TYPE_GOODS;
	PbcTerm.ForceOnline = SUPPORT_NO;
	PbcTerm.GetDataPIN = SUPPORT_YES;
	PbcTerm.SupportPSESel = SUPPORT_YES;
	PbcTerm.SupportAccountTypeSel = SUPPORT_YES;
	PbcTerm.ECTSI = SUPPORT_NO;

	m_pDevCmn->fnPBC_TlvSetParam(&PbcTerm);						// Store Term Data

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcInitialData():return");
	return T_OK;
}

// Pboc Sel App
int	CTranCmn::fnAPD_PbcSelApp(int nSelAppMode, int btTryCntIn)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcSelApp(nSelAppMode[%d], btTryCntIn[%d])", nSelAppMode, btTryCntIn);

	int		nResult = FALSE;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	memset(PbcAppList, 0, sizeof(PbcAppList));					// Pboc App List
	memset(PbcAppDisp, 0, sizeof(PbcAppDisp));
	PbcAppTotal = 0;											// Pboc App Total
	PbcAppIndex = 0;											// Pboc App Index
	PbcAppDispTotal = 0;
/////////////////////////////////////////////////////////////////////////////

	PbcAppTotal = 0;											// Pboc App Total
	nResult = m_pDevCmn->fnPBC_SelApplicationSelection(0, 0, btTryCntIn, PbcAppList, &PbcAppTotal, nSelAppMode);
	if ((nResult == EMV_OK)	&&
		(PbcAppTotal > 0))
	{
		 fnAPD_PbcMakeAppDisplay(TRUE);							// 2010.10.20
	}
	else
	if ((nResult == ERR_EMVDENIAL)	||
		(nResult == ERR_ICCBLOCK)	||
		(nResult == ERR_APPBLOCK)	||
		(nResult == ERR_EMVDATA)	||
		(nResult == ERR_BLACKLIST))
		fnAPP_CancelProc(T_MSG, TranMsgTbl[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcSelApp[%d]", FallBack);
		return T_EXIT;											// FallBack
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcSelApp():return");
	return T_OK;
}

// Pboc Final App
int	CTranCmn::fnAPD_PbcFinalApp(int nFinalMode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcFinalApp(nFinalMode[%d])", nFinalMode);

	int		nResult = FALSE;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

	nResult = m_pDevCmn->fnPBC_SelFinalAppSelection(PbcAppIndex, nFinalMode);
	if (nResult == EMV_OK)
		;
	else
	if ((nResult == ERR_EMVDENIAL)	||
		(nResult == ERR_ICCBLOCK)	||
		(nResult == ERR_APPBLOCK)	||
//		(nResult == ERR_NOAPP)		||							// added by liuxl 20110506
		(nResult == ERR_BLACKLIST))
		fnAPP_CancelProc(T_MSG, TranMsgTbl[1][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
//	else
//	if (nResult == ERR_APPBLOCK)
//		return T_CANCEL;
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcFinalApp[%d]", FallBack);
		return T_EXIT;											// FallBack
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcFinalApp():return");
	return T_OK;
}

// Pboc Read Track
int	CTranCmn::fnAPD_PbcReadTrack()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadTrack()");

	int		nResult = FALSE;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	PbcCardData2Length = 0;										// Pboc Card Data 2TR Length
	memset(PbcCardData2, 0, sizeof(PbcCardData2));				// Pboc Card Data 2TR
/////////////////////////////////////////////////////////////////////////////

	nResult = m_pDevCmn->fnPBC_TransReadAppData(0x57);			// Read Card Data 2TR
	if (nResult == EMV_OK)
		;
	else
	if ((nResult == ERR_EMVDENIAL)	||
		(nResult == ERR_ICCBLOCK)	||
		(nResult == ERR_APPBLOCK)	||
		(nResult == ERR_BLACKLIST))
		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcCardData2Length[%d]", FallBack);
		return T_EXIT;											// FallBack
	}
																// Get Tag 5700 Data
	nResult = m_pDevCmn->fnPBC_TlvGetValue(0x57, PbcCardData2, &PbcCardData2Length);
	if (nResult == EMV_OK)
		;
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcReadTrack[%d]", FallBack);
		return T_EXIT;											// FallBack
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadTrack():return");
	return T_OK;
}

// Pboc Read Pan
int	CTranCmn::fnAPD_PbcReadPan()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadPan()");

	int		nResult = FALSE;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	PbcCardData2Length = 0;										// Pboc Card Data 2TR Length
	memset(PbcCardData2, 0, sizeof(PbcCardData2));				// Pboc Card Data 2TR
/////////////////////////////////////////////////////////////////////////////

	nResult = m_pDevCmn->fnPBC_TransReadAppData(0x5A);			// Read Primary	Account	Number
	if (nResult == EMV_OK)
		;
	else
	if ((nResult == ERR_EMVDENIAL)	||
		(nResult == ERR_ICCBLOCK)	||
		(nResult == ERR_APPBLOCK)	||
		(nResult == ERR_BLACKLIST))
		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcReadPan[%d]", FallBack);
		return T_EXIT;											// FallBack
	}
																// Get Tag 5a00 Data(Primary Account Number)
	nResult = m_pDevCmn->fnPBC_TlvGetValue(0x5A, PbcCardData2, &PbcCardData2Length);
	if (nResult == EMV_OK)
		;
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcReadPan[%d]", FallBack);
		return T_EXIT;											// FallBack
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadPan():return");
	return T_OK;
}

// Pboc Read App
int	CTranCmn::fnAPD_PbcReadApp()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadApp()");

	int		nResult = FALSE;
	CString			strCardData("");
	CStringArray	strCardDataArray;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	PbcCardData2Length = 0;										// Pboc Card Data 2TR Length
	memset(PbcCardData2, 0, sizeof(PbcCardData2));				// Pboc Card Data 2TR
/////////////////////////////////////////////////////////////////////////////

	nResult = m_pDevCmn->fnPBC_TransReadAppData(0x00);			// Read App All Data Set Tag 0
	if (nResult == EMV_OK)
		;
	else
	if ((nResult == ERR_EMVDENIAL)	||
		(nResult == ERR_ICCBLOCK)	||
		(nResult == ERR_EMVDATA)	||
		(nResult == ERR_APPBLOCK)	||
		(nResult == ERR_BLACKLIST))
		return T_EXIT;
//		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcReadApp[%d]", FallBack);
		return T_EXIT;											// FallBack
	}
																// Get Tag 5700 Data(Track2 Equivalent Data)
	if (m_pDevCmn->fnPBC_TlvGetValue(0x57, PbcCardData2, &PbcCardData2Length) == EMV_OK)
		;
	else														// Tag "5A00" - Primary Account Number
	if (m_pDevCmn->fnPBC_TlvGetValue(0x5A, PbcCardData2, &PbcCardData2Length) == EMV_OK)
		;
	else
		return T_EXIT;
//		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);

/////////////////////////////////////////////////////////////////////////////
	strCardData = fstrAPD_PbcMakeCardData();						// Pboc Make Card Data : 2010.10.20
	fnAPD_CardIcCheck(strCardData);								// Card Ic Check
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadApp():return");
	return T_OK;
}

// Pboc Store Tlv
int	CTranCmn::fnAPD_PbcStoreTlv()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcStoreTlv()");

	int		nResult = FALSE;
	BYTE	btVal[1024];
	
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
	m_pDevCmn->Pbc.TranAmount = Asc2Int(Accept.Money, sizeof(Accept.Money));			
																// Tag "9F02" - Amount Authorized (Numeric)

/////////////////////////////////////////////////////////////////////////////
	memset(btVal, 0, sizeof(btVal));
	if (TranCode == TC_WITHDRAWAL)								
		btVal[0] = 0x01;
	else
	if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)
		btVal[0] = 0x40;
	else
	if (TranCode == TC_INQUIRY || TranCode == TC_OTHER_INQ || TranCode == TC_DETAIL	||
		TranCode == TC_ECHANNEL_INQ || TranCode == TC_ECHANNEL ||TranCode == TC_INQUIRY_CheckPin)
		btVal[0] = 0x30;
	else
	if (TranCode == TC_LOAD)
		btVal[0] = 0x60;
	else
	if (TranCode == TC_BOUNDMDOIFY)
		btVal[0] = 0x57;
	else
	if (TranCode == TC_CHANGEPIN)
		btVal[0] = 0x70;
	else
		return T_OK;

	m_pDevCmn->fnPBC_TlvStoreValue(0x9C, btVal, 1);
	
	memset(btVal, 0, sizeof(btVal));						// added by liuxl 20110602
	int len = 0;
	m_pDevCmn->fnPBC_TlvGetValue(0x95, btVal, &len);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TVR(95):len[%d]data[%s]", len, MakeUnPack(btVal, len));
	btVal[0] |= 0x80;
	m_pDevCmn->fnPBC_TlvStoreValue(0x95, btVal, len);
/////////////////////////////////////////////////////////////////////////////
	memset(btVal, 0, sizeof(btVal));							// Tag "9F41" - Transaction Sequence Counter
//	MakePack(Int2Asc((Asc2Int(m_pProfile->TRANS.SerialNo, 8) + 1), 8).GetBuffer(0), &btVal[1], 8);
	MakePack(Int2Asc((Asc2Int(m_pProfile->TRANS.SerialNo, 8) + 1), 8).GetBuffer(0), &btVal[0], 8);	// added by liuxl 20120328
	m_pDevCmn->fnPBC_TlvStoreValue(0x9f41, btVal, 4);

	m_pDevCmn->fnPBC_TlvStoreValue(0x9f39, (BYTE*)&m_pDevCmn->Pbc.POSEntryCode, 1);
																// Tag "9F39" - POS Entry Code
	
//	btVal[0] = ACCNT_TYPE_DEFAULT;								// Tag "5F57" - Account Type
	btVal[0] = ACCNT_TYPE_SAVING;								// Saving
//	btVal[0] = ACCNT_TYPE_DEBIT;								// Checking
//	btVal[0] = ACCNT_TYPE_CREDIT;								// Credit
	m_pDevCmn->fnPBC_TlvStoreValue(0x5f57, btVal, 1);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcStoreTlv():return");
	return T_OK;
}

// Pboc Ready Trans
int	CTranCmn::fnAPD_PbcReadyTrans()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadyTrans()");

	int		nResult = FALSE;
	BYTE	btRet[1024];
	
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	memset(&PbcHostData, 0, sizeof(PbcHostData));				// Initialize Host Respose Ic Send Data
	PbcHostData.Result = ONLINE_FAILED;							// Host Result Default Fail
	PbcHostData.AuthCodeLen = 0;								// ATM no  8583 bit 38 Data
	PbcHostData.AuthDataLen = 0;								// Tag "9100" - Issuer Authorization Data Len Default 0
	PbcHostData.ScripDataLen = 0;								// ScriptLen Default 0
/////////////////////////////////////////////////////////////////////////////

	memset(btRet, 0, sizeof(btRet));
	nResult = m_pDevCmn->fnPBC_TransProcess(btRet);
	if (nResult == EMV_OK)
		;
	else
	if ((nResult == ERR_EMVDENIAL)	||
		(nResult == ERR_ICCBLOCK)	||
		(nResult == ERR_APPBLOCK)	||
		(nResult == ERR_BLACKLIST))
		return T_EXIT;
//		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	else
	{
		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcReadyTrans[%d]", FallBack);
		return T_EXIT;											// FallBack
	}

	if (btRet[0] == TRUE)
	{
/////////////////////////////////////////////////////////////////////////////
		PbcTermFlag = FALSE;									// Pboc Terminate Flag
/////////////////////////////////////////////////////////////////////////////
	}
	else														// OFFLINE_TRANS, REVERSAL_TRANS
	{
		PbcHostData.Result = ONLINE_ABORT;						// Host Result Default Fail
		nResult = m_pDevCmn->fnPBC_TransCompletion(PbcHostData.Result, PbcHostData.RspCode,
												PbcHostData.AuthCode, PbcHostData.AuthCodeLen,
												PbcHostData.AuthData, PbcHostData.AuthDataLen,
												PbcHostData.ScripData, PbcHostData.ScripDataLen);

		FallBack = 1;											// added by liuxl 20110512
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcReadyTrans[%d]", FallBack);
		return T_EXIT;											// FallBack
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcReadyTrans():return");
	return T_OK;
}

// Pboc Set Response
int	CTranCmn::fnAPD_PbcSetResponse()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcSetResponse()");

	int		nResult = FALSE;
	int		nLen = 0;

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranResult)		// Host Ok
		PbcHostData.Result = ONLINE_APPROVE;
	else
		PbcHostData.Result = ONLINE_DENIAL;						// Host Ng

	if (TranCode == TC_INQUIRY	||
		TranCode == TC_OTHER_INQ||
		TranCode == TC_DETAIL	||
		TranCode == TC_ECHANNEL_INQ	||
		TranCode == TC_ECHANNEL		||
		TranCode == TC_INQUIRY_CheckPin)
		PbcHostData.Result = ONLINE_DENIAL;

	memcpy(PbcHostData.RspCode, HostResp.RejectCode, 2);		// ATM 8583 bit 39 Data

	memcpy(PbcHostData.AuthData, IsuAuthData, IsuAuthDataLen);		// Tag "9100" - Issuer Authorization Data
	PbcHostData.AuthDataLen = IsuAuthDataLen;

	nLen = 0;

	if (IcScriptDataLen != 0)									// added by yusy 20110526
	{
		PbcHostData.ScripData[nLen] = 0x71;
		nLen += 1;
		PbcHostData.ScripData[nLen] = IcScriptDataLen;
		nLen += 1;
		memcpy(&PbcHostData.ScripData[nLen], IcScriptData, IcScriptDataLen);		// Tag "7100" - Issuer Script Template 1
		nLen += IcScriptDataLen;
	}
	if (IcScriptDataLen2 != 0)									// added by yusy 20110526
	{
		PbcHostData.ScripData[nLen] = 0x72;
		nLen += 1;
		PbcHostData.ScripData[nLen] = IcScriptDataLen2;
		nLen += 1;
		memcpy(&PbcHostData.ScripData[nLen], IcScriptData2, IcScriptDataLen2);
																// Tag "7200" - Issuer Script Template 2
		nLen += IcScriptDataLen2;
	}

	PbcHostData.ScripDataLen = nLen;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcSetResponse():return");
	return T_OK;
}

// Pboc Result
int	CTranCmn::fnAPD_PbcTerminate()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcTerminate()");

	int		nResult = FALSE;
	int		nRet = FALSE;
	BYTE	btRet[1024];
	int		nLen = 0;
	BYTE	icdata[512];
	char	icdatalist[1024];
	int		len = 0;
	int		listlen = 0;
	char	tagbuffer[3];
	char	lenbuffer[3];

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

	if (PbcTermFlag)											// Pboc Terminate Flag Check
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	PbcTermFlag = TRUE;											// Pboc Terminate Flag : 2010.10.20
/////////////////////////////////////////////////////////////////////////////

	nResult = m_pDevCmn->fnPBC_TransCompletion(PbcHostData.Result, PbcHostData.RspCode,
											PbcHostData.AuthCode, PbcHostData.AuthCodeLen,
											PbcHostData.AuthData, PbcHostData.AuthDataLen,
											PbcHostData.ScripData, PbcHostData.ScripDataLen);
	
	memset(icdata, 0, sizeof(icdata));
	memset(icdatalist, 0, sizeof(icdatalist));
	m_pDevCmn->fnPBC_TlvGetValue(0x9F26, icdata, &len);
	
	if (nResult == EMV_OK)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s GENAC2 TC:%s",
			GetTime().GetBuffer(0),
			&(GetTime().GetBuffer(0)[2]),
			&(GetTime().GetBuffer(0)[4]),
			MakeUnPack(icdata, len)));
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s GENAC2 AAC:%s",
			GetTime().GetBuffer(0),
			&(GetTime().GetBuffer(0)[2]),
			&(GetTime().GetBuffer(0)[4]),
			MakeUnPack(icdata, len)));
	}

	if (TranCode == TC_WITHDRAWAL	||
		TranCode == TC_LOAD)
	{
// 		if (TranCode == TC_WITHDRAWAL)							// added by liuxl 20110508
// 		{
// 			int nlen = 0;
// 			char szTmpData[1024];
// 			memset(szTmpData, 0, sizeof(szTmpData));
// 			
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");
// 			fnAPD_PbcReadIcDataSmp(szTmpData, &nlen);
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, MakeUnPack(szTmpData, nlen));
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TRANFIELD55: %s", MakeUnPack(szTmpData, nlen));
// 		}

		if (m_pDevCmn->TranResult)								// Host Ok
		{
			if (nResult == EMV_OK)
				;
			else
			{
				if (TranCode == TC_LOAD		&&
					ICScriptWriteFlag)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo("0110291 - IC FAIL AAC");
//					nRet = TRUE;
				}
				else
				{
					m_pDevCmn->fnAPL_StackError("0110291", "IC FAIL AAC");
					SprPrintATCFlag = 1;
					if (TranCode == TC_WITHDRAWAL)					// added by liuxl 20120313
						fnAPD_CwcInfoSet(1);
					fnAPP_CancelProc(T_MSG, TranMsgTbl[24][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
																// Reverse(CWC)
				}
			}
		}
	}

	memset(btRet, 0, sizeof(btRet));
	memset(tagbuffer, 0, sizeof(tagbuffer));
	memset(lenbuffer, 0, sizeof(lenbuffer));
	m_pDevCmn->fnPBC_TlvGetValue(0x95, btRet, &nLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcTerminate(TVR0x95[%s]):return", btRet);
	m_pDevCmn->fnPBC_TlvGetValue(0x9B, btRet, &nLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcTerminate(TSI0x9B[%s]):return", btRet);

//	if (m_pDevCmn->TranResult && ICScriptWriteFlag)		// Host Ok
	if (ICScriptWriteFlag)								// added by liuxl 20110508
	{
		memset(btRet, 0, sizeof(btRet));
		nResult = m_pDevCmn->fnPBC_TlvGetScriptResult(btRet, &nLen);
		CString strTmp = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM", "");
		int nScriptLen = 0;
		if (strTmp == "" || Asc2Int(strTmp) <= 0)
			;
		else
			nScriptLen = Asc2Int(strTmp);
		nScriptLen += 1;
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM", nScriptLen);
		fnAPD_PbcReadIcData(icdatalist, &listlen);
		MakePack("DF31", tagbuffer, 4);
		memcpy(&icdatalist[listlen], tagbuffer, 2);
		MakePack(GetSprintf("%02X", nLen).GetBuffer(0), lenbuffer, 2);
		memcpy(&icdatalist[listlen+2], lenbuffer, 1);
		memcpy(&icdatalist[listlen+3], btRet, nLen);
		listlen += 3 + nLen;
		memset(icdata, 0, sizeof(icdata));
		m_pDevCmn->fnPBC_TlvGetValue(0x5F34, (BYTE*)icdata, &len);
		strTmp = GetSprintf("%6.6s,%8.8s,%6.6s,%8.8s,%12.12s,%s,%s,%03d", HMERecv.Data[3],
									HMERecv.Data[13],
									HMERecv.Data[12],
									HMERecv.Data[11],
//									HMERecv.Data[4],
									HMESend.Data[4],					// added by liuxl 20110509
									CardAccountNo,
									MakeUnPack(icdatalist, listlen),
									atoi(MakeUnPack(icdata, len)));

		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScriptLen), strTmp);

		if (nResult == EMV_OK)
		{
			if ((btRet[0] & 0xf0) == 0x20)					// Success
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s =>>%s OK",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					CardAccountNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s =>>%s FAILURE",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					CardAccountNo));
				SprPrintATCFlag = 1;
				if (TranCode == TC_WITHDRAWAL)					// added by liuxl 20120330
					fnAPD_CwcInfoSet(1);
				if (TranCode == TC_LOAD)
					nRet = TRUE;
				else
					fnAPP_CancelProc(T_MSG, TranMsgTbl[24][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s =>>%s UNKNOWN",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4]),
				CardAccountNo));
			SprPrintATCFlag = 1;
			if (TranCode == TC_WITHDRAWAL)					// added by liuxl 20120330
					fnAPD_CwcInfoSet(1);
			if (TranCode == TC_LOAD)
				nRet = TRUE;
			else
				fnAPP_CancelProc(T_MSG, TranMsgTbl[24][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
		}
		
		if (nRet)
		{
			if (fnAPD_PbcChangeScript())
			{
				m_pDevCmn->fnAPL_StackError("0110291", "SCRIPT FAIL");
				fnAPP_CancelProc(T_MSG, TranMsgTbl[24][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}
		}
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcTerminate():return");
	return T_OK;
}

// Pboc Make Card Data
CString	CTranCmn::fstrAPD_PbcMakeCardData()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcMakeCardData()");

	int		nResult = FALSE;
	CString strTrack2EquData("");
	CString strReturn(",,");

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return strReturn;
/////////////////////////////////////////////////////////////////////////////

	strTrack2EquData = MakeUnPack(PbcCardData2, PbcCardData2Length);
	
	if (strTrack2EquData.GetLength())
	{
		strTrack2EquData.Replace("D", "=");						// =
		strTrack2EquData.Replace("F", "");						// For Size 37
	}

	strReturn.Format("%s,%s,%s", "", MakeUnPack(strTrack2EquData.GetBuffer(0), strTrack2EquData.GetLength()), "");
	////2014.04.19 by jl 屏蔽IC卡的二三磁道信息 /////
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcMakeCardData():return()");
	//	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcMakeCardData():return(,%s,)", strTrack2EquData);
	return strReturn;
}

// Pboc EC Inq
CString	CTranCmn::fstrAPD_PbcECInq()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcECInq()");

	int		nResult = FALSE;
	BYTE	btRet[1024];
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return strReturn;
/////////////////////////////////////////////////////////////////////////////

	memset(btRet, 0, sizeof(btRet));
	nResult = m_pDevCmn->fnPBC_EcGetBalance(btRet); 
	if (nResult == EMV_OK)
		;
	else
		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	
	strReturn.Format("%s", MakeUnPack(btRet, 6));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcECInq():return(%s)", strReturn);
	return strReturn;
}

// Pboc EC Limit
CString	CTranCmn::fstrAPD_PbcECLimit()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcECLimit()");

	int		nResult = FALSE;
	BYTE	btRet[1024];
	int		nLen = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return strReturn;
/////////////////////////////////////////////////////////////////////////////

	memset(btRet, 0, sizeof(btRet));
	nResult = m_pDevCmn->fnPBC_EcGetIccData(0x9f77, btRet, &nLen); 
	if (nResult == EMV_OK)
		;
	else
		fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
	
	strReturn.Format("%s", MakeUnPack(btRet, nLen));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcECLimit():return(%s)", strReturn);
	return strReturn;
}

// Pboc EC Detail Inq
CString	CTranCmn::fstrAPD_PbcECDetailInq()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcECDetailInq()");

	int		nResult = FALSE;
	BYTE	btRet[1024];
	int		nLen = 0;
	CString strReturn("");

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
		(m_pDevCmn->McuReadMSOnlyFlag))
		return strReturn;
/////////////////////////////////////////////////////////////////////////////

	memset(btRet, 0, sizeof(btRet));
	for (int i = 1; i <= 10; i++)
	{
		nResult = m_pDevCmn->fnPBC_EcReadLogRecord(i);
		if (nResult == EMV_OK)
			;
		else
			break;
		
		if (i != 1)												// Record Mark
			strReturn = strReturn + ";";
																// Tag "9A00" -	Transaction	Date
        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9a, btRet, &nLen))
			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
		else
			strReturn = strReturn + ",";
																// Tag "9F21" -	Transaction	Time
        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9f21, btRet, &nLen))
			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
		else
			strReturn = strReturn + ",";
																// Tag "9F36" -	Application	Transaction	Counter
        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9f36, btRet, &nLen))
			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
		else
			strReturn = strReturn + ",";
																// Tag "9C00" -	Transaction	Type
        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9c, btRet, &nLen))
			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
		else
			strReturn = strReturn + ",";
																// Tag "9F02" -	Amount Authorized (Numeric)
		if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9f02, btRet, &nLen))
			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
		else
			strReturn = strReturn + ",";
																// Tag "9F4E"
		if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9f4e, btRet, &nLen))
			strReturn = strReturn + MakeUnPack(btRet, nLen);
//
//																// Tag "9F03" -	Amount Other (Numeric)
//        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9f03, btRet, &nLen))
//			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
//		else
//			strReturn = strReturn + ",";
//																// Tag "9F1A" -	Terminal Countery Code
//        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x9f1a, btRet, &nLen))
//			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
//		else
//			strReturn = strReturn + ",";
//																// Tag "5F2A" -	Transaction	Currency Code
//        if (!m_pDevCmn->fnPBC_EcGetLogItem(0x5f2a, btRet, &nLen))
//			strReturn = strReturn + MakeUnPack(btRet, nLen) + ",";
//		else
//			strReturn = strReturn + ",";
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fstrAPD_PbcECDetailInq():return(%s)", strReturn);
	return strReturn;
}

// Pboc Make App Display
int CTranCmn::fnAPD_PbcMakeAppDisplay(BOOL bFlag)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcMakeAppDisplay()");
	
	int				i = 0, j = 0, k = 0;
	BYTE			btIndex;
	PBCst_APPLIST   stTempAppDisp[MAX_APP_NUM];

/////////////////////////////////////////////////////////////////////////////
    if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||           // 2010.10.20
        (m_pDevCmn->McuReadMSOnlyFlag))
		return T_OK;
/////////////////////////////////////////////////////////////////////////////

	if (bFlag)
    {                                                                                                       // Sort App List
		if (PbcAppTotal == 1)
			memcpy(PbcAppDisp, PbcAppList, sizeof(PbcAppList));
        else
        {
			j = 0;
			k = 0;
			btIndex = 0x00;
			for (i = 1; i <= 16; i++)
			{
				if (i == 16)
					btIndex = 0x00;
				else
					btIndex = i & 0x0F;
				
				for(j = 0; j < PbcAppTotal; j++)
				{
					if ((PbcAppList[j].Priority & 0x0F) == btIndex)
					{
						memcpy(&PbcAppDisp[k++], &PbcAppList[j], sizeof(PBCst_APPLIST));
					}
				}
			}
		}
		PbcAppDispTotal = PbcAppTotal;
	}
	else
	{                                                                                                       // Del Final Select Fail Aid
		j = 0;
		memset(stTempAppDisp, 0, sizeof(stTempAppDisp));

        for (i = 0; i < PbcAppDispTotal; i++)
		{
			if ((PbcAppList[PbcAppIndex].AidLen == PbcAppDisp[i].AidLen) &&
				(memcmp(PbcAppList[PbcAppIndex].AID, PbcAppDisp[i].AID, PbcAppList[PbcAppIndex].AidLen) == 0))
				continue;
			else
				memcpy(&stTempAppDisp[j++], &PbcAppDisp[i], sizeof(PBCst_APPLIST));
		}

		memcpy(PbcAppDisp, stTempAppDisp, sizeof(stTempAppDisp));

		PbcAppDispTotal--;

        if (PbcAppDispTotal == 0)
		{
			FallBack = 1;											// added by liuxl 20110512
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_PbcMakeAppDisplay[%d]", FallBack);
			return T_EXIT;											// FallBack
		}
	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPD_PbcMakeAppDisplay():return");

	return T_OK;
}
