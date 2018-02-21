/////////////////////////////////////////////////////////////////////////////
// TranHost.cpp : implementation file
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
//	SEND/RECV FUNCTION
/////////////////////////////////////////////////////////////////////////////
// Send Host
int CTranCmn::fnAPP_SendHost()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost()");

	int		ScrNum = 0;
	int		LineOpenTime = LINE_OPCL_TIME;
	int		LineSendTime = InitInfo.LineSendTime;
	
	int		iADTime = 0;										// 2008.05.09
	BYTE    c15Data[] = {0x24, 0x3b, 0x30, 0x8f, 0x0d, 0x0a};
	BYTE    c5Data[] = {0x24, 0x3a, 0x30, 0x8e, 0x0d, 0x0a};
	int		ret = 0, len = 0;
	BYTE	icdata[256];

	char	sLastSerialNo[9];									// 读出的上一次流水号

	CString strTmp("");
/////////////////////////////////////////////////////////////////////////////
//	Transaction Result Initialize
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->TranResult = FALSE;								// Host Result
	TotalAddFlag = 1;

/////////////////////////////////////////////////////////////////////////////
//	Send Host Variable Initialize
/////////////////////////////////////////////////////////////////////////////
	strBitMap = "";												// Bit Map
	SendLength = 0;												// Send Length
	MacLength = 0;												// Mac Length
	
	memset(SendBuffer, 0, sizeof(SendBuffer));					// Send Buffer
	memset(MacBuffer, 0, sizeof(MacBuffer));					// Mac Buffer

	memset(&HMESend, 0, sizeof(HMESend));						// Host Message Edit Send

	memset(&AtmStatusInfo, 0, sizeof(AtmStatusInfo));			// Atm Status Info
	memset(&AtmTotalInfo, 0, sizeof(AtmTotalInfo));				// Atm Total Info
	memset(&AtmAddCashInfo, 0, sizeof(AtmAddCashInfo));			// Atm AddCash Info

	memset(sLastSerialNo,0,sizeof(sLastSerialNo));
	
/////////////////////////////////////////////////////////////////////////////
//	Library Variable Initialize
/////////////////////////////////////////////////////////////////////////////
	RecvOkFlag = FALSE;											// 2006.07.03
	NmsSendFlag = FALSE;
	StatusEtcLength = 0;
	memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));

	DCEjrWriteFlag = FALSE;
	DCEjrSendCnt = 0;
	DCEjrSendData = "";

	ICScriptWriteFlag = 0;
	
    bAgentWithFlag = FALSE;            //取款成功标志

	if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||			// Open & Send Time Setting
		(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||			// 2004.06.22
		(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
		(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
		(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
	{
		LineOpenTime = LINE_BID_OPCL_TIME;
		LineSendTime = LINE_BID_SEND_TIME;
	}
	else
	{
		LineOpenTime = LINE_OPCL_TIME;
		LineSendTime = InitInfo.LineSendTime;
	}
	
/////////////////////////////////////////////////////////////////////////////
//	if ((m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||			// No Screen
//		(m_pDevCmn->TranStatus == TRAN_BIDCOMMAND)	||
//		(m_pDevCmn->TranStatus == TRAN_TOTAL)		||
//		(m_pDevCmn->TranStatus == TRAN_SUBTOTAL)	||			// 2004.06.22
//		(m_pDevCmn->TranStatus == TRAN_ADDCASH)		||
//		(m_pDevCmn->TranStatus == TRAN_SUBCASH)		||
//		(m_pDevCmn->TranStatus == TRAN_CLEARCASH)	||
//		(m_pDevCmn->TranStatus == TRAN_PRINTCASH)	||
//		(m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||
//		(m_pDevCmn->TranStatus == TRAN_SENDDETAIL)	||
//		(m_pDevCmn->TranStatus == TRAN_DETAILEND)	||
//		(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)	||
//		(m_pDevCmn->TranStatus == TRAN_DCSENDEJR)	||			// 2006.07.03
//		(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
//		(m_pDevCmn->TranStatus == TRAN_OPEN)		||
//		(m_pDevCmn->TranStatus == TRAN_DOWNPARM)	||
//		(m_pDevCmn->TranStatus == TRAN_DOWNPAY)		||
//		(m_pDevCmn->TranStatus == TRAN_DCEJREND))
	if (m_pDevCmn->TranStatus != TRAN_TRAN)				// No Screen
		;
	else
	{
		// 2008.05.09
		if(IniGetInt(_ADMACHINE_INI, _INIKEY_ADMACHINE, _INIKEY_ADMACHINE_ADMODE, 0) == 1)
		{
			if ( TranCode == TC_WITHDRAWAL)
			{
				iADTime =IniGetInt(_ADMACHINE_INI, _INIKEY_ADMACHINE, _INIKEY_ADMACHINE_LONGADTIME,15);
				ret=m_pDevCmn->fnAPP_WriteADComm(c15Data,6);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm=%d,c15Data", ret);
			}
			else
			{
				iADTime =IniGetInt(_ADMACHINE_INI, _INIKEY_ADMACHINE, _INIKEY_ADMACHINE_SHORTADTIME,5);
				ret=m_pDevCmn->fnAPP_WriteADComm(c5Data,6);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm=%d,c5Data", ret);
			}
		}
		
		if(InitInfo.TranDvrMode == 1)
		     fnAPP_DvrDataComm(CMDCODE_OVERLAY,DVRCHAN_IDX12,5,"PROCESS");

		if (m_pDevCmn->fnSCR_GetCurrentScreenNo() != 711)
			m_pDevCmn->fnSCR_DisplayScreen(711, iADTime);			// Processing Screen

		if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
			(m_pDevCmn->McuReadMSOnlyFlag))
			;
		else
		{
			if ((TranCode == TC_ICINQUIRY)	||
				(TranCode == TC_ICDETAIL))
			{
//				PbcAppIndex = 0;								// Pboc Tran AppIndex Set : 2010.10.20	liuxl 20110510
				if (fnAPD_PbcFinalApp(SEL_FOR_LOG) != T_OK)		// Pboc Final App
					fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				SeriesTran = 1;									// added by liuxl 20110926
			}
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
					TranCode == TC_ECHANNEL_INQ	||
					TranCode == TC_ECHANNEL		||
					TranCode == TC_INQUIRY_CheckPin)
				{
//					PbcAppIndex = 0;								// Pboc Tran AppIndex Set : 2010.10.20	liuxl 20110510
					if (SeriesTran == 1)							// added by yusy 20110526
					{
//						if (fnAPD_PbcSelApp(SEL_FOR_TRAN, 0) != T_OK)		// Pboc Sel App
//							fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);

						if (fnAPD_PbcFinalApp(SEL_FOR_TRAN) != T_OK)	// Pboc Final App
							fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
						
						if (fnAPD_PbcReadApp() != T_OK)					// Pboc Read App
							fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
					}
					else
						SeriesTran = 1;

					if (fnAPD_PbcStoreTlv() != T_OK)				// Pboc Store Tlv
						fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);

					if (fnAPD_PbcReadyTrans() != T_OK)				// Pboc Ready Trans
						fnAPP_CancelProc(T_MSG, TranMsgTbl[22][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				}
			}
		}
	}
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	if (!AddSerialFlag)											// China Spec : 2005.04.19 所有的通讯包都要流水号  by zjd
	{
		AddSerialFlag = TRUE;									// Add Serial Flag
		m_pDevCmn->fnAPL_AddSerialNo();							// Add Serial No
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::m_pProfile->TRANS.SerialNo(%8.8s)", m_pProfile->TRANS.SerialNo);

		// 读文件中的记录比较：如果相同，程序终止；如果不同，程序继续
		memcpy(sLastSerialNo, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, "LastSerialNo", "00000000"), 8);	
		if (memcmp(sLastSerialNo,m_pProfile->TRANS.SerialNo,8) == 0)
		{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost::m_pProfile->TRANS.SerialNo ERR");
			fnAPP_CancelProc(T_MSG, TranMsgTbl[15][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
		}
		else
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, "LastSerialNo", GetSprintf("%8.8s", m_pProfile->TRANS.SerialNo));
		}
	}
	SendLength = fnAPP_SendHostMake(TRUE);						// Send Host Make

/////////////////////////////////////////////////////////////////////////////
//	Recv Host Variable Initialize
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus != TRAN_CONFIRMWITH )				// 2005.08.22
	{
		RecvLength = 0;											// Recv Length
		
		memset(RecvBuffer, 0, sizeof(RecvBuffer));				// Recv Buffer

		memset(&HMERecv, 0, sizeof(HMERecv));					// Host Message Edit Recv

		memset(&PSPRecvData, 0, sizeof(PSPRecvData));			// Parameter Slip Print Recv Data
		memset(&PPPRecvData, 0, sizeof(PPPRecvData));			// Parameter Passbook Print Recv Data
		memset(&PPWRecvData, 0, sizeof(PPWRecvData));			// Parameter Passbook Write Recv Data
		memset(&PCWRecvData, 0, sizeof(PCWRecvData));			// Parameter Card Write Recv Data
		memset(&PMERecvData, 0, sizeof(PMERecvData));			// Parameter Message Edit Recv Data
		memset(&PMDRecvData, 0, sizeof(PMDRecvData));			// Parameter Message Display Recv Data

/////////////////////////////////////////////////////////////////////////////
//	After Recv Host Variable Initialize
/////////////////////////////////////////////////////////////////////////////
		memset(&HostResp, 0, sizeof(HostResp));
	}
	
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_ONLINE_MODE)			// Host Online Mode
	{
		if (!SendLength)
			fnAPP_CancelProc(T_PROGRAM);
	}

	if (!m_pDevCmn->fnAPL_CheckHostLine())						// Check Host Line
	{
		if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||		// No Cancel
			(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||		// 2004.06.22
			(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
			(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
			(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
			return T_OK;										// DO NOT MODIFY : 2005.08.22
		else
		{
//			if (m_pDevCmn->TranStatus != TRAN_OPEN) 			// NOT OPEN TRANSACTION : 2005.05.26
//				m_pDevCmn->fnAPL_StackError("7710891", "HOST LINE FAIL");
			fnAPP_CancelProc(T_MSG, TranMsgTbl[8][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
		}
	}

	if ((TranCode == TC_ICINQUIRY)	||							// 2010.10.20
		(TranCode == TC_ICDETAIL))
	{
		return T_OK;
	}

	if (m_pProfile->NETWORK.InterfaceKind == 2500)				// POST-TY(HNC)			// 2007.05.15 : ATMU
		;
	else
	{
		if (fnAPP_LineOpen(LineOpenTime) != T_OK)					
		{
			if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||		// No Cancel
				(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||		// 2004.06.22
				(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
				(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
				(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
				return T_OK;										// DO NOT MODIFY : 2005.08.22
			else
			{
				if (m_pDevCmn->TranStatus != TRAN_OPEN) 			// NOT OPEN TRANSACTION : 2005.05.26
				{
					m_pDevCmn->fnAPL_StackError("7710892", "HOST OPEN FAIL");		//  打印流水发送错  by zjd
					if (LangMode)
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****COMM SEND ERROR****",
							GetTime().GetBuffer(0),
							&(GetTime().GetBuffer(0)[2]),
							&(GetTime().GetBuffer(0)[4])));
					}
					else
					{
						m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****交易申请包发送错误****",
							GetTime().GetBuffer(0),
							&(GetTime().GetBuffer(0)[2]),
							&(GetTime().GetBuffer(0)[4])));
					}
				}
				fnAPP_CancelProc(T_MSG, TranMsgTbl[8][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}
		}
	}

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus == TRAN_TRAN)						// TRANSACTION	added by liuxl 20110714
	{
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANPROC, TranProc);
//		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCODE, TranCode);
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANDATA, MakeUnPack(SendBuffer, SendLength));
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR, "0000000");
		IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, T_OK);
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANKIND, 
//				  MakeUnPack(HMESend.Data[3], HMESend.DataLength[3]));
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANSERIALNO, GetSprintf("%8.8s", m_pProfile->TRANS.SerialNo));
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANDATETIME, 
//				  MakeUnPack(HMESend.Data[7], HMESend.DataLength[7]));
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD32, "");
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD33, "");

		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO, "");

		if (TranProc == TRAN_WITH		||
			TranCode == TC_REMIT_ADD	|| 
			TranCode == TC_REMIT_PWD	|| 
			TranCode == TC_REMIT_IN)
		{
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, MakeUnPack(SendBuffer, SendLength));
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHERROR, "0000000");
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSUM, 0);
//			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT, 0);
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO, GetSprintf("%8.8s", m_pProfile->TRANS.SerialNo));
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT, GetSprintf("%s", CardAccountNo));
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME, 
					  MakeUnPack(HMESend.Data[13], HMESend.DataLength[13]) + MakeUnPack(HMESend.Data[12], HMESend.DataLength[12]));
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT, GetSprintf("%12.12s", Accept.Money));
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO, "");
																// 2005.09.08
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD32, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD33, "");

			//20080319 by hyl 取款手续费 以分为单位
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFEE, GetSprintf("%08d",Asc2Float(HMERecv.Data[28], 8) + Asc2Float(HMERecv.Data[29], 8)));
            IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHLOCALCARDFLAG,GetSprintf("%d",LocalCardFlag));
//			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 1);
		}
		else
		if (TranCode == TC_LOAD)
		{
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, MakeUnPack(SendBuffer, SendLength));
 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHERROR, "0000000");
 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSUM, 0);
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT, 0);
 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO, GetSprintf("%8.8s", m_pProfile->TRANS.SerialNo));
 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT, GetSprintf("%s", CardAccountNo));
 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME, 
 				MakeUnPack(HMESend.Data[13], HMESend.DataLength[13]) + MakeUnPack(HMESend.Data[12], HMESend.DataLength[12]));
 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT, GetSprintf("%12.12s", Accept.Money));
 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO, "");
// 			// 2005.09.08
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD32, "");
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD33, "");
// 			
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 5);
		}
//		if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD || TranCode == TC_REMIT_IN)		// 2008.10.13
//		{
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 3);		//汇兑异常通知
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, MakeUnPack(SendBuffer, SendLength));
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHERROR, "0000000");
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSUM, 0);
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT, 0);
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO, GetSprintf("%8.8s", m_pProfile->TRANS.SerialNo));
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT, GetSprintf("%s", CardAccountNo));
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME, 
// 					  MakeUnPack(HMESend.Data[13], HMESend.DataLength[13]) + MakeUnPack(HMESend.Data[12], HMESend.DataLength[12]));
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT, GetSprintf("%12.12s", Accept.Money));
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO, "");
// 																// 2005.09.08
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD32, "");
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD33, "");
//		}
	}
/////////////////////////////////////////////////////////////////////////////
	strTmp.Format("%2.2s:%2.2s:%2.2s [TCPIP SENDDATA], Length : [%d]",
								GetTime().GetBuffer(0),
								&(GetTime().GetBuffer(0)[2]),
								&(GetTime().GetBuffer(0)[4]),
								SendLength);
	m_pDevCmn->fnCMN_SaveLineTrace(strTmp.GetBuffer(0), strTmp.GetLength());
///2014.04.19 by jl  屏蔽Line日志中报文信息/////
//	m_pDevCmn->fnCMN_SaveLineTrace(SendBuffer, SendLength, 1);

	if ((m_pDevCmn->TranStatus == TRAN_TRAN)	||
		(m_pDevCmn->TranStatus == TRAN_CANCEL))           //打印交易要在发包之前 BY HYL
	{
		switch (TranCode)
		{
		case TC_INQUIRY:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  INQ",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_INQUIRY_CheckPin:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  INQ",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 查询验密",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;

		case TC_CUS_INF_INQ:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  INQ",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 户名查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_CUS_INF_INQ_OTHERBANK:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  INQ",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 跨行户名查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_ECHANNEL_INQ:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  INQ",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 电银查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;

		case TC_SMSPARITY:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  SMSPARITY",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 短信验证",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;

		case TC_ECHANNEL:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  ECHANNEL",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 开通电渠",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
			
		case TC_DETAIL:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  IND",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 明细查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}

			break;
		// 20080904 by yaokq for 绿卡通查询其它储种
		case TC_OTHER_INQ:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  INC",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 储种查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_WITHDRAWAL:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  CWD AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 取款:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
			}
			break;
		case TC_TRANSFER:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  TFR AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TFR_TO:%-28.28s", Accept.InAccountNum));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 转账:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转入账号:%-28.28s", Accept.InAccountNum));
			}
			break;
		case TC_LOAD:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  TSAL AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 圈存:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
			}
			break;
		case TC_BOUNDMDOIFY:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  MAL AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 修限:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
			}
			break;
		case TC_HZT:														//liyi add 绿卡通
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  HZT AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				if (m_pTranCmn->strTFRSubType == "7")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TFR_TO:1  CQ:%s",m_pTranCmn->strTFRInData));
				}
				else if (m_pTranCmn->strTFRSubType == "8")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TFR_TO:3  CQ:%s",m_pTranCmn->strTFRInData));
				}
				else if (m_pTranCmn->strTFRSubType == "9")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TFR_TO:2  CQ:%s",m_pTranCmn->strTFRInData));
				}
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 活它:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				if (m_pTranCmn->strTFRSubType == "7")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转入储种:整整 存期/品种:%s",m_pTranCmn->strTFRInData));
				}
				else if (m_pTranCmn->strTFRSubType == "8")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转入储种:定活 存期/品种:%s",m_pTranCmn->strTFRInData));
				}
				else if (m_pTranCmn->strTFRSubType == "9")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转入储种:通知 存期/品种:%s",m_pTranCmn->strTFRInData));
				}
			}
			break;
		case TC_TZH:														//liyi add 绿卡通
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  TZH AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TFR_FROM:%s  %s",m_pTranCmn->strTFRSubType,m_pTranCmn->strTFRSubNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 它活:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				if (m_pTranCmn->strTFRSubType == "1")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转出储种:整整 子账号:%s",m_pTranCmn->strTFRSubNo));
				}
				else if (m_pTranCmn->strTFRSubType == "3")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转出储种:定活 子账号:%s",m_pTranCmn->strTFRSubNo));
				}
				else if (m_pTranCmn->strTFRSubType == "2")
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转出储种:通知 子账号:%s",m_pTranCmn->strTFRSubNo));
				}
			}
			break;
		case TC_REMIT:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  SKI",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 收款人信息查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_REMIT_INF_INQ:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  HPI",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 汇票信息查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_REMIT_ADD:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  AZHK AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("SKR DM:%s",m_pTranCmn->strRemitAddNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 址汇:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("收款人信息代码:%s",m_pTranCmn->strRemitAddNo));
			}
			break;
		case TC_REMIT_PWD:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  MMHK AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("SKR DM:%s",m_pTranCmn->strRemitAddNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 密汇:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("收款人信息代码:%s",m_pTranCmn->strRemitAddNo));
			}
			break;
		case TC_REMIT_IN:	
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  HKRZ AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("HPHM:%s",m_pTranCmn->strRemitInNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 入账:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("汇票号码:%s",m_pTranCmn->strRemitInNo));
			}
			break;
		case TC_PAY:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  PAY AMT:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TFR_TO:%-28.28s", Accept.InAccountNum));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 缴充:%s",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo,
					GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100)));
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("转入账号:%-28.28s", Accept.InAccountNum));
			}
			
			break;
		case TC_PAY_INQ:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  PAY INQ",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 缴充查询",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}

			break;
		case TC_CHANGEPIN:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  PIN",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 改密",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}

			break;
		case TC_CANCEL:	
			if (TranCode2 == 0)							// 2008.10.13
			{
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  CWC AMT:%s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						m_pProfile->TRANS.SerialNo,
						GetSprintf("%.2f",Asc2Float(CwcInfo.TranAmount, sizeof(CwcInfo.TranAmount))/100)));						// added by liuxl 20110805
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CPAN:%s OTNRS:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT),
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 冲正:%s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						m_pProfile->TRANS.SerialNo,
						GetSprintf("%.2f",Asc2Float(CwcInfo.TranAmount, sizeof(CwcInfo.TranAmount))/100)));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("冲正卡号:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT)));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("原交易流水号:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)));
				}
			}
			else
			if (TranCode2 == 2)
			{
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						m_pProfile->TRANS.SerialNo));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("HDYCTZ AMT:%s",GetSprintf("%.2f", Asc2Float(GetSprintf("%12.12s", CwcInfo.TranAmount)) / 100)));		// added by liuxl 20111015
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CPAN:%s OTNRS:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT),
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 冲正:%s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						m_pProfile->TRANS.SerialNo,
						GetSprintf("%.2f", Asc2Float(GetSprintf("%12.12s", CwcInfo.TranAmount)) / 100)));		// added by liuxl 20111015
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("冲正卡号:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT)));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("原交易流水号:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)));
				}
			}
			else
			if (TranCode2 == 3)
			{
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						m_pProfile->TRANS.SerialNo));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("TSALCZ AMT:%s",GetSprintf("%.2f", Asc2Float(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT)) / 100)));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("CPAN:%s OTNRS:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT),
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 冲正:%s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						m_pProfile->TRANS.SerialNo,
						GetSprintf("%.2f", Asc2Float(GetSprintf("%12.12s", CwcInfo.TranAmount)) / 100)));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("冲正卡号:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT)));
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("原交易流水号:%s",
						IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)));
				}
					
			}
			break;
		case TC_TOTAL:
		case TC_TOTALPRE:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  TTI",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 发起对账交易",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		case TC_TOTALKIND:
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  TTK",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 选择轧账方式",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			break;
		default:														// 需要再补充   by zjd
			if (LangMode)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s  ???",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			else
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 流水号:%8.8s 未知交易",
					GetTime().GetBuffer(0),
					&(GetTime().GetBuffer(0)[2]),
					&(GetTime().GetBuffer(0)[4]),
					m_pProfile->TRANS.SerialNo));
			}
			
		
			break;
		}

		if ((TranCode == TC_WITHDRAWAL)	||
			(TranCode == TC_TRANSFER)	||
			(TranCode == TC_LOAD)		||			//added by yusy ic
			(TranCode == TC_BOUNDMDOIFY)||
			(TranCode == TC_HZT)		||			//liyi add 绿卡通
			(TranCode == TC_TZH)		||
			(TranCode == TC_REMIT_ADD)	||
			(TranCode == TC_REMIT_IN)	||
			(TranCode == TC_REMIT_PWD)	||
			(TranCode == TC_PAY)        ||
			(TranCode == TC_PAY_INQ)	||
			(TranCode == TC_OTHER_INQ))				//20080904 by yaokq
		{
/////////////////////////////////////////////////////////////////////////////
			fnAPL_WriteTransactonEjr(1);						// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
		}

		if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||		// 2010.10.20
			(m_pDevCmn->McuReadMSOnlyFlag))
			;
		else
		{
			if ((TranCode == TC_WITHDRAWAL)	||
				(TranCode == TC_INQUIRY)	||
				(TranCode == TC_TRANSFER)	||
				(TranCode == TC_LOAD)		||		
				(TranCode == TC_BOUNDMDOIFY)||
				(TranCode == TC_CHANGEPIN)	||
				(TranCode == TC_HZT)		||			
				(TranCode == TC_TZH)		||
				(TranCode == TC_ECHANNEL_INQ)	||
				(TranCode == TC_ECHANNEL)	||
				(TranCode == TC_INQUIRY_CheckPin))
				{
					memset(icdata, 0, sizeof(icdata));
					m_pDevCmn->fnPBC_TlvGetValue(0x9F36, icdata, &len);
					memcpy(icdata9F36, icdata, len);
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATC: %s",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4]),
						GetSprintf("%08d", ByteHighOrder2Int(icdata, 2))));
				}
		}
	}

	if (fnAPP_SendData(SendBuffer, SendLength, LineSendTime) != T_OK)
	{
//		if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||		// No Cancel
//			(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||		// 2004.06.22
//			(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
//			(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
//			(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
//		{
//			HostNms.AexCustomerCardTimeOutSendFlag = FALSE;
//			HostNms.AexCustomerCashTimeOutSendFlag = FALSE;
//			HostNms.AexCustomerHostRetractSendFlag = FALSE;
//			return T_OK;										// DO NOT MODIFY : 2005.08.22
//		}
//		else
		{
			if ((TranCode == TC_WITHDRAWAL)	||
				(TranCode == TC_TRANSFER)	||
				(TranCode == TC_LOAD)		||			//added by yusy ic
				(TranCode == TC_BOUNDMDOIFY)||
				(TranCode == TC_HZT)		||			//liyi add 绿卡通
				(TranCode == TC_TZH)		||
				(TranCode == TC_REMIT_ADD)	||
				(TranCode == TC_REMIT_IN)	||
				(TranCode == TC_REMIT_PWD)	||
				(TranCode == TC_PAY)        ||
				(TranCode == TC_PAY_INQ)	||
				(TranCode == TC_OTHER_INQ))					//20080904 by yaokq
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(14);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
			}

			if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||		// No Cancel add by hyl
				(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||		
				(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
				(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
				(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
			{
				HostNms.AexCustomerCardTimeOutSendFlag = FALSE;
				HostNms.AexCustomerCashTimeOutSendFlag = FALSE;
				HostNms.AexCustomerHostRetractSendFlag = FALSE;
			}

			if (m_pDevCmn->TranStatus != TRAN_OPEN) 			// NOT OPEN TRANSACTION : 2005.05.26
			{													//  打印流水发送错  by zjd
				m_pDevCmn->fnAPL_StackError("8860291", "SEND TIME OVER");
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****COMM SEND ERROR****",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****交易申请包发送错误****",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4])));
				}
			}
			fnAPP_CancelProc(T_SENDERROR);
		}
	}

	if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||		// No Cancel
		(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||		// 2004.06.22
		(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
		(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
		(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
	{
		HostNms.AexCustomerCardTimeOutSendFlag = FALSE;
		HostNms.AexCustomerCashTimeOutSendFlag = FALSE;
		HostNms.AexCustomerHostRetractSendFlag = FALSE;
		return T_OK;										// DO NOT MODIFY : 2005.08.22
	}

	
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHost():return");
	return T_OK;
}

// Get Atm Status Info
int CTranCmn::fnAPP_GetAtmStatusInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo()");

	int		TempRejectCount1 = 0, TempRejectCount2 = 0, TempRejectCount3 = 0, TempRejectCount4 =0;
	int		TotalRejectCount = 0;
	int		nRemainAmount = 0;
	char	szTempBuff[1024];									// 2005.09.11
	int		tempcount;

	memset(szTempBuff, 0, sizeof(szTempBuff));
	memset(&AtmStatusInfo, '0', sizeof(AtmStatusInfo));

	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))				// Cdu Status
		AtmStatusInfo.CduStatus[0] = '8';
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		AtmStatusInfo.CduStatus[0] = '8';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.CduStatus[%1.1s]", AtmStatusInfo.CduStatus);
		
	AtmStatusInfo.DepStatus[0] = '9';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.DepStatus[%1.1s]", AtmStatusInfo.DepStatus);
			
		
	AtmStatusInfo.ICStatus[0] = '0';
	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))				// Mcu Status
		AtmStatusInfo.McuStatus[0] = '8';					
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))						
		AtmStatusInfo.McuStatus[0] = '8';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.McuStatus[%1.1s]", AtmStatusInfo.McuStatus);

	if (m_pDevCmn->DoorStatus == DOOR_OPENED)					// Door Status
		AtmStatusInfo.DoorStatus[0] = '9';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.DoorStatus[%1.1s]", AtmStatusInfo.DoorStatus);

	AtmStatusInfo.DESStatus[0] = '0';
	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_PIN))				// Pin Status
	{
		AtmStatusInfo.PinpadStatus[0] = '8';
//		AtmStatusInfo.DESStatus[0] = '8';
	}
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))
	{
		AtmStatusInfo.PinpadStatus[0] = '8';
//		AtmStatusInfo.DESStatus[0] = '8';
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.PinpadStatus[%1.1s]", AtmStatusInfo.PinpadStatus);

	//增加取款锁定 added by liuxl 20110609
	int icwdmaxshutter = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM, 1);
	if (icwdmaxshutter > 0 &&
		IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, 0) == icwdmaxshutter)
	{
		AtmStatusInfo.CduStatus[0] = '8';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.CduStatus(Change)[%1.1s]", AtmStatusInfo.CduStatus);
 	}

	AtmStatusInfo.TicketStatus[0] = '9';
	AtmStatusInfo.VoiceStatus[0] = '9';
	AtmStatusInfo.PassbookStatus[0] = '9';
	AtmStatusInfo.DevelopStatus[0]= '9';
	AtmStatusInfo.DVRStatus[0] = '9';
	AtmStatusInfo.Cam1Status[0] = '9';
	AtmStatusInfo.Cam2Status[0] = '9';
	AtmStatusInfo.Cam3Status[0] = '9';

	//增加DVR与摄像头状态上送状态 BY HYL
	if(InitInfo.TranDvrMode == 1)
	{
		if(memcmp(InitInfo.DvrStatus,"0004",4) == 0  ||
			memcmp(InitInfo.DvrStatus,"0014",4) == 0 ||
			memcmp(InitInfo.DvrStatus,"0024",4) == 0 ||
			memcmp(InitInfo.DvrStatus,"0044",4) == 0 ||
			memcmp(InitInfo.DvrStatus,"0084",4) == 0)
			AtmStatusInfo.DVRStatus[0] = '8';
		else
			AtmStatusInfo.DVRStatus[0] = '0';

		tempcount = 0;
		for (int i = 0; i<4; i++)
		{
			if (InitInfo.DvrChanStatus[i] == '2')
				tempcount++;
		}

		if (tempcount <= 1)
		{
			AtmStatusInfo.Cam1Status[0] = '0';
			AtmStatusInfo.Cam2Status[0] = '0';
			AtmStatusInfo.Cam3Status[0] = '0';
		}
		else
		{
			if(InitInfo.DvrChanStatus[0] == '0'||InitInfo.DvrChanStatus[0] == '1')
				AtmStatusInfo.Cam1Status[0] = '0';
			else
			if(InitInfo.DvrChanStatus[0] == '2')
				AtmStatusInfo.Cam1Status[0] = '8';
			
			if(InitInfo.DvrChanStatus[1] == '0'||InitInfo.DvrChanStatus[1] == '1')
				AtmStatusInfo.Cam2Status[0] = '0';
			else
			if(InitInfo.DvrChanStatus[1] == '2')
				AtmStatusInfo.Cam2Status[0] = '8';
			
			if(InitInfo.DvrChanStatus[2] == '0'||InitInfo.DvrChanStatus[2] == '1')
				AtmStatusInfo.Cam3Status[0] = '0';
			else
			if(InitInfo.DvrChanStatus[2] == '2')
				AtmStatusInfo.Cam3Status[0] = '8';
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.DVRStatus[%1.1s]", AtmStatusInfo.DVRStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cam1Status[%1.1s]", AtmStatusInfo.Cam1Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cam2Status[%1.1s]", AtmStatusInfo.Cam2Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cam3Status[%1.1s]", AtmStatusInfo.Cam3Status);


	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))				// Jpr Status
		AtmStatusInfo.JprStatus[0] = '9';
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
		AtmStatusInfo.JprStatus[0] = '8';
	else
	if (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)
		AtmStatusInfo.JprStatus[0] = '2';
	else
	if (m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)
		AtmStatusInfo.JprStatus[0] = '8';
	else
	if (m_pDevCmn->JnlStatus == JNL_NORMAL)
		AtmStatusInfo.JprStatus[0] = '0';
	else
	if (m_pDevCmn->JnlStatus == JNL_LOW_END)
		AtmStatusInfo.JprStatus[0] = '1';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.JprStatus[%1.1s]", AtmStatusInfo.JprStatus);

	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))				// Spr Status
		AtmStatusInfo.SprStatus[0] = '9';
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))
		AtmStatusInfo.SprStatus[0] = '8';
	else
	if (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)
		AtmStatusInfo.SprStatus[0] = '2';
	else
	if (m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)
		AtmStatusInfo.SprStatus[0] = '8';
	else
	if (m_pDevCmn->SlipStatus == SLIP_NORMAL)
		AtmStatusInfo.SprStatus[0] = '0';
	else
	if (m_pDevCmn->SlipStatus == SLIP_LOW_END)
		AtmStatusInfo.SprStatus[0] = '1';

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.SprStatus[%1.1s]", AtmStatusInfo.SprStatus);
	AtmStatusInfo.DvrHDStatus[0] = '9';
	if(InitInfo.TranDvrMode == 1)
	{
       AtmStatusInfo.DvrHDStatus[0] = '0';
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.DvrHDStatus[%1.1s]", AtmStatusInfo.DvrHDStatus);


	AtmStatusInfo.Deno1Status[0] = '9';							// Missing
	AtmStatusInfo.Deno2Status[0] = '9';							// Missing
	AtmStatusInfo.Deno3Status[0] = '9';							// Missing
	AtmStatusInfo.Deno4Status[0] = '9';							// Missing
	AtmStatusInfo.Deno5Status[0] = '9';							// Missing
	AtmStatusInfo.Deno6Status[0] = '9';							// Missing
	
	if (m_pDevCmn->CSTCnt >= 1)
	{
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))		// 2008.11.03				
//			AtmStatusInfo.Deno1Status[0] = '8';					// Error
//		else
		if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
			AtmStatusInfo.Deno1Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->FstCSTStatus == CST_NORMAL)
			AtmStatusInfo.Deno1Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->FstCSTStatus == CST_NEAR)
			AtmStatusInfo.Deno1Status[0] = '1';					// Low
		else
		if (m_pDevCmn->FstCSTStatus == CST_EMPTY)
			AtmStatusInfo.Deno1Status[0] = '2';					// Empty
		else
		if (m_pDevCmn->FstCSTStatus == CST_FULL)
			AtmStatusInfo.Deno1Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->FstCSTStatus == CST_ERROR)
			AtmStatusInfo.Deno1Status[0] = '8';					// Error
		else
		if (m_pDevCmn->FstCSTStatus == CST_HIGH)
			AtmStatusInfo.Deno1Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Deno1Status[%1.1s]", AtmStatusInfo.Deno1Status);
	}
	if (m_pDevCmn->CSTCnt >= 2)
	{
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))		// 2008.11.03			
//			AtmStatusInfo.Deno2Status[0] = '8';					// Error
// 		else
		if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
			AtmStatusInfo.Deno2Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->SndCSTStatus == CST_NORMAL)
			AtmStatusInfo.Deno2Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->SndCSTStatus == CST_NEAR)
			AtmStatusInfo.Deno2Status[0] = '1';					// Low
		else
		if (m_pDevCmn->SndCSTStatus == CST_EMPTY)
			AtmStatusInfo.Deno2Status[0] = '2';					// Empty
		else
		if (m_pDevCmn->SndCSTStatus == CST_FULL)
			AtmStatusInfo.Deno2Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->SndCSTStatus == CST_ERROR)
			AtmStatusInfo.Deno2Status[0] = '8';					// Error
		else
		if (m_pDevCmn->SndCSTStatus == CST_HIGH)
			AtmStatusInfo.Deno2Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Deno2Status[%1.1s]", AtmStatusInfo.Deno2Status);
	}
	if (m_pDevCmn->CSTCnt >= 3)
	{
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))		// 2008.11.03			
//			AtmStatusInfo.Deno3Status[0] = '8';					// Error
//		else
		if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
			AtmStatusInfo.Deno3Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NORMAL)
			AtmStatusInfo.Deno3Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
			AtmStatusInfo.Deno3Status[0] = '1';					// Low
		else
		if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)
			AtmStatusInfo.Deno3Status[0] = '2';					// Empty
		else
		if (m_pDevCmn->TrdCSTStatus == CST_FULL)
			AtmStatusInfo.Deno3Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->TrdCSTStatus == CST_ERROR)
			AtmStatusInfo.Deno3Status[0] = '8';					// Error
		else
		if (m_pDevCmn->TrdCSTStatus == CST_HIGH)
			AtmStatusInfo.Deno3Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Deno3Status[%1.1s]", AtmStatusInfo.Deno3Status);
	}
	if (m_pDevCmn->CSTCnt >= 4)
	{
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))		// 2008.11.03				
//			AtmStatusInfo.Deno4Status[0] = '8';					// Error
//		else
		if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
			AtmStatusInfo.Deno4Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->FthCSTStatus == CST_NORMAL)
			AtmStatusInfo.Deno4Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->FthCSTStatus == CST_NEAR)
			AtmStatusInfo.Deno4Status[0] = '1';					// Low
		else
		if (m_pDevCmn->FthCSTStatus == CST_EMPTY)
			AtmStatusInfo.Deno4Status[0] = '2';					// Empty
		else
		if (m_pDevCmn->FthCSTStatus == CST_FULL)
			AtmStatusInfo.Deno4Status[0] = '0';					// Normal
		else
		if (m_pDevCmn->FthCSTStatus == CST_ERROR)
			AtmStatusInfo.Deno4Status[0] = '8';					// Error
		else
		if (m_pDevCmn->FthCSTStatus == CST_HIGH)
			AtmStatusInfo.Deno4Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Deno4Status[%1.1s]", AtmStatusInfo.Deno4Status);
	}

	AtmStatusInfo.DepBoxStatus[0] = '9';

	AtmStatusInfo.RejectStatus[0] = '0';
	if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
		AtmStatusInfo.RejectStatus[0] = '8';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_NORMAL)
		AtmStatusInfo.RejectStatus[0] = '0';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_NEAR)					// 2008.12.04
		AtmStatusInfo.RejectStatus[0] = '0';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)				// 2008.12.04
		AtmStatusInfo.RejectStatus[0] = '0';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_FULL)
		AtmStatusInfo.RejectStatus[0] = '3';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_HIGH)
		AtmStatusInfo.RejectStatus[0] = '4';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.RejectStatus[%1.1s]", AtmStatusInfo.RejectStatus);
	
	if (m_pDevCmn->CSTCnt >= 1)
	{
		if (m_pDevCmn->FstCSTValue == 100)
		{
			AtmStatusInfo.Cst1Value[0] = '1';
			AtmStatusInfo.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
		else if (m_pDevCmn->FstCSTValue == 50)
		{
			AtmStatusInfo.Cst1Value[0] = '2';
			AtmStatusInfo.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
		else if (m_pDevCmn->FstCSTValue == 10)
		{
			AtmStatusInfo.Cst1Value[0] = '3';
			AtmStatusInfo.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
		else if (m_pDevCmn->FstCSTValue == 20)
		{
			AtmStatusInfo.Cst1Value[0] = '4';
			AtmStatusInfo.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst1Value[%2.2s]", AtmStatusInfo.Cst1Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst1Count[%4.4s]", AtmStatusInfo.Cst1Count);
	}
	if (m_pDevCmn->CSTCnt >= 2)
	{
		if (m_pDevCmn->SndCSTValue == 100)
		{
			AtmStatusInfo.Cst2Value[0] = '1';
			AtmStatusInfo.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
		else if (m_pDevCmn->SndCSTValue == 50)
		{
			AtmStatusInfo.Cst2Value[0] = '2';
			AtmStatusInfo.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
		else if (m_pDevCmn->SndCSTValue == 10)
		{
			AtmStatusInfo.Cst2Value[0] = '3';
			AtmStatusInfo.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
		else if (m_pDevCmn->SndCSTValue == 20)
		{
			AtmStatusInfo.Cst2Value[0] = '4';
			AtmStatusInfo.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst2Value[%2.2s]", AtmStatusInfo.Cst2Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst2Count[%4.4s]", AtmStatusInfo.Cst2Count);
	}
	if (m_pDevCmn->CSTCnt >= 3)
	{
		if (m_pDevCmn->TrdCSTValue == 100)
		{
			AtmStatusInfo.Cst3Value[0] = '1';
			AtmStatusInfo.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == 50)
		{
			AtmStatusInfo.Cst3Value[0] = '2';
			AtmStatusInfo.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == 10)
		{
			AtmStatusInfo.Cst3Value[0] = '3';
			AtmStatusInfo.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == 20)
		{
			AtmStatusInfo.Cst3Value[0] = '4';
			AtmStatusInfo.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst3Value[%2.2s]", AtmStatusInfo.Cst3Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst3Count[%4.4s]", AtmStatusInfo.Cst3Count);
	}
	if (m_pDevCmn->CSTCnt >= 4)
	{
		if (m_pDevCmn->FthCSTValue == 100)
		{
			AtmStatusInfo.Cst4Value[0] = '1';
			AtmStatusInfo.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
		else if (m_pDevCmn->FthCSTValue == 50)
		{
			AtmStatusInfo.Cst4Value[0] = '2';
			AtmStatusInfo.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
		else if (m_pDevCmn->FthCSTValue == 10)
		{
			AtmStatusInfo.Cst4Value[0] = '3';
			AtmStatusInfo.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
		else if (m_pDevCmn->FthCSTValue == 20)
		{
			AtmStatusInfo.Cst4Value[0] = '4';
			AtmStatusInfo.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst4Value[%2.2s]", AtmStatusInfo.Cst4Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.Cst4Count[%4.4s]", AtmStatusInfo.Cst4Count);
	}

	TempRejectCount1 =	m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1)	-
						m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)			-	
						m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)	-
						m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 >= 0)
		TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	else
		TempRejectCount1 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 < 0)
		TempRejectCount1 = 0;

	TempRejectCount2 =	m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2)	-
						m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)			-	
						m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)	-
						m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 >= 0)
		TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	else
		TempRejectCount2 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 < 0)
		TempRejectCount2 = 0;

	TempRejectCount3 =	m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3)	-
						m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)			-	
						m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3)	-
						m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3);
	if (TempRejectCount3 >= 0)
		TempRejectCount3 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3);
	else
		TempRejectCount3 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3);
	if (TempRejectCount3 < 0)
		TempRejectCount3 = 0;

	TempRejectCount4 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_4)	-
					   m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)			-	
					   m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4)	-
					   m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4);
	if (TempRejectCount4 >= 0)
		TempRejectCount4 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4);
	else
		TempRejectCount4 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4);
	if (TempRejectCount4 < 0)
		TempRejectCount4 = 0;

	TotalRejectCount = TempRejectCount1 + TempRejectCount2 + TempRejectCount3 + TempRejectCount4;
	memcpy(AtmStatusInfo.RejectCount, GetSprintf("%04d", TotalRejectCount), 4);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.RejectCount[%4.4s]", AtmStatusInfo.RejectCount);

	if ((InitInfo.BidAtmColseFlag)	&&							// 2005.09.11
		(!m_pDevCmn->OpenKey))
//		AtmStatusInfo.AtmStatus[0] = 'C';						// Closed : 2005.09.11
		AtmStatusInfo.AtmStatus[0] = 'O';
	else
	if (!m_pDevCmn->OpenKey)									// 2005.09.07
		AtmStatusInfo.AtmStatus[0] = 'M';						// Clerk Mode
	else														// 2006.03.21
	if (m_pDevCmn->DoorStatus == DOOR_OPENED)					// Door Status
//		AtmStatusInfo.AtmStatus[0] = 'C';						// Clerk Mode
		AtmStatusInfo.AtmStatus[0] = 'O';
	else
	{
		if ((m_pDevCmn->AtmStatus == ATM_INIT)		||
			(m_pDevCmn->AtmStatus == ATM_ERROR)		||
			(m_pDevCmn->AtmStatus == ATM_READY)		||
			(m_pDevCmn->AtmStatus == ATM_DOWN))
//			AtmStatusInfo.AtmStatus[0] = 'C';					// Closed
			AtmStatusInfo.AtmStatus[0] = 'O';
		else
			AtmStatusInfo.AtmStatus[0] = 'O';					// Clerk, Custom, Tran : 2006.03.21
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.AtmStatus[%1.1s]", AtmStatusInfo.AtmStatus);

	memcpy(AtmStatusInfo.CardRetainCnt, GetSprintf("%02s", m_pDevCmn->fstrMCU_GetRetractCnt()), 2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.CardRetainCnt[%2.2s]", AtmStatusInfo.CardRetainCnt);
	
    //取版本号  BY HYL
    CString  HostUpdateVersion("");
	if(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTUPDATEVERSION) != "")
	{
		HostUpdateVersion = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTUPDATEVERSION);
		memcpy(AtmStatusInfo.Version, HostUpdateVersion, 4);
	}
	else
	{
		memcpy(AtmStatusInfo.Version, "0001", 4);
	}

	memcpy(AtmStatusInfo.Reverse, "99999", 5);

	/***************************************************/

	fnAPP_FilterAtmStatusInfo();

	/***************************************************/
	int nTempLen = 0;
	if(m_pDevCmn->CSTCnt >=1)
	{
		memcpy(&szTempAtmStatus[nTempLen],"1",1);
		if (m_pDevCmn->FstCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->FstCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);

		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashFstCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);          //存款张数
		if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}
	if(m_pDevCmn->CSTCnt >=2)
	{
		memcpy(&szTempAtmStatus[nTempLen],"2",1);
		if (m_pDevCmn->SndCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->SndCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);

		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashSndCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
		if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}
	if(m_pDevCmn->CSTCnt >=3)
	{
		memcpy(&szTempAtmStatus[nTempLen],"3",1);
		if (m_pDevCmn->FthCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->TrdCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);

		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashTrdCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
		if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}
	if(m_pDevCmn->CSTCnt >=4)
	{
		memcpy(&szTempAtmStatus[nTempLen],"4",1);
		if (m_pDevCmn->FthCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->FthCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);
		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashFthCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
		if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}

	// 废钞箱
	memcpy(&szTempAtmStatus[nTempLen],"8",1);
	memcpy(&szTempAtmStatus[nTempLen+1], "0B", 2);
	memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d",m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT)), 4);
	memcpy(&szTempAtmStatus[nTempLen+7],"0000", 4);
	memcpy(&szTempAtmStatus[nTempLen+11],"0000", 4);
	memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
	if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
		memcpy(&szTempAtmStatus[nTempLen+19],"9",1);
	else
	if (m_pDevCmn->RejectCSTStatus == CST_NORMAL)
		memcpy(&szTempAtmStatus[nTempLen+19],"0",1);
	else
	if (m_pDevCmn->RejectCSTStatus == CST_NEAR)
		memcpy(&szTempAtmStatus[nTempLen+19],"3",1);
	else
	if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)
		memcpy(&szTempAtmStatus[nTempLen+19],"0",1);
	else
	if (m_pDevCmn->RejectCSTStatus == CST_FULL)
		memcpy(&szTempAtmStatus[nTempLen+19],"4",1);
	else
	if (m_pDevCmn->RejectCSTStatus == CST_HIGH)
		memcpy(&szTempAtmStatus[nTempLen+19],"4",1);

	memcpy(&szTempAtmStatus[nTempLen+20],"          ", 10);   //预留
	/***************************************************/
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szTempAtmStatus[%s]", szTempAtmStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HostUpdateVersion[%s]", HostUpdateVersion);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatusInfo.Version[%s]", AtmStatusInfo.Version);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo():Return");
	return T_OK;
}

// Get Atm Status Info
int CTranCmn::fnAPP_GetAtmStatusInfo127()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127()");

	int		TempRejectCount1 = 0, TempRejectCount2 = 0, TempRejectCount3 = 0, TempRejectCount4 =0;
	int		TotalRejectCount = 0;
	int		nRemainAmount = 0;
	char	szTempBuff[1024];									// 2005.09.11
	int		idepmaxretain = 0;

	memset(szTempBuff, 0, sizeof(szTempBuff));
	memset(&AtmStatusInfo127, '0', sizeof(AtmStatusInfo127));

	switch (TranCode)
	{
		case TC_INQUIRY:
			if (TranCodeSave == TC_TRANSFER)
				memcpy(AtmStatusInfo127.ProductCode, "810009", 6);
// 			else
// 			if (TranCodeSave == TC_REMIT)
// 				memcpy(AtmStatusInfo127.ProductCode, "810014", 6);
// 			else
// 			if (TranCodeSave == TC_PAY)
// 				memcpy(AtmStatusInfo127.ProductCode, "820001", 6);
			else
				memcpy(AtmStatusInfo127.ProductCode, "810001", 6);
			break;
		case TC_INQUIRY_CheckPin:
			memcpy(AtmStatusInfo127.ProductCode, "810001", 6);
			break;

		case TC_CUS_INF_INQ:
			if (TranCodeSave == TC_DEPOSIT)
				memcpy(AtmStatusInfo127.ProductCode, "810005", 6);
			else
			if (TranCodeSave == TC_TRANSFER)
				memcpy(AtmStatusInfo127.ProductCode, "810008", 6);
//			else
//			if (TranCodeSave == TC_CASHTRANS)
//			{
//				if (strCashCreditFlag == "CASH")
//					memcpy(AtmStatusInfo127.ProductCode, "810006", 6);
//				else
//				if (strCashCreditFlag == "CREDIT")
//					memcpy(AtmStatusInfo127.ProductCode, "810007", 6);
//			}
			else
				memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_DETAIL:
			memcpy(AtmStatusInfo127.ProductCode, "810002", 6);
			break;

		case TC_OTHER_INQ:
 			if (TranCodeSave == TC_TRANSFER)
 				memcpy(AtmStatusInfo127.ProductCode, "810009", 6);
 			else
				memcpy(AtmStatusInfo127.ProductCode, "810003", 6);
			break;

		case TC_WITHDRAWAL:
			memcpy(AtmStatusInfo127.ProductCode, "810004", 6);
			break;

		case TC_DEPOSIT:
			memcpy(AtmStatusInfo127.ProductCode, "810005", 6);
			break;

// 		case TC_CASHTRANS:
// 			if (strCashCreditFlag == "CASH")
// 				memcpy(AtmStatusInfo127.ProductCode, "810006", 6);
// 			else
// 			if (strCashCreditFlag == "CREDIT")
// 				memcpy(AtmStatusInfo127.ProductCode, "810007", 6);
// 			break;

		case TC_TRANSFER:
			memcpy(AtmStatusInfo127.ProductCode, "810008", 6);
			break;
		case TC_CUS_INF_INQ_OTHERBANK:
			memcpy(AtmStatusInfo127.ProductCode, "810008", 6);
			break;

		case TC_LOAD:
			memcpy(AtmStatusInfo127.ProductCode, "810012", 6);
			break;

// 		case TC_ICPAY:
// 			memcpy(AtmStatusInfo127.ProductCode, "810013", 6);
// 			break;

		case TC_BOUNDMDOIFY:
			memcpy(AtmStatusInfo127.ProductCode, "810011", 6);
			break;
			
		case TC_SCRIPT:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_HZT:
			memcpy(AtmStatusInfo127.ProductCode, "810009", 6);
			break;

		case TC_TZH:
			memcpy(AtmStatusInfo127.ProductCode, "810009", 6);
			break;

		case TC_CHANGEPIN:	
			memcpy(AtmStatusInfo127.ProductCode, "810010", 6);
			break;

// 		case TC_OPEN:
// 			if (RQKCode == 0)
// 				memcpy(AtmStatusInfo127.ProductCode, "810018", 6);
// 			else
// 				memcpy(AtmStatusInfo127.ProductCode, "810018", 6);
// 			break;

		case TC_OEXSTATUS:
		case TC_ERROR:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);				
			break;

		case TC_MSRSTATUS:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_AEXSTATUS:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_CARDRETAIN:
			memcpy(AtmStatusInfo127.ProductCode, "810017", 6);
			break;

		case TC_DOWNFIT:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_DOWNICFIT:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_DOWNAIDLIST:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;
				
		case TC_DOWNPARM:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_DOWNPBOCPARA:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;
				
		case TC_DOWNPAY:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;
				
		case TC_ADDCASH:
			memcpy(AtmStatusInfo127.ProductCode, "830004", 6);
			break;

		case TC_SUBCASH:
		case TC_CLEARCASH:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_TOTAL:
			memcpy(AtmStatusInfo127.ProductCode, "830001", 6);
			break;

		case TC_TOTALPRE:
			memcpy(AtmStatusInfo127.ProductCode, "830002", 6);
			break;

		case TC_TOTALKIND:
			memcpy(AtmStatusInfo127.ProductCode, "830003", 6);
			break;

		case TC_DETAILEND:
		case TC_SENDDETAIL:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_REMIT:									// 2008.10.05
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_REMIT_ADD:
			memcpy(AtmStatusInfo127.ProductCode, "810014", 6);
			break;
		case TC_REMIT_PWD:
			memcpy(AtmStatusInfo127.ProductCode, "810016", 6);
			break;

		case TC_REMIT_INF_INQ:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_REMIT_IN:
			memcpy(AtmStatusInfo127.ProductCode, "810015", 6);
			break;

		case TC_PAY_INQ:
			if( PayType == S_PAY1 )
			{
				memcpy(AtmStatusInfo127.ProductCode, "820001", 6);
			}
			else
			if( PayType == S_PAY2 )
			{
                memcpy(AtmStatusInfo127.ProductCode, "820002", 6);
			}
			break;

		case TC_PAY:
            if( PayType == S_PAY1 )
			{
				memcpy(AtmStatusInfo127.ProductCode, "820001", 6);
			}
			else
			if( PayType == S_PAY2 )
			{
                memcpy(AtmStatusInfo127.ProductCode, "820002", 6);
			}
			break;

		case TC_ATMOPEN:
		case TC_ATMCLOSE:
		case TC_ATMREBOOT:
		case TC_ATMSTATUS:
		case TC_ATMRESETALL:
		case TC_ATMBIDSENDDETAIL:
		case TC_ATMBIDADDOWNLOAD:
	    case TC_ATMBIDSOFTUPDATE:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;
			
		case TC_ATMWORKPARAM:									// CUP(SH) : 2006.04.19
		case TC_ATMFTPSEND:
		case TC_ATMRESETDVR:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_CONFIRMWITH:	
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		case TC_ECHANNEL:
		case TC_ECHANNEL_INQ:
		case TC_SMSPARITY:
		case TC_DOWNECHANNELPARA:
		case TC_DOWNECHANNELPROT:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;

		default:
			memcpy(AtmStatusInfo127.ProductCode, "000000", 6);
			break;
	}

	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))				// Cdu Status
		AtmStatusInfo127.CduDepStatus[0] = '8';
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
		AtmStatusInfo127.CduDepStatus[0] = '8';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.CduDepStatus[%1.1s]", AtmStatusInfo127.CduDepStatus);
		
	//增加取款锁定 added by liuxl 20110609
	int icwdmaxshutter = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM, 1);
	if ( icwdmaxshutter > 0 &&
		IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, 0) == icwdmaxshutter)
	{
		AtmStatusInfo127.CduDepStatus[0] = '8';
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.CduDepStatus(Change)[%1.1s]", AtmStatusInfo127.CduDepStatus);
	}

// 	AtmStatusInfo.DepStatus[0] = '9';
// 	if ((m_pDevCmn->AtmDefine.CashHandler == BRM_SET)				||
// 		(m_pDevCmn->AtmDefine.CashHandler == BRM_OKIBRMNHSP_SET)	||
// 		(m_pDevCmn->AtmDefine.CashHandler == BRM_NHBRMNHSP_SET))
// 	{
// 		AtmStatusInfo.DepStatus[0] = AtmStatusInfo.CduStatus[0];
// 		
// 		//锁定存取款 2008.05.15
// 		idepmaxretain = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DEPMAXRETAINNUM, 1);
// 		if (idepmaxretain == 1)
// 		{
// 			idepmaxretain = 2;
// 		}
// 		if ( idepmaxretain > 0 &&
// 			IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DEPCURRETAINNUM, 0) >= idepmaxretain )
// 		{
// 			AtmStatusInfo.DepStatus[0] = '8';
// 		}
// 		
// 		
// 	}
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo() AtmStatusInfo.DepStatus[%1.1s]", AtmStatusInfo.DepStatus);
			
		
	AtmStatusInfo127.ICStatus[0] = '9';							// IC Status
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.ICStatus[%1.1s]", AtmStatusInfo127.ICStatus);

	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))				// Mcu Status
		AtmStatusInfo127.McuStatus[0] = '8';					
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))						
		AtmStatusInfo127.McuStatus[0] = '8';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.McuStatus[%1.1s]", AtmStatusInfo127.McuStatus);

	AtmStatusInfo127.McuExitStatus[0] = '9';						// Mcu Exit Status
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.McuExitStatus[%1.1s]", AtmStatusInfo127.McuExitStatus);

	if (m_pDevCmn->DoorStatus == DOOR_OPENED)					// Door Status
		AtmStatusInfo127.DoorStatus[0] = '9';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.DoorStatus[%1.1s]", AtmStatusInfo127.DoorStatus);

	AtmStatusInfo.DESStatus[0] = '0';
	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_PIN))				// Pin Status
	{
		AtmStatusInfo127.PinpadStatus[0] = '8';
	}
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))
	{
		AtmStatusInfo127.PinpadStatus[0] = '8';
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.PinpadStatus[%1.1s]", AtmStatusInfo127.PinpadStatus);

	AtmStatusInfo127.TicketStatus[0] = '9';
	AtmStatusInfo127.VoiceStatus[0] = '9';
	AtmStatusInfo127.PassbookStatus[0] = '9';
	AtmStatusInfo127.DevelopStatus[0]= '9';
	AtmStatusInfo127.DVRStatus[0] = '9';
	AtmStatusInfo127.Cam1Status[0] = '9';
	AtmStatusInfo127.Cam2Status[0] = '9';
	AtmStatusInfo127.Cam3Status[0] = '9';
	AtmStatusInfo127.Cam4Status[0] = '9';
	AtmStatusInfo127.Cam5Status[0] = '9';
	AtmStatusInfo127.Cam6Status[0] = '9';
	//增加DVR与摄像头状态上送状态 BY HYL
	if(InitInfo.TranDvrMode == 1)
	{
		if(memcmp(InitInfo.DvrStatus,"0004",4) == 0  ||
			memcmp(InitInfo.DvrStatus,"0014",4) == 0 ||
			memcmp(InitInfo.DvrStatus,"0024",4) == 0 ||
			memcmp(InitInfo.DvrStatus,"0044",4) == 0 ||
			memcmp(InitInfo.DvrStatus,"0084",4) == 0)
			AtmStatusInfo127.DVRStatus[0] = '8';
		else
			AtmStatusInfo127.DVRStatus[0] = '0';

		if(InitInfo.DvrChanStatus[0] == '0'||InitInfo.DvrChanStatus[0] == '1')
			AtmStatusInfo127.Cam1Status[0] = '0';
		if(InitInfo.DvrChanStatus[1] == '0'||InitInfo.DvrChanStatus[1] == '1')
			AtmStatusInfo127.Cam2Status[0] = '0';
		if(InitInfo.DvrChanStatus[2] == '0'||InitInfo.DvrChanStatus[2] == '1')
			AtmStatusInfo127.Cam3Status[0] = '0';
		if(InitInfo.DvrChanStatus[3] == '0'||InitInfo.DvrChanStatus[3] == '1')
			AtmStatusInfo127.Cam3Status[0] = '0';
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.DVRStatus[%1.1s]", AtmStatusInfo127.DVRStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cam1Status[%1.1s]", AtmStatusInfo127.Cam1Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cam2Status[%1.1s]", AtmStatusInfo127.Cam2Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cam3Status[%1.1s]", AtmStatusInfo127.Cam3Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cam4Status[%1.1s]", AtmStatusInfo127.Cam4Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cam5Status[%1.1s]", AtmStatusInfo127.Cam5Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cam6Status[%1.1s]", AtmStatusInfo127.Cam6Status);


	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))				// Jpr Status
		AtmStatusInfo127.JprStatus[0] = '9';
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
		AtmStatusInfo127.JprStatus[0] = '8';
	else
	if (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)
		AtmStatusInfo127.JprStatus[0] = '2';
	else
	if (m_pDevCmn->JnlHeadStatus != HEADER_NORMAL)
		AtmStatusInfo127.JprStatus[0] = '8';
	else
	if (m_pDevCmn->JnlStatus == JNL_NORMAL)
		AtmStatusInfo127.JprStatus[0] = '0';
	else
	if (m_pDevCmn->JnlStatus == JNL_LOW_END)
		AtmStatusInfo127.JprStatus[0] = '1';
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.JprStatus[%1.1s]", AtmStatusInfo127.JprStatus);

	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))				// Spr Status
		AtmStatusInfo127.SprStatus[0] = '9';
	else
	if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))
		AtmStatusInfo127.SprStatus[0] = '8';
	else
	if (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)
		AtmStatusInfo127.SprStatus[0] = '2';
	else
	if (m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)
		AtmStatusInfo127.SprStatus[0] = '8';
	else
	if (m_pDevCmn->SlipStatus == SLIP_NORMAL)
		AtmStatusInfo127.SprStatus[0] = '0';
	else
	if (m_pDevCmn->SlipStatus == SLIP_LOW_END)
		AtmStatusInfo127.SprStatus[0] = '1';

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.SprStatus[%1.1s]", AtmStatusInfo127.SprStatus);
	AtmStatusInfo127.DvrHDStatus[0] = '9';
	if(InitInfo.TranDvrMode == 1)
	{
       AtmStatusInfo127.DvrHDStatus[0] = '0';
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.DvrHDStatus[%1.1s]", AtmStatusInfo127.DvrHDStatus);


	AtmStatusInfo127.Deno1Status[0] = '9';						// Missing
	AtmStatusInfo127.Deno2Status[0] = '9';						// Missing
	AtmStatusInfo127.Deno3Status[0] = '9';						// Missing
	AtmStatusInfo127.Deno4Status[0] = '9';						// Missing
	AtmStatusInfo127.Deno5Status[0] = '9';						// Missing
	AtmStatusInfo127.Deno6Status[0] = '9';						// Missing
	
	if (m_pDevCmn->CSTCnt >= 1)
	{
		if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
			AtmStatusInfo127.Deno1Status[0] = '9';				// Missing
		else
		if (m_pDevCmn->FstCSTStatus == CST_ERROR)
			AtmStatusInfo127.Deno1Status[0] = '8';				// Error
		else
			AtmStatusInfo127.Deno1Status[0] = '0';				// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Deno1Status[%1.1s]", AtmStatusInfo127.Deno1Status);
	}
	if (m_pDevCmn->CSTCnt >= 2)
	{
		if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
			AtmStatusInfo127.Deno2Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->SndCSTStatus == CST_ERROR)
			AtmStatusInfo127.Deno2Status[0] = '8';					// Error
		else
			AtmStatusInfo127.Deno2Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Deno2Status[%1.1s]", AtmStatusInfo127.Deno2Status);
	}
	if (m_pDevCmn->CSTCnt >= 3)
	{
		if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
			AtmStatusInfo127.Deno3Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->TrdCSTStatus == CST_ERROR)
			AtmStatusInfo127.Deno3Status[0] = '8';					// Error
		else
			AtmStatusInfo127.Deno3Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Deno3Status[%1.1s]", AtmStatusInfo127.Deno3Status);
	}
	if (m_pDevCmn->CSTCnt >= 4)
	{
		if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
			AtmStatusInfo127.Deno4Status[0] = '9';					// Missing
		else
		if (m_pDevCmn->FthCSTStatus == CST_ERROR)
			AtmStatusInfo127.Deno4Status[0] = '8';					// Error
		else
			AtmStatusInfo127.Deno4Status[0] = '0';					// Normal

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Deno4Status[%1.1s]", AtmStatusInfo127.Deno4Status);
	}	

	AtmStatusInfo127.RejectStatus[0] = '0';
	if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
		AtmStatusInfo.RejectStatus[0] = '9';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_ERROR)
		AtmStatusInfo127.RejectStatus[0] = '0';
	else
		AtmStatusInfo127.RejectStatus[0] = '0';

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.RejectStatus[%1.1s]", AtmStatusInfo127.RejectStatus);


/////////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->CSTCnt >= 1)
	{
		if (m_pDevCmn->FstCSTValue == CASH_CHINA_100)
		{
			AtmStatusInfo127.Cst1Value[0] = '1';
			AtmStatusInfo127.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
		else if (m_pDevCmn->FstCSTValue == CASH_CHINA_50)
		{
			AtmStatusInfo127.Cst1Value[0] = '2';
			AtmStatusInfo127.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
		else if (m_pDevCmn->FstCSTValue == CASH_CHINA_10)
		{
			AtmStatusInfo127.Cst1Value[0] = '3';
			AtmStatusInfo127.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
		else if (m_pDevCmn->FstCSTValue == CASH_CHINA_20)
		{
			AtmStatusInfo127.Cst1Value[0] = '4';
			AtmStatusInfo127.Cst1Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst1Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst1Value[%2.2s]", AtmStatusInfo127.Cst1Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst1Count[%4.4s]", AtmStatusInfo127.Cst1Count);
	}
	if (m_pDevCmn->CSTCnt >= 2)
	{
		if (m_pDevCmn->SndCSTValue == CASH_CHINA_100)
		{
			AtmStatusInfo127.Cst2Value[0] = '1';
			AtmStatusInfo127.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
		else if (m_pDevCmn->SndCSTValue == CASH_CHINA_50)
		{
			AtmStatusInfo127.Cst2Value[0] = '2';
			AtmStatusInfo127.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
		else if (m_pDevCmn->SndCSTValue == CASH_CHINA_10)
		{
			AtmStatusInfo127.Cst2Value[0] = '3';
			AtmStatusInfo127.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
		else if (m_pDevCmn->SndCSTValue == CASH_CHINA_20)
		{
			AtmStatusInfo127.Cst2Value[0] = '4';
			AtmStatusInfo127.Cst2Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst2Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst2Value[%2.2s]", AtmStatusInfo127.Cst2Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst2Count[%4.4s]", AtmStatusInfo127.Cst2Count);
	}
	if (m_pDevCmn->CSTCnt >= 3)
	{
		if (m_pDevCmn->TrdCSTValue == CASH_CHINA_100)
		{
			AtmStatusInfo127.Cst3Value[0] = '1';
			AtmStatusInfo127.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == CASH_CHINA_50)
		{
			AtmStatusInfo127.Cst3Value[0] = '2';
			AtmStatusInfo127.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == CASH_CHINA_10)
		{
			AtmStatusInfo127.Cst3Value[0] = '3';
			AtmStatusInfo127.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == CASH_CHINA_20)
		{
			AtmStatusInfo127.Cst3Value[0] = '4';
			AtmStatusInfo127.Cst3Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst3Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst3Value[%2.2s]", AtmStatusInfo127.Cst3Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst3Count[%4.4s]", AtmStatusInfo127.Cst3Count);
	}
	if (m_pDevCmn->CSTCnt >= 4)
	{
		if (m_pDevCmn->FthCSTValue == CASH_CHINA_100)
		{
			AtmStatusInfo127.Cst4Value[0] = '1';
			AtmStatusInfo127.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
		else if (m_pDevCmn->FthCSTValue == CASH_CHINA_50)
		{
			AtmStatusInfo127.Cst4Value[0] = '2';
			AtmStatusInfo127.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
		else if (m_pDevCmn->FthCSTValue == CASH_CHINA_10)
		{
			AtmStatusInfo127.Cst4Value[0] = '3';
			AtmStatusInfo127.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
		else if (m_pDevCmn->FthCSTValue == CASH_CHINA_20)
		{
			AtmStatusInfo127.Cst4Value[0] = '4';
			AtmStatusInfo127.Cst4Value[1] = '1';
			memcpy(AtmStatusInfo127.Cst4Count, GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst4Value[%2.2s]", AtmStatusInfo127.Cst4Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst4Count[%4.4s]", AtmStatusInfo127.Cst4Count);
	}

/////////////////////////////////////////////////////////////////////////////////////////////

	TempRejectCount1 =	m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1)	-
						m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)			-	
						m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)	-
						m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 >= 0)
		TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	else
		TempRejectCount1 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 < 0)
		TempRejectCount1 = 0;

	TempRejectCount2 =	m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2)	-
						m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)			-	
						m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)	-
						m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 >= 0)
		TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	else
		TempRejectCount2 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 < 0)
		TempRejectCount2 = 0;

	TempRejectCount3 =	m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3)	-
						m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)			-	
						m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3)	-
						m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3);
	if (TempRejectCount3 >= 0)
		TempRejectCount3 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3);
	else
		TempRejectCount3 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3);
	if (TempRejectCount3 < 0)
		TempRejectCount3 = 0;

	TempRejectCount4 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_4)	-
					   m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)			-	
					   m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4)	-
					   m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4);
	if (TempRejectCount4 >= 0)
		TempRejectCount4 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4);
	else
		TempRejectCount4 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4);
	if (TempRejectCount4 < 0)
		TempRejectCount4 = 0;

	TotalRejectCount = TempRejectCount1 + TempRejectCount2 + TempRejectCount3 + TempRejectCount4;
	memcpy(AtmStatusInfo127.RejectCount, GetSprintf("%04d", TotalRejectCount), 4);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.RejectCount[%4.4s]", AtmStatusInfo127.RejectCount);

	if ((InitInfo.BidAtmColseFlag)	&&							// 2005.09.11
		(!m_pDevCmn->OpenKey))
//		AtmStatusInfo127.AtmStatus[0] = 'C';					// Closed : 2005.09.11
		AtmStatusInfo127.AtmStatus[0] = 'O';
	else
	if (!m_pDevCmn->OpenKey)									// 2005.09.07
		AtmStatusInfo127.AtmStatus[0] = 'M';					// Clerk Mode
	else														// 2006.03.21
	if (m_pDevCmn->DoorStatus == DOOR_OPENED)					// Door Status
//		AtmStatusInfo127.AtmStatus[0] = 'C';					// Clerk Mode
		AtmStatusInfo127.AtmStatus[0] = 'O';
	else
	{
		if ((m_pDevCmn->AtmStatus == ATM_INIT)		||
			(m_pDevCmn->AtmStatus == ATM_ERROR)		||
			(m_pDevCmn->AtmStatus == ATM_READY)		||
			(m_pDevCmn->AtmStatus == ATM_DOWN))
//			AtmStatusInfo127.AtmStatus[0] = 'C';				// Closed
			AtmStatusInfo127.AtmStatus[0] = 'O';
		else
			AtmStatusInfo127.AtmStatus[0] = 'O';				// Clerk, Custom, Tran : 2006.03.21
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.AtmStatus[%1.1s]", AtmStatusInfo127.AtmStatus);

	memcpy(AtmStatusInfo127.CardRetainCnt, GetSprintf("%02s", m_pDevCmn->fstrMCU_GetRetractCnt()), 2);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.CardRetainCnt[%2.2s]", AtmStatusInfo127.CardRetainCnt);
	
    //取版本号  BY HYL
    CString  HostUpdateVersion("");
	if(IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTUPDATEVERSION) != "")
	{
		HostUpdateVersion = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTUPDATEVERSION);
		memcpy(AtmStatusInfo127.Version, HostUpdateVersion, 4);
	}
	else
	{
		 memcpy(AtmStatusInfo127.Version, "0001", 4);
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////
	// 整机取款状态
	AtmStatusInfo127.WithdrawStatus[0] = '9';
	
	char WithBoxTemp1 = '9';
	char WithBoxTemp2 = '9';
	char WithBoxTemp3 = '9';
	char WithBoxTemp4 = '9';
	
	if (m_pDevCmn->CSTCnt >= 1)
	{
		if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
			WithBoxTemp1 = '9';									// Missing
		else
		if (m_pDevCmn->FstCSTStatus == CST_NEAR)
			WithBoxTemp1 = '1';									// NEAR
		else
		if (m_pDevCmn->FstCSTStatus == CST_EMPTY)				
			WithBoxTemp1 = '2';									// EMPTY
		else
			WithBoxTemp1 = '0';									// Normal
	}
	if (m_pDevCmn->CSTCnt >= 2)
	{
		if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
			WithBoxTemp2 = '9';									// Missing
		else
		if (m_pDevCmn->SndCSTStatus == CST_NEAR)
			WithBoxTemp2 = '1';									// NEAR
		else
		if (m_pDevCmn->SndCSTStatus == CST_EMPTY)				
			WithBoxTemp2 = '2';									// EMPTY
		else
			WithBoxTemp2 = '0';									// Normal
	}
	if (m_pDevCmn->CSTCnt >= 3)
	{
		if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
			WithBoxTemp3 = '9';									// Missing
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
			WithBoxTemp3 = '1';									// NEAR
		else
		if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)				
			WithBoxTemp3 = '2';									// EMPTY
		else
			WithBoxTemp3 = '0';									// Normal
	}
	if (m_pDevCmn->CSTCnt >= 4)
	{
		if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
			WithBoxTemp4 = '9';									// Missing
		else
		if (m_pDevCmn->FthCSTStatus == CST_NEAR)
			WithBoxTemp4 = '1';									// NEAR
		else
		if (m_pDevCmn->FthCSTStatus == CST_EMPTY)				
			WithBoxTemp4 = '2';									// EMPTY
		else
			WithBoxTemp4 = '0';									// Normal
	}
	
	if (WithBoxTemp1 == 2 &&
		WithBoxTemp2 == 2 && 
		WithBoxTemp3 == 2 &&
		WithBoxTemp4 == 2)
	{
		AtmStatusInfo127.WithdrawStatus[0] = '2';
	}
	else
	if ((WithBoxTemp1 == 1 || WithBoxTemp1 == 2) &&
		(WithBoxTemp2 == 1 || WithBoxTemp2 == 2) && 
		(WithBoxTemp3 == 1 || WithBoxTemp3 == 2) &&
		(WithBoxTemp4 == 1 || WithBoxTemp4 == 2) )
	{
		AtmStatusInfo127.WithdrawStatus[0] = '1';
	}
	else
		AtmStatusInfo127.WithdrawStatus[0] = '0';
	
	if (!(WithAvail & TRAN_WITH_BOTH))
	{
		AtmStatusInfo127.WithdrawStatus[0] = '9';				// 取款不可用
	}
	
	if (AtmStatusInfo127.CduDepStatus[0] == '8')
	{
		AtmStatusInfo127.WithdrawStatus[0] = '9';
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.WithdrawStatus[%1.1s]", AtmStatusInfo127.WithdrawStatus);

/////////////////////////////////////////////////////////////////////////////////////////////////////

	// 废钞箱业务状态

	AtmStatusInfo127.RejectOperationStatus[0] = '0';
	
	if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
		AtmStatusInfo127.RejectOperationStatus[0] = '0';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_HIGH)
		AtmStatusInfo127.RejectOperationStatus[0] = '4';
	else
	if (m_pDevCmn->RejectCSTStatus == CST_FULL)					// 2008.12.04
		AtmStatusInfo127.RejectOperationStatus[0] = '3';
	else
		AtmStatusInfo127.RejectOperationStatus[0] = '0';

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.RejectOperationStatus[%1.1s]", AtmStatusInfo127.RejectOperationStatus);
	

	// 总行广告版本号
	memcpy(AtmStatusInfo127.HeadAdVersion, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_HEADADVERSION, "000001"), 6);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.HeadAdVersion[%6.6s]", AtmStatusInfo127.HeadAdVersion);
	
	// 分行广告版本号
	memcpy(AtmStatusInfo127.BranchAdVersion, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_BRANCHADVERSION, "110001"), 6);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.BranchAdVersion[%6.6s]", AtmStatusInfo127.BranchAdVersion);
	
	// 资费标准公告版本号
	memcpy(AtmStatusInfo127.PoundageVersion, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_POUNDAGEVERSION, "110001"), 6);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.PoundageVersion[%6.6s]", AtmStatusInfo127.PoundageVersion);

	memcpy(AtmStatusInfo127.Reverse, "99999", 5);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // 一体机增加 存款箱状态 存款箱张数 存款箱面额 整机存款状态
	// 整机存款状态
	AtmStatusInfo127.DepositStatus[0] = '9';

	if (m_pDevCmn->AtmDefine.CashHandler == BRM_SET)
	{
		int Dep100Pos = 0;
		int Dep50Pos = 0;
		
		char Dep100Status = '9';
		char Dep50Status = '9';
		char DepMixStatus = '9';
		
		int Dep100Count = 0;
		int Dep50Count = 0;
		int DepMixCount = 0;
		
		CString		strReturn("");
		CStringArray strReturnArray;

		if (m_pDevCmn->CshD100DepositLoc == NORMAL_POS)
		{
			if (AtmStatusInfo127.Deno1Status[0] == '9')
				Dep100Pos = 1;
			else if (AtmStatusInfo127.Deno2Status[0] == '9')
				Dep100Pos = 2;
			else if (AtmStatusInfo127.Deno3Status[0] == '9')
				Dep100Pos = 3;
			else if (AtmStatusInfo127.Deno4Status[0] == '9')
				Dep100Pos = 4;	
			
			if (m_pDevCmn->CshD100DepositStatus == CST_ERROR)
				Dep100Status = '8';
			else
				Dep100Status = '0';
			
			strReturn = m_pDevCmn->fstrCSH_GetNumberOfDeposit(CASH_TYPE_DEPOSIT100);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				Dep100Count = Asc2Int(strReturnArray[0]);
			
		}
		
		if (m_pDevCmn->CshD50DepositLoc == NORMAL_POS)
		{
			if (AtmStatusInfo127.Deno1Status[0] == '9')
				Dep50Pos = 1;
			else if (AtmStatusInfo127.Deno2Status[0] == '9')
				Dep50Pos = 2;
			else if (AtmStatusInfo127.Deno3Status[0] == '9')
				Dep50Pos = 3;
			else if (AtmStatusInfo127.Deno4Status[0] == '9')
				Dep50Pos = 4;	
			
			if (m_pDevCmn->CshD50DepositStatus == CST_ERROR)
				Dep50Status = '8';
			else
				Dep50Status = '0';
			
			strReturn = m_pDevCmn->fstrCSH_GetNumberOfDeposit(CASH_TYPE_DEPOSIT50);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				Dep50Count = Asc2Int(strReturnArray[0]);
		}
		
		if (m_pDevCmn->DepositCSTLoc == NORMAL_POS)
		{
			if (m_pDevCmn->DepositCSTStatus == CST_ERROR)
				DepMixStatus = '8';
			else
				DepMixStatus = '0';
			
			strReturn = m_pDevCmn->fstrCSH_GetNumberOfDeposit(CASH_TYPE_DEPOSIT);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				DepMixCount = Asc2Int(strReturnArray[0]);
		}

//////////////////////////////////////////////////////////////////////////////////////////////

		if (Dep100Pos != 0)
		{
			if (Dep100Pos == 1)
				AtmStatusInfo127.Deno1Status[0] = Dep100Status;	
			else if (Dep100Pos == 2)
				AtmStatusInfo127.Deno2Status[0] = Dep100Status;	
			else if (Dep100Pos == 3)
				AtmStatusInfo127.Deno3Status[0] = Dep100Status;	
			else if (Dep100Pos == 4)
				AtmStatusInfo127.Deno4Status[0] = Dep100Status;	
		}
		
		if (Dep50Pos != 0)
		{
			if (Dep50Pos == 1)
				AtmStatusInfo127.Deno1Status[0] = Dep50Status;	
			else if (Dep50Pos == 2)
				AtmStatusInfo127.Deno2Status[0] = Dep50Status;	
			else if (Dep50Pos == 3)
				AtmStatusInfo127.Deno3Status[0] = Dep50Status;	
			else if (Dep50Pos == 4)
				AtmStatusInfo127.Deno4Status[0] = Dep50Status;	
		}

		AtmStatusInfo127.Deno5Status[0] = DepMixStatus;		


///////////////////////////////////////////////////////////////////////////////////////////////////////

		if (Dep100Pos != 0)
		{
			if (Dep100Pos == 1)
			{
				AtmStatusInfo127.Cst1Value[0] = '1';
				AtmStatusInfo127.Cst1Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst1Count, GetSprintf("%04d", Dep100Count), 4);
			}
			else if (Dep100Pos == 2)
			{
				AtmStatusInfo127.Cst2Value[0] = '1';
				AtmStatusInfo127.Cst2Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst2Count, GetSprintf("%04d", Dep100Count), 4);
			}
			else if (Dep100Pos == 3)
			{
				AtmStatusInfo127.Cst3Value[0] = '1';
				AtmStatusInfo127.Cst4Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst3Count, GetSprintf("%04d", Dep100Count), 4);
			}
			else if (Dep100Pos == 4)
			{
				AtmStatusInfo127.Cst4Value[0] = '1';
				AtmStatusInfo127.Cst4Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst4Count, GetSprintf("%04d", Dep100Count), 4);
			}
		}
		
		if (Dep50Pos != 0)
		{
			if (Dep50Pos == 1)
			{
				AtmStatusInfo127.Cst1Value[0] = '2';
				AtmStatusInfo127.Cst1Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst1Count, GetSprintf("%04d", Dep50Count), 4);
			}
			else if (Dep50Pos == 2)
			{
				AtmStatusInfo127.Cst2Value[0] = '2';
				AtmStatusInfo127.Cst2Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst2Count, GetSprintf("%04d", Dep50Count), 4);
			}
			else if (Dep50Pos == 3)
			{
				AtmStatusInfo127.Cst3Value[0] = '2';
				AtmStatusInfo127.Cst3Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst3Count, GetSprintf("%04d", Dep50Count), 4);
			}
			else if (Dep50Pos == 4)
			{
				AtmStatusInfo127.Cst4Value[0] = '2';
				AtmStatusInfo127.Cst4Value[1] = '1';
				memcpy(AtmStatusInfo127.Cst4Count, GetSprintf("%04d", Dep50Count), 4);
			}
		}
		
		AtmStatusInfo127.Cst5Value[0] = '5';
		AtmStatusInfo127.Cst5Value[1] = '1';
		memcpy(AtmStatusInfo127.Cst5Count, GetSprintf("%04d", DepMixCount), 4);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Deno5Status[%1.1s]", AtmStatusInfo127.Deno5Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst5Value[%2.2s]", AtmStatusInfo127.Cst5Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.Cst5Count[%4.4s]", AtmStatusInfo127.Cst5Count);


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() Dep100Pos[%d] Dep100Status[%c] Dep100Count[%d]", 
		Dep100Pos,Dep100Status,Dep100Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() Dep50Pos[%d] Dep50Status[%c] Dep50Count[%d]", 
		Dep50Pos,Dep50Status,Dep50Count);


//////////////////////////////////////////////////////////////////////////////////////////

		char DepBoxTemp = '9';
		char Dep100BoxTemp = '9';
		char Dep50BoxTemp = '9';

		if (m_pDevCmn->DepositCSTLoc != NORMAL_POS)
			DepBoxTemp = '9';
		else
		if (m_pDevCmn->DepositCSTStatus == CST_HIGH)
			DepBoxTemp = '4';
		else
		if (m_pDevCmn->DepositCSTStatus == CST_FULL)
		{
			DepBoxTemp = '3';
		}
		else
			DepBoxTemp = '0';

		if (m_pDevCmn->CshD100DepositLoc != NORMAL_POS)
			Dep100BoxTemp = '9';
		else
		if (m_pDevCmn->CshD100DepositStatus == CST_HIGH)
			Dep100BoxTemp = '4';
		else
		if (m_pDevCmn->CshD100DepositStatus == CST_FULL)
		{
			Dep100BoxTemp = '3';
		}
		else
			Dep100BoxTemp = '0';


		if (m_pDevCmn->CshD50DepositLoc != NORMAL_POS)
			Dep50BoxTemp = '9';
		else
		if (m_pDevCmn->CshD50DepositStatus == CST_HIGH)
			Dep50BoxTemp = '4';
		else
		if (m_pDevCmn->CshD50DepositStatus == CST_FULL)
		{
			Dep50BoxTemp = '3';
		}
		else
			Dep50BoxTemp = '0';

		if (DepBoxTemp == '3' &&
			Dep100BoxTemp == '3' &&
			Dep50BoxTemp == '3')
		{
			AtmStatusInfo127.DepositStatus[0] = '3';
		}
		else
		if ((DepBoxTemp == '3' || DepBoxTemp == '4')&&
			(Dep100BoxTemp == '3' || Dep100BoxTemp == '4')&&
			(Dep50BoxTemp == '3' || Dep50BoxTemp == '4'))
		{
			AtmStatusInfo127.DepositStatus[0] = '4';
		}
		else
			AtmStatusInfo127.DepositStatus[0] = '0';


		//锁定存取款 2008.05.15
		idepmaxretain = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _T("DEPMAXRETAINNUM"), 1);
		if (idepmaxretain == 1)
		{
			idepmaxretain = 2;
		}
		if ( idepmaxretain > 0 &&
			IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _T("DEPMAXRETAINNUM"), 0) >= idepmaxretain )
		{
			AtmStatusInfo127.DepositStatus[0] = '7';					// 回收超限
		}
		
		if (!(DepAvail & TRAN_DEP_CASH))							// 存款不可用
		{
			AtmStatusInfo127.DepositStatus[0] = '9';
		}

		if (AtmStatusInfo127.CduDepStatus[0] == '8')
		{
			AtmStatusInfo127.DepositStatus[0] = '9';
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo127() AtmStatusInfo127.DepositStatus[%1.1s]", AtmStatusInfo127.DepositStatus);

	/***************************************************/

	fnAPP_FilterAtmStatusInfo127();

	/***************************************************/
	int nTempLen = 0;
	if(m_pDevCmn->CSTCnt >=1)
	{
		memcpy(&szTempAtmStatus[nTempLen],"1",1);
		if (m_pDevCmn->FstCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->FstCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);

		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashFstCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);          //存款张数
		if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->FstCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}
	if(m_pDevCmn->CSTCnt >=2)
	{
		memcpy(&szTempAtmStatus[nTempLen],"2",1);
		if (m_pDevCmn->SndCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->SndCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);

		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashSndCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
		if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->SndCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}
	if(m_pDevCmn->CSTCnt >=3)
	{
		memcpy(&szTempAtmStatus[nTempLen],"3",1);
		if (m_pDevCmn->FthCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->TrdCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);

		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashTrdCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
		if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->TrdCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}
	if(m_pDevCmn->CSTCnt >=4)
	{
		memcpy(&szTempAtmStatus[nTempLen],"4",1);
		if (m_pDevCmn->FthCSTValue == 100)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "02", 2);
		}
		else
		if (m_pDevCmn->FthCSTValue == 50)
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "01", 2);
		}
		else
		{
			memcpy(&szTempAtmStatus[nTempLen+1], "00", 2);
		}
		memcpy(&szTempAtmStatus[nTempLen+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
		memcpy(&szTempAtmStatus[nTempLen+7],GetSprintf("%04d", CashFthCSTDispRejCnt), 4);
		memcpy(&szTempAtmStatus[nTempLen+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4)), 4);
        memcpy(&szTempAtmStatus[nTempLen+15],"0000", 4);
		if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
			memcpy(&szTempAtmStatus[nTempLen+19], "9",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_NORMAL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_NEAR)
			memcpy(&szTempAtmStatus[nTempLen+19], "1",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_EMPTY)
			memcpy(&szTempAtmStatus[nTempLen+19], "2",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_FULL)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_ERROR)
			memcpy(&szTempAtmStatus[nTempLen+19], "8",1);
		else
		if (m_pDevCmn->FthCSTStatus == CST_HIGH)
			memcpy(&szTempAtmStatus[nTempLen+19], "0",1);
		nTempLen += 20;
	}

	/***************************************************/
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szTempAtmStatus[%s]", szTempAtmStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HostUpdateVersion[%s]", HostUpdateVersion);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStatusInfo.Version[%4.4s]", AtmStatusInfo.Version);	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmStatusInfo():Return");
	return T_OK;
}

// Filter Atm Status Info
int CTranCmn::fnAPP_FilterAtmStatusInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo()");
	// 钞箱故障，存款模块正常，取款模块正常
	if (AtmStatusInfo.Deno1Status[0] == '8' ||
		AtmStatusInfo.Deno2Status[0] == '8' ||
		AtmStatusInfo.Deno3Status[0] == '8' ||
		AtmStatusInfo.Deno4Status[0] == '8' ||
		AtmStatusInfo.Deno5Status[0] == '8' ||
		AtmStatusInfo.Deno6Status[0] == '8' ||
		AtmStatusInfo.DepBoxStatus[0] == '3'||
		AtmStatusInfo.DepBoxStatus[0] == '8')
	{
		AtmStatusInfo.CduStatus[0] = '0';
		AtmStatusInfo.DepStatus[0] = '0';
	}
	
	// 废钞箱满，存款模块正常，取款模块正常
	// 废钞箱故障，存款模块正常，取款模块正常
	if (AtmStatusInfo.RejectStatus[0] == '3' ||
		AtmStatusInfo.RejectStatus[0] == '8')
	{
		AtmStatusInfo.CduStatus[0] = '0';
		AtmStatusInfo.DepStatus[0] = '0';
	}
	
	// 存取款模块故障，存款模块故障，取款模块正常
	if (AtmStatusInfo.DepStatus[0] == '8')
	{
		AtmStatusInfo.CduStatus[0] = '0';
	}

/***************************************************/

	int		statusflag = 0;										// 交易模式下上送真实，维护模式下上送正常
	int		timeFilterflag = 0;									// 定时优化标志
	int		DvrStatusflag = 0;									// DVR状态上送正常
	int		SprSet	= 0;										// 凭条优化标志
	int		cashboxopt	= 0;									// 钞箱优化标志

	int		statusFilter = 0;									// 优化标志

	statusflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_STATUSFLAG, 0);
	timeFilterflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TIMEFILTERFLAG, 0);
	DvrStatusflag = IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRSTATUSFLAG,0);
	SprSet = Asc2Int(IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"SPRSET","0"));
	cashboxopt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CASHBOXOPTFLAG, 0);

	if (nAgentOperatorMode == TRUE)								// 维护模式下						
	{
		statusFilter = 1;
	}

	if (statusflag)
	{	
		if (TranCode == TC_AEXSTATUS ||
			TranCode == TC_OEXSTATUS ||
			TranCode == TC_MSRSTATUS)							// 状态报文
		{
			statusFilter = 1;
		}
	}

	if (timeFilterflag)
	{
		// 取配置文件时间
		CString strTimeStart = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REPORTSTARTTIME, "090000");
		CString strTimeEnd = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REPORTENDTTIME, "170000");

		if ((GetTime() >= strTimeStart) &&						// 当天 100000-170000
			(GetTime() <= strTimeEnd))
		{
			;
		}
		else
		{
			statusFilter = 1;
		}
	}

	if (DvrStatusflag)
	{
		if (AtmStatusInfo.DVRStatus[0] != '9')
			AtmStatusInfo.DVRStatus[0] = '0';

		if (AtmStatusInfo.Cam1Status[0] != '9')
			AtmStatusInfo.Cam1Status[0] = '0';

		if (AtmStatusInfo.Cam2Status[0] != '9')
			AtmStatusInfo.Cam2Status[0] = '0';

		if (AtmStatusInfo.Cam3Status[0] != '9')
			AtmStatusInfo.Cam3Status[0] = '0';
	}

	if (SprSet)
	{
		if (AtmStatusInfo.SprStatus[0] != '9')
			AtmStatusInfo.SprStatus[0] = '0';
	}

	if (cashboxopt)
	{
		if (AtmStatusInfo.Deno1Status[0] != '9'	||
			AtmStatusInfo.Cst1Value[0] != '0'	||
			AtmStatusInfo.Cst1Value[1] != '0')
			AtmStatusInfo.Deno1Status[0] = '0';

		if (AtmStatusInfo.Deno2Status[0] != '9'	||
			AtmStatusInfo.Cst2Value[0] != '0'	||
			AtmStatusInfo.Cst2Value[1] != '0')
			AtmStatusInfo.Deno2Status[0] = '0';
		
		if (AtmStatusInfo.Deno3Status[0] != '9'	||
			AtmStatusInfo.Cst3Value[0] != '0'	||
			AtmStatusInfo.Cst3Value[1] != '0')
			AtmStatusInfo.Deno3Status[0] = '0';
		
		if (AtmStatusInfo.Deno4Status[0] != '9'	||
			AtmStatusInfo.Cst4Value[0] != '0'	||
			AtmStatusInfo.Cst4Value[1] != '0')
			AtmStatusInfo.Deno4Status[0] = '0';
		
		if (AtmStatusInfo.Deno5Status[0] != '9'	||
			AtmStatusInfo.Cst5Value[0] != '0'	||
			AtmStatusInfo.Cst5Value[1] != '0')
			AtmStatusInfo.Deno5Status[0] = '0';
		
		if (AtmStatusInfo.Deno6Status[0] != '9'	||
			AtmStatusInfo.Cst6Value[0] != '0'	||
			AtmStatusInfo.Cst6Value[1] != '0')
			AtmStatusInfo.Deno6Status[0] = '0';

		if (AtmStatusInfo.DepBoxStatus[0] != '9')
			AtmStatusInfo.DepBoxStatus[0] = '0';
		
		if (AtmStatusInfo.RejectStatus[0] != '9')
			AtmStatusInfo.RejectStatus[0] = '0';
	}
	
	if (statusFilter)
	{
		if (AtmStatusInfo.CduStatus[0] != '9')
			AtmStatusInfo.CduStatus[0] = '0';

		if (AtmStatusInfo.DepStatus[0] != '9')
			AtmStatusInfo.DepStatus[0] = '0';

		if (AtmStatusInfo.ICStatus[0] != '9')
			AtmStatusInfo.ICStatus[0] = '0';
		
		if (AtmStatusInfo.McuStatus[0] != '9')
			AtmStatusInfo.McuStatus[0] = '0';
		
		if (AtmStatusInfo.DoorStatus[0] != '9')
			AtmStatusInfo.DoorStatus[0] = '0';

		if (AtmStatusInfo.PinpadStatus[0] != '9')
			AtmStatusInfo.PinpadStatus[0] = '0';

		if (AtmStatusInfo.DispStatus[0] != '9')
			AtmStatusInfo.DispStatus[0] = '0';

		if (AtmStatusInfo.DESStatus[0] != '9')
			AtmStatusInfo.DESStatus[0] = '0';

		if (AtmStatusInfo.MBStatus[0] != '9')
			AtmStatusInfo.MBStatus[0] = '0';

		if (AtmStatusInfo.TicketStatus[0] != '9')
			AtmStatusInfo.TicketStatus[0] = '0';

		if (AtmStatusInfo.VoiceStatus[0] != '9')
			AtmStatusInfo.VoiceStatus[0] = '0';

		if (AtmStatusInfo.PassbookStatus[0] != '9')
			AtmStatusInfo.PassbookStatus[0] = '0';

		if (AtmStatusInfo.DevelopStatus[0] != '9')
			AtmStatusInfo.DevelopStatus[0] = '0';

		if (AtmStatusInfo.DVRStatus[0] != '9')
			AtmStatusInfo.DVRStatus[0] = '0';

		if (AtmStatusInfo.Cam1Status[0] != '9')
			AtmStatusInfo.Cam1Status[0] = '0';

		if (AtmStatusInfo.Cam2Status[0] != '9')
			AtmStatusInfo.Cam2Status[0] = '0';

		if (AtmStatusInfo.Cam3Status[0] != '9')
			AtmStatusInfo.Cam3Status[0] = '0';

		if (AtmStatusInfo.JprStatus[0] != '9')
			AtmStatusInfo.JprStatus[0] = '0';

		if (AtmStatusInfo.SprStatus[0] != '9')
			AtmStatusInfo.SprStatus[0] = '0';

		if (AtmStatusInfo.DvrHDStatus[0] != '9')
			AtmStatusInfo.DvrHDStatus[0] = '0';


		if (AtmStatusInfo.Deno1Status[0] != '9'	||
			AtmStatusInfo.Cst1Value[0] != '0'	||
			AtmStatusInfo.Cst1Value[1] != '0')
			AtmStatusInfo.Deno1Status[0] = '0';

		if (AtmStatusInfo.Deno2Status[0] != '9'	||
			AtmStatusInfo.Cst2Value[0] != '0'	||
			AtmStatusInfo.Cst2Value[1] != '0')
			AtmStatusInfo.Deno2Status[0] = '0';
		
		if (AtmStatusInfo.Deno3Status[0] != '9'	||
			AtmStatusInfo.Cst3Value[0] != '0'	||
			AtmStatusInfo.Cst3Value[1] != '0')
			AtmStatusInfo.Deno3Status[0] = '0';
		
		if (AtmStatusInfo.Deno4Status[0] != '9'	||
			AtmStatusInfo.Cst4Value[0] != '0'	||
			AtmStatusInfo.Cst4Value[1] != '0')
			AtmStatusInfo.Deno4Status[0] = '0';
		
		if (AtmStatusInfo.Deno5Status[0] != '9'	||
			AtmStatusInfo.Cst5Value[0] != '0'	||
			AtmStatusInfo.Cst5Value[1] != '0')
			AtmStatusInfo.Deno5Status[0] = '0';
		
		if (AtmStatusInfo.Deno6Status[0] != '9'	||
			AtmStatusInfo.Cst6Value[0] != '0'	||
			AtmStatusInfo.Cst6Value[1] != '0')
			AtmStatusInfo.Deno6Status[0] = '0';

		if (AtmStatusInfo.DepBoxStatus[0] != '9')
			AtmStatusInfo.DepBoxStatus[0] = '0';

		if (AtmStatusInfo.RejectStatus[0] != '9')
			AtmStatusInfo.RejectStatus[0] = '0';

		if (AtmStatusInfo.AtmStatus[0] != 'M')
			AtmStatusInfo.AtmStatus[0] = 'O';
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.CduStatus[%1.1s]", AtmStatusInfo.CduStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.DepStatus[%1.1s]", AtmStatusInfo.DepStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.McuStatus[%1.1s]", AtmStatusInfo.McuStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.DoorStatus[%1.1s]", AtmStatusInfo.DoorStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.PinpadStatus[%1.1s]", AtmStatusInfo.PinpadStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.DVRStatus[%1.1s]", AtmStatusInfo.DVRStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cam1Status[%1.1s]", AtmStatusInfo.Cam1Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cam2Status[%1.1s]", AtmStatusInfo.Cam2Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cam3Status[%1.1s]", AtmStatusInfo.Cam3Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.JprStatus[%1.1s]", AtmStatusInfo.JprStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.SprStatus[%1.1s]", AtmStatusInfo.SprStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.DvrHDStatus[%1.1s]", AtmStatusInfo.DvrHDStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Deno1Status[%1.1s]", AtmStatusInfo.Deno1Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Deno2Status[%1.1s]", AtmStatusInfo.Deno2Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Deno3Status[%1.1s]", AtmStatusInfo.Deno3Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Deno4Status[%1.1s]", AtmStatusInfo.Deno4Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.DepBoxStatus[%1.1s]", AtmStatusInfo.DepBoxStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.RejectStatus[%1.1s]", AtmStatusInfo.RejectStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst1Value[%2.2s]", AtmStatusInfo.Cst1Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst1Count[%4.4s]", AtmStatusInfo.Cst1Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst2Value[%2.2s]", AtmStatusInfo.Cst2Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst2Count[%4.4s]", AtmStatusInfo.Cst2Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst3Value[%2.2s]", AtmStatusInfo.Cst3Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst3Count[%4.4s]", AtmStatusInfo.Cst3Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst4Value[%2.2s]", AtmStatusInfo.Cst4Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.Cst4Count[%4.4s]", AtmStatusInfo.Cst4Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.RejectCount[%4.4s]", AtmStatusInfo.RejectCount);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.AtmStatus[%1.1s]", AtmStatusInfo.AtmStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo() AtmStatusInfo.CardRetainCnt[%2.2s]", AtmStatusInfo.CardRetainCnt);



/***************************************************/


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo():Return");
	return T_OK;
}
	
// Filter Atm Status Info
int CTranCmn::fnAPP_FilterAtmStatusInfo127()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo()");
	// 钞箱故障，存款模块正常，取款模块正常
	if (AtmStatusInfo127.Deno1Status[0] == '8' ||
		AtmStatusInfo127.Deno2Status[0] == '8' ||
		AtmStatusInfo127.Deno3Status[0] == '8' ||
		AtmStatusInfo127.Deno4Status[0] == '8' ||
		AtmStatusInfo127.Deno5Status[0] == '8' ||
		AtmStatusInfo127.Deno6Status[0] == '8')
	{
		AtmStatusInfo127.CduDepStatus[0] = '0';
	}
	
	// 废钞箱满，存款模块正常，取款模块正常
	// 废钞箱故障，存款模块正常，取款模块正常
	if (AtmStatusInfo127.RejectStatus[0] == '3' ||
		AtmStatusInfo127.RejectStatus[0] == '8')
	{
		AtmStatusInfo127.CduDepStatus[0] = '0';
	}
	

/***************************************************/

	int		statusflag = 0;										// 交易模式下上送真实，维护模式下上送正常
	int		timeFilterflag = 0;									// 定时优化标志
	int		DvrStatusflag = 0;									// DVR状态上送正常
	int		SprSet	= 0;										// 凭条优化标志
	int		cashboxopt	= 0;									// 钞箱优化标志

	int		statusFilter = 0;									// 优化标志

	statusflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_STATUSFLAG, 0);
	timeFilterflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TIMEFILTERFLAG, 0);
	DvrStatusflag = IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRSTATUSFLAG,0);
	SprSet = Asc2Int(IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"SPRSET","0"));
	cashboxopt = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CASHBOXOPTFLAG, 0);

	if (nAgentOperatorMode == TRUE)								// 维护模式下						
	{
		statusFilter = 1;
	}

	if (statusflag)
	{	
		if (TranCode == TC_AEXSTATUS ||
			TranCode == TC_OEXSTATUS ||
			TranCode == TC_MSRSTATUS)							// 状态报文
		{
			statusFilter = 1;
		}
	}

	if (timeFilterflag)
	{
		// 取配置文件时间
		CString strTimeStart = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REPORTSTARTTIME, "090000");
		CString strTimeEnd = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REPORTENDTTIME, "170000");

		if ((GetTime() >= strTimeStart) &&						// 当天 100000-170000
			(GetTime() <= strTimeEnd))
		{
			;
		}
		else
		{
			statusFilter = 1;
		}
	}

	if (DvrStatusflag)
	{
		if (AtmStatusInfo127.DVRStatus[0] != '9')
			AtmStatusInfo127.DVRStatus[0] = '0';

		if (AtmStatusInfo127.Cam1Status[0] != '9')
			AtmStatusInfo127.Cam1Status[0] = '0';

		if (AtmStatusInfo127.Cam2Status[0] != '9')
			AtmStatusInfo127.Cam2Status[0] = '0';

		if (AtmStatusInfo127.Cam3Status[0] != '9')
			AtmStatusInfo127.Cam3Status[0] = '0';

		if (AtmStatusInfo127.Cam4Status[0] != '9')
			AtmStatusInfo127.Cam4Status[0] = '0';
		
		if (AtmStatusInfo127.Cam5Status[0] != '9')
			AtmStatusInfo127.Cam5Status[0] = '0';
		
		if (AtmStatusInfo127.Cam6Status[0] != '9')
			AtmStatusInfo127.Cam6Status[0] = '0';
	}

	if (SprSet)
	{
		if (AtmStatusInfo127.SprStatus[0] != '9')
			AtmStatusInfo127.SprStatus[0] = '0';
	}

	if (cashboxopt)
	{
		if (AtmStatusInfo127.Deno1Status[0] != '9'	||
			AtmStatusInfo127.Cst1Value[0] != '0'	||
			AtmStatusInfo127.Cst1Value[1] != '0')
			AtmStatusInfo127.Deno1Status[0] = '0';

		if (AtmStatusInfo127.Deno2Status[0] != '9'	||
			AtmStatusInfo127.Cst2Value[0] != '0'	||
			AtmStatusInfo127.Cst2Value[1] != '0')
			AtmStatusInfo127.Deno2Status[0] = '0';
		
		if (AtmStatusInfo127.Deno3Status[0] != '9'	||
			AtmStatusInfo127.Cst3Value[0] != '0'	||
			AtmStatusInfo127.Cst3Value[1] != '0')
			AtmStatusInfo127.Deno3Status[0] = '0';
		
		if (AtmStatusInfo127.Deno4Status[0] != '9'	||
			AtmStatusInfo127.Cst4Value[0] != '0'	||
			AtmStatusInfo127.Cst4Value[1] != '0')
			AtmStatusInfo127.Deno4Status[0] = '0';
		
		if (AtmStatusInfo127.Deno5Status[0] != '9'	||
			AtmStatusInfo127.Cst5Value[0] != '0'	||
			AtmStatusInfo127.Cst5Value[1] != '0')
			AtmStatusInfo127.Deno5Status[0] = '0';
		
		if (AtmStatusInfo127.Deno6Status[0] != '9'	||
			AtmStatusInfo127.Cst6Value[0] != '0'	||
			AtmStatusInfo127.Cst6Value[1] != '0')
			AtmStatusInfo127.Deno6Status[0] = '0';
		
		if (AtmStatusInfo127.RejectStatus[0] != '9')
			AtmStatusInfo127.RejectStatus[0] = '0';
	}
	
	if (statusFilter)
	{
		if (AtmStatusInfo127.CduDepStatus[0] != '9')
			AtmStatusInfo127.CduDepStatus[0] = '0';

		if (AtmStatusInfo127.ICStatus[0] != '9')
			AtmStatusInfo127.ICStatus[0] = '0';
		
		if (AtmStatusInfo127.McuStatus[0] != '9')
			AtmStatusInfo127.McuStatus[0] = '0';

		if (AtmStatusInfo127.McuExitStatus[0] != '9')
			AtmStatusInfo127.McuExitStatus[0] = '0';
		
		if (AtmStatusInfo127.DoorStatus[0] != '9')
			AtmStatusInfo127.DoorStatus[0] = '0';

		if (AtmStatusInfo127.PinpadStatus[0] != '9')
			AtmStatusInfo127.PinpadStatus[0] = '0';

		if (AtmStatusInfo127.DispStatus[0] != '9')
			AtmStatusInfo127.DispStatus[0] = '0';

		if (AtmStatusInfo127.DESStatus[0] != '9')
			AtmStatusInfo127.DESStatus[0] = '0';

		if (AtmStatusInfo127.MBStatus[0] != '9')
			AtmStatusInfo127.MBStatus[0] = '0';

		if (AtmStatusInfo127.TicketStatus[0] != '9')
			AtmStatusInfo127.TicketStatus[0] = '0';

		if (AtmStatusInfo127.VoiceStatus[0] != '9')
			AtmStatusInfo127.VoiceStatus[0] = '0';

		if (AtmStatusInfo127.PassbookStatus[0] != '9')
			AtmStatusInfo127.PassbookStatus[0] = '0';

		if (AtmStatusInfo127.DevelopStatus[0] != '9')
			AtmStatusInfo127.DevelopStatus[0] = '0';

		if (AtmStatusInfo127.DVRStatus[0] != '9')
			AtmStatusInfo127.DVRStatus[0] = '0';

		if (AtmStatusInfo127.Cam1Status[0] != '9')
			AtmStatusInfo127.Cam1Status[0] = '0';

		if (AtmStatusInfo127.Cam2Status[0] != '9')
			AtmStatusInfo127.Cam2Status[0] = '0';

		if (AtmStatusInfo127.Cam3Status[0] != '9')
			AtmStatusInfo127.Cam3Status[0] = '0';

		if (AtmStatusInfo127.Cam4Status[0] != '9')
			AtmStatusInfo127.Cam4Status[0] = '0';
		
		if (AtmStatusInfo127.Cam5Status[0] != '9')
			AtmStatusInfo127.Cam5Status[0] = '0';
		
		if (AtmStatusInfo127.Cam6Status[0] != '9')
			AtmStatusInfo127.Cam6Status[0] = '0';

		if (AtmStatusInfo127.JprStatus[0] != '9')
			AtmStatusInfo127.JprStatus[0] = '0';

		if (AtmStatusInfo127.SprStatus[0] != '9')
			AtmStatusInfo127.SprStatus[0] = '0';

		if (AtmStatusInfo127.DvrHDStatus[0] != '9')
			AtmStatusInfo127.DvrHDStatus[0] = '0';

		if (AtmStatusInfo127.Deno1Status[0] != '9'	||
			AtmStatusInfo127.Cst1Value[0] != '0'	||
			AtmStatusInfo127.Cst1Value[1] != '0')
			AtmStatusInfo127.Deno1Status[0] = '0';
		
		if (AtmStatusInfo127.Deno2Status[0] != '9'	||
			AtmStatusInfo127.Cst2Value[0] != '0'	||
			AtmStatusInfo127.Cst2Value[1] != '0')
			AtmStatusInfo127.Deno2Status[0] = '0';
		
		if (AtmStatusInfo127.Deno3Status[0] != '9'	||
			AtmStatusInfo127.Cst3Value[0] != '0'	||
			AtmStatusInfo127.Cst3Value[1] != '0')
			AtmStatusInfo127.Deno3Status[0] = '0';
		
		if (AtmStatusInfo127.Deno4Status[0] != '9'	||
			AtmStatusInfo127.Cst4Value[0] != '0'	||
			AtmStatusInfo127.Cst4Value[1] != '0')
			AtmStatusInfo127.Deno4Status[0] = '0';
		
		if (AtmStatusInfo127.Deno5Status[0] != '9'	||
			AtmStatusInfo127.Cst5Value[0] != '0'	||
			AtmStatusInfo127.Cst5Value[1] != '0')
			AtmStatusInfo127.Deno5Status[0] = '0';
		
		if (AtmStatusInfo127.Deno6Status[0] != '9'	||
			AtmStatusInfo127.Cst6Value[0] != '0'	||
			AtmStatusInfo127.Cst6Value[1] != '0')
			AtmStatusInfo127.Deno6Status[0] = '0';

		if (AtmStatusInfo127.RejectStatus[0] != '9')
			AtmStatusInfo127.RejectStatus[0] = '0';

//		if (AtmStatusInfo127.WithdrawStatus[0] != '9')
		if (WithAvail & TRAN_WITH_BOTH)
			AtmStatusInfo127.WithdrawStatus[0] = '0';

//		if (AtmStatusInfo127.DepositStatus[0] != '9')
		if (DepAvail & TRAN_DEP_CASH)
			AtmStatusInfo127.DepositStatus[0] = '0';

		if (AtmStatusInfo127.RejectOperationStatus[0] != '9')
			AtmStatusInfo127.RejectOperationStatus[0] = '0';

		if (AtmStatusInfo127.AtmStatus[0] != 'M')
			AtmStatusInfo127.AtmStatus[0] = 'O';
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.CduStatus[%1.1s]", AtmStatusInfo127.CduDepStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.McuStatus[%1.1s]", AtmStatusInfo127.McuStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.DoorStatus[%1.1s]", AtmStatusInfo127.DoorStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.PinpadStatus[%1.1s]", AtmStatusInfo127.PinpadStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.DVRStatus[%1.1s]", AtmStatusInfo127.DVRStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cam1Status[%1.1s]", AtmStatusInfo127.Cam1Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cam2Status[%1.1s]", AtmStatusInfo127.Cam2Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cam3Status[%1.1s]", AtmStatusInfo127.Cam3Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.JprStatus[%1.1s]", AtmStatusInfo127.JprStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.SprStatus[%1.1s]", AtmStatusInfo127.SprStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.DvrHDStatus[%1.1s]", AtmStatusInfo127.DvrHDStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Deno1Status[%1.1s]", AtmStatusInfo127.Deno1Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Deno2Status[%1.1s]", AtmStatusInfo127.Deno2Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Deno3Status[%1.1s]", AtmStatusInfo127.Deno3Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Deno4Status[%1.1s]", AtmStatusInfo127.Deno4Status);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.RejectStatus[%1.1s]", AtmStatusInfo127.RejectStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst1Value[%2.2s]", AtmStatusInfo127.Cst1Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst1Count[%4.4s]", AtmStatusInfo127.Cst1Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst2Value[%2.2s]", AtmStatusInfo127.Cst2Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst2Count[%4.4s]", AtmStatusInfo127.Cst2Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst3Value[%2.2s]", AtmStatusInfo127.Cst3Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst3Count[%4.4s]", AtmStatusInfo127.Cst3Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst4Value[%2.2s]", AtmStatusInfo127.Cst4Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.Cst4Count[%4.4s]", AtmStatusInfo127.Cst4Count);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.RejectCount[%4.4s]", AtmStatusInfo127.RejectCount);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.AtmStatus[%1.1s]", AtmStatusInfo127.AtmStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127() AtmStatusInfo127.CardRetainCnt[%2.2s]", AtmStatusInfo127.CardRetainCnt);



/***************************************************/


MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_FilterAtmStatusInfo127():Return");
	return T_OK;
}

		
// Get Atm Total Info
int CTranCmn::fnAPP_GetAtmTotalInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo()");
	int iRemainAmount = 0;

	m_pProfile->GetWithTotal();
	m_pProfile->GetTransTotal();

	memset(&AtmTotalInfo, '0', sizeof(AtmTotalInfo));

	memcpy(AtmTotalInfo.WithdrawCount, GetSprintf("%6.6s", m_pProfile->WITHTOTAL.DATA[0].Count), 6);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.WithdrawCount[%6.6s]", AtmTotalInfo.WithdrawCount);
	memcpy(AtmTotalInfo.WithdrawAmount,	GetSprintf("%12.12s", &m_pProfile->WITHTOTAL.DATA[0].Amount[2]), 12);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.WithdrawAmount[%12.12s]", AtmTotalInfo.WithdrawAmount);

	memcpy(AtmTotalInfo.LoadCount, GetSprintf("%6.6s", m_pProfile->TRANSTOTAL.DATA[8].Count), 6);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.LoadCount[%6.6s]", AtmTotalInfo.LoadCount);
	memcpy(AtmTotalInfo.LoadAmount,	GetSprintf("%12.12s", &m_pProfile->TRANSTOTAL.DATA[8].Amount[2]), 12);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.LoadAmount[%12.12s]", AtmTotalInfo.LoadAmount);

	memcpy(AtmTotalInfo.TransferCount, GetSprintf("%6.6s", m_pProfile->TRANSTOTAL.DATA[0].Count), 6);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.TransferCount[%6.6s]", AtmTotalInfo.TransferCount);
	memcpy(AtmTotalInfo.TransferAmount,	GetSprintf("%12.12s", &m_pProfile->TRANSTOTAL.DATA[0].Amount[2]), 12);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.TransferAmount[%12.12s]", AtmTotalInfo.TransferAmount);

	if (m_pDevCmn->CSTCnt >= 1)
		iRemainAmount += (m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)) * m_pDevCmn->FstCSTValue;
	if (m_pDevCmn->CSTCnt >= 2)
		iRemainAmount += (m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)) * m_pDevCmn->SndCSTValue;
	if (m_pDevCmn->CSTCnt >= 3)
		iRemainAmount += (m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3)) * m_pDevCmn->TrdCSTValue;
	if (m_pDevCmn->CSTCnt >= 4)
		iRemainAmount += (m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_4) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4)) * m_pDevCmn->FthCSTValue;
	
	memcpy(AtmTotalInfo.RemainAmount, GetSprintf("%010.10d00", iRemainAmount), 12);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo() AtmTotalInfo.RemainAmount[%12.12s]", AtmTotalInfo.RemainAmount);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmTotalInfo():Return");
	return T_OK;
}

// Get Atm AddCash Info
int CTranCmn::fnAPP_GetAtmAddCashInfo()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo()");
	
	memset(&AtmAddCashInfo, '0', sizeof(AtmAddCashInfo));

	if (m_pDevCmn->CSTCnt >= 1)
	{
		if (m_pDevCmn->FstCSTValue == 100)
		{
			AtmAddCashInfo.Cst1Value[0] = '1';
			AtmAddCashInfo.Cst1Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
		}
		else if (m_pDevCmn->FstCSTValue == 50)
		{
			AtmAddCashInfo.Cst1Value[0] = '2';
			AtmAddCashInfo.Cst1Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
		}
		else if (m_pDevCmn->FstCSTValue == 10)
		{
			AtmAddCashInfo.Cst1Value[0] = '3';
			AtmAddCashInfo.Cst1Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
		}
		else if (m_pDevCmn->FstCSTValue == 20)
		{
			AtmAddCashInfo.Cst1Value[0] = '4';
			AtmAddCashInfo.Cst1Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst1Count, GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst1Value[%2.2s]", AtmAddCashInfo.Cst1Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst1Count[%4.4s]", AtmAddCashInfo.Cst1Count);
	}
	if (m_pDevCmn->CSTCnt >= 2)
	{
		if (m_pDevCmn->SndCSTValue == 100)
		{
			AtmAddCashInfo.Cst2Value[0] = '1';
			AtmAddCashInfo.Cst2Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
		}
		else if (m_pDevCmn->SndCSTValue == 50)
		{
			AtmAddCashInfo.Cst2Value[0] = '2';
			AtmAddCashInfo.Cst2Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
		}
		else if (m_pDevCmn->SndCSTValue == 10)
		{
			AtmAddCashInfo.Cst2Value[0] = '3';
			AtmAddCashInfo.Cst2Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
		}
		else if (m_pDevCmn->SndCSTValue == 20)
		{
			AtmAddCashInfo.Cst2Value[0] = '4';
			AtmAddCashInfo.Cst2Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst2Count, GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst2Value[%2.2s]", AtmAddCashInfo.Cst2Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst2Count[%4.4s]", AtmAddCashInfo.Cst2Count);
	}
	if (m_pDevCmn->CSTCnt >= 3)
	{
		if (m_pDevCmn->TrdCSTValue == 100)
		{
			AtmAddCashInfo.Cst3Value[0] = '1';
			AtmAddCashInfo.Cst3Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == 50)
		{
			AtmAddCashInfo.Cst3Value[0] = '2';
			AtmAddCashInfo.Cst3Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == 10)
		{
			AtmAddCashInfo.Cst3Value[0] = '3';
			AtmAddCashInfo.Cst3Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
		}
		else if (m_pDevCmn->TrdCSTValue == 20)
		{
			AtmAddCashInfo.Cst3Value[0] = '4';
			AtmAddCashInfo.Cst3Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst3Count, GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst3Value[%2.2s]", AtmAddCashInfo.Cst3Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst3Count[%4.4s]", AtmAddCashInfo.Cst3Count);
	}
	if (m_pDevCmn->CSTCnt >= 4)
	{
		if (m_pDevCmn->FthCSTValue == 100)
		{
			AtmAddCashInfo.Cst4Value[0] = '1';
			AtmAddCashInfo.Cst4Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
		}
		else if (m_pDevCmn->FthCSTValue == 50)
		{
			AtmAddCashInfo.Cst4Value[0] = '2';
			AtmAddCashInfo.Cst4Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
		}
		else if (m_pDevCmn->FthCSTValue == 10)
		{
			AtmAddCashInfo.Cst4Value[0] = '3';
			AtmAddCashInfo.Cst4Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
		}
		else if (m_pDevCmn->FthCSTValue == 20)
		{
			AtmAddCashInfo.Cst4Value[0] = '4';
			AtmAddCashInfo.Cst4Value[1] = '1';
			memcpy(AtmAddCashInfo.Cst4Count, GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
		}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst4Value[%2.2s]", AtmAddCashInfo.Cst4Value);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo() AtmAddCashInfo.Cst4Count[%4.4s]", AtmAddCashInfo.Cst4Count);
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetAtmAddCashInfo():Return");
	return T_OK;
}	

// Send Host Make Header
int CTranCmn::fnAPP_SendHostMakeHeader(int RealFlag, void* MakeArea)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeHeader(RealFlag[%d])", RealFlag);

	HHMSendHeader*		pHHMSendData = (HHMSendHeader*)MakeArea;
	memset(pHHMSendData, '0', sizeof(HHMSendHeader));

//	switch (TranCode)
//	{
//		case TC_INQUIRY:
//		case TC_TRANSFER:
//		case TC_HZT						//liyi add 绿卡通
//		case TC_TZH		
//		case TC_WITHDRAWAL:
//		case TC_CHANGEPIN:
//		case TC_DETAIL:
//		case TC_REMIT_ADD:
//		case TC_REMIT_IN:
//		case TC_REMIT_PWD:
//		case TC_PAY:
//		case TC_CANCEL:
//		case TC_CONFIRMWITH:
//		case TC_OPEN:
//		case TC_ERROR:
//		case TC_AEXSTATUS:
//		case TC_OEXSTATUS:
//		case TC_MSRSTATUS:
//		case TC_TOTAL:
//		case TC_TOTALPRE:
//		case TC_TOTALKIND:
//		case TC_CARDRETAIN:
//		case TC_ADDCASH:
//		case TC_SUBCASH:
//		case TC_CLEARCASH:
//		case TC_PRINTCASH:
//		case TC_DOWNFIT:
//		case TC_DOWNPARM:
//		case TC_DOWNPAY:
//		case TC_DOWNECHANNELPARA:
//		case TC_DOWNECHANNELPROT:
//		case TC_SMSPARITY
//		case TC_ECHANNEL_INQ
//		case TC_ECHANNEL
//		case TC_SENDDETAIL:
//		case TC_DETAILEND:
//		case TC_ATMOPEN:
//		case TC_ATMCLOSE:
//		case TC_ATMSTATUS:
//		case TC_ATMBIDSENDDETAIL:
//		case TC_ATMWORKPARAM:
//		case TC_ATMREBOOT:
//		case TC_ATMFTPSEND:
//		case TC_ATMRESETDVR:
//			break;
//			
//		default:
//			break;
//	}

	memcpy(pHHMSendData->TPDU, "00000", 5);
	memcpy(pHHMSendData->NetworkFlag, "01", 2);
	memcpy(pHHMSendData->TransSerialNo, m_pProfile->TRANS.SerialNo, 8);
	memcpy(pHHMSendData->ErrorCode, "0000", 4);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeHeader():return");
	return T_OK;
}
// Send Host Make Set
int CTranCmn::fnAPP_SendHostMakeSet(int RealFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeSet(RealFlag[%d])", RealFlag);

	CStringArray	strTempArray;

	if ((TranCode == TC_CANCEL)	||								// 2005.09.11
		(TranCode == TC_ERROR))
	{
		SplitString(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCARD), ",", strTempArray);
		if (strTempArray.GetSize() == 6)
		{
			MakePack(strTempArray[0].GetBuffer(0), &CardTran,		sizeof(CardTran) * 2);
			MakePack(strTempArray[1].GetBuffer(0), &CardBank,		sizeof(CardBank) * 2);
			MakePack(strTempArray[2].GetBuffer(0), &CardService,	sizeof(CardService) * 2);
			MakePack(strTempArray[3].GetBuffer(0), &CardData,		sizeof(CardData) * 2);
			MakePack(strTempArray[4].GetBuffer(0), &CardAccountNo,	sizeof(CardAccountNo) * 2);
			MakePack(strTempArray[5].GetBuffer(0), &CardPan,		sizeof(CardPan) * 2);
		}
		else
		{
			CardTran = MENU_NULL;								// Card Transaction
			CardBank = 0;										// Card Bank
			CardService = 0;									// Card Service
			memset(&CardData, 0, sizeof(CardData));				// Card Data 
			memset(CardAccountNo, 0, sizeof(CardAccountNo));	// Card Account No
			memset(CardPan, 0, sizeof(CardPan));				// Card Pan
		}
	}

	strBitMap.Format("%0129.129s", "");

	switch (TranCode)
	{
		case TC_INQUIRY:
		case TC_INQUIRY_CheckPin:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			}
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
//			strBitMap.SetAt(42, '1');							// Card Acceptor Name/Location
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');						// Pay Attention
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110505
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
		case TC_CUS_INF_INQ:
		case TC_CUS_INF_INQ_OTHERBANK:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(Accept.InAccountNum))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			if(fnAPL_checkLocalCard(Accept.InAccountNum))
									;
			else if(Asc2Int(HMESend.Data[45]) != 0)
			{
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "data11223345[%d]",&HMESend.Data[45]);
			strBitMap.SetAt(45, '1');
			}
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
		case TC_ECHANNEL_INQ:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(55, '1');
			strBitMap.SetAt(122,'1');							// 发起自定义数据
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
// 20080904 by yaokq for 绿卡通
		case TC_OTHER_INQ:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			}
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			//			strBitMap.SetAt(42, '1');							// Card Acceptor Name/Location
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110519
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(122,'1');							// 发起自定义数据
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
		case TC_TRANSFER:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');							// Transaction Amount
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
//			strBitMap.SetAt(16, '1');							// 存期，转入定期使用   by zjd
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			if(fnAPL_checkLocalCard(CString(Accept.InAccountNum)))
					;
			else if(Asc2Int(HMESend.Data[45]) != 0)
			strBitMap.SetAt(45, '1');
//			strBitMap.SetAt(42, '1');							// Card Acceptor Name/Location
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110519
			strBitMap.SetAt(72, '1');							// Batch Number
			if (strlen(CardAccountNo))
				strBitMap.SetAt(102,'1');						// Transfer Account No(Card Account No)
			if (Accept.InAccountNumSize)
				strBitMap.SetAt(103,'1');						// To Card No
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_LOAD:
		case TC_BOUNDMDOIFY:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');							// Transaction Amount
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(55, '1');
			strBitMap.SetAt(59, '1');
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_HZT:											//liyi add 绿卡通
		case TC_TZH:											
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');							// Transaction Amount
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(16, '1');
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
//			strBitMap.SetAt(42, '1');							// Card Acceptor Name/Location
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110519
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(102,'1');							// Transfer Account No(Card Account No)
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_REMIT:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(61, '1');
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(106,'1');
			strBitMap.SetAt(122,'1');							// 发起自定义数据
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_REMIT_ADD:
		case TC_REMIT_PWD:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');							// Transaction Amount
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(61, '1');
			if (TranCode == TC_REMIT_PWD)
				strBitMap.SetAt(62, '1');
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(103,'1');
			strBitMap.SetAt(106,'1');
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data			
			break;

		case TC_REMIT_IN:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');							// Transaction Amount
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(62, '1');
			strBitMap.SetAt(68, '1');							// liyi add 2008.10.17 汇票号，接口文档未写
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data			
			break;

		case TC_REMIT_INF_INQ:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(62, '1');
			strBitMap.SetAt(68, '1');
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data			
			break;
			

		case TC_WITHDRAWAL:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');							// Transaction Amount
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');						// 
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110505
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_CHANGEPIN:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110519
			strBitMap.SetAt(62, '1');							// New Pin Data
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_DETAIL:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(49, '1');							// Currency Code
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20110519
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(122,'1');							// Inq Condition
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
		case TC_PAY_INQ:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
		       strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20 20111011
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20111011
			strBitMap.SetAt(72, '1');							// Batch Number
			if (Accept.InAccountNumSize)
				strBitMap.SetAt(103,'1');						// To Card No
			strBitMap.SetAt(122,'1');							// Inq Condition
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
			
		case TC_PAY:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(4,  '1');
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20 20111011
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
				strBitMap.SetAt(23, '1');						// IC SeiralNo
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			if (CardTran & MENU_MCU2)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardTran & MENU_MCU3)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(49, '1');
			strBitMap.SetAt(52, '1');							// Pin Data
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
//				strBitMap.SetAt(59, '1');
			}
			strBitMap.SetAt(59, '1');							// added by liuxl 20111011
			strBitMap.SetAt(72, '1');							// Batch Number

			if (Accept.InAccountNumSize)
				strBitMap.SetAt(103,'1');						// To Card No
			strBitMap.SetAt(122,'1');							// Inq Condition
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;
			
		case TC_OPEN:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(53, '1');						// Additional Data
			if (RQKCode == 0)
				strBitMap.SetAt(96, '1');						// Network Management Information Code
//			if (RQKCode == 1 || RQKCode == 2)
//				strBitMap.SetAt(128,'1');						// Mac Data
			break;

		case TC_DOWNPARM:
		case TC_DOWNPAY:
		case TC_DOWNPBOCPARA:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_SCRIPT:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(2, '1');							// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(23, '1');
			strBitMap.SetAt(25, '1');							// Condition Code
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(49, '1');
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(55, '1');
			strBitMap.SetAt(59, '1');							// added by liuxl 20110508
			strBitMap.SetAt(60, '1');							// Reserved(Message Reason Code)
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(90, '1');							// Original Data
			strBitMap.SetAt(128,'1');							// Mac Data	
			break;
		case TC_CANCEL:
			if (TranCode2 == 2)										// 汇兑异常通知 2008.10.13
			{
				strBitMap.SetAt(0,  '1');							// Transaction Kind
				strBitMap.SetAt(1,  '1');							// Bit Map
				if (strlen(CardAccountNo))
					strBitMap.SetAt(2, '1');						// Card Account No
				strBitMap.SetAt(3,  '1');							// Process Code
				strBitMap.SetAt(7,  '1');							// ChannelNum
				strBitMap.SetAt(11, '1');							// Serial No
				strBitMap.SetAt(12, '1');							// Time
				strBitMap.SetAt(13, '1');							// Date
				strBitMap.SetAt(20, '1');							// Trans Flag
				strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
				strBitMap.SetAt(25, '1');							// Condition Code
				strBitMap.SetAt(41, '1');							// Atm Serial Number
				strBitMap.SetAt(53, '1');							// Security Related Control Information
				strBitMap.SetAt(60, '1');							// Reserved(Message Reason Code)
				strBitMap.SetAt(72, '1');							// Batch Number
				strBitMap.SetAt(90, '1');							// Original Data
				strBitMap.SetAt(128,'1');							// Mac Data	
			}
			else
			{
				strBitMap.SetAt(0,  '1');							// Transaction Kind
				strBitMap.SetAt(1,  '1');							// Bit Map
				if (strlen(CardAccountNo))
					strBitMap.SetAt(2, '1');						// Card Account No
				strBitMap.SetAt(3,  '1');							// Process Code
				strBitMap.SetAt(4,  '1');							// Transaction Amount
				strBitMap.SetAt(7,  '1');							// ChannelNum
				strBitMap.SetAt(11, '1');							// Serial No
				strBitMap.SetAt(12, '1');							// Time
				strBitMap.SetAt(13, '1');							// Date
				strBitMap.SetAt(20, '1');							// Trans Flag
				strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
				if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||			// added by liuxl 20110511
					(m_pDevCmn->McuReadMSOnlyFlag))
					;
				else
					strBitMap.SetAt(23, '1');
				strBitMap.SetAt(25, '1');							// Condition Code
				strBitMap.SetAt(41, '1');							// Atm Serial Number
				strBitMap.SetAt(49, '1');							// Currency Code
				strBitMap.SetAt(53, '1');							// Security Related Control Information
				if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||			// added by liuxl 20110508
					(m_pDevCmn->McuReadMSOnlyFlag))
					;
				else
					strBitMap.SetAt(55, '1');
				strBitMap.SetAt(59, '1');							// added by liuxl 20110508
				strBitMap.SetAt(60, '1');							// Reserved(Message Reason Code)
				strBitMap.SetAt(72, '1');							// Batch Number
				strBitMap.SetAt(90, '1');							// Original Data
				strBitMap.SetAt(128,'1');							// Mac Data
			}
			break;

		case TC_ERROR:		//也是OEX
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(122,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_AEXSTATUS:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			if (CardData.ISO2size)
				strBitMap.SetAt(35, '1');						// Track2
			if (CardData.ISO3size)
				strBitMap.SetAt(36, '1');						// Track3
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(60,'1');							// Message reason
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_CARDRETAIN:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(2, '1');							// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(20, '1');							// Trans Flag
//			strBitMap.SetAt(22, '1');							// Point Of Service Entry Mode Code
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(120,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_OEXSTATUS:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(122,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			break;
			
		case TC_MSRSTATUS:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(122,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_TOTAL:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(51, '1');							// Teller Number
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(121,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_TOTALPRE:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(51, '1');							// Teller Number
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_TOTALKIND:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(25, '1');							// Condition Code
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(51, '1');							// Teller Number
			strBitMap.SetAt(120,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_SUBTOTAL:
			break;

		case TC_ADDCASH:
		case TC_SUBCASH:
		case TC_CLEARCASH:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(21, '1');							// AddCash Info
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(51, '1');							// Teller Number
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_PRINTCASH:
			break;

		case TC_DOWNFIT:										// 主动下载和维护都使用   by zjd
		case TC_DOWNICFIT:										// added by yusy ic
		case TC_DOWNAIDLIST:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(120,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_DOWNECHANNELPARA:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(127,'1');							// Status 127
			break;
			
		case TC_DOWNECHANNELPROT:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(120,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_SMSPARITY:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(120,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			break;

		case TC_ECHANNEL:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			if (strlen(CardAccountNo))
				strBitMap.SetAt(2, '1');						// Card Account No
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				;
			else
			{
				strBitMap.SetAt(55, '1');
			}
			strBitMap.SetAt(61, '1');
			strBitMap.SetAt(63, '1');
			strBitMap.SetAt(122,'1');							// Batch Number
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_SENDDETAIL:
		case TC_DETAILEND:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(7,  '1');							// ChannelNum
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(24, '1');							// Status
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(53, '1');							// Security Related Control Information
			strBitMap.SetAt(72, '1');							// Batch Number
			strBitMap.SetAt(120,'1');							// Additional Data
			strBitMap.SetAt(127,'1');							// Status 127
			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_CONFIRMWITH:
//			strBitMap.SetAt(0,  '1');							// Transaction Kind
//			strBitMap.SetAt(1,  '1');							// Bit Map
//			if (strlen(CardAccountNo))
//				strBitMap.SetAt(2, '1');						// Card Account No
//			strBitMap.SetAt(3,  '1');							// Process Code
//			strBitMap.SetAt(4,  '1');							// Transaction Amount
//			strBitMap.SetAt(7,  '1');							// Date & Time
//			strBitMap.SetAt(11, '1');							// Serial No
//			strBitMap.SetAt(12, '1');							// Time
//			strBitMap.SetAt(13, '1');							// Date
//			strBitMap.SetAt(14, '1');							// Card Expire Date
//			strBitMap.SetAt(15, '1');							// Settlement Date
//			strBitMap.SetAt(32, '1');							// Acquiring Institution ID Code
//			strBitMap.SetAt(33, '1');							// Forwarding Institution ID Code
//			strBitMap.SetAt(37, '1');							// Reference No
//			strBitMap.SetAt(39, '1');							// Response No
//			strBitMap.SetAt(41, '1');							// Atm Serial Number
//			strBitMap.SetAt(43, '1');							// Card Acceptor Name/Location
//			strBitMap.SetAt(49, '1');							// Currency Code
//			strBitMap.SetAt(100,'1');							// Receiving Institution ID Code
//			strBitMap.SetAt(128,'1');							// Mac Data
			break;

		case TC_ATMOPEN:
		case TC_ATMCLOSE:
		case TC_ATMSTATUS:
		case TC_ATMWORKPARAM:
		case TC_ATMRESETALL:
		case TC_ATMREBOOT:
		case TC_ATMFTPSEND:
		case TC_ATMRESETDVR:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(39, '1');							// Response No
			strBitMap.SetAt(41, '1');							// Atm Serial Number
            break;
//////////////广告通知、下载，流水通知、下载，软件更新通知、下载使用   by zjd
		case TC_ATMBIDSENDDETAIL:
		case TC_ATMBIDADDOWNLOAD:
		case TC_ATMBIDSOFTUPDATE:
			strBitMap.SetAt(0,  '1');							// Transaction Kind
			strBitMap.SetAt(1,  '1');							// Bit Map
			strBitMap.SetAt(3,  '1');							// Process Code
			strBitMap.SetAt(11, '1');							// Serial No
			strBitMap.SetAt(12, '1');							// Time
			strBitMap.SetAt(13, '1');							// Date
			strBitMap.SetAt(39, '1');							// Response No
			strBitMap.SetAt(41, '1');							// Atm Serial Number
			strBitMap.SetAt(122, '1');							// Send Information
			strBitMap.SetAt(123, '1');							// Receive Information
			break;
			
		default:
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeSet():return");
	return T_OK;
}

// Send Host Make Msg
int CTranCmn::fnAPP_SendHostMakeMsg(int RealFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg(RealFlag[%d])", RealFlag);

	CString			strTemp("");
	CStringArray	strTempArray;
	char			szTmpData[1024];
	char			szTmpData1[1024];
	char			PDate[256];
	char			PTime[256];
//	char	MasterKey[16];
//	char	MasterKeyCHK[16];
	CString strMasterKey("");
	char			icdata[256];
	int				nlen = 0;

//	char szCduStatus;
//	char szSprStatus;
//	char szJprStatus;
//	char szMcuStatus;
// 	char szPbStatus;
	
//	CString strMasterKey = GetShareData("OP_MASTERKEY");	//长度为32，两倍长
	
	memset(PDate, 0, sizeof(PDate));
	memset(PTime, 0, sizeof(PTime));
	memset(szTmpData, 0, sizeof(szTmpData));
	memset(szTmpData1, 0, sizeof(szTmpData1));

	if (strBitMap.GetAt(0) == '1')								// 0 Transaction Kind
	{
		HMESend.DataLength[0] = 4;
		memset(HMESend.Data[0], '0', HMESend.DataLength[0]);
		switch (TranCode)
		{
			case TC_INQUIRY:
			case TC_TRANSFER:
			case TC_LOAD:						// added by yusy ic
			case TC_HZT:										//liyi add 绿卡通
			case TC_TZH:											
			case TC_WITHDRAWAL:
//			case TC_CONFIRMWITH:
			case TC_DETAIL:
			case TC_PAY:
			case TC_PAY_INQ:
			case TC_CHANGEPIN:
			case TC_OTHER_INQ:                  // 20080904 by yaokq
			case TC_CUS_INF_INQ:				// 2010.03.22 by yusy
			case TC_CUS_INF_INQ_OTHERBANK:		//20140119 jl
			case TC_ECHANNEL_INQ:
			case TC_SMSPARITY:
			case TC_ECHANNEL:
			case TC_INQUIRY_CheckPin:
				memcpy(HMESend.Data[0], "0200", HMESend.DataLength[0]);
				break;

			case TC_REMIT_ADD:
			case TC_REMIT_IN:
			case TC_REMIT_PWD:
			case TC_REMIT:
			case TC_REMIT_INF_INQ:
				memcpy(HMESend.Data[0], "9100", HMESend.DataLength[0]);
				break;

			case TC_BOUNDMDOIFY:
				memcpy(HMESend.Data[0], "0600", HMESend.DataLength[0]);
				break;

			case TC_CANCEL:		
				if (TranCode2 == 2)							// 2008.10.13
					memcpy(HMESend.Data[0], "9120", HMESend.DataLength[0]);
				else
					memcpy(HMESend.Data[0], "0420", HMESend.DataLength[0]);
				break;

			case TC_TOTAL:
			case TC_TOTALPRE:
			case TC_DOWNFIT:
			case TC_DOWNICFIT:								// added by yusy ic
			case TC_DOWNAIDLIST:
			case TC_DOWNPARM:
			case TC_DOWNPBOCPARA:
			case TC_DOWNPAY:
			case TC_SENDDETAIL:
			case TC_DETAILEND:
			case TC_DOWNECHANNELPARA:
			case TC_DOWNECHANNELPROT:
				memcpy(HMESend.Data[0], "0800", HMESend.DataLength[0]);
				break;

			case TC_OPEN:
			case TC_ERROR:
			case TC_AEXSTATUS:
			case TC_CARDRETAIN:
			case TC_TOTALKIND:
			case TC_OEXSTATUS:
			case TC_MSRSTATUS:
			case TC_ADDCASH:
			case TC_SUBCASH:
			case TC_CLEARCASH:
			case TC_PRINTCASH:
			case TC_SCRIPT:
				memcpy(HMESend.Data[0], "0820", HMESend.DataLength[0]);
				break;

			case TC_ATMOPEN:
			case TC_ATMCLOSE:
			case TC_ATMSTATUS:
			case TC_ATMBIDSENDDETAIL:
			case TC_ATMBIDADDOWNLOAD:
		    case TC_ATMBIDSOFTUPDATE:
			case TC_ATMWORKPARAM:
			case TC_ATMRESETALL:
			case TC_ATMREBOOT:
			case TC_ATMFTPSEND:
			case TC_ATMRESETDVR:
				memcpy(HMESend.Data[0], "0830", HMESend.DataLength[0]);
				break;
				
			default:
				break;
		}
	}

	if (strBitMap.GetAt(1) == '1')								// 1 Bit Map
	{
		HMESend.DataLength[1] = 16;
		
		memset(HMESend.Data[1], 0, HMESend.DataLength[1]);
	}
	
	if (strBitMap.GetAt(2) == '1')								// 2 Card Account No
	{
		memset(szTmpData, 0, sizeof(szTmpData));
		if((TranCode == TC_CUS_INF_INQ)||
			(TranCode == TC_CUS_INF_INQ_OTHERBANK))
		{
			if (strlen(Accept.InAccountNum) <= 19)
			{
				sprintf(szTmpData, "%02d", strlen(Accept.InAccountNum));
				memcpy(&HMESend.Data[2][0], szTmpData, 2);
				memcpy(&HMESend.Data[2][2], Accept.InAccountNum, strlen(Accept.InAccountNum));
				HMESend.DataLength[2] = strlen(Accept.InAccountNum) + 2;
			}
			else
			{
				sprintf(szTmpData, "%02d", 19);
				memcpy(&HMESend.Data[2][0], szTmpData, 2);
				memcpy(&HMESend.Data[2][2], Accept.InAccountNum, 19);
				HMESend.DataLength[2] = 19 + 2;
			}
		}
		else
		if (TranCode == TC_SCRIPT)
		{
			int nScritpLen = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM"));
			CString strScript = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScritpLen));
			CStringArray strScriptArray;
			if (strScript != "")
			{
				SplitString(strScript, ",", strScriptArray);
				sprintf(szTmpData, "%02d", strlen(strScriptArray[5].GetBuffer(0)));
				memcpy(&HMESend.Data[2][0], szTmpData, 2);
				memcpy(&HMESend.Data[2][2], strScriptArray[5].GetBuffer(0), strlen(strScriptArray[5].GetBuffer(0)));
				HMESend.DataLength[2] = strlen(strScriptArray[5].GetBuffer(0)) + 2;
			}
		}
		else
		if (TranCode == TC_CARDRETAIN)
		{
			sprintf(szTmpData, "%02d", CardRetainData.TranAccountLen);
			memcpy(&HMESend.Data[2][0], szTmpData, 2);
			memcpy(&HMESend.Data[2][2], CardRetainData.TranAccount, CardRetainData.TranAccountLen);
			HMESend.DataLength[2] = CardRetainData.TranAccountLen + 2;
		}
		else
		{
			if (strlen(CardAccountNo) <= 19)
			{
				sprintf(szTmpData, "%02d", strlen(CardAccountNo));
				memcpy(&HMESend.Data[2][0], szTmpData, 2);
				memcpy(&HMESend.Data[2][2], CardAccountNo, strlen(CardAccountNo));
				HMESend.DataLength[2] = strlen(CardAccountNo) + 2;
			}
			else
			{
				sprintf(szTmpData, "%02d", 19);
				memcpy(&HMESend.Data[2][0], szTmpData, 2);
				memcpy(&HMESend.Data[2][2], CardAccountNo, 19);
				HMESend.DataLength[2] = 19 + 2;
			}
		}
	}

	if (strBitMap.GetAt(3) == '1')								// 3 Process Code
	{
		HMESend.DataLength[3] = 6;
		memset(HMESend.Data[3], '0', HMESend.DataLength[3]);
		switch (TranCode)
		{
			case TC_INQUIRY:
				memcpy(HMESend.Data[3], "100100", HMESend.DataLength[3]);
				break;
			case TC_CUS_INF_INQ:
				memcpy(HMESend.Data[3], "100100", HMESend.DataLength[3]);		//jl20140307
				break;
			case TC_CUS_INF_INQ_OTHERBANK:
				memcpy(HMESend.Data[3], "101000", HMESend.DataLength[3]);
				break;

			case TC_INQUIRY_CheckPin:
				memcpy(HMESend.Data[3], "100100", HMESend.DataLength[3]);		//jl20140307
				break;

			case TC_ECHANNEL_INQ:
				memcpy(HMESend.Data[3], "100700", HMESend.DataLength[3]);
				break;

			case TC_ECHANNEL:
				memcpy(HMESend.Data[3], "141400", HMESend.DataLength[3]);
				break;

			case TC_DETAIL:
				memcpy(HMESend.Data[3], "100200", HMESend.DataLength[3]);
				break;

			case TC_OTHER_INQ:						// yaokq
				memcpy(HMESend.Data[3],"100400",HMESend.DataLength[3]);
				break;
			case TC_WITHDRAWAL:
				memcpy(HMESend.Data[3], "110100", HMESend.DataLength[3]);
				break;

			case TC_CANCEL:	
				if (TranCode2 == 2)							// 2008.10.13
					memcpy(HMESend.Data[3], "180700", HMESend.DataLength[3]);
				else
				if (TranCode2 == 3)
					memcpy(HMESend.Data[3], "111300", HMESend.DataLength[3]);
				else
					memcpy(HMESend.Data[3], "110230", HMESend.DataLength[3]);
				break;

			case TC_TRANSFER:
				memcpy(HMESend.Data[3], "110500", HMESend.DataLength[3]);
				break;

			case TC_LOAD:									// added by yusy ic
				memcpy(HMESend.Data[3], "111100", HMESend.DataLength[3]);
				break;

			case TC_BOUNDMDOIFY:
				memcpy(HMESend.Data[3], "150400", HMESend.DataLength[3]);
				break;
			
			case TC_SCRIPT:
				memcpy(HMESend.Data[3], "161100", HMESend.DataLength[3]);
				break;

			case TC_HZT:										//liyi add 绿卡通
			case TC_TZH:
				memcpy(HMESend.Data[3], "110600", HMESend.DataLength[3]);
				break;

			case TC_CHANGEPIN:	
				memcpy(HMESend.Data[3], "150100", HMESend.DataLength[3]);
				break;

			case TC_SMSPARITY:	
				memcpy(HMESend.Data[3], "150500", HMESend.DataLength[3]);
				break;

			case TC_OPEN:
				if (RQKCode == 0)
					memcpy(HMESend.Data[3], "160300", HMESend.DataLength[3]);
				else
					memcpy(HMESend.Data[3], "160400", HMESend.DataLength[3]);
				break;

			case TC_OEXSTATUS:
			case TC_ERROR:
				memcpy(HMESend.Data[3], "160500", HMESend.DataLength[3]);				
				break;

			case TC_MSRSTATUS:
				memcpy(HMESend.Data[3], "160600", HMESend.DataLength[3]);
				break;

			case TC_AEXSTATUS:
				memcpy(HMESend.Data[3], "160700", HMESend.DataLength[3]);
				break;

			case TC_CARDRETAIN:
				memcpy(HMESend.Data[3], "161200", HMESend.DataLength[3]);
				break;

			case TC_DOWNFIT:
				memcpy(HMESend.Data[3], "190200", HMESend.DataLength[3]);
				break;

			case TC_DOWNICFIT:			// added by yusy ic
				memcpy(HMESend.Data[3], "190400", HMESend.DataLength[3]);
				break;

			case TC_DOWNAIDLIST:
				memcpy(HMESend.Data[3], "190600", HMESend.DataLength[3]);
				break;
				
			case TC_DOWNPARM:
				memcpy(HMESend.Data[3], "190100", HMESend.DataLength[3]);
				break;

			case TC_DOWNPBOCPARA:
				memcpy(HMESend.Data[3], "190500", HMESend.DataLength[3]);
				break;
				
			case TC_DOWNPAY:
				memcpy(HMESend.Data[3], "190300", HMESend.DataLength[3]);
				break;

			case TC_DOWNECHANNELPARA:
				memcpy(HMESend.Data[3], "190700", HMESend.DataLength[3]);
				break;

			case TC_DOWNECHANNELPROT:
				memcpy(HMESend.Data[3], "190800", HMESend.DataLength[3]);
				break;
				

			case TC_ADDCASH:
			case TC_SUBCASH:
			case TC_CLEARCASH:
				memcpy(HMESend.Data[3], "160800", HMESend.DataLength[3]);
				break;

			case TC_TOTAL:
				memcpy(HMESend.Data[3], "170100", HMESend.DataLength[3]);
				break;

			case TC_TOTALPRE:
				memcpy(HMESend.Data[3], "170600", HMESend.DataLength[3]);
				break;

			case TC_TOTALKIND:
				memcpy(HMESend.Data[3], "161300", HMESend.DataLength[3]);
				break;

			case TC_DETAILEND:
			case TC_SENDDETAIL:
				memcpy(HMESend.Data[3], "170200", HMESend.DataLength[3]);
				break;

			case TC_REMIT:									// 2008.10.05
				memcpy(HMESend.Data[3], "180100", HMESend.DataLength[3]);
				break;

			case TC_REMIT_ADD:
			case TC_REMIT_PWD:
				memcpy(HMESend.Data[3], "180200", HMESend.DataLength[3]);
				break;

			case TC_REMIT_INF_INQ:
				memcpy(HMESend.Data[3], "180500", HMESend.DataLength[3]);
				break;

			case TC_REMIT_IN:
				memcpy(HMESend.Data[3], "180600", HMESend.DataLength[3]);
				break;

			case TC_PAY_INQ:
				if( PayType == S_PAY1 )
				{
				  memcpy(HMESend.Data[3], "140100", HMESend.DataLength[3]);
				}
				else
				if( PayType == S_PAY2 )
				{
                  memcpy(HMESend.Data[3], "140500", HMESend.DataLength[3]);
				}
				break;

			case TC_PAY:
                if( PayType == S_PAY1 )
				{
					memcpy(HMESend.Data[3], "140200", HMESend.DataLength[3]);
				}
				else
				if( PayType == S_PAY2 )
				{
                    memcpy(HMESend.Data[3], "140400", HMESend.DataLength[3]);
				}
				break;

//			case TC_ATMOPEN:
//				memcpy(HMESend.Data[3], "200100", HMESend.DataLength[3]);
//				break;
//
//			case TC_ATMCLOSE:
//				memcpy(HMESend.Data[3], "200200", HMESend.DataLength[3]);
//				break;
//
//			case TC_ATMREBOOT:
//				memcpy(HMESend.Data[3], "200300", HMESend.DataLength[3]);
//				break;
//
//			case TC_ATMSTATUS:
//				memcpy(HMESend.Data[3], "200400", HMESend.DataLength[3]);
//				break;
			case TC_ATMOPEN:
			case TC_ATMCLOSE:
			case TC_ATMREBOOT:
			case TC_ATMSTATUS:
			case TC_ATMRESETALL:
			case TC_ATMBIDSENDDETAIL:
			case TC_ATMBIDADDOWNLOAD:
		    case TC_ATMBIDSOFTUPDATE:
				memcpy(HMESend.Data[3], HMERecv.Data[3], HMERecv.DataLength[3]);
				break;

			
			case TC_ATMWORKPARAM:									// CUP(SH) : 2006.04.19
			case TC_ATMFTPSEND:
			case TC_ATMRESETDVR:
				memcpy(HMESend.Data[3], "000000", HMESend.DataLength[3]);
				break;

			case TC_CONFIRMWITH:	
				memcpy(HMESend.Data[3], "000000", HMESend.DataLength[3]);
				break;

			default:
				break;
		}
	}

	if (strBitMap.GetAt(4) == '1')									// 4 Transaction Amount
	{
		HMESend.DataLength[4] = 12;
		memset(HMESend.Data[4], '0', HMESend.DataLength[4]);
		if (TranCode == TC_CANCEL)
		{
//			Sprintf(Accept.Money, 12, "%12.12s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT));
			Sprintf(Accept.Money, 12, "%12.12s", CwcInfo.TranAmount);		// added by liuxl 20110714
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() _INIKEY_TRANSACTION_WITHAMOUNT[%12.12s]", Accept.Money);
			if (IsNum(Accept.Money, sizeof(Accept.Money)))
				memcpy(HMESend.Data[4], Accept.Money, sizeof(Accept.Money));
			else
				memset(Accept.Money, '0', sizeof(Accept.Money));// 2006.05.18
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() CANCEL AMOUNT[%12.12s]", HMESend.Data[4]);
		}
		else
		{
			memcpy(HMESend.Data[4], Accept.Money, HMESend.DataLength[4]);
		}
	}

	if (strBitMap.GetAt(7) == '1')								// 7 Type Flag
	{
		HMESend.DataLength[7] = 2;
		memcpy(HMESend.Data[7], "10", HMESend.DataLength[7]);
	}

	if (strBitMap.GetAt(11) == '1')								// 11 Serial No
	{
		HMESend.DataLength[11] = 8;
		memcpy(HMESend.Data[11], m_pProfile->TRANS.SerialNo, 8);
		
		if (TranCode == TC_WITHDRAWAL)							// Save : 2005.09.11
		{
			memcpy(WithSerialNo, m_pProfile->TRANS.SerialNo, 8);
		}
		else
		if (TranCode == TC_CANCEL)
		{
//			if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWCCOUNT) > 0)
			if (CwcInfo.CwcRetryCnt > 0)						// added by liuxl 20110714
			{
				memset(szTmpData, 0, sizeof(szTmpData));
// 				MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CANCELSERIALNO).GetBuffer(0), 
// 						 szTmpData, 
// 						 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CANCELSERIALNO).GetLength());
				memcpy(szTmpData, CwcInfo.CancelSerialNo, sizeof(CwcInfo.CancelSerialNo));	// added by liuxl 20110714
				if (IsNum(szTmpData, 8))
					memcpy(HMESend.Data[11], szTmpData, HMESend.DataLength[11]);
			}
			else
			{
// 				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CANCELSERIALNO, 
// 						  MakeUnPack(HMESend.Data[11], HMESend.DataLength[11]));
				memcpy(CwcInfo.CancelSerialNo, HMESend.Data[11], HMESend.DataLength[11]);	// added by liuxl 20110714
			}
		}
		else
		if ((TranCode == TC_ATMOPEN)	  	    ||
			(TranCode == TC_ATMCLOSE)	   	    ||
			(TranCode == TC_ATMREBOOT)		    ||
			(TranCode == TC_ATMRESETALL)	    ||
			(TranCode == TC_ATMBIDSENDDETAIL)	||
			(TranCode == TC_ATMBIDADDOWNLOAD)         ||
		    (TranCode == TC_ATMBIDSOFTUPDATE)         ||
			(TranCode == TC_ATMSTATUS))
		{
			memcpy(HMESend.Data[11], HMERecv.Data[11], HMERecv.DataLength[11]);
		}
	}

	GetDateTime(PDate, PTime);
	if (strBitMap.GetAt(12) == '1')								// 12 Time
	{
		HMESend.DataLength[12] = 6;
		memcpy(HMESend.Data[12], PTime, HMESend.DataLength[12]);// 2005.09.11
		if (TranCode == TC_CANCEL)
		{
			memset(szTmpData, 0, sizeof(szTmpData));
// 			MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetBuffer(0), 
// 					 szTmpData, 
// 					 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetLength());

			memcpy(szTmpData, CwcInfo.TranTime, sizeof(CwcInfo.TranTime));	// added by liuxl 20110714
			if (IsNum(szTmpData, HMESend.DataLength[12]))
				memcpy(HMESend.Data[12], szTmpData, HMESend.DataLength[12]);
		}
		else
		if ((TranCode == TC_ATMOPEN)		    ||
			(TranCode == TC_ATMCLOSE)	    	||
			(TranCode == TC_ATMREBOOT)	   	    ||
			(TranCode == TC_ATMRESETALL)	    ||
			(TranCode == TC_ATMBIDSENDDETAIL)	||
			(TranCode == TC_ATMBIDADDOWNLOAD)         ||
		    (TranCode == TC_ATMBIDSOFTUPDATE)         ||
			(TranCode == TC_ATMSTATUS))
		{
			memcpy(HMESend.Data[12], HMERecv.Data[12], HMERecv.DataLength[12]);
		}
	}

	if (strBitMap.GetAt(13) == '1')								// 13 Date
	{
		HMESend.DataLength[13] = 8;
		memcpy(HMESend.Data[13], PDate, HMESend.DataLength[13]);
		if (TranCode == TC_CANCEL)
		{
			memset(szTmpData, 0, sizeof(szTmpData));
// 			MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetBuffer(0), 
// 					 szTmpData, 
// 					 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetLength());

			memcpy(szTmpData, CwcInfo.TranDate, sizeof(CwcInfo.TranDate));	// added by liuxl 20110714
			if (IsNum(szTmpData, HMESend.DataLength[13]))
				memcpy(HMESend.Data[13], szTmpData, HMESend.DataLength[13]);
		}
		else
		if ((TranCode == TC_ATMOPEN)		   ||
			(TranCode == TC_ATMCLOSE)	  	   ||
			(TranCode == TC_ATMREBOOT)		   ||
			(TranCode == TC_ATMRESETALL)	   ||
			(TranCode == TC_ATMBIDSENDDETAIL)  ||
			(TranCode == TC_ATMBIDADDOWNLOAD)         ||
		    (TranCode == TC_ATMBIDSOFTUPDATE)         ||
			(TranCode == TC_ATMSTATUS))
		{
			memcpy(HMESend.Data[13], HMERecv.Data[13], HMERecv.DataLength[13]);
		}
	}

	if (strBitMap.GetAt(14) == '1')								// 14 Card Expire Date
	{
		HMESend.DataLength[14] = HMERecv.DataLength[14];
		memcpy(HMESend.Data[14], HMERecv.Data[14], HMESend.DataLength[14]);
	}

	if (strBitMap.GetAt(15) == '1')								// 15 Settlement Date
	{
		HMESend.DataLength[15] = HMERecv.DataLength[15];
		memcpy(HMESend.Data[15], HMERecv.Data[15], HMESend.DataLength[15]);
	}

	if (strBitMap.GetAt(16) == '1')								// 16 liyi add 绿卡通
	{
		HMESend.DataLength[16] = 2;
		memset(HMESend.Data[16], '0', HMESend.DataLength[16]);
		if (TranCode == TC_HZT)
		{
			memcpy(HMESend.Data[16], m_pTranCmn->strTFRInData.GetBuffer(0), HMESend.DataLength[16]);
		}
		if (TranCode == TC_TZH)
		{
			memcpy(HMESend.Data[16], "00", HMESend.DataLength[16]);
		}		
	}	
	if (strBitMap.GetAt(20) == '1')								// 20 Transaction Flag
	{
		HMESend.DataLength[20] = 8;
		memset(HMESend.Data[20], '0', HMESend.DataLength[20]);
		HMESend.Data[20][0] = '2';
		memcpy(&HMESend.Data[20][1], m_pTranCmn->AccountType.GetBuffer(0), 1);

		if ((TranCode == TC_CUS_INF_INQ)||
			(TranCode == TC_CUS_INF_INQ_OTHERBANK))
		{
			memset(&HMESend.Data[20][2], '1', 1);
			if (Accept.InAccountNum[0] == '6' && Accept.InAccountNum[1] == '0')
				HMESend.Data[20][0] = '1';
			else
				HMESend.Data[20][0] = '2';
		}
		else
		if (TranCode == TC_TRANSFER)
		{
			memset(&HMESend.Data[20][2], '1', 1);
			if (Accept.InAccountNum[0] == '6' && Accept.InAccountNum[1] == '0')
			{
				m_pTranCmn->strTFRInaccountType = "1";
			}
			else
			{
				m_pTranCmn->strTFRInaccountType = "2";
			}
			memcpy(&HMESend.Data[20][3], m_pTranCmn->strTFRInaccountType.GetBuffer(0), 1);
		}
		else
		if (TranCode == TC_HZT)									// liyi add 绿卡通
		{
			memcpy(&HMESend.Data[20][2], m_pTranCmn->strTFRSubType.GetBuffer(0), 1);
		}
		else
		if (TranCode == TC_TZH)
		{
			memset(&HMESend.Data[20][2], '2', 1);
		}
		else
		if (TranCode == TC_REMIT_INF_INQ || TranCode == TC_REMIT_IN)
		{
			memset(&HMESend.Data[20][4], '1', 1);
		}
		else
		{
			memset(&HMESend.Data[20][2], '0', 1);
			memset(&HMESend.Data[20][3], '0', 1);
		}
		if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
			(m_pDevCmn->McuReadMSOnlyFlag))
			;
		else
		{
			memset(&HMESend.Data[20][5], '1', 1);
		}
	}

	if (strBitMap.GetAt(21) == '1')								// 21 Add Cash Information
	{
		fnAPP_GetAtmAddCashInfo();
		HMESend.DataLength[21] = 36;
		memcpy(HMESend.Data[21], &AtmAddCashInfo, 36);
	}

	if (strBitMap.GetAt(22) == '1')								// 22 Entry Mode Code
	{
		HMESend.DataLength[22] = 3;
		if (TranCode == TC_REMIT_INF_INQ)						// liyi add 2008.10.17 汇票信息查询交易，密码加密方式
		{
			memcpy(HMESend.Data[22], "002", HMESend.DataLength[22]);
		}
		else
		if((TranCode == TC_CUS_INF_INQ)||
			(TranCode == TC_CUS_INF_INQ_OTHERBANK))
		{
			memcpy(HMESend.Data[22], "012", HMESend.DataLength[22]);
		}
//		else
//		if (TranCode == TC_CARDRETAIN)
//		{
//			if (CardRetainData.TranType == '1')
//				memcpy(HMESend.Data[22], "051", HMESend.DataLength[22]);
//			else
//				memcpy(HMESend.Data[22], "021", HMESend.DataLength[22]);
//		}
		else
		{
			if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
				(m_pDevCmn->McuReadMSOnlyFlag))
				memcpy(HMESend.Data[22], "021", HMESend.DataLength[22]);
			else
			{
				memcpy(HMESend.Data[22], "051", HMESend.DataLength[22]);
			}
		}
	}

	if (strBitMap.GetAt(23) == '1')
	{
		memset(szTmpData, 0, sizeof(szTmpData));
		memset(icdata, 0, sizeof(icdata));
		HMESend.DataLength[23] = 3;
		if (TranCode == TC_SCRIPT)
		{
			int nScritpLen = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM"));
			CString strScript = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScritpLen));
			CStringArray strScriptArray;
			if (strScript != "")
			{
				SplitString(strScript, ",", strScriptArray);
				memcpy(HMESend.Data[23], strScriptArray[7].GetBuffer(0), HMESend.DataLength[23]);
			}
		}
		else
		if (TranCode == TC_CANCEL)								// added by liuxl 20110714
			memcpy(HMESend.Data[23], CwcInfo.ICSerialNo, HMESend.DataLength[23]);
		else
		{
			m_pDevCmn->fnPBC_TlvGetValue(0x5F34, (BYTE*)icdata, &nlen);
			sprintf(szTmpData, "%03d", atoi(MakeUnPack(icdata, nlen)));
			memcpy(HMESend.Data[23], szTmpData, HMESend.DataLength[23]);
		}
	}

	if (strBitMap.GetAt(24) == '1')								// 24 ATM Status Information
	{
		fnAPP_GetAtmStatusInfo();
		HMESend.DataLength[24] = 80;
		memcpy(HMESend.Data[24], &AtmStatusInfo, 80);
	}

	if (strBitMap.GetAt(25) == '1')								// 25 Services Code
	{
		HMESend.DataLength[25] = 2;
		if((TranCode == TC_CUS_INF_INQ)||
			(TranCode == TC_CUS_INF_INQ_OTHERBANK))
			memcpy(HMESend.Data[25], "15", HMESend.DataLength[25]);
		else
			memcpy(HMESend.Data[25], "00", HMESend.DataLength[25]);
	}

	if (strBitMap.GetAt(35) == '1')								// 35 Track2
	{
		memset(szTmpData, 0, sizeof(szTmpData));
		if (CardData.ISO2size <= 37)
		{
			sprintf(szTmpData, "%02d", CardData.ISO2size);
			memcpy(&HMESend.Data[35][0], szTmpData, 2);
			memcpy(&HMESend.Data[35][2], CardData.ISO2Buff, CardData.ISO2size);
			HMESend.DataLength[35] = CardData.ISO2size + 2;

		}
		else 
		{
			sprintf(szTmpData, "%02d", 37);
			memcpy(&HMESend.Data[35][0], szTmpData, 2);
			memcpy(&HMESend.Data[35][2], CardData.ISO2Buff, 37);
			HMESend.DataLength[35] = 37 + 2;
		}
	}

	if (strBitMap.GetAt(36) == '1')								// 36 Track3
	{
		memset(szTmpData, 0, sizeof(szTmpData));
		if (CardData.ISO3size <= 104)
		{
			sprintf(szTmpData, "%03d", CardData.ISO3size);
			memcpy(&HMESend.Data[36][0], szTmpData, 3);
			memcpy(&HMESend.Data[36][3], CardData.ISO3Buff, CardData.ISO3size);
			HMESend.DataLength[36] = CardData.ISO3size + 3;
		}
		else 
		{
			sprintf(szTmpData, "%03d", 104);
			memcpy(&HMESend.Data[36][0], szTmpData, 3);
			memcpy(&HMESend.Data[36][3], CardData.ISO3Buff, 104);
			HMESend.DataLength[36] = 104 + 3;
		}
	}
	
	if (strBitMap.GetAt(37) == '1')								// 37 Reference No
	{
		HMESend.DataLength[37] = 12;
		memset(HMESend.Data[37], '0', HMESend.DataLength[37]);	// 2005.09.08
		if (TranCode == TC_CANCEL)
		{
			memset(szTmpData, 0, sizeof(szTmpData));
			MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO).GetBuffer(0), 
					 szTmpData, 
					 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO).GetLength());
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() _INIKEY_TRANSACTION_REFERENCENO[%12.12s]", szTmpData);
			if (strlen(szTmpData) >= 12)
			{
				memcpy(HMESend.Data[37], szTmpData, HMESend.DataLength[37]);
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() CANCEL REFERENCE NO[%12.12s]", HMESend.Data[37]);
		}
	}

	if (strBitMap.GetAt(39) == '1')								// 39 Response No
	{
		HMESend.DataLength[39] = 2;
		memcpy(HMESend.Data[39], "00", HMESend.DataLength[39]);
	}

	if (strBitMap.GetAt(41) == '1')								// 41 Atm Serial Number
	{
		HMESend.DataLength[41] = 8;
		memcpy(HMESend.Data[41], m_pProfile->NETWORK.AtmSerialNum, HMESend.DataLength[41]);
		if ((TranCode == TC_ATMOPEN)		   ||
			(TranCode == TC_ATMCLOSE)	   	   ||
			(TranCode == TC_ATMREBOOT)		   ||
			(TranCode == TC_ATMRESETALL)	   ||
			(TranCode == TC_ATMBIDSENDDETAIL)  ||
			(TranCode == TC_ATMBIDADDOWNLOAD)  ||
		    (TranCode == TC_ATMBIDSOFTUPDATE)  ||
			(TranCode == TC_ATMSTATUS))
		{
			memcpy(HMESend.Data[41], HMERecv.Data[41], HMERecv.DataLength[41]);
		}
	}

	if (strBitMap.GetAt(42) == '1')								// 42 Card Acceptor ID Code
	{
		HMESend.DataLength[42] = 15;
		memset(HMESend.Data[42], ' ', HMESend.DataLength[42]);
		strTemp = IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CARDACCEPTORID);
		if (strTemp.GetLength() <= 15)
			memcpy(HMESend.Data[42], strTemp.GetBuffer(0), strTemp.GetLength());
		else
			memcpy(HMESend.Data[42], strTemp.GetBuffer(0), HMESend.DataLength[42]);
	}
	if (strBitMap.GetAt(45) == '1')								// 14 Card Expire Date
	{
		HMESend.DataLength[45] = HMERecv.DataLength[45];
		memcpy(HMESend.Data[45], HMERecv.Data[45], HMESend.DataLength[45]);
	}

	if (strBitMap.GetAt(49) == '1')								// 49 Currency Code
	{
		HMESend.DataLength[49] = 3;
		memcpy(HMESend.Data[49], "156", HMESend.DataLength[49]);
	}

	if (strBitMap.GetAt(51) == '1')								// 51 ATM Counter Number
	{
		HMESend.DataLength[51] = 7;
		memcpy(HMESend.Data[51], szADDCashCounterNumber, HMESend.DataLength[51]);
	}

	if (strBitMap.GetAt(52) == '1')								// 52 Pin Data     by  zjd
	{
		HMESend.DataLength[52] = 8;
		if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
		{
			memset(szTmpData, 0, sizeof(szTmpData));
			MakePack(Accept.PassWord, szTmpData, sizeof(Accept.PassWord));
			memcpy(HMESend.Data[52], szTmpData, HMESend.DataLength[52]);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PINKEY[%16.16s]", szTmpData);
		}
		else
		{
			char PinBlock[16];
			char PinBlock2[16];
			memset(PinBlock, '0', sizeof(PinBlock));
			if (strlen(CardAccountNo) >=13)
				memcpy(PinBlock + 4, &CardAccountNo[strlen(CardAccountNo) - 13], 12);
			else
				memcpy(PinBlock + 16 - strlen(CardAccountNo), CardAccountNo, strlen(CardAccountNo));
			
//			if (iDEBUGFLAG)
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PinBlock1[%16.16s]", PinBlock);
			MakePack(PinBlock, PinBlock2, 16);
			
//			if (iDEBUGFLAG)
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PinBlock2[%16.16s]", Accept.PassWord);
			MakePack(Accept.PassWord, PinBlock, 16);
			
			for (int i = 0; i<8; i++)
				PinBlock[i] ^= PinBlock2[i];
//			if (iDEBUGFLAG)
//			{
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PinBlock[%16.16s]", MakeUnPack(PinBlock, 8));
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PINKEY[%32.32s]", MakeUnPack(SavePINKEY, 16));
//			}
			
			DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
//			if (iDEBUGFLAG)
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() Step1[%16.16s]", MakeUnPack(PinBlock, 8));
			
			DesEncryptDecrypt((UCHAR *)(SavePINKEY + 8), (UCHAR *)PinBlock, 0);
//			if (iDEBUGFLAG)
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() Step2[%16.16s]", MakeUnPack(PinBlock, 8));
			
			DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
			if (iDEBUGFLAG)
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() Step3[%16.16s]", MakeUnPack(PinBlock, 8));
			
			memcpy(HMESend.Data[52], PinBlock, HMESend.DataLength[52]);
		}
	}
	
	if (strBitMap.GetAt(53) == '1')								// 53 Control Information
	{
		HMESend.DataLength[53] = 16;
		memset(HMESend.Data[53], '0', HMESend.DataLength[53]);
		if(TranCode == TC_OPEN)
		{
			if (RQKCode == 0)
			{
				memcpy(HMESend.Data[53], "3062", 4);
			}
			else if (RQKCode == 1)
			{
				memcpy(HMESend.Data[53], "1061", 4);
			}
			else if (RQKCode == 2)
			{
				memcpy(HMESend.Data[53], "2062", 4);
			}
		}
		else
		{
			memcpy(HMESend.Data[53], "11206", 5);
		}
	}

	if (strBitMap.GetAt(55) == '1')	
	{
		int len = 0;
		memset(szTmpData, 0, sizeof(szTmpData));
		memset(szTmpData1, 0, sizeof(szTmpData1));
		
		if (TranCode == TC_SCRIPT)
		{
			int nScritpLen = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM"));
			CString strScript = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScritpLen));
			CStringArray strScriptArray;
			if (strScript != "")
			{
				SplitString(strScript, ",", strScriptArray);
				len = MakePack(strScriptArray[6].GetBuffer(0), szTmpData, strScriptArray[6].GetLength());
			}
		}
		else
		if (TranCode == TC_CANCEL)				// added by liuxl 20110508 20110714
		{
// 			CString strICData = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55);
// 			if (strICData != "")
// 				len = MakePack(strICData.GetBuffer(0), szTmpData, strICData.GetLength());
			len = CwcInfo.ICCwcDataLen;
			memcpy(szTmpData, CwcInfo.ICCwcData, len);
		}
		else
		{
			fnAPD_PbcReadIcData(szTmpData, &len);
		}

		sprintf(szTmpData1, "%03d", len);
		memcpy(&HMESend.Data[55][0], szTmpData1, 3);
		memcpy(&HMESend.Data[55][3], szTmpData, len);
		HMESend.DataLength[55] = len + 3;
	}

	if (strBitMap.GetAt(59) == '1')
	{
		memset(szTmpData, 0, sizeof(szTmpData));
//		HMESend.DataLength[59] = 9;
		HMESend.DataLength[59] = 10;
		sprintf(szTmpData, "%03d", HMESend.DataLength[59]);
		memcpy(&HMESend.Data[59][0], szTmpData, 3);
//		memcpy(&HMESend.Data[59][3], "000005000", HMESend.DataLength[59]);
		memcpy(&HMESend.Data[59][3], "0000050001", HMESend.DataLength[59]);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fallback_host[%d]", FallBack);
		if (FallBack)						// added by liuxl 20110505
			memcpy(&HMESend.Data[59][3], "0000052001", HMESend.DataLength[59]);

//		HMESend.DataLength[59] = 12;
		HMESend.DataLength[59] = 13;

		if (TranCode == TC_CANCEL)
		{
			memset(szTmpData1, 0, sizeof(szTmpData1));
			sprintf(szTmpData1, "%03d", CwcInfo.TranExpInfoLen);
			memcpy(&HMESend.Data[59][0], szTmpData1, 3);
			memcpy(&HMESend.Data[59][3], CwcInfo.TranExpInfo, CwcInfo.TranExpInfoLen);
			HMESend.DataLength[59] = CwcInfo.TranExpInfoLen + 3;
		}
	}

	if (strBitMap.GetAt(60) == '1')								// 60 Reserved
	{
		HMESend.DataLength[60] = 4;
		if (TranCode == TC_CANCEL)
		{
			memset(HMESend.Data[60], ' ', HMESend.DataLength[60]);
//			memcpy(HMESend.Data[60], fnAPL_GetBankErrorCode(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR)).GetBuffer(0), 4);
//			if (memcmp(HMESend.Data[60], "????", 4) == 0)	// 2006.04.19
				memcpy(HMESend.Data[60], "0004", 4);
			if (TranCode2 == 3)
				memcpy(HMESend.Data[60], "4351", 4);
		}
		else
		if (TranCode == TC_AEXSTATUS)
		{
			if (HostNms.AexCustomerCardTimeOutSendFlag)
				memcpy(HMESend.Data[60], "0006", 4);
			else if (HostNms.AexCustomerCashTimeOutSendFlag)
				memcpy(HMESend.Data[60], "0007", 4);
			else if (HostNms.AexCustomerHostRetractSendFlag)
				memcpy(HMESend.Data[60], "0008", 4);
			else
				memcpy(HMESend.Data[60], "0000", 4);
		}
		else
		{
			memset(HMESend.Data[60], ' ', HMESend.DataLength[60]);
			memcpy(HMESend.Data[60], "0000", 4);
		}
	}
	
	if (strBitMap.GetAt(61) == '1')								// 63 ID Information 2008.10.05
	{
		HMESend.DataLength[61] = 2;
		memcpy(HMESend.Data[61], "00", HMESend.DataLength[61]);
		if (TranCode == TC_ECHANNEL)
		{
			HMESend.DataLength[61] = Asc2Int(Accept.RecvData61, 2) + 2;
			memset(HMESend.Data[61], '0', HMESend.DataLength[61]);
			memcpy(HMESend.Data[61], Accept.RecvData61, HMESend.DataLength[61]);
		}
	}	

	if (strBitMap.GetAt(62) == '1')								// 62 Pin Data     by  zjd
	{
		HMESend.DataLength[62] = 8;
		if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
		{
			memset(szTmpData, 0, sizeof(szTmpData));
			MakePack(Accept.NewPassWord, szTmpData, sizeof(Accept.NewPassWord));
			memcpy(HMESend.Data[62], szTmpData, HMESend.DataLength[62]);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PINKEY[%16.16s]", szTmpData);
		}
		else
		{
			char PinBlock[16];
			char PinBlock2[16];
			memset(PinBlock, '0', sizeof(PinBlock));
			if (strlen(CardAccountNo) >=13)
				memcpy(PinBlock + 4, &CardAccountNo[strlen(CardAccountNo) - 13], 12);
			else
				memcpy(PinBlock + 16 - strlen(CardAccountNo), CardAccountNo, strlen(CardAccountNo));
			MakePack(PinBlock, PinBlock2, 16);
			MakePack(Accept.NewPassWord, PinBlock, 16);
			for (int i = 0; i<8; i++)
				PinBlock[i] ^= PinBlock2[i];
			DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
			DesEncryptDecrypt((UCHAR *)(SavePINKEY + 8), (UCHAR *)PinBlock, 0);
			DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
			memcpy(HMESend.Data[62], PinBlock, HMESend.DataLength[62]);
		}
	}
	
	if (strBitMap.GetAt(63) == '1')								// 63 ID Information
	{
		HMESend.DataLength[63] = 2;
		memcpy(HMESend.Data[63], "00", HMESend.DataLength[63]);
		if (TranCode == TC_ECHANNEL)
		{
			HMESend.DataLength[63] = Asc2Int(Accept.RecvData63, 2) + 2;
			memset(HMESend.Data[63], '0', HMESend.DataLength[63]);
			memcpy(HMESend.Data[63], Accept.RecvData63, HMESend.DataLength[63]);
		}
	}

	if (strBitMap.GetAt(68) == '1')								// 63 Ticket Information   票据号码  2008.10.05
	{
		memset(szTmpData, 0, sizeof(szTmpData));
		sprintf(szTmpData, "%02d", m_pTranCmn->strRemitInNo.GetLength());
		memcpy(&HMESend.Data[68][0], szTmpData, 2);
		memcpy(&HMESend.Data[68][2], m_pTranCmn->strRemitInNo, m_pTranCmn->strRemitInNo.GetLength());
		HMESend.DataLength[68] = m_pTranCmn->strRemitInNo.GetLength() + 2;
	}

	if (strBitMap.GetAt(72) == '1')								// 72 Batch Number
	{
		HMESend.DataLength[72] = 4;
		memcpy(HMESend.Data[72],
			GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001").GetBuffer(0)),
			HMESend.DataLength[72]);
	}

	if (strBitMap.GetAt(90) == '1')								// 90 Original Data
	{
		HMESend.DataLength[90] = 40;
		memset(HMESend.Data[90], '0', HMESend.DataLength[90]);
		memset(szTmpData, 0, sizeof(szTmpData));
		if (TranCode == TC_SCRIPT)
		{
			int nScritpLen = Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM"));
			CString strScript = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, GetSprintf("SCRIPT%d", nScritpLen));
			CStringArray strScriptArray;
			if (strScript != "")
			{
				SplitString(strScript, ",", strScriptArray);
				memcpy(&HMESend.Data[90][0], strScriptArray[0].GetBuffer(0), 6);
				memcpy(&HMESend.Data[90][6], strScriptArray[1].GetBuffer(0), 8);
				memcpy(&HMESend.Data[90][14], strScriptArray[2].GetBuffer(0), 6);
				memcpy(&HMESend.Data[90][20], strScriptArray[3].GetBuffer(0), 8);
				memcpy(&HMESend.Data[90][28], strScriptArray[4].GetBuffer(0), 12);
			}
		}
		else
		{
			memcpy(HMESend.Data[90], &CwcInfo, HMESend.DataLength[90]);				// added by liuxl 20110714

// 			MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANKIND).GetBuffer(0), 
// 					 szTmpData, 
// 					 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANKIND).GetLength());
// 			memcpy(HMESend.Data[90], szTmpData, 6);				//原交易处理码   by zjd
// 			
// 			memset(szTmpData, 0, sizeof(szTmpData));
// 			MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetBuffer(0), 
// 					 szTmpData, 
// 					 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetLength());
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() ORIGINAL FIELD13-12[%14.14s]", szTmpData);
// 			if (IsNum(szTmpData, 14))
// 				memcpy(&HMESend.Data[90][6], szTmpData, 14);	//原交易日期、时间
// 
// 			memset(szTmpData, 0, sizeof(szTmpData));
// 			memset(OrgSerialNo, '0', sizeof(OrgSerialNo));
// 			Sprintf(szTmpData, 8, "%8.8s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO));
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() ORIGINAL FIELD11[%8.8s]", szTmpData);
// 			if (IsNum(szTmpData, 8))
// 			{
// 				memcpy(&HMESend.Data[90][20], szTmpData, sizeof(OrgSerialNo));
// 				memcpy(OrgSerialNo, szTmpData, sizeof(OrgSerialNo));
// 			}													//原交易流水号
// 
// 			memset(szTmpData, 0, sizeof(szTmpData));
// 			Sprintf(szTmpData, 12, "%12.12s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT));
// 	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() _INIKEY_TRANSACTION_WITHAMOUNT[%12.12s]", szTmpData);
// 			if (IsNum(szTmpData, 12))
// 				memcpy(&HMESend.Data[90][28], szTmpData, 12);	//原交易金额
		}
	}

	if (strBitMap.GetAt(96) == '1')								// 96 secret code  初始化CDK时使用    by zjd
	{
// 		strMasterKey = GetShareData("OP_MASTERKEY");			//长度为32，两倍长
// 		if (strMasterKey == "")
// 		{
// 			strMasterKey = "00000000000000000000000000000000";
// 			SetShareData("OP_MASTERKEY", strMasterKey);
// 		}
// 		HMESend.DataLength[96] = 18;
// 		MakePack(strMasterKey.GetBuffer(0), MasterKey, 32);
// 		memset(HMESend.Data[96], '0', HMESend.DataLength[96]);
// 		memcpy(HMESend.Data[96], "16", 2);
// 		memset(MasterKeyCHK, '0', sizeof(MasterKeyCHK));
// 		for (int i = 0; i < 4; i++)
// 		{
// 			MasterKeyCHK[i] = MasterKey[i] ^ MasterKey[i + 4];
// 			MasterKeyCHK[i + 8] = MasterKey[i + 8] ^ MasterKey[i + 12];
// 		}
// 		memcpy(&HMESend.Data[96][2], MasterKeyCHK, 16);
		char PinBlock[8];
		if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
		{
			char PinBlock2[16];
			memset(PinBlock, '0', sizeof(PinBlock));
			memset(PinBlock2, '0', sizeof(PinBlock2));
			if(RQKCode2 == 0)
			{
				m_pDevCmn->fnPIN_EncryptECB("TRIDESECB", TCONSTMASTERKEY, "", 15, ZERO16);
			}
			else
			{
				m_pDevCmn->fnPIN_EncryptECB("TRIDESECB", TMASTERKEY, "", 15, ZERO16);
			}
			memcpy(PinBlock2, m_pDevCmn->fnPIN_GetEncryptedData(), 16);
			MakePack(PinBlock2, PinBlock, 16, 1);
		}
		else
		{
			char MasterKey[16];
			memset(&PinBlock, 0x00, sizeof(PinBlock));
			strMasterKey = GetShareData("OP_MASTERKEY");			//长度为32，两倍长
			if (strMasterKey == "")
			{
				strMasterKey = "00000000000000000000000000000000";
				SetShareData("OP_MASTERKEY", strMasterKey);
 			}
			MakePack(strMasterKey.GetBuffer(0), MasterKey, 32);
			DesEncryptDecrypt((UCHAR *)MasterKey, (UCHAR *)PinBlock, 1);
			DesEncryptDecrypt((UCHAR *)(MasterKey + 8), (UCHAR *)PinBlock, 0);
			DesEncryptDecrypt((UCHAR *)MasterKey, (UCHAR *)PinBlock, 1);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SoftEncryptedData():return(%s)", PinBlock);
		}
		
		HMESend.DataLength[96] = 10;
		memcpy(HMESend.Data[96], "08", 2);
		memcpy(&HMESend.Data[96][2], PinBlock, 8);
	}
	
	if (strBitMap.GetAt(102) == '1')							// 102 Transfer Card Account No
	{
		if (TranCode == TC_HZT)									// liyi add 绿卡通
		{
			//HMESend.DataLength[102] = 4;
			//memcpy(&HMESend.Data[102], "0001", HMESend.DataLength[102]);
			memcpy(&HMESend.Data[102][0], "04", 2);
			memcpy(&HMESend.Data[102][2], "0001", 4);
			HMESend.DataLength[102] = 4 + 2;
		}
		else if (TranCode == TC_TZH)									
		{
			//HMESend.DataLength[102] = 4;
			memcpy(&HMESend.Data[102][0], "04", 2);
			memcpy(&HMESend.Data[102][2], m_pTranCmn->strTFRSubNo, 4);
			HMESend.DataLength[102] = 4 + 2;
		}
		else
		{
			HMESend.DataLength[102] = HMESend.DataLength[2];
			memcpy(HMESend.Data[102], HMESend.Data[2], HMESend.DataLength[2]);
		}
	}

	if (strBitMap.GetAt(103) == '1')							// 103 To Card No
	{
		if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD)				// 2008.10.05
		{
			memset(szTmpData, 0, sizeof(szTmpData));
			sprintf(szTmpData, "%02d", m_pTranCmn->strRemitAddNo.GetLength());
			memcpy(&HMESend.Data[103][0], szTmpData, 2);
			memcpy(&HMESend.Data[103][2], m_pTranCmn->strRemitAddNo, m_pTranCmn->strRemitAddNo.GetLength());
			HMESend.DataLength[103] = 2 + m_pTranCmn->strRemitAddNo.GetLength();
//			HMESend.DataLength[103] = 28;
//			memset(HMESend.Data[103], '0', HMESend.DataLength[103]);
//			memcpy(HMESend.Data[103], GetSprintf("%28.28s", m_pTranCmn->strRemitAddNo.GetBuffer(0)),HMESend.DataLength[103]);
			
		}
		else
		{
			memset(szTmpData, 0, sizeof(szTmpData));
			sprintf(szTmpData, "%02d", Accept.InAccountNumSize);
			memcpy(&HMESend.Data[103][0], szTmpData, 2);
			memcpy(&HMESend.Data[103][2], Accept.InAccountNum, Accept.InAccountNumSize);
			HMESend.DataLength[103] = 2 + Accept.InAccountNumSize;
		}
	}

	if (strBitMap.GetAt(106) == '1')							//汇票附加标志    2008.10.05
	{
		HMESend.DataLength[106] = 16;
		memset(HMESend.Data[106], '0', HMESend.DataLength[106]);
		if (TranCode == TC_REMIT)
		{
			if (m_pTranCmn->RemitType == S_REMIT_ADD)
			{
				memcpy(HMESend.Data[106], "00", 2);
			}
			else
			if (m_pTranCmn->RemitType == S_REMIT_PWD)
			{
				memcpy(HMESend.Data[106], "01", 2);
			}
		}
		else
		if (TranCode == TC_REMIT_ADD)
		{
			memcpy(HMESend.Data[106], "00", 2);
		}
		else
		if (TranCode == TC_REMIT_PWD)
		{
			memcpy(HMESend.Data[106], "01", 2);
		}	
	}
	
	if (strBitMap.GetAt(120) == '1')								// 120 附加信息  ans999
	{
		if (TranCode == TC_DOWNFIT		||
			TranCode == TC_DOWNICFIT)
		{
			HMESend.DataLength[120] = 6;
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(HMESend.Data[120], "003", 3);
			memcpy(&HMESend.Data[120][3], GetSprintf("%03.3d", DownloadCurrNum + 1).GetBuffer(0), 3);		// 注意起始索引号是否为1开始  by zjd
		}
		else if (TranCode == TC_DOWNECHANNELPROT)
		{
			CStringArray strEchannelItemsArray;
			SplitString(CString(ECHANNELContent_POST[DownloadEchannelProtCurrNum]), ",", strEchannelItemsArray);

			HMESend.DataLength[120] = 20;
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(HMESend.Data[120], "017", 3);
			memcpy(&HMESend.Data[120][3], strEchannelItemsArray[0], 3);
			memcpy(&HMESend.Data[120][6], strEchannelItemsArray[2], 8);
			memcpy(&HMESend.Data[120][14], GetSprintf("%06.6d", DownloadCurrNum).GetBuffer(0), 6);
		}
		else if (TranCode == TC_SMSPARITY)
		{
			HMESend.DataLength[120] = 14;
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(HMESend.Data[120], "011", 3);
			memcpy(&HMESend.Data[120][3], Accept.CellPhoneNo, 11);
		}
		else if ((TranCode == TC_SENDDETAIL) || (TranCode == TC_DETAILEND))
		{
			HMESend.DataLength[120] =  3 + 16 + strDetailData.GetLength();
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(&HMESend.Data[120][0], GetSprintf("%03d", 16 + strDetailData.GetLength()), 3);
			memcpy(&HMESend.Data[120][3], m_pProfile->NETWORK.AtmSerialNum, 8);
			memcpy(&HMESend.Data[120][11], GetSprintf("%03.3d", IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCURRCNT, 0)), 3);
			if (fnAPL_GetDetailCount() > 0)
				memcpy(&HMESend.Data[120][14], "1", 1);
			else
				memcpy(&HMESend.Data[120][14], "0", 1);
			memcpy(&HMESend.Data[120][15],
				GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILBATCHNO, "0001").GetBuffer(0)), 4);
			memcpy(&HMESend.Data[120][19], strDetailData, strDetailData.GetLength());
		}
		else
		if (TranCode == TC_CARDRETAIN)
		{
			HMESend.DataLength[120] =  20;
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(HMESend.Data[120], "017", 3);
			memcpy(&HMESend.Data[120][3], &CardRetainData, 17);
		}
		else
		if (TranCode == TC_TOTALKIND)
		{
			HMESend.DataLength[120] =  3 + 9;
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(HMESend.Data[120], "009", 3);
			if (totalKindFlag == '0')
				memcpy(&HMESend.Data[120][3], "1", 1);
			else
				memcpy(&HMESend.Data[120][3], "2", 1);
			memcpy(&HMESend.Data[120][4], HMESend.Data[13], HMESend.DataLength[13]);
		}
		else
		{
			HMESend.DataLength[120] = 3;
			memset(HMESend.Data[120], '0', HMESend.DataLength[120]);
			memcpy(HMESend.Data[120], "000", 3);
		}
	}
	
	if (strBitMap.GetAt(121) == '1')								// 121 对帐信息  ans999
	{
// 		fnAPP_GetAtmTotalInfo();
// 		char	TempAmount[12];
// 		memset(TempAmount,0,sizeof(TempAmount));
// 		if(TranCode == TC_TOTAL)
// 		{
// 			
// 		    memcpy(TempAmount,AtmTotalInfo.RemainAmount,12);
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TempAmount[%s]",TempAmount);
//             memset(AtmTotalInfo.RemainAmount,0,sizeof(AtmTotalInfo.RemainAmount));
// 			memcpy(AtmTotalInfo.RemainAmount,InputRemainAmount,12);
// 			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InputRemainAmount[%12.12s]",InputRemainAmount);
// 		}
// 		HMESend.DataLength[121] = 117;
// 		memset(HMESend.Data[121], '0', HMESend.DataLength[121]);
// 		memcpy(HMESend.Data[121], "114", 3);
// 		memcpy(&HMESend.Data[121][3], &AtmTotalInfo, 114);

		HMESend.DataLength[121] = 27;
		memset(HMESend.Data[121], '0', HMESend.DataLength[121]);
		memcpy(HMESend.Data[121], "024", 3);
		memcpy(&HMESend.Data[121][3], InputRemainAmount, 12);
		memset(&HMESend.Data[121][15], '0', 12);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InputRemainAmount[%12.12s]",InputRemainAmount);

// 		if(TranCode == TC_TOTAL)
// 		{
//  			memset(AtmTotalInfo.RemainAmount,0,sizeof(AtmTotalInfo.RemainAmount));
// 			memcpy(AtmTotalInfo.RemainAmount,TempAmount,12);
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmTotalInfo.RemainAmount[%12.12s]",AtmTotalInfo.RemainAmount);
// 		}
	}
	
	if (strBitMap.GetAt(122) == '1')								// 122 发起自定义数据  ans999 需要完成   by zjd
	{
		if (TranCode == TC_DETAIL)
		{
			HMESend.DataLength[122] = 22;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			memcpy(HMESend.Data[122], "019", 3);
			memcpy(&HMESend.Data[122][19], "001", 3);				// 明细查询的起始检索号
		}
		else
		if (TranCode == TC_ECHANNEL_INQ)
		{
			HMESend.DataLength[122] = 4;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			memcpy(HMESend.Data[122], "001", 3);
			memcpy(&HMESend.Data[122][3], "1", 1);
		}
		else
		if (TranCode == TC_ECHANNEL)
		{
			int len = 3;

			if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
			{
				memset(szTmpData, 0, sizeof(szTmpData));
				MakePack(Accept.LogInPassWord, szTmpData, sizeof(Accept.LogInPassWord));
				memcpy(&HMESend.Data[122][len], szTmpData, 8);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PINKEY[%16.16s]", szTmpData);
			}
			else
			{
				char PinBlock[16];
				char PinBlock2[16];
				memset(PinBlock, '0', sizeof(PinBlock));
				if (strlen(CardAccountNo) >=13)
					memcpy(PinBlock + 4, &CardAccountNo[strlen(CardAccountNo) - 13], 12);
				else
					memcpy(PinBlock + 16 - strlen(CardAccountNo), CardAccountNo, strlen(CardAccountNo));
				MakePack(PinBlock, PinBlock2, 16);
				MakePack(Accept.LogInPassWord, PinBlock, 16);
				for (int i = 0; i<8; i++)
					PinBlock[i] ^= PinBlock2[i];
				DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
				DesEncryptDecrypt((UCHAR *)(SavePINKEY + 8), (UCHAR *)PinBlock, 0);
				DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
				memcpy(&HMESend.Data[122][len], PinBlock, 8);
			}
			len += 8;

			if (TransPassWordFlag)
			{
				TransPassWordFlag = 0;
				if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				{
					memset(szTmpData, 0, sizeof(szTmpData));
					MakePack(Accept.TransPassWord, szTmpData, sizeof(Accept.TransPassWord));
					memcpy(&HMESend.Data[122][len], szTmpData, 8);
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg() PINKEY[%16.16s]", szTmpData);
				}
				else
				{
					char PinBlock[16];
					char PinBlock2[16];
					memset(PinBlock, '0', sizeof(PinBlock));
					if (strlen(CardAccountNo) >=13)
						memcpy(PinBlock + 4, &CardAccountNo[strlen(CardAccountNo) - 13], 12);
					else
						memcpy(PinBlock + 16 - strlen(CardAccountNo), CardAccountNo, strlen(CardAccountNo));
					MakePack(PinBlock, PinBlock2, 16);
					MakePack(Accept.TransPassWord, PinBlock, 16);
					for (int i = 0; i<8; i++)
						PinBlock[i] ^= PinBlock2[i];
					DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
					DesEncryptDecrypt((UCHAR *)(SavePINKEY + 8), (UCHAR *)PinBlock, 0);
					DesEncryptDecrypt((UCHAR *)SavePINKEY, (UCHAR *)PinBlock, 1);
					memcpy(&HMESend.Data[122][len], PinBlock, 8);
				}
			}
			else
//				memset(&HMESend.Data[122][len], 0, 8);
				memset(&HMESend.Data[122][len], ' ', 8);
			len += 8;

			memcpy(&HMESend.Data[122][len], Accept.CellPhoneNo, 11);
			len += 11;

			memset(&HMESend.Data[122][len], '0', 1);
			len += 1;

			memset(&HMESend.Data[122][len], ' ', 64);
			len += 64;

			memcpy(&HMESend.Data[122][len], &m_pProfile->NETWORK.BranchNum[0], 9);
			len += 9;

			memset(&HMESend.Data[122][len], '1', 1);
			len += 1;

			memset(&HMESend.Data[122][len], '2', 1);
			len += 1;

			memcpy(&HMESend.Data[122][len], Accept.SMSParityCode, 6);
			len += 6;

			memset(&HMESend.Data[122][len], '0', 3);
			len += 3;

			CStringArray strEchannelItemsArray;
			CStringArray strEchannelInqArray;
			int selNum = 0;
			for (int i = 0; i < itemof(ECHANNELContent_POST); i++)
			{
				if (memcmp(ECHANNELContent_POST[i], "AAAAAA", 6) == 0)
					break;
				SplitString(CString(ECHANNELContent_POST[i]), ",", strEchannelItemsArray);
				if (ECHANNELSEL[i])
				{
					for (int j = 0; j < itemof(ECHANNELInq_POST); j++)
					{
						if (memcmp(ECHANNELInq_POST[j], "AAAAAA", 6) == 0)
							break;
						SplitString(CString(ECHANNELInq_POST[j]), ",", strEchannelInqArray);

						if (Asc2Int(strEchannelItemsArray[0], 3) == Asc2Int(strEchannelInqArray[0], 3))
						{
							memcpy(&HMESend.Data[122][len], strEchannelInqArray[0], 3);
							len += 3;
							memcpy(&HMESend.Data[122][len], "1", 1);
							len += 1;
							
							selNum++;
						}
					}

					if (Asc2Int(strEchannelItemsArray[0], 3) == 3)
					{
						memset(&HMESend.Data[122][30], '1', 1);
						memcpy(&HMESend.Data[122][31], Accept.CellPhoneNo, 11);
					}
				}
			}

			if (Asc2Int(&HMESend.Data[122][30], 1) == 1);
			else
			{
				memcpy(&HMESend.Data[122][31], &HMESend.Data[122][95], 20 + selNum * 4);
				len -= 64;
			}
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "selNum[%d]", selNum);
			if (selNum)
				memcpy(&HMESend.Data[122][len - selNum * 4 - 3], GetSprintf("%03d", selNum), 3);

			HMESend.DataLength[122] = len;
			memcpy(&HMESend.Data[122][0], GetSprintf("%03d", len - 3), 3);
		}
		else
		if(TranCode == TC_OTHER_INQ)       // 20080904 by yaokq
		{
			HMESend.DataLength[122] = 22;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			memcpy(&HMESend.Data[122][3], PDate, 8);
			memcpy(&HMESend.Data[122][11], PDate, 8);

			memcpy(HMESend.Data[122], "019", 3);
			char	BeginIndex[4];
			memset(BeginIndex,'0',sizeof(BeginIndex));
			sprintf(BeginIndex,"%.3d",ZzhCurNum+1);
			memcpy(&HMESend.Data[122][19], BeginIndex, 3);
		}
		else
		if (TranCode == TC_REMIT)			// 2008.10.05
		{
			HMESend.DataLength[122] = 9;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);

			memcpy(HMESend.Data[122], "006", 3);
			char	BeginIndex[4];
			memset(BeginIndex,'0',sizeof(BeginIndex));
			sprintf(BeginIndex,"%.3d",ZzhCurNum+1);
			memcpy(&HMESend.Data[122][3], BeginIndex, 3);
			memcpy(&HMESend.Data[122][6], "010", 3);
		}
		else
		if (TranCode == TC_PAY_INQ)								// added by liuxl 20111012
		{
			HMESend.DataLength[122] = 12;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			memcpy(HMESend.Data[122], "009", 3);
			if(PayType == S_PAY1)
				memcpy(&HMESend.Data[122][3], GetSprintf("%03d",stPayJF[PayNum].iPayNum), 3);	// 缴费类型
			else
			if( PayType == S_PAY2)
				memcpy(&HMESend.Data[122][3], GetSprintf("%03d",stPayCZ[PayNum].iPayNum), 3);	// 充值类型  
//			memcpy(&HMESend.Data[122][6], "000000", 6);				// 代缴费的所属市县局号
			memcpy(&HMESend.Data[122][6], &m_pProfile->NETWORK.BranchNum[0], 6);
		
//			memcpy(&HMESend.Data[122][12], strPayInqRefNo.GetBuffer(0), 12);					// added by liyi 20110926
			}
		else
		if (TranCode == TC_PAY)
		{
//			HMESend.DataLength[122] = 12;
			HMESend.DataLength[122] = 24;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
//			memcpy(HMESend.Data[122], "009", 3);
			memcpy(HMESend.Data[122], "021", 3);
			if(PayType == S_PAY1)
			   memcpy(&HMESend.Data[122][3], GetSprintf("%03d",stPayJF[PayNum].iPayNum), 3);	// 缴费类型
			else
			if( PayType == S_PAY2)
               memcpy(&HMESend.Data[122][3], GetSprintf("%03d",stPayCZ[PayNum].iPayNum), 3);	// 充值类型  
//			memcpy(&HMESend.Data[122][6], "000000", 6);				// 代缴费的所属市县局号
			memcpy(&HMESend.Data[122][6], &m_pProfile->NETWORK.BranchNum[0], 6);

			memcpy(&HMESend.Data[122][12], strPayInqRefNo.GetBuffer(0), 12);					// added by liyi 20110926
		}
		else
		if(TranCode == TC_ATMBIDADDOWNLOAD) 		//广告,远程提取流水,软件更新  BY HYL
		{   
			if (memcmp(HMERecv.Data[3], "200600", 6) == 0)
			{
				 HMESend.DataLength[122] = HMERecv.DataLength[122];
				 memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
				 memcpy(HMESend.Data[122], HMERecv.Data[122], HMESend.DataLength[122]);
			}
			else
			if (memcmp(HMERecv.Data[3], "200700", 6) == 0)
			{
				HMESend.DataLength[122] = 28;
				memset(HMESend.Data[122], '0', HMESend.DataLength[123]);
				memcpy(&HMESend.Data[122][0],"025",3);
				memcpy(&HMESend.Data[122][3],&szDownLoadFileName,12);                                  //每次固定下载长度400
				memcpy(&HMESend.Data[122][15],GetSprintf("%08s",Int2Asc(SetBegin)),8);               //起始位置
				memcpy(&HMESend.Data[122][23],&FileEnd,1);                                             //文件长度   BY HYL
				memcpy(&HMESend.Data[122][24],"0400",4);  //每次固定下载长度400
			}
		}
		else
		if(TranCode == TC_ATMBIDSENDDETAIL) 		//广告,远程提取流水,软件更新  BY HYL
		{   
			if (memcmp(HMERecv.Data[3], "200800", 6) == 0)
			{
				HMESend.DataLength[122] = HMERecv.DataLength[122];
				memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
				memcpy(HMESend.Data[122], HMERecv.Data[122], HMESend.DataLength[122]);
			}
			else
				if (memcmp(HMERecv.Data[3], "200900", 6) == 0)
				{
				HMESend.DataLength[122] = 11;
				memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			    memcpy(HMESend.Data[122], HMERecv.Data[122], HMESend.DataLength[122]);
			}
		}
		else
		if(TranCode == TC_ATMBIDSOFTUPDATE) 		//软件更新  BY HYL
		{   
			if (memcmp(HMERecv.Data[3], "201000", 6) == 0)
			{
			 HMESend.DataLength[122] = HMERecv.DataLength[122];
			 memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			 memcpy(HMESend.Data[122], HMERecv.Data[122], HMESend.DataLength[122]);
			}
			else
			if (memcmp(HMERecv.Data[3], "201100", 6) == 0)
			{
			 HMESend.DataLength[122] = 28;
			 memset(HMESend.Data[122], '0', HMESend.DataLength[123]);
			 memcpy(&HMESend.Data[122][0],"025",3);
			 memcpy(&HMESend.Data[122][3],&szDownLoadFileName,12);                                  //每次固定下载长度400
			 memcpy(&HMESend.Data[122][15],GetSprintf("%08s",Int2Asc(SetBegin)),8);               //起始位置
			 memcpy(&HMESend.Data[122][23],&FileEnd,1);                                        //文件长度   BY HYL
			 memcpy(&HMESend.Data[122][24],"0400",4); 
			 
			}
		}  
		else
		if (memcmp(HMESend.Data[3], "160500", 6) == 0)			//例外详细通知代码 BY HYL
		{			
			
			HMESend.DataLength[122] = 21;
			memset(HMESend.Data[122], '0', 21);
	        memcpy(&HMESend.Data[122][0],"018",3);
			
			CString Data("");
			if (HostNms.SaveErrCnt == 0)
			    Data = GetSprintf("ERROR MTC: %7.7s", m_pDevCmn->CurErrBuff.ErrorCode);
			else
			    Data = GetSprintf("ERROR MTC: %7.7s", HostNms.SaveErrBuff.ErrorCode);	
            memcpy(&HMESend.Data[122][3], Data.GetBuffer(Data.GetLength()), Data.GetLength());
		}
		else
		{
			HMESend.DataLength[122] = 3;
			memset(HMESend.Data[122], '0', HMESend.DataLength[122]);
			memcpy(HMESend.Data[122], "000", 3);
		}

	}
	
	if (strBitMap.GetAt(123) == '1')								// 123 应答自定义数据  ans999
	{   

		if(TranCode == TC_ATMBIDSENDDETAIL)
		{
			if (memcmp(HMERecv.Data[3], "200800", 6) == 0)
			{
			  HMESend.DataLength[123] = 34;
			  memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
			  memcpy(&HMESend.Data[123][0],"031",3);
			  memcpy(&HMESend.Data[123][3],"024",3);
			  memcpy(&HMESend.Data[123][6],GetSprintf("%09s",Int2Asc(SendFileLength)),9);  //文件长度   BY HYL
			  memcpy(&HMESend.Data[123][15],&HMERecv.Data[122][3],14);
			  memcpy(&HMESend.Data[123][29],&HMERecv.Data[122][17],1);
			  memcpy(&HMESend.Data[123][30],"0400",4);                            //主机上传明细报文固定为400
			}
			else
			if (memcmp(HMERecv.Data[3], "200900", 6) == 0)
			{

/*
			 			 SendBufferLength = (SendFileLength - SetBegin) < 400 ? (SendFileLength - SetBegin ):400;
			  
			 			 HMESend.DataLength[123] = 411;  
			 		     memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
			 			 memcpy(&HMESend.Data[123][0],"408",3);
			 			 memcpy(&HMESend.Data[123][3],&FileEnd,1);
			 			 memcpy(&HMESend.Data[123][4],"0400",4);
			 			 memcpy(&HMESend.Data[123][8],"400",3);
			 			 memcpy(&HMESend.Data[123][11],DetailTblTemp, SendBufferLength);*/
			 

			}
		}
		else
		if(TranCode == TC_ATMBIDADDOWNLOAD) 		                         //广告,远程提取流水,软件更新  BY HYL
		{   
			if (memcmp(HMERecv.Data[3], "200600", 6) == 0)
			{
			  HMESend.DataLength[123] = 27;
		      memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
		      memcpy(&HMESend.Data[123][0],"024",3);
		      memcpy(&HMESend.Data[123][3],"0400",4);                                  //每次固定下载长度400
		      memcpy(&HMESend.Data[123][7],GetSprintf("%08s",Int2Asc(SetBegin)),8);  //文件长度   BY HYL
		      memcpy(&HMESend.Data[123][15],&szDownLoadFileName,12);                   //下载的文件名
			}
			else
			if (memcmp(HMERecv.Data[3], "200700", 6) == 0)
			{
			  HMESend.DataLength[123] = 3;
			  memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
 			}
 		}
 		else
		if(TranCode == TC_ATMBIDSOFTUPDATE) 		//广告,远程提取流水,软件更新  BY HYL
		{   
			if (memcmp(HMERecv.Data[3], "201000", 6) == 0)
			{
			  HMESend.DataLength[123] = 27;
			  memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
			  memcpy(&HMESend.Data[123][0],"024",3);
			  memcpy(&HMESend.Data[123][3],"0400",4);                                  //每次固定下载长度400
		      memcpy(&HMESend.Data[123][7],GetSprintf("%08s",Int2Asc(SetBegin)),8);  //文件长度   BY HYL
			  memcpy(&HMESend.Data[123][15],&szDownLoadFileName,12);	               //下载的文件名
			}
			else
			if (memcmp(HMERecv.Data[3], "201100", 6) == 0)
			{
			  HMESend.DataLength[123] = 3;
			  memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
 			}
		}
		else
		{
		HMESend.DataLength[123] = 3;
		memset(HMESend.Data[123], '0', HMESend.DataLength[123]);
		memcpy(HMESend.Data[123], "000", 3);
		}
	}

	if (strBitMap.GetAt(127) == '1')							// 127 ATM Status Information
	{
		fnAPP_GetAtmStatusInfo127();

		memset(szTmpData, 0, sizeof(szTmpData));
		sprintf(szTmpData, "%03d", 109);
		memcpy(&HMESend.Data[127][0], szTmpData, 3);
		memcpy(&HMESend.Data[127][3], &AtmStatusInfo127, 109);
		HMESend.DataLength[127] = 109 + 3;
	}
	
//	if (TranCode == TC_OEXSTATUS)
//	{
//		if (HostNms.OexDoorOpenErrorFlag)					// 2006.03.21
//		{
//			HostNms.OexDoorOpenErrorFlag		= FALSE;
//		}
//		else
//		if ((HostNms.OexServiceModeSendFlag)	||
//			(HostNms.OexOpenBySpvSendFlag)		||			// 2005.09.07
//			(HostNms.OexDoorOpenSendFlag))					// 2006.03.21
//		{
//			HostNms.OexServiceModeSendFlag		= FALSE;
//			HostNms.OexOpenBySpvSendFlag		= FALSE;
//			HostNms.OexDoorOpenSendFlag			= FALSE;
//		}
//		else
//		{
//			HostNms.OexSlipPaperOutSendFlag		= FALSE;
//			HostNms.OexJnlPaperOutSendFlag		= FALSE;
//			HostNms.OexBothDenoNotesSendFlag	= FALSE;
//			HostNms.OexPowerOnSendFlag			= FALSE;
//			HostNms.OexCloseBySpvSendFlag		= FALSE;
//		}
//	}

	if (TranCode == TC_WITHDRAWAL)
	{
/////////////////////////////////////////////////////////////////////////////
		strTemp = m_pDevCmn->fstrCSH_GetDispenseOfCST(Asc2Int(&Accept.Money[4], 6));
		SplitString(strTemp, ",", strTempArray);
		if (strTempArray.GetSize() >= 1)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST1, Asc2Int(strTempArray[0]));
		}
		if (strTempArray.GetSize() >= 2)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST2, Asc2Int(strTempArray[1]));
		}
		if (strTempArray.GetSize() >= 3)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST3, Asc2Int(strTempArray[2]));
		}
		if (strTempArray.GetSize() >= 4)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST4, Asc2Int(strTempArray[3]));
		}
/////////////////////////////////////////////////////////////////////////////
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMsg():return");
	return T_OK;
}

// Send Host Make
int CTranCmn::fnAPP_SendHostMake(int RealFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMake(RealFlag[%d])", RealFlag);

	int		Cnt = 0;

/////////////////////////////////////////////////////////////////////////////
//	Send Variable Initialize
/////////////////////////////////////////////////////////////////////////////
	strBitMap = "";												// Bit Map
	SendLength = 0;												// Send Length
	MacLength = 0;												// Mac Length
	
	memset(SendBuffer, 0, sizeof(SendBuffer));					// Send Buffer
	memset(MacBuffer, 0, sizeof(MacBuffer));					// Mac Buffer

	fnAPP_SendHostMakeSet(RealFlag);							// Send Host Make Set
	fnAPP_SendHostMakeHeader(RealFlag, SendBuffer);				// Send Host Make Header
	fnAPP_SendHostMakeMsg(RealFlag);							// Send Host Make Message
	Cnt = fnAPP_SendHostMakeBuffer(RealFlag);					// Send Host Make Buffer
	Cnt = fnAPP_SendHostMakeMac(RealFlag, MacBuffer, Cnt, MacLength);

	if (RealFlag)
		TranSend++;												// Add Tran Send

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMake(RealFlag[%d]):return(%d)", RealFlag, Cnt);
	return Cnt;
}

// Send Host Make Buffer
int CTranCmn::fnAPP_SendHostMakeBuffer(int RealFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeBuffer(RealFlag[%d])", RealFlag);

	int		nTmp = 1;
	int		Cnt = 0;
	int		TempLength = 0;
	char	szTmpBit[1024];										// 2005.09.11
	char	szTmpData[1024];

	memset(szTmpBit, 0, sizeof(szTmpBit));

	Cnt += sizeof(HHMSendHeader);

//	if (TranCode == TC_TOTAL)
//	{
//		for (int i = 0; i < MAXBITMAP2; i++)
//		{
//			if (strBitMap.GetAt(i) == '1')
//			{
//				if (i > 1)										// 2005.09.01
//					szTmpBit[(i - 1) / 8] |= (nTmp << (7 - (i - 1) % 8));
//				memcpy(&SendBuffer[Cnt], HMESend.Data[i], HMESend.DataLength[i]);
//				Cnt += HMESend.DataLength[i];
//				
//				MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeBuffer() HMESend.Data[%d][%s]", i, HMESend.Data[i]);
//			}
//		}
//	}
//	else
	{
		for (int i = 0; i < MAXBITMAP; i++)
		{
			if (strBitMap.GetAt(i) == '1')
			{
				if (i > 0)
					szTmpBit[(i - 1) / 8] |= (nTmp << (7 - (i - 1) % 8));
				memcpy(&SendBuffer[Cnt], HMESend.Data[i], HMESend.DataLength[i]);
				Cnt += HMESend.DataLength[i];

				if (i == 52)
					HexDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeBuffer() HMESend.Data[52]", HMESend.Data[i], HMESend.DataLength[i]);
				else
					MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeBuffer() HMESend.Data[%d][%s]", i, HMESend.Data[i]);

				if ((i == 0)  || (i == 2)   || (i == 3)   ||
					(i == 4)  || (i == 11)  || (i == 12)  ||
					(i == 13) || (i == 39)  || ((i == 53) && (TranCode == TC_OPEN))  ||
					(i == 68) || (i == 102) || (i == 103))
				{
					memset(szTmpData, 0, sizeof(szTmpData));
					TempLength = HMESend.DataLength[i];

					fnAPL_FormatMacData(HMESend.Data[i], TempLength, szTmpData);
					memcpy(&MacBuffer[MacLength], szTmpData, TempLength);
					MacLength += TempLength;
				}
			}
		}
	}
	
	memcpy(HMESend.Data[1], szTmpBit, HMESend.DataLength[1]);
	memcpy(&SendBuffer[HMESend.DataLength[0] + sizeof(HHMSendHeader)], HMESend.Data[1], HMESend.DataLength[1]);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeBuffer(RealFlag[%d]):return(%d)", RealFlag, Cnt);
	return Cnt;
}


// Send Host Make Mac
int CTranCmn::fnAPP_SendHostMakeMac(int RealFlag, void* MakeArea, int SendBuffLength, int MacDataLength)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMac(RealFlag[%d], MakeArea[%s], SendBuffLength[%d], MacDataLength[%d])", RealFlag, MakeArea, SendBuffLength, MacDataLength);

	int			Cnt = 0;
	int			nTempLength = 0;
//	CString		MacData("");
	CString		strTemp("");
	char szMacData[8];
	char sztemp[8];

	Cnt = SendBuffLength;

	if (strBitMap.GetAt(128) == '1')
	{
		strTemp.Format("%s", MakeArea);
		strTemp.TrimLeft();
		strTemp.TrimRight();

// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMac() RAW MACDATA[%s]", strTemp);
	   
       if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)	
	   {
			CString MacData("");
			m_pDevCmn->fnPIN_MacingData(MakeUnPack(strTemp.GetBuffer(0), strTemp.GetLength()));
			MacData = m_pDevCmn->fstrPIN_GetMacingData();
			MakePack(MacData.GetBuffer(0), szMacData, 16);
	   }
	   else
			fnAPL_GetMACData(strTemp, (UCHAR *)SaveMACKEY, (UCHAR *)szMacData);

		HMESend.DataLength[128] = 8;
		memcpy(HMESend.Data[128], szMacData, HMESend.DataLength[128]);
		memcpy(&SendBuffer[Cnt], HMESend.Data[128], HMESend.DataLength[128]);
		Cnt += HMESend.DataLength[128];
	}

	HHMSendHeader*	pHHMSendData = (HHMSendHeader*)SendBuffer;
	
	MakePack(GetSprintf("%04X", Cnt - 2).GetBuffer(0), sztemp, 4);
	memcpy(pHHMSendData->Length, sztemp, 2);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SendHostMakeMac(RealFlag[%d], MacData[%s], return[%d])", RealFlag, MakeUnPack(szMacData,sizeof(szMacData)), Cnt);
	return Cnt;
}

// Recv Host
int CTranCmn::fnAPP_RecvHost()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost()");

	int			LineCloseTime = LINE_OPCL_TIME;
	int			LineRecvTime = InitInfo.LineRecvTime;

/////////////////////////////////////////////////////////////////////////////////////////////////
	int		nWithMaxAmt = 0;									// Withdraw Max Amount
	int		nWithMinAmt = 0;									// Withdraw Min Amount
	int     nDetailNum  = 0;                                    // 明细条数
	int     nADSetNum   = 0;                                    // 广告张数
	int     nADTime     = 0;                                    // 广告时间
	int     nErrorNoticeTime = 0;

//add by yaokq in 20080903 
	double		nZztohMaxNum = 0;									//整整转活期最大限额
	double		nZztohMinNum = 0;									//整整转活期最小限额
	double		nDhtohMaxNum = 0;									//定活转活期最大限额
	double		nDhtohMinNum = 0;									//定活转活期最小限额
	double		nTzcktohMaxNum = 0;									//通知存款转活期最大限额
	double		nTzcktohMinNum = 0;									//通知存款转活期最小限额
	double		nHtodhMaxNum = 0;									//活转定活最大限额
	double		nHtodhMinNum = 0;									//活转定活最小限额
	double		nHtotzcqMaxNum = 0;									//活转通知存款最大限额
	double		nHtotzcqMinNum = 0;									//活转通知存款最小限额
	double		nHtozzMaxNum = 0;									//活转整整最大限额
	double		nHtozzMinNum = 0;									//活转整整最小限额
	double		nSaveKindPara = 0;									//储蓄种类参数

////////////////////////////////////////////////////////////////////////////////////////////////	
//			double	dTransferMaxAmt = 0;								// Transfer Max Amount
	int		nMsrSendTime = LINE_MSR_SEND_TIME;					// Msr Send Time
	int		nTakeCard = TAKE_CARD_TIME_OUT;						// Take Card TimeOut
	int		nTakeCash = TAKE_CASH_TIME_OUT;						// Take Cash TimeOut
	int		nTransRetryTime = 0;								// Trans Retry Max Time
	int		nCWCRetryTime = 3;									// CWC Retry Time
	int		iCount, i, i1, i2, i3;
	CString	strFITTemp("");
	CStringArray strTempArray, strTempArray1;
	char	szFITTemp1[256];
	char	szFITTemp2[256];
	char	szFITTemp3[256];
	char	MasterKey[16];
	CString strMasterKey("");
	CString strTmp("");
//	CString strMasterKey = GetShareData("OP_MASTERKEY");	//长度为32，两倍长

/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||			// Recv & Close Time Setting
		(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||			// 2004.06.22
		(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
//		(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
		(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
	{
		LineCloseTime = LINE_BID_OPCL_TIME;
		LineRecvTime = LINE_BID_RECV_TIME;

		if (m_pProfile->NETWORK.InterfaceKind == 2500)			// POST-TY(HNC)			// 2007.05.15 : ATMU
			;
		else
			fnAPP_LineClose(LineCloseTime);


		m_pDevCmn->TranResult = TRUE;
		memset(HostResp.RejectCode, '0', sizeof(HostResp.RejectCode));
		return T_OK;
	}
	else
	if ((TranCode == TC_ICINQUIRY)	||							// 2010.10.20
		(TranCode == TC_ICDETAIL)	||
		(TranCode == TC_SCRIPT))
	{
		m_pDevCmn->TranResult = TRUE;
		memcpy(HostResp.RejectCode, "00", sizeof(HostResp.RejectCode));
		return T_OK;
	}
	else
	if (m_pDevCmn->TranStatus == TRAN_OPEN)
	{
		LineCloseTime = LINE_OPCL_TIME;
		LineRecvTime = InitInfo.LineRecvTime;
	}
	else
	{
		LineCloseTime = LINE_OPCL_TIME;
		LineRecvTime = InitInfo.LineRecvTime;
	}
	
/////////////////////////////////////////////////////////////////////////////
	if (fnAPP_RecvData(RecvBuffer, &RecvLength, LineRecvTime) != T_OK)		
	{

		if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||		// No Cancel
			(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||		// 2004.06.22
			(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
//			(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
			(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
			;
		else													// Error & Cancel
		{
			if ((TranCode == TC_WITHDRAWAL)	||
				(TranCode == TC_TRANSFER)	||
				(TranCode == TC_LOAD)		||
				(TranCode == TC_BOUNDMDOIFY)||
				(TranCode == TC_HZT)		||					// liyi add 绿卡通
				(TranCode == TC_TZH)		||
				(TranCode == TC_REMIT_ADD)	||
				(TranCode == TC_REMIT_IN)	||
				(TranCode == TC_REMIT_PWD)	||
				(TranCode == TC_PAY)        ||
				(TranCode == TC_PAY_INQ)	||
				(TranCode == TC_OTHER_INQ))				//20080904 by yaokq
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(14);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
			}

			if (m_pDevCmn->TranStatus != TRAN_OPEN) 			// NOT OPEN TRANSACTION : 2005.05.26
			{													//  打印流水接收错  by zjd
				if (TranCode == TC_TOTAL	||
					TranCode == TC_ADDCASH);
				else
					m_pDevCmn->fnAPL_StackError("8860292", "RECV TIME OVER");
				if (LangMode)
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****COMM RECV ERROR****",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4])));
				}
				else
				{
					m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ****交易应答包接收错误****",
						GetTime().GetBuffer(0),
						&(GetTime().GetBuffer(0)[2]),
						&(GetTime().GetBuffer(0)[4])));
				}
			}

			if (TranCode == TC_WITHDRAWAL)						// added by liuxl 20110714
				fnAPD_CwcInfoSet(1);
			else
			if (TranCode == TC_REMIT_ADD	|| 
				TranCode == TC_REMIT_PWD	|| 
				TranCode == TC_REMIT_IN)
				fnAPD_CwcInfoSet(3);
			else
			if (TranCode == TC_LOAD)
			{
				SprPrintATCFlag = 1;
				fnAPD_CwcInfoSet(5);
			}
			else
			if (TranCode == TC_TOTAL	||
				TranCode == TC_ADDCASH)
			{
				TotalAddFlag = 0;
				return T_OK;
			}

			fnAPP_CancelProc(T_RECVERROR);
		}
	}
    
	bCommRecvFlag = 1;  //置接收标志为完成

	strTmp.Format("%2.2s:%2.2s:%2.2s [TCPIP RECVDATA], Length : [%d]",
								GetTime().GetBuffer(0),
								&(GetTime().GetBuffer(0)[2]),
								&(GetTime().GetBuffer(0)[4]),
								RecvLength);
	m_pDevCmn->fnCMN_SaveLineTrace(strTmp.GetBuffer(0), strTmp.GetLength());
///2014.04.19 by jl 屏蔽Line日志报文信息
//	m_pDevCmn->fnCMN_SaveLineTrace(RecvBuffer, RecvLength, 1);

	if (m_pProfile->NETWORK.InterfaceKind == 2500)			// POST-TY(HNC)			// 2007.05.15 : ATMU
		;
	else
		fnAPP_LineClose(LineCloseTime);

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
	{
		m_pDevCmn->TranResult = TRUE;
		memset(HostResp.RejectCode, '0', sizeof(HostResp.RejectCode));
		DownloadOverFlag = 1;
		return T_OK;
	}

	if ((m_pDevCmn->TranStatus == TRAN_ERROR)		||			// No Cancel
		(m_pDevCmn->TranStatus == TRAN_AEXSTATUS)	||			// 2004.06.22
		(m_pDevCmn->TranStatus == TRAN_OEXSTATUS)	||
//		(m_pDevCmn->TranStatus == TRAN_MSRSTATUS)	||
		(m_pDevCmn->TranStatus == TRAN_CONFIRMWITH))
	{
		m_pDevCmn->TranResult = TRUE;
		memset(HostResp.RejectCode, '0', sizeof(HostResp.RejectCode));
		return T_OK;
	}

	RecvOkFlag = TRUE;											// 2006.07.03

	fnAPP_RecvAnal();											// Recv Anal

	if ((m_pDevCmn->TranStatus == TRAN_TRAN)	||				// TRANSACTION
		(m_pDevCmn->TranStatus == TRAN_ADDCASH)	||
		(m_pDevCmn->TranStatus == TRAN_TOTAL)	||
		(m_pDevCmn->TranStatus == TRAN_TOTALKIND)	||
		(m_pDevCmn->TranStatus == TRAN_TOTALPRE)	||
		(m_pDevCmn->TranStatus == TRAN_CANCEL))		
	{
		if (LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s HOST RET<%2.2s>HOST:%12s",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode, HMERecv.Data[37]));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 返回码:%2.2s 主机流水:%12s",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode, HMERecv.Data[37]));
		}
	}
	else
	if ((m_pDevCmn->TranStatus == TRAN_DOWNFIT)		||				// 060411 完善流水
		(m_pDevCmn->TranStatus == TRAN_DOWNICFIT)	||				// added by yusy ic
		(m_pDevCmn->TranStatus == TRAN_DOWNAIDLIST)	||
		(m_pDevCmn->TranStatus == TRAN_DOWNPARM)	||
		(m_pDevCmn->TranStatus == TRAN_DOWNPBOCPARA)||
		(m_pDevCmn->TranStatus == TRAN_DOWNPAY)		||
		(m_pDevCmn->TranStatus == TRAN_INITCDK)		||
		(m_pDevCmn->TranStatus == TRAN_INITMAC)		||
		(m_pDevCmn->TranStatus == TRAN_INITPIN)	||
		(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPARA)	||
		(m_pDevCmn->TranStatus == TRAN_DOWNECHANNELPROT))
	{
		if (LangMode)
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s HOST RETURN<%2.2s>",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode));
		}
		else
		{
			m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 返回码:%2.2s",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode));
		}
	}
	else
	if (m_pDevCmn->TranStatus == TRAN_OPEN)
	{
		if (LangMode)
		{
			if (RQKCode == 0)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s HOST RETURN<%2.2s> CDK",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
						HostResp.RejectCode));
			}
			else
			if (RQKCode == 1)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s HOST RETURN<%2.2s> MACKEY",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
						HostResp.RejectCode));
			}
			else
			if (RQKCode == 2)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s HOST RETURN<%2.2s> PINKEY",
						GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
						HostResp.RejectCode));
			}
		}
		else
		{
			if (RQKCode == 0)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 返回码:%2.2s CDK",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode));
			}
			else
			if (RQKCode == 1)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 返回码:%2.2s MAC",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode));
			}
			else
			if (RQKCode == 2)
			{
				m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 返回码:%2.2s PIN",
					GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
					HostResp.RejectCode));
			}
		}
	}
// 	if(m_pDevCmn->bAgentMode == 1)
// 	{
// 		if( TranCode == TC_INQUIRY)
// 			fnAPP_AgentIdxGetData(AGENT_INQUIRY);
// 		else
// 		if( TranCode == TC_CHANGEPIN)
// 			fnAPP_AgentIdxGetData(AGENT_CHANGEPIN);
// 		else
// 		if( TranCode == TC_TRANSFER)
// 			fnAPP_AgentIdxGetData(AGENT_TRANSFER);
// 		else
// 		if( TranCode == TC_DETAIL)
// 			fnAPP_AgentIdxGetData(AGENT_DETAIL);
// 		else
// 		if( TranCode == TC_CANCEL)
// 			fnAPP_AgentIdxGetData(AGENT_CANCEL);
// 		else
// 		if( TranCode == TC_OPEN && RQKCode == 0)
// 		{
// 			if((memcmp(HostResp.RejectCode, "00", 2) == 0))
// 			{
// 				if(m_pDevCmn->AtmStatus != ATM_ERROR)                //故障模式下不往ATOS发签到
// 				{
// 					fnAPP_AgentIdxGetData(AGENT_OPEN);
// 					nAgnetAtmOpenFistFailFlag = TRUE;
// 	//  			nAgentAtmAfterOpenFistFailFlag = TRUE;
// 				}
// 			}
// 			else
// 			{
// 				if(nAgnetAtmOpenFistFailFlag == TRUE)
// 				{
// 				   fnAPP_AgentIdxGetData(AGENT_OPENFAIL);
// 				   nAgnetAtmOpenFistFailFlag = FALSE;
// 				}
// 			}
// 		}
// 		else
// 		if( TranCode == TC_PAY)
// 			fnAPP_AgentIdxGetData(AGENT_PAY);
// 	}
	
	switch (TranCode) 
	{
		case TC_OPEN:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if ((!IsNum(HMERecv.Data[12], HMERecv.DataLength[12])) ||
				(IsZero(HMERecv.Data[12], HMERecv.DataLength[12])))
				break;

			if ((!IsNum(HMERecv.Data[13], HMERecv.DataLength[13])) ||
				(IsZero(HMERecv.Data[13], HMERecv.DataLength[13])))
				break;

			if (HMERecv.DataRecvBit[96] == 0)					// 2005.09.11
				break;
				
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
				break;

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
				break;

//需要判断密钥的类型，并保存相应的密钥   by zjd
//	char		SaveCDK[16];									// CDK Key
//	char		SaveMACKEY[8];									// MAC Key
//	char		SavePINKEY[16];									// PIN Key
			if (RQKCode == 0)
			{													//保存CDK
				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				{
					if(HMERecv.DataLength[96] != 26)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DataLen Error");
						fnAPP_CancelProc(T_RECVERROR);
					}
					char SaveCDK[32];
					char tempData[16];
					char PinBlock[8];
					char PinBlock2[8];
					m_pDevCmn->fnPIN_LoadIV("Vector");
					MakeUnPack(&HMERecv.Data[96][2], SaveCDK, 16, 1);//CDK KEY
					memcpy(PinBlock2, &HMERecv.Data[96][18], 8);
					if(RQKCode2 == 0)
					{
						m_pDevCmn->fnPIN_ImportKey(TMASTERKEY, TCONSTMASTERKEY, GetSprintf("%32.32s", SaveCDK));
						m_pDevCmn->fnPIN_EncryptECB("TRIDESECB", TMASTERKEY, "", 15, ZERO16);
						memcpy(tempData, m_pDevCmn->fnPIN_GetEncryptedData(), 16);
						MakePack(tempData, PinBlock, 16);
						if(memcmp(PinBlock2, PinBlock, 8) != 0)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PinBlock Error[%s][%s]", MakeUnPack(PinBlock, sizeof(PinBlock)), MakeUnPack(PinBlock2, sizeof(PinBlock2)));
							fnAPP_CancelProc(T_RECVERROR);
						}
					}
					else
					{
						m_pDevCmn->fnPIN_ImportKey(TMASTERKEY, TMASTERKEY, GetSprintf("%32.32s", SaveCDK));
						m_pDevCmn->fnPIN_EncryptECB("TRIDESECB", TMASTERKEY, "", 15, ZERO16);
						memcpy(tempData, m_pDevCmn->fnPIN_GetEncryptedData(), 16);
						MakePack(tempData, PinBlock, 16);
						if(memcmp(PinBlock2, PinBlock, 8) != 0)
						{
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PinBlock Error[%s][%s]", MakeUnPack(PinBlock, sizeof(PinBlock)), MakeUnPack(PinBlock2, sizeof(PinBlock2)));
							fnAPP_CancelProc(T_RECVERROR);
						}
					}

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveCDK_epp[%s])",SaveCDK);
				}
				else
				{
					if(HMERecv.DataLength[96] != 26)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DataLen Error");
						fnAPP_CancelProc(T_RECVERROR);
					}
					strMasterKey = GetShareData("OP_MASTERKEY");			//长度为32，两倍长
					if (strMasterKey == "")
					{
						strMasterKey = "00000000000000000000000000000000";
						SetShareData("OP_MASTERKEY", strMasterKey);
					}
					memcpy(SaveCDK, &HMERecv.Data[96][2], 16);
					MakePack(strMasterKey.GetBuffer(0), MasterKey, 32);
					if (iDEBUGFLAG)
					{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MasterKey[%s])", MakeUnPack(MasterKey,sizeof(MasterKey)));
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveCDK[%s])", MakeUnPack(SaveCDK,sizeof(SaveCDK)));
					}
					

					DesEncryptDecrypt((UCHAR *)MasterKey, (UCHAR *)SaveCDK, 0);
					DesEncryptDecrypt((UCHAR *)(MasterKey + 8), (UCHAR *)SaveCDK, 1);
					DesEncryptDecrypt((UCHAR *)MasterKey, (UCHAR *)SaveCDK, 0);
					if (iDEBUGFLAG)
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveCDK[%s])", MakeUnPack(SaveCDK,sizeof(SaveCDK)));

					DesEncryptDecrypt((UCHAR *)MasterKey, (UCHAR *)(SaveCDK + 8), 0);
					DesEncryptDecrypt((UCHAR *)(MasterKey + 8), (UCHAR *)(SaveCDK + 8), 1);
					DesEncryptDecrypt((UCHAR *)MasterKey, (UCHAR *)(SaveCDK + 8), 0);
					if (iDEBUGFLAG)
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveCDK[%s])", MakeUnPack(SaveCDK,sizeof(SaveCDK)));

					CString strSaveCDK("");
					strSaveCDK = MakeUnPack(SaveCDK, sizeof(SaveCDK));
					SetShareData("OP_MASTERKEY", strSaveCDK);

					char PinBlock[8];
					char PinBlock2[8];
					memcpy(PinBlock2, &HMERecv.Data[96][18], 8);
					memset(&PinBlock, 0x00, sizeof(PinBlock));
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)PinBlock, 1);
					DesEncryptDecrypt((UCHAR *)(SaveCDK + 8), (UCHAR *)PinBlock, 0);
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)PinBlock, 1);
					if(memcmp(PinBlock2, PinBlock, 8) != 0)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PinBlock Error[%s][%s]", MakeUnPack(PinBlock, sizeof(PinBlock)), MakeUnPack(PinBlock2, sizeof(PinBlock2)));
						fnAPP_CancelProc(T_RECVERROR);
					}
				}
			}
			else if(RQKCode == 1)
			{		
				//保存MACKEY
				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				{   
					char MacKey[16];
					MakeUnPack(&HMERecv.Data[96][2], MacKey, 8, 1);	// Mac Key	
					m_pDevCmn->fnPIN_LoadIV("Vector");
					m_pDevCmn->fnPIN_ImportKey(TMACKEY, TMASTERKEY, GetSprintf("%16.16s",MacKey));
					m_pDevCmn->fnPIN_SetMacKeyName(TMACKEY);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost MACKEY[%s])", MacKey);
				}
				else
				{
					memcpy(SaveMACKEY, &HMERecv.Data[96][2], 8);
					if (iDEBUGFLAG)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveMACKEY[%s])", MakeUnPack(SaveMACKEY,sizeof(SaveMACKEY)));
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveCDK[%s])", MakeUnPack(SaveCDK,sizeof(SaveCDK)));
					}
					
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)SaveMACKEY, 0);
					DesEncryptDecrypt((UCHAR *)(SaveCDK + 8), (UCHAR *)SaveMACKEY, 1);
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)SaveMACKEY, 0);
					if (iDEBUGFLAG)
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveMACKEY[%s])", MakeUnPack(SaveMACKEY,sizeof(SaveMACKEY)));
				}

				memcpy(HostResp.MacData, HMERecv.Data[128], sizeof(HostResp.MacData));
				
				CString strTemp("");
				strTemp.Format("%s", MacBuffer);
				strTemp.TrimLeft();
				strTemp.TrimRight();
				if (iDEBUGFLAG)
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MAC) MACDATA[%s]", strTemp);
				char szMacData[8];
				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)	
				{
					CString MacData("");
					m_pDevCmn->fnPIN_MacingData(MakeUnPack(strTemp.GetBuffer(0), strTemp.GetLength()));
					MacData = m_pDevCmn->fstrPIN_GetMacingData();
					MakePack(MacData.GetBuffer(0), szMacData, 16);
				}
	            else
				    fnAPL_GetMACData(strTemp, (UCHAR *)SaveMACKEY, (UCHAR *)szMacData);
				if (iDEBUGFLAG)
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MAC) MACRESULT[%s]", MakeUnPack(szMacData, 8));
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MAC) PACKAGE[%s]", MakeUnPack(HostResp.MacData, 8));
				}
				if (memcmp(szMacData, HostResp.MacData, 8) != 0)
				{
					HostNms.RqkSendFlag = TRUE;
					m_pDevCmn->fnAPL_StackError("8850102", "MAC MISMATCH");
					fnAPP_CancelProc(T_RECVERROR);
				}
				
			}
			else if(RQKCode == 2)
			{   //保存PINKEY
				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				{
					char PinKey[32];
					MakeUnPack(&HMERecv.Data[96][2], PinKey,16, 1);	// Pin Key	
					m_pDevCmn->fnPIN_LoadIV("Vector");
			        m_pDevCmn->fnPIN_ImportKey(TPINKEY, TMASTERKEY, GetSprintf("%32.32s",PinKey));
					m_pDevCmn->fnPIN_SetPinKeyName(TPINKEY);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost PINKEY[%s])", PinKey);

					m_pDevCmn->fnPIN_SetMacKeyName(TPINKEY);
					memcpy(HostResp.MacData, HMERecv.Data[128], sizeof(HostResp.MacData));
					
					CString strTemp("");
					strTemp.Format("%s", MacBuffer);
					strTemp.TrimLeft();
					strTemp.TrimRight();
					if (iDEBUGFLAG)
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MAC) MACDATA[%s]", strTemp);
					char szMacData[8];
					{
						CString MacData("");
						m_pDevCmn->fnPIN_MacingData(MakeUnPack(strTemp.GetBuffer(0), strTemp.GetLength()));
						MacData = m_pDevCmn->fstrPIN_GetMacingData();
						MakePack(MacData.GetBuffer(0), szMacData, 16);
					}
					if (iDEBUGFLAG)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MAC) MACRESULT[%s]", MakeUnPack(szMacData, 8));
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(MAC) PACKAGE[%s]", MakeUnPack(HostResp.MacData, 8));
					}
					m_pDevCmn->fnPIN_SetMacKeyName(TMACKEY);
					if (memcmp(szMacData, HostResp.MacData, 8) != 0)
					{
						HostNms.RqkSendFlag = TRUE;
						m_pDevCmn->fnAPL_StackError("8850102", "MAC MISMATCH");
						fnAPP_CancelProc(T_RECVERROR);
					}

				}
				else
				{
					memcpy(SavePINKEY, &HMERecv.Data[96][2], 16);
					if (iDEBUGFLAG)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SavePINKEY[%s])", MakeUnPack(SavePINKEY,sizeof(SavePINKEY)));
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SaveCDK[%s])", MakeUnPack(SaveCDK,sizeof(SaveCDK)));
					}
					
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)SavePINKEY, 0);
					DesEncryptDecrypt((UCHAR *)(SaveCDK + 8), (UCHAR *)SavePINKEY, 1);
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)SavePINKEY, 0);
					if (iDEBUGFLAG)
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SavePINKEY[%s])", MakeUnPack(SavePINKEY,sizeof(SavePINKEY)));
					
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)(SavePINKEY + 8), 0);
					DesEncryptDecrypt((UCHAR *)(SaveCDK + 8), (UCHAR *)(SavePINKEY + 8), 1);
					DesEncryptDecrypt((UCHAR *)SaveCDK, (UCHAR *)(SavePINKEY + 8), 0);
					if (iDEBUGFLAG)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(SavePINKEY[%s])", MakeUnPack(SavePINKEY,sizeof(SavePINKEY)));
				}
				
			}
			else
				break;

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_ERROR:
		case TC_AEXSTATUS:
		case TC_OEXSTATUS:
		case TC_CARDRETAIN:
		case TC_TOTALKIND:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;
			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_MSRSTATUS:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (HMERecv.DataRecvBit[120] == 0)
				break;
			
//需要处理时间同步，以及是否有新的东西需要下载，并置标志，然后由fnAPP_NmsProc来进行下载  by zjd
//主要为120域的处理。
			if (memcmp(HMERecv.Data[120] + 3, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_FITVERSION,"00000000").GetBuffer(0),8) != 0)
			{
				memcpy(NewFITVersion, HMERecv.Data[120] + 3, sizeof(NewFITVersion));
				DownloadFITFlag = 1;
			}
			// added by yusy ic
			if (memcmp(HMERecv.Data[120] + 41, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ICFITVERSION,"00000000").GetBuffer(0),8) != 0)
			{
				memcpy(NewICFITVersion, HMERecv.Data[120] + 41, sizeof(NewICFITVersion));
				DownloadICFITFlag = 1;
			}
			if (memcmp(HMERecv.Data[120] + 49, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PBOCPARAVERSION,"00000000").GetBuffer(0),8) != 0)
			{
				memcpy(NewPBOCParaVersion, HMERecv.Data[120] + 49, sizeof(NewPBOCParaVersion));
				DownloadPBOCParaFlag = 1;
			}
			if (memcmp(HMERecv.Data[120] + 57, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_AIDLISTVERSION,"00000000").GetBuffer(0),8) != 0)
			{
				memcpy(NewAIDListVersion, HMERecv.Data[120] + 57, sizeof(NewAIDListVersion));
				DownloadAIDListFlag = 1;
			}
 			if ((HMERecv.DataLength[120] > 65)	&&
				(memcmp(HMERecv.Data[120] + 65, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELVERSION,"11111111").GetBuffer(0),8) != 0)	&&
				(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELFLAG, 1))	&&
				(DownloadEchannelNum <= IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DOWNLOADECHNUM, 3)))
 			{
 				memcpy(NewEchannelParaVersion, HMERecv.Data[120] + 65, sizeof(NewEchannelParaVersion));
 				DownloadEchannelParaFlag = 1;
				DownloadEchannelNum++;
				DownloadEchannelMtd = 1;
				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 0);
 			}
			if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELFLAG, 1))
			{
				if (memcmp(HMERecv.Data[120] + 65, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELVERSION,"11111111").GetBuffer(0),8) == 0)
//				if (memcmp(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ECHANNELVERSION,"00000000").GetBuffer(0),
//					IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPROT, _INIKEY_TRANSACTION_ECHVERSION,"00000000").GetBuffer(0),
//					8) == 0)
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
						IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 1);
					}
					else
					{
						IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 0);
					}
				}
				else
				{
					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DOWNLOADECHRSLT, 0);
				}
			}
			if (memcmp(HMERecv.Data[120] + 11, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PARMVERSION,"00000000").GetBuffer(0),8) != 0)
			{
				memcpy(NewParmVersion, HMERecv.Data[120] + 11, sizeof(NewParmVersion));
				DownloadParmFlag = 1;
			}
			if (memcmp(HMERecv.Data[120] + 19, IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PAYVERSION,"00000000").GetBuffer(0),8) != 0)
			{
				memcpy(NewPayVersion, HMERecv.Data[120] + 19, sizeof(NewPayVersion));
				DownloadPayFlag = 1;
			}

			if (IsNum(GetSprintf("%14.14s", HMERecv.Data[120] + 27).GetBuffer(0), 14))
			{
				if ((Asc2Float(GetSprintf("%14.14s", HMERecv.Data[120] + 27), 14) > Asc2Float(GetSprintf("%8.8s%6.6s", GetDate(), GetTime()), 14) + 3000) ||
					(Asc2Float(GetSprintf("%14.14s", HMERecv.Data[120] + 27), 14) < Asc2Float(GetSprintf("%8.8s%6.6s", GetDate(), GetTime()), 14) - 3000))
				{												//做时间同步  by zjd
					PutDate(GetSprintf("%8.8s", HMERecv.Data[120] + 27));
					PutTime(GetSprintf("%6.6s", HMERecv.Data[120] + 35));
				}
			}

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_INQUIRY:
		case TC_CHANGEPIN:
		case TC_DETAIL:
		case TC_PAY_INQ:
		case TC_OTHER_INQ:						// 20080904 by yaokq
		case TC_REMIT:
		case TC_REMIT_ADD:
		case TC_REMIT_PWD:
		case TC_REMIT_IN:
		case TC_REMIT_INF_INQ:
		case TC_CUS_INF_INQ:					// 2010.03.22 by yusy
		case TC_CUS_INF_INQ_OTHERBANK:
		case TC_SMSPARITY:
		case TC_ECHANNEL:
		case TC_INQUIRY_CheckPin:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_TRANSFER:
		case TC_LOAD:
		case TC_BOUNDMDOIFY:
		case TC_HZT:											// liyi add 绿卡通
		case TC_TZH:
		case TC_PAY:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (TranCode == TC_LOAD	&&
				!ICScriptWriteFlag)
			{
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TC_LOAD]:No Script");
				m_pDevCmn->fnAPL_StackError("8860292", "NO SCRIPT");
				fnAPD_CwcInfoSet(5);
				SprPrintATCFlag = 1;
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[4], HMERecv.Data[4], HMESend.DataLength[4]) != 0)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(20);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnAPL_StackError("8850113", "AMOUNT MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(20);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(20);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
			
		case TC_WITHDRAWAL:			
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (memcmp(HMESend.Data[4], HMERecv.Data[4], HMESend.DataLength[4]) != 0)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(16);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnAPL_StackError("8850113", "AMOUNT MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(16);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(16);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_CANCEL:	
	        if (TranCode2 == 0) 					// 2008.10.13
			{
				if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				{
				   //可疑交易处理 BY HYL
 					fnAPP_SuspiciProc();     
					break;
 				}
			}
			
			if (memcmp(HMESend.Data[4], HMERecv.Data[4], HMESend.DataLength[4]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850113", "AMOUNT MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
			
		case TC_TOTAL:
//			InitInfo.TotalDisplayNo = Asc2Int(HMERecv.Data[66], 1);
//			if (memcmp(HMERecv.Data[66], "1", 1) != 0)			// 2005.09.08
//				break;
//			

			if (memcmp(HostResp.RejectCode, "40", 2) == 0	||
				memcmp(HostResp.RejectCode, "Z0", 2) == 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 2;
				break;
			}

			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 3;
				break;
			}

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 3;
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 3;
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			m_pTranCmn->InitInfo.TotalDisplayNo = 1;
			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_TOTALPRE:
//			if (memcmp(HostResp.RejectCode, "40", 2) == 0	||
//				memcmp(HostResp.RejectCode, "Z0", 2) == 0)
//			{
//				m_pTranCmn->InitInfo.TotalDisplayNo = 2;
//				break;
//			}
			
			if (memcmp(HostResp.RejectCode, "00", 2) != 0 &&
				memcmp(HostResp.RejectCode, "3L", 2) != 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 3;
				break;
			}
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 3;
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pTranCmn->InitInfo.TotalDisplayNo = 3;
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			m_pTranCmn->InitInfo.TotalDisplayNo = 1;
			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_ADDCASH:
			//if (memcmp(HostResp.RejectCode, "00", 2) != 0	&&  // LIYI 0816
			//	memcmp(HostResp.RejectCode, "3M", 2) != 0)
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
			{
				break;
			}
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
		case TC_SUBCASH:
		case TC_CLEARCASH:
		case TC_PRINTCASH:
		case TC_SENDDETAIL:
		case TC_DETAILEND:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
			{
				break;
			}

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
			
		case TC_DOWNFIT:										//以下三个需要处理   by zjd
		case TC_DOWNICFIT:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

//			145 0 03 043 003 2 00 06 622151   00 00 00 00 00 19 2 1111000111111111
//			    	 045 004 2 00 08 62218830 00 00 00 00 00 19 1 1111100111111111
//					 045 005 2 00 08 62218831 00 00 00 00 00 19 1 1111100111111111
			if (memcmp(HMERecv.Data[120] + 3, "1", 1) == 0)
				DownloadOverFlag = 1;
			if (IsNum(&HMESend.Data[120][4], 2))
			{
				iCount = Asc2Int(&HMERecv.Data[120][4], 2);
				DownloadCurrNum += iCount;
			}

			i = 6;
			while((iCount--) > 0)
			{
				i += 3;//Asc2Int(&HMERecv.Data[120][3], 3) + 
				memset(szFITTemp1, 0, sizeof(szFITTemp1));
				memset(szFITTemp2, 0, sizeof(szFITTemp2));
				memset(szFITTemp3, 0, sizeof(szFITTemp3));
				i1 = Asc2Int(&HMERecv.Data[120][i + 6], 2);
				memcpy(szFITTemp1, &HMERecv.Data[120][i + 8], i1);
				i2 = Asc2Int(&HMERecv.Data[120][i + 10 + i1], 2);
				memcpy(szFITTemp2, &HMERecv.Data[120][i + 12 + i1], i2);
				i3 = Asc2Int(&HMERecv.Data[120][i + 14 + i1 + i2], 2);
				memcpy(szFITTemp3, &HMERecv.Data[120][i + 16 + i1 + i2], i3);
				strFITTemp = GetSprintf("%3.3s,%1.1s,%2.2s,%2.2s,%s,%2.2s,%2.2s,%s,%2.2s,%2.2s,%s,%2.2s,%2.2s,%1.1s,%16.16s",
											&HMERecv.Data[120][i],
											&HMERecv.Data[120][i + 3],
											&HMERecv.Data[120][i + 4],
											&HMERecv.Data[120][i + 6],
											szFITTemp1,
											&HMERecv.Data[120][i + 8 + i1],
											&HMERecv.Data[120][i + 10 + i1],
											szFITTemp2,
											&HMERecv.Data[120][i + 12 + i1 + i2],
											&HMERecv.Data[120][i + 14 + i1 + i2],
											szFITTemp3,
											&HMERecv.Data[120][i + 16 + i1 + i2 + i3],
											&HMERecv.Data[120][i + 18 + i1 + i2 + i3],
											&HMERecv.Data[120][i + 20 + i1 + i2 + i3],
											&HMERecv.Data[120][i + 21 + i1 + i2 + i3]
											);
				if (TranCode == TC_DOWNFIT)
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_FITPARM, GetSprintf("%s%d","FITT",Asc2Int(&HMERecv.Data[120][i], 3)), GetSprintf("%s",strFITTemp));
				else
					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ICFITPARM, GetSprintf("%s%d","FITT",Asc2Int(&HMERecv.Data[120][i], 3)), GetSprintf("%s",strFITTemp));
					
				i += 37 + i1 + i2 + i3;
			}
			if(TranCode == TC_DOWNFIT)
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_FITPARM, "FITTNUM", GetSprintf("%d",DownloadCurrNum));
			else
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ICFITPARM, "FITTNUM", GetSprintf("%d",DownloadCurrNum));

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;

		case TC_ECHANNEL_INQ:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			//			memcpy(NewEchannelParaVersion, &HMERecv.Data[120][3], 8);
			
			iCount = 0;
			if (IsNum(&HMERecv.Data[123][14], 3))
			{
				iCount = Asc2Int(&HMERecv.Data[123][14], 3);
			}
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ECHANNELInq_POST_iCount[%d]", iCount);

			i1 = 0;
			i = 17;
			while ((iCount--) > 0)
			{
				strFITTemp = GetSprintf("%3.3s,%1.1s",
					&HMERecv.Data[123][i],
					&HMERecv.Data[123][i + 3]
					);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ECHANNELInq_POST_strFITTemp[%s]", strFITTemp);
				
				if (strFITTemp != "")
				{
					memcpy(ECHANNELInq_POST[i1], strFITTemp.GetBuffer(0), strlen(strFITTemp));
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ECHANNELInq_POST[%d][%s]", i1, ECHANNELInq_POST[i1]);
					i1 ++;
				}

				i += 4;
			}
			memcpy(ECHANNELInq_POST[i1], "AAAAAA", 6);
			
			m_pDevCmn->TranResult = TRUE;
			break;

		case TC_DOWNECHANNELPARA:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (!DownloadEchannelMtd)
				memcpy(NewEchannelParaVersion, &HMERecv.Data[120][3], 8);
			
			if (IsNum(&HMERecv.Data[120][11], 3))
			{
				iCount = Asc2Int(&HMERecv.Data[120][11], 3);
				DownloadCurrNum = iCount;
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPARM, "ECHANNELNUM", GetSprintf("%d", DownloadCurrNum));
			}
			else
				break;
			
			i = 14;
			while ((iCount--) > 0)
			{
				strFITTemp = GetSprintf("%3.3s,%20.20s,%8.8s,%6.6s,%1.1s,%1.1s",
											&HMERecv.Data[120][i],
											&HMERecv.Data[120][i + 3],
											&HMERecv.Data[120][i + 23],
											&HMERecv.Data[120][i + 31],
											&HMERecv.Data[120][i + 37],
											&HMERecv.Data[120][i + 38]
											);

//				SplitString(CString(strFITTemp), ",", strTempArray1);
//				for (int j = 0; j < itemof(ECHANNELContent_POST); j++)
//				{
//					if (memcmp(ECHANNELContent_POST[j], "AAAAAA", 6) == 0)
//						break;
//					SplitString(CString(ECHANNELContent_POST[j]), ",", strTempArray);
//
//					if (memcmp(strTempArray[0], strTempArray1[0], 3) == 0	&&
//						memcmp(strTempArray[2], strTempArray1[2], 8) == 0)
//						PROTVERSIONDIFF[j] = 0;
//				}
				
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_ECHANNELPARM, GetSprintf("%s%d", "ECHANNEL", DownloadCurrNum-iCount), GetSprintf("%s",strFITTemp));
				
				i += 39;
			}
			
			m_pDevCmn->TranResult = TRUE;
			break;

		case TC_DOWNECHANNELPROT:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (IsNum(&HMERecv.Data[120][20], 6))
			{
				iCount = Asc2Int(&HMERecv.Data[120][20], 6);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iCount[%d]", iCount);
				if (iCount > 0)
				{
					fnAPP_SaveECTempData(&HMERecv.Data[120][26], iCount);
//					memset(EchannelProtContent, 0, sizeof(EchannelProtContent));
//					memcpy(EchannelProtContent, &HMERecv.Data[120][26], iCount);
//					SplitString(CString(EchannelProtContent), "\r\n", strTempArray);
//					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Row[%d]", strTempArray.GetSize());
//					for (i = 0; i < strTempArray.GetSize(); i++)
//					{
//						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Length[%d][%d]", i, strTempArray[i].GetLength());
//						if (strTempArray[i].GetLength() == 0)
//							break;
//						IniSetStr(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dRow%04d", DownloadEchannelProtCurrNum + 1, EProtRowNum + 1), strTempArray[i]);
//						EProtRowNum++;
//					}
					DownloadCurrNum += iCount;
					DownloadCurrProtLen -= iCount;
				}
				else
//					DownloadOverFlag = 1;
					break;

				if (DownloadCurrProtLen <= 0)
					DownloadOverFlag = 1;
			}
			else
//				DownloadOverFlag = 1;
				break;
//			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "EProtRowNum[%d]", EProtRowNum);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadOverFlag[%d]", DownloadOverFlag);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadCurrNum[%d]", DownloadCurrNum);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DownloadCurrProtLen[%d]", DownloadCurrProtLen);
			m_pDevCmn->TranResult = TRUE;
			break;

		case TC_DOWNAIDLIST:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (IsNum(&HMERecv.Data[120][3], 2))
			{
				iCount = Asc2Int(&HMERecv.Data[120][3], 2);
				DownloadCurrNum = iCount;
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_AIDLISTPARM, "AIDNUM", GetSprintf("%d", DownloadCurrNum));
			}

			i = 5;
			while ((iCount--) > 0)
			{
				memset(szFITTemp1, 0, sizeof(szFITTemp1));
				memset(szFITTemp2, 0, sizeof(szFITTemp2));
				memset(szFITTemp3, 0, sizeof(szFITTemp3));
				memcpy(szFITTemp1, &HMERecv.Data[120][i], 32);
				memcpy(szFITTemp2, &HMERecv.Data[120][i+32], 1);
				memcpy(szFITTemp3, &HMERecv.Data[120][i+32+1], 4);
				CString temp("");
				temp = GetSprintf("%s", szFITTemp1);
				temp.TrimRight();
				strFITTemp = GetSprintf("%s,%s,%s", temp.GetBuffer(0), szFITTemp2, szFITTemp3);

				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_AIDLISTPARM, GetSprintf("%s%d", "AID", DownloadCurrNum-iCount), GetSprintf("%s",strFITTemp));

				i += 37;
			}

			m_pDevCmn->TranResult = TRUE;
			break;
		case TC_DOWNPBOCPARA:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;
			
			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
			
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPBOCPARA)");

			if (HMERecv.DataRecvBit[120])
				;
			else
				return T_OK;

			if (IsNum(HMERecv.Data[120], 3))
			{
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_PBOCPARAMETER, 
					MakeUnPack(&HMERecv.Data[120][3], Asc2Int(HMERecv.Data[120], 3)));
			}

			if (Asc2Int(HMERecv.Data[120], 3) >= 42)
				;
			else
				return T_OK;

			InitInfo.nIcLoadMinNum = Asc2Int(&HMERecv.Data[120][3], 6);
			InitInfo.nIcLoadMaxNum = Asc2Int(&HMERecv.Data[120][9], 6);
			if (InitInfo.nIcLoadMinNum > 0)
			{
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ICLOADMINNUM , GetSprintf("%.0f",InitInfo.nIcLoadMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPBOCPARA) _INIKEY_TIMEDEFINE_ICLOADMINNUM[%d]", InitInfo.nIcLoadMinNum);
			}
			if (InitInfo.nIcLoadMaxNum > 0)
			{
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ICLOADMAXNUM , GetSprintf("%.0f",InitInfo.nIcLoadMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPBOCPARA) _INIKEY_TIMEDEFINE_ICLOADMAXNUM[%d]", InitInfo.nIcLoadMaxNum);
			}

			char temp[256];
			memset(temp, 0, sizeof(temp));
			memcpy(temp, &HMERecv.Data[120][15], 10);
			tacDenial = GetSprintf("%s", temp);
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TACDENIAL, tacDenial);
			memset(temp, 0, sizeof(temp));
			memcpy(temp, &HMERecv.Data[120][25], 10);
			tacOnline = GetSprintf("%s", temp);
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TACONLINE, tacOnline);
			memset(temp, 0, sizeof(temp));
			memcpy(temp, &HMERecv.Data[120][35], 10);
			tacDefault = GetSprintf("%s", temp);
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TACDEFAULT, tacDefault);
			
			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
		case TC_DOWNPARM:
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM)");
//			int		nWithMaxAmt = 0;									// Withdraw Max Amount
//			int		nWithMinAmt = 0;									// Withdraw Min Amount
////			double	dTransferMaxAmt = 0;								// Transfer Max Amount
//			int		nMsrSendTime = LINE_MSR_SEND_TIME;					// Msr Send Time
//			int		nTakeCard = TAKE_CARD_TIME_OUT;						// Take Card TimeOut
//			int		nTakeCash = TAKE_CASH_TIME_OUT;						// Take Cash TimeOut
//			int		nCWCRetryTime = 10;									// CWC Retry Time

			if (HMERecv.DataRecvBit[120])
				;
			else
				return T_OK;

			if (IsNum(HMERecv.Data[120], 3))
			{
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WORKPARAMETER, 
						  MakeUnPack(&HMERecv.Data[120][3], Asc2Int(HMERecv.Data[120], 3)));
			}

			if (Asc2Int(HMERecv.Data[120], 3) >= 64)
				;
			else
				return T_OK;


			nWithMaxAmt		= Asc2Int(&HMERecv.Data[120][3], 10);			// 以分为单位，只取前10位   by zjd
			nWithMinAmt		= Asc2Int(&HMERecv.Data[120][15], 10);			// 以分为单位，只取前10位   by zjd
			nDetailNum      = Asc2Int(&HMERecv.Data[120][27], 3);           // 查询明细交易的明细条数   by hyl
			nADSetNum       = Asc2Int(&HMERecv.Data[120][32], 3);           // 广告图片数量             by hyl
			nADTime         = Asc2Int(&HMERecv.Data[120][35], 6);           // 广告图片播放时间         by hyl
			nMsrSendTime	= Asc2Int(&HMERecv.Data[120][41], 6);			// 2005.09.08
			nTransRetryTime	= Asc2Int(&HMERecv.Data[120][47], 2);
			nCWCRetryTime	= Asc2Int(&HMERecv.Data[120][49], 2);
			nTakeCard		= Asc2Int(&HMERecv.Data[120][51], 3);
			nTakeCash		= Asc2Int(&HMERecv.Data[120][54], 3);
			nErrorNoticeTime= Asc2Int(&HMERecv.Data[120][57], 2);          //异常通知次数               by hyl
		//	dTransferMaxAmt = Asc2Float(&HMERecv.Data[48][35], 12) / 100;

		// 20080903 by yaokq
			if (Asc2Int(HMERecv.Data[120], 3) >= 168)
			{			
				nZztohMaxNum	= Asc2Float(&HMERecv.Data[120][59],8);
				nZztohMinNum	= Asc2Float(&HMERecv.Data[120][67],8);
				nDhtohMaxNum	= Asc2Float(&HMERecv.Data[120][75],8);
				nDhtohMinNum	= Asc2Float(&HMERecv.Data[120][83],8);
				nTzcktohMaxNum	= Asc2Float(&HMERecv.Data[120][91],8);
				nTzcktohMinNum	= Asc2Float(&HMERecv.Data[120][99],8);
				nHtodhMaxNum	= Asc2Float(&HMERecv.Data[120][107],8);
				nHtodhMinNum	= Asc2Float(&HMERecv.Data[120][115],8);
				nHtotzcqMaxNum	= Asc2Float(&HMERecv.Data[120][123],8);
				nHtotzcqMinNum	= Asc2Float(&HMERecv.Data[120][131],8);
				nHtozzMaxNum	= Asc2Float(&HMERecv.Data[120][139],8);
				nHtozzMinNum	= Asc2Float(&HMERecv.Data[120][147],8);
				nSaveKindPara	= Asc2Float(&HMERecv.Data[120][155],8);
			}
//////////////////////////////////////////////////////////////////////////////////////
			if (nTakeCard > 0)
			{
				InitInfo.TakeCardTimeOut = nTakeCard;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT , InitInfo.TakeCardTimeOut);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT[%d]", InitInfo.TakeCardTimeOut);
			}
			if (nTakeCash > 0)
			{
				InitInfo.TakeCashTimeOut = nTakeCash;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT , InitInfo.TakeCashTimeOut);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT[%d]", InitInfo.TakeCashTimeOut);
			}
			if (nTransRetryTime > 0)
			{
				InitInfo.TransRetryTime = nTransRetryTime;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANRETRYTIME, InitInfo.TransRetryTime);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_TRANRETRYTIME[%d]", InitInfo.TransRetryTime);
			}

			//20070910 by hyl 
			if (nDetailNum > 0)
			{
				InitInfo.DetailNum = nDetailNum;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DETAILNUM , InitInfo.DetailNum);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_DETAILNUM[%d]", InitInfo.DetailNum);
			}
			if (nADSetNum > 0)
			{
//				InitInfo.ADSetNum = nADSetNum;
//				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM , InitInfo.ADSetNum);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY__TIMEDEFINE_ADSETNUM[%d]", InitInfo.ADSetNum);
			}
			if (nADTime > 0)
			{
				InitInfo.ADTime = nADTime;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADTIME , InitInfo.ADTime);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_ADTIME[%d]", InitInfo.ADTime);
			}
			if (nErrorNoticeTime > 0)
			{
				InitInfo.ErrorNoticeTime = nErrorNoticeTime;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ERRORNOTICETIME , InitInfo.ErrorNoticeTime);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_ERRORNOTICETIME[%d]", InitInfo.ErrorNoticeTime);
			}
			if (nCWCRetryTime > 0)
			{
				InitInfo.CwcRetryTime = nCWCRetryTime;
				IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWCRETRYTIME , InitInfo.CwcRetryTime);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_CWCRETRYTIME[%d]", InitInfo.CwcRetryTime);
			}


			
			if ((m_pDevCmn->FstCSTValue == CASH_CHINA_100) ||
				(m_pDevCmn->FstCSTValue == CASH_CHINA_50))
			{
				if ((nWithMaxAmt < CSH_M10050_MINOUT) ||
					(nWithMaxAmt > CSH_M10050_MAXOUT))
					InitInfo.WithMaxAmt = CSH_M10050_MAXOUT;			// Withdraw Max Amount : 2004.11.05
				else
					InitInfo.WithMaxAmt = nWithMaxAmt;

				if ((nWithMinAmt < CSH_M10050_MINOUT) ||
					(nWithMinAmt > CSH_M10050_MAXOUT))
					InitInfo.WithMinAmt = CSH_M10050_MINOUT;			// Withdraw Min Amount : 2004.11.05
				else
					InitInfo.WithMinAmt = nWithMinAmt;

				if (InitInfo.WithMinAmt < CSH_M10050_MINOUT)
				{
					InitInfo.WithMinAmt = CSH_M10050_MINOUT;
				}
				else
				if (InitInfo.WithMinAmt > CSH_M10050_MAXOUT)	
			        InitInfo.WithMinAmt = CSH_M10050_MAXOUT;
			}
			else
			if ((m_pDevCmn->FstCSTValue == CASH_CHINA_20) ||
				(m_pDevCmn->FstCSTValue == CASH_CHINA_10))
			{
				if ((nWithMaxAmt < CSH_M2010_MINOUT) ||
					(nWithMaxAmt > CSH_M2010_MAXOUT))
					InitInfo.WithMaxAmt = CSH_M2010_MAXOUT;				// Withdraw Max Amount : 2004.11.05
				else
					InitInfo.WithMaxAmt = nWithMaxAmt;

				if ((nWithMinAmt < CSH_M2010_MINOUT) ||
					(nWithMinAmt > CSH_M2010_MAXOUT))
					InitInfo.WithMinAmt = CSH_M2010_MINOUT;			// Withdraw Min Amount : 2004.11.05
				else
					InitInfo.WithMinAmt = nWithMinAmt;
			}

			/////////////////////////////////////////////////////////////////////////////
			 m_pDevCmn->fnAPL_SetCashInfo(CSH_M10050_MAXOUT, InitInfo.WithMaxAmt);
			 m_pDevCmn->fnAPL_SetCashInfo(CSH_M10050_MINOUT, InitInfo.WithMinAmt);
																  // 2007.06.06
			/////////////////////////////////////////////////////////////////////////////
			 
			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, InitInfo.WithMaxAmt);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_WITHMAXAMOUNT[%d]", InitInfo.WithMaxAmt);
            
		    IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMINAMOUNT, InitInfo.WithMinAmt);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_WITHMINAMOUNT[%d]", InitInfo.WithMinAmt);

		//	InitInfo.TransferMaxAmt = dTransferMaxAmt;					// Transfer Max Amount : 2005.09.11
		//	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXAMOUNT, GetSprintf("%12.12s", &HMERecv.Data[48][35]));
		//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_BidSetWorkParamProc() _INIKEY_TIMEDEFINE_TRANMAXAMOUNT[%s]", GetSprintf("%12.12s", &HMERecv.Data[48][35]));

			if (nMsrSendTime < 30)										// Msr Send Time
				InitInfo.MsrSendTime = 30;
			else
			if (nMsrSendTime > 3600)									// Msr Send Time
				InitInfo.MsrSendTime = 3600;
			else
				InitInfo.MsrSendTime = nMsrSendTime;

			IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME, InitInfo.MsrSendTime);
			

		// 20080903 by yaokq
			if (nZztohMaxNum > 0)
			{
				InitInfo.ZztohMaxNum = nZztohMaxNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMAXNUM , GetSprintf("%.0f",InitInfo.ZztohMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_ZZTOHMAXNUM[%f]", InitInfo.ZztohMaxNum);
			}

			if (nZztohMinNum > 0)
			{
				InitInfo.ZztohMinNum = nZztohMinNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMINNUM , GetSprintf("%.0f",InitInfo.ZztohMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_ZZTOHMINNUM[%f]", InitInfo.ZztohMinNum);
			}

			if (nDhtohMaxNum > 0)
			{
				InitInfo.DhtohMaxNum = nDhtohMaxNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMAXNUM, GetSprintf("%.0f",InitInfo.DhtohMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_DHTOHMAXNUM[%f]", InitInfo.DhtohMaxNum);
			}

			if (nDhtohMinNum > 0)
			{
				InitInfo.DhtohMinNum = nDhtohMinNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMINNUM, GetSprintf("%.0f",InitInfo.DhtohMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_DHTOHMINNUM[%f]", InitInfo.DhtohMinNum);
			}

			if (nTzcktohMaxNum> 0)
			{
				InitInfo.TzcktohMaxNum = nTzcktohMaxNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMAXNUM, GetSprintf("%.0f",InitInfo.TzcktohMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_TZCKTOHMAXNUM[%f]", InitInfo.TzcktohMaxNum);
			}

			if (nTzcktohMinNum> 0)
			{
				InitInfo.TzcktohMinNum = nTzcktohMinNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMINNUM, GetSprintf("%.0f",InitInfo.TzcktohMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_TZCKTOHMINNUM[%f]", InitInfo.TzcktohMinNum);
			}

			if (nHtodhMaxNum > 0)
			{
				InitInfo.HtodhMaxNum = nHtodhMaxNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMAXNUM, GetSprintf("%.0f",InitInfo.HtodhMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_HTODHMAXNUM[%f]", InitInfo.HtodhMaxNum);
			}

			if (nHtodhMinNum > 0)
			{
				InitInfo.HtodhMinNum = nHtodhMinNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMINNUM, GetSprintf("%.0f",InitInfo.HtodhMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_HTODHMINNUM[%f]", InitInfo.HtodhMinNum);
			}

			if (nHtotzcqMaxNum > 0)
			{
				InitInfo.HtotzcqMaxNum = nHtotzcqMaxNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM, GetSprintf("%.0f",InitInfo.HtotzcqMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM[%f]", InitInfo.HtotzcqMaxNum);
			}

			if (nHtotzcqMinNum > 0)
			{
				InitInfo.HtotzcqMinNum = nHtotzcqMinNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMINNUM, GetSprintf("%.0f",InitInfo.HtotzcqMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_HTOTZCQMINNUM[%f]", InitInfo.HtotzcqMinNum);
			}

			if (nHtozzMaxNum > 0)
			{
				InitInfo.HtozzMaxNum = nHtozzMaxNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMAXNUM, GetSprintf("%.0f",InitInfo.HtozzMaxNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_HTOZZMAXNUM[%f]", InitInfo.HtozzMaxNum);
			}

			if (nHtozzMinNum > 0)
			{
				InitInfo.HtozzMinNum = nHtozzMinNum;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMINNUM, GetSprintf("%.0f",InitInfo.HtozzMinNum));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_HTOZZMINNUM[%f]", InitInfo.HtozzMinNum);
			}

			if (nSaveKindPara > 0)
			{
				InitInfo.SaveKindPara = nSaveKindPara;
				IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SAVEKINDPARA, GetSprintf("%.0f",InitInfo.SaveKindPara));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_SAVEKINDPARA[%f]", InitInfo.SaveKindPara);
			}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM) _INIKEY_TIMEDEFINE_IDLERETRYTIME[%d]", InitInfo.MsrSendTime);

		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPARM): return()");

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
			
		case TC_DOWNPAY:
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPAY): return()");
			if (memcmp(HostResp.RejectCode, "00", 2) != 0)
				break;

			if (memcmp(HMESend.Data[11], HMERecv.Data[11], HMESend.DataLength[11]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850112", "SERIAL NO MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			if (memcmp(HMESend.Data[41], HMERecv.Data[41], HMESend.DataLength[41]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850115", "ATM ID MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}

			DownloadOverFlag = 1;

			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPAY): HMERecv.DataLength[120]=[%d]",HMERecv.DataLength[120]);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPAY): HMERecv.DataLength[123]=[%d]",HMERecv.DataLength[123]);

			if (HMERecv.DataLength[120] > 3)
			{
				strFITTemp = GetSprintf("%3.3s",HMERecv.Data[120]);
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYFLAG1", strFITTemp);
				strFITTemp = GetSprintf("%s",&HMERecv.Data[120][3]);
				strFITTemp.TrimRight();
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYDATA1", strFITTemp);
			}
			else
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYFLAG1", "000");

			if (HMERecv.DataLength[123] > 3)
			{
				strFITTemp = GetSprintf("%3.3s",HMERecv.Data[123]);
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYFLAG2", strFITTemp);
				strFITTemp = GetSprintf("%s",&HMERecv.Data[123][3]);
				strFITTemp.TrimRight();
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYDATA2", strFITTemp);
			}
			else
				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_PAYPARM, "PAYFLAG2", "000");
			
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost(TC_DOWNPAY): return()");

			m_pDevCmn->TranResult = TRUE;						// Hosk Ok
			break;
			
		default:
			break;
	}

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus == TRAN_TRAN)						// 2010.10.20
	{
		if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
			(m_pDevCmn->McuReadMSOnlyFlag))
			;
		else
		{
			if ((TranCode == TC_ICINQUIRY)	||					// 2010.10.20
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
					TranCode == TC_ECHANNEL_INQ	||
					TranCode == TC_ECHANNEL		||
					TranCode == TC_INQUIRY_CheckPin)
				{
					fnAPD_PbcSetResponse();							// Pboc Set Response
					fnAPD_PbcTerminate();							// Pboc Terminate
				}
			}
		}
	}
/////////////////////////////////////////////////////////////////////////////
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvHost():return");
	return T_OK;
}

// Recv Analysis
int CTranCmn::fnAPP_RecvAnal()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()");

	int		Cnt = 0;
	int		Length1 = 0;
	int		nTempLength = 0;
	char	szTmpBit[1024];										// 2005.09.11
	char	szTmpBuff[1024];
	CString	strTemp("");
	BYTE	BT = 0x01;
	
	memset(szTmpBit, 0, sizeof(szTmpBit));
	memset(szTmpBuff, 0, sizeof(szTmpBuff));

	MacLength = 0;												// Mac Length
	memset(MacBuffer, 0, sizeof(MacBuffer));					// Mac Buffer

	// Recv Data Field Length
	HMERecv.DataLength[0]  = 4;
	HMERecv.DataLength[1]  = MAXBITMAP/8;
	HMERecv.DataLength[2]  = -99;
	HMERecv.DataLength[3]  = 6;
	HMERecv.DataLength[4]  = 12;
	HMERecv.DataLength[7]  = 2;
	HMERecv.DataLength[11] = 8;
	HMERecv.DataLength[12] = 6;
	HMERecv.DataLength[13] = 8;
	HMERecv.DataLength[14] = 8;
	HMERecv.DataLength[15] = 8;
	HMERecv.DataLength[16] = 2;
	HMERecv.DataLength[18] = 4;
	HMERecv.DataLength[20] = 8;
	HMERecv.DataLength[21] = 36;
	HMERecv.DataLength[22] = 3;
	HMERecv.DataLength[23] = 3;
	HMERecv.DataLength[24] = 80;
	HMERecv.DataLength[25] = 2;
	HMERecv.DataLength[28] = 8;
	HMERecv.DataLength[29] = 8;
	HMERecv.DataLength[30] = 8;
	HMERecv.DataLength[35] = -99;
	HMERecv.DataLength[36] = -999;
	HMERecv.DataLength[37] = 12;
	HMERecv.DataLength[38] = 16;
	HMERecv.DataLength[39] = 2;
	HMERecv.DataLength[40] = -99;
	HMERecv.DataLength[41] = 8;
	HMERecv.DataLength[42] = 15;
	HMERecv.DataLength[45] = -999;			//20140119 jl
	HMERecv.DataLength[46] = 10;
	HMERecv.DataLength[49] = 3;
	HMERecv.DataLength[51] = 7;
	HMERecv.DataLength[52] = 8;
	HMERecv.DataLength[53] = 16;
	HMERecv.DataLength[54] = 39;
	HMERecv.DataLength[55] = -999;
	HMERecv.DataLength[59] = -999;			// added by liuxl 20110518
	HMERecv.DataLength[60] = 4;
	HMERecv.DataLength[61] = -99;
	HMERecv.DataLength[62] = 8;
	HMERecv.DataLength[63] = -99;
	HMERecv.DataLength[68] = -99;			// 2008.10.05
	HMERecv.DataLength[72] = 4;
	HMERecv.DataLength[90] = 40;
	HMERecv.DataLength[93] = 3;
	HMERecv.DataLength[96] = -99;
	HMERecv.DataLength[102] = -99;
	HMERecv.DataLength[103] = -99;
	HMERecv.DataLength[106] = 16;			// 2008.10.05
	HMERecv.DataLength[120] = -999;
	HMERecv.DataLength[121] = -999;
	HMERecv.DataLength[122] = -999;
	HMERecv.DataLength[123] = -999;
	HMERecv.DataLength[128] = 8;

	// 0 Transaction Kind
	memcpy(HMERecv.Data[0], &RecvBuffer[Cnt], HMERecv.DataLength[0]);
	Cnt += HMERecv.DataLength[0];

	memset(szTmpBuff, 0, sizeof(szTmpBuff));
	nTempLength = HMERecv.DataLength[0];
	fnAPL_FormatMacData(HMERecv.Data[0], nTempLength, szTmpBuff);
	memcpy(&MacBuffer[MacLength], szTmpBuff, nTempLength);
	MacLength += nTempLength;

	if (iDEBUGFLAG)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()HMERecv.Data[0][%s]", HMERecv.Data[0]);

	if ((TranCode == TC_INQUIRY)		||
		(TranCode == TC_CUS_INF_INQ)	||					// yusy add 2010.03.22
		(TranCode == TC_TRANSFER)		||
		(TranCode == TC_LOAD)			||
		(TranCode == TC_HZT)			||					// liyi add 绿卡通
		(TranCode == TC_TZH)			||
		(TranCode == TC_WITHDRAWAL)		||
		(TranCode == TC_DETAIL)			||
		(TranCode == TC_PAY)			||
		(TranCode == TC_PAY_INQ)		||
		(TranCode == TC_CHANGEPIN)		||
		(TranCode == TC_OTHER_INQ)		||
		(TranCode == TC_ECHANNEL_INQ)	||
		(TranCode == TC_SMSPARITY)		||
		(TranCode == TC_ECHANNEL)		||					// 20080904 by yaokq
		(TranCode == TC_INQUIRY_CheckPin)||
		(TranCode == TC_CUS_INF_INQ_OTHERBANK))				//20140119 jl
	{
		if (memcmp(HMERecv.Data[0], "0210", HMERecv.DataLength[0]) != 0)
		{
			if (TranCode == TC_WITHDRAWAL)
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(16);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
			}
			else
			if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)		// liyi add 绿卡通
			{
/////////////////////////////////////////////////////////////////////////////
				fnAPL_WriteTransactonEjr(20);					// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
			}
			m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if ((TranCode == TC_REMIT_ADD)		||
		(TranCode == TC_REMIT_IN)		||
		(TranCode == TC_REMIT_PWD)		||
		(TranCode == TC_REMIT)			||
		(TranCode == TC_REMIT_INF_INQ))				// 2008.10.05
	{
		if (memcmp(HMERecv.Data[0], "9110", HMERecv.DataLength[0]) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_BOUNDMDOIFY)
	{
		if (memcmp(HMERecv.Data[0], "0610", HMERecv.DataLength[0]) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_CANCEL)
	{
		if (TranCode2 == 2)				// 2008.10.13
		{
			if (memcmp(HMERecv.Data[0], "9130", HMERecv.DataLength[0]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
				fnAPP_CancelProc(T_RECVERROR);
			}
		}
		else
		{
			if (memcmp(HMERecv.Data[0], "0430", HMERecv.DataLength[0]) != 0)
			{
				m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
				fnAPP_CancelProc(T_RECVERROR);
			}
		}
	}
	else
		if ((TranCode == TC_TOTAL)		||
			(TranCode == TC_TOTALPRE)	||
			(TranCode == TC_DOWNFIT)	||
			(TranCode == TC_DOWNICFIT)	||		// added by yusy ic
			(TranCode == TC_DOWNAIDLIST)||
			(TranCode == TC_DOWNPBOCPARA)||
			(TranCode == TC_DOWNPARM)	||
			(TranCode == TC_DOWNPAY)	||
			(TranCode == TC_SENDDETAIL)	||
			(TranCode == TC_DETAILEND)	||
			(TranCode == TC_DOWNECHANNELPARA)	||
			(TranCode == TC_DOWNECHANNELPROT))
	{
		if (memcmp(HMERecv.Data[0], "0810", HMERecv.DataLength[0]) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if ((TranCode == TC_OPEN)		||
		(TranCode == TC_ERROR)		||
		(TranCode == TC_AEXSTATUS)	||
		(TranCode == TC_CARDRETAIN)	||
		(TranCode == TC_TOTALKIND)	||
		(TranCode == TC_OEXSTATUS)	||
		(TranCode == TC_MSRSTATUS)	||
		(TranCode == TC_ADDCASH)	||
		(TranCode == TC_SUBCASH)	||
		(TranCode == TC_CLEARCASH)	||
		(TranCode == TC_PRINTCASH))
	{
		if (memcmp(HMERecv.Data[0], "0830", HMERecv.DataLength[0]) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850191", "RECV HEADER FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}

	// 1 Bitmap
//	memcpy(HMERecv.Data[1], &RecvBuffer[Cnt], HMERecv.DataLength[1]);
	if ((RecvBuffer[Cnt] & (BT << 7)) >> 7)
		HMERecv.DataLength[1] = MAXBITMAP/8;
	else
	{
		HMERecv.DataLength[1] = MAXBITMAP2/8;
	}

	if (iDEBUGFLAG)
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HMERecv.DataLength[1] = [%d]", HMERecv.DataLength[1]);
	memcpy(HMERecv.Data[1], &RecvBuffer[Cnt], HMERecv.DataLength[1]);
	Cnt += HMERecv.DataLength[1];

	memset(szTmpBit, 0, sizeof(szTmpBit));
	memcpy(szTmpBit, HMERecv.Data[1], HMERecv.DataLength[1]);

//	BT = 0x01;
//	szTmpBit[0] |= BT << 7;

	for (int i = 2; i < MAXBITMAP; i++)
	{
		BT = 0x01;
		if ((szTmpBit[(i-1)/8] & (BT<<(7-(i-1)%8))) >> (7-(i-1)%8))
		{
			HMERecv.DataRecvBit[i] = TRUE;
			if (-99 == HMERecv.DataLength[i])
			{
				Length1 = 0;
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				memcpy(szTmpBuff, RecvBuffer + Cnt, 2);	
				Cnt += 2;
				memcpy(HMERecv.Data[i], szTmpBuff, 2);			// LL
				Length1 = Asc2Int(szTmpBuff, 2);
				memcpy(HMERecv.Data[i] + 2, &RecvBuffer[Cnt], Length1);
				HMERecv.DataLength[i] = 2 + Length1;
				Cnt += Length1;
				if (iDEBUGFLAG)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()HMERecv.Data[%d][%s], Length[%d]", i, HMERecv.Data[i], Length1);
				if ((i == 2)   || (i == 3)   || (i == 4)  || 
					(i == 11)  || (i == 12)  || (i == 13) || 
					(i == 39)  || ((i == 53) && (TranCode == TC_OPEN))  || (i == 68) || 
					(i == 102) || (i == 103))
				{
					memset(szTmpBuff, 0, sizeof(szTmpBuff));
					nTempLength = HMERecv.DataLength[i];
					fnAPL_FormatMacData(HMERecv.Data[i], nTempLength, szTmpBuff);
					memcpy(&MacBuffer[MacLength], szTmpBuff, nTempLength);
					MacLength += nTempLength;
				}
				continue;
			}
			else
			if (-999 == HMERecv.DataLength[i])
			{
				Length1 = 0;
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				memcpy(szTmpBuff, RecvBuffer + Cnt, 3);
				Cnt += 3;
				memcpy(HMERecv.Data[i], szTmpBuff, 3);			// LLL
				Length1 = Asc2Int(szTmpBuff, 3);
				memcpy(HMERecv.Data[i] + 3, &RecvBuffer[Cnt], Length1);
				HMERecv.DataLength[i] = 3 + Length1;
				Cnt += Length1;
				if ((i == 2)   || (i == 3)   || (i == 4)  || 
					(i == 11)  || (i == 12)  || (i == 13) || 
					(i == 39)  || ((i == 53) && (TranCode == TC_OPEN))  || (i == 68) || 
					(i == 102) || (i == 103))
				{
					memset(szTmpBuff, 0, sizeof(szTmpBuff));
					nTempLength = HMERecv.DataLength[i];
					fnAPL_FormatMacData(HMERecv.Data[i], nTempLength, szTmpBuff);
					memcpy(&MacBuffer[MacLength], szTmpBuff, nTempLength);
					MacLength += nTempLength;
				}
				if (iDEBUGFLAG)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()HMERecv.Data[%d][%s], Length[%d]", i, HMERecv.Data[i], Length1);
				continue;
			}

			memcpy(HMERecv.Data[i], &RecvBuffer[Cnt], HMERecv.DataLength[i]);
			Cnt += HMERecv.DataLength[i];
			if (iDEBUGFLAG)
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()HMERecv.Data[%d][%s]", i, HMERecv.Data[i]);

			if ((i == 2)   || (i == 3)   || (i == 4)  || 
				(i == 11)  || (i == 12)  || (i == 13) || 
				(i == 39)  || ((i == 53) && (TranCode == TC_OPEN))  || (i == 68) || 
				(i == 102) || (i == 103))
			{
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				nTempLength = HMERecv.DataLength[i];
				fnAPL_FormatMacData(HMERecv.Data[i], nTempLength, szTmpBuff);
				memcpy(&MacBuffer[MacLength], szTmpBuff, nTempLength);
				MacLength += nTempLength;
			}
		}
	}

	if (HMERecv.DataRecvBit[37])
	{
		memcpy(HostResp.ReferenceNo, HMERecv.Data[37], sizeof(HostResp.ReferenceNo));
	}

	if (HMERecv.DataRecvBit[39])
	{
		memcpy(HostResp.RejectCode, HMERecv.Data[39], sizeof(HostResp.RejectCode));
	}

	// added by yusy ic
	if (HMERecv.DataRecvBit[55])
	{
		memset(IsuAuthData, 0, sizeof(IsuAuthData));
		memset(IcScriptData, 0, sizeof(IcScriptData));
		memset(IcScriptData2, 0, sizeof(IcScriptData2));
		IsuAuthDataLen = 0;
		IcScriptDataLen = 0;
		IcScriptDataLen2 = 0;
		int len = HMERecv.DataLength[55];
		int tempLen = 3;
		while(tempLen < len)
		{
			BYTE BT = 0x80;
			BYTE CC = HMERecv.Data[55][tempLen];
			BYTE CTemp;
			CString strTemp("");
			
			for(int j=3;j<8;j++) 
			{
				CTemp = (CC<<j) & BT;
				if(CTemp == 0)
					strTemp = strTemp + "0";
				else
					strTemp = strTemp + "1";
			}
			CString tagName("");
			char strlen[2];
			char tempTag[2];
			memset(tempTag, 0, sizeof(tempTag));
			memset(strlen, 0, sizeof(strlen));
			if(memcmp(strTemp, "11111", 5) == 0)
			{
				memcpy(tempTag, &HMERecv.Data[55][tempLen], 2);
				tagName = MakeUnPack(tempTag, 2);
				tempLen += 2;
			}
			else
			{
				memcpy(tempTag, &HMERecv.Data[55][tempLen], 1);
				tagName = MakeUnPack(tempTag, 1);
				tempLen += 1;
			}
			sprintf(strlen, "%d", HMERecv.Data[55][tempLen]);
			int len = atoi(strlen);
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "tagName:[%s]len:[%d]", tagName, len);
			tempLen += 1;
			if(memcmp(tagName, "91", 2) == 0)
			{
				IsuAuthDataLen = len;
				memcpy(IsuAuthData, &HMERecv.Data[55][tempLen], len);
			}
			else
			if(memcmp(tagName, "71", 2) == 0)
			{
				ICScriptWriteFlag = 1;
				IcScriptDataLen = len;
				memcpy(IcScriptData, &HMERecv.Data[55][tempLen], len);
			}
			else
			if(memcmp(tagName, "72", 2) == 0)
			{
				ICScriptWriteFlag = 1;
				IcScriptDataLen2 = len;
				memcpy(IcScriptData2, &HMERecv.Data[55][tempLen], len);
			}
			tempLen += len;
		}

	}

	HostResp.CardRetractFlag = FALSE;								//2008.12.10
	// liyi add 绿卡通 判断93域吞卡标志位吞卡 2008.09.20
	if (HMERecv.DataRecvBit[93])
	{
		if (memcmp(&HMERecv.Data[93][2], "1", 1) == 0)
		{
			HostResp.CardRetractFlag = TRUE;
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO, GetSprintf("%2.2s", HostResp.RejectCode));
		}
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()HMERecv.Data[93][2]=[%1.1s]", &HMERecv.Data[93][2]);
	}

	if (HMERecv.DataRecvBit[128])
	{
		memcpy(HostResp.MacData, HMERecv.Data[128], sizeof(HostResp.MacData));
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal()HostResp.RejectCode[%2.2s]", HostResp.RejectCode);

/////////////////////////////////////////////////////////////////////////////
	if (TranCode == TC_INQUIRY)
	{
		if (memcmp(HMERecv.Data[3], "100100", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_INQUIRY_CheckPin)
	{
		if (memcmp(HMERecv.Data[3], "100100", 6) != 0)						//jl20140307
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_TRANSFER)
	{
		if (memcmp(HMERecv.Data[3], "110500", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_LOAD)
	{
		if (memcmp(HMERecv.Data[3], "111100", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_BOUNDMDOIFY)
	{
		if (memcmp(HMERecv.Data[3], "150400", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_REMIT)			// 2008.10.05
	{
		if (memcmp(HMERecv.Data[3], "180100", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD)
	{
		if (memcmp(HMERecv.Data[3], "180200", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_REMIT_INF_INQ)
	{
		if (memcmp(HMERecv.Data[3], "180500", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_REMIT_IN)
	{
		if (memcmp(HMERecv.Data[3], "180600", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_HZT || TranCode == TC_TZH)				//liyi add 绿卡通
	{
		if (memcmp(HMERecv.Data[3], "110600", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_WITHDRAWAL)
	{
		if (memcmp(HMERecv.Data[3], "110100", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_CHANGEPIN)
	{
		if (memcmp(HMERecv.Data[3], "150100", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else
	if (TranCode == TC_DETAIL)
	{
		if (memcmp(HMERecv.Data[3], "100200", 6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else if (TranCode == TC_OTHER_INQ)   // 20080904 by yaokq for 其它储种子账户查询 
	{
		if(memcmp(HMERecv.Data[3],"100400",6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else if (TranCode == TC_SMSPARITY)
	{
		if(memcmp(HMERecv.Data[3],"150500",6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else if (TranCode == TC_ECHANNEL_INQ) 
	{
		if(memcmp(HMERecv.Data[3],"100700",6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
	else if (TranCode == TC_ECHANNEL)
	{
		if(memcmp(HMERecv.Data[3],"141400",6) != 0)
		{
			m_pDevCmn->fnAPL_StackError("8850192", "RECV DETAIL FAIL");
			fnAPP_CancelProc(T_RECVERROR);
		}
	}
/////////////////////////////////////////////////////////////////////////////
	if (TranCode == TC_MSRSTATUS)
	{
		fnAPP_SetHostTimeProc();								// BCTC : 2005.09.07
	}

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->TranStatus == TRAN_TRAN)						// TRANSACTION : 2005.09.11
	{
//		if (HMERecv.DataRecvBit[32])
//		{
//			if (HMERecv.DataLength[32] > 2)						// LL Data Type
//				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD32, 
//						  MakeUnPack(&HMERecv.Data[32][2], HMERecv.DataLength[32] - 2));
//		}
//		if (HMERecv.DataRecvBit[33])
//		{
//			if (HMERecv.DataLength[33] > 2)						// LL Data Type
//				IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD33, 
//						  MakeUnPack(&HMERecv.Data[33][2], HMERecv.DataLength[33] - 2));
//		}
		if (HMERecv.DataRecvBit[37])
		{
			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO, 
					  MakeUnPack(HMERecv.Data[37], HMERecv.DataLength[37]));
		}

/////////////////////////////////////////////////////////////////////////////
//		if (TranCode == TC_WITHDRAWAL)							// 2005.09.27
//		{
//			if (HMERecv.DataRecvBit[32])
//			{
//				if (HMERecv.DataLength[32] > 2)					// LL Data Type
//					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD32, 
//							  MakeUnPack(&HMERecv.Data[32][2], HMERecv.DataLength[32] - 2));
//			}
//			if (HMERecv.DataRecvBit[33])
//			{
//				if (HMERecv.DataLength[33] > 2)					// LL Data Type
//					IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFIELD33, 
//							  MakeUnPack(&HMERecv.Data[33][2], HMERecv.DataLength[33] - 2));
//			}
//		}
	}

/////////////////////////////////////////////////////////////////////////////
	if (memcmp(HostResp.RejectCode, "Z5", 2) == 0 ||            //Mac/Pin error return card by hyl
		memcmp(HostResp.RejectCode, "ZI", 2) == 0 )				// Mac Mismatch   by zjd
	{
// 		if (TranProc == TRAN_WITH)								// 2004.08.19 added by liuxl 20110714
// 		{
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
// 		}
// 
// 		if (TranCode == TC_REMIT_ADD || TranCode == TC_REMIT_PWD || TranCode == TC_REMIT_IN)		// 2008.10.13
// 		{
// 			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG, 0);
// 			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATA, "");
//			IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANFIELD55, "");	// added by liuxl 20110508
// 		}
		HostNms.RqkSendFlag = TRUE;     
		HostResp.CardEjectFlag = TRUE;
	}
	else
	if (memcmp(HostResp.RejectCode, "00", 2) == 0)					// Host Ok : 2005.10.04
	{
		if ((TranCode == TC_INQUIRY)		||						// Mac Data Check
			(TranCode == TC_CUS_INF_INQ)	||	
			(TranCode == TC_TRANSFER)		||
			(TranCode == TC_LOAD)			||
			(TranCode == TC_BOUNDMDOIFY)	||
			(TranCode == TC_HZT)			||						// liyi add 绿卡通
			(TranCode == TC_TZH)			||
			(TranCode == TC_REMIT)			||
			(TranCode == TC_REMIT_ADD)		||
			(TranCode == TC_REMIT_IN)		||
			(TranCode == TC_REMIT_PWD)		||
			(TranCode == TC_REMIT_INF_INQ)	||
			(TranCode == TC_PAY)			||
			(TranCode == TC_PAY_INQ)		||
			(TranCode == TC_WITHDRAWAL)		||
			(TranCode == TC_CHANGEPIN)		||
			(TranCode == TC_DETAIL)			||
//			(TranCode == TC_CANCEL)			||
			(TranCode == TC_OTHER_INQ)		||
			(TranCode == TC_ECHANNEL_INQ)	||
			(TranCode == TC_ECHANNEL)		||			// 20080904 by yaokq
			(TranCode == TC_INQUIRY_CheckPin)||
			(TranCode == TC_CUS_INF_INQ_OTHERBANK))
		{
			memcpy(HostResp.MacData, HMERecv.Data[128], sizeof(HostResp.MacData));

			strTemp.Format("%s", MacBuffer);
			strTemp.TrimLeft();
			strTemp.TrimRight();
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal() MACDATA[%s]", strTemp);
			char szMacData[8];

			if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)	
			{
				CString MacData("");
				m_pDevCmn->fnPIN_MacingData(MakeUnPack(strTemp.GetBuffer(0), strTemp.GetLength()));
				MacData = m_pDevCmn->fstrPIN_GetMacingData();
				MakePack(MacData.GetBuffer(0), szMacData, 16);
			}
	        else
			    fnAPL_GetMACData(strTemp, (UCHAR *)SaveMACKEY, (UCHAR *)szMacData);
			
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal() MACRESULT[%s]", MakeUnPack(szMacData, 8));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal() HostResp.MacData[%s]", MakeUnPack(HostResp.MacData,8));

			if (memcmp(szMacData, HostResp.MacData, 8) != 0)
			{
				if (TranCode == TC_WITHDRAWAL)
				{
/////////////////////////////////////////////////////////////////////////////
					fnAPL_WriteTransactonEjr(16);				// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				}
				else
				if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)		//liyi add 绿卡通
				{
/////////////////////////////////////////////////////////////////////////////
					fnAPL_WriteTransactonEjr(20);				// 2005.05.26
/////////////////////////////////////////////////////////////////////////////
				}
				HostNms.RqkSendFlag = TRUE;
				m_pDevCmn->fnAPL_StackError("8850102", "MAC MISMATCH");
				fnAPP_CancelProc(T_RECVERROR);
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////
//	HostResp.CardRetractFlag = FALSE;							// 2008.12.10
	HostResp.CardEjectFlag = FALSE;
															
//	if ((memcmp(HostResp.RejectCode, "09", 2) == 0) || 			// liyi add 绿卡通 通过93域判断吞卡
//		(memcmp(HostResp.RejectCode, "11", 2) == 0) ||
//		(memcmp(HostResp.RejectCode, "13", 2) == 0) ||
//		(memcmp(HostResp.RejectCode, "14", 2) == 0) ||
//		(memcmp(HostResp.RejectCode, "20", 2) == 0) ||
//		(memcmp(HostResp.RejectCode, "21", 2) == 0) ||
//		(memcmp(HostResp.RejectCode, "23", 2) == 0))			// Retract Card Code(Host Order)   主机返回吞卡  by zjd
//	{
//		HostResp.CardRetractFlag = TRUE;
//		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO, GetSprintf("%2.2s", HostResp.RejectCode));
//	}
// 	else
	if (memcmp(HostResp.RejectCode, "Z5", 2) == 0 ||            //Mac/Pin error return card by hyl
		memcmp(HostResp.RejectCode, "ZI", 2) == 0 )				// Mac Mismatch   by zjd
	{
		HostNms.RqkSendFlag = TRUE;     
		HostResp.CardEjectFlag = TRUE;
	}
// 	else
// 	if (memcmp(HostResp.RejectCode,"26",2) == 0)
// 	{
// 		if ((m_pDevCmn->AtmDefine.ICCardDevice == NOT_SET) ||	// 2010.10.20
// 			(m_pDevCmn->McuReadMSOnlyFlag))
// 			;
// 		else
// 			HostResp.CardEjectFlag = TRUE;
// 	}
	else
	if (memcmp(HostResp.RejectCode,"27",2) == 0	||
		(memcmp(HostResp.RejectCode,"2H",2) == 0 && TranCode == TC_INQUIRY_CheckPin)	||
		(memcmp(HostResp.RejectCode,"12",2) == 0 && TranCode == TC_INQUIRY_CheckPin)	||
		memcmp(HostResp.RejectCode,"7I",2) == 0	||
		memcmp(HostResp.RejectCode,"04",2) == 0	||
		memcmp(HostResp.RejectCode,"25",2) == 0	||
		memcmp(HostResp.RejectCode,"N8",2) == 0	||
		memcmp(HostResp.RejectCode,"97",2) == 0	||
		memcmp(HostResp.RejectCode,"2N",2) == 0	||
		memcmp(HostResp.RejectCode,"22",2) == 0	||
		memcmp(HostResp.RejectCode,"31",2) == 0	||
		memcmp(HostResp.RejectCode,"30",2) == 0	||
		memcmp(HostResp.RejectCode,"17",2) == 0	||
		memcmp(HostResp.RejectCode,"10",2) == 0	||
		memcmp(HostResp.RejectCode,"08",2) == 0	||
		memcmp(HostResp.RejectCode,"E5",2) == 0 ||
		memcmp(HostResp.RejectCode,"3I",2) == 0 ||
		memcmp(HostResp.RejectCode,"Z6",2) == 0 ||
		memcmp(HostResp.RejectCode,"ZZ",2) == 0 ||
		memcmp(HostResp.RejectCode,"79",2) == 0 ||
		memcmp(HostResp.RejectCode,"5A",2) == 0 ||
		memcmp(HostResp.RejectCode,"5B",2) == 0 ||
		memcmp(HostResp.RejectCode,"5C",2) == 0 ||
		memcmp(HostResp.RejectCode,"5D",2) == 0 ||
		memcmp(HostResp.RejectCode,"5E",2) == 0 ||
		memcmp(HostResp.RejectCode,"5F",2) == 0 ||
		memcmp(HostResp.RejectCode,"5G",2) == 0 ||
		memcmp(HostResp.RejectCode,"5H",2) == 0 ||
		memcmp(HostResp.RejectCode,"5I",2) == 0 ||
		memcmp(HostResp.RejectCode,"5J",2) == 0 ||
		memcmp(HostResp.RejectCode,"5K",2) == 0 ||
		memcmp(HostResp.RejectCode,"5L",2) == 0 ||
		memcmp(HostResp.RejectCode,"5M",2) == 0 ||
		memcmp(HostResp.RejectCode,"5N",2) == 0 ||
		memcmp(HostResp.RejectCode,"5O",2) == 0 ||
		memcmp(HostResp.RejectCode,"5P",2) == 0 ||
		memcmp(HostResp.RejectCode,"5Q",2) == 0 ||
		memcmp(HostResp.RejectCode,"5R",2) == 0 ||
		memcmp(HostResp.RejectCode,"5V",2) == 0 ||
		memcmp(HostResp.RejectCode,"6G",2) == 0 ||
		memcmp(HostResp.RejectCode,"6X",2) == 0 ||
		memcmp(HostResp.RejectCode,"7A",2) == 0 ||
		memcmp(HostResp.RejectCode,"7C",2) == 0 ||
		memcmp(HostResp.RejectCode,"7D",2) == 0 ||
		memcmp(HostResp.RejectCode,"7E",2) == 0 ||
		memcmp(HostResp.RejectCode,"7G",2) == 0 ||
		memcmp(HostResp.RejectCode,"7K",2) == 0 ||
		memcmp(HostResp.RejectCode,"7L",2) == 0 ||
		memcmp(HostResp.RejectCode,"7M",2) == 0)
	{
		HostResp.CardEjectFlag = TRUE;
	}
    else
	if (memcmp(HostResp.RejectCode, "00", 2) == 0)
		;
	else
	{   
		
		if (m_pProfile->NETWORK.InterfaceKind == 17)			// Cup(BJHX) :2006.05.20
			HostResp.CardEjectFlag = TRUE;

		for (i = 0; i < itemof(HostNgTbl); i++)
		{
			if (memcmp(HostNgTbl[i].Code, HostResp.RejectCode, 2) == 0)
			{
				if (TranCode == TC_ECHANNEL_INQ	||
					TranCode == TC_SMSPARITY	||
					TranCode == TC_ECHANNEL)
					HostResp.CardEjectFlag = TRUE;
				break;
			}
			else
			if (memcmp(HostNgTbl[i].Code, "??", 2) == 0)
			{
				HostResp.CardEjectFlag = TRUE;
				break;
			}
		}
	}

///////////////////////////////////////////////////////////////////////////////
	fnAPP_RecvPSPGet(RecvBuffer, &PSPRecvData, RecvLength);		// Recv Parameter Slip Print Get
	fnAPP_RecvPPPGet(RecvBuffer, &PPPRecvData, RecvLength);		// Recv Parameter Passbook Print Get
	fnAPP_RecvPPWGet(RecvBuffer, &PPWRecvData, RecvLength);		// Recv Parameter Passbook Write Get
	fnAPP_RecvPCWGet(RecvBuffer, &PCWRecvData, RecvLength);		// Recv Parameter Card Write Get
	fnAPP_RecvPMEGet(RecvBuffer, &PMERecvData, RecvLength);		// Recv Parameter Message Edit Get
	fnAPP_RecvPMDGet(RecvBuffer, &PMDRecvData, RecvLength);		// Recv Parameter Message Display Get

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnal():return");
	return T_OK;
}

// Recv Analysis Bid Command
int CTranCmn::fnAPP_RecvAnalBidCommand()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand()");

	int		Cnt = 0;
	int		Length1 = 0;
	char	szTmpBit[1024];										// 2005.09.11
	char	szTmpBuff[1024];
	BYTE	TempBuff[NETBUFFSIZE];
	
///////////////////////////////////////////////////////////////////////////////
	TranCode = 0;												// Transaction Code
	TranCode2 = 0;												// Transaction Code2
///////////////////////////////////////////////////////////////////////////////

	memset(szTmpBit, 0, sizeof(szTmpBit));
	memset(szTmpBuff, 0, sizeof(szTmpBuff));
	memset(TempBuff, 0, sizeof(TempBuff));

	memset(&HMERecv, 0, sizeof(HMERecv));						// Host Message Edit Recv

	// Recv Data Field Length
	HMERecv.DataLength[0]  = 4;
	HMERecv.DataLength[1]  = MAXBITMAP/8;
	HMERecv.DataLength[2]  = -99;
	HMERecv.DataLength[3]  = 6;
	HMERecv.DataLength[4]  = 12;
	HMERecv.DataLength[7]  = 2;
	HMERecv.DataLength[11] = 8;
	HMERecv.DataLength[12] = 6;
	HMERecv.DataLength[13] = 8;
	HMERecv.DataLength[14] = 8;
	HMERecv.DataLength[15] = 8;
	HMERecv.DataLength[16] = 2;
	HMERecv.DataLength[18] = 4;
	HMERecv.DataLength[20] = 8;
	HMERecv.DataLength[21] = 36;
	HMERecv.DataLength[22] = 3;
	HMERecv.DataLength[24] = 80;
	HMERecv.DataLength[25] = 2;
	HMERecv.DataLength[28] = 8;
	HMERecv.DataLength[29] = 8;
	HMERecv.DataLength[30] = 8;
	HMERecv.DataLength[35] = -99;
	HMERecv.DataLength[36] = -999;
	HMERecv.DataLength[37] = 12;
	HMERecv.DataLength[38] = 16;
	HMERecv.DataLength[39] = 2;
	HMERecv.DataLength[40] = -99;
	HMERecv.DataLength[41] = 8;
	HMERecv.DataLength[42] = 15;
	HMERecv.DataLength[45] = -999;					//20140119.jl
	HMERecv.DataLength[46] = 10;
	HMERecv.DataLength[49] = 3;
	HMERecv.DataLength[51] = 7;
	HMERecv.DataLength[52] = 8;
	HMERecv.DataLength[53] = 16;
	HMERecv.DataLength[54] = 39;
	HMERecv.DataLength[60] = 4;
	HMERecv.DataLength[61] = -99;
	HMERecv.DataLength[62] = 8;
	HMERecv.DataLength[63] = -99;
	HMERecv.DataLength[68] = -99;						// 2008.10.05
	HMERecv.DataLength[72] = 4;
	HMERecv.DataLength[90] = 40;
	HMERecv.DataLength[93] = 3;
	HMERecv.DataLength[96] = -99;
	HMERecv.DataLength[102] = -99;
	HMERecv.DataLength[103] = -99;
	HMERecv.DataLength[106] = 16;						// 2008.10.05
	HMERecv.DataLength[120] = -999;
	HMERecv.DataLength[121] = -999;
	HMERecv.DataLength[122] = -999;
	HMERecv.DataLength[123] = -999;
	HMERecv.DataLength[128] = 8;

	memcpy(HMERecv.Data[0], &NmsRecvBuffer[Cnt], HMERecv.DataLength[0]);
	Cnt += HMERecv.DataLength[0];								// 0 Transaction Kind

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand()HMERecv.Data[0][%s]", HMERecv.Data[0]);
	if (memcmp(HMERecv.Data[0], "0820", HMERecv.DataLength[0]) == 0)
		;
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand() INFORMATION TYPE MISMATCH");
		return T_OK;
	}

	memcpy(HMERecv.Data[1], &NmsRecvBuffer[Cnt], HMERecv.DataLength[1]);
																// 1 Bitmap
	memset(szTmpBit, 0, sizeof(szTmpBit));
	memcpy(szTmpBit, HMERecv.Data[1], HMERecv.DataLength[1]);

	
	for (int i = 1; i < MAXBITMAP; i++)
	{
		BYTE BT = 0x01;
		if ((szTmpBit[(i-1)/8] & (BT<<(7-(i-1)%8))) >> (7-(i-1)%8))
		{
			HMERecv.DataRecvBit[i] = TRUE;
			if (-99 == HMERecv.DataLength[i])
			{
				Length1 = 0;
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				memcpy(szTmpBuff, NmsRecvBuffer + Cnt, 2);	
				Cnt += 2;
				memcpy(HMERecv.Data[i], szTmpBuff, 2);			// LL
				Length1 = Asc2Int(szTmpBuff, 2);
				memcpy(HMERecv.Data[i] + 2, &NmsRecvBuffer[Cnt], Length1);
				HMERecv.DataLength[i] = 2 + Length1;
				Cnt += Length1;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand()HMERecv.Data[%d][%s], Length[%d]", i, HMERecv.Data[i], Length1);
				continue;
			}
			else
			if (-999 == HMERecv.DataLength[i])
			{
				Length1 = 0;
				memset(szTmpBuff, 0, sizeof(szTmpBuff));
				memcpy(szTmpBuff, NmsRecvBuffer + Cnt, 3);
				Cnt += 3;
				memcpy(HMERecv.Data[i], szTmpBuff, 3);			// LLL
				Length1 = Asc2Int(szTmpBuff, 3);
				memcpy(HMERecv.Data[i] + 3, &NmsRecvBuffer[Cnt], Length1);
				HMERecv.DataLength[i] = 3 + Length1;
				Cnt += Length1;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand()HMERecv.Data[%d][%s], Length[%d]", i, HMERecv.Data[i], Length1);
				continue;
			}

			memcpy(HMERecv.Data[i], &NmsRecvBuffer[Cnt], HMERecv.DataLength[i]);
			Cnt += HMERecv.DataLength[i];
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand()HMERecv.Data[%d][%s]", i, HMERecv.Data[i]);
		}
	}

	if (memcmp(HMERecv.Data[41], m_pProfile->NETWORK.AtmSerialNum, HMERecv.DataLength[41]) == 0)
		;
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand() ATM ID MISMATCH");
		return T_OK;
	}

	TranCode = 0;											// 接收判断前置为空，注意后续的处理   by zjd
	if (memcmp(HMERecv.Data[3], "200100", 6) == 0)
		TranCode = TC_ATMOPEN;
	else
	if (memcmp(HMERecv.Data[3], "200200", 6) == 0)
		TranCode = TC_ATMCLOSE;
	else
	if (memcmp(HMERecv.Data[3], "200300", 6) == 0)
		TranCode = TC_ATMREBOOT;
	else
	if (memcmp(HMERecv.Data[3], "200400", 6) == 0)
		TranCode = TC_ATMSTATUS;
	else
	if (memcmp(HMERecv.Data[3], "200500", 6) == 0)
		TranCode = TC_ATMRESETALL;

	else
	if (memcmp(HMERecv.Data[3], "200600", 6) == 0)
		TranCode = TC_ATMBIDADDOWNLOAD;
	else
	if (memcmp(HMERecv.Data[3], "200700", 6) == 0)
		  TranCode = TC_ATMBIDADDOWNLOAD;
		  
	else
	if (memcmp(HMERecv.Data[3], "200800", 6) == 0)
		{
		   TranCode = TC_ATMBIDSENDDETAIL;
		}
   	else
	if (memcmp(HMERecv.Data[3], "200900", 6) == 0)
		{
		   TranCode = TC_ATMBIDSENDDETAIL;
		}
    else
	if (memcmp(HMERecv.Data[3], "201000", 6) == 0)
		{
  		   TranCode = TC_ATMBIDSOFTUPDATE;
		}
    else
	if (memcmp(HMERecv.Data[3], "201100", 6) == 0)
		   TranCode = TC_ATMBIDSOFTUPDATE;
//	else
//	if (memcmp(HMERecv.Data[3], "200000", 6) == 0)
//		TranCode = TC_ATMWORKPARAM;
//	else
//	if (m_pProfile->DEVICE.NmsDevice == 1)						// ZL NMS : 2006.04.19
//	{
//		if (memcmp(HMERecv.Data[3], "200000", 6) == 0)
//			TranCode = TC_ATMREBOOT;
//		else
//		if (memcmp(HMERecv.Data[3], "200000", 6) == 0)
//			TranCode = TC_ATMFTPSEND;
//		else
//		if (memcmp(HMERecv.Data[3], "200000", 6) == 0)
//			TranCode = TC_ATMRESETDVR;
//	}
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalBidCommand(TranCode[%d]):return", TranCode);
	return T_OK;
}

// Recv Analysis DC Bid Command
int CTranCmn::fnAPP_RecvAnalDCBidCommand()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalDCBidCommand()");

	CStringArray strTempArray;
	
///////////////////////////////////////////////////////////////////////////////
	TranCode = 0;												// Transaction Code
	TranCode2 = 0;												// Transaction Code2
///////////////////////////////////////////////////////////////////////////////

	SplitString(CString(NmsRecvBuffer), "^", strTempArray);		// 2006.07.03
	memset(NmsRecvBuffer, 0, sizeof(NmsRecvBuffer));

	if (strTempArray.GetSize() == 3)
	{
		if (memcmp(&(strTempArray[0].GetBuffer(0)[1]), m_pProfile->NETWORK.AtmSerialNum, 8) != 0)
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalDCBidCommand() ATM ID MISMATCH");
			return T_OK;
		}
	}

	if ((NmsRecvLength == 4)	&&
		(strTempArray[0] == ZERO4))
		TranCode = TC_DCSENDEJR;
	else
	if ((memcmp(strTempArray[0], "H", 1) == 0)	||
		(memcmp(strTempArray[0], "A", 1) == 0)	||
		(memcmp(strTempArray[0], "R", 1) == 0))
		;
	else
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalDCBidCommand() DATA FORMAT ERROR!!!");
		return T_OK;
	}

	if ((strTempArray[1] == "01")	||
		(strTempArray[1] == "02")	||
		(strTempArray[1] == "66"))
	{
		if ((strTempArray[2].GetLength() >= sizeof(DCNmsDateTime))	&&
			(IsNum(strTempArray[2].GetBuffer(0), strTempArray[2].GetLength())))
		{
			memcpy(DCNmsDateTime, strTempArray[2], sizeof(DCNmsDateTime));
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalDCBidCommand() DCNmsDateTime ERROR!!!");
			return T_OK;
		}
	}

	if (memcmp(strTempArray[0], "R", 1) == 0)
	{
		if ((strTempArray[0].GetLength() >= 9)	&&
			(IsNum(&(strTempArray[0].GetBuffer(0)[1]), sizeof(DCBidEjrDate))))
		{
			memcpy(DCBidEjrDate, &(strTempArray[0].GetBuffer(0)[1]), sizeof(DCBidEjrDate));
			TranCode = TC_ATMBIDDCSENDEJR;
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalDCBidCommand() DCBidEjrDate ERROR!!!");
			return T_OK;
		}
	}

	if (strTempArray[1] == "01")
	{
		if (memcmp(strTempArray[0], "H", 1) == 0)
			TranCode = TC_ATMOPENDC;
		else
		if (memcmp(strTempArray[0], "A", 1) == 0)
			TranCode = TC_ATMOPENSYNC;
	}
	else
	if (strTempArray[1] == "02")
	{
		if (memcmp(strTempArray[0], "H", 1) == 0)
			TranCode = TC_ATMCLOSEDC;
		else
		if (memcmp(strTempArray[0], "A", 1) == 0)
			TranCode = TC_ATMCLOSESYNC;
	}
	else
	if (strTempArray[1] == "66")
	{
		if (memcmp(strTempArray[0], "H", 1) == 0)
			TranCode = TC_ATMSTATUSDC;
		else
		if (memcmp(strTempArray[0], "A", 1) == 0)
			TranCode = TC_ATMSTATUSSYNC;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvAnalDCBidCommand(TranCode[%d]):return", TranCode);
	return T_OK;
}

// Recv Parameter Slip Print Get
int CTranCmn::fnAPP_RecvPSPGet(void* RecvArea, PSPRecv* DataArea, int Length)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPSPGet(Length[%d])", Length);

	int		Cnt = 0;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPSPGet(Length[%d]):return(%d)", Length, Cnt);
	return Cnt;
}

// Recv Parameter Slip Print Move
int CTranCmn::fnAPP_RecvPSPMove(char Data, PSPRecv* pDataArea, int Length)
{
	int		i = 0;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// Limit Check
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line Setting
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// Parameter Length
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}

// Recv Parameter Passbook Print Get
int CTranCmn::fnAPP_RecvPPPGet(void* RecvArea, PPPRecv* DataArea, int Length)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet(%d)", Length);

	int		Cnt = 0;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPPGet(Length[%d]):return(%d)", Length, Cnt);
	return Cnt;
}

// Recv Parameter Passbook Print Move
int CTranCmn::fnAPP_RecvPPPMove(char Data, PPPRecv* pDataArea, int Length)
{
	int		i = 0;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->p >= itemof(pDataArea->Data))	 ||		// Limit Check
			(pDataArea->y >= itemof(pDataArea->Data[0])) ||
			(pDataArea->x >= itemof(pDataArea->Data[0][0])))
			break;
		
		pDataArea->Data[pDataArea->p][pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line Setting
		{
			pDataArea->Bp = pDataArea->p;
			pDataArea->Ep = pDataArea->p;
		}
		pDataArea->ParaLength += Length;						// Parameter Length
		if (pDataArea->p < pDataArea->Bp)						// Find Begin Page
			pDataArea->Bp = pDataArea->p;
		if (pDataArea->p > pDataArea->Ep)						// Find End Page
			pDataArea->Ep = pDataArea->p;

		pDataArea->DataLength[pDataArea->p][pDataArea->y] = pDataArea->x;		
																// Data Length
	}
	return T_OK;
}

// Recv Parameter Passbook Write Get
int CTranCmn::fnAPP_RecvPPWGet(void* RecvArea, PPWRecv* DataArea, int Length)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPWGet(%d)", Length);

	int		Cnt = 0;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPPWGet(Length[%d]):return(%d)", Length, Cnt);
	return Cnt;
}
		
// Recv Parameter Passbook Write Move
int CTranCmn::fnAPP_RecvPPWMove(char Data, PPWRecv* pDataArea, int Length)
{
	int		i = 0;

	for (i = 1; i <= Length; i++)
	{
		if (pDataArea->x >= sizeof(pDataArea->Data))			// Limit Check
			break;
		
		pDataArea->Data[pDataArea->x++] = Data;
		pDataArea->ParaLength += Length;						// Parameter Length
	}
	return T_OK;
}

// Recv Parameter Card Write Get
int CTranCmn::fnAPP_RecvPCWGet(void* RecvArea, PCWRecv* DataArea, int Length)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPCWGet(%d)", Length);

	int		Cnt = 0;

MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPCWGet(Length[%d]):return(%d)", Length, Cnt);
	return Cnt;
}
		
// Recv Parameter Card Write Move
int CTranCmn::fnAPP_RecvPCWMove(char Data, PCWRecv* pDataArea, int Length)
{
	int		i = 0;

	for (i = 1; i <= Length; i++)
	{
		if (pDataArea->x >= sizeof(pDataArea->Data))			// Limit Check
			break;
		
		pDataArea->Data[pDataArea->x++] = Data;
		pDataArea->ParaLength += Length;						// Parameter Length
	}
	return T_OK;
}

// Recv Parameter Message Edit Get
int CTranCmn::fnAPP_RecvPMEGet(void* RecvArea, PMERecv* DataArea, int Length)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMEGet(%d)", Length);

	int		Cnt = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMEGet(Length[%d]):return(%d)", Length, Cnt);
	return Cnt;
}

// Recv Parameter Message Edit Sub Get
int CTranCmn::fnAPP_RecvPMESubGet(void* RecvArea, PMERecv* DataArea, int Cnt, int Length)
{
	return Cnt;
}

// Recv Parameter Message Edit Move
int CTranCmn::fnAPP_RecvPMEMove(char Data, PMERecv* pDataArea, int Length)
{
	int		i = 0;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// Limit Check
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line Setting
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// Parameter Length
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}

// Recv Parameter Message Display Get
int CTranCmn::fnAPP_RecvPMDGet(void* RecvArea, PMDRecv* DataArea, int Length)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMDGet(%d)", Length);

	int			Cnt = 0;
	char		szTempZero[1024];
	CString		strReturn("");
	CStringArray strTempArray;
	int			i = 0, j = 0, k = 0;
	char		tempRetCode[2];

	memset(szTempZero, 0, sizeof(szTempZero));					// No Message Reject Code
	if ((memcmp(HostResp.RejectCode, szTempZero, 2) == 0)	||
		(memcmp(HostResp.RejectCode, "  ", 2) == 0)			||
		(memcmp(HostResp.RejectCode, "00", 2) == 0))
		return Cnt;

	if ((TranCode == TC_INQUIRY && iFstPIN)||
		(TranCode == TC_INQUIRY_CheckPin && iFstPIN))
	{
		if (memcmp(HostResp.RejectCode, "26", 2) == 0	||
			memcmp(HostResp.RejectCode, "27", 2) == 0	||
			memcmp(HostResp.RejectCode, "3I", 2) == 0	||
			memcmp(HostResp.RejectCode, "04", 2) == 0	||
			memcmp(HostResp.RejectCode, "08", 2) == 0	||
			memcmp(HostResp.RejectCode, "09", 2) == 0	||
			memcmp(HostResp.RejectCode, "11", 2) == 0	||
			memcmp(HostResp.RejectCode, "17", 2) == 0	||
			memcmp(HostResp.RejectCode, "20", 2) == 0	||
			memcmp(HostResp.RejectCode, "21", 2) == 0	||
			memcmp(HostResp.RejectCode, "25", 2) == 0	||
			memcmp(HostResp.RejectCode, "30", 2) == 0	||
			memcmp(HostResp.RejectCode, "23", 2) == 0	||
			memcmp(HostResp.RejectCode, "31", 2) == 0	||
			memcmp(HostResp.RejectCode, "66", 2) == 0	||
			memcmp(HostResp.RejectCode, "97", 2) == 0	||
			memcmp(HostResp.RejectCode, "2N", 2) == 0	||
			memcmp(HostResp.RejectCode, "3F", 2) == 0	||
			memcmp(HostResp.RejectCode, "13", 2) == 0	||
			memcmp(HostResp.RejectCode, "14", 2) == 0	||
			memcmp(HostResp.RejectCode, "12", 2) == 0	||
//			memcmp(HostResp.RejectCode, "25", 2) == 0	||
			memcmp(HostResp.RejectCode, "4F", 2) == 0	||
			memcmp(HostResp.RejectCode, "7A", 2) == 0	||
			memcmp(HostResp.RejectCode, "7I", 2) == 0	||
			memcmp(HostResp.RejectCode, "5P", 2) == 0)
			memcpy(tempRetCode, HostResp.RejectCode, 2);
		else
			memcpy(tempRetCode, "!!", 2);
	}
	else
		memcpy(tempRetCode, HostResp.RejectCode, 2);

	for (i = 0; i < itemof(HostNgTbl); i++)
	{
		if ((memcmp(HostNgTbl[i].Code, tempRetCode, 2) == 0)	||
			(memcmp(HostNgTbl[i].Code, "??", 2) == 0))
		{
			strReturn = HostNgTbl[i].Msg[m_pDevCmn->fnSCR_GetCurrentLangMode()];
			SplitString(strReturn, "\n", strTempArray);
			for (j = 0; j < strTempArray.GetSize(); j++)
			{
				for (k = 0; k < strTempArray[j].GetLength(); k++)
					fnAPP_RecvPMDMove(strTempArray[j].Mid(k, 1).GetBuffer(0)[0], DataArea, 1);

				DataArea->y++;		
				DataArea->x = 0;
			}
			break;
		}
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_RecvPMDGet(Length[%d]):return(%d)", Length, Cnt);
	return Cnt;
}

// Recv Parameter Message Display Get
int CTranCmn::fnAPP_RecvPMDMove(char Data, PMDRecv* pDataArea, int Length)
{
	int		i = 0;

	for (i = 1; i <= Length; i++)
	{
		if ((pDataArea->y >= itemof(pDataArea->Data)) ||		// Limit Check
			(pDataArea->x >= itemof(pDataArea->Data[0])))
			break;
		
		pDataArea->Data[pDataArea->y][pDataArea->x++] = Data;
		if (!pDataArea->ParaLength)								// Find Line Setting
		{
			pDataArea->By = pDataArea->y;
			pDataArea->Ey = pDataArea->y;
		}
		pDataArea->ParaLength += Length;						// Parameter Length
		if (pDataArea->y < pDataArea->By)						// Find Begin Line
			pDataArea->By = pDataArea->y;
		if (pDataArea->y > pDataArea->Ey)						// Find End Line
			pDataArea->Ey = pDataArea->y;
	}
	return T_OK;
}


int CTranCmn::fnAPP_ACSendHost()                 //A/C MANAGER SEND HOST
{
 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACSendHost()");
	int ret;
	int rec;
	char retmsg[300];
	char retcode[10];
	int m_callid = 1234567;
	
	memset(retmsg,0,sizeof(retmsg));
    
	AddSerialFlag = TRUE;									// Add Serial Flag
	m_pDevCmn->fnAPL_AddSerialNo();							// Add Serial No
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACSendHost::m_pProfile->TRANS.SerialNo(%8.8s)", m_pProfile->TRANS.SerialNo);
	
	SetTxCode(m_callid,ACTranCode,&ret);      //设置请求字段内容

	if( ACTranCode == 5505 )                  //查询客户基本信息
	{
		SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);               //客户端流水号(可选)
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型","1",&ret);
//		SetNameValue(m_callid,"密码","000000",&ret);                      
		SetNameValue(m_callid,"密码验证标志","0",&ret);

//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)KHCC",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
//  				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5507 )           // 查询账户余额
	{
		SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型","1",&ret);
		SetNameValue(m_callid,"密码验证标志","0",&ret);
		
//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5507",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
//  				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5518 )        // 查询客户资金明细
	{
		SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型","1",&ret);
// 		SetNameValue(m_callid,"验证码","111111",&ret);
		SetNameValue(m_callid,"密码验证标志","0",&ret);

//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5518",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5403 )         // 客户身份认证
	{
        SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"交易类型",ACHostSend.TranType,&ret);                           //从返回的信息取出
		SetNameValue(m_callid,"金额",ACHostSend.TranMoney,&ret);

//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5403",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5407 )       // 查询账户余额
	{
        SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型",ACHostSend.TranType,&ret);
// 		SetNameValue(m_callid,"密码","000000",&ret);                       //需要把密码解密成明文 by hyl
		SetNameValue(m_callid,"密码验证标志","0",&ret);

//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5407",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5416 )        // 查询应用交易类型
	{
		SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);		
//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5507",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5418 )         // 查询客户资金明细
	{
        SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"资金类型","",&ret);                           //从返回的信息取出（交易类型）
//		SetNameValue(m_callid,"操作员",,&ret);
//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5418",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5303 )      // 客户身份认证
	{
        SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
        
//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5303",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	else
	if( ACTranCode == 5317 )          // 约定交易
	{
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.ReservePin[%d]", ACHostSend.ReservePin);

        SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型","1",&ret);
		SetNameValue(m_callid,"密码验证标志","0",&ret);
		SetNameValue(m_callid,"预留序号",ACHostSend.ReserveNum,&ret);                      
		SetNameValue(m_callid,"预留密码",ACHostSend.ReservePin,&ret);
		SetNameValue(m_callid,"金额",ACHostSend.TranMoney,&ret);
		SetNameValue(m_callid,"一本通存期",Int2Asc(ACHostSend.DepTime),&ret);   //需要修改

// 		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.ReservePin[%s]", Int2Asc(ACHostSend.ReservePin));
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.ReserveNum[%s]", ACHostSend.ReserveNum);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.TranMoney[%s]", ACHostSend.TranMoney);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.DepTime[%d]", ACHostSend.DepTime);
		
		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5317",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4]),
				m_pProfile->TRANS.SerialNo));
		
	}
	else
	if( ACTranCode == 5327 )           // 非约定缴费
	{
        SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型","1",&ret);
		SetNameValue(m_callid,"密码验证标志","0",&ret);
		SetNameValue(m_callid,"交易类型",ACHostSend.TranType,&ret);
		SetNameValue(m_callid,"缴费号码",Accept.InAccountNum,&ret);                      
		SetNameValue(m_callid,"金额",ACHostSend.TranMoney,&ret);

		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5318",
				GetTime().GetBuffer(0),
				&(GetTime().GetBuffer(0)[2]),
				&(GetTime().GetBuffer(0)[4]),
				m_pProfile->TRANS.SerialNo));

	}
	else
	if( ACTranCode == 5328 )          // 缴费欠费查询
	{
        SetNameValue(m_callid,"渠道","5",&ret);
		SetNameValue(m_callid,"银行柜员",InitInfo.ACCounterNumber,&ret);
		SetNameValue(m_callid,"银行网点",m_pProfile->NETWORK.BranchNum,&ret);
		SetNameValue(m_callid,"终端号",m_pProfile->NETWORK.AtmSerialNum,&ret);
		SetNameValue(m_callid,"前置流水号",m_pProfile->TRANS.SerialNo,&ret);           //客户端流水号(可选)
		SetNameValue(m_callid,"银行账号",CardAccountNo,&ret);
		SetNameValue(m_callid,"账户类型","1",&ret);
	    SetNameValue(m_callid,"密码验证标志","0",&ret);
		SetNameValue(m_callid,"交易类型",ACHostSend.TranType,&ret);
		SetNameValue(m_callid,"缴费号码",Accept.InAccountNum,&ret);    
		
//		m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s TRNS:%8.8s (A/C)5328",
//				GetTime().GetBuffer(0),
//				&(GetTime().GetBuffer(0)[2]),
//				&(GetTime().GetBuffer(0)[4]),
// 				m_pProfile->TRANS.SerialNo));
	}
	 		
    
	SendRequest(m_callid,&rec,retcode,retmsg,&ret);

    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACSendHost() retmsg[%s]",retmsg);
	bCommRecvFlag = 1;
	if (ret < 0)
	{
		ACHostResp.ReturnCode = ret;							// added by yusy 2009.05.13
		bCommRecvFlag = FALSE;
		if(m_pDevCmn->bAgentMode == 1)							
		{
			if (ACTranCode == TC_AC_CUSASSU_DETAIL)
				fnAPP_AgentIdxGetData(AGENT_AC_CUSASSU_DETAIL);
			else
			if (ACTranCode == TC_AC_ASSU_TRAN)
				fnAPP_AgentIdxGetData(AGENT_AC_ASSU_TRAN);
			else
			if (ACTranCode == TC_AC_NOASSU_PAY)
				fnAPP_AgentIdxGetData(AGENT_AC_NOASSU_PAY);
			else
			if (ACTranCode == TC_AC_PAY_INQ)
				fnAPP_AgentIdxGetData(AGENT_AC_PAY_INQ);
			else
			if (ACTranCode == TC_AC_BALANCE_INQ)				// added by liyi 2009.07.21
			{
				fnAPP_AgentIdxGetData(AGENT_AC_BALANCE_INQ);
			}
		}
		m_pDevCmn->fnSCR_DisplayString(2,retmsg); 
		m_pDevCmn->fnSCR_DisplayScreen(702);
		Delay_Msg(1000);
		fnAPP_CancelProc(T_CANCEL);
// 		return T_EXIT;
	}

	

	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACSendHost():return ");
	return T_OK;
}

int CTranCmn::fnAPP_ACRecvHost()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACRecvHost()");

    int ret;
	int rec;
	char retmsg[300];
	char tmpchar[200];
	int m_callid = 1234567;
	CString			strTemp("");

// 	memset(&ACHostSend,0,sizeof(ACHostSend));
	
	GetReturnCode(m_callid,&ACHostResp.ReturnCode,retmsg,&ret);           //返回码参看errcode.h
	if (ret !=0)
		ACHostResp.ReturnCode = ret;

//	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2sHOST RETURN<%06.6d>HOST:%12s",
//				GetTime().GetBuffer(0), &(GetTime().GetBuffer(0)[2]), &(GetTime().GetBuffer(0)[4]),
// 				ACHostResp.ReturnCode, HMERecv.Data[37]));

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACRecvHost()ACTRAN[%d]ReturnCode[%d]",ACHostResp.TranCode,ACHostResp.ReturnCode);

	GetNameValue(m_callid,"返回信息",ACHostResp.ReturnMess,retmsg,&ret);
	//从记录里取出字段内容
	if( ACTranCode == 5505 )	// 查询客户基本信息
	{
		GetNameValueByRec(m_callid,0,"银行账号",ACHostResp.BankAccountNo,retmsg,&ret);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACRecvHost()BankAccountNo[%s]",ACHostResp.BankAccountNo);
	}
	else
	if( ACTranCode == 5507 )   // 查询客户约定细项信息	
	{
		//记录数
		PayAssuCn=0;           
		RemitCn=0;
    	GetTotalRecCount(m_callid,&rec,retmsg,&ret);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "5507RecNum[%d]", rec);

        if(rec < 1 && ACHostResp.ReturnCode == 0)     //当记录为1时，rec GET出来值为0 ！！！所以做此修改  by hyl
		{
			rec = 1;
 		}

	    for (int i=0;i<rec;i++)
		{
			GetNameValueByRec(m_callid,i,"交易类型",ACHostRespACManager[i].TranType,retmsg,&ret);
		    GetNameValueByRec(m_callid,i,"交易类型属性",tmpchar,retmsg,&ret);
			ACHostRespACManager[i].TranTypeAttri = Asc2Int(tmpchar,1);  

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespACManager[%d].TranType[%s]", i,ACHostRespACManager[i].TranType);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Asc2Int(ACHostRespACManager[%d].TranType[%s]=[%d]", i,ACHostRespACManager[i].TranType,Asc2Int(ACHostRespACManager[i].TranType));

			if( Asc2Int(ACHostRespACManager[i].TranType) == 4)
			{
				GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
		    	GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人邮编",ACHostRespRemitAssu[RemitCn].PayeePostCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人地址",ACHostRespRemitAssu[RemitCn].PayeeAdd,retmsg,&ret);
 				GetNameValueByRec(m_callid,i,"汇兑回音标志",ACHostRespRemitAssu[RemitCn].RemitReturnFlag,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
				ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;

			}
			else
			if( Asc2Int(ACHostRespACManager[i].TranType) == 5)
			{
     			GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
	    		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人邮编",ACHostRespRemitAssu[RemitCn].PayeePostCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
			    ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[RemitCn].ReserveNum[%s]", ACHostRespRemitAssu[RemitCn].ReserveNum);

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;

			}
			else
			if( Asc2Int(ACHostRespACManager[i].TranType) == 6)
			{
     			GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
	    		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人邮编",ACHostRespRemitAssu[RemitCn].PayeePostCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人账号",ACHostRespRemitAssu[RemitCn].PayeeAccount,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
			    ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;

			}else
			if( Asc2Int(ACHostRespACManager[i].TranType) == 7)
			{
     			GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
	    		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人账号",ACHostRespRemitAssu[RemitCn].PayeeAccount,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
		    	ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;

			}
			else
			if( Asc2Int(ACHostRespACManager[i].TranType) == 8)
			{
     			GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
	    		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人邮编",ACHostRespRemitAssu[RemitCn].PayeePostCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人账号",ACHostRespRemitAssu[RemitCn].PayeeAccount,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"转入银行代码",ACHostRespRemitAssu[RemitCn].PayeeBankCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"转入银行名称",ACHostRespRemitAssu[RemitCn].PayeeBankName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
			    ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;
		
			}
			else
			if( Asc2Int(ACHostRespACManager[i].TranType) == 9)
			{
     			GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
	    		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人邮编",ACHostRespRemitAssu[RemitCn].PayeePostCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人账号",ACHostRespRemitAssu[RemitCn].PayeeAccount,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"保留金额",ACHostRespRemitAssu[RemitCn].ReserveSum,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"转入银行名称",ACHostRespRemitAssu[RemitCn].PayeeBankName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
                ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;			
			
			}
			else
			if( Asc2Int(ACHostRespACManager[i].TranType) == 10)
			{
     			GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[RemitCn].ReserveNum,retmsg,&ret);
	    		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[RemitCn].TranType,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespRemitAssu[RemitCn].PayeeName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"收款人账号",ACHostRespRemitAssu[RemitCn].PayeeAccount,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"转入银行代码",ACHostRespRemitAssu[RemitCn].PayeeBankCode,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易名称",ACHostRespRemitAssu[RemitCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"转入银行名称",ACHostRespRemitAssu[RemitCn].PayeeBankName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespRemitAssu[RemitCn].SumLimit,retmsg,&ret);
			    ACHostRespRemitAssu[RemitCn].ArrayNum = RemitCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].ArrayNum[%d]", RemitCn,RemitCn+1);
				RemitCn++;
			}
			else
			if( Asc2Int(ACHostRespACManager[i].TranType) >= 1000 &&
				Asc2Int(ACHostRespACManager[i].TranType) <= 2000 &&
			    ACHostRespACManager[i].TranTypeAttri != 3)
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespACManager[%d].TranTypeAttri[%d]", i,ACHostRespACManager[i].TranTypeAttri);
            ACHostRespPayAssu[PayAssuCn].TranTypeAttri = ACHostRespACManager[i].TranTypeAttri;
	    	GetNameValueByRec(m_callid,i,"交易类型",ACHostRespPayAssu[PayAssuCn].TranType,retmsg,&ret);
     		GetNameValueByRec(m_callid,i,"预留序号",ACHostRespPayAssu[PayAssuCn].ReserveNum,retmsg,&ret);
			GetNameValueByRec(m_callid,i,"收款人姓名",ACHostRespPayAssu[PayAssuCn].PayeeName,retmsg,&ret);
			GetNameValueByRec(m_callid,i,"缴费号码",ACHostRespPayAssu[PayAssuCn].JHNumber,retmsg,&ret);
			GetNameValueByRec(m_callid,i,"交易名称",ACHostRespPayAssu[PayAssuCn].TranName,retmsg,&ret);
			GetNameValueByRec(m_callid,i,"约定汇款额度",ACHostRespPayAssu[PayAssuCn].SumLimit,retmsg,&ret);
            ACHostRespPayAssu[PayAssuCn].ArrayNum = PayAssuCn + 1;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PayAssuCn[%d]", PayAssuCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespPayUnAssu[%d].ArrayNum[%d]", PayAssuCn,PayAssuCn+1);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ReserveNum[%s]", ACHostRespPayAssu[PayAssuCn].ReserveNum);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYUNASSUReserveNum[%s]", ACHostRespPayUnAssu[PayAssuCn].ReserveNum);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "TranName[%s]", ACHostRespPayAssu[PayAssuCn].TranName);
			 PayAssuCn++;
			 
			}
			
		}
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AC5507RemitCN[%d]", RemitCn);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AC5507PayAssuCn[%d]", PayAssuCn);

		
	}
	else
	if( ACTranCode == 5518 )   // 开通业务查询	
	{
//   		GetNameValueByRec(m_callid,i,"预留序号",ACHostRespRemitAssu[i].ReserveNum,retmsg,&ret);
//   		GetNameValueByRec(m_callid,i,"交易类型",ACHostRespRemitAssu[i].TranType,retmsg,&ret);
		GetTotalRecCount(m_callid,&rec,retmsg,&ret);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "5518RecNum[%d]", rec);
        
        if(rec < 1 && ACHostResp.ReturnCode == 0)     //当记录为1时，rec GET出来值为0 ！！！所以做此修改  by hyl
		{
			rec = 1;
 		}
        
		UnAssuPayOpenNum = 0;
	    for (int i=0;i<rec;i++)
		{  
			GetNameValueByRec(m_callid,i,"应用类型",ACHostRespACManager[i].TranType,retmsg,&ret);
 			GetNameValueByRec(m_callid,i,"应用代码",ACHostRespACManager[i].JHTranCode,retmsg,&ret);		
//			GetNameValueByRec(m_callid,i,"交易名称",ACHostRespACManager[i].TranName,retmsg,&ret);
// 			GetNameValueByRec(m_callid,i,"应用信息1",ACHostRespACManager[i].SumLimit,retmsg,&ret);
			if(Asc2Int(ACHostRespACManager[i].TranType) == 7)         //返回码为7说明此业务开通
                        UnAssuPayOpenNum++;
		}
		
	}
	else
	if( ACTranCode == 5416 )	// 查询应用交易类型
	{
		UnPayAssuCn=0;
		GetTotalRecCount(m_callid,&rec,retmsg,&ret);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "5416RecNum[%d]", rec);

        if(rec < 1 && ACHostResp.ReturnCode == 0)     //当记录为1时，rec GET出来值为0 ！！！所以做此修改  by hyl
		{
			rec = 1;
 		} 

	    for (int i=0;i<rec;i++)
		{
		    GetNameValueByRec(m_callid,i,"交易类型",ACHostRespACManager[i].TranType,retmsg,&ret);
			GetNameValueByRec(m_callid,i,"缴费类型属性",tmpchar,retmsg,&ret);
			ACHostRespACManager[i].TranTypeAttri = Asc2Int(tmpchar,1); 
		
			if(Asc2Int(ACHostRespACManager[i].TranType) >= 1000 && 
			   Asc2Int(ACHostRespACManager[i].TranType) <= 2000 &&
			   ACHostRespACManager[i].TranTypeAttri != 3)
			{
                GetNameValueByRec(m_callid,i,"交易名称",ACHostRespPayUnAssu[UnPayAssuCn].TranName,retmsg,&ret);
				GetNameValueByRec(m_callid,i,"交易类型",ACHostRespPayUnAssu[UnPayAssuCn].TranType,retmsg,&ret);
				ACHostRespPayUnAssu[UnPayAssuCn].TranTypeAttri = ACHostRespACManager[i].TranTypeAttri;
				ACHostRespPayUnAssu[UnPayAssuCn].ArrayNum = UnPayAssuCn + 1;
				UnPayAssuCn++;
			}
			
		}
	}
	else
	if( ACTranCode == 5403 )      // 计算手续费
	{
// 		GetNameValueByRec(m_callid,0,"手续费",ACHostResp.CommCharge,retmsg,&ret);
		
		GetNameValueByRec(m_callid,0,"手续费",tmpchar,retmsg,&ret);
		strTemp.Format("%.2f", atof(tmpchar));
 	    memcpy(ACHostResp.CommCharge, strTemp.GetBuffer(0), strTemp.GetLength());
	}
	else
	if( ACTranCode == 5407 )     // 查询账户余额
	{
// 		GetNameValueByRec(m_callid,"账户余额",ACHostResp.,retmsg,&ret);
// 		GetNameValueByRec(m_callid,0,"可用金额",ACHostResp.AcBalance,retmsg,&ret);
// 		GetNameValueByRec(m_callid,"冻结金额",tmpchar,retmsg,&ret);

		GetNameValueByRec(m_callid,0,"可用金额",tmpchar,retmsg,&ret);
		strTemp.Format("%.2f", atof(tmpchar));
 	    memcpy(ACHostResp.AcBalance, strTemp.GetBuffer(0), strTemp.GetLength());

 	}
	else
	if( ACTranCode == 5317 )        // 约定交易
	{
		GetNameValueByRec(m_callid,0,"本地流水号",ACHostResp.ACTranServiceNo,retmsg,&ret);
		GetNameValueByRec(m_callid,0,"前置流水号",ACHostResp.HostServiceNo,retmsg,&ret);
		GetNameValueByRec(m_callid,0,"主机流水号",ACHostResp.HostServiceNo1,retmsg,&ret);
		GetNameValueByRec(m_callid,0,"汇票号",ACHostResp.PostalOrderNo,retmsg,&ret);
// 		GetNameValueByRec(m_callid,0,"应用流水号",ACHostResp.OperServiceNo,retmsg,&ret); 
		GetNameValueByRec(m_callid,0,"汇兑流水号",ACHostResp.OperServiceNo,retmsg,&ret);
		GetNameValueByRec(m_callid,0,"中间业务流水号",ACHostResp.OperServiceNo1,retmsg,&ret);

	}
	else
	if( ACTranCode == 5327 )       // 非约定缴费
	{
		GetNameValueByRec(m_callid,0,"本地流水号",ACHostResp.ACTranServiceNo,retmsg,&ret);
//		GetNameValueByRec(m_callid,0,"应用流水号",ACHostResp.OperServiceNo,retmsg,&ret);
		GetNameValueByRec(m_callid,0,"前置流水号",ACHostResp.HostServiceNo,retmsg,&ret);
        GetNameValueByRec(m_callid,0,"中间业务流水号",ACHostResp.OperServiceNo1,retmsg,&ret);
	}
	else
	if( ACTranCode == 5328 )      // 缴费欠费查询
	{
// 		GetNameValueByRec(m_callid,0,"应缴费用金额",ACHostResp.JHAmount,retmsg,&ret);

		GetNameValueByRec(m_callid,0,"应缴费用金额",tmpchar,retmsg,&ret);
		strTemp.Format("%.2f", atof(tmpchar));
 	    memcpy(ACHostResp.JHAmount, strTemp.GetBuffer(0), strTemp.GetLength());

		GetNameValueByRec(m_callid,0,"交易类型属性",tmpchar,retmsg,&ret);
		ACHostResp.TranTypeAttri = Asc2Int(tmpchar,1);
	}

	if (m_pDevCmn->bAgentMode == 1)								// added by yusy 2009.05.13
	{
		if (ACTranCode == TC_AC_PAY_INQ)
			fnAPP_AgentIdxGetData(AGENT_AC_PAY_INQ);
		else
		if (ACTranCode == TC_AC_CUSASSU_DETAIL)
			fnAPP_AgentIdxGetData(AGENT_AC_CUSASSU_DETAIL);
		else
		if (ACTranCode == TC_AC_NOASSU_PAY)
			fnAPP_AgentIdxGetData(AGENT_AC_NOASSU_PAY);
		else
		if (ACTranCode == TC_AC_ASSU_TRAN)
			fnAPP_AgentIdxGetData(AGENT_AC_ASSU_TRAN);
		else
		if (ACTranCode == TC_AC_BALANCE_INQ)					// added by liyi 2009.07.21
		{
			fnAPP_AgentIdxGetData(AGENT_AC_BALANCE_INQ);
		}
	}

	if(ACHostResp.ReturnCode != 0)                            //交易失败处理
	    fnAPP_PMEAcceptACMangerContinue();
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_ACRecvHost():return");
	return T_OK;
}

int CTranCmn::fnAPP_SaveECTempData(char* strData, int Length)
{
	FILE	*fp;
	char	szTempFile[100];
	
	
	memset(szTempFile, 0x00, sizeof(szTempFile));	
	sprintf(szTempFile, "%s", "D:\\ECTempData.txt");

	if (Length > 900)
		Length = 900;
	
	if (DownloadCurrNum)
		fp = fopen(szTempFile, "a+b");
	else
		fp = fopen(szTempFile, "w+b");
	
	if (fp == NULL)
		return FALSE;
	
	fwrite(strData, Length, 1, fp);
	fclose(fp);

	return TRUE;
}