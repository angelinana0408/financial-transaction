 /////////////////////////////////////////////////////////////////////////////
// AtmHostCtrl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
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
// Clerk Procedure
int	CWinAtmCtrl::ClerkProc()
{
/////////////////////////////////////////////////////////////////////////////
	static	int		nOperatorSwitch = FALSE;
	static	int		nOperatorMode = FALSE;
	static	int		nOperatorSwitchStatus = OPER_NONE;

/////////////////////////////////////////////////////////////////////////////
	if (!m_pDevCmn->fnAPM_ClerkProc())							// Clerk Procedure
		return TRUE;
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->AtmStatus != m_pDevCmn->AtmStatusSave)		// Difference Mode
		return TRUE;

	if (m_pDevCmn->AtmStatus == ATM_TRAN)
		return TRUE;
																// 2005.04.19
	nOperatorSwitchStatus = m_pDevCmn->fnSNS_GetOperatorSwitch();

// 	m_pTranCmn->nAgentOperatorMode = FALSE;
	if (nOperatorSwitch)
	{
/////////////////////////////////////////////////////////////////////////////
		m_pTranCmn->ErrorSendFlag = FALSE;						// 2008.11.14
		nOperatorSwitch = FALSE;								// Next Clerk Procedure Stop
		nOperatorMode = TRUE;									// Operator Mode Setting
/////////////////////////////////////////////////////////////////////////////
       	   
		m_pTranCmn->nAgentOperatorMode = TRUE;                  
		m_pTranCmn->nAgentAtmAfterOpenFistFailFlag = FALSE;                      // 20080416 by hyl

		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ENTER OPERATOR ADMIN",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 进入后台管理模式",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
        if(m_pDevCmn->bAgentMode == 1)
            m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_ENTER_ADMIN);


		switch (m_pDevCmn->AtmDefine.OpDevice)					// 2005.11.11
		{
			case NOT_SET:
				SetShareData("OP_MODE", "OP");
				break;
			case OP_TOUCH:
				SetShareData("OP_MODE", "");
				break;
			case OP_TSPL:
				SetShareData("OP_MODE", "");
				break;
			case OP_TSPLCHN:
				SetShareData("OP_MODE", "");
				break;
			case OP_HSPL:
				SetShareData("OP_MODE", "");
				break;
		}
		SetShareData("OP_OPSTATUS", "OP");						// 2006.01.10
	}
	else
	if (nOperatorMode)
		;
//	else
//	if (m_pDevCmn->AtmDefine.OpDevice == OP_TOUCH)				// 2006.06.02
//		;
	else
	{
		if (m_pDevCmn->AtmDefine.SplDevice == SPL_ONOFFSW)		// 2006.06.02
		{
			if ((nOperatorSwitchStatus == OPER_SUPERVISOR) ||
				(nOperatorSwitchStatus == OPER_SUPERVISORCHANGED))
			{
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->ErrorSendFlag = FALSE;				// 2008.11.14
				nOperatorSwitch = TRUE;							// Next Clerk Procedure Run
				m_pDevCmn->OpenKey = FALSE;						// Custom => Clerk Mode
				m_pTranCmn->InitInfo.BidAtmColseFlag = FALSE;	// 2005.09.11
/////////////////////////////////////////////////////////////////////////////

				m_pDevCmn->fnAPL_UnUsed();						// Un Used Mode Setting
				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 27)
					m_pDevCmn->fnSCR_DisplayScreen(27);
				if (m_pDevCmn->AtmStatus == ATM_CUSTOM)
					m_pTranCmn->setAtmStatus = 0;
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->HostNms.OexOpenBySpvSendFlag = TRUE;// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				return TRUE;
			}
		}
		else														
		if (m_pDevCmn->AtmDefine.SplDevice == SPL_TOGGLESW)		// 2006.06.02
		{
			if ((nOperatorSwitchStatus == OPER_SUPERVISORCHANGED) ||
				(nOperatorSwitchStatus == OPER_RUNCHANGED))
			{
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->ErrorSendFlag = FALSE;				// 2008.11.14
				nOperatorSwitch = TRUE;							// Next Clerk Procedure Run
				m_pDevCmn->OpenKey = FALSE;						// Custom => Clerk Mode
				m_pTranCmn->InitInfo.BidAtmColseFlag = FALSE;	// 2005.09.11
/////////////////////////////////////////////////////////////////////////////

				m_pDevCmn->fnAPL_UnUsed();						// Un Used Mode Setting
				if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 27)
					m_pDevCmn->fnSCR_DisplayScreen(27);
				if (m_pDevCmn->AtmStatus == ATM_CUSTOM)
					m_pTranCmn->setAtmStatus = 0;
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->HostNms.OexOpenBySpvSendFlag = TRUE;// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				return TRUE;
			}
		}
	}
	
/////////////////////////////////////////////////////////////////////////////
	// OP_POWEROFF
	// OP_REBOOT
	// OP_POWEROFFSTATUS
	// OP_READY
	// OP_RESET
	// OP_ALLRESET												// 2004.11.05
	// OP_RUN

	// OP_OPENSHUTTER											// 2004.12.22
	// OP_CLOSESHUTTER											// 2004.12.22

	// OP_CHANGEASPIN||...
	// OP_VDM
	// OP_WORKPARAM||...
	// OP_DBINITIAL												// 2004.12.28

	// OP_CHANGEOPPIN||...
	// OP_ADDCASH||...
	// OP_SUBCASH||...
	// OP_CLEARCASH||...
	// OP_PRINTCASH
	// OP_CASHVALUE||...										// 2005.02.24
	// OP_DOWNFIT
	// OP_SETPARAM||...											// 2005.10.25
	// OP_SETMASTERKEY||...										// 2005.10.25
	// OP_RQK													// 2004.12.22
	// OP_TOTAL
	// OP_TOTALCB
	// OP_TOTALIB
	// OP_SUBTOTAL

	// OP_SPRPRINT||...
	// OP_JPRPRINT||...
	// OP_JPRDIRECTPRINT||...
	// OP_SPRJPRPRINT||...
	// OP_PINMODE||...

	// OP_INITEPP
	// OP_INITJOURNALPTR
	// OP_INITRECEIPTPTR
	// OP_INITDISPENCE
	// OP_INITCARDREADER
	// OP_CLEARCARD
	// OP_UPLOADJOUR
	// OP_REQUESTMAC
	// OP_REQUESTPIN
	// OP_DOWNLOADCONTROLPARA
	// OP_DOWNLOADCARDTABEL
	// OP_DOWNLOADEXTPARA
		
	static	int		ClerkProcInCnt = 0;							// Clerk Procedure In Count
	static	CString	strOpResult("");							// Op Result

	CString			strOpCommand("");
	CStringArray	strOpCommandArray;
	CString			strTemp("");
	int				i = 0;
	int				nErrorFlag = FALSE;
	int				DeviceStatusSave;							// 2006.02.09
	int				SprSet, TotalAddSet;						// added by liuxl 20110407

/////////////////////////////////////////////////////////////////////////////
	if (GetShareData("OP_RESULT") != OP_DO)						// OP_DO
		return TRUE;

/////////////////////////////////////////////////////////////////////////////
	if (ClerkProcInCnt > 0)
	{
		ClerkProcInCnt--;
		if (ClerkProcInCnt == 0)
		{
			m_pDevCmn->fnAPL_ClerkInformation();
			SetShareData("OP_RESULT", strOpResult);				// OP_RESULT
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkProc:strOpResult[%s]", strOpResult);
			return TRUE;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	strOpCommand = GetShareData("OP_COMMAND");					// OP_COMMAND
	if (strOpCommand.Find("OP_SETMASTERKEY") >= 0)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkProc:strOpCommand[]");
	}
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ClerkProc:strOpCommand[%s]", strOpCommand);
	}
	SplitString(strOpCommand, "||", strOpCommandArray);
	
/////////////////////////////////////////////////////////////////////////////
	strOpResult = OP_ERROR;										// Default Setting
	if (strOpCommandArray[0] == "OP_POWEROFF")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR SHUTDOWN ATM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员关闭系统",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		if(m_pDevCmn->bAgentMode == 1)
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_ATM_POWEROFF);


		m_pDevCmn->fnAPL_SetClerkPowerOff();					// Set Clerk Power Off : 2004.06.27

		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Null Message
	}
	else
	if (strOpCommandArray[0] == "OP_REBOOT")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR Restart ATM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员重启ATM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pTranCmn->HostNms.RebootFlag = TRUE;
		m_pDevCmn->fnAPL_SetClerkPowerOff();					// Set Clerk Power Off : 2004.06.27

		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Null Message
	}
	else
	if (strOpCommandArray[0] == "OP_POWEROFFSTATUS")
	{
//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR Restart ATM",
//													GetTime().GetBuffer(0),
//													&(GetTime().GetBuffer(0)[2]),
//													&(GetTime().GetBuffer(0)[4])));

		m_pDevCmn->fnAPL_ClerkInformation();
		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Null Message
		SetShareData("OP_RESULT", strOpResult);
		return FALSE;											// Terminate
	}
	else
	if (strOpCommandArray[0] == "OP_UPLOADJOUR")				// 上传明细
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR UPLOAD DETAILSEND",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 交易周期明细上送",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_SENDDETAIL);	// SENDDETAIL Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if(m_pDevCmn->bAgentMode == 1)
			m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_READY")
	{ 
		m_pTranCmn->ErrorSendFlag = FALSE;					// 2009.04.08
		m_pTranCmn->nAgentOperatorMode = TRUE;                  
		m_pTranCmn->nAgentAtmAfterOpenFistFailFlag = FALSE; 

		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ENTER OPERATOR ADMIN",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 进入后台管理模式",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
				
        if(m_pDevCmn->bAgentMode == 1)
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_ENTER_ADMIN);	// 2009.04.08
/////////////////////////////////////////////////////////////////////////////
		switch (m_pDevCmn->AtmDefine.OpDevice)					// 2005.11.11
		{
			case NOT_SET:										// 2006.06.02
				if (m_pDevCmn->AtmDefine.SplDevice == SPL_TOGGLESW)
					m_pDevCmn->fnSNS_ClearOperatorSwitch();
				break;
			case OP_TOUCH:
				m_pDevCmn->fnSNS_ClearOperatorSwitch();
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->HostNms.OexOpenBySpvSendFlag = TRUE;// 2005.11.11
/////////////////////////////////////////////////////////////////////////////
				break;
			case OP_TSPL:
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->HostNms.OexOpenBySpvSendFlag = TRUE;// 2005.11.11
/////////////////////////////////////////////////////////////////////////////
				break;
			case OP_TSPLCHN:
				/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->HostNms.OexOpenBySpvSendFlag = TRUE;// 2007.06.08
				/////////////////////////////////////////////////////////////////////////////
				break;	
			case OP_HSPL:
				m_pDevCmn->fnSNS_ClearOperatorSwitch();
/////////////////////////////////////////////////////////////////////////////
				m_pTranCmn->HostNms.OexOpenBySpvSendFlag = TRUE;// 2005.11.11
/////////////////////////////////////////////////////////////////////////////
				break;
		}
		SetShareData("OP_OPSTATUS", "OP");						// 2006.01.10

		m_pDevCmn->OpenKey = FALSE;
		m_pTranCmn->InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
		
		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Null Message
	}
	else
	if (strOpCommandArray[0] == "OP_RESET")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR RESET ERROR DEVICE",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 故障设备重置",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
        if(m_pDevCmn->bAgentMode == 1)
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_DEVICE_INIT);

		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;						// 2004.11.05
		if (m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus) | m_pDevCmn->DeviceDownDevice))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Null Message
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Null Message
		}
	}
	else
	if (strOpCommandArray[0] == "OP_ALLRESET")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT ALL DEVICE",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 设备重置",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		
		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
        if(m_pDevCmn->bAgentMode == 1)        
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_DEVICE_INIT);
		
		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_MAIN))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Null Message
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Null Message
		}
	}
	else
	if (strOpCommandArray[0] == "OP_INITJOURNALPTR")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT JOURNAL PRINTER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 初始化流水打印机",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		
		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_JPR))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_INITRECEIPTPTR")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT RECEIPT PRINTER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 初始化凭条打印机",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_SPR))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_UNLOCKCWD")					// added by liuxl 20110609
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR UNLOCKCWD",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 解除取款锁定",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, 0);
		
		strOpResult = OP_NORMAL;
		strOpResult += "||";
		strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
	}
	else
	if (strOpCommandArray[0] == "OP_RECEIPTPTRSET")					// added by liuxl 20110407
	{
		SprSet = Asc2Int(IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"SPRSET","0"));
		if (SprSet == 1)
			IniSetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"SPRSET","0");
		else
			IniSetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"SPRSET","1");

// 		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT RECEIPT PRINTER",
// 			GetTime().GetBuffer(0),
// 			&(GetTime().GetBuffer(0)[2]),
// 			&(GetTime().GetBuffer(0)[4])));
// 		
// 		m_pTranCmn->HostNms.SaveErrCnt = 0;
// 		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
// 		
// 		m_pDevCmn->fnAPL_ClearError();
// 		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
// 		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
// 		m_pDevCmn->DeviceRetract = DEV_NONE;
// 		m_pDevCmn->DeviceLost = DEV_NONE;
// 		if (m_pDevCmn->fnAPL_ResetDevice(DEV_SPR))
// 		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
// 		}
// 		else
// 		{
// 			strOpResult = OP_ERROR;
// 			strOpResult += "||";
// 			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
// 		}
	}
	else
	if (strOpCommandArray[0] == "OP_TOTALADDRULE")
	{
		TotalAddSet = Asc2Int(IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"TOTALADDRULESET","1"));
		if (TotalAddSet == 1)
			IniSetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"TOTALADDRULESET","0");
		else
			IniSetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"TOTALADDRULESET","1");

		strOpResult = OP_NORMAL;
		strOpResult += "||";
		strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
	}
	else
	if (strOpCommandArray[0] == "OP_INITDISPENCE")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT DISPENCER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 初始化出钞模块",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_CSH))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_INITCARDREADER")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT CARD READER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 初始化读卡器",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;
		if (m_pDevCmn->fnAPL_ResetDevice(DEV_MCU))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_INITEPP")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT EPP",  // 密码键盘  by zjd
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 初始化密码键盘",  // 密码键盘  by zjd
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		
		m_pTranCmn->HostNms.SaveErrCnt = 0;
		memset(&m_pTranCmn->HostNms.SaveErrBuff, 0, sizeof(m_pTranCmn->HostNms.SaveErrBuff));
		
		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;
// 		if (m_pDevCmn->fnAPL_ResetDevice(DEV_PIN))
		if(m_pDevCmn->fnPIN_InitializeEPP() == 0)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_CLEARCARD")					// 2004.11.05
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR CLEAR RETAINED CARD",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 清卡",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pDevCmn->fnAPL_ClearError();
		m_pDevCmn->RecoveryType = 1;							// Manual Recovery
		m_pDevCmn->DeviceAutoOff = DEV_NONE;					// 2004.06.30
		m_pDevCmn->DeviceRetract = DEV_NONE;
		m_pDevCmn->DeviceLost = DEV_NONE;

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
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("清卡张数: %2.2d", iRetractCnt));
			
			IniSetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
		}

		if (m_pDevCmn->fnAPL_ClearRetractCnt(DEV_MCU))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_RUN")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s QUIT OPERATOR ADMIN",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 退出后台管理模式",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
  
        if(m_pDevCmn->bAgentMode == 1)
		{
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_ENTER_TRAN);
		}

		ULONG	WaitTime;
		switch (m_pDevCmn->AtmDefine.OpDevice)					// 2005.11.11
		{
			case NOT_SET:
				SetShareData("OP_MODE", "AP");

				WaitTime = TimerSet(K_10_WAIT);
				while (TRUE)
				{
					Delay_Msg(100);								// Msg & Task
					if (GetShareData("OP_MODE").CompareNoCase("APOK") == 0)
						break;
					if (CheckTimer(WaitTime))
						break;
				}
				
				m_pDevCmn->AtmStatusReEntryFlag= TRUE;
				break;
			case OP_TOUCH:
				break;
			case OP_TSPL:
				break;
			case OP_TSPLCHN:
				break;
			case OP_HSPL:
				break;
		}
		SetShareData("OP_OPSTATUS", "AP");						// 2006.01.10

/////////////////////////////////////////////////////////////////////////////
		switch (m_pDevCmn->AtmDefine.OpDevice)					// 2005.11.11
		{
			case NOT_SET:										// 2006.06.02
				if (m_pDevCmn->AtmDefine.SplDevice == SPL_TOGGLESW)
					m_pDevCmn->fnSNS_ClearOperatorSwitch();
				break;
			case OP_TOUCH:
				m_pDevCmn->fnSNS_ClearOperatorSwitch();
				break;
			case OP_TSPL:
				break;
			case OP_TSPLCHN:
				break;	
			case OP_HSPL:
				m_pDevCmn->fnSNS_ClearOperatorSwitch();
				break;
		}

/////////////////////////////////////////////////////////////////////////////
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			// 2006.06.02
		DeviceStatusSave = m_pDevCmn->DeviceStatus;				// 2006.02.09
		if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)
			m_pDevCmn->fnCSH_CloseShutter();					// 2005.03.30
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);
		m_pDevCmn->DeviceStatus = DeviceStatusSave;				// 2006.02.09

/////////////////////////////////////////////////////////////////////////////
		m_pTranCmn->ErrorSendFlag = FALSE;						// 2008.11.14
		nOperatorSwitch = FALSE;								// Clear
		nOperatorMode = FALSE;
		m_pDevCmn->fnAPL_CloseHost();							// 2005.09.11 : DO NOT MODIFY

		if (m_pDevCmn->bFirstStartRQK)							// added by liuxl 20120209
			m_pDevCmn->bFirstStartRQK = FALSE;

		m_pDevCmn->OpenKey = TRUE;
		m_pTranCmn->InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
/////////////////////////////////////////////////////////////////////////////

		m_pTranCmn->nAgentOperatorMode = FALSE;                             // 2008.04.14 by hyl 

		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Null Message
	}
	else
	if (strOpCommandArray[0] == "OP_OPENSHUTTER")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR OPENSHUTTER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 打开出钞门",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
	
		if(m_pDevCmn->bAgentMode == 1)
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_SHUTTER_OPEN);

		if (m_pDevCmn->DoorStatus == DOOR_OPENED)
		{
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
			DeviceStatusSave = m_pDevCmn->DeviceStatus;			// 2006.02.09
			if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)
				m_pDevCmn->fnCSH_OpenShutter();					// Open Shutter
			m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
			m_pDevCmn->DeviceStatus = DeviceStatusSave;			// 2006.02.09
			
			if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Null Message
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Null Message
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Open Safe Door Message
			strOpResult += GuideMsgTbl[31][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_CLOSESHUTTER")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR CLOSESHUTTER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 关闭出钞门",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
	
		if(m_pDevCmn->bAgentMode == 1)
			m_pTranCmn->fnAPP_AgentIdxGetData(CLERK_SHUTTER_CLOSE);
		
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		DeviceStatusSave = m_pDevCmn->DeviceStatus;				// 2006.02.09
		if (m_pDevCmn->fnCSH_GetDeviceStatus() != NODEVICE)		// 2005.03.30
			m_pDevCmn->fnCSH_CloseShutter();					// Close Shutter
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
		m_pDevCmn->DeviceStatus = DeviceStatusSave;				// 2006.02.09
		
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Null Message
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Null Message
		}
	}
	else
	if (strOpCommandArray[0] == "OP_CHANGEASPIN")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR change PIN",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员改密",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			m_pProfile->GetTransProfile();

			strTemp = strOpCommandArray[1] + SPACE8;
			strTemp = strTemp.Left(8);
			memcpy(m_pProfile->TRANS.AsPassWord, strTemp.GetBuffer(0), 8);
			m_pProfile->TRANS.AsPassWord[8] = NULL;
			m_pProfile->PutTransProfile();

			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_VDM")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR ENTER VDM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员进入VDM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
	
/////////////////////////////////////////////////////////////////////////////
		m_pDevCmn->fnAPL_ClearDeviceExecOffInfo(TRUE);			// Clear Device Exec Off Infomation

/////////////////////////////////////////////////////////////////////////////
		m_pDevCmn->fnAPL_ClerkInformation();
		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Null Message
		SetShareData("OP_RESULT", strOpResult);

/////////////////////////////////////////////////////////////////////////////
		m_pDevCmn->fnAPL_ClerkInitModeSet();					// Clerk Initialize Mode Set
		KillTimer(TIMER_THREAD);								// Timer Stop
		KillTimer(TIMER_VFD_THREAD);							// Vfd Stop
		m_pDevCmn->fnNET_Deinitialize3(LINE_OPCL_TIME);
		m_pDevCmn->fnNET_Deinitialize(LINE_OPCL_TIME);
		m_pDevCmn->fnAPL_UnLoadDevice();
		m_pDevCmn->AtmDoing = FALSE;							// Atm Stop
	
/////////////////////////////////////////////////////////////////////////////
		WinExecWaitEnd("C:\\T1ATM\\APP\\EXERELE\\GOMA.EXE       Rear.exe ",
																SW_HIDE);
																// 2004.07.08
		WinExecWaitEnd(_AP_VDM, SW_SHOW);						// Run Vdm
		return FALSE;											// Terminate
	}
	else
	if (strOpCommandArray[0] == "OP_WORKPARAM")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR CHANGE WORKPARAM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 参数设置",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		if (strOpCommandArray.GetSize() < 16)                  //加上银行柜员号则改为17
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			m_pProfile->GetNetworkProfile();					// Get Network Profile

			strTemp = ZERO8 + strOpCommandArray[1];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.AtmSerialNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.AtmSerialNum[8] = NULL;

			strTemp.Empty();
			strTemp = ZERO9 + strOpCommandArray[2];
			strTemp = strTemp.Right(9);
			memcpy(m_pProfile->NETWORK.BranchNum, strTemp.GetBuffer(0), 9);
			m_pProfile->NETWORK.BranchNum[9] = NULL;

			strTemp.Empty();
			strTemp = ZERO8 + strOpCommandArray[3];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.SubBranchNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.SubBranchNum[8] = NULL;

			strTemp.Empty();
			strTemp = ZERO8 + strOpCommandArray[4];
			strTemp = strTemp.Right(8);
			memcpy(m_pProfile->NETWORK.RoomNum, strTemp.GetBuffer(0), 8);
			m_pProfile->NETWORK.RoomNum[8] = NULL;

			strTemp.Empty();
			strTemp = ZERO12 + strOpCommandArray[5];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.BpIpAddress, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.BpIpAddress[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO5 + strOpCommandArray[6];
			strTemp = strTemp.Right(5);
			memcpy(m_pProfile->NETWORK.BpPortNo, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo[5] = NULL;

			strTemp.Empty();
			strTemp = ZERO12 + strOpCommandArray[7];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.BpIpAddress2, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.BpIpAddress2[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO5 + strOpCommandArray[8];
			strTemp = strTemp.Right(5);
			memcpy(m_pProfile->NETWORK.BpPortNo2, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo2[5] = NULL;

			strTemp.Empty();
			strTemp = ZERO12 + strOpCommandArray[9];
			strTemp = strTemp.Right(12);

			CString strComputerName("");
			strComputerName = strTemp;							// IP Save For Set Computer Name

			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.AtmIPAddress, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.AtmIPAddress[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO12 + strOpCommandArray[10];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.AtmSubnetMask, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.AtmSubnetMask[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO12 + strOpCommandArray[11];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.AtmGateWay, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.AtmGateWay[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO5 + strOpCommandArray[12];
			strTemp = strTemp.Right(5);
			memcpy(m_pProfile->NETWORK.AtmPortNo, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.AtmPortNo[5] = NULL;

			strTemp.Empty();									// 2005.04.19
			strTemp = ZERO12 + strOpCommandArray[13];
			strTemp = strTemp.Right(12);
			strTemp.Format("%3.3s.%3.3s.%3.3s.%3.3s", 
							strTemp.Mid(0,3), 
							strTemp.Mid(3,3), 
							strTemp.Mid(6,3),
							strTemp.Mid(9,3));
			memcpy(m_pProfile->NETWORK.BpIpAddress3, strTemp.GetBuffer(0), 15);
			m_pProfile->NETWORK.BpIpAddress3[15] = NULL;

			strTemp.Empty();
			strTemp = ZERO5 + strOpCommandArray[14];
			strTemp = strTemp.Right(5);
			memcpy(m_pProfile->NETWORK.BpPortNo3, strTemp.GetBuffer(0), 5);
			m_pProfile->NETWORK.BpPortNo3[5] = NULL;

//			strTemp.Empty();
//			strTemp = ZERO + strOpCommandArray[15];
//			strTemp = strTemp.Right(1);
//            IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_TRANMIADDCASHMODE,strTemp);//加钞方式 by hyl
		
//			strTemp = ZERO8 + strOpCommandArray[16];
//			strTemp = strTemp.Right(8);
//			memcpy(m_pProfile->NETWORK.ACCounterNumber, strTemp.GetBuffer(0), 8);
// 			m_pProfile->NETWORK.ACCounterNumber[8] = NULL;
			
			m_pProfile->PutNetworkProfile();					// Put Network Profile

			char	IpAddress[256];
			memset(IpAddress, 0, sizeof(IpAddress));

			sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmIPAddress, 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[4], 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[8], 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[12], 3));

			AtmSetIpAddress(IpAddress);

			memset(IpAddress, 0, sizeof(IpAddress));
			sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmSubnetMask, 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[4], 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[8], 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmSubnetMask[12], 3));

			AtmSetSubnetMask(IpAddress);

			memset(IpAddress, 0, sizeof(IpAddress));
			sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmGateWay, 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[4], 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[8], 3),
											  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[12], 3));

			AtmSetGateWay(IpAddress);

			strComputerName = "ATM" + strComputerName;			// 2006.06.02
			if ((m_pProfile->NETWORK.InterfaceKind == 10)	||	// Cup(TJ)
				(m_pProfile->NETWORK.InterfaceKind == 15)	||	// Cup(SH) : 2006.04.19
				(m_pProfile->NETWORK.InterfaceKind == 20))		// Cup(SHGD) : 2006.09.22
			{
				strComputerName.Format("ATM%8.8s", m_pProfile->NETWORK.AtmSerialNum);
			}

			RegSetStr(_REGKEY_WIN2000_COMPUTERNAME, "NV Hostname" , strComputerName);
			SetComputerName(strComputerName);					// Set Computer Name With IP Address

			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_DBINITIAL")					// 2005.05.18
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DBINITIAL",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s DB初始化",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);				// Set Print Image Mode
		m_pDevCmn->fnAPL_ClearStatistics();						// Clear Statistics Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);				// Set Print Image Mode

		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Success Message
		strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
	}
	else
	if (strOpCommandArray[0] == "OP_CHANGEOPPIN")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR change PIN",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员改密",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			m_pProfile->GetTransProfile();

			strTemp = strOpCommandArray[1] + SPACE8;
			strTemp = strTemp.Left(8);
			memcpy(m_pProfile->TRANS.OpPassWord, strTemp.GetBuffer(0), 8);
			m_pProfile->TRANS.OpPassWord[8] = NULL;
			m_pProfile->PutTransProfile();

			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_ADDCASH")
	{
/////////////////////////开始添钞的拍摄   by zjd
//		if (m_pDevCmn->iMonitorCS_AddCash != -1)
//		{
//			hy_HCMonitor2_CustomerServiceEnd(m_pDevCmn->iMonitorCS_AddCash);
//			m_pDevCmn->iMonitorCS_AddCash = -1;
//		}
//		m_pDevCmn->iMonitorCS_AddCash = hy_HCMonitor2_CustomerServiceBegin();
//		hy_HCMonitor2_StartCap(m_pDevCmn->iAddCashCamNo, m_pDevCmn->iMonitorCS_AddCash,
//			"9999999999", "ADD CASH", "NO");
////////////////////////////////////////////////////////
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR ADDCASH",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员加钞",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		if (m_pDevCmn->AtmDefine.CashHandler == BRM_SET)		// Cst Value Check : 2008.04.10
		{
			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))	// 2009.12.21
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [AUTORESET]",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
				m_pDevCmn->fnAPL_ClearError();
				m_pDevCmn->RecoveryType = 1;					// Manual Recovery
				m_pDevCmn->DeviceAutoOff = DEV_NONE;			// 2004.06.30
				m_pDevCmn->DeviceRetract = DEV_NONE;
				m_pDevCmn->DeviceLost = DEV_NONE;				// 2004.11.05
				m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus) | m_pDevCmn->DeviceDownDevice);
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);
				CString strErrorCode = m_pDevCmn->fstrCSH_GetErrorCode();
				if (strErrorCode.Left(7) == "5190001")			// CST Changed
				{
					m_pDevCmn->fnCSH_SetValueOfCash("");
					m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);

					m_pDevCmn->fnAPL_ClearError();
					m_pDevCmn->RecoveryType = 1;				// Manual Recovery
					m_pDevCmn->DeviceAutoOff = DEV_NONE;		// 2004.06.30
					m_pDevCmn->DeviceRetract = DEV_NONE;
					m_pDevCmn->DeviceLost = DEV_NONE;			// 2004.11.05
					m_pDevCmn->CSTCnt = 0;
					m_pDevCmn->fnAPL_ResetDevice(m_pDevCmn->fnAPL_GetErrorDevice(m_pDevCmn->DeviceStatus) | m_pDevCmn->DeviceDownDevice);
					m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);
				}
			}
		}

//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR | DEV_CSH))
		{
			if (strOpCommandArray.GetSize() >= 6)
			{
				memcpy(m_pTranCmn->szADDCashCounterNumber, GetSprintf("%07.7s",strOpCommandArray[1]).GetBuffer(0), 7);
				switch (m_pDevCmn->AtmDefine.CashHandler)		// Cst Limit Check : 2006.06.02
				{
					case BRM_SET:
						break;
					case CDU_SHUTTER1510_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if (Asc2Int(strOpCommandArray[2]) > 1500)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if (Asc2Int(strOpCommandArray[3]) > 1000)
								nErrorFlag = TRUE;
						}
						break;
					case CDU_SHUTTER20_SET:
					case CDU_RETRACT20_SET:
					case CDU_PRESENT20_SET:
					case CDU_TRAY20_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if (Asc2Int(strOpCommandArray[2]) > 2000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if (Asc2Int(strOpCommandArray[3]) > 2000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 3)
						{
							if (Asc2Int(strOpCommandArray[4]) > 2000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 4)
						{
							if (Asc2Int(strOpCommandArray[5]) > 2000)
								nErrorFlag = TRUE;
						}
						break;
					case CDU_SHUTTER30_SET:
					case CDU_RETRACT30_SET:
					case CDU_PRESENT30_SET:
					case CDU_TRAY30_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if (Asc2Int(strOpCommandArray[2]) > 3000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if (Asc2Int(strOpCommandArray[3]) > 3000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 3)
						{
							if (Asc2Int(strOpCommandArray[4]) > 3000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 4)
						{
							if (Asc2Int(strOpCommandArray[5]) > 3000)
								nErrorFlag = TRUE;
						}
						break;

					case CDU_GPRESENT20_SET:     // 2006.10.30
					case CDU_GPRESENT30_SET:
						break;

					default:
						nErrorFlag = TRUE;
						break;
				}

				if (nErrorFlag)
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";						// Error Message(Max Over)
					strOpResult += GuideMsgTbl[26][m_pDevCmn->OpDeviceLangMode];
				}
				else
				{
					m_pDevCmn->FstCSTSetCash = 0;
					m_pDevCmn->SndCSTSetCash = 0;
					m_pDevCmn->TrdCSTSetCash = 0;
					m_pDevCmn->FthCSTSetCash = 0;
					if (m_pDevCmn->CSTCnt >= 1)
						m_pDevCmn->FstCSTSetCash = Asc2Int(strOpCommandArray[2]);
					if (m_pDevCmn->CSTCnt >= 2)
						m_pDevCmn->SndCSTSetCash = Asc2Int(strOpCommandArray[3]);
					if (m_pDevCmn->CSTCnt >= 3)
						m_pDevCmn->TrdCSTSetCash = Asc2Int(strOpCommandArray[4]);
					if (m_pDevCmn->CSTCnt >= 4)
						m_pDevCmn->FthCSTSetCash = Asc2Int(strOpCommandArray[5]);
					m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);	// Set Print Image Mode
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALBATCHNO, 
								GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001")));
                    
					//增加可疑交易处理 清除列表  BY HYL  
//                  IniSetInt(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, "SUSPICIOUSNUM", 0);

					// 添钞交易先清除吞卡张数，本机统计，对帐和上送明细   by zjd
//					int iRetractCnt = 0;
//					iRetractCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
//					if (iRetractCnt > 0)
//					{
//						for(int jj = 0; jj < iRetractCnt; jj++)
//						{
//							m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST1%02.2d",jj), 0));
//							m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST2%02.2d",jj), 0));
//							m_pDevCmn->fnAPL_PrintJnlInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_RETRACTCARD, GetSprintf("RETRACTCARDLIST3%02.2d",jj), 0));
//						}
//						if (m_pTranCmn->LangMode)
//							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("Retained Cards : %2.2d", iRetractCnt));
//						else
//							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("吞卡张数: %2.2d", iRetractCnt));
//						IniSetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
//					}
					
					// 先打印本机汇总，再清除吞卡
//					m_pTranCmn->fnAPP_MainTranProc(TRAN_SUBTOTAL);
					
//					m_pDevCmn->fnAPL_ClearRetractCnt(DEV_MCU);

					
					
 		            if(m_pDevCmn->bAgentMode == 1)
			             m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_ADDCASHTOTAL);

					
//					m_pTranCmn->fnAPP_MainTranProc(TRAN_TOTAL);	// Total Procedure : 2005.09.01
//
//					if (m_pDevCmn->TranResult)					// 2005.08.31
//						;
//					else
//						m_pTranCmn->fnAPP_MainTranProc(TRAN_SENDDETAIL);

					m_pTranCmn->fnAPP_MainTranProc(TRAN_ADDCASH);
																// Add Cash Procedure

					CString strTempResult;
					if (m_pTranCmn->TotalAddFlag)
						strTempResult = GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
					else
						strTempResult = "超时重试且金额需一致";
					//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pTranCmn->HostResp.RejectCode[%s]", m_pTranCmn->HostResp.RejectCode);

					if (memcmp(m_pTranCmn->HostResp.RejectCode, "3M", 2) == 0) // LIYI 0816
					{
						strTempResult = "先进行扎帐再加钞";
					}

					// 20080407 by hyl 
 					if(m_pDevCmn->bAgentMode == 1)
			            m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_ADDCASH);

					if(m_pDevCmn->bAgentMode == 1)
						m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);

					m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);	// Set Print Image Mode
					if (m_pDevCmn->TranResult)
					{
						strOpResult = OP_NORMAL;
						strOpResult += "||";					// Success Message
						strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
						RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "0");	// added by yusy 20100322
					}
					else
					{
						strOpResult = OP_ERROR;
						strOpResult += "||";					// Error Message
//						strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
						strOpResult += strTempResult;
					}
				}
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
////////////////////////////////添钞结束摄像头拍摄   by zjd
//		hy_HCMonitor2_StopCap(m_pDevCmn->iAddCashCamNo);
	}
	else
	if (strOpCommandArray[0] == "OP_SUBCASH")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR SUBCASH",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		{
			if (strOpCommandArray.GetSize() >= 6)
			{
				memcpy(m_pTranCmn->szADDCashCounterNumber, GetSprintf("%07.7s",strOpCommandArray[1]).GetBuffer(0), 7);
				switch (m_pDevCmn->AtmDefine.CashHandler)		// Cst Limit Check : 2006.06.02
				{
					case BRM_SET:
						break;
					case CDU_SHUTTER1510_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if (Asc2Int(strOpCommandArray[2]) > 1500)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if (Asc2Int(strOpCommandArray[3]) > 1000)
								nErrorFlag = TRUE;
						}
						break;
					case CDU_SHUTTER20_SET:
					case CDU_RETRACT20_SET:
					case CDU_PRESENT20_SET:
					case CDU_TRAY20_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if (Asc2Int(strOpCommandArray[2]) > 2000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if (Asc2Int(strOpCommandArray[3]) > 2000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 3)
						{
							if (Asc2Int(strOpCommandArray[4]) > 2000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 4)
						{
							if (Asc2Int(strOpCommandArray[5]) > 2000)
								nErrorFlag = TRUE;
						}
						break;
					case CDU_SHUTTER30_SET:
					case CDU_RETRACT30_SET:
					case CDU_PRESENT30_SET:
					case CDU_TRAY30_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if (Asc2Int(strOpCommandArray[2]) > 3000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if (Asc2Int(strOpCommandArray[3]) > 3000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 3)
						{
							if (Asc2Int(strOpCommandArray[4]) > 3000)
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 4)
						{
							if (Asc2Int(strOpCommandArray[5]) > 3000)
								nErrorFlag = TRUE;
						}
						break;

					case CDU_GPRESENT20_SET:     // 2006.10.30
					case CDU_GPRESENT30_SET:
						break;
					default:
						nErrorFlag = TRUE;
						break;
				}

				if (nErrorFlag)
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";						// Error Message(Max Over)
					strOpResult += GuideMsgTbl[26][m_pDevCmn->OpDeviceLangMode];
				}
				else
				{
					m_pDevCmn->FstCSTSetCash = 0;
					m_pDevCmn->SndCSTSetCash = 0;
					m_pDevCmn->TrdCSTSetCash = 0;
					m_pDevCmn->FthCSTSetCash = 0;
					if (m_pDevCmn->CSTCnt >= 1)
						m_pDevCmn->FstCSTSetCash = Asc2Int(strOpCommandArray[2]);
					if (m_pDevCmn->CSTCnt >= 2)
						m_pDevCmn->SndCSTSetCash = Asc2Int(strOpCommandArray[3]);
					if (m_pDevCmn->CSTCnt >= 3)
						m_pDevCmn->TrdCSTSetCash = Asc2Int(strOpCommandArray[4]);
					if (m_pDevCmn->CSTCnt >= 4)
						m_pDevCmn->FthCSTSetCash = Asc2Int(strOpCommandArray[5]);
					m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);	// Set Print Image Mode
					m_pTranCmn->fnAPP_MainTranProc(TRAN_SUBCASH);
																// Sub Cash Procedure
					m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);	// Set Print Image Mode
					if(m_pDevCmn->bAgentMode == 1)
						m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
					if (m_pDevCmn->TranResult)
					{
						strOpResult = OP_NORMAL;
						strOpResult += "||";					// Success Message
						strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
					}
					else
					{
						strOpResult = OP_ERROR;
						strOpResult += "||";					// Error Message
						strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
					}
				}
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_CLEARCASH")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR CLEARCASH",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		{
			memcpy(m_pTranCmn->szADDCashCounterNumber, GetSprintf("%07.7s",strOpCommandArray[1]).GetBuffer(0), 7);
			if (strOpCommandArray.GetSize() >= 6)
			{
				m_pDevCmn->FstCSTSetCash = 0;
				m_pDevCmn->SndCSTSetCash = 0;
				m_pDevCmn->TrdCSTSetCash = 0;
				m_pDevCmn->FthCSTSetCash = 0;
				m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);		// Set Print Image Mode
				m_pTranCmn->fnAPP_MainTranProc(TRAN_CLEARCASH);	// Clear Cash Procedure
				m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);		// Set Print Image Mode
				if(m_pDevCmn->bAgentMode == 1)
					m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
				if (m_pDevCmn->TranResult)
				{
					strOpResult = OP_NORMAL;
					strOpResult += "||";						// Success Message
					strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
				}
				else
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";						// Error Message
					strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
				}
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_PRINTCASH")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR PRINTCASH",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 打印钞箱信息",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_PRINTCASH);		// Print Cash Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if(m_pDevCmn->bAgentMode == 1)
				m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_CASHVALUE")					// 2005.02.24
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR CHANGE",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("       CASHVALUE:%3.3s,%3.3s,%3.3s,%3.3s",
													strOpCommandArray[1],
													strOpCommandArray[2],
													strOpCommandArray[3],
													strOpCommandArray[4]));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 设置钞箱面额",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("各钞箱面额:%3.3s,%3.3s,%3.3s,%3.3s",
													strOpCommandArray[1],
													strOpCommandArray[2],
													strOpCommandArray[3],
													strOpCommandArray[4]));
		}
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		{
			if (strOpCommandArray.GetSize() >= 5)
			{
				switch (m_pDevCmn->AtmDefine.CashHandler)		// Cst Value Check : 2006.06.02
				{
					case BRM_SET:
					case CDU_SHUTTER1510_SET:
					case CDU_SHUTTER20_SET:
					case CDU_SHUTTER30_SET:
					case CDU_RETRACT20_SET:
					case CDU_RETRACT30_SET:
					case CDU_PRESENT20_SET:
					case CDU_PRESENT30_SET:
					case CDU_GPRESENT20_SET:
					case CDU_GPRESENT30_SET:
					case CDU_TRAY20_SET:
					case CDU_TRAY30_SET:
						if (m_pDevCmn->CSTCnt >= 1)
						{
							if ((Asc2Int(strOpCommandArray[1]) == 100)	||
								(Asc2Int(strOpCommandArray[1]) == 50)	||
								(Asc2Int(strOpCommandArray[1]) == 20)	||
								(Asc2Int(strOpCommandArray[1]) == 10))
								;
							else
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 2)
						{
							if ((Asc2Int(strOpCommandArray[2]) == 100)	||
								(Asc2Int(strOpCommandArray[2]) == 50)	||
								(Asc2Int(strOpCommandArray[2]) == 20)	||
								(Asc2Int(strOpCommandArray[2]) == 10))
								;
							else
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 3)
						{
							if ((Asc2Int(strOpCommandArray[3]) == 100)	||
								(Asc2Int(strOpCommandArray[3]) == 50)	||
								(Asc2Int(strOpCommandArray[3]) == 20)	||
								(Asc2Int(strOpCommandArray[3]) == 10))
								;
							else
								nErrorFlag = TRUE;
						}
						if (m_pDevCmn->CSTCnt >= 4)
						{
							if ((Asc2Int(strOpCommandArray[4]) == 100)	||
								(Asc2Int(strOpCommandArray[4]) == 50)	||
								(Asc2Int(strOpCommandArray[4]) == 20)	||
								(Asc2Int(strOpCommandArray[4]) == 10))
								;
							else
								nErrorFlag = TRUE;
						}
						break;

					default:
						nErrorFlag = TRUE;
						break;
				}

				if (nErrorFlag)
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";						// Error Message(Invalid Cash Value)
					strOpResult += GuideMsgTbl[27][m_pDevCmn->OpDeviceLangMode];
				}
				else
				{
					CString strCSTVaules("");
					CString strCSTVaulesPrint("");
					strCSTVaules += "0";
					if (m_pDevCmn->CSTCnt >= 1)
					{
						strCSTVaules += ",";
						strCSTVaules += strOpCommandArray[1];
						
						strCSTVaulesPrint += strOpCommandArray[1];
					}
					if (m_pDevCmn->CSTCnt >= 2)
					{
						strCSTVaules += ",";
						strCSTVaules += strOpCommandArray[2];
						strCSTVaulesPrint += ",";
						strCSTVaulesPrint += strOpCommandArray[2];
					}
					if (m_pDevCmn->CSTCnt >= 3)
					{
						strCSTVaules += ",";
						strCSTVaules += strOpCommandArray[3];
						strCSTVaulesPrint += ",";
						strCSTVaulesPrint += strOpCommandArray[3];
					}
					if (m_pDevCmn->CSTCnt >= 4)
					{
						strCSTVaules += ",";
						strCSTVaules += strOpCommandArray[4];
						strCSTVaulesPrint += ",";
						strCSTVaulesPrint += strOpCommandArray[4];
					}

					m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);			
					m_pDevCmn->fnCSH_SetValueOfCash(strCSTVaules);			
																// Set Value Of Cash
					if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH))
					{
 						m_pDevCmn->FstCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_1);
 						m_pDevCmn->SndCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_2);
 						m_pDevCmn->TrdCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_3);
 						m_pDevCmn->FthCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_4);
						if (m_pTranCmn->LangMode)
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s  VALUERESULT:%3.3d,%3.3d,%3.3d,%3.3d",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4]),
																m_pDevCmn->FstCSTValue,
																m_pDevCmn->SndCSTValue,
																m_pDevCmn->TrdCSTValue,
																m_pDevCmn->FthCSTValue));
						}
						else
						{
							m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s  面额设置结果:%3.3d,%3.3d,%3.3d,%3.3d",
																GetTime().GetBuffer(0),
																&(GetTime().GetBuffer(0)[2]),
																&(GetTime().GetBuffer(0)[4]),
																m_pDevCmn->FstCSTValue,
																m_pDevCmn->SndCSTValue,
																m_pDevCmn->TrdCSTValue,
																m_pDevCmn->FthCSTValue));
						}
				
						m_pDevCmn->FstCSTSetCash = 0;
						m_pDevCmn->SndCSTSetCash = 0;
						m_pDevCmn->TrdCSTSetCash = 0;
						m_pDevCmn->FthCSTSetCash = 0;

