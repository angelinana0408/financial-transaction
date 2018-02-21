   /////////////////////////////////////////////////////////////////////////////
// TranMain.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include <wininet.h>

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
//	Main Function
/////////////////////////////////////////////////////////////////////////////
// Main Open Procedure
int	CTranCmn::fnAPP_MainOpenProc()
{
	if (m_pDevCmn->fnAPL_CheckHostOpen())						// 2006.08.31
	{
		if (HostNms.CwcSendFlag)
			return FALSE;
		else
			return TRUE;
	}

	if (m_pDevCmn->Timer.HostOpenRetryTime)
		return FALSE;

	if (m_pDevCmn->HostStatus == HOST_WAITREADY)				// Wait Ready
	{
		m_pDevCmn->HostOpenFlag = FALSE;						// 2004.06.30
		if (fnAPP_MainTranProc(TRAN_READY))
		{
			m_pDevCmn->HostStatus = HOST_WAITONLINE;			// 2004.06.22
		}
		else
		{
			m_pDevCmn->HostStatus = HOST_WAITREADY;
			m_pDevCmn->HostOpenRetryCnt++;						// 2006.03.21
			if (m_pDevCmn->HostOpenRetryCnt < 3)				// 2005.08.22
				m_pDevCmn->Timer.HostOpenRetryTime = 10;		// 10 Sec : 2004.08.30
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = InitInfo.OpenRetryTime;
		}
	}

	if (m_pDevCmn->HostStatus == HOST_WAITONLINE)				// Wait Online
	{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MainOpenProc() TRAN_MSRSTATUS");
//		fnAPP_MainTranProc(TRAN_MSRSTATUS);						// 为了进行相关参数的下载先发送一个MSR包  by zjd
//		if (DownloadParmFlag || DownloadFITFlag || DownloadPayFlag)
//			return FALSE;										// 2009.03.18
		
		if (m_pDevCmn->HostOpenFlag)							// 2004.06.22
		{
			if (HostNms.CwcSendFlag)							// 2004.07.05
				m_pDevCmn->Timer.HostOpenRetryTime = 0;			// 0 Sec
			else
			if (
				//(HostNms.OexSendFlag)					||		// 060411 
				(HostNms.AexCustomerCardTimeOutSendFlag)||
				(HostNms.AexCustomerCashTimeOutSendFlag)||
				(HostNms.AexCustomerHostRetractSendFlag)||
				(HostNms.OexBothDenoNotesSendFlag)		||
				(HostNms.OexPowerOnSendFlag)			||
				(HostNms.OexCloseBySpvSendFlag)			||
				(HostNms.OexOpenBySpvSendFlag)			||
				(HostNms.OexSlipPaperOutSendFlag)		||
				(HostNms.OexJnlPaperOutSendFlag)		||
				(HostNms.OexDoorOpenErrorFlag)			||		// 2006.03.21
				(HostNms.OexDoorOpenSendFlag)			||
				(HostNms.OexServiceModeSendFlag))
				m_pDevCmn->Timer.HostOpenRetryTime = 0;			// 0 Sec
			else
			{
				m_pDevCmn->HostStatus = HOST_ONLINE;
				m_pDevCmn->HostOpenRetryCnt = 0;				// Clear
				m_pDevCmn->Timer.HostOpenRetryTime = 0;			// Clear
			}
		}
		else
		if (fnAPP_MainTranProc(TRAN_OPEN))
		{														// Host Ok
/////////////////////////////////////////////////////////////////////////////
			m_pDevCmn->HostOpenFlag = TRUE;						// Setting TRUE
			HostNms.FirstOpenFlag = TRUE;						// Setting TRUE
			HostNms.OpenFlag = TRUE;							// Setting TRUE

			nAgentAtmAfterOpenFistFailFlag = TRUE;                   //20080415 by hyl

			m_pDevCmn->HostOpenRetryCnt = 0;					// Clear
			m_pDevCmn->Timer.HostOpenRetryTime = 0;				// Clear
/////////////////////////////////////////////////////////////////////////////
		}
		else 
		{														// Host Ng
			m_pDevCmn->HostStatus = HOST_WAITONLINE;
			m_pDevCmn->HostOpenRetryCnt++;						// 2006.03.21
			if (m_pDevCmn->HostOpenRetryCnt < 3)				// 2005.08.22
				m_pDevCmn->Timer.HostOpenRetryTime = 10;		// 10 Sec : 2004.08.30
			else 
				m_pDevCmn->Timer.HostOpenRetryTime = InitInfo.OpenRetryTime;
		}
	}

	if (m_pDevCmn->fnAPL_CheckHostOpen())						// 2006.08.31
	{
		m_pDevCmn->HostOpenRetryCnt = 0;						// Clear
		m_pDevCmn->Timer.HostOpenRetryTime = 0;					// Clear

		if (HostNms.CwcSendFlag)
			return FALSE;
		else
			return TRUE;
	}

	return FALSE;
}	

// Main Transaction Procedure
int	CTranCmn::fnAPP_MainTranProc(int TranValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MainTranProc(%d)", TranValue);

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Op.TransationStatus = "ON";						// TransationStatus(ON)
	m_pDevCmn->fnAPL_ClerkInformation();						// Clerk Information
/////////////////////////////////////////////////////////////////////////////
	
	m_pDevCmn->AtmStatusReEntryFlag = TRUE;						// Default TRUE
	m_pDevCmn->TranResult = FALSE;								// Default FALSE
	m_pDevCmn->TranStatus = TranValue;

	switch (m_pDevCmn->TranStatus) 
	{
		case TRAN_IDLE:											// IDLE
			m_pDevCmn->TranStatusSave = TRAN_IDLE;
			break;

		case TRAN_READY:										// READY
		case TRAN_OPEN:											// OPEN
		case TRAN_CANCEL:										// CANCEL 
		case TRAN_CARDRETAIN:									// CARDRETAIN
		case TRAN_ERROR:										// ERROR 
		case TRAN_AEXSTATUS:									// AEXSTATUS 
		case TRAN_OEXSTATUS:									// OEXSTATUS 
		case TRAN_MSRSTATUS:									// MSRSTATUS 
		case TRAN_BIDCOMMAND:									// BIDCOMMAND
		case TRAN_TRAN:											// TRANSACTION
		case TRAN_TOTAL:										// TOTAL
		case TRAN_TOTALPRE:										// TOTALPRE
		case TRAN_TOTALKIND:									// TOTALKIND
		case TRAN_SUBTOTAL:										// SUBTOAL
		case TRAN_ADDCASH:										// ADDCASH
		case TRAN_SUBCASH:										// SUBCASH
		case TRAN_CLEARCASH:									// CLEARCASH
		case TRAN_PRINTCASH:									// PRINTCASH
		case TRAN_DOWNFIT:										// DOWNFIT
		case TRAN_DOWNICFIT:									// DOWNICFIT added by yusy ic
		case TRAN_DOWNAIDLIST:
		case TRAN_DOWNPBOCPARA:
		case TRAN_DOWNECHANNELPARA:
		case TRAN_DOWNECHANNELPROT:
		case TRAN_SCRIPT:
		case TRAN_DOWNPARM:										// DOWNPARM
		case TRAN_DOWNPAY:										// DOWNPAY
		case TRAN_SENDDETAIL:									// SENDDETAIL
		case TRAN_DETAILEND:									// DETAILEND
		case TRAN_CONFIRMWITH:									// CONFIRMWITH
		case TRAN_DCSENDEJR:									// DCSENDEJR : 2006.07.03
		case TRAN_DCEJREND:										// DCEJREND
		case TRAN_INITCDK:										// INIT CDK  by zjd
		case TRAN_INITMAC:										// INIT MAC  by zjd
		case TRAN_INITPIN:										// INIT PIN  by zjd
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranStatus[%d]", m_pDevCmn->TranStatus);
			m_pDevCmn->TranStatusSave = m_pDevCmn->TranStatus;

			fnAPP_TranVirtualMainProc();
			break;

		default:
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MainTranProc(...):return(%d)", m_pDevCmn->TranResult);
	return m_pDevCmn->TranResult;
}

// Virtual Main Procedure
int CTranCmn::fnAPP_TranVirtualMainProc()
{
	try 
	{
		fnAPP_TranMainProc();
	}

	catch (CTranCmnExit& e)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Catch CTranCmnExit(%d)", e.TranExitResult);
		e.TranExitResult = e.TranExitResult;
	}
// 	catch (...)
// 	{
// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Catch ...");
// 	}

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranVirtualMainProc():return");

	return T_OK;
}

// Main Procedure
int CTranCmn::fnAPP_TranMainProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMainProc:m_pDevCmn->TranStatus[%d]", m_pDevCmn->TranStatus);

	switch (m_pDevCmn->TranStatus) 
	{
		case TRAN_READY:										// READY
			fnAPP_TranReadyProc();
			break;

		case TRAN_OPEN:											// OPEN 
			fnAPP_TranOpenProc();
			break;

		case TRAN_CANCEL:										// CANCEL 
			fnAPP_TranCancelProc();
			break;

		case TRAN_ERROR:										// ERROR 
			fnAPP_TranErrorProc();
			break;

		case TRAN_AEXSTATUS:									// AEXSTATUS 
			while ((HostNms.AexCustomerCardTimeOutSendFlag)	||
				   (HostNms.AexCustomerHostRetractSendFlag) ||
				   (HostNms.AexCustomerCashTimeOutSendFlag))
				fnAPP_TranAexStatusProc();
			break;

		case TRAN_CARDRETAIN:									// CARDRETAIN
			fnAPP_TranCardRetainProc();
			break;

		case TRAN_OEXSTATUS:									// OEXSTATUS 
			while ((HostNms.OexBothDenoNotesSendFlag)	||
				   (HostNms.OexPowerOnSendFlag)			||
				   (HostNms.OexCloseBySpvSendFlag)		||
				   (HostNms.OexOpenBySpvSendFlag)		||
				   (HostNms.OexSlipPaperOutSendFlag)	||
				   (HostNms.OexJnlPaperOutSendFlag)		||
				   (HostNms.OexDoorOpenErrorFlag)		||		// 2006.03.21
				   (HostNms.OexDoorOpenSendFlag)		||
				   (HostNms.OexServiceModeSendFlag))
				fnAPP_TranOexStatusProc();
			break;

		case TRAN_MSRSTATUS:									// MSRSTATUS 
			fnAPP_TranMsrStatusProc();
			break;

		case TRAN_SCRIPT:
			fnAPP_TranScriptProc();
			break;

		case TRAN_BIDCOMMAND:									// BIDCOMMAND
			fnAPP_TranBidResponseProc();
			break;

		case TRAN_TRAN:											// TRANSACTION
			fnAPPM_TranTranProc();								// 2004.11.05
			break;

		case TRAN_TOTAL:										// TOTAL
			fnAPP_TranTotalProc();
			break;

		case TRAN_TOTALPRE:										// TOTALPRE
			fnAPP_TranTotalPreProc();
			break;

		case TRAN_TOTALKIND:									// TOTALKIND
			fnAPP_TranTotalKindProc();
			break;

		case TRAN_SUBTOTAL:										// SUBTOAL
			fnAPP_TranSubTotalProc();
			break;

		case TRAN_ADDCASH:										// ADDCASH
			fnAPP_TranAddCashProc();
			break;

		case TRAN_SUBCASH:										// SUBCASH
			fnAPP_TranSubCashProc();
			break;

		case TRAN_CLEARCASH:									// CLEARCASH
			fnAPP_TranClearCashProc();
			break;

		case TRAN_PRINTCASH:									// PRINTCASH
			fnAPP_TranPrintCashProc();
			break;

		case TRAN_DOWNFIT:										// DOWNFIT
			fnAPP_TranDownFitProc();
			break;

		case TRAN_DOWNICFIT:									// DOWNICFIT added by yusy ic
			fnAPP_TranDownICFitProc();
			break;

		case TRAN_DOWNAIDLIST:
			fnAPP_TranDownAIDListProc();
			break;

		case TRAN_DOWNPBOCPARA:
			fnAPP_TranDownPBOCParaProc();
			break;

		case TRAN_DOWNECHANNELPARA:
			fnAPP_TranDownEchannelParaProc();
			break;

		case TRAN_DOWNECHANNELPROT:
			fnAPP_TranDownEchannelProtProc();
			break;

		case TRAN_SENDDETAIL:									// SENDDETAIL
			fnAPP_TranSendDetailProc();
			break;

		case TRAN_DETAILEND:									// DETAILEND
			fnAPP_TranDetailEndProc();
			break;

		case TRAN_CONFIRMWITH:									// CONFIRMWITH
			break;
			
		case TRAN_DCSENDEJR:									// DCSENDEJR : 2006.07.03
			fnAPP_TranDCSendEjrProc();
			break;
			
		case TRAN_DCEJREND:										// DCEJREND	: 2006.07.03
			fnAPP_TranDCEjrEndProc();
			break;
			
		case TRAN_DOWNPARM:										// DOWNFIT
			fnAPP_TranDownParmProc();
			break;

		case TRAN_DOWNPAY:										// DOWNFIT
			fnAPP_TranDownPayProc();
			break;

		case TRAN_INITCDK:										// INIT CDK  by zjd
			fnAPP_TranInitCDKProc();
			break;

		case TRAN_INITMAC:										// INIT MAC  by zjd
			fnAPP_TranInitMACProc();
			break;

		case TRAN_INITPIN:										// INIT PIN  by zjd
			fnAPP_TranInitPINProc();
			break;

		default:
			break;
	}

	fnAPP_TerminateProc(T_OK);									// Normal Terminate
	return T_OK;
}

// Ready Procedure
int CTranCmn::fnAPP_TranReadyProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranReadyProc()");

	fnAPP_InitializeDS();
	TranCode = TC_MSRSTATUS;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

	if (DownloadParmFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNPARM);
	}

	if (DownloadFITFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNFIT);
	}

	if (DownloadICFITFlag)										// added by yusy ic
	{
		fnAPP_MainTranProc(TRAN_DOWNICFIT);
	}

	if (DownloadAIDListFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNAIDLIST);
	}	

	if (DownloadEchannelParaFlag)
	{
		DownloadEchannelParaFlag = 0;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadEchannelParaFlag[%d]", DownloadEchannelParaFlag);
		fnAPP_MainTranProc(TRAN_DOWNECHANNELPARA);
//		if (m_pDevCmn->TranResult)
//			fnAPP_MainTranProc(TRAN_DOWNECHANNELPROT);
	}

	if (DownloadPBOCParaFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNPBOCPARA);
	}

	if (DownloadPayFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNPAY);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranReadyProc():return");
	return T_OK;
}

