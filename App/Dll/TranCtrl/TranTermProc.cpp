/////////////////////////////////////////////////////////////////////////////
// TranOutput.cpp : implementation file
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
//	Terminate Function
/////////////////////////////////////////////////////////////////////////////
// Cancel Deposit Reject Procedure(Normal Process)
int	CTranCmn::fnAPP_CancelDepRejectProc(int CancelCode, char *pCancelMsg, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelDepRejectProc:"	\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlag[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlag,
													 m_pDevCmn->TranStatus);

	if (!CancelDepRejectFlag)									// Cancel Deposit Reject Flag
	{
		CancelDepRejectFlag = TRUE;
		if ((TranProc == TRAN_DEP) &&							// Deposit
			(m_pDevCmn->fnAPL_CheckError()))					// No Error Check
		{
			CancelDepRejectProcFlag = TRUE;						// Cancel Deposit Reject Flag
			// Cash Reject Section
		}
	}
	
	fnAPP_CancelProc(CancelCode, pCancelMsg, CancelScrNum);		// Cancel Fucntion
	return T_CANCEL;
}

// Cancel Procedure
int	CTranCmn::fnAPP_CancelProc(int CancelCode, char *pCancelMsg, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc:"				\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlag[%d], "		\
													"m_pDevCmn->TranStatus[%d],"	\
													"CancelScrNum[%d]",	
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlag,
													 m_pDevCmn->TranStatus,
													 CancelScrNum);

	int		DelaySec = 0;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranCode[%d]:",TranCode);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACTranCode[%d]:",ACTranCode);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranWriteFlag[%d]:",TranWriteFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckError[%d]:",m_pDevCmn->fnAPL_CheckError());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->TranStatus[%d]:",m_pDevCmn->TranStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->bAgentMode[%d]:",m_pDevCmn->bAgentMode);

/////////////////////////////////////////////////////////////////////////////
//	Main Call Support
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus == TRAN_IDLE)						// Return Support
		return T_CANCEL;

/////////////////////////////////////////////////////////////////////////////
//	Transaction Procedure (NON-STOP)							// 2006.06.02
/////////////////////////////////////////////////////////////////////////////
//	Device Disable (NON-STOP)									// 2006.06.02
//	Get Error Infomation (NON-STOP)								// 2006.06.02
/////////////////////////////////////////////////////////////////////////////
	if (CancelProcNum == 0)										// First
	{
/////////////////////////////////////////////////////////////////////////////
		if (m_pDevCmn->TranStatus == TRAN_TRAN)					// 2010.10.20
		{
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				if ((TranCode == TC_ICINQUIRY)	||				// 2010.10.20
					(TranCode == TC_ICDETAIL))
					;
				else
				{
					if (TranCode == TC_INQUIRY		||
						TranCode == TC_DETAIL		||
						TranCode == TC_OTHER_INQ	||
						TranCode == TC_WITHDRAWAL	||
						TranCode == TC_TRANSFER		||
						TranCode == TC_HZT			||
						TranCode ==	TC_TZH			||
						TranCode == TC_CHANGEPIN	||
						TranCode == TC_LOAD			||
						TranCode == TC_BOUNDMDOIFY	||
						TranCode == TC_ECHANNEL		||
						TranCode == TC_ECHANNEL_INQ)
					{
						fnAPD_PbcTerminate();						// Pboc Terminate
					}
				}
			}
		}