//						m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);	
//																// Set Print Image Mode
//						m_pTranCmn->fnAPP_MainTranProc(TRAN_CLEARCASH);					
//																// Clear Cash Procedure
//						m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);	
//																// Set Print Image Mode
						if (m_pDevCmn->TranResult)
						{
							strOpResult = OP_NORMAL;
							strOpResult += "||";				// Success Message
							strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
						}
						else
						{
							strOpResult = OP_ERROR;
							strOpResult += "||";				// Error Message
							strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
						}
					}
					else
					{											// 2005.02.24
 						m_pDevCmn->FstCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_1);
 						m_pDevCmn->SndCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_2);
 						m_pDevCmn->TrdCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_3);
 						m_pDevCmn->FthCSTValue = m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_4);

						strOpResult = OP_ERROR;
						strOpResult += "||";					// Error Message
						strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
					}
				}
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNFIT") || (strOpCommandArray[0] == "OP_DOWNLOADCARDTABEL"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DOWNLOAD CARD TABLE",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载卡表",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN))
//		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNFIT);		// Down Fit Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if(m_pDevCmn->bAgentMode == 1)
				m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
//		}
//		else
//		{
//			strOpResult = OP_ERROR;
//			strOpResult += "||";								// Device Error Message
//			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
//		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNLOADECHANNEL"))
	{
		if (!IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELFLAG, 1))
		{
			m_pDevCmn->TranResult = TRUE;
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}

			ClerkProcInCnt = 1;
			return TRUE;
		}

		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s DOWNLOAD ECHANNEL PARAMETER",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载电子渠道导航参数",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		
		m_pTranCmn->DownloadEchannelMtd = 0;
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 0);
		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadEchannelParaFlag[%d]", m_pTranCmn->DownloadEchannelParaFlag);
		m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNECHANNELPARA);		// Down Fit Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
// 			if (m_pTranCmn->LangMode)
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s DOWNLOAD ECHANNEL PROTOCOL",
// 					GetTime().GetBuffer(0),
// 					&(GetTime().GetBuffer(0)[2]),
// 					&(GetTime().GetBuffer(0)[4])));
// 			}
// 			else
// 			{
// 				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载电子渠道协议",
// 					GetTime().GetBuffer(0),
// 					&(GetTime().GetBuffer(0)[2]),
// 					&(GetTime().GetBuffer(0)[4])));
// 			}
// 			
// 			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
// 			m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNECHANNELPROT);		// Down Fit Procedure
// 			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
//			if (m_pDevCmn->TranResult)
//			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
//			}
//			else
//			{
//				strOpResult = OP_ERROR;
//				strOpResult += "||";							// Error Message
//				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
//			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNLOADICCARDTABLE"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DOWNLOAD ICCARD TABLE",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载IC卡卡表",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNICFIT);		// Down Fit Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNLOADPBOCPARA"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DOWNLOAD PBOC PARA",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载PBOC参数",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
			
		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNPBOCPARA);		// Down Fit Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNLOADAIDLIST"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DOWNLOAD AID LIST",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载AID应用列表",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
			
		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNAIDLIST);		// Down Fit Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNLOADCONTROLPARA"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DOWNLOAD CONTROL PARAMATER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载控制参数",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN))
//		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNPARM);		// Down PARM Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if(m_pDevCmn->bAgentMode == 1)
				m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
//		}
//		else
//		{
//			strOpResult = OP_ERROR;
//			strOpResult += "||";								// Device Error Message
//			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
//		}
	}
	else
	if ((strOpCommandArray[0] == "OP_DOWNLOADEXTPARA"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR DOWNLOAD EXTEND PARAMATER",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 下载代理业务参数",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN))
//		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_DOWNPAY);		// Down PAY Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if(m_pDevCmn->bAgentMode == 1)
				m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
//		}
//		else
//		{
//			strOpResult = OP_ERROR;
//			strOpResult += "||";								// Device Error Message
//			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
//		}
	}
	else
	if (strOpCommandArray[0] == "OP_SETPARAM")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR SET SYSPARAM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 控制参数修改",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		if (strOpCommandArray.GetSize() < 30)			// liyi add 绿卡通
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			// 需要对设置的控制参数赋值给变量,并在程序中用控制参数进行控制
			strTemp.Empty();
			strTemp = strOpCommandArray[1];
			strTemp = ZERO2 + strTemp.Right(2);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.ADSetNum = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO2 +strOpCommandArray[2];
			strTemp = strTemp.Right(2);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADTIME, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.ADTime = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[3];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_RECVTIMEOUT, Asc2Int(strTemp));
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT, Asc2Int(strTemp));
			m_pTranCmn->InitInfo.LineRecvTime = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[4];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_KEYINTIMEOUT, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.KeyInTimeOut = Asc2Int(strTemp); 

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[5];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.TakeCardTimeOut = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[6];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT, Asc2Int(strTemp));
			m_pTranCmn->InitInfo.TakeCashTimeOut = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[7];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.MsrSendTime = Asc2Int(strTemp);
			
			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[8];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DETAILNUM, Asc2Int(strTemp));
			m_pTranCmn->InitInfo.DetailNum = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[9];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ERRORNOTICETIME, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.ErrorNoticeTime = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[10];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_AUTORERETRYTIME, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.AutoRetryTime = Asc2Int(strTemp);
			
			strTemp.Empty();
			strTemp = ZERO3 + strOpCommandArray[11];
			strTemp = strTemp.Right(3);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWCRETRYTIME, Asc2Int(strTemp));
			m_pTranCmn->InitInfo.CwcRetryTime = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO4 + strOpCommandArray[12];
			strTemp = strTemp.Right(4);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.WithMaxAmt = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO4 + strOpCommandArray[13];
			strTemp = strTemp.Right(4);
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMINAMOUNT, Asc2Int(strTemp));
            m_pTranCmn->InitInfo.WithMinAmt = Asc2Int(strTemp);

			strTemp.Empty();
			strTemp = ZERO6 + strOpCommandArray[14];
			strTemp = strTemp.Right(6);
			m_pTranCmn->InitInfo.TranMaxRemittance = Asc2Int(strTemp);
            IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXREMITTANCE, GetSprintf("%.0f", m_pTranCmn->InitInfo.TranMaxRemittance));

			strTemp.Empty();
			strTemp = ZERO6 + strOpCommandArray[15];
			strTemp = strTemp.Right(6);
			m_pTranCmn->InitInfo.TranMinRemittance = Asc2Int(strTemp);
            IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINREMITTANCE, GetSprintf("%.0f", m_pTranCmn->InitInfo.TranMinRemittance));

			strTemp.Empty();
			strTemp = ZERO6 + strOpCommandArray[16];
			strTemp = strTemp.Right(6);
			m_pTranCmn->InitInfo.TransferMaxAmt = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXAMOUNT, GetSprintf("%.0f", m_pTranCmn->InitInfo.TransferMaxAmt));

			strTemp.Empty();
			strTemp = ZERO4 + strOpCommandArray[17];
			strTemp = strTemp.Right(4);
			m_pTranCmn->InitInfo.TransferMinAmt = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINAMOUNT, GetSprintf("%.0f", m_pTranCmn->InitInfo.TransferMinAmt));
            
//////////////////////////////////////////////////////////////////////////
////////////liyi add 2008.09.23
			strTemp.Empty();												//储蓄种类参数
			strTemp = ZERO8 + strOpCommandArray[18];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.SaveKindPara = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SAVEKINDPARA, GetSprintf("%.0f", m_pTranCmn->InitInfo.SaveKindPara));

			strTemp.Empty();												//整整转活期最大限额
			strTemp = ZERO8 + strOpCommandArray[19];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.ZztohMaxNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMAXNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.ZztohMaxNum));

			strTemp.Empty();												//整整转活期最小限额
			strTemp = ZERO8 + strOpCommandArray[20];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.ZztohMinNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMINNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.ZztohMinNum));

			strTemp.Empty();												//定活转活期最大限额
			strTemp = ZERO8 + strOpCommandArray[21];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.DhtohMaxNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMAXNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.DhtohMaxNum));

			strTemp.Empty();												//定活转活期最小限额
			strTemp = ZERO8 + strOpCommandArray[22];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.DhtohMinNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMINNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.DhtohMinNum));

			strTemp.Empty();												//通知存款转活期最大限额
			strTemp = ZERO8 + strOpCommandArray[23];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.TzcktohMaxNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMAXNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.TzcktohMaxNum));

			strTemp.Empty();												//通知存款转活期最小限额
			strTemp = ZERO8 + strOpCommandArray[24];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.TzcktohMinNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMINNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.TzcktohMinNum));

			strTemp.Empty();												//活转定活最大限额
			strTemp = ZERO8 + strOpCommandArray[25];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.HtodhMaxNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMAXNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.HtodhMaxNum));

			strTemp.Empty();												//活转定活最小限额
			strTemp = ZERO8 + strOpCommandArray[26];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.HtodhMinNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMINNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.HtodhMinNum));

			strTemp.Empty();												//活转通知存款最大限额
			strTemp = ZERO8 + strOpCommandArray[27];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.HtotzcqMaxNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.HtotzcqMaxNum));

			strTemp.Empty();												//活转通知存款最小限额
			strTemp = ZERO8 + strOpCommandArray[28];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.HtotzcqMinNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMINNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.HtotzcqMinNum));

			strTemp.Empty();												//活转整整最大限额
			strTemp = ZERO8 + strOpCommandArray[29];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.HtozzMaxNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMAXNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.HtozzMaxNum));

			strTemp.Empty();												//活转整整最小限额
			strTemp = ZERO8 + strOpCommandArray[30];
			strTemp = strTemp.Right(8);
			m_pTranCmn->InitInfo.HtozzMinNum = Asc2Int(strTemp);
			IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMINNUM, GetSprintf("%.0f", m_pTranCmn->InitInfo.HtozzMinNum));

			strOpResult = OP_NORMAL;
			strOpResult += "||";								// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_SETMASTERKEY")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR INIT CD KEY",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 初始化CDK",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{	
			//EPP ADD BY hyl 20070902
			if ((m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP) &&
				(strOpCommandArray[1].GetLength() == 32))
			{
				m_pDevCmn->fnPIN_InitializeEPP();
				m_pDevCmn->fnPIN_LoadIV("Vector");
				m_pDevCmn->fnPIN_ImportKey(TCONSTMASTERKEY, "", strOpCommandArray[1]);
				m_pDevCmn->fnPIN_EncryptECB("TRIDESECB", TCONSTMASTERKEY, "", 15, ZERO16);
				char checkValue[16];
				memcpy(checkValue, m_pDevCmn->fnPIN_GetEncryptedData(), 16);
				SetShareData("OP_CHECKVALUE", checkValue);
				m_pDevCmn->fnAPL_CheckDeviceAction(DEV_PIN);
				if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))
				{
					strOpResult = OP_NORMAL;
					strOpResult += "||";					// Success Message
					strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
				}
				else
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";					// Error Message
					strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
				}
			}
			                                                    // 需要发送CDK初始化包，校验CDK是否正常  by zjd
			m_pTranCmn->RQKCode2 = 0;
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_INITCDK);		// INIT CDK Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
		}
	}
	else
	if (strOpCommandArray[0] == "OP_REQUESTCDK")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR REQUEST CDK KEY",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 更新CDK",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		
		m_pTranCmn->RQKCode2 = 1;
		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_INITCDK);		// INIT CDK Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_REQUESTMAC")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR REQUEST MAC KEY",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员更新MAC密钥",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_INITMAC);		// INIT MAC Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_REQUESTPIN")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR REQUEST PIN KEY",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 操作员更新PIN密钥",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
		m_pTranCmn->fnAPP_MainTranProc(TRAN_INITPIN);		// INIT MAC Procedure
		m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		if (m_pDevCmn->TranResult)
		{
			strOpResult = OP_NORMAL;
			strOpResult += "||";							// Success Message
			strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";							// Error Message
			strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_RQK")
	{
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR REQUEST KEY",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_OPEN);			// RQK Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if ((strOpCommandArray[0] == "OP_TOTAL")	||				// 2005.08.22
		(strOpCommandArray[0] == "OP_TOTALCB"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s Operator ATMP TOTAL",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 终端轧账",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}


		if (strOpCommandArray[0] == "OP_TOTAL")
		{
			if (strOpCommandArray.GetSize() < 2)
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";								// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				memcpy(m_pTranCmn->szADDCashCounterNumber, GetSprintf("%07.7s",strOpCommandArray[1]).GetBuffer(0), 7);
				strTemp.Empty();
				strTemp = strOpCommandArray[2];
				strTemp = strTemp.Right(7);
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_INPUTREMAINAMOUNT, strTemp);
 				memset(m_pTranCmn->InputRemainAmount,0,sizeof(m_pTranCmn->InputRemainAmount));
				memcpy(m_pTranCmn->InputRemainAmount, GetSprintf("%010.10s00",strTemp).GetBuffer(0),12);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pTranCmn->InputRemainAmount[%s]",m_pTranCmn->InputRemainAmount);
			}
		}
        
//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))		// 2004.08.23
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR | DEV_CSH))
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALBATCHNO, 
						GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001")));

			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->InitInfo.TotalDisplayNo = 0;			// 2005.09.08
			m_pTranCmn->fnAPP_MainTranProc(TRAN_TOTAL);			// Total Procedure
			
			CString strTempResult;
			if (m_pTranCmn->TotalAddFlag)
				strTempResult = GetSprintf("后台拒绝, 应答码: %2.2s",m_pTranCmn->HostResp.RejectCode);
			else
				strTempResult = "超时重试并核对长短款";

            // 20080407 by hyl 
 		    if(m_pDevCmn->bAgentMode == 1)
			     m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_ATMPTOTAL);		// 2009.03.31 yusy

// 			if (m_pDevCmn->TranResult)
// 				;
// 			else
// 			{
// 				m_pTranCmn->fnAPP_MainTranProc(TRAN_SENDDETAIL);
// 			}
			if (m_pTranCmn->InitInfo.TotalDisplayNo == 2	&&
				IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_SENDDETAILFLAG, 1))
				m_pTranCmn->fnAPP_MainTranProc(TRAN_SENDDETAIL);

			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if(m_pDevCmn->bAgentMode == 1)
				m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
			if (m_pTranCmn->InitInfo.TotalDisplayNo == 1)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Total Match Message
				strOpResult += GuideMsgTbl[28][m_pDevCmn->OpDeviceLangMode];
				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "1");
			}
			else
			if (m_pTranCmn->InitInfo.TotalDisplayNo == 2)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Total Mismatch Message
				strOpResult += GuideMsgTbl[29][m_pDevCmn->OpDeviceLangMode];
				RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "1");
			}
			else
			if (m_pTranCmn->InitInfo.TotalDisplayNo == 3	||
				!m_pTranCmn->TotalAddFlag)
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Total Error Message
//				strOpResult += GuideMsgTbl[30][m_pDevCmn->OpDeviceLangMode];
				strOpResult += strTempResult;
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_TOTALPRE")
		{
			if (m_pTranCmn->LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s Operator ATMP TOTALPRE",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 预轧账",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4])));
			}			
			
			if (strOpCommandArray.GetSize() < 2)
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";								// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				memcpy(m_pTranCmn->szADDCashCounterNumber, GetSprintf("%07.7s",strOpCommandArray[1]).GetBuffer(0), 7);
			}
			
//			if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))		// 2004.08.23
			if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR | DEV_CSH))
			{
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALBATCHNO, 
					GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001")));
				
				m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
				m_pTranCmn->InitInfo.TotalDisplayNo = 0;
				m_pTranCmn->fnAPP_MainTranProc(TRAN_TOTALPRE);			// Total Procedure
				m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
				
//				if (m_pDevCmn->TranResult)
//				{
//					strOpResult = OP_NORMAL;
//					strOpResult += "||";							// Success Message
//					strOpResult += GuideMsgTbl[28][m_pDevCmn->OpDeviceLangMode];
//				}
//				else
//				{
//					strOpResult = OP_ERROR;
//					strOpResult += "||";							// Error Message
//					strOpResult += GuideMsgTbl[29][m_pDevCmn->OpDeviceLangMode];
//				}

				CString strTempResult = GetSprintf("后台拒绝, 应答码: %2.2s",m_pTranCmn->HostResp.RejectCode);

				if (m_pTranCmn->InitInfo.TotalDisplayNo == 1)
				{
					strOpResult = OP_NORMAL;
					strOpResult += "||";							// Total Match Message
					strOpResult += GuideMsgTbl[28][m_pDevCmn->OpDeviceLangMode];
					RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_ATMPTOTALS, "1");
				}
// 				else
// 				if (m_pTranCmn->InitInfo.TotalDisplayNo == 2)
// 				{
// 					strOpResult = OP_ERROR;
// 					strOpResult += "||";							// Total Mismatch Message
// 					strOpResult += GuideMsgTbl[29][m_pDevCmn->OpDeviceLangMode];
// 					strOpResult += strTempResult;
// 				}
				else
				if (m_pTranCmn->InitInfo.TotalDisplayNo == 3)
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";							// Total Error Message
//					strOpResult += GuideMsgTbl[30][m_pDevCmn->OpDeviceLangMode];
					strOpResult += strTempResult;
				}
				else
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";							// Error Message
					strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
				}
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";								// Device Error Message
				strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
			}
	}
	else
	if (strOpCommandArray[0] == "OP_TOTALIB")					// 2005.08.22
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s Operator ATMP TOTAL",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 与后台对账",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}

		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))		// 2004.08.23
		{
			if (strOpCommandArray.GetSize() >= 3)
			{													// 2005.09.11
				if ((strOpCommandArray[2].GetLength() >= 14)	&&
					(IsNum(strOpCommandArray[2].GetBuffer(0), 14)))
				{
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TOTALBATCHNO, GetSprintf("%014.14s", strOpCommandArray[2]));

					m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);	// Set Print Image Mode
					m_pTranCmn->InitInfo.TotalDisplayNo = 0;	// 2005.09.08
					m_pTranCmn->fnAPP_MainTranProc(TRAN_TOTAL);	// Total Procedure

					if (m_pDevCmn->TranResult)
						;
					else
						m_pTranCmn->fnAPP_MainTranProc(TRAN_SENDDETAIL);

					m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);	// Set Print Image Mode
					if(m_pDevCmn->bAgentMode == 1)
						m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
					if (m_pTranCmn->InitInfo.TotalDisplayNo == 1)
					{
						strOpResult = OP_NORMAL;
						strOpResult += "||";					// Total Match Message
						strOpResult += GuideMsgTbl[28][m_pDevCmn->OpDeviceLangMode];
					}
					else
					if (m_pTranCmn->InitInfo.TotalDisplayNo == 2)
					{
						strOpResult = OP_ERROR;
						strOpResult += "||";					// Total Mismatch Message
						strOpResult += GuideMsgTbl[29][m_pDevCmn->OpDeviceLangMode];
					}
					else
					if (m_pTranCmn->InitInfo.TotalDisplayNo == 3)
					{
						strOpResult = OP_ERROR;
						strOpResult += "||";					// Total Error Message
						strOpResult += GuideMsgTbl[30][m_pDevCmn->OpDeviceLangMode];
					}
					else
					{
						strOpResult = OP_ERROR;
						strOpResult += "||";					// Error Message
						strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
					}
				}
				else
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";						// Error Message
					strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
				}
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_SUBTOTAL")
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s Operator ATMC TOTAL",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 本地统计值",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
		}
																// 2005.04.19
//		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CLERK))
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_SUBTOTAL);		// Sub Toral Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if(m_pDevCmn->bAgentMode == 1)
				m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}

            if(m_pDevCmn->bAgentMode == 1)
			     m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_ATMCTOTAL);

		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_SPRPRINT")
	{
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))
		{
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR);
			for (i = 1; i < strOpCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR, i - 1, 0, strOpCommandArray[i]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR);

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				m_pDevCmn->DeviceTimeout &= ~DEV_SPR;
				m_pDevCmn->fnSPR_Eject(K_1_WAIT);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
				{
					m_pDevCmn->fnSPR_WaitTaken();
					if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
					{
						if (m_pDevCmn->fnSPR_GetMaterialInfo())
							m_pDevCmn->fnSPR_Retract();								
					}
				}
			}
				
			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Null Message
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_JPRPRINT")
	{
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
		{
			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_JPR);
			for (i = 1; i < strOpCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, i - 1, 0, strOpCommandArray[i]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_JPR);

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_JPR))
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Null Message
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_JPRDIRECTPRINT")
	{
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
		{
			if ((strOpCommandArray.GetSize() == 2)		&&
				(strOpCommandArray[1].GetLength() == 0))
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_JPR);
				for (i = 1; i < strOpCommandArray.GetSize();  i++)
					m_pDevCmn->fnAPL_EjrSpoolData(DEV_JPR, i - 1, 0, strOpCommandArray[i]);
				m_pDevCmn->fnAPL_EjrDirectPrint(DEV_JPR);

				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_JPR))
				{
					strOpResult = OP_NORMAL;
					strOpResult += "||";						// Success Message
					strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
				}
				else
				{
					strOpResult = OP_ERROR;
					strOpResult += "||";						// Error Message
					strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
				}
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else
	if (strOpCommandArray[0] == "OP_SPRJPRPRINT")
	{
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		if (!m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR | DEV_JPR))
		{														// 2004.06.30
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pDevCmn->fnAPL_EjrSpoolPrintStart(DEV_SPR | DEV_JPR);
			for (i = 1; i < strOpCommandArray.GetSize();  i++)
				m_pDevCmn->fnAPL_EjrSpoolData(DEV_SPR | DEV_JPR, i - 1, 0, strOpCommandArray[i]);
			m_pDevCmn->fnAPL_EjrSpoolEnd(DEV_SPR | DEV_JPR);

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
			{
				m_pDevCmn->DeviceTimeout &= ~DEV_SPR;
				m_pDevCmn->fnSPR_Eject(K_1_WAIT);
				if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
				{
					m_pDevCmn->fnSPR_WaitTaken();
					if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR))
					{
						if (m_pDevCmn->fnSPR_GetMaterialInfo())
							m_pDevCmn->fnSPR_Retract();								
					}
				}
			}

			if (m_pDevCmn->fnAPL_CheckDeviceAction(DEV_SPR | DEV_JPR))
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Null Message
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Device Error Message
			strOpResult += GuideMsgTbl[25][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else														// Not Found
	if (strOpCommandArray[0] == "OP_PINMODE")
	{
		if (strOpCommandArray.GetSize() < 2)
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";								// Flash Error Message
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
		else
		{
			if ((Asc2Int(strOpCommandArray[1]) >= PIN_DISABLE_MODE)		&&
				(Asc2Int(strOpCommandArray[1]) <= PIN_ALL_MODE))
			{
				switch (m_pDevCmn->AtmDefine.OpDevice)			// 2005.11.11
				{
					case NOT_SET:
						if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
							m_pDevCmn->fnAPL_DeviceEnDisable(DEV_PIN, ENABLE, Asc2Int(strOpCommandArray[1]));
						break;
					case OP_TOUCH:
						break;
					case OP_TSPL:
						break;
					case OP_TSPLCHN:
				        break;	
					case OP_HSPL:
						break;
				}

				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Null Message
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Flash Error Message
				strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
			}
		}
	}
	else
	if (strOpCommandArray[0] == "OP_CHANGESPRPAPER"  ||			// added by yusy 08.09.09
		strOpCommandArray[0] == "OP_CHANGEJPRPAPER"  ||
		strOpCommandArray[0] == "OP_CHANGEINK")
	{
		m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
		strOpResult = OP_NORMAL;
		strOpResult += "||";									// Success Message	added by yusy 09.02.16
		strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
	}
	else
	if ((strOpCommandArray[0] == "OP_TOTALKIND"))
	{
		if (m_pTranCmn->LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s OPERATOR ATMP TOTAL KIND",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 配置轧账方式",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4])));
		}

		if (strOpCommandArray.GetSize() >= 3)
		{
			memcpy(m_pTranCmn->szADDCashCounterNumber, GetSprintf("%07.7s",strOpCommandArray[1]).GetBuffer(0), 7);
			memcpy(&(m_pTranCmn->totalKindFlag), GetSprintf("%01.1s",strOpCommandArray[2]).GetBuffer(0), 1);
			m_pDevCmn->fnSPR_SetPrintImageMode(FALSE);			// Set Print Image Mode
			m_pTranCmn->fnAPP_MainTranProc(TRAN_TOTALKIND);		// Down Fit Procedure
			m_pDevCmn->fnSPR_SetPrintImageMode(TRUE);			// Set Print Image Mode
			if (m_pDevCmn->TranResult)
			{
				strOpResult = OP_NORMAL;
				strOpResult += "||";							// Success Message
				strOpResult += GuideMsgTbl[23][m_pDevCmn->OpDeviceLangMode];
			}
			else
			{
				strOpResult = OP_ERROR;
				strOpResult += "||";							// Error Message
				strOpResult += GuideMsgTbl[24][m_pDevCmn->OpDeviceLangMode];
			}
		}
		else
		{
			strOpResult = OP_ERROR;
			strOpResult += "||";
			strOpResult += GuideMsgTbl[22][m_pDevCmn->OpDeviceLangMode];
		}
	}
	else														// Not Found
	{
		strOpResult = OP_ERROR;
	}

/////////////////////////////////////////////////////////////////////////////
	ClerkProcInCnt = 1;
	return TRUE;
}