// Open Procedure
int CTranCmn::fnAPP_TranOpenProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOpenProc()");

	// added by liuxl 20110622
	int keyflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_SWITCHCDKFLAG, 0);
	if (keyflag && m_pDevCmn->bFirstStartRQK == FALSE)
	{
		m_pDevCmn->bFirstStartRQK = TRUE;			// added by liuxl 20110622
		fnAPP_InitializeDS();
		TranCode = TC_OPEN;
		TranCode2 = 0;

		if (hy_CheckAuthorithCodeFromFile3() == AUTH_FAIL)          // 授权移至OPEN
		{
			m_pDevCmn->TranResult = FALSE;
			m_pDevCmn->fnAPL_SetProcCount('9');
			return T_OK;
		}
		else
		{
			m_pDevCmn->TranResult = TRUE;
			m_pDevCmn->fnAPL_SetProcCount('5');					// Host Ok
			fnAPP_OpenProc();
			fnAPL_SendHost();										// 2006.07.03
			fnAPL_WriteDCTransactionEjr();
			fnAPP_OpenPrintResult(DEV_JPR, TRUE);
		}
		return T_OK;											// 不是维护需要，而且不是启动后第一次，就不再申请密钥了  by zjd
	}

	fnAPP_InitializeDS();
	TranCode = TC_OPEN;
	TranCode2 = 0;
	RQKCode = 0;												//CDK   by zjd

    if (hy_CheckAuthorithCodeFromFile3() == AUTH_FAIL)          // 授权移至OPEN
	{
		m_pDevCmn->TranResult = FALSE;
		m_pDevCmn->fnAPL_SetProcCount('9');
		return T_OK;
	}

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

	if (m_pDevCmn->bAgentMode == 1)								// added by yusy 08.09.09
	{
		if (m_pDevCmn->TranResult)								// added by yusy 08.09.09
			fnAPP_AgentIdxGetData(AGENT_OPEN);
		else
			fnAPP_AgentIdxGetData(AGENT_OPENFAIL);
	}

	if (m_pDevCmn->TranResult)
	{
		fnAPP_OpenPrintResult(DEV_JPR, TRUE);
		RQKCode = 2;												//PINKEY   by zjd

		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
		if (m_pDevCmn->TranResult)
		{
			fnAPP_OpenPrintResult(DEV_JPR, TRUE);
			RQKCode = 1;											//MACKEY   by zjd

			m_pDevCmn->fnAPL_SetProcCount('3');
			fnAPP_SendHost();
			fnAPP_RecvHost();
			if (m_pDevCmn->TranResult)
				m_pDevCmn->fnAPL_SetProcCount('5');					// Host Ok
			else m_pDevCmn->fnAPL_SetProcCount('9');				// Host Ng
			if (m_pDevCmn->TranResult)
			{
				fnAPP_OpenProc();
/////////////////////////////////////////////////////////////////////////////
				fnAPL_SendHost();										// 2006.07.03
				fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////
				if (m_pDevCmn->TranResult)								// 2005.07.22
					fnAPP_OpenPrintResult(DEV_JPR, TRUE);
			}
		}

////////////////////////////////////////////////////////////////////////////
//AGNET 签到时处理 2008.03.07 by hyl
		_mkdir(_AGENT_PATH);
 		IniSetStr(_AGENT_ATMS,_AGENT_ATM_AP,"BRANCH",GetSprintf("0100A%3.3sA%3.3sA%3.3s",&m_pProfile->NETWORK.BranchNum[0],&m_pProfile->NETWORK.BranchNum[3],&m_pProfile->NETWORK.BranchNum[6]));
        IniSetStr(_AGENT_ATMS,_AGENT_ATM_AP,"TERMINAL",m_pProfile->NETWORK.AtmSerialNum);
        IniSetStr(_AGENT_ATMS,_AGENT_ATM_AP,"MACHINENO",m_pProfile->NETWORK.AtmSerialNum);
		if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))
		{
			IniSetInt(_AGENT_ATMS,_AGENT_ATM_AP,"JRNLOPERMODE",2);
		}
		else
		{
			IniSetInt(_AGENT_ATMS,_AGENT_ATM_AP,"JRNLOPERMODE",3);
		}
        
        IniSetInt(_AGENT_ATMS,_AGENT_ATM_AP,"MONITOR_StartX",0);
        IniSetInt(_AGENT_ATMS,_AGENT_ATM_AP,"MONITOR_StartY",0);
        IniSetInt(_AGENT_ATMS,_AGENT_ATM_AP,"MONITOR_WIDTTH",800);
        IniSetInt(_AGENT_ATMS,_AGENT_ATM_AP,"MONITOR_HEIGHT",600);

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::_AGENT_ATMS");


		char strTo[256];
		char strFrom[256];
		CString szOldJrnlDir("");
		CString szJrnlDir("");
		SHFILEOPSTRUCT   FileOp;
		szJrnlDir = GetSprintf("%s",IniGetStr(_AGENT_ATMS,_AGENT_COMMON,"JRNLDIR","D:\\ATMS"));
		szOldJrnlDir = GetSprintf("%s",IniGetStr(_AGENT_ATMS,_AGENT_COMMON,"OLDJRNLDIR",szJrnlDir));
		
		memcpy(&strTo,szJrnlDir,szJrnlDir.GetLength());
        

        if(szJrnlDir != szOldJrnlDir)
		{
			IniSetStr(_AGENT_ATMS,_AGENT_COMMON,"OLDJRNLDIR",szJrnlDir);
            szOldJrnlDir.TrimRight(); 
			szOldJrnlDir.TrimRight(_T('\\')); 
			szOldJrnlDir += _T("\\*.*"); 

			szJrnlDir.TrimRight(); 
			szJrnlDir.TrimRight(_T('\\')); 
			szJrnlDir += _T("\\"); 	

			CFileFind ff; 
			BOOL bWorking = ff.FindFile(szOldJrnlDir); 

			while( bWorking ) 
			{ 
				
				bWorking = ff.FindNextFile();
				if(ff.IsDots())
				{
					continue;
					
				} 
				else	
				{
					if(ff.IsDirectory())
					{
						// Copy file 
						
     					memcpy(&strFrom,ff.GetFilePath(),ff.GetFilePath().GetLength());
				        FileOp.pFrom = strFrom;
						FileOp.pTo = strTo;	
						
						FileOp.wFunc = FO_MOVE;
						FileOp.fFlags= FOF_SILENT|FOF_NOCONFIRMATION|FOF_SILENT;
						
						if(!SHFileOperation(&FileOp))
						{	
                            MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::AGENT JRNLDIR CHANGE LOG [%s] MOVE PATH TO [%s] SUCCESS!",strFrom,strTo);
						}
						else 
						{
		                	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::AGENT JRNLDIR CHANGE LOG [%s] MOVE PATH TO [%s] FAIL!",strFrom,strTo);
						}
					}
					else
					{
					    if (!CopyFile(ff.GetFilePath(), szJrnlDir + ff.GetFileName(), FALSE)) 
	               		    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::AGENT JRNLDIR CHANGE LOG [%s] MOVE FILE TO [%s] FAIL!",ff.GetFilePath(),szJrnlDir+ff.GetFileName());
						else
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::AGENT JRNLDIR CHANGE LOG [%s] MOVE FILE TO [%s] SUCCESS!",ff.GetFilePath(),szJrnlDir+ff.GetFileName());
						} 	
					}
					
	
				}
				
			}
			
		}
		
		
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOpenProc():return");
	return T_OK;
}

// INIT CDK Procedure
int CTranCmn::fnAPP_TranInitCDKProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranInitCDKProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_OPEN;
	TranCode2 = 0;
	RQKCode = 0;												//CDK   by zjd
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranInitCDKProc():return");
	return T_OK;
}

// INIT MAC Procedure
int CTranCmn::fnAPP_TranInitMACProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranInitMACProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_OPEN;
	TranCode2 = 0;
	RQKCode = 1;												//CDK   by zjd
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranInitMACProc():return");
	return T_OK;
}

// INIT PIN Procedure
int CTranCmn::fnAPP_TranInitPINProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranInitPINProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_OPEN;
	TranCode2 = 0;
	RQKCode = 2;												//CDK   by zjd
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranInitPINProc():return");
	return T_OK;
}

// Cancel Procedure
int CTranCmn::fnAPP_TranCancelProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCancelProc()");

	fnAPP_InitializeDS();
	TranCode = TC_CANCEL;
//	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1)
	if (CwcInfo.CwcFlag == 1)		// added by liuxl 20110714
		TranCode2 = 0;				//取款冲正   by liyi
	else
//	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3)
	if (CwcInfo.CwcFlag == 3)
		TranCode2 = 2;				//汇兑异常通知   by liyi
 	else
// 	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 5)
	if (CwcInfo.CwcFlag == 5)
 		TranCode2 = 3;				//指定账户圈存冲正 added by yusy ic

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();

//	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT) == 0)
	if (CwcInfo.CwcRetryCnt == 0)	// added by liuxl 20110714
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(7);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
	}
	
/////////////////////////////////////////////////////////////////////////////
// 	IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT, 
// 			IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT) + 1);
	CwcInfo.CwcRetryCnt++;	// added by liuxl 20110714
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "_INIKEY_TRANSACTION_CWCCOUNT[%d]", IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT));
/////////////////////////////////////////////////////////////////////////////

	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

	if (m_pDevCmn->TranResult)									// Host Ok
	{	
		if (TranCode2 == 0)										// 2008.10.13
		{
/////////////////////////////////////////////////////////////////////////////
			fnAPL_WriteTransactonEjr(8);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
//			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
			
			fnAPD_CwcInfoSet(0);		// added by liuxl 20110714
			if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSUM) == 0)
			{
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSUM, 1);
				fnAPP_SumProc(0);									// Sum Procedure(Host)
			}
			fnAPP_SumProc(11);										// Sum Procedure(Cancel Withdraw)
			fnAPP_ErrorCancelProc();
/////////////////////////////////////////////////////////////////////////////
		}
		else
		if (TranCode2 == 2 || TranCode2 == 3)
		{
//			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
			
			fnAPD_CwcInfoSet(0);		// added by liuxl 20110714
			fnAPP_ErrorCancelProc();
		}

	}
	else														// Host Ng
	{
		fnAPD_CwcInfoSet(0);		// added by liuxl 20110715
	}
	
/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

	fnAPP_PSPProc(DEV_JPR);										// Jpr Print : 2004.06.30

	if(m_pDevCmn->TranResult && m_pDevCmn->bAgentMode ==1)		// added by yusy 08.09.09
	{
		fnAPP_AgentIdxGetData(AGENT_CANCEL);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCancelProc():return");
	return T_OK;
}

// Error Procedure
int CTranCmn::fnAPP_TranErrorProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranErrorProc()");


	fnAPP_InitializeDS();
	TranCode = TC_ERROR;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranErrorProc():return");
	return T_OK;
}

// Aex Status Procedure
int CTranCmn::fnAPP_TranAexStatusProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAexStatusProc()");

	fnAPP_InitializeDS(TRUE);
	TranCode = TC_AEXSTATUS;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAexStatusProc():return");
	return T_OK;
}

// Card Retain Procedure
int CTranCmn::fnAPP_TranCardRetainProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCardRetainProc()");

	fnAPP_InitializeDS();
	TranCode = TC_CARDRETAIN;
	TranCode2 = 0;

	CardRetainData.CardRetCnt++;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

	if (m_pDevCmn->TranResult)
		fnAPD_CardRetainData(0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranCardRetainProc():return");
	return T_OK;
}

// Oex Status Procedure
int CTranCmn::fnAPP_TranOexStatusProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOexStatusProc()");

	fnAPP_InitializeDS();
	TranCode = TC_OEXSTATUS;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranOexStatusProc():return");
	return T_OK;
}

int CTranCmn::fnAPP_TranScriptProc()
{
	int len = 0;

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranScriptProc()");

	fnAPP_InitializeDS();
	TranCode = TC_SCRIPT;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	m_pDevCmn->fnAPL_SetProcCount('5');

	len = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM"));
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", len), "");
	len -= 1;
	IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM", GetSprintf("%d", len));

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranScriptProc():return");
	return T_OK;
}

// Msr Status Procedure
int CTranCmn::fnAPP_TranMsrStatusProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc()");

	fnAPP_InitializeDS();
	TranCode = TC_MSRSTATUS;
	TranCode2 = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexOpenBySpvSendFlag[%d]", HostNms.OexOpenBySpvSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexDoorOpenErrorFlag[%d]", HostNms.OexDoorOpenErrorFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexDoorOpenSendFlag[%d]", HostNms.OexDoorOpenSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexServiceModeSendFlag[%d]", HostNms.OexServiceModeSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexBothDenoNotesSendFlag[%d]", HostNms.OexBothDenoNotesSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexPowerOnSendFlag[%d]", HostNms.OexPowerOnSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexCloseBySpvSendFlag[%d]", HostNms.OexCloseBySpvSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexSlipPaperOutSendFlag[%d]", HostNms.OexSlipPaperOutSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.OexJnlPaperOutSendFlag[%d]", HostNms.OexJnlPaperOutSendFlag);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc() HostNms.MsrSendFlag[%d]", HostNms.MsrSendFlag);

	HostNms.OexOpenBySpvSendFlag = FALSE;
	HostNms.OexDoorOpenErrorFlag = FALSE;
	HostNms.OexDoorOpenSendFlag = FALSE;
	HostNms.OexServiceModeSendFlag = FALSE;
	HostNms.OexBothDenoNotesSendFlag = FALSE;
	HostNms.OexPowerOnSendFlag = FALSE;
	HostNms.OexCloseBySpvSendFlag = FALSE;
	HostNms.OexOpenBySpvSendFlag = FALSE;
	HostNms.OexSlipPaperOutSendFlag = FALSE;
	HostNms.OexJnlPaperOutSendFlag = FALSE;
	HostNms.MsrSendFlag = FALSE;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

	if (DownloadParmFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNPARM);
	}

	if (DownloadFITFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNFIT);
	}

	if (DownloadICFITFlag)										// added by yusy ic
	{
		fnAPP_MainTranProc(TRAN_DOWNICFIT);
	}

	if (DownloadAIDListFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNAIDLIST);
	}	

	if (DownloadPBOCParaFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNPBOCPARA);
	}	

	if (DownloadPayFlag)
	{
		fnAPP_MainTranProc(TRAN_DOWNPAY);
	}

	if (DownloadEchannelParaFlag)
	{
		DownloadEchannelParaFlag = 0;
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadEchannelParaFlag[%d]", DownloadEchannelParaFlag);
		fnAPP_MainTranProc(TRAN_DOWNECHANNELPARA);
//		if (m_pDevCmn->TranResult)
//			fnAPP_MainTranProc(TRAN_DOWNECHANNELPROT);
	}

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranMsrStatusProc():return");
	return T_OK;
}

// Bid Response Procedure
int CTranCmn::fnAPP_TranBidResponseProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranBidResponseProc()");

	fnAPP_InitializeDS();

	if (m_pProfile->DEVICE.NmsDevice == 2)						// 2006.07.03
	{
		fnAPP_RecvAnalDCBidCommand();

		if ((TranCode == TC_ATMOPENDC)			||
			(TranCode == TC_ATMCLOSEDC)			||
			(TranCode == TC_ATMSTATUSDC)		||
			(TranCode == TC_ATMBIDDCSENDEJR)	||
			(TranCode == TC_ATMOPENSYNC)		||
			(TranCode == TC_ATMCLOSESYNC)		||
			(TranCode == TC_ATMSTATUSSYNC))
		{
			if ((TranCode == TC_ATMOPENDC)		||
				(TranCode == TC_ATMOPENSYNC))
			{
				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
					m_pDevCmn->Timer.HostWaitLinkTime = 0;		// BCTC : 2005.09.01
					m_pDevCmn->HostOpenRetryCnt = 0;			// Clear
					m_pDevCmn->Timer.HostOpenRetryTime = 0;		// Clear
					m_pDevCmn->fnAPL_CloseHost();				// 2005.09.11 : DO NOT MODIFY
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
				}
			}
			else
			if ((TranCode == TC_ATMCLOSEDC)	||
				(TranCode == TC_ATMCLOSESYNC))
			{
				m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;				// 2005.09.11
			}
			else
			if ((TranCode == TC_ATMSTATUSDC)	||
				(TranCode == TC_ATMSTATUSSYNC))
				;
			else
			if (TranCode == TC_ATMBIDDCSENDEJR)
			{
				fnAPP_TranAtmBidDCSendEjrProc();
				return T_OK;
			}
			else
				return T_OK;

			if ((TranCode == TC_ATMOPENDC)	||
				(TranCode == TC_ATMCLOSEDC)	||
				(TranCode == TC_ATMSTATUSDC))
			{
				fnAPL_SendHost();
			}
			else
			{
				fnAPL_SendMake();
				fnAPP_NmsSendData(NmsSendBuffer, NmsSendLength, LINE_BID_SEND_TIME);
			}
		}
	}
	else
	{
		fnAPP_RecvAnalBidCommand();

		if ((TranCode == TC_ATMOPEN)			||
			(TranCode == TC_ATMCLOSE)			||
			(TranCode == TC_ATMSTATUS)			||
			(TranCode == TC_ATMRESETALL)		||
			(TranCode == TC_ATMREBOOT)			||
			(TranCode == TC_ATMBIDADDOWNLOAD)	||
			(TranCode == TC_ATMBIDSENDDETAIL)	||
			(TranCode == TC_ATMBIDSOFTUPDATE)	||
			(TranCode == TC_ATMWORKPARAM)		||
			(TranCode == TC_ATMFTPSEND)			||
			(TranCode == TC_ATMRESETDVR))
		{
//			if (IsNum(HMERecv.Data[11], 8))
//			{
//				m_pDevCmn->fnAPL_SetSerialNo(HMERecv.Data[11]);	// 2005.08.22
//			}
//
			if (TranCode == TC_ATMOPEN)
			{
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  OPEN ATM",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  逻辑开机",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
				}

				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
					m_pDevCmn->Timer.HostWaitLinkTime = 0;		// BCTC : 2005.09.01
					m_pDevCmn->HostOpenRetryCnt = 0;			// Clear
					m_pDevCmn->Timer.HostOpenRetryTime = 0;		// Clear
					m_pDevCmn->fnAPL_CloseHost();				// 2005.09.11 : DO NOT MODIFY
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
				}
			}
			else
			if (TranCode == TC_ATMCLOSE)
			{
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  CLOSE ATM",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  逻辑关机",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
				}

				m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;				// 2005.09.11
			}
			else
			if (TranCode == TC_ATMREBOOT)
			{
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  RESTART ATM",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  热启动",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4])));
				}

				HostNms.RebootFlag = TRUE;
				m_pDevCmn->fnAPL_SetClerkPowerOff();
			}
			else
			if (TranCode == TC_ATMRESETALL)
			{
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  RESET ATM",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  强制检测",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}

				HostNms.HostResetAll = TRUE;
			}
			else
			if (TranCode == TC_ATMSTATUS)
			{
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  GET ATM STATUS",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  运行状态检测",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				}

			}
			else
			if (TranCode == TC_ATMBIDADDOWNLOAD)				
			{	
                if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM Download PIC INFO",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 广告画面、公告下发通知",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
				}

			    m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;
				m_pDevCmn->fnSCR_DisplayScreen(27);							// Stop Service
	            m_pDevCmn->fnAPL_UnUsed();
 			 	
				fnAPP_TranAtmBidAdDownloadProc();

				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
		
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
				}
				
				return T_OK;		
			}

			else
			if (TranCode == TC_ATMBIDSENDDETAIL)				// 提取远程流水管理命令    需要修改   by zjd
			{   
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM UPLOAD JOURNAL INFO",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 提取远程流水通知",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
				}

				m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;
				m_pDevCmn->fnSCR_DisplayScreen(27);							// Stop Service
	            m_pDevCmn->fnAPL_UnUsed();

				fnAPP_TranAtmBidSendDetailProc();

				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
				}	
				return T_OK;
			}
			else
			if (TranCode == TC_ATMBIDSOFTUPDATE)				
			{	
				if (m_pTranCmn->LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM UPDATE SOFTWARE INFO",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 软件更新通知",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])));
				}

				m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;
				m_pDevCmn->fnSCR_DisplayScreen(27);							// Stop Service
	            m_pDevCmn->fnAPL_UnUsed();
									 		
				fnAPP_TranAtmBidSoftUpdateProc();

				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
				}
				
				return T_OK;
			}						
			
			else
			if (TranCode == TC_ATMWORKPARAM)
				fnAPL_BidSetWorkParamProc();
			else
			if (m_pProfile->DEVICE.NmsDevice == 1)				// ZL NMS : 2006.04.19
			{
				if (TranCode == TC_ATMREBOOT)
				{
					HostNms.RebootFlag = TRUE;
					m_pDevCmn->fnAPL_SetClerkPowerOff();
				}
				else
				if (TranCode == TC_ATMFTPSEND)
					fnAPL_FtpSetFileNameProc();
				else
				if (TranCode == TC_ATMRESETDVR)
				{
					fnAPL_DvrSendDataProc("RS");
					DvrStatusReturn = 0;						// 2006.05.10
				}
				else
					return T_OK;
			}
			else
				return T_OK;

			SendLength = 0;										// Send Length		
			memset(SendBuffer, 0, sizeof(SendBuffer));			// Send Buffer
			
			SendLength = fnAPP_SendHostMake(TRUE);
			fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);
			if (TranCode == TC_ATMSTATUS)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranBidResponseProc() TRAN_MSRSTATUS");
				fnAPP_MainTranProc(TRAN_MSRSTATUS);
				if(m_pDevCmn->bAgentMode == 1)
				{
					m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
				}
			}
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranBidResponseProc():return");
	return T_OK;
}

// Total Procedure
int CTranCmn::fnAPP_TranTotalProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalProc()");

	//yaokq add 20081029
	int			FlagReturn;
	FlagReturn = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_SPRJPRFLAG,1);

	fnAPP_InitializeDS();
	TranCode = TC_TOTAL;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

// 	fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);
//	iSusTotalNum = 0 ;                 
//	if(FlagReturn == 0)
//		fnAPP_SumPrint(DEV_JPR, TRUE);    //不出凭条 by hyl
//	else
//		fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);   //yaokq add 20081029

//	if (m_pDevCmn->TranResult)
	if (m_pTranCmn->InitInfo.TotalDisplayNo == 1	||
		m_pTranCmn->InitInfo.TotalDisplayNo == 2)
	{
		iSusTotalNum = 0;
		if(FlagReturn == 0)
			fnAPP_SumPrint(DEV_JPR, TRUE);    //不出凭条 by hyl
		else
			fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);   //yaokq add 20081029

//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALBATCHNO, 
//				GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001")));
		
		IniSetInt(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, "SUSPICIOUSNUM", 0);
		
		int iRetractCnt = 0;
		iRetractCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
		if (iRetractCnt > 0)
		{
			for(int jj = 0; jj < iRetractCnt; jj++)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST1%02.2d",jj), 0));
				m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST2%02.2d",jj), 0));
				m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST3%02.2d",jj), 0));
			}
			if (m_pTranCmn->LangMode)
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Retained Cards : %2.2d", iRetractCnt));
			else
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("吞卡张数: %2.2d", iRetractCnt));
			IniSetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
		}
		m_pDevCmn->fnAPL_ClearRetractCnt(DEV_MCU);
		
		fnAPL_AddDayTotalCount();
		fnAPL_SetTotalDate();
		fnAPP_SumClear();
		fnAPL_AddBatchNo();
		
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);

//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDRAWALFLAG1, 0);
	}
	
	if (m_pTranCmn->InitInfo.TotalDisplayNo == 2)
	{															// 2005.09.11
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, "0000");
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBEGINP, "0000");
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, 0);

		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBATCHNO, 
					GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO)));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalProc():return");
	return T_OK;
}

// Total Kind Procedure
int CTranCmn::fnAPP_TranTotalKindProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalKindProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_TOTALKIND;
	TranCode2 = 0;
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

	if (m_pDevCmn->TranResult)
	{
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALKINDSET, 1);
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ATMPTOTALKIND, &totalKindFlag);
	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalKindProc():return");
	return T_OK;
}

// Total Preliminary Procedure
int CTranCmn::fnAPP_TranTotalPreProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalPreProc()");
	
	int			FlagReturn;
	FlagReturn = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_SPRJPRFLAG,1);
	
	fnAPP_InitializeDS();
	TranCode = TC_TOTALPRE;
	TranCode2 = 0;
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
	
	/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
	/////////////////////////////////////////////////////////////////////////////
	
	// 	fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);
//	iSusTotalNum = 0 ;                 
//	if(FlagReturn == 0)
//		fnAPP_SumPrint(DEV_JPR, TRUE);    //不出凭条 by hyl
//	else
//		fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);   //yaokq add 20081029
	
//	if (m_pDevCmn->TranResult)
	if (m_pTranCmn->InitInfo.TotalDisplayNo == 1)
	{
		iSusTotalNum = 0;
		if(FlagReturn == 0)
			fnAPP_SumPrint(DEV_JPR, TRUE);    //不出凭条 by hyl
		else
			fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);   //yaokq add 20081029

//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALBATCHNO, 
//			GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001")));
				
		IniSetInt(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, "SUSPICIOUSNUM", 0);

		int iRetractCnt = 0;
		iRetractCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
		if (iRetractCnt > 0)
		{
			for(int jj = 0; jj < iRetractCnt; jj++)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST1%02.2d",jj), 0));
				m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST2%02.2d",jj), 0));
				m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST3%02.2d",jj), 0));
			}
			if (m_pTranCmn->LangMode)
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Retained Cards : %2.2d", iRetractCnt));
			else
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("吞卡张数: %2.2d", iRetractCnt));
			IniSetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
		}
		m_pDevCmn->fnAPL_ClearRetractCnt(DEV_MCU);

		fnAPL_AddDayTotalCount();
		fnAPL_SetTotalDate();
		fnAPP_SumClear();
		fnAPL_AddBatchNo();

		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);

		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDRAWALFLAG1, 0);
	}
//	else
//	{															// 2005.09.11
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDP, "0000");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBEGINP, "0000");
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, 0);
//		
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBATCHNO, 
//			GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO)));
//	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranTotalPreProc():return");
	return T_OK;
}

// Sub Total Procedure
int CTranCmn::fnAPP_TranSubTotalProc()
{
	//yaokq add 20081029
	int				FlagReturn;
	FlagReturn = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_SPRJPRFLAG,1);

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSubTotalProc()");

//	fnAPP_InitializeDS();
	TranCode = TC_SUBTOTAL;
	TranCode2 = 0;

//	m_pDevCmn->TranResult = TRUE;

// 	fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);
	if(FlagReturn == 0)
		fnAPP_SumPrint(DEV_JPR, TRUE);    //不出凭条 by hyl
	else
		fnAPP_SumPrint((DEV_SPR | DEV_JPR), TRUE);    //出凭条 by yaokq

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSubTotalProc():return");
	return T_OK;
}

// Add Cash Procedure
int CTranCmn::fnAPP_TranAddCashProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAddCashProc()");
// yaokq add 20081031
	int				FlagReturn;
	
	FlagReturn = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_SPRJPRFLAG,1);

	fnAPP_InitializeDS();
	TranCode = TC_ADDCASH;
	TranCode2 = 0;
	int iAddCashMode = Asc2Int(RegGetStr(_REGKEY_ADDCASHMODE, ZERO2, "1").GetBuffer(0), 1);
	
	if (iAddCashMode == 0)
	{
		if (LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR RWT",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 发送清机加钞交易到前置",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}

		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
	}
	else
	{
		m_pDevCmn->TranResult = TRUE;
	}
/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranResult)
	{
//		fnAPL_AddDayTotalCount();
//		fnAPL_FtpSendEjrProc();
//		fnAPL_SetTotalDate();
// 		fnAPL_FtpSendEjmProc(12);
	
//		if (HMERecv.DataRecvBit[48])
//		{
//			if (IsNum(&HMERecv.Data[48][5], 14))
//			{
//				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TRANBATCHNO, GetSprintf("%014.14s", &HMERecv.Data[48][5]));
//				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, GetSprintf("%014.14s", &HMERecv.Data[48][5]));
//			}
//			else
//			{
//				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TRANBATCHNO, GetSprintf("%8.8s%6.6s", GetDate(), GetTime()));
//				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, GetSprintf("%8.8s%6.6s", GetDate(), GetTime()));
//			}
//			
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAddCashProc()_INIKEY_TRANSACTION_TRANBATCHNO[%014.14s]", &HMERecv.Data[48][5]);
//		}
		
//		fnAPP_SumClear();
//		fnAPL_AddBatchNo();
//		m_pDevCmn->fnAPL_ClearSerialNo();                 //加钞不清流水号          
		fnAPP_CashProc();
// 		fnAPP_CashPrintResult((DEV_SPR | DEV_JPR), TRUE);

		if(FlagReturn == 0)     //yaokq add 20081031 打印凭条信息
			fnAPP_CashPrintResult( DEV_JPR, TRUE);            //去掉凭条 BY HYL
		else
			fnAPP_CashPrintResult((DEV_SPR | DEV_JPR), TRUE);
		/////////////清除吞钞的统计值   by zjd
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0);
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0);

		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDRAWALFLAG1, 1);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAddCashProc():return");
	return T_OK;
}

// Sub Cash Procedure
int CTranCmn::fnAPP_TranSubCashProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSubCashProc()");

	fnAPP_InitializeDS();
	TranCode = TC_SUBCASH;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng
	
/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranResult)
	{
		fnAPP_CashProc();
// 		fnAPP_CashPrintResult((DEV_SPR | DEV_JPR), TRUE);
		fnAPP_CashPrintResult( DEV_JPR, TRUE);             //去掉凭条 BY HYL
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSubCashProc():return");
	return T_OK;
}

// Clear Cash Procedure
int CTranCmn::fnAPP_TranClearCashProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranClearCashProc()");

	fnAPP_InitializeDS();
	TranCode = TC_CLEARCASH;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranResult)
	{
		fnAPP_CashProc();
// 		fnAPP_CashPrintResult((DEV_SPR | DEV_JPR), TRUE);
		fnAPP_CashPrintResult( DEV_JPR, TRUE);             //去掉凭条 BY HYL
	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranClearCashProc():return");
	return T_OK;
}

// Print Cash Procedure
int CTranCmn::fnAPP_TranPrintCashProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranPrintCashProc()");

	fnAPP_InitializeDS();
	TranCode = TC_PRINTCASH;
	TranCode2 = 0;

	m_pDevCmn->TranResult = TRUE;

// 	fnAPP_CashPrint((DEV_SPR | DEV_JPR), TRUE);
	fnAPP_CashPrint( DEV_JPR, TRUE);    //去掉凭条 BY HYL

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranPrintCashProc():return");
	return T_OK;
}

// Down Fit Procedure
int CTranCmn::fnAPP_TranDownFitProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownFitProc()");

	fnAPP_InitializeDS();
	TranCode = TC_DOWNFIT;
	TranCode2 = 0;
	DownloadOverFlag = 0;
	DownloadCurrNum = 0;

	while (TRUE)
	{
		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng

		if (m_pDevCmn->TranResult)
		{
			//判断是否还有需要下载的，没有就出来   by zjd
			if(DownloadOverFlag == 0)
			{
				continue;
			}
			else
			{											// 下载完全，版本号记录到INI中   by zjd
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_FITVERSION, GetSprintf("%8.8s", NewFITVersion));
				fnAPL_GetFitFileContent();				// 重新导入内存中  by zjd
				DownloadFITFlag = 0;
				break;
			}
		}
		else 
			break;
	}