/////////////////////////////////////////////////////////////////////////////

		if (m_pDevCmn->TranStatus == TRAN_TRAN)					// TRANSACTION : 2004.08.23
		{
			if(m_pDevCmn->bAgentMode == 1 && !m_pDevCmn->fnAPL_CheckError())
			{
				if (TranWriteFlag == FALSE)
				{
					if (TranCode == TC_INQUIRY)
						fnAPP_AgentIdxGetData(AGENT_INQUIRY);
					else
					if (TranCode == TC_INQUIRY_CheckPin)
						fnAPP_AgentIdxGetData(AGENT_INQUIRY_CheckPin);
					else
					if (TranCode == TC_WITHDRAWAL)
						fnAPP_AgentIdxGetData(AGENT_WITHDRAWAL);
					else
					if (TranCode == TC_CHANGEPIN)
						fnAPP_AgentIdxGetData(AGENT_CHANGEPIN);
					else
					if (TranCode == TC_TRANSFER)
						fnAPP_AgentIdxGetData(AGENT_TRANSFER);
					else
					if (TranCode == TC_REMIT_ADD)						// 2009.08.06 yusy
						fnAPP_AgentIdxGetData(AGENT_REMIT_ADD);
					else
					if (TranCode == TC_REMIT_PWD)
						fnAPP_AgentIdxGetData(AGENT_REMIT_PWD);
					else
					if (TranCode == TC_REMIT_IN)
						fnAPP_AgentIdxGetData(AGENT_REMIT_IN);
					else
					if (TranCode == TC_DETAIL)
						fnAPP_AgentIdxGetData(AGENT_DETAIL);
					else
					if (TranCode == TC_PAY)
			    		fnAPP_AgentIdxGetData(AGENT_PAY);
					else
					if (TranCode == TC_HZT)								// added by yusy 2009.05.13
						fnAPP_AgentIdxGetData(AGENT_HZT);
					else
					if (TranCode == TC_TZH)
						fnAPP_AgentIdxGetData(AGENT_TZH);
					else
					if (TranCode == TC_OTHER_INQ)
						fnAPP_AgentIdxGetData(AGENT_OTHER_INQ);
				}

				if (szAgentErrorCode[0] == 'X')
				{
					memcpy(&szAgentErrorCode[0], GetSprintf("%c",m_pDevCmn->CurErrBuff.ProcCount[0]), 1);
					memcpy(&szAgentErrorCode[1], GetSprintf("%-10.10s",GetSprintf("%7.7s",m_pDevCmn->CurErrBuff.ErrorCode)),10);
				}


			}
			

/////////////////////////////////////////////////////////////////////////////
			if (m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_CSH))	// 2008.11.14
			{													// 2005.11.07
				int		DeviceStatusSave = m_pDevCmn->DeviceStatus;
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);
				fnAPD_MoneyCountPrint();						// NON-STOP : 2005.02.24
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);
				m_pDevCmn->fnCSH_CloseShutter();				// Close Shutter
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);	// 2005.05.26
				m_pDevCmn->DeviceStatus = DeviceStatusSave;		// 2005.02.24

			}
/////////////////////////////////////////////////////////////////////////////

			m_pDevCmn->fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);	
																// All Disable
			m_pDevCmn->fnAPL_DeviceSetFlicker(DEV_MAIN, FLICKER_OFF);			
																// All Flicker Off
		}
	}
	else
	{
		if (m_pDevCmn->TranStatus == TRAN_TRAN)					// TRANSACTION : 2004.08.23
		{
			if(m_pDevCmn->bAgentMode == 1 && !m_pDevCmn->fnAPL_CheckError())
			{
				if (szAgentErrorCode[0] == 'X')
				{
					memcpy(&szAgentErrorCode[0], GetSprintf("%c",m_pDevCmn->CurErrBuff.ProcCount[0]), 1);
					memcpy(&szAgentErrorCode[1], GetSprintf("%-10.10s",GetSprintf("%7.7s",m_pDevCmn->CurErrBuff.ErrorCode)),10);
				}
			}
		}
	}

	m_pDevCmn->fnAPL_CheckError();								// Check Error
	if (CancelProcNum == 0)										// First
	{
		if (m_pDevCmn->TranStatus == TRAN_TRAN)					// TRANSACTION
		{
			if (!m_pDevCmn->fnAPL_CheckError())
			{
				if (HostNms.SaveErrCnt == 0)
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR, m_pDevCmn->CurErrBuff.ErrorCode);
					HostNms.SaveErrCnt = m_pDevCmn->CurErrCnt;
					memcpy(&HostNms.SaveErrBuff, &m_pDevCmn->CurErrBuff, sizeof(HostNms.SaveErrBuff));
				}
				else
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR, HostNms.SaveErrBuff.ErrorCode);
				}
			}
			else
			{
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR, "0000000");
				HostNms.SaveErrCnt = 0;
				memset(&HostNms.SaveErrBuff, 0, sizeof(HostNms.SaveErrBuff));
			}
//			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, CancelCode);

			if (TranProc == TRAN_WITH)
			{
				if (!m_pDevCmn->fnAPL_CheckError())
				{
					if (HostNms.SaveErrCnt == 0)
					{
						IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHERROR, m_pDevCmn->CurErrBuff.ErrorCode);
						HostNms.SaveErrCnt = m_pDevCmn->CurErrCnt;
						memcpy(&HostNms.SaveErrBuff, &m_pDevCmn->CurErrBuff, sizeof(HostNms.SaveErrBuff));
					}
					else
						IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHERROR, HostNms.SaveErrBuff.ErrorCode);
				}
				else
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHERROR, "0000000");
					HostNms.SaveErrCnt = 0;
					memset(&HostNms.SaveErrBuff, 0, sizeof(HostNms.SaveErrBuff));
				}
			}
		}
	}
	else														// 2004.07.05
	{
		if (m_pDevCmn->TranStatus == TRAN_TRAN)					// TRANSACTION
		{
			if (!m_pDevCmn->fnAPL_CheckError())
			{
				if (HostNms.SaveErrCnt == 0)
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR, m_pDevCmn->CurErrBuff.ErrorCode);
					HostNms.SaveErrCnt = m_pDevCmn->CurErrCnt;
					memcpy(&HostNms.SaveErrBuff, &m_pDevCmn->CurErrBuff, sizeof(HostNms.SaveErrBuff));
				}
				else
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR, HostNms.SaveErrBuff.ErrorCode);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	Nms Procedure (NON-STOP)
/////////////////////////////////////////////////////////////////////////////
	if (CancelProcNum == 0)
	{
		int		nTranStatus = m_pDevCmn->TranStatus;			// 2006.07.03

		if (nTranStatus == TRAN_ERROR)
		{
			if ((m_pDevCmn->DeviceRetract & DEV_MCU)	||		// Card Retract(Send Transaction)
				(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO).GetLength()))
				;
			else
				nTranStatus = TRAN_MSRSTATUS;					// Send Status
		}

		if ((nTranStatus == TRAN_OPEN)			||
			(nTranStatus == TRAN_CANCEL)		||
			(nTranStatus == TRAN_ERROR)			||
			(nTranStatus == TRAN_TRAN)			||
			(nTranStatus == TRAN_TOTAL)			||
			(nTranStatus == TRAN_TOTALPRE)		||
			(nTranStatus == TRAN_TOTALKIND)		||
			(nTranStatus == TRAN_DOWNECHANNELPARA)||
			(nTranStatus == TRAN_DOWNECHANNELPROT)||
			(nTranStatus == TRAN_ADDCASH)		||
			(nTranStatus == TRAN_SUBCASH)		||
			(nTranStatus == TRAN_CLEARCASH)		||
			(nTranStatus == TRAN_DOWNFIT)		||
			(nTranStatus == TRAN_DOWNICFIT)		||				// added by yusy ic
			(nTranStatus == TRAN_DOWNAIDLIST)	||
			(nTranStatus == TRAN_CONFIRMWITH))
		{
			fnAPL_SendHost();
			fnAPL_WriteDCTransactionEjr();
		}
		else
		if ((nTranStatus == TRAN_AEXSTATUS)		||
			(nTranStatus == TRAN_OEXSTATUS)		||
			(nTranStatus == TRAN_CARDRETAIN)	||
			(nTranStatus == TRAN_MSRSTATUS)		||
			(nTranStatus == TRAN_BIDCOMMAND)	||
			(nTranStatus == TRAN_DCSENDEJR)		||
			(nTranStatus == TRAN_DCEJREND))
		{
			fnAPL_SendHost();
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	Not Transaction Procedure (NON-STOP)						// 2006.06.02
/////////////////////////////////////////////////////////////////////////////
//	Total Support (NON-STOP)									// 2006.06.02
//	Terminate Support (TERMINATE)								// 2006.06.02
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus != TRAN_TRAN)						// Not Transaction
	{
		if ((!m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_MAIN))	&&
			(!m_pDevCmn->fnAPL_CheckError())					&&
			((memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "77", 2) == 0) ||
			 (memcmp(m_pDevCmn->CurErrBuff.ErrorCode, "88", 2) == 0)))
		{
			if (m_pDevCmn->TranStatus == TRAN_TOTAL)			// TOTAL
// 				fnAPP_SumPrintResult((DEV_SPR | DEV_JPR), TRUE);
		    	fnAPP_SumPrint(DEV_JPR, TRUE);                  //不出凭条 by hyl
			else
			if ((m_pDevCmn->TranStatus == TRAN_ADDCASH)		||	// ADDCASH
				(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||	// SUBCASH
				(m_pDevCmn->TranStatus == TRAN_CLEARCASH))		// CLEARCASH
// 				fnAPP_CashPrintResult((DEV_SPR | DEV_JPR), TRUE);
				fnAPP_CashPrintResult( DEV_JPR, TRUE);           //不出凭条 by hyl
		}

		if ((!m_pDevCmn->fnAPL_GetAvailErrorDevice(DEV_MAIN)) &&
			(!m_pDevCmn->fnAPL_CheckError()))
			m_pDevCmn->fnAPL_ClearError();
	}

	if (m_pDevCmn->TranStatus != TRAN_TRAN)						// Not Transaction
	{
		fnAPP_TerminateProc(T_CANCEL);							// Terminate Procedure
	}

/////////////////////////////////////////////////////////////////////////////
//	Limit Check
/////////////////////////////////////////////////////////////////////////////
	switch (CancelProcNum)										// Cancel Process Number
	{
		case	0:
			CancelProcNum = 1;
			CancelProcCount = m_pProfile->TRANS.ProcCount[0];	// Cancel Process
			break;

		case	1:
			CancelProcNum = 2;
			fnAPP_CancelProc2(CancelCode, pCancelMsg, CancelScrNum);	
																// Cancel Fucntion2
			break;

		case	2:
			CancelProcNum = 3;
			fnAPP_CancelProc3(CancelCode, pCancelMsg, CancelScrNum);	
																// Cancel Fucntion3
			break;

		case	3:
			CancelProcNum = 4;
			fnAPP_CancelProc4(CancelCode, pCancelMsg, CancelScrNum);	
																// Cancel Fucntion4
			break;

		case	4:
			CancelProcNum = 5;
			fnAPP_CancelProc5(CancelCode, pCancelMsg, CancelScrNum);	
																// Cancel Fucntion5
			break;

		default:
			if (!m_pDevCmn->fnAPL_CheckError())
			{
				fnAPP_ErrorTranProc();
			}
			fnAPP_TerminateProc(T_CANCEL);						// Terminate Procedure
			break;
	}

/////////////////////////////////////////////////////////////////////////////
//	Screen Display
/////////////////////////////////////////////////////////////////////////////
	if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo()))	||
		((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo()))	||
		((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))
		DelaySec = 1;											// Material Screen Wait Time

	if (CancelScrNum)
	{
		m_pDevCmn->fnSCR_DisplayScreen(CancelScrNum);			// Display Screen
		Delay_Msg(DelaySec * 1000);
	}
	else 
	{
		int		ScrNum = 702;									// Guide Screen
		if ((m_pDevCmn->TranStatus == TRAN_TRAN) && (CancelCode == T_SENDERROR || CancelCode == T_RECVERROR))
		{
			TransCommTimeOut = 1;	

			if(TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)   //可疑交易处理 // liyi add 绿卡通
			{
				fnAPP_SuspiciProc();  
 			}
		}
		switch (CancelCode)
		{
			case	T_EXIT:
//				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[10][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
//				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
//				Delay_Msg(DelaySec * 1000);
				break;

			case	T_CANCEL:
//				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[11][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
//				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
//				Delay_Msg(DelaySec * 1000);
				break;

			case	T_TIMEOVER:
//				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[12][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
//				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
//				Delay_Msg(DelaySec * 1000);
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****OPERATION TIMEOUT****",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****操作超时****",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}

//				m_pDevCmn->fnSCR_DisplayScreen(602);
//				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_INPUTOVER:
				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[13][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				if (TranCode == TC_LOAD)
					ScrNum = 705;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_INPUTERROR:
				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[14][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				if (TranCode == TC_LOAD)
					ScrNum = 705;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_ERROR:
				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[15][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				if (TranCode == TC_LOAD)
					ScrNum = 705;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;

			case	T_SENDERROR:
			case	T_RECVERROR:
				if (((TranProc == TRAN_WITH) || (TranProc == TRAN_TRANS)) && (TranSend))
					m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[17][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				else 
					m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[17][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				if (TranCode == TC_LOAD)
					ScrNum = 705;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_PROGRAM:
				m_pDevCmn->fnSCR_DisplayStringArray(1, TranMsgTbl[18][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				if (TranCode == TC_LOAD)
					ScrNum = 705;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;
			
			case	T_MSG:
				if (pCancelMsg)
					m_pDevCmn->fnSCR_DisplayStringArray(1, pCancelMsg);
				if (TranCode == TC_LOAD)
					ScrNum = 705;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
				Delay_Msg(DelaySec * 1000);
				break;

			default:
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	Send Make
/////////////////////////////////////////////////////////////////////////////
	if (!TranSend)
	{
		fnAPP_SendHostMake(FALSE);								// Send Host Make
	}
	
/////////////////////////////////////////////////////////////////////////////
//	Print/Out
/////////////////////////////////////////////////////////////////////////////
	if (!CancelPrintFlag)										// Cancel Print Flag
	{
		CancelPrintFlag = TRUE;
		if (!m_pDevCmn->fnAPL_CheckError())
		{
			JnlPrintFlag = FALSE;
			fnAPP_PrintError(DEV_JPR);

			if (TranCode == TC_WITHDRAWAL	||
				TranCode == TC_LOAD			||
				TranCode == TC_PAY)									// added by liuxl 20111012
			{
				if (!SlipPrintFlag)									//除取款外的错误交易不打收条   by zjd
				{
					fnAPP_PrintError(DEV_SPR);
					fnAPD_CheckDeviceAction(DEV_SPR);
				}
			}
		}
		else
		{
			JnlPrintFlag = FALSE;
			if ((TranProc == TRAN_DEP) &&
				(!CancelDepPrintFlag))							// Cancel Deposit Print Flag
			{
				CancelDepPrintFlag = TRUE;
				fnAPP_PrintCancel(DEV_JPR);
			}
			else
			if (TranSend)
			{
				fnAPP_PrintCancel(DEV_JPR);
			}
		}
	}
	
	// Withdraw Cash Retract Section
	// Deposit Cash Stack Section

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}

// Cancel Fucntion2
int	CTranCmn::fnAPP_CancelProc2(int CancelCode, char *pCancelMsg, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc2:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlag[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlag,
													 m_pDevCmn->TranStatus);


/////////////////////////////////////////////////////////////////////////////
//	Cancel 
/////////////////////////////////////////////////////////////////////////////
	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// Cancel Procedure3
int	CTranCmn::fnAPP_CancelProc3(int CancelCode, char *pCancelMsg, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc3:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlag[%d], "		\
													"CancelWithProcFlag[%d], "		\
													"m_pDevCmn->TranStatus[%d]",	
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlag,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	After Cancel 
/////////////////////////////////////////////////////////////////////////////
	if (CancelDepRejectProcFlag)								// Cancel Deposit Reject Procedure Flag
	{
		// Material All Out Section
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM, InitInfo.TakeCardTimeOut);
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
	}
	else
	{
		fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM, InitInfo.TakeCardTimeOut);
		fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// Cancel Procedure4
int	CTranCmn::fnAPP_CancelProc4(int CancelCode, char *pCancelMsg, int CancelScrNum)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc4:"			\
													"CancelCode[%d], "				\
													"CancelProcNum[%d], "			\
													"CancelDepRejectFlag[%d], "		\
													"m_pDevCmn->TranStatus[%d]",
													 CancelCode, 
													 CancelProcNum, 
													 CancelDepRejectFlag,
													 m_pDevCmn->TranStatus);

/////////////////////////////////////////////////////////////////////////////
//	Material Retract
/////////////////////////////////////////////////////////////////////////////
	if (CancelDepRejectProcFlag)								// Cancel Deposit Reject Procedure Flag
	{
		if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo())) ||
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo())) ||
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))
		{
			fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM, InitInfo.TakeCardTimeOut);
			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
		}
	}
	else
	{
		if (((m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR)) && (m_pDevCmn->fnSPR_GetMaterialInfo())) ||
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_MCU)) && (m_pDevCmn->fnMCU_GetMaterialInfo())) ||
			((m_pDevCmn->fnAPL_GetAvailDevice(DEV_PBM)) && (m_pDevCmn->fnPBM_GetMaterialInfo())))
		{
			fnAPD_MaterialOut(DEV_SPR | DEV_MCU | DEV_PBM, InitInfo.TakeCardTimeOut);
			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU | DEV_PBM);
		}
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}
	
// Cancel Procedure5
int	CTranCmn::fnAPP_CancelProc5(int CancelCode, char *pCancelMsg, int CancelScrNum)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CancelProc5:"			\
														"CancelCode[%d], "				\
														"CancelProcNum[%d], "			\
														"CancelDepRejectFlag[%d], "		\
														"m_pDevCmn->TranStatus[%d]",
														CancelCode, 
														CancelProcNum, 
														CancelDepRejectFlag,
														m_pDevCmn->TranStatus);
	
	if (HostResp.CardRetractFlag)								// Card Retract Flag : 2004.08.11
		;
	else
	if (MaterialRetractFlag & DEV_MCU)							// Lost Card : 2004.08.11
	{
		m_pDevCmn->fnSCR_DisplayScreen(411);					// Mcu Timeout Retract Guide !!! : 2005.02.28
		Delay_Msg(5000);										// 2004.11.05
		
		if (m_pProfile->NETWORK.InterfaceKind == 18)			// Cup(BJGD) :2006.07.07
			;
		else
		{
			if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM) == 1)
			{
				JnlPrintFlag = FALSE;
				fnAPP_PrintError(DEV_JPR);
//				fnAPP_PrintError(DEV_SPR);
//				fnAPD_CheckDeviceAction(DEV_SPR);
//				fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);
//				fnAPD_MaterialOutCheckRetract(DEV_SPR);
			}
		}
	}

	fnAPP_CancelProc(T_CANCEL);
	return T_CANCEL;
}

// Terminate Procedure
int	CTranCmn::fnAPP_TerminateProc(int TerminateCode)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TerminateProc(TerminateCode[%d])", TerminateCode);

	int		ChkID = DEV_MAIN;
	
	if (m_pDevCmn->TranStatus == TRAN_CANCEL)					// CANCEL
	{															// 2004.08.19
// 		if ((IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1) ||
// 			(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3) ||
// 			(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 5))		// 2008.10.13
		if (CwcInfo.CwcFlag != 0)								// added by liuxl 20110714
		{
//			if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT) >= InitInfo.CwcRetryTime)
			if (CwcInfo.CwcRetryCnt >= InitInfo.CwcRetryTime)		// added by liuxl 20110714
			{
				
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(9);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
				
				fnAPD_CwcInfoSet(0);							// added by liuxl 20110714
				fnAPP_ErrorCancelProc();
			}
		}
	}

	if (m_pDevCmn->TranStatus == TRAN_CARDRETAIN)
	{
		if (CardRetainData.CardRetFlag != 0)
		{
			if (CardRetainData.CardRetCnt >= IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CARDRETAINRTYNUM, 1))
			{				
				/////////////////////////////////////////////////////////////////////////////
//				fnAPL_WriteTransactonEjr(9);					// 2005.05.26
				/////////////////////////////////////////////////////////////////////////////
				fnAPD_CardRetainData(0);
//				fnAPP_ErrorCancelProc();
			}
		}
	}

	if ((m_pDevCmn->TranStatus == TRAN_BIDCOMMAND)	||
		(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
		(m_pDevCmn->TranStatus == TRAN_DETAILEND))
	{
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, "0000");
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBEGINP, "0000");
	}
	
	if (m_pDevCmn->TranStatus == TRAN_DETAILEND)				// 2006.03.21
	{
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, 0);
	}

	if ((m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||			// 2006.07.03
		(m_pDevCmn->TranStatus == TRAN_DCEJREND))
	{
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);
	}

	if (m_pDevCmn->TranStatus == TRAN_TOTAL		||
		m_pDevCmn->TranStatus == TRAN_TOTALPRE)					// TOTAL
	{
// 		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
		
		fnAPD_CwcInfoSet(0);									// added by liuxl 20110714
//		m_pDevCmn->fnAPL_ClearRetractCnt();						// Clear Retract Count
	}


	if (m_pDevCmn->TranStatus == TRAN_ADDCASH)					// ADDCASH
	{
// 		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
//		if (!AddCashFlag)										// 2004.08.23
//			fnAPP_CashProc();									// Non Stop Procedure

		fnAPD_CwcInfoSet(0);									// added by liuxl 20110714
//		m_pDevCmn->fnAPL_ClearRetractCnt();						// Clear Retract Count
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILWRITEP, "0000");

		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, "0000");
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBEGINP, "0000");
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, 0);
	}

	if ((m_pDevCmn->TranStatus == TRAN_SUBCASH)	||				// SUBCASH
		(m_pDevCmn->TranStatus == TRAN_CLEARCASH))				// CLEARCASH
	{
// 		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
//		if (!AddCashFlag)										// 2004.08.23
//			fnAPP_CashProc();									// Non Stop Procedure

		fnAPD_CwcInfoSet(0);									// added by liuxl 20110714
//		m_pDevCmn->fnAPL_ClearRetractCnt();						// Clear Retract Count
	}

	if (m_pDevCmn->TranStatus == TRAN_PRINTCASH)				// PRINTCASH : CUP(TJ) 2005.07.05
	{
//		m_pDevCmn->fnAPL_ClearRetractCnt();						// Clear Retract Count
	}
	
	if (m_pDevCmn->TranStatus == TRAN_TRAN)						// TRANSACTION
	{
		m_pDevCmn->TranProc = TranProc;							// Transaction Procedure Save
		fnAPD_PinEnDisable(DISABLE);							// Pin Disable : 2004.08.23
	}

	ChkID &= m_pDevCmn->fnAPL_GetAvailDevice(ChkID);
	ChkID &= ~DEV_JPR;											// Speed Up : 2004.07.05
	m_pDevCmn->fnAPL_CheckDeviceAction(ChkID);					// All Check Device Action

	if (TerminateCode == T_OK)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Throw CTranCmnExit(TRUE)");
		throw CTranCmnExit(TRUE);								// TRUE
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Throw CTranCmnExit(FALSE)");
		throw CTranCmnExit(FALSE);								// FALSE
	}

	return TerminateCode;
}