//	m_pDevCmn->TranResult = TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownFitProc():return");
	return T_OK;
}

// Down IC Fit Procedure added by yusy ic
int CTranCmn::fnAPP_TranDownICFitProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownICFitProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_DOWNICFIT;
	TranCode2 = 0;
	DownloadOverFlag = 0;
	DownloadCurrNum = 0;
	
	while (TRUE)
	{
		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng
		
		if (m_pDevCmn->TranResult)
		{
			//判断是否还有需要下载的，没有就出来   by zjd
			if(DownloadOverFlag == 0)
			{
				continue;
			}
			else
			{											// 下载完全，版本号记录到INI中   by zjd
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ICFITVERSION, GetSprintf("%8.8s", NewICFITVersion));
				fnAPL_GetICFitFileContent();				// 重新导入内存中  by zjd
				DownloadICFITFlag = 0;
				break;
			}
		}
		else 
			break;
	}
	
	//	m_pDevCmn->TranResult = TRUE;
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownICFitProc():return");
	return T_OK;
}

// Down AID List Procedure added by yusy ic
int CTranCmn::fnAPP_TranDownAIDListProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownAIDListProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_DOWNAIDLIST;
	TranCode2 = 0;
	DownloadOverFlag = 0;
	DownloadCurrNum = 0;
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng
		
	if (m_pDevCmn->TranResult)
	{
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_AIDLISTVERSION, GetSprintf("%8.8s", NewAIDListVersion));
		DownloadAIDListFlag = 0;
	}
	
	//	m_pDevCmn->TranResult = TRUE;
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownAIDListProc():return");
	return T_OK;
}

// Down Echannel Parameter Procedure
int CTranCmn::fnAPP_TranDownEchannelParaProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownEchannelParaProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_DOWNECHANNELPARA;
	TranCode2 = 0;
	DownloadOverFlag = 0;
	DownloadCurrNum = 0;
//	memset(PROTVERSIONDIFF, 1, sizeof(PROTVERSIONDIFF));
	
	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng
	
	if (m_pDevCmn->TranResult)
	{
		fnAPL_GetEchannelFileContent();
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELVERSION, GetSprintf("%8.8s", NewEchannelParaVersion));
		DownloadEchannelParaFlag = 0;
		DownloadEchannelNum = 1;
	}

	CStringArray	strTempArray;
	if (m_pDevCmn->TranResult)
	{
		for (int EchannelCurrNum = 0; EchannelCurrNum < itemof(ECHANNELContent_POST); EchannelCurrNum++)
		{
			if (memcmp(ECHANNELContent_POST[EchannelCurrNum], "AAAAAA", 6) == 0)
				break;
			else
			{
				SplitString(CString(ECHANNELContent_POST[EchannelCurrNum]), ",", strTempArray);
				if ((memcmp(strTempArray[2].GetBuffer(0),
					IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dVER", EchannelCurrNum + 1), "11111111").GetBuffer(0), 8) == 0));
				else
					break;
			}
		}
		if (EchannelCurrNum == Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPARM, "ECHANNELNUM", "0")))
		{
//			if (memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELVERSION,"00000000").GetBuffer(0),
//				IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPROT, _INIKEY_TRANSACTION_ECHVERSION,"00000000").GetBuffer(0),
//				8) == 0)
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 1);
		}
	}
	
	//	m_pDevCmn->TranResult = TRUE;
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownEchannelParaProc():return");
	return T_OK;
}

// Down Echannel Protocol Procedure
int CTranCmn::fnAPP_TranDownEchannelProtProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownEchannelProtProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_DOWNECHANNELPROT;
	TranCode2 = 0;

	for (DownloadEchannelProtCurrNum = 0; DownloadEchannelProtCurrNum < itemof(ECHANNELContent_POST); DownloadEchannelProtCurrNum++)
	{
		if ((memcmp(ECHANNELContent_POST[DownloadEchannelProtCurrNum], "AAAAAA", 6) == 0)	||	// End Check
			(DownloadEchannelProtCurrNum == 1))
		{
			if (m_pDevCmn->TranResult	||
				!DownloadEchannelProtCurrNum)
			{
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELVERSION, GetSprintf("%8.8s", NewEchannelParaVersion));
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 1);
				DownloadEchannelParaFlag = 0;
				DownloadEchannelNum = 1;
				m_pDevCmn->TranResult = TRUE;
			}
			break;
		}

//		if (PROTVERSIONDIFF[DownloadEchannelProtCurrNum] == 0	&&
//			DownloadEchannelParaFlag)
//		{
//			m_pDevCmn->TranResult = TRUE;
//			continue;
//		}
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadEchannelProtCurrNum[%d]", DownloadEchannelProtCurrNum);
		
		DownloadOverFlag = 0;
		DownloadCurrNum = 0;
//		EProtRowNum = 0;
		CStringArray strTempArray;
		SplitString(CString(ECHANNELContent_POST[DownloadEchannelProtCurrNum]), ",", strTempArray);
		DownloadCurrProtLen = Asc2Int(strTempArray[3], 6);

		if (memcmp(strTempArray[2].GetBuffer(0),
			IniGetStr(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dVER", DownloadEchannelProtCurrNum + 1),"00000000").GetBuffer(0), 8) == 0)
		{
			m_pDevCmn->TranResult = TRUE;
			continue;
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadEchannelProtCurrNum[%d]", DownloadEchannelProtCurrNum);
		
		while (TRUE)
		{
			m_pDevCmn->fnAPL_SetProcCount('3');
			fnAPP_SendHost();
			fnAPP_RecvHost();
			if (m_pDevCmn->TranResult)
				m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
			else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Return[%d], DownloadOverFlag[%d]", m_pDevCmn->TranResult, DownloadOverFlag);
			if (m_pDevCmn->TranResult)
			{
				//判断是否还有需要下载的，没有就出来   by zjd
				if(DownloadOverFlag == 0)
				{
					continue;
				}
				else
				{
					fnAPP_EditECProtDataProc(DownloadEchannelProtCurrNum);
					IniSetStr(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dVER", DownloadEchannelProtCurrNum + 1), GetSprintf("%8.8s", strTempArray[2].GetBuffer(0)));
//					IniSetInt(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dRowNUM", DownloadEchannelProtCurrNum + 1), EProtRowNum);
					break;
				}
			}
			else 
				break;
		}

		if (m_pDevCmn->TranResult != TRUE)
			break;
	}
	
	//	m_pDevCmn->TranResult = TRUE;
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownEchannelProtProc():return");
	return T_OK;
}

// Down Parm Procedure
int CTranCmn::fnAPP_TranDownParmProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownParmProc()");

	fnAPP_InitializeDS();
	TranCode = TC_DOWNPARM;
	TranCode2 = 0;

	while (TRUE)
	{
		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng

		if (m_pDevCmn->TranResult)
		{	 											// 下载完全，版本号记录到INI中   by zjd
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PARMVERSION, GetSprintf("%8.8s", NewParmVersion));
			DownloadParmFlag = 0;
			break;
		}
		else 
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownParmProc():return");
	return T_OK;
}

// Down PBOC Parm Procedure
int CTranCmn::fnAPP_TranDownPBOCParaProc()
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownPBOCParaProc()");
	
	fnAPP_InitializeDS();
	TranCode = TC_DOWNPBOCPARA;
	TranCode2 = 0;
	
	while (TRUE)
	{
		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng
		
		if (m_pDevCmn->TranResult)
		{	 											// 下载完全，版本号记录到INI中
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PBOCPARAVERSION, GetSprintf("%8.8s", NewPBOCParaVersion));
			DownloadPBOCParaFlag = 0;
			break;
		}
		else 
			break;
	}
	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownPBOCParaProc():return");
	return T_OK;
}

// Down Pay Procedure
int CTranCmn::fnAPP_TranDownPayProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownPayProc()");

	fnAPP_InitializeDS();
	TranCode = TC_DOWNPAY;
	TranCode2 = 0;
	DownloadOverFlag = 0;

	while (TRUE)
	{
		m_pDevCmn->fnAPL_SetProcCount('3');
		fnAPP_SendHost();
		fnAPP_RecvHost();
		if (m_pDevCmn->TranResult)
			m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
		else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng

		if (m_pDevCmn->TranResult)
		{
			//判断是否还有需要下载的，没有就出来   by zjd
			if(DownloadOverFlag == 0)
			{
				continue;
			}
			else
			{											// 下载完全，版本号记录到INI中   by zjd
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PAYVERSION, GetSprintf("%8.8s", NewPayVersion));
				DownloadPayFlag = 0;
				break;
			}
		}
		else 
			break;
	}

//	m_pDevCmn->TranResult = TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDownPayProc():return");
	return T_OK;
}

// Send Detail Procedure
int CTranCmn::fnAPP_TranSendDetailProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSendDetailProc()");

	fnAPP_InitializeDS();
	TranCode = TC_SENDDETAIL;
	TranCode2 = 0;

	while (TRUE)
	{
		if (fnAPL_GetDetailData())
		{
			while (TRUE)
			{
				m_pDevCmn->fnAPL_SetProcCount('3');
				fnAPP_SendHost();
				fnAPP_RecvHost();
				if (m_pDevCmn->TranResult)
					m_pDevCmn->fnAPL_SetProcCount('5');			// Host Ok
				else m_pDevCmn->fnAPL_SetProcCount('9');		// Host Ng

// 				if (m_pDevCmn->TranResult)
 					break;
// 				else 
// 					continue;
			}
		}
		else
		{
//			fnAPP_MainTranProc(TRAN_DETAILEND);
			break;
		}

	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranSendDetailProc():return");
	return T_OK;
}





// Detail End Procedure
int CTranCmn::fnAPP_TranDetailEndProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDetailEndProc()");

	fnAPP_InitializeDS();
	TranCode = TC_DETAILEND;
	TranCode2 = 0;

	m_pDevCmn->fnAPL_SetProcCount('3');
	fnAPP_SendHost();
	fnAPP_RecvHost();
	if (m_pDevCmn->TranResult)
		m_pDevCmn->fnAPL_SetProcCount('5');						// Host Ok
	else m_pDevCmn->fnAPL_SetProcCount('9');					// Host Ng

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDetailEndProc():return");
	return T_OK;
}

// Atm Bid Send Detail Procedure
int CTranCmn::fnAPP_TranAtmBidSendDetailProc()
{   
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidSendDetailProc()");
// 	fnAPP_InitializeDS();
	
	char		SendHostIp[15];
	char        iHostPort[5];
	char    	DetailTblTemp[400];      //数据长度
	int			nTempCount = 0;
	char        szFileType;
	FILE*		TempFp;
	CFileStatus	TempFileStatus;
	char        FileEnd;
	char        szTempFileNamePath[256];
	char        szDetailFileName[14];
	
	SetBegin=0;
	memset(szDetailFileName,0,sizeof(szDetailFileName));
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
    //200800获取主机下发的信息
	memcpy(szDetailFileName,&HMERecv.Data[122][3],14);
	memcpy(&szFileType,&HMERecv.Data[122][17],1);
	SetBegin = Asc2Int(&HMERecv.Data[122][18], 8);	
	memcpy(SendHostIp,&HMERecv.Data[122][26],15);
	memcpy(iHostPort,&HMERecv.Data[122][41],5);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szDetailFileName(%s)",szDetailFileName);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szFileType(%c)",szFileType);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin(%d)",SetBegin);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendHostIp(%s)",SendHostIp);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iHostPort(%s)",iHostPort);

	
	if(szFileType == 'J')
	{
		sprintf(szTempFileNamePath, "%s\\%14s.LOG", _EJR_DETAILSEND, szDetailFileName);
	}
	else
	if(szFileType == 'R')
	{
		sprintf(szTempFileNamePath, "%s\\%14s.SPR.LOG", _EJR_DETAILSEND, szDetailFileName);
	}
	else
	if(szFileType == 'S')
	{
	    sprintf(szTempFileNamePath, "%s\\%14s.LINE.LOG", _EJR_DETAILSEND, szDetailFileName);
	}
    else
	{   
    	return T_EXIT;
	}

	TempFp = fopen(szTempFileNamePath, "r");
	if(TempFp)
	{
	fseek(TempFp,0L,SEEK_END);   
    SendFileLength = ftell(TempFp);
	fclose(TempFp);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SendFileLength[%d]",SendFileLength);

    }
	else
	{   
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Open File[%s] Fail ",szTempFileNamePath);
		return T_EXIT;
	}
	

	TranCode = TC_ATMBIDSENDDETAIL;
	TranCode2 = 0;
	SendLength = 0;	// Send Length
	if (m_pTranCmn->LangMode)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM Upload Journal",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 提取远程流水",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
	}

	memset(SendBuffer, 0, sizeof(SendBuffer));					// Send Buffer
	SendLength = fnAPP_SendHostMake(TRUE);
	fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);    //发送取远程流水通知交易报文的应答  BY HLY
    
	while (TRUE)
	{   
		fnAPP_InitializeDS();	
		TranCode = TC_ATMBIDSENDDETAIL;
	    TranCode2 = 0;
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)   //接收提取流水的报文说明
		{
 		  fnAPP_RecvAnalBidCommand();


		  if(memcmp(HMERecv.Data[3], "200900", 6) != 0)              //20070703 
		  {
		  	  return T_EXIT;
		  }
  

		  SetBegin = Asc2Int(&HMERecv.Data[122][4],8);                //获取偏移量
		  

	  	  if(SetBegin >= SendFileLength)                               //偏移量大于文件长度则不发报文
		  {   
		   	  MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin > = SendFileLentch");
		   	  return T_EXIT;
		  } 
		  
 		   
	    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetBegin(%d)",SetBegin);
		}
	     else
			break;

		
		if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
		    TempFp = fopen(szTempFileNamePath, "r");			
			if (TempFp)
			{
                if((SendFileLength - SetBegin) > 400)
				{
					memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
					fseek(TempFp,SetBegin,0);
					fread(&DetailTblTemp, 400, 1, TempFp);
					fclose(TempFp);
					FileEnd = '1';
				}
				else
				if((SendFileLength - SetBegin) == 400)
				{   
					memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
					fseek(TempFp,SetBegin,0);
					fread(&DetailTblTemp, 400, 1, TempFp);
					fclose(TempFp);
					FileEnd = '0';	
				}
				else
                if((SendFileLength - SetBegin) <  400 && (SendFileLength - SetBegin) > 0 )
				{
					memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
					fseek(TempFp,SetBegin,0);
					fread(&DetailTblTemp, SendFileLength - SetBegin, 1, TempFp);
					fclose(TempFp);
					FileEnd = '0';	
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DetailTblTemp[%s]",DetailTblTemp);
				}	

			}
		   

				HMESend.DataLength[123] = 411;  
				memset(HMESend.Data[123], ' ', HMESend.DataLength[123]);
				memcpy(&HMESend.Data[123][0],"408",3);
				memcpy(&HMESend.Data[123][3],&FileEnd,1);
				memcpy(&HMESend.Data[123][4],"0400",4);
				memcpy(&HMESend.Data[123][8],"400",3);
				memcpy(&HMESend.Data[123][11],DetailTblTemp, (SendFileLength - SetBegin) < 400 ? (SendFileLength - SetBegin ):400 );
   
			
            
			SendLength = 0;												// Send Length		
			memset(SendBuffer, 0, sizeof(SendBuffer));					// Send Buffer
			SendLength = fnAPP_SendHostMake(TRUE);
	        fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);	                         //发送流水数据报文
			
			if (FileEnd == '0')
				break;
			else
			if(FileEnd == '1')
				continue;
		}
	}
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidSendDetailProc():return");
	return T_OK;
}


int CTranCmn::fnAPP_TranAtmBidAdDownloadProc()
{   
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranAtmBidAdDownloadProc()");
// 	fnAPP_InitializeDS();
	
//    char		FileVersion[4];
	char    	EquipmentBrand[3];      
	CString     ADFileUseTime("");
	CString     ADFileEndTime("");
	int         EquipmentType;
	int         FileType;
	char        OperatingSystem[3];
	int         DataLength;	
	CFileStatus	TempFileStatus;	
	CString	    FileList("");
    CStringArray FileNameList;
	int         Filelength;
	int         FNameLength;
	char        TempNameList2[21]; 
	SHFILEOPSTRUCT   FileOp;   
	char fPath[256],tPath[256];
	FileOp.wFunc = FO_COPY;
	FileOp.fFlags= FOF_NOCONFIRMATION;
	 
	
	
    //200600获取主机下发的信息
	ADFileUseTime = GetSprintf("%12.12s",&HMERecv.Data[122][3]);
	ADFileEndTime = GetSprintf("%12.12s",&HMERecv.Data[122][15]);
	FileType = Asc2Int(&HMERecv.Data[122][27], 1);
	memcpy(EquipmentBrand,&HMERecv.Data[122][28], 3);
	EquipmentType = Asc2Int(&HMERecv.Data[122][31],1);
	memcpy(OperatingSystem,&HMERecv.Data[122][32],3);
	DataLength = Asc2Int(&HMERecv.Data[122][35],3);
   	FileList = GetSprintf("%s", &HMERecv.Data[122][38]);
	
	
	SplitString(FileList, "|", FileNameList);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileList[%s]",FileList);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileNameList.GetSize()[%d]",FileNameList.GetSize());
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::EquipmentType[%d]",EquipmentType);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileType[%d]",FileType);

    _mkdir(_AP_ADUPDATEDOWNLOAD_DIR);
	_mkdir(_AP_ADUPDATEBACK_DIR);
	_mkdir(_AP_NOTICEUPDATEDOWNLOAD_DIR);
	_mkdir(_AP_NOTICEUPDATEBACK_DIR);
	_mkdir(_AP_NOTICEUPDATE_DIR);
	_mkdir(_AP_CAUTIONUPDATEDOWNLOAD_DIR);
	_mkdir(_AP_CAUTIONUPDATEBACK_DIR);
	_mkdir(_AP_CAUTIONUPDATE_DIR);
	
	TranCode = TC_ATMBIDADDOWNLOAD;
	TranCode2 = 0;
	SetBegin = 0;
	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
	Filelength = Asc2Int(FileNameList[0], 9);
	FNameLength = FileNameList[0].GetLength() - 9;
	memcpy(&TempNameList2,GetSprintf("%s",FileNameList[0]),FileNameList[0].GetLength());
	memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList2[9]),FNameLength);
	
	if (m_pTranCmn->LangMode)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s  ATMP_ADM Download PIC",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s  监控管理 广告画面、公告下发",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
	}

	SendLength = 0;	                                                     // Send Length		
	memset(SendBuffer, 0, sizeof(SendBuffer));					         // Send Buffer
	SendLength = fnAPP_SendHostMake(TRUE);
	fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);    
    if( FileType ==1 )                                 //广告下发
	{	
		int count;
		//原图片为永久广告进行备份 BY HYL
	    for(count=1;count<=10;count++)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::ADPICTUREENDTIME[%d][%s]",count,IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,GetSprintf("%s%d","ADPICTUREENDTIME",count)));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::ADPICTUREENDTIME[%d][%s]",count,IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,GetSprintf("%s%d","ADPICTUREENDTIME",count)));
			if(memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,GetSprintf("%s%d","ADPICTUREENDTIME",count)),"999999999999",12) == 0)
// 				&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADPICTUREUSETIME,GetSprintf("%d",count))=="")
			{	                                                      
// 				DeleteFiles(_AP_ADUPDATEBACK_DIR, -1);
				DeleteFile(GetSprintf("%s\\%s%d%s",_AP_ADUPDATEBACK_DIR,"AD",count,".jpg"));
				memset(fPath,0,sizeof(fPath));
				memset(tPath,0,sizeof(tPath));
				strcpy(fPath,GetSprintf("%s\\%s%d%s",_AP_ADUPDATE_DIR,"AD",count,".jpg"));   
				strcpy(tPath,_AP_ADUPDATEBACK_DIR);   
				FileOp.pFrom = fPath;   
				FileOp.pTo = tPath;   
				FileOp.wFunc = FO_COPY;
	            FileOp.fFlags= FOF_NOCONFIRMATION;
				if(!SHFileOperation(&FileOp))
				{		  
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::[AD%d.jpg]ADBACKFile Move Success!",count);
				}
				else 
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::[AD%d.jpg]ADBACKFile Move Failed!",count);
					return T_EXIT;
				}
			}
		}
		
		int FileListNum=0;
		//判断是否存在未下完的文件by hyl
		if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH) !=0 
			&& IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM) !=0
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME) !=""
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST) == FileList)    //判断是否存在未下完的文件
		{
			SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,0);
			FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM);          //如果存在未下完的文件,为列表第几个

		}
		
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST,FileList);               //正在下载的文件列表

		while (FileListNum < FileNameList.GetSize())
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileNameList[%d][%s]",FileListNum,FileNameList[FileListNum]);
			
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM,FileListNum);
			
			if(fnAPP_TranAtmBidAdverDownLoadFileProc(FileNameList[FileListNum],FileListNum) == T_OK)
			{
			  IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAME,"");                     //下载完一个文件把文件名清空
			  IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
			  //增加下载个数记录是下载列表中的第几个
			  FileListNum++;
			}
			else
			{
				break;
			}
		}
	    if( FileListNum== FileNameList.GetSize())
		{
		
		 IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADPICTURELIST, FileList);                        //完成后保存下载列表
         IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELISTNUM,0);              //如果存在未下完的文件,为列表第几个
		 IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILENAMELIST,"");                //完成下载,清除下载列表

		 SplitString(FileList ,"|", FileNameList);
		 int ADDOWNCOUNT=0;
		 char TempFileName[12];
		 char ADFileName[12];
		 while (ADDOWNCOUNT < FileNameList.GetSize())
		 {
			 FNameLength = FileNameList[ADDOWNCOUNT].GetLength() - 9;
			 memcpy(&TempFileName,GetSprintf("%s",FileNameList[ADDOWNCOUNT]),FileNameList[ADDOWNCOUNT].GetLength());
			 memcpy(ADFileName,GetSprintf("%s",&TempFileName[9]),FNameLength);
             INT ADNUM;
			 for(ADNUM=1;ADNUM<=10;ADNUM++)
			 {
				 if(memcpy(ADFileName,GetSprintf("%s%d%s","AD",ADNUM,".jpg"),FNameLength))
				 {
                     IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",ADNUM),"ADFileUseTime");
				     IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ADNUM),"ADFileEndTime");
				 }				 
			 }
         }		 
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidAdDownloadProc()");
		}
	}
	else
	if(FileType == 2 )                            
	{
		
		//原图片为永久公告进行备份 BY HYL
		if(memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME),"999999999999",12) == 0)     //需要生效后才会备份
		{	                                                      
			DeleteFiles(_AP_NOTICEUPDATEBACK_DIR, -1);
			strcpy(fPath,GetSprintf("%s\\%s",_AP_NOTICEUPDATE_DIR,"NOTICE*.*"));   
			strcpy(tPath,_AP_NOTICEUPDATEBACK_DIR);   
			FileOp.pFrom = fPath;   
			FileOp.pTo = tPath;   
			if(!SHFileOperation(&FileOp))
			{		  
				int NoticeNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM);
				IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTRUEBACKNUM,NoticeNum);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NOTICEBACKFile Move Success!");
			}
			else 
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NOTICEBACKFile Move Failed!");
				return T_EXIT;
			}
		}		
		

		int FileListNum=0;
		//判断是否存在未下完的文件by hyl
		if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH) !=0 
			&& IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM) !=0
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURETEMPLIST) !=""
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME) == FileList)    //判断是否存在未下完的文件
		{
			SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,0);
			FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM);          //如果存在未下完的文件,为列表第几个
		}
		
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURETEMPLIST,FileList);               //正在下载的文件列表

		while (FileListNum < FileNameList.GetSize())
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NoticeFileNameList[%d][%s]",FileListNum,FileNameList[FileListNum]);
			
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM,FileListNum);

			if(fnAPP_TranAtmBidNoticeDownLoadFileProc(FileNameList[FileListNum],FileListNum) == T_OK)
			{
			  IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAME,"");                     //下载完一个文件把文件名清空
			  IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
				FileListNum++;
			}
			else
			{
				break;
			}
		}
		if( FileListNum== FileNameList.GetSize())
		{
			
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREDOWNLOADNUM, FileListNum);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREUSETIME, ADFileUseTime);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME, ADFileEndTime);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILENAMELISTNUM,0);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidNoticeDownloadProc()");
		}
           
	}
	else
	if(FileType == 3)									// added by yusy 2010.03.22		安全提示画面            
	{
		//原图片为永久公告进行备份
		if(memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME),"999999999999",12) == 0)     //需要生效后才会备份
		{	                                                      
			DeleteFiles(_AP_CAUTIONUPDATEBACK_DIR, -1);
			strcpy(fPath,GetSprintf("%s\\%s",_AP_CAUTIONUPDATE_DIR,"CAUTION*.*"));   
			strcpy(tPath,_AP_CAUTIONUPDATEBACK_DIR);   
			FileOp.pFrom = fPath;   
			FileOp.pTo = tPath;   
			if(!SHFileOperation(&FileOp))
			{		  
				int NoticeNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURENUM);
				IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTRUEBACKNUM,NoticeNum);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CAUTIONBACKFile Move Success!");
			}
			else 
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CAUTIONBACKFile Move Failed!");
				return T_EXIT;
			}
		}		
		
		
		int FileListNum=0;
		//判断是否存在未下完的文件
		if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH) !=0 
			&& IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM) !=0
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURETEMPLIST) !=""
			&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME) == FileList)    //判断是否存在未下完的文件
		{
			SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,0);
			FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM);          //如果存在未下完的文件,为列表第几个
		}
		
		IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURETEMPLIST,FileList);               //正在下载的文件列表
		
		while (FileListNum < FileNameList.GetSize())
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CautionFileNameList[%d][%s]",FileListNum,FileNameList[FileListNum]);
			
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM,FileListNum);
			
			if(fnAPP_TranAtmBidCautionDownLoadFileProc(FileNameList[FileListNum],FileListNum) == T_OK)
			{
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAME,"");                     //下载完一个文件把文件名清空
				IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
				FileListNum++;
			}
			else
			{
				break;
			}
		}
		if( FileListNum== FileNameList.GetSize())
		{
			
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREDOWNLOADNUM, FileListNum);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREUSETIME, ADFileUseTime);
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME, ADFileEndTime);
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILENAMELISTNUM,0);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidCautionDownloadProc()");
		}
			
	}
	else 
		return T_EXIT;

	return T_OK;
}




int CTranCmn::fnAPP_TranAtmBidSoftUpdateProc()
{   
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranAtmBidSoftUpdateProc()");
// 	fnAPP_InitializeDS();
	

	char    	EquipmentBrand[3];      
	CString     UpdateFileUseTime("");
	CString  	FileVersion("");
	char        EquipmentType;
	char        OperatingSystem[3];
	int         DataLength;	
	CFileStatus	TempFileStatus;	
	CString	    FileList("");
    CStringArray FileNameList;
	int         Filelength;
	int         FNameLength;
	char        TempNameList2[21]; 

    _mkdir(_AP_SOFTUPDATE_DIR);
	_mkdir(_AP_SOFTBACKUPDATE_DIR);
	
    //201000获取主机下发的信息
	FileVersion = GetSprintf("%4.4s",&HMERecv.Data[122][3]);
	UpdateFileUseTime = GetSprintf("%12.12s",&HMERecv.Data[122][7]);
	memcpy(EquipmentBrand,&HMERecv.Data[122][19], 3);
	EquipmentType = Asc2Int(&HMERecv.Data[122][22],1);
	memcpy(OperatingSystem,&HMERecv.Data[122][23],3);
	DataLength = Asc2Int(&HMERecv.Data[122][26],3);
	FileList = GetSprintf("%s", &HMERecv.Data[122][29]);

	SplitString(FileList, "|", FileNameList);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::UpdateFileUseTime[%s]",UpdateFileUseTime);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileList[%s]",FileList);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileNameList.GetSize()[%d]",FileNameList.GetSize());
	TranCode = TC_ATMBIDSOFTUPDATE;
	TranCode2 = 0;
	SetBegin = 0;
	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
	Filelength = Asc2Int(FileNameList[0], 9);
	FNameLength = FileNameList[0].GetLength() - 9;
	memcpy(&TempNameList2,GetSprintf("%s",FileNameList[0]),FileNameList[0].GetLength());
	memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList2[9]),FNameLength);
	
	SendLength = 0;	                                                     // Send Length		
	memset(SendBuffer, 0, sizeof(SendBuffer));					         // Send Buffer
	SendLength = fnAPP_SendHostMake(TRUE);
	fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);    //发送取远程升级通知交易报文的应答  BY HLY
    
	if (m_pTranCmn->LangMode)
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s  ATMP_ADM UPDATE SOFTWARE",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])));
	}
	else
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s  监控管理 软件更新",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])));
	}

    ///////////////////////////////////////////////////////////////////////////////

	int FileListNum=0;
	//判断是否存在未下完的文件by hyl
	if(IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH) !=0                         //已下完的文件长度
		&& IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM) != 0               //一次下载中的第几个文件
		&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME) !=""                      //正在下载的文件名
		&& IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST) == FileList)         //判断是否存在未下完的文件
	{
		SetBegin = IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,0);
		FileListNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM);          //如果存在未下完的文件,为列表第几个
	}
	
	IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST,FileList);                    //保存正在下载的文件列表
   		
    while (FileListNum < FileNameList.GetSize())
	{   
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::FileNameList[%d][%s]",FileListNum,FileNameList[FileListNum]);
		
        IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM,FileListNum);

		if(fnAPP_TranAtmBidDownloadFileProc(FileNameList[FileListNum]) == T_OK)
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME,"");                     //下载完一个文件把文件名清空
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,0);                    //下载完一个文件把文件偏移量清空
			FileListNum++;
		}
		else 
		{
			break;
		}
    }
	if(FileListNum == FileNameList.GetSize())      
	{
	 IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,_INIKEY_TRANSACTION_HOSTDOWNLOADVERSION, FileVersion);
     IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,_INIKEY_TRANSACTION_SOFTUPDATEFILEUSETIME, UpdateFileUseTime);
     IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,_INIKEY_TRANSACTION_SOFTUPDATEFILELIST, FileList);                      //完成后保存下载列表
	 IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELIST,"");                          //完成后清除临时下载列表
	 IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAMELISTNUM,0);                        //如果存在未下完的文件,为列表第几个
	 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "insert UpdateFileUseTime",UpdateFileUseTime);
	 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "insert FileList",FileList);

    } 
  MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidSoftUpdateProc()");
  return T_OK;	
}


int CTranCmn::fnAPP_TranAtmBidDownloadFileProc(CString TempNameList)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranAtmBidDownloadFileProc()");
    FILE*		TempFp;
	int        TempDownLength;
	int        TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	
//	SetBegin = 0;

	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(TempNameList1,0,sizeof(TempNameList1));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);
    sprintf(szTempFileNamePath, "%s\\%s", _AP_SOFTBACKUPDATE_DIR, szDownLoadFileName);
    IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILENAME,szDownLoadFileName); //正在下载的文件
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
//	TempFp = fopen(szTempFileNamePath, "wb");
	TempFp = fopen(szTempFileNamePath, "ab");
	if(TempFp)
	{
 		fclose(TempFp);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",szTempFileNamePath);	
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",szTempFileNamePath);
		return T_EXIT;
	}

	while (TRUE)
	{   

		fnAPP_InitializeDS();  	
		TranCode = TC_ATMBIDSOFTUPDATE;
		TranCode2 = 0;
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)   
		{
			fnAPP_RecvAnalBidCommand();	

            if(memcmp(HMERecv.Data[3], "201100", 6) != 0)
			{
			   return T_EXIT;
 			}
					
			memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
			memset(szRecvFileName,0,sizeof(szRecvFileName));
			memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
			memcpy(szRecvFileName,GetSprintf("%-12s",&HMERecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	
			memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);
			RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMERecv.Data[122][15]),8);
		    memcpy(&FileEnd,&HMERecv.Data[122][23],1);
			TempDownLength = Asc2Int(&HMERecv.Data[122][24],4);
            TempDownLength1 = Asc2Int(&HMERecv.Data[122][28],3);
            memcpy(&DetailTblTemp,&HMERecv.Data[122][31],TempDownLength1);	
			
			if(memcmp(szRecvFileName,szDownLoadFileName,DownFileNameLength)!= 0 || RecvSetbegin != SetBegin )
			{
			 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
			 return T_EXIT;
            }			   
		}
		else
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsRecvData TimeOut");
			return T_EXIT;
		}
		if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
		{   
			TempFp = fopen(szTempFileNamePath, "ab");
			if (TempFp)
			{   
                MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
                if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
				{
					OneFileEnd = '0';
				}
				fseek(TempFp,SetBegin,0);
				fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
				fclose(TempFp);
			}
			

			TranCode = TC_ATMBIDSOFTUPDATE;
		    TranCode2 = 0;
			SetBegin += TempDownLength1;                                             //设置偏移量
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTTEMPFILELENGTH,SetBegin);   //文件已传送长度
			SendLength = 0;												             // Send Length		
			memset(SendBuffer, 0, sizeof(SendBuffer));				               	 // Send Buffer
			SendLength = fnAPP_SendHostMake(TRUE);
			fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);	     //发送流水数据报文
            
			if ((FileEnd == '0')||(OneFileEnd == '0'))        
				break;	
			else
		    if(FileEnd == '1')                                                       //如有多个文件则文件全部传完前,前置下送后续包标志都为1
				continue;
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
	}
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidDownloadFileProc():return");
    return T_OK;  
}

int CTranCmn::fnAPP_TranAtmBidAdverDownLoadFileProc(CString TempNameList,int i)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranAtmBidAdverDownLoadFileProc()");
    FILE*		TempFp;
	int        TempDownLength;
	int        TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
	char        AdNewFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	

// 	SetBegin = 0;

	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(AdNewFileName,0,sizeof(AdNewFileName));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);

	////////////////////////////////////////////////////////////////////////////////////////
//	memcpy(&AdNewFileName,GetSprintf("%2s%s%s","AD",Int2Asc(i + 1),".jpg"),6+strlen(Int2Asc(i)));
    sprintf(szTempFileNamePath, "%s\\%s", _AP_ADUPDATEDOWNLOAD_DIR, szDownLoadFileName);
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szTempFileNamePath[%s]",szDownLoadFileName);	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
//	TempFp = fopen(szTempFileNamePath, "wb");
	TempFp = fopen(szTempFileNamePath, "ab");
	if(TempFp)
	{
 		fclose(TempFp);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",szDownLoadFileName);	
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",szDownLoadFileName);
		return T_EXIT;
	}
    
	while (TRUE)
	{   
		fnAPP_InitializeDS();  	
		TranCode = TC_ATMBIDADDOWNLOAD;
		TranCode2 = 0;
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)   //接收提取流水的报文说明
		{
			fnAPP_RecvAnalBidCommand();	

 			if(memcmp(HMERecv.Data[3], "200700", 6) != 0)              //20070703
 			{
 				return T_EXIT;
 			}
			
			memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
			memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
			memset(szRecvFileName,0,sizeof(szRecvFileName));
			memcpy(szRecvFileName,GetSprintf("%-12s",&HMERecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	 
			memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);
			RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMERecv.Data[122][15]),8);
		    memcpy(&FileEnd,&HMERecv.Data[122][23],1);
			TempDownLength = Asc2Int(&HMERecv.Data[122][24],4);
            TempDownLength1 = Asc2Int(&HMERecv.Data[122][28],3);
            memcpy(&DetailTblTemp,&HMERecv.Data[122][31],TempDownLength1);
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szDownLoadFileName[%s]",szDownLoadFileName);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

			if(RecvSetbegin != SetBegin )
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
				return T_EXIT;
            }			   
		}
		else
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsRecvData TimeOut");
			return T_EXIT;
		}
		
		if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
		{   
			TempFp = fopen(szTempFileNamePath, "ab");
			if (TempFp)
			{   
                MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
                if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
					OneFileEnd = '0';
				fseek(TempFp,SetBegin,0);
				fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
				fclose(TempFp);
						
			}
			
			TranCode = TC_ATMBIDADDOWNLOAD;
	        TranCode2 = 0;
			SetBegin += TempDownLength1;                                             //设置偏移量
			IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADTEMPFILELENGTH,SetBegin);   //文件已传送长度
			SendLength = 0;												             // Send Length		
			memset(SendBuffer, 0, sizeof(SendBuffer));				               	 // Send Buffer
			SendLength = fnAPP_SendHostMake(TRUE);
			fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);	     //发送流水数据报文
            
			if ((FileEnd == '0')||(OneFileEnd == '0'))        
				break;	
			else
		    if(FileEnd == '1')                                                       //如有多个文件则文件全部传完前,前置下送后续包标志都为1
				continue;
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
	}
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidAdverDownLoadFileProc():return");
    return T_OK;  
}


int CTranCmn::fnAPP_TranAtmBidNoticeDownLoadFileProc(CString TempNameList,int i)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranAtmBidNoticeDownLoadFileProc()");
    FILE*		TempFp;
	int         TempDownLength;
	int         TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
	char        NoticeNewFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	

	SetBegin = 0;
	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(TempNameList1,0,sizeof(TempNameList));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);

	////////////////////////////////////////////////////////////////////////////////////////
	memcpy(&NoticeNewFileName,GetSprintf("%6s%s%s","NOTICE",Int2Asc(i + 1),".jpg"),10+strlen(Int2Asc(i)));
    sprintf(szTempFileNamePath, "%s\\%s", _AP_NOTICEUPDATEDOWNLOAD_DIR, NoticeNewFileName);
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szTempFileNamePath[%s]",szTempFileNamePath);	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
	TempFp = fopen(szTempFileNamePath, "wb");
	if(TempFp)
	{
 		fclose(TempFp);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",szTempFileNamePath);	
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",szTempFileNamePath);
		return T_EXIT;
	}
    
	while (TRUE)
	{   
		fnAPP_InitializeDS();  	
		TranCode = TC_ATMBIDADDOWNLOAD;
		TranCode2 = 0;
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)   //接收提取流水的报文说明
		{
			fnAPP_RecvAnalBidCommand();	

 			if(memcmp(HMERecv.Data[3], "200700", 6) != 0)              //20070703
 			{
 				return T_EXIT;
 			}
			
			memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
	    	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
			memset(szRecvFileName,0,sizeof(szRecvFileName));
			memcpy(szRecvFileName,GetSprintf("%-12s",&HMERecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	 
			memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);
			RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMERecv.Data[122][15]),8);
		    memcpy(&FileEnd,&HMERecv.Data[122][23],1);
			TempDownLength = Asc2Int(&HMERecv.Data[122][24],4);
            TempDownLength1 = Asc2Int(&HMERecv.Data[122][28],3);
            memcpy(&DetailTblTemp,&HMERecv.Data[122][31],TempDownLength1);
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szDownLoadFileName[%s]",szDownLoadFileName);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

			if(RecvSetbegin != SetBegin )
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
				return T_EXIT;
            }			   
		}
		else
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsRecvData TimeOut");
			return T_EXIT;
		}
		
		if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
		{   
			TempFp = fopen(szTempFileNamePath, "ab");
			if (TempFp)
			{   
                MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
                if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
				{
					OneFileEnd = '0';
				}
				fseek(TempFp,SetBegin,0);
				fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
				fclose(TempFp);
					
			}
			
			TranCode = TC_ATMBIDADDOWNLOAD;
	        TranCode2 = 0;
			SetBegin += TempDownLength1;    //设置偏移量			
            IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICETEMPFILELENGTH,SetBegin);   //文件已传送长度
			SendLength = 0;												             // Send Length		
			memset(SendBuffer, 0, sizeof(SendBuffer));				               	 // Send Buffer
			SendLength = fnAPP_SendHostMake(TRUE);
			fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);	     //发送流水数据报文
            
			if ((FileEnd == '0')||(OneFileEnd == '0'))        
				break;	
			else
		    if(FileEnd == '1')                                                       //如有多个文件则文件全部传完前,前置下送后续包标志都为1
				continue;
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
	}
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidNoticeDownLoadFileProc():return");
    return T_OK;  
}

int CTranCmn::fnAPP_TranAtmBidCautionDownLoadFileProc(CString TempNameList,int i)
{   
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::fnAPP_TranAtmBidCautionDownLoadFileProc()");
    FILE*		TempFp;
	int         TempDownLength;
	int         TempDownLength1;            //前置下送数据包长度
	CFileStatus	TempFileStatus;
	char        szTempFileNamePath[256];    //路径文件名
	BYTE        DetailTblTemp[1024];
	char        TempNameList1[21];          
	long        RecvSetbegin;
	char        szRecvFileName[24];
	char        CautionNewFileName[24];
	char        OneFileEnd;
	int         DownFileNameLength;
	

	SetBegin = 0;
	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
	memset(szTempFileNamePath,0,sizeof(szTempFileNamePath));
	memset(TempNameList1,0,sizeof(TempNameList));
	DownFilelength = Asc2Int(TempNameList, 9);
	DownFileNameLength = TempNameList.GetLength() - 9;
 	memcpy(&TempNameList1,GetSprintf("%s",TempNameList),TempNameList.GetLength());
	memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);

	////////////////////////////////////////////////////////////////////////////////////////
	memcpy(&CautionNewFileName,GetSprintf("%6s%s%s","NOTICE",Int2Asc(i + 1),".jpg"),10+strlen(Int2Asc(i)));
    sprintf(szTempFileNamePath, "%s\\%s", _AP_CAUTIONUPDATEDOWNLOAD_DIR, CautionNewFileName);
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szTempFileNamePath[%s]",szTempFileNamePath);	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DownFilelength[%d]",DownFilelength);
	
	TempFp = fopen(szTempFileNamePath, "wb");
	if(TempFp)
	{
 		fclose(TempFp);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Success:[%s]",szTempFileNamePath);	
	}
	else
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Create File Fail:[%s] ",szTempFileNamePath);
		return T_EXIT;
	}
    
	while (TRUE)
	{   
		fnAPP_InitializeDS();  	
		TranCode = TC_ATMBIDADDOWNLOAD;
		TranCode2 = 0;
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)   //接收提取流水的报文说明
		{
			fnAPP_RecvAnalBidCommand();	

 			if(memcmp(HMERecv.Data[3], "200700", 6) != 0)              //20070703
 			{
 				return T_EXIT;
 			}
			
			memset(&DetailTblTemp, ' ', sizeof(DetailTblTemp));
	    	memset(szDownLoadFileName,0,sizeof(szDownLoadFileName));
			memset(szRecvFileName,0,sizeof(szRecvFileName));
			memcpy(szRecvFileName,GetSprintf("%-12s",&HMERecv.Data[122][3]).GetBuffer(0), DownFileNameLength);	 
			memcpy(&szDownLoadFileName,GetSprintf("%s",&TempNameList1[9]),DownFileNameLength);
			RecvSetbegin = Asc2Int(GetSprintf("%08s", &HMERecv.Data[122][15]),8);
		    memcpy(&FileEnd,&HMERecv.Data[122][23],1);
			TempDownLength = Asc2Int(&HMERecv.Data[122][24],4);
            TempDownLength1 = Asc2Int(&HMERecv.Data[122][28],3);
            memcpy(&DetailTblTemp,&HMERecv.Data[122][31],TempDownLength1);
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szRecvFileName[%s]",szRecvFileName);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szDownLoadFileName[%s]",szDownLoadFileName);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvSetbegin[%d]",RecvSetbegin);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Setbegin[%d]",SetBegin);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempDownLength1[%d]",TempDownLength1);

			if(RecvSetbegin != SetBegin )
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecvFileName or RecvSetbegin error");
				return T_EXIT;
            }			   
		}
		else
		{   
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NmsRecvData TimeOut");
			return T_EXIT;
		}
		
		if (CFile::GetStatus(szTempFileNamePath, TempFileStatus))
		{   
			TempFp = fopen(szTempFileNamePath, "ab");
			if (TempFp)
			{   
                MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownFilelength - SetBegin[%d]",DownFilelength - SetBegin);
                if(DownFilelength - SetBegin <= 400)                                //每次固定传400字节
				{
					OneFileEnd = '0';
				}
				fseek(TempFp,SetBegin,0);
				fwrite(&DetailTblTemp, TempDownLength1, 1, TempFp);
				fclose(TempFp);
					
			}
			
			TranCode = TC_ATMBIDADDOWNLOAD;
	        TranCode2 = 0;
			SetBegin += TempDownLength1;    //设置偏移量			
            IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONTEMPFILELENGTH,SetBegin);   //文件已传送长度
			SendLength = 0;												             // Send Length		
			memset(SendBuffer, 0, sizeof(SendBuffer));				               	 // Send Buffer
			SendLength = fnAPP_SendHostMake(TRUE);
			fnAPP_NmsSendData(SendBuffer, SendLength, InitInfo.LineSendTime);	     //发送流水数据报文
            
			if ((FileEnd == '0')||(OneFileEnd == '0'))        
				break;	
			else
		    if(FileEnd == '1')                                                       //如有多个文件则文件全部传完前,前置下送后续包标志都为1
				continue;
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CFile::GetStatus[%d]",CFile::GetStatus(szTempFileNamePath, TempFileStatus));
	}
    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidCautionDownLoadFileProc():return");
    return T_OK;  
}

// Atm DC Bid Send Ejr Procedure
int CTranCmn::fnAPP_TranAtmBidDCSendEjrProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidDCSendEjrProc()");

	fnAPP_InitializeDS();
	TranCode = TC_ATMBIDDCSENDEJR;
	TranCode2 = 0;

	fnAPP_MainTranProc(TRAN_DCSENDEJR);							// 2006.07.03

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAtmBidDCSendEjrProc():return");
	return T_OK;
}

// DC Send Ejr Procedure
int CTranCmn::fnAPP_TranDCSendEjrProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDCSendEjrProc()");

	fnAPP_InitializeDS();
	TranCode = TC_DCSENDEJR;
	TranCode2 = 0;

	int		nCount = 0;

	NmsRecvLength = 0;
	memset(NmsRecvBuffer, 0, sizeof(NmsRecvBuffer));

	nCount = fnAPL_GetDCEjrCount();								// 2006.07.03
	if (nCount)
	{
		memcpy(NmsSendBuffer, "0004", 4);
		memcpy(&NmsSendBuffer[4], GetSprintf("%04d", nCount), 4);
		NmsSendLength = 8;
		fnAPP_NmsSendData(NmsSendBuffer, NmsSendLength, LINE_BID_SEND_TIME);
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)
		{
			fnAPP_RecvAnalDCBidCommand();
			if (TranCode == TC_DCSENDEJR)
				;
			else 
				return T_OK;
		}
		else
			return T_OK;

		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);
		
		while (TRUE)
		{
			if (fnAPL_GetDCTransactionEjr())
			{
				fnAPL_SendMake();
				fnAPP_NmsSendData(NmsSendBuffer, NmsSendLength, LINE_BID_SEND_TIME);
				NmsRecvLength = 0;
				memset(NmsRecvBuffer, 0, sizeof(NmsRecvBuffer));
				if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)
				{
					fnAPP_RecvAnalDCBidCommand();
					if (TranCode == TC_DCSENDEJR)
						;
					else
						break;
				}
				else
					break;
			}
			else
			{
				fnAPP_MainTranProc(TRAN_DCEJREND);
				break;
			}

		}
	}
	else
	{
		memcpy(NmsSendBuffer, "0004", 4);
		memcpy(&NmsSendBuffer[4], ZERO4, 4);
		NmsSendLength = 8;
		fnAPP_NmsSendData(NmsSendBuffer, NmsSendLength, LINE_BID_SEND_TIME);
		if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, LINE_BID_RECV_TIME) == T_OK)
		{
			fnAPP_RecvAnalDCBidCommand();
			if (TranCode == TC_DCSENDEJR)
				fnAPP_MainTranProc(TRAN_DCEJREND);
		}
	}
	
	IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDCSendEjrProc():return");
	return T_OK;
}

// DC Ejr End Procedure
int CTranCmn::fnAPP_TranDCEjrEndProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDCEjrEndProc()");

	fnAPP_InitializeDS();
	TranCode = TC_DCEJREND;
	TranCode2 = 0;

	fnAPL_SendMake();											// 2006.07.03
	fnAPP_NmsSendData(NmsSendBuffer, NmsSendLength, LINE_BID_SEND_TIME);
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranDCEjrEndProc():return");
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Transaction Function
/////////////////////////////////////////////////////////////////////////////
// Initialize Procedure
int CTranCmn::fnAPP_TranInitProc()
{
	MainTranDoingFlag = TRUE;									// Main Transaction Doing Flag
	MainTranDoingCount = 0;										// Main Transaction Doing Count
	return T_OK;
}

// Menu Procedure
int CTranCmn::fnAPP_TranMenuProc()
{
	fnAPP_MenuProc();											// Main Procedure
	return T_OK;
}

// Get Transaciton Procedure
int CTranCmn::fnAPP_TranGetTranProc()
{
	switch (TranProc)
	{
		case TRAN_WITH:											// WITHDRAW
			m_nRetCode = RET_WITHDRAW;
			break;

		case TRAN_INQ:											// INQUIRY
			m_nRetCode = RET_INQUIRY;
			break;

		case TRAN_TRANS:										// TRANSFER
			m_nRetCode = RET_TRANSFER;
			break;

		case TRAN_DEP:											// DEPOSIT
			m_nRetCode = RET_DEPOSIT;
			break;

		case TRAN_PB:											// PASSBOOK
			m_nRetCode = RET_PASSBOOK;
			break;

		case TRAN_IC:											// IC
			m_nRetCode = RET_IC;
			break;

		default: 
			fnAPP_CancelProc(T_PROGRAM);
			break;
	}
	return T_OK;
}

// Get Menu Doing Procedure
int CTranCmn::fnAPP_TranGetMenuDoingProc()
{
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	Transaction Withdraw Function
/////////////////////////////////////////////////////////////////////////////
// Withdraw Initialize Procedure
int CTranCmn::fnAPP_TranWithInitProc()
{
	MainTranDoingFlag = FALSE;									// Defualt FALSE
	return T_OK;
}

// Withdraw Input Procedure
int CTranCmn::fnAPP_TranWithInputProc()
{
	if (MainTranDoingCount == 0)
	{
		fnAPL_DvrSendDataProc("TT");							// 2005.06.15
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		fnAPP_AcceptWith();										// Accept Withdraw
		if (MainTranReturnFlag)
			return T_OK;
	}
	return T_OK;
}

// Withdraw Output Procedure
int CTranCmn::fnAPP_TranWithOutputProc()
{
	if (m_pProfile->DEVICE.NmsDevice == 1)						// ZL NMS : 2006.04.19
	{
		fnAPL_SendHost();										// 2006.07.03
		fnAPL_WriteDCTransactionEjr();
	}

	if (m_pDevCmn->TranResult)
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(2);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSUM, 1);
		fnAPP_SumProc(0);										// Sum Procedure(Host)
	}
	else
	{
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(20);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
	}

	if (m_pDevCmn->TranResult)									// Host Ok
	{
		CashCountErrorFlag = 1;									// added by liuxl 20110805
		CashRetractFlag = TRUE;									// 2011.01.04
		fnAPD_MoneyCountInitialize();							// Money Count Initialize
		fnAPD_MoneyCount();										// Money Count
		fnAPD_MoneyCountCheck();								// Money Count Check : 2005.04.07
		fnAPD_CheckDeviceAction(DEV_CSH);	
		fnAPD_MoneyCountPrint();								// 2005.02.24

/////////////////////////////////////////////////////////////////////////////
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0); // added by liuxl 20110714
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
/////////////////////////////////////////////////////////////////////////////
		fnAPP_SumProc(1);										// Sum Procedure(Atm)
		m_pDevCmn->fnAPL_SetProcCount('6');						// Shutter Open(Withdraw Only)
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(4);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		fnAPL_SaveDetailData();									// 2005.08.22
/////////////////////////////////////////////////////////////////////////////
//		m_pDevCmn->TranStatus = TRAN_CONFIRMWITH;
//		TranCode = TC_CONFIRMWITH;
//		fnAPP_SendHost();										// Non Stop : 2005.08.22
//		fnAPP_RecvHost();										// Non Stop : 2005.08.22
/////////////////////////////////////////////////////////////////////////////
//		if (m_pProfile->DEVICE.NmsDevice == 1)					// ZL NMS : 2006.04.19
//		{
//			fnAPL_SendHost();									// 2006.07.03(限于有取款确认交易时加到这儿)
//		}
/////////////////////////////////////////////////////////////////////////////
		m_pDevCmn->TranStatus = TRAN_TRAN;
		TranCode = TC_WITHDRAWAL;
		CashCountErrorFlag = 0;									// added by liuxl 20110805
/////////////////////////////////////////////////////////////////////////////
		fnAPD_MoneyOutCheck(0, InitInfo.TakeCashTimeOut);		// Money Out Check
/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(5);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		bAgentWithFlag = TRUE;									// 出钞成功才往AGENT发送成功  2009.04.01 yusy
	}
	else
	{
/////////////////////////////////////////////////////////////////////////////
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0); // added by liuxl 20110714
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
/////////////////////////////////////////////////////////////////////////////
	}

    if(m_pDevCmn->bAgentMode == 1)
	{
		TranWriteFlag = TRUE;
    	fnAPP_AgentIdxGetData(AGENT_WITHDRAWAL);				// 20080319  by hyl
	}


	if (m_pProfile->DEVICE.NmsDevice == 2)						// DC NMS : 2006.06.16
	{
		fnAPL_SendHost();										// 2006.07.03
		fnAPL_WriteDCTransactionEjr();
	}

	fnAPP_PSPProc(DEV_JPR);										// Jpr Print

	if (m_pDevCmn->TranResult)									// Host Ok
	{
		fnAPP_PMEAcceptSlipSelect();							// Accept Slip Select
		fnAPP_PSPProc(DEV_SPR);									// Spr Print
		fnAPD_CheckDeviceAction(DEV_SPR);
		fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);	// Spr Out
		fnAPD_MaterialOutCheckRetract(DEV_SPR);					// Spr Out Check & Retract
		fnAPP_PMEAcceptTranContinue();							// Accept Tran Continue
	}
	else														// Host Ng
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
			MenuDoingFlag = FALSE;
		}
		else
		if (HostResp.CardEjectFlag)								// Card Eject Flag
		{
			fnAPP_PMDProc();									// Message Display
//			fnAPP_PSPProc(DEV_SPR);								// Spr Print
//			fnAPD_CheckDeviceAction(DEV_SPR | DEV_MCU);
//			fnAPD_MaterialOut(DEV_SPR | DEV_MCU, InitInfo.TakeCardTimeOut);
//																// Spr Card Out
//			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);	// Spr Card Out Check & Retract
			//交易主机拒绝不打收条   by zjd
			fnAPD_CheckDeviceAction(DEV_MCU);
			fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Spr Card Out
			fnAPD_MaterialOutCheckRetract(DEV_MCU);	// Spr Card Out Check & Retract
			MenuDoingFlag = FALSE;
		}	
		else
		{
			if ((memcmp(HostResp.RejectCode, "26", 2) == 0) && (MainTranDoingCount < InitInfo.TransRetryTime))		// PassWord Miss
			{
				fnAPP_PMEAcceptPassWord();						// Accept PassWord
				MainTranDoingFlag = TRUE;
				MainTranDoingCount++;
			}
			else
			{													// 2005.11.11
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();							// Message Display
//				        fnAPP_PSPProc(DEV_SPR);							// Spr Print
//				        fnAPD_CheckDeviceAction(DEV_SPR | DEV_MCU);
//				        fnAPD_MaterialOut(DEV_SPR | DEV_MCU, InitInfo.TakeCardTimeOut);
//																// Spr Card Out
//				        fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);	
//																// Spr Card Out Check & Retract
				        //交易主机拒绝不打收条   by zjd
				        fnAPD_CheckDeviceAction(DEV_MCU);
				        fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Spr Card Out
				        fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																// Spr Card Out Check & Retract
					MenuDoingFlag = FALSE;
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}

	return T_OK;
}

// Withdraw Check Device Action Procedure
int CTranCmn::fnAPP_TranWithCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU | DEV_CSH);
	return T_OK;
}

// Withdraw Check Menu Continue Procedure
int CTranCmn::fnAPP_TranWithCheckMenuContinueProc()
{
	if ((MenuDoingFlag)		||
		(MainTranDoingFlag))
	{
		if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||	// 2005.11.11
			(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE)	||
			(fnAPP_MainMenuContinueCheck() != T_OK))
		{
			MenuDoingFlag = FALSE;
			MainTranDoingFlag = FALSE;
			fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Card Out			
			fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
		}
	}
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	Transaction Inquiry Function
/////////////////////////////////////////////////////////////////////////////
// Inquiry Initialize Procedure
int CTranCmn::fnAPP_TranInqInitProc()
{
	MainTranDoingFlag = FALSE;									// Defualt FALSE
	
	// 20080905 by yaokq for绿卡通
	ZzhTotalNum = 0;
	ZzhCurNum = 0;
	
	return T_OK;
}

// Inquiry Input Procedure
int CTranCmn::fnAPP_TranInqInputProc()
{
	if (MainTranDoingCount == 0)
	{
		fnAPL_DvrSendDataProc("TT");							// 2005.06.15
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		fnAPP_AcceptInq();										// Accept Inquiry
		if (MainTranReturnFlag)
			return T_OK;
	}
	return T_OK;
}

// Inquiry Output Procedure
int CTranCmn::fnAPP_TranInqOutputProc()
{
	if(m_pDevCmn->bAgentMode == 1)							// added by yusy 08.09.09
	{
		if(TranCode == TC_INQUIRY)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_INQUIRY);
		}
		else
		if(TranCode == TC_INQUIRY_CheckPin)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_INQUIRY_CheckPin);
		}
		else
		if(TranCode == TC_CHANGEPIN)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_CHANGEPIN);
		}
		else
		if(TranCode == TC_DETAIL)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_DETAIL);
		}
		else
		if(TranCode == TC_OTHER_INQ)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_OTHER_INQ);
		}
	}

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->TranResult)
	{
		if (TranCode == TC_INQUIRY||
			TranCode == TC_INQUIRY_CheckPin)
		{
			fnAPP_SumProc(2);									// Sum Procedure(Host)
			if (LocalCardFlag == 3)								// 跨行查询交易需要参与对帐交易   by zjd
				fnAPL_SaveDetailData();
		}
		else
		if (TranCode == TC_CHANGEPIN)
			fnAPP_SumProc(3);									// Sum Procedure(Host)
		else
		if (TranCode == TC_DETAIL)
			fnAPP_SumProc(5);									// Sum Procedure(Host)
	}

	fnAPP_PSPProc(DEV_JPR);										// Jpr Print

	if (m_pDevCmn->TranResult)									// Host Ok
	{
		if (TranCode == TC_INQUIRY||
			TranCode == TC_INQUIRY_CheckPin)
			fnAPP_PMEAcceptInquiryConfirm();					// Accept Inquiry Confirm
		else
		if (TranCode == TC_CHANGEPIN)
			fnAPP_PMEAcceptChangePassWord();					// Accept Change PassWord
		else
		if (TranCode == TC_DETAIL)
			fnAPP_PMEAcceptDetailConfirm();						// Accept Detail Confirm
		else
		if(TranCode == TC_OTHER_INQ)							// 20080904 by yaokq for 绿卡通子账户查询
			fnAPP_PMEAcceptOtherConfirm();
	}
	else														// Host Ng
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
			MenuDoingFlag = FALSE;
		}
		else
		if (HostResp.CardEjectFlag)								// Card Eject Flag
		{
			fnAPP_PMDProc();									// Message Display
//			fnAPP_PSPProc(DEV_SPR);								// Spr Print
//			fnAPD_CheckDeviceAction(DEV_SPR | DEV_MCU);
//			fnAPD_MaterialOut(DEV_SPR | DEV_MCU, InitInfo.TakeCardTimeOut);
//																// Spr Card Out
//			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);	// Spr Card Out Check & Retract
			//交易主机拒绝不打收条   by zjd
			fnAPD_CheckDeviceAction(DEV_MCU);
			fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Spr Card Out
			fnAPD_MaterialOutCheckRetract(DEV_MCU);	// Spr Card Out Check & Retract
			MenuDoingFlag = FALSE;
		}	
		else
		{
			if ((memcmp(HostResp.RejectCode, "26", 2) == 0) && (MainTranDoingCount < InitInfo.TransRetryTime))		// PassWord Miss
			{
				if(TranCode == TC_CHANGEPIN)
					fnAPP_AcceptNewPassWord(1);
				else
					fnAPP_PMEAcceptPassWord();						// Accept PassWord
				MainTranDoingFlag = TRUE;
				MainTranDoingCount++;
			}
			else
			{													// 2005.11.11
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();							// Message Display
//				fnAPP_PSPProc(DEV_SPR);							// Spr Print
//				fnAPD_CheckDeviceAction(DEV_SPR | DEV_MCU);
//				fnAPD_MaterialOut(DEV_SPR | DEV_MCU, InitInfo.TakeCardTimeOut);
//																// Spr Card Out
//				fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);	
//																// Spr Card Out Check & Retract
				//交易主机拒绝不打收条   by zjd
				fnAPD_CheckDeviceAction(DEV_MCU);
				fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Spr Card Out
				fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																// Spr Card Out Check & Retract
					MenuDoingFlag = FALSE;
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	return T_OK;
}

// Inquiry Check Device Action Procedure
int CTranCmn::fnAPP_TranInqCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);
	return T_OK;
}

// Inquiry Check Menu Continue Procedure
int CTranCmn::fnAPP_TranInqCheckMenuContinueProc()
{
	if ((MenuDoingFlag)		||
		(MainTranDoingFlag))
	{
		if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||	// 2005.11.11
			(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE)	||
			(fnAPP_MainMenuContinueCheck() != T_OK))
		{
			MenuDoingFlag = FALSE;
			MainTranDoingFlag = FALSE;
			fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Card Out			
			fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
		}
	}
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	Transaction Transfer Function
/////////////////////////////////////////////////////////////////////////////
// Transfer Initialize Procedure
int CTranCmn::fnAPP_TranTransInitProc()
{
	MainTranDoingFlag = FALSE;									// Defualt FALSE
	return T_OK;
}

// Transfer Input Procedure
int CTranCmn::fnAPP_TranTransInputProc()
{
	if (MainTranDoingCount == 0)
	{
		fnAPL_DvrSendDataProc("TT");							// 2005.06.15
		fnAPP_InitializeSerialTranDS();							// Initialize Serial Transaction DS
		fnAPP_AcceptTrans();									// Accept Transfer
		if (MainTranReturnFlag)
			return T_OK;
	}
	return T_OK;
}

// Transfer Output Procedure
int CTranCmn::fnAPP_TranTransOutputProc()
{
	if(m_pDevCmn->bAgentMode == 1)							// added by yusy 08.09.09
	{
		if (TranCode == TC_TRANSFER)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_TRANSFER);
		}
		else 
		if (TranCode == TC_PAY)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_PAY);
		}
		else
		if (TranCode == TC_REMIT_ADD)										// 2009.08.06 yusy
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_REMIT_ADD);
		}
		else
		if (TranCode == TC_REMIT_PWD)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_REMIT_PWD);
		}
		else
		if (TranCode == TC_REMIT_IN)										// 2009.07.17 yusy
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_REMIT_IN);
		}
		else 
		if (TranCode == TC_TZH)												// added by yusy 2009.05.13
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_TZH);
		}
		else 
		if (TranCode == TC_HZT)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_HZT);
		}
		else
		if (TranCode == TC_SMSPARITY)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_SMSPARITY);
		}
		else
		if (TranCode == TC_ECHANNEL_INQ)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_ECHANNEL_INQ);
		}
		else
		if (TranCode == TC_ECHANNEL)
		{
			TranWriteFlag = TRUE;
			fnAPP_AgentIdxGetData(AGENT_ECHANNEL);
		}
	}

/////////////////////////////////////////////////////////////////////////////
	fnAPL_SendHost();											// 2006.07.03
	fnAPL_WriteDCTransactionEjr();
/////////////////////////////////////////////////////////////////////////////
	
	if (m_pDevCmn->TranResult)
	{
// 		if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD || TranCode == TC_REMIT_IN)	// 2008.10.13 added by liuxl 20110714
// 		{
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
// 		}

/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(2);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)		//liyi add 绿卡通
			fnAPP_SumProc(4);									// Sum Procedure(Host)
		else
		if (TranCode == TC_LOAD)
			fnAPP_SumProc(15);
		else
		if (TranCode == TC_PAY)
			fnAPP_SumProc(6);									// Sum Procedure(Host)
		else
		if (TranCode == TC_REMIT_ADD)
			fnAPP_SumProc(7);									// Sum Procedure(Host)
		else
		if (TranCode == TC_REMIT_IN)
			fnAPP_SumProc(8);									// Sum Procedure(Host)
		else
		if (TranCode == TC_REMIT_PWD)
			fnAPP_SumProc(9);									// Sum Procedure(Host)
//		else
//		if (TranCode == TC_AC_REMIT)
//			fnAPP_SumProc(10);									// Sum Procedure(Host)
//		else
//		if (TranCode == TC_AC_PAY_ASSU)
//			fnAPP_SumProc(11);									// Sum Procedure(Host)
//		else
//		if (TranCode == TC_AC_PAY_NOASSU)
//			fnAPP_SumProc(12);									// Sum Procedure(Host)
//		else
//		if (TranCode == TC_AC_PAY_ASSU)
//			fnAPP_SumProc(13);									// Sum Procedure(Host)
//		else
//		if (TranCode == TC_AC_PAY_NOASSU)
//			fnAPP_SumProc(14);									// Sum Procedure(Host)

		fnAPL_SaveDetailData();									// 2005.08.22

	}
	else
	{
// 		if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD || TranCode == TC_REMIT_IN)	// 2008.10.13 added by liuxl 20110714
// 		{
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
// 		}

/////////////////////////////////////////////////////////////////////////////
		fnAPL_WriteTransactonEjr(20);							// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
	}

	fnAPP_PSPProc(DEV_JPR);										// Jpr Print


	if (m_pDevCmn->TranResult)									// Host Ok
	{
		if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH) // liyi add 绿卡通
		{
			transInqCnt = 0;
			fnAPP_PMEAcceptSlipSelect();						// Accept Slip Select
			fnAPP_PSPProc(DEV_SPR);								// Spr Print
			fnAPD_CheckDeviceAction(DEV_SPR);
			fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);// Spr Out
			fnAPD_MaterialOutCheckRetract(DEV_SPR);				// Spr Out Check & Retract
			fnAPP_PMEAcceptTranContinue();							// Accept Tran Continue
		}
		else
		if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_IN || TranCode == TC_REMIT_PWD)		// 2008.10.05
		{
			fnAPP_PMEAcceptRemit();								// 汇款成功
//			fnAPP_PMEAcceptSlipSelect();						// Accept Slip Select
			Accept.SlipSelectFlag = TRUE;
			fnAPP_PSPProc(DEV_SPR);								// Spr Print
			fnAPD_CheckDeviceAction(DEV_SPR);
			fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);// Spr Out
			fnAPD_MaterialOutCheckRetract(DEV_SPR);				// Spr Out Check & Retract
		}
		else
		if (TranCode == TC_PAY || TranCode == TC_ECHANNEL)
		{
			fnAPP_PMEAcceptSlipSelect();						// Accept Slip Select
			fnAPP_PSPProc(DEV_SPR);								// Spr Print
			fnAPD_CheckDeviceAction(DEV_SPR);
			fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);// Spr Out
			fnAPD_MaterialOutCheckRetract(DEV_SPR);				// Spr Out Check & Retract
			fnAPP_PMEAcceptTranContinue();
		}
		else
		if (TranCode == TC_LOAD)								// added by yusy ic
		{
			fnAPP_PMEAcceptSlipSelect();						// Accept Slip Select
			fnAPP_PSPProc(DEV_SPR);								// Spr Print
			fnAPD_CheckDeviceAction(DEV_SPR);
			fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);// Spr Out
			fnAPD_MaterialOutCheckRetract(DEV_SPR);				// Spr Out Check & Retract
			fnAPP_PMEAcceptTranContinue();						// added by liuxl 20110512
		}
		else
		if (TranCode == TC_ICINQUIRY)
			fnAPP_PMEAcceptIcInquiryConfirm();					// Accept Ic Inquiry Confirm
		else
		if (TranCode == TC_ICDETAIL)
			fnAPP_PMEAcceptIcDetailConfirm();					// Accept Ic Detail Confirm
		else
		if (TranCode == TC_BOUNDMDOIFY)
			fnAPP_PMEAcceptIcLimitConfirm();						// Accept Slip Select
	}
	else														// Host Ng
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
			MenuDoingFlag = FALSE;
		}
		else
		if (HostResp.CardEjectFlag)								// Card Eject Flag
		{
			fnAPP_PMDProc();									// Message Display
//			fnAPP_PSPProc(DEV_SPR);								// Spr Print
//			fnAPD_CheckDeviceAction(DEV_SPR | DEV_MCU);
//			fnAPD_MaterialOut(DEV_SPR | DEV_MCU, InitInfo.TakeCardTimeOut);
//																// Spr Card Out
//			fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);	// Spr Card Out Check & Retract
			//交易主机拒绝不打收条   by zjd
			fnAPD_CheckDeviceAction(DEV_MCU);
			fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Spr Card Out
			fnAPD_MaterialOutCheckRetract(DEV_MCU);	// Spr Card Out Check & Retract
			MenuDoingFlag = FALSE;
		}	
		else
		{
			if ((memcmp(HostResp.RejectCode, "26", 2) == 0) && (MainTranDoingCount < InitInfo.TransRetryTime))		// PassWord Miss
			{
				fnAPP_PMEAcceptPassWord();						// Accept PassWord
				MainTranDoingFlag = TRUE;
				MainTranDoingCount++;
			}
			else
			{													// 2005.11.11
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();							// Message Display
//				fnAPP_PSPProc(DEV_SPR);							// Spr Print
//				fnAPD_CheckDeviceAction(DEV_SPR | DEV_MCU);
//				fnAPD_MaterialOut(DEV_SPR | DEV_MCU, InitInfo.TakeCardTimeOut);
//																// Spr Card Out
//				fnAPD_MaterialOutCheckRetract(DEV_SPR | DEV_MCU);	
//																// Spr Card Out Check & Retract
				//交易主机拒绝不打收条   by zjd
				fnAPD_CheckDeviceAction(DEV_MCU);
				fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Spr Card Out
				fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																// Spr Card Out Check & Retract
					MenuDoingFlag = FALSE;
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	return T_OK;
}

// Transfer Check Device Action Procedure
int CTranCmn::fnAPP_TranTransCheckDeviceActionProc()
{
	fnAPD_CheckDeviceAction(DEV_SPR | DEV_JPR | DEV_MCU);
	return T_OK;
}

// Transfer Check Menu Continue Procedure
int CTranCmn::fnAPP_TranTransCheckMenuContinueProc()
{
	if ((MenuDoingFlag)		||
		(MainTranDoingFlag))
	{
		if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||	// 2005.11.11
			(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE)	||
			(fnAPP_MainMenuContinueCheck() != T_OK))
		{
			MenuDoingFlag = FALSE;
			MainTranDoingFlag = FALSE;
			fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Card Out			
			fnAPD_MaterialOutCheckRetract(DEV_MCU);				// Card Out Check & Retract
		}
	}
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	Transaction Deposit Function
/////////////////////////////////////////////////////////////////////////////
// Deposit Initialize Procedure
int CTranCmn::fnAPP_TranDepInitProc()
{
	return T_OK;
}

// Deposit Input Procedure
int CTranCmn::fnAPP_TranDepInputProc()
{
	return T_OK;
}

// Deposit Output Procedure
int CTranCmn::fnAPP_TranDepOutputProc()
{
	return T_OK;
}

// Deposit Check Device Action Procedure
int CTranCmn::fnAPP_TranDepCheckDeviceActionProc()
{
	return T_OK;
}

// Deposit Check Menu Continue Procedure
int CTranCmn::fnAPP_TranDepCheckMenuContinueProc()
{
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	Transaction Passbook Function
/////////////////////////////////////////////////////////////////////////////
// Passbook Initialize Procedure
int CTranCmn::fnAPP_TranPbInitProc()
{
	return T_OK;
}

// Passbook Input Procedure
int CTranCmn::fnAPP_TranPbInputProc()
{
	return T_OK;
}

// Passbook Output Procedure
int CTranCmn::fnAPP_TranPbOutputProc()
{
	return T_OK;
}

// Passbook Check Device Action Procedure
int CTranCmn::fnAPP_TranPbCheckDeviceActionProc()
{
	return T_OK;
}

// Passbook Check Menu Continue Procedure
int CTranCmn::fnAPP_TranPbCheckMenuContinueProc()
{
	return T_OK;
}


/////////////////////////////////////////////////////////////////////////////
//	Transaction Ic Function
/////////////////////////////////////////////////////////////////////////////
// Ic Initialize Procedure
int CTranCmn::fnAPP_TranIcInitProc()
{
	return T_OK;
}

// Ic Input Procedure
int CTranCmn::fnAPP_TranIcInputProc()
{
	return T_OK;
}

// Ic Output Procedure
int CTranCmn::fnAPP_TranIcOutputProc()
{
	return T_OK;
}

// Ic Check Device Action Procedure
int CTranCmn::fnAPP_TranIcCheckDeviceActionProc()
{
	return T_OK;
}

// Ic Check Menu Continue Procedure
int CTranCmn::fnAPP_TranIcCheckMenuContinueProc()
{
	return T_OK;
}

int	CTranCmn::fnAPP_EditECProtDataProc(int ProtOrder)
{
	FILE	*fp;
	char	szTempFile[100], szTempData[1000];
	int		i;
	
	
	memset(szTempFile, 0x00, sizeof(szTempFile));
	sprintf(szTempFile, "%s", "D:\\ECTempData.txt");
	
	fp = fopen(szTempFile, "r");
	if (fp == NULL)
		return FALSE;
	
	i = 0;
	
	while(!feof(fp))
	{	
		memset(szTempData, 0x00, sizeof(szTempData));	
		fgets(szTempData, 200, fp);
		if (strlen(szTempData) <= 1)
			continue;
		
		if (memcmp(&szTempData[strlen(szTempData) - 1], "\n", 1) == 0)
			szTempData[strlen(szTempData) - 1] = 0;

		i++;
		IniSetStr(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dRow%04d", ProtOrder + 1, i), szTempData);

		if (i >= 9999)
			break;
	}
	
	IniSetInt(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dRowNUM", ProtOrder + 1), i);
	
	fclose(fp);

	return TRUE;
}


// Copy directory : new version 
//BOOL MyCopyDirectoryEx(LPCTSTR lpszFrom, LPCTSTR lpszTo) 
//{ 
//	CString strFindFile = lpszFrom; 
//	strFindFile.TrimRight(); 
//	strFindFile.TrimRight(_T('\\')); 
//	strFindFile += _T("\\*.*"); 
//
//	CFileFind ff; 
//	BOOL bWorking = ff.FindFile(strFindFile); 
//
//	while( bWorking ) 
//	{ 
//		bWorking = ff.FindNextFile(); 
//		if( ff.IsDots() ) continue; 
//
//		// Copy file 
//		CString strTo = lpszTo; 
//		strTo.TrimRight(); 
//		strTo.TrimRight(_T('\\')); 
//		strTo += _T("\\"); 
//
//		if (!CopyFile(ff.GetFilePath(), strTo + ff.GetFileName(), FALSE)) 
//			return FALSE; 
//	} 
//
//} 
