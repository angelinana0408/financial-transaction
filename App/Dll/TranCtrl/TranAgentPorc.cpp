     /////////////////////////////////////////////////////////////////////////////
// TranAgent.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include <wininet.h>

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Dll\LcTCPClient.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\TranCmn.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int CTranCmn::fnAPP_AgentLoadLibrary()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentLoadLibrary()");
	HINSTANCE m_hins = NULL;	
	/* DLL的位置可以指定 */ 
	if((m_hins = LoadLibrary("C:\\T1Atm\\App\\ExeRele\\ATMSSHMEM.dll"))==NULL) 
	{//ATMSSHMEM.dll 加载错误 
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentLoadLibrary():LoadLibary Fail!");
	    return FALSE;
	}
	
	memset(szAgentErrorCode, 'X', sizeof(szAgentErrorCode));
	memset(szAgentCheckErrorCode, '0', sizeof(szAgentCheckErrorCode));

	pGetSharedMessage = (GetSharedMessage)::GetProcAddress(m_hins, "GetSharedMessage");	 
	pDataInitSharedMessage =(DataInitSharedMessage)::GetProcAddress(m_hins, "DataInitSharedMessage");
	pPutSharedMessage = (PutSharedMessage)::GetProcAddress(m_hins, "PutSharedMessage");

	// Library 地址检查
	if((pGetSharedMessage == NULL)      ||
	   (pDataInitSharedMessage == NULL) ||
	   (pPutSharedMessage == NULL))
	{// DLL 加载错误
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentLoadLibrary():LoadLibary Fail!");
	    return FALSE;
	}
	else
	{
        MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentLoadLibrary() return Success");
	    return TRUE;
	}	
}

int  CTranCmn::fnAPP_AgentIdxGetData(int nAppRunType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AgentIdxGetData()");
    CString szAgentIdxCommonData("");
	char szAgentIdxTranData[1024];
	CString szAgentTranData("");
	CString szFee("");
	CString strTemp("");
// 	CString szMachineKind("");
	CStringArray strTempArray;
	CString cDvrstatus("");
	CString cDvrChanStatus("");
	CString strReturn("");
    char iTranCode;
	char szTranUseCode[7];
	int iTranOkFlag;
	int nTempLen;
	int nTempLen1;
	int iLineCnt;
	int JprMode;
	int iDevNum;
	int CashCheckFlag = 1;   //取款默认为1， 1:现金, 2:支票, 3:现金+支票  
    int	nFitPos  = 0;
	char cErrorCode;
	char szCduStatus;
	char szSprStatus;
	char szPinStatus;
	char szBrmStatus;
	char szJprStatus;
	char szDvrStatus;
	char szMcuStatus;
	char szPbStatus;
	char szBox1Status;
	char szBox2Status;
	char szBox3Status;
	char szBox4Status;
	char szRejectStatus;
	char szErrorCode[5];
	char szReturnCode[5];
	char cMediaKind = 'A';
	char szErrorMsg[41];     // HOST返回码说明
	char szCardAccountNo[23];
	char szTotalTranAmount[17];
	char szCheckTranAmount[17];
	char szBalance[17];
	char szTranCode;

	// added by yusy 08.09.09
	char			szAtmStatus;
	char			szCSTStatus;
	char			szCommStatus;
	char			szTTUStatus;
	char			szSprPaperStatus;
	char			szJprPaperStatus;
	char			szMcuRetractStatus;
	char			szPbRetractStatus;
	char			szMaterialStatus;


	if( nAppRunType == CLERK_DOOR_OPEN     ||
		nAppRunType == CLERK_SHUTTER_OPEN  ||
		nAppRunType == CLERK_SHUTTER_CLOSE ||
		nAppRunType == CLERK_ENTER_ADMIN   ||
		nAppRunType == CLERK_ENTER_TRAN    ||
		nAppRunType == CLERK_DEVICE_INIT   ||
		nAppRunType == CLERK_ATM_OPEN      ||
		nAppRunType == CLERK_ATM_POWEROFF  ||
		nAppRunType == CLERK_DOOR_CLOSE    ||
		nAppRunType == CLERK_OTHER         ||
		nAppRunType == AGENT_OPENFAIL) 
	{
		szTranCode ='5';
		iTranCode = '9';
		nTempLen = 0;

		memset(szAgentIdxTranData,0,1024);
		memcpy(&szAgentIdxTranData[nTempLen],GetSprintf("%02d",nAppRunType),2);
		memcpy(&szAgentIdxTranData[nTempLen+2],"          ",10);
	}
	else
	if( nAppRunType == AGENT_WITHDRAWAL        ||
		nAppRunType == AGENT_INQUIRY           ||
		nAppRunType == AGENT_DETAIL            ||
		nAppRunType == AGENT_TRANSFER          ||
		nAppRunType == AGENT_CHANGEPIN         ||
		nAppRunType == AGENT_ADDCASH           ||
		nAppRunType == AGENT_OPEN              ||
		nAppRunType == AGENT_ERRORMODE         ||
		nAppRunType == AGENT_STATUS            ||
		nAppRunType == AGENT_ATMPTOTAL         ||
		nAppRunType == AGENT_ADDCASHTOTAL      ||
		nAppRunType == AGENT_ATMCTOTAL         ||
		nAppRunType == AGENT_CANCEL            ||
		nAppRunType == AGENT_PAY               ||
		nAppRunType == AGENT_REMIT_ADD         ||
		nAppRunType == AGENT_REMIT_PWD		   ||
		nAppRunType == AGENT_REMIT_IN		   ||
		nAppRunType == AGENT_REMIT_INF_INQ     ||
		nAppRunType == AGENT_AC_PAY_INQ        ||
		nAppRunType == AGENT_AC_ASSU_TRAN      ||
		nAppRunType == AGENT_AC_NOASSU_PAY     ||
		nAppRunType == AGENT_AC_CUSASSU_DETAIL ||
		nAppRunType == AGENT_AC_BALANCE_INQ	   ||				// added by liyi 2009.07.21
		nAppRunType == AGENT_CARD_RETRACT      ||
		nAppRunType == AGENT_HZT               ||
		nAppRunType == AGENT_TZH               ||
		nAppRunType == AGENT_OTHER_INQ		   ||
		nAppRunType == AGENT_INQUIRY_CheckPin)					//20140119 jl
	{	
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "bAgetDoorFLAG[%d]",bAgetDoorFLAG);

			memset(szErrorCode,' ',4);
			memset(szReturnCode,' ',4);
			memset(szErrorMsg,' ',40);
			memset(szCardAccountNo,' ',22);
			memset(szTotalTranAmount,'0',16);
			memset(szCheckTranAmount,'0',16);
			memset(szBalance,'0',16);
			memset(szAgentIdxTranData,0,1024);
			memcpy(&szReturnCode[0],GetSprintf("%-4.4s",HMERecv.Data[39]),4);

			if(	nAppRunType == AGENT_AC_ASSU_TRAN      ||
				nAppRunType == AGENT_AC_NOASSU_PAY     ||
				nAppRunType == AGENT_AC_CUSASSU_DETAIL ||
				nAppRunType == AGENT_AC_PAY_INQ		   ||
				nAppRunType == AGENT_AC_BALANCE_INQ)			// added by liyi 2009.07.21
			{
				if(ACHostResp.ReturnCode == 0)
					iTranOkFlag = 1;
				else
					iTranOkFlag = 0;
				if(bCommRecvFlag == FALSE)
					iTranOkFlag = 0;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "bCommRecvFlag[%d],iTranOkFlag[%d]",bCommRecvFlag,iTranOkFlag);
				memcpy(&szErrorCode[0],GetSprintf("%-04d",ACHostResp.ReturnCode),4);
     			memcpy(&szErrorMsg[0],GetSprintf("%-40.40s",ACHostResp.ReturnMess),40); 
			}
			else
			if(!m_pDevCmn->TranResult)
			{
				iTranOkFlag = 0;
				memcpy(&szErrorCode[0],GetSprintf("%-4.4s",HMERecv.Data[39]),4);
                if(memcmp(&HMERecv.Data[39],"  ", 2) != 0)
				{
					for (int i = 1; i < itemof(HostNgTbl); i++)
					{
						if ((memcmp(HostNgTbl[i].Code, &HMERecv.Data[39], 2) == 0 && HostNgTbl[i].Code!="")	||
							(memcmp(HostNgTbl[i].Code, "??", 2) == 0))
						{
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HostNgTbl[i].Code[%s]",HostNgTbl[i].Code);
							strReturn = HostNgTbl[i].Msg[0];
							SplitString(strReturn, "\n", strTempArray);
							memcpy(&szErrorMsg[0],GetSprintf("%-40.40s",strTempArray[1]),40); 
							break;
						}

					}
				}
			}
			else
			{
				iTranOkFlag =1;
			}

			memcpy(&szCardAccountNo, CardAccountNo, strlen(CardAccountNo));
			
			if(nAppRunType == AGENT_REMIT_ADD || nAppRunType == AGENT_REMIT_PWD || nAppRunType == AGENT_REMIT_IN)
			{
				if ((Asc2Float(HMERecv.Data[28], 8) + Asc2Float(HMERecv.Data[29], 8) + Asc2Float(HMERecv.Data[30], 8)) / 100 > 0.00)
				{
					szFee.Format("%08d", (Asc2Int(HMERecv.Data[28], 8) + Asc2Int(HMERecv.Data[29], 8) + Asc2Int(HMERecv.Data[30], 8)));
				}
			}
			else
			{
				if ((Asc2Float(HMERecv.Data[28], 8) + Asc2Float(HMERecv.Data[29], 8)) / 100 > 0.00)
				{
					szFee.Format("%08d", (Asc2Int(HMERecv.Data[28], 8) + Asc2Int(HMERecv.Data[29], 8)));
				}
			}

//			if(m_pDevCmn->AtmDefine.MachineKind == 8)
//			{
//				szMachineKind = "111";
//			}
//			else
//			if(m_pDevCmn->AtmDefine.MachineKind == 7)
//			{
//				szMachineKind = "113";
// 			}

			if(nAppRunType == AGENT_WITHDRAWAL)
			{
				iTranCode = '2';
				memcpy(&szTranUseCode,"110100",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",bAgentWithFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);           //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],GetSprintf("%d",CashCheckFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+107],GetSprintf("%016ds",Asc2Int(Accept.Money,sizeof(Accept.Money))),16);
				memcpy(&szAgentIdxTranData[nTempLen+123],"                ",16);   //支票金额
				memcpy(&szAgentIdxTranData[nTempLen+139],"                ",16);   //交易后余额
				memcpy(&szAgentIdxTranData[nTempLen+155],GetSprintf("%8.8s",szFee),8);
				memcpy(&szAgentIdxTranData[nTempLen+163],"0000",4);
				int cstCnt = m_pDevCmn->CSTCnt + 1;
				memcpy(&szAgentIdxTranData[nTempLen+167],GetSprintf("%d",cstCnt),1);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FstCSTLoc[%d]",m_pDevCmn->FstCSTLoc);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FstCSTStatus[%d]",m_pDevCmn->FstCSTStatus);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SndCSTLoc[%d]",m_pDevCmn->SndCSTLoc);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SndCSTStatus[%d]",m_pDevCmn->SndCSTStatus);
				
				memcpy(&szAgentIdxTranData[nTempLen+168], szTempAtmStatus, 110);
				nTempLen1 = nTempLen + 168;

				if(m_pDevCmn->CSTCnt >=1)
				{
					memcpy(&szAgentIdxTranData[nTempLen1+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+7],GetSprintf("%04d", CashFstCSTDispRejCnt), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1)), 4);
					nTempLen1 += 20;
				}
				if(m_pDevCmn->CSTCnt >=2)
				{
					memcpy(&szAgentIdxTranData[nTempLen1+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+7],GetSprintf("%04d", CashSndCSTDispRejCnt), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2)), 4);
					nTempLen1 += 20;
				}
				if(m_pDevCmn->CSTCnt >=3)
				{
					memcpy(&szAgentIdxTranData[nTempLen1+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+7],GetSprintf("%04d", CashTrdCSTDispRejCnt), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3)), 4);
					nTempLen1 += 20;
				}
				if(m_pDevCmn->CSTCnt >=4)
				{
					memcpy(&szAgentIdxTranData[nTempLen1+3],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+7],GetSprintf("%04d", CashFthCSTDispRejCnt), 4);
					memcpy(&szAgentIdxTranData[nTempLen1+11],GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4)), 4);
					nTempLen1 += 20;
				}
				memcpy(&szAgentIdxTranData[nTempLen1+3],GetSprintf("%04d",m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_REJECT)), 4);

			}
			else
			if(nAppRunType == AGENT_INQUIRY           ||
			   nAppRunType == AGENT_DETAIL            ||
			   nAppRunType == AGENT_OTHER_INQ         ||
			   nAppRunType == AGENT_REMIT_INF_INQ     ||
			   nAppRunType == AGENT_AC_CUSASSU_DETAIL ||
			   nAppRunType == AGENT_AC_PAY_INQ		  ||
			   nAppRunType == AGENT_AC_BALANCE_INQ	  ||			// added by liyi 2009.07.21
			   nAppRunType == AGENT_INQUIRY_CheckPin)
			{
				iTranCode = '3';
				if(nAppRunType == AGENT_INQUIRY)
					memcpy(&szTranUseCode,"100100",6);
				else
				if(nAppRunType == AGENT_DETAIL)
					memcpy(&szTranUseCode,"100200",6);
				else
				if(nAppRunType == AGENT_OTHER_INQ)
					memcpy(&szTranUseCode,"100400",6);
				else
				if(nAppRunType == AGENT_INQUIRY_CheckPin)
					memcpy(&szTranUseCode,"100100",6);			//jl20140307
				else
				if(nAppRunType == AGENT_REMIT_INF_INQ)
					memcpy(&szTranUseCode,"180500",6);
				else
				if(nAppRunType == AGENT_AC_CUSASSU_DETAIL || 
				   nAppRunType == AGENT_AC_PAY_INQ		  ||
				   nAppRunType == AGENT_AC_BALANCE_INQ)			// added by liyi 2009.07.21
					memcpy(&szTranUseCode,"000000",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode[0],6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22s",&szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22s",&szCardAccountNo),22);
				if(nAppRunType == AGENT_INQUIRY)
					memcpy(&szAgentIdxTranData[nTempLen+106],"1",1);
				else
				if(nAppRunType == AGENT_INQUIRY_CheckPin)
					memcpy(&szAgentIdxTranData[nTempLen+106],"1",1);
				else
				if(nAppRunType == AGENT_DETAIL)
					memcpy(&szAgentIdxTranData[nTempLen+106],"2",1);
				else
				if(nAppRunType == AGENT_OTHER_INQ)
					memcpy(&szAgentIdxTranData[nTempLen+106],"3",1);
				else
				if(nAppRunType == AGENT_REMIT_INF_INQ)
					memcpy(&szAgentIdxTranData[nTempLen+106],"7",1);
				else
				if(nAppRunType == AGENT_AC_CUSASSU_DETAIL)
					memcpy(&szAgentIdxTranData[nTempLen+106],"6",1);
				else
				if(nAppRunType == AGENT_AC_PAY_INQ)
					memcpy(&szAgentIdxTranData[nTempLen+106],"4",1);
				else
				if (nAppRunType == AGENT_AC_BALANCE_INQ)		// added by liyi 2009.07.21
				{
					memcpy(&szAgentIdxTranData[nTempLen+106],"8",1);
				}
				memcpy(&szAgentIdxTranData[nTempLen+107],GetSprintf("%016d",Asc2Int(&HMERecv.Data[54][1], 12)),16);
				memcpy(&szAgentIdxTranData[nTempLen+123],"          ", 10);        //预留
			}
			else
			if(nAppRunType == AGENT_TRANSFER      || 
			   nAppRunType == AGENT_TZH           || 
			   nAppRunType == AGENT_HZT           ||
			   nAppRunType == AGENT_AC_ASSU_TRAN  ||
			   nAppRunType == AGENT_AC_NOASSU_PAY)
			{
				if(nAppRunType == AGENT_TRANSFER)
				{
					iTranCode = '4';
					memcpy(&szTranUseCode,"110500",6);
				}
				else
				if(nAppRunType == AGENT_TZH)
				{
					iTranCode = 'F';
					memcpy(&szTranUseCode,"110600",6);
				}
				else
				if(nAppRunType == AGENT_HZT)
				{
					iTranCode = 'G';
					memcpy(&szTranUseCode,"110600",6);
				}
				else
				if(nAppRunType == AGENT_AC_ASSU_TRAN)
				{
					iTranCode = 'N';
					memcpy(&szTranUseCode,"000000",6);
				}
				else
				if(nAppRunType == AGENT_AC_NOASSU_PAY)
				{
					iTranCode = 'M';
					memcpy(&szTranUseCode,"000000",6);
				}
				
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],"0000",4);                  //转入银行代码
				memcpy(&szAgentIdxTranData[nTempLen+110],GetSprintf("%-28.28s",Accept.InAccountNum),28);
				if(nAppRunType == AGENT_AC_ASSU_TRAN || nAppRunType == AGENT_AC_NOASSU_PAY)
					memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d",Asc2Int(ACHostSend.TranMoney)),16);
				else
					memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d",Asc2Int(Accept.Money,sizeof(Accept.Money))),16);
				memcpy(&szAgentIdxTranData[nTempLen+154],"                ",16);    //转账后余额不清，不填
				
				if(nAppRunType == AGENT_AC_ASSU_TRAN || nAppRunType == AGENT_AC_NOASSU_PAY)
				{
					CString tempFee = "";
					tempFee.Format("%s", ACHostResp.CommCharge);
					tempFee.Replace(".", "");
					memcpy(&szAgentIdxTranData[nTempLen+170],GetSprintf("%8.8s",tempFee),8);
				}
				else
					memcpy(&szAgentIdxTranData[nTempLen+170],GetSprintf("%8.8s",szFee),8);
				memcpy(&szAgentIdxTranData[nTempLen+178],"          ", 10);        //预留
			}
			else
			if(nAppRunType == AGENT_CARD_RETRACT)
			{
				iTranCode = 'D';
				memcpy(&szTranUseCode,"      ",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				
				if(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))			// 2009.03.31 yusy
				{
					memcpy(&szAgentIdxTranData[nTempLen+8],"1",1);
					memcpy(&szAgentIdxTranData[nTempLen+9],"0",1);
				}
				else
				{
					memcpy(&szAgentIdxTranData[nTempLen+9],"1",1);
					memcpy(&szAgentIdxTranData[nTempLen+8],"1",1);
				}
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],"    ",4);
				memcpy(&szAgentIdxTranData[nTempLen+110],"                            ",28);
				memcpy(&szAgentIdxTranData[nTempLen+138],"                ",16);
				memcpy(&szAgentIdxTranData[nTempLen+154],"                ",16);
				memcpy(&szAgentIdxTranData[nTempLen+170],"        ",8);
				memcpy(&szAgentIdxTranData[nTempLen+178],"          ", 10);        //预留
			}
			else
			if(nAppRunType == AGENT_CANCEL )
			{
				iTranCode = '7';
				memcpy(&szTranUseCode,"110230",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHLOCALCARDFLAG),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],"    ",4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT)),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHACCOUNT)),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],GetSprintf("%016s",IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHAMOUNT)),16);
				

				char szTmpData[1024];
				memset(szTmpData, 0, sizeof(szTmpData));
				MakePack(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetBuffer(0), 
						 szTmpData, 
						 IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHDATETIME).GetLength());

				if (IsNum(szTmpData, 14))
   	    			memcpy(&szAgentIdxTranData[nTempLen+122],GetSprintf("%14.14s",&szTmpData[0]),14);            //交易日期+时间

				memcpy(&szAgentIdxTranData[nTempLen+136],GetSprintf("%-8.8s",IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHSERIALNO)),8);
				memcpy(&szAgentIdxTranData[nTempLen+144],GetSprintf("%-12.12s",IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_REFERENCENO)),12);
       			memcpy(&szAgentIdxTranData[nTempLen+156],GetSprintf("%-8.8s",IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFEE)),8);
       			memcpy(&szAgentIdxTranData[nTempLen+164],"          ",10);
			}
			else
			if(nAppRunType == AGENT_CHANGEPIN )
			{
				iTranCode = '6';
				memcpy(&szTranUseCode,"150100",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",szCardAccountNo),22);
                memcpy(&szAgentIdxTranData[nTempLen+106],"          ",10);
			}
			else
			if(nAppRunType == AGENT_ADDCASH) 
			{
				iTranCode = '9';
				memcpy(&szTranUseCode,"160800",6);
				nTempLen =0;
				szTranCode = '7';   //日志种类区分 7:加钞

				memcpy(&szAgentIdxTranData[nTempLen],GetSprintf("%d",m_pDevCmn->CSTCnt),1);
				if (m_pDevCmn->CSTCnt >= 1)
				{
					memcpy(&szAgentIdxTranData[nTempLen+1], "1", 1);
					if(m_pDevCmn->FstCSTValue == 50)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 2], "01", 2);
					}
					else
					if(m_pDevCmn->FstCSTValue == 100)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 2], "02", 2);
					}
					else
					{
						memcpy(&szAgentIdxTranData[nTempLen + 2], "00", 2);
					}
					memcpy(&szAgentIdxTranData[nTempLen + 4], GetSprintf("%04d", m_pDevCmn->FstCSTSetCash), 4);
					memcpy(&szAgentIdxTranData[nTempLen + 8], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)), 4);
					memcpy(&szAgentIdxTranData[nTempLen +12], GetSprintf("%8.8s", "        "), 8);
					memcpy(&szAgentIdxTranData[nTempLen +20],GetSprintf("%10.10s","          "),10);

				}
				if (m_pDevCmn->CSTCnt >= 2)
				{
					memcpy(&szAgentIdxTranData[nTempLen+20], "2", 1);
					if(m_pDevCmn->SndCSTValue == 50)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 21], "01", 2);
					}
					else
					if(m_pDevCmn->SndCSTValue == 100)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 21], "02", 2);
					}
					else
					{
						memcpy(&szAgentIdxTranData[nTempLen + 21], "00", 2);
					}
					memcpy(&szAgentIdxTranData[nTempLen + 23], GetSprintf("%04d", m_pDevCmn->SndCSTSetCash), 4);
					memcpy(&szAgentIdxTranData[nTempLen + 27], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)), 4);
					memcpy(&szAgentIdxTranData[nTempLen +31], GetSprintf("%8.8s", "        "), 8);
					memcpy(&szAgentIdxTranData[nTempLen +39],GetSprintf("%10.10s","          "),10);
				}
				if (m_pDevCmn->CSTCnt >= 3)
				{
					memcpy(&szAgentIdxTranData[nTempLen+39], "3", 1);
					if(m_pDevCmn->TrdCSTValue == 50)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 40], "01", 2);
					}
					else
					if(m_pDevCmn->TrdCSTValue == 100)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 40], "02", 2);
					}
					else
					{
						memcpy(&szAgentIdxTranData[nTempLen + 40], "00", 2);
					}
					memcpy(&szAgentIdxTranData[nTempLen + 42], GetSprintf("%04d", m_pDevCmn->TrdCSTSetCash), 4);
					memcpy(&szAgentIdxTranData[nTempLen + 46], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)), 4);
					memcpy(&szAgentIdxTranData[nTempLen + 50], GetSprintf("%8.8s", "        "), 8);		
					memcpy(&szAgentIdxTranData[nTempLen + 58],GetSprintf("%10.10s","          "),10);
				}
				if (m_pDevCmn->CSTCnt >= 4)
				{
					memcpy(&szAgentIdxTranData[nTempLen + 58], "4", 1);
					if(m_pDevCmn->FthCSTSetCash == 50)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 59], "01", 2);
					}
					else
					if(m_pDevCmn->FthCSTSetCash == 100)
					{
               			memcpy(&szAgentIdxTranData[nTempLen + 59], "02", 2);
					}
					else
					{
						memcpy(&szAgentIdxTranData[nTempLen + 59], "00", 2);
					}			
					memcpy(&szAgentIdxTranData[nTempLen + 61], GetSprintf("%04d", m_pDevCmn->FthCSTSetCash), 4);
					memcpy(&szAgentIdxTranData[nTempLen + 65], GetSprintf("%04d", m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)), 4);
					memcpy(&szAgentIdxTranData[nTempLen + 69], GetSprintf("%8.8s", "        "), 8);		
					memcpy(&szAgentIdxTranData[nTempLen + 77],GetSprintf("%10.10s","          "),10);
				}
        
			}
			else
			if(nAppRunType  == AGENT_OPEN)
			{
				iTranCode = '9';
				memcpy(&szTranUseCode,"160300",6);
				nTempLen =0;
				szTranCode = '1';   //日志种类区分 4:交易
				CStringArray strTempArray;

				strTemp = GetShareData("OP_VERSION");				// OP_VERSION
				SplitString(strTemp, "||", strTempArray);

				memcpy(&szAgentIdxTranData[nTempLen],&szCompanyCode,2);

			    if(m_pDevCmn->AtmDefine.MachineKind == 8)
				{
					memcpy(&szAgentMachineKind,"111",3);
				}
				else
				if(m_pDevCmn->AtmDefine.MachineKind == 7)
				{
					memcpy(&szAgentMachineKind,"113",3);
				}
			    memcpy(&szAgentMachineKind[0],GetSprintf("%-3.3s",IniGetStr(_AGENT_ATMS,_AGENT_ATM_AP,"MACHINEKIND",szAgentMachineKind)),3);

				memcpy(&szAgentIdxTranData[nTempLen+2],&szAgentMachineKind,3);

				char	IpAddress[256];
				memset(IpAddress, 0, sizeof(IpAddress));
				sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmIPAddress, 3),
													  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[4], 3),
													  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[8], 3),
													  Asc2Int(&m_pProfile->NETWORK.AtmIPAddress[12], 3));

				memcpy(&szAgentIdxTranData[nTempLen+5],GetSprintf("%-15.15s",&IpAddress),15);
				// TEMP 北京测试
				if (m_pDevCmn->CashCashMode	== CSH_TESTCASH_MODE)
					memcpy(&szAgentIdxTranData[nTempLen+5], "221.238.92.49  ",15);

				memset(IpAddress, 0, sizeof(IpAddress));
				sprintf(IpAddress, "%d.%d.%d.%d", Asc2Int(m_pProfile->NETWORK.AtmGateWay, 3),
										  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[4], 3),
										  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[8], 3),
										  Asc2Int(&m_pProfile->NETWORK.AtmGateWay[12], 3));
				
				memcpy(&szAgentIdxTranData[nTempLen+20],GetSprintf("%-15.15s",&IpAddress),15);
				
				memcpy(&szAgentIdxTranData[nTempLen+35],GetSprintf("%-15.15s",&szAgentFtpServerIP),15);
				memcpy(&szAgentIdxTranData[nTempLen+50],GetSprintf("%-16.16s",strTempArray[0]),16);
				memcpy(&szAgentIdxTranData[nTempLen+66],GetSprintf("%-16.16s",strTempArray[11]),16);
				memcpy(&szAgentIdxTranData[nTempLen+82],GetSprintf("%-16.16s",strTempArray[4]),16);
				memcpy(&szAgentIdxTranData[nTempLen+98],GetSprintf("%-16.16s",strTempArray[3]),16);
				memcpy(&szAgentIdxTranData[nTempLen+114],GetSprintf("%-16.16s",strTempArray[6]),16);
				memcpy(&szAgentIdxTranData[nTempLen+130],GetSprintf("%-16.16s",strTempArray[5]),16);
				memcpy(&szAgentIdxTranData[nTempLen+146],GetSprintf("%-16.16s",strTempArray[2]),16);
				memcpy(&szAgentIdxTranData[nTempLen+162],GetSprintf("%-16.16s",strTempArray[15]),16);
				memcpy(&szAgentIdxTranData[nTempLen+178],GetSprintf("%-16.16s",strTempArray[14]),16);
				memcpy(&szAgentIdxTranData[nTempLen+194],GetSprintf("%-16.16s",strTempArray[17]),16);
				memcpy(&szAgentIdxTranData[nTempLen+210],GetSprintf("%-16.16s",strTempArray[16]),16);
				memcpy(&szAgentIdxTranData[nTempLen+226],GetSprintf("%-16.16s",strTempArray[13]),16);
				memcpy(&szAgentIdxTranData[nTempLen+242],"                ",16);    
				memcpy(&szAgentIdxTranData[nTempLen+242],"                ",16);    
				memcpy(&szAgentIdxTranData[nTempLen+258],GetSprintf("%-20.20s",IniGetStr(_AGENT_ATMS,_AGENT_ATM_AP,"MACHINECODE","00000000000000000000")),20);    
				memcpy(&szAgentIdxTranData[nTempLen+278],"          ",10);

			}	
			else
			if(nAppRunType == AGENT_ERRORMODE ||nAppRunType == AGENT_STATUS)
			{
				iTranCode = '9';
				nTempLen =0;
			    
				//AGENT_ERRORMODE 分两种情况 1:设备故障时发故障 2:设备故障恢愎发时状态
				if(nAppRunType == AGENT_ERRORMODE)
				{
					szTranCode = '2';   //日志种类区分 2:故障 3:状态
				}
				else
				{
					szTranCode = '3';
				}
				
				szMaterialStatus = '0';

				m_pProfile->GetTransProfile();
				int tempJPRPrintCnt = m_pProfile->TRANS.JPRPrintCnt;
				int tempSPRPrintCnt = m_pProfile->TRANS.SPRPrintCnt;
				int tempINKPrintCnt = m_pProfile->TRANS.INKPrintCnt;
				m_pProfile->PutTransProfile();

				strTemp = GetShareData("OP_COMMAND");
				SplitString(strTemp, "||", strTempArray);
				if (strTempArray[0] == "OP_CHANGESPRPAPER")
				{
					szMaterialStatus = '1';
					m_pProfile->GetTransProfile();
					m_pProfile->TRANS.SPRPrintCnt = 0;
					m_pProfile->PutTransProfile();
				}
				else
				if (strTempArray[0] == "OP_CHANGEJPRPAPER")
				{
					szMaterialStatus = '2';
					m_pProfile->GetTransProfile();
					m_pProfile->TRANS.JPRPrintCnt = 0;
					m_pProfile->PutTransProfile();
				}
				else
				if (strTempArray[0] == "OP_CHANGEINK")
				{
					szMaterialStatus = '3';
					m_pProfile->GetTransProfile();
					m_pProfile->TRANS.INKPrintCnt = 0;
					m_pProfile->PutTransProfile();
				}

				memcpy(&szAgentIdxTranData[nTempLen],&szCompanyCode,2);    //公司代码
                
				if(m_pDevCmn->AtmDefine.MachineKind == 8)
				{
					memcpy(&szAgentMachineKind,"111",3);
				}
				else
				if(m_pDevCmn->AtmDefine.MachineKind == 7)
				{
					memcpy(&szAgentMachineKind,"113",3);
				}
			    memcpy(&szAgentMachineKind[0],GetSprintf("%-3.3s",IniGetStr(_AGENT_ATMS,_AGENT_ATM_AP,"MACHINEKIND",szAgentMachineKind)),3);

				memcpy(&szAgentIdxTranData[nTempLen+2],&szAgentMachineKind,3);

				if(szAgentErrorCode[0] != 'X')
					memcpy(&szAgentIdxTranData[nTempLen+5],GetSprintf("%c", szAgentErrorCode[0]),1);
				else
					memcpy(&szAgentIdxTranData[nTempLen+5],GetSprintf("%c",m_pDevCmn->CurErrBuff.ProcCount[0]),1);
				
				if((nAppRunType == AGENT_ERRORMODE && m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN)) ||
					(nAppRunType == AGENT_ERRORMODE && !m_pDevCmn->fnAPL_CheckError()) ||
					(nAppRunType == AGENT_ERRORMODE && nAgentAtmAfterOpenFistFailFlag == TRUE && m_pDevCmn->HostOpenFlag == FALSE))
				{
					if(szAgentErrorCode[1] != 'X')
					{
						memcpy(&szAgentIdxTranData[nTempLen+6], &szAgentErrorCode[1],10);
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "szAgentErrorCode return[%12.12s]", szAgentErrorCode);
						memset(&szAgentErrorCode[0], 'X', 11);
					}
					else
						memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%-10.10s",GetSprintf("%7.7s",m_pDevCmn->CurErrBuff.ErrorCode)),10);
				}
				else
 					memcpy(&szAgentIdxTranData[nTempLen+6],"          ",10);
				
				szCSTStatus = '0';
                szCSTStatus = fnAPP_GetCstStatus();               //Cdu status
				szCduStatus = '0';
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetCstStatus return[%c]",szCduStatus);

				if (m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER)    //spr status
					szSprPaperStatus = '2';
				else
				if (m_pDevCmn->SlipStatus == SLIP_NORMAL)
					szSprPaperStatus = '0';
				else
				if (m_pDevCmn->SlipStatus == SLIP_LOW_END)
					szSprPaperStatus = '1';

				if (m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)     //jpr status
					szJprPaperStatus = '2';
				else
				if (m_pDevCmn->JnlStatus == JNL_NORMAL)
					szJprPaperStatus = '0';
				else
				if (m_pDevCmn->JnlStatus == JNL_LOW_END)
					szJprPaperStatus = '1';

				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->JnlStatus return[%d]",m_pDevCmn->JnlStatus);

                cErrorCode = ' ';      	  //模块状态 
				szMcuStatus = '0';        //读卡器  0：正常 1:满
				szPbStatus  = '0';        //存折打印机状态 0：正常 1:满 
				szBrmStatus = '0';
               	szPinStatus = '0';
				szCommStatus = '0';

                // cdu jpr spr  0:正常 1:少 2:空 8:error 9:miss 
				// mcu pb       0:正常 1:满 8:error 9:miss 
                // 管理设备 DES IC MEV RF IR DVR PIN 0:not support 1:ok 9:error
				// 注意不同设备的错误码表示不同状态  BY HYL

				if(nAppRunType == AGENT_ERRORMODE)
				{
                    nAgentErrorDeviceNum = 0;
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))				// Mcu Status
					{
						szMcuStatus= '8';
						nAgentErrorDeviceNum++;
					}
					else
 						szMcuStatus= '0';

					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))				// PIN Status
					{
						szPinStatus = '8';
						nAgentErrorDeviceNum++;
					}
					else
 						szPinStatus = '0';

					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))				// JPR Status
					{
						szJprStatus= '8';
						nAgentErrorDeviceNum++;
					}
					else
 						szJprStatus= '0';
					
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))				// CDU Status
					{	
						szCduStatus= '8';
					    nAgentErrorDeviceNum++;
					}
					else
 						szCduStatus= '0';

//					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_BRM))				// BRM Status
//					{	
//						szBrmStatus = '8';
//					    nAgentErrorDeviceNum++;
//					}
//					else
//						szBrmStatus = '0';

					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))				// SPR Status
					{	
						szSprStatus = '8';
						nAgentErrorDeviceNum++;
					}
					else
 						szSprStatus = '0';

					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PBM))				// PBM Status
					{
						szPbStatus = '8';
						nAgentErrorDeviceNum++;
					}
					else
						szPbStatus = '0';

					if((nAppRunType == AGENT_ERRORMODE)			&& 
// 					   (nAgentAtmAfterOpenFistFailFlag == TRUE)	&&
					   (!m_pDevCmn->fnAPL_CheckHostLine()))
					{
						cErrorCode = 'A';                                 //通讯链路断开
					    memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%-10.10s",GetSprintf("%7.7s","7710891")),10);
					}

					//故障位，只有一位取最主要故障
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))		// Mcu Status
					    cErrorCode = '3';
					else
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))				// PIN Status
						cErrorCode = '6';
					else
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))				// JPR Status
						cErrorCode = '4';
					else
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))				// CDU Status	// 2008.11.18
						cErrorCode = '1';
					else
// 					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_BRM))				// BRM Status	// 2008.11.18
// 						cErrorCode = '2';
// 					else
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))				// SPR Status
						cErrorCode = '5';
					else
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PBM))				// PBM Status
						cErrorCode = '7';


					if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))
						szCduStatus= '9';   
					if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))
						szMcuStatus= '9';  
					if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))
						szJprStatus= '9';  
					if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_PIN))
						szPinStatus= '9';  
					if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))
						szSprStatus= '9';  
					
					if(!m_pDevCmn->fnAPL_CheckHostLine())							// 2009.03.30 yusy
						szCommStatus = '8';
				}
				else
				{
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))				// Mcu Status
						szMcuStatus= '8';
					else
 						szMcuStatus= '0';
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))				// PIN Status
						szPinStatus = '8';
					else
 						szPinStatus = '0';
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))				// JPR Status
						szJprStatus= '8';
					else
 						szJprStatus= '0';
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))				// CDU Status
						szCduStatus= '8';
					else
 						szCduStatus= '0';
					if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))				// SPR Status
						szSprStatus = '8';
					else
 						szSprStatus = '0';
					
					if(!m_pDevCmn->fnAPL_CheckHostLine())							// 2009.03.30 yusy
						szCommStatus = '8';
				}
				szAgentCheckErrorCode[0] = szCommStatus;

                szDvrStatus = '0';                                    //管理设备 9:no support 0:ok 8:error
				if(InitInfo.TranDvrMode == 1)
				{
//				    cDvrstatus = IniGetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRSTATUS,"0002");
//					cDvrChanStatus = IniGetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_CHANSTATUS,"2222");
					if(memcmp(InitInfo.DvrStatus,"0004",4) == 0 ||
						InitInfo.DvrChanStatus[0] == '2'||
						InitInfo.DvrChanStatus[1] == '2'||
						InitInfo.DvrChanStatus[2] == '2'||
						InitInfo.DvrChanStatus[3] == '2')    //by hyl 
					{
						szDvrStatus = '8';
						nAgentErrorDeviceNum++;
					}

					if(nAgentAtmDvrErrorFlag == TRUE)
					{
                        cErrorCode = 'B';                                 //通讯链路断开
					    memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%-10.10s",GetSprintf("%7.7s","9702013")),10);

					}
				}
				else
					szDvrStatus = '9';
						
				memcpy(&szAgentIdxTranData[nTempLen+16],GetSprintf("%c",cErrorCode),1);
				memcpy(&szAgentIdxTranData[nTempLen+17],m_pProfile->TRANS.SerialNo,8);

				// added by yusy 08.09.09
				szAtmStatus = '0';
				szTTUStatus = '0';
				szMcuRetractStatus = '0';
				szPbRetractStatus = '0';

				memcpy(&szAgentIdxTranData[nTempLen+25],&szCSTStatus,1);
				memcpy(&szAgentIdxTranData[nTempLen+26],&szSprPaperStatus,1);
				memcpy(&szAgentIdxTranData[nTempLen+27],&szJprPaperStatus,1);
				memcpy(&szAgentIdxTranData[nTempLen+28],&szMcuRetractStatus,1);
				memcpy(&szAgentIdxTranData[nTempLen+29],&szPbRetractStatus,1);
				
				if(m_pDevCmn->AtmStatus == ATM_ERROR)
					szAtmStatus = '2';
				else
				if(m_pDevCmn->AtmStatus == ATM_CUSTOM || m_pDevCmn->AtmStatus == ATM_TRAN)
					szAtmStatus = '0';
				else
					szAtmStatus = '1';

				szAgentCheckErrorCode[1] = szAtmStatus;

				szBox1Status = '9';
				szBox2Status = '9';
				szBox3Status = '9';
				szBox4Status = '9';
				szRejectStatus = '9';
				// box1
				if(m_pDevCmn->CSTCnt >= 1)
				{
					if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
						szBox1Status = '9';
					else
					if (m_pDevCmn->FstCSTStatus == CST_NORMAL)
						szBox1Status = '0';
					else
					if (m_pDevCmn->FstCSTStatus == CST_NEAR)
						szBox1Status = '1';
					else
					if (m_pDevCmn->FstCSTStatus == CST_EMPTY)
						szBox1Status = '2';
					else
					if (m_pDevCmn->FstCSTStatus == CST_FULL)
						szBox1Status = '0';
					else
					if (m_pDevCmn->FstCSTStatus == CST_ERROR)
						szBox1Status = '8';
					else
					if (m_pDevCmn->FstCSTStatus == CST_HIGH)
						szBox1Status = '0';
				}

				// box2
				if(m_pDevCmn->CSTCnt >= 2)
				{
					if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
						szBox2Status = '9';
					else
					if (m_pDevCmn->SndCSTStatus == CST_NORMAL)
						szBox2Status = '0';
					else
					if (m_pDevCmn->SndCSTStatus == CST_NEAR)
						szBox2Status = '1';
					else
					if (m_pDevCmn->SndCSTStatus == CST_EMPTY)
						szBox2Status = '2';
					else
					if (m_pDevCmn->SndCSTStatus == CST_FULL)
						szBox2Status = '0';
					else
					if (m_pDevCmn->SndCSTStatus == CST_ERROR)
						szBox2Status = '8';
					else
					if (m_pDevCmn->SndCSTStatus == CST_HIGH)
						szBox2Status = '0';
				}

				// box3
				if(m_pDevCmn->CSTCnt >= 3)
				{
					if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
						szBox3Status = '9';
					else
					if (m_pDevCmn->TrdCSTStatus == CST_NORMAL)
						szBox3Status = '0';
					else
					if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
						szBox3Status = '1';
					else
					if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)
						szBox3Status = '2';
					else
					if (m_pDevCmn->TrdCSTStatus == CST_FULL)
						szBox3Status = '0';
					else
					if (m_pDevCmn->TrdCSTStatus == CST_ERROR)
						szBox3Status = '8';
					else
					if (m_pDevCmn->TrdCSTStatus == CST_HIGH)
						szBox3Status = '0';
				}

				// box4
				if(m_pDevCmn->CSTCnt >= 4)
				{
					if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
						szBox4Status = '9';
					else
					if (m_pDevCmn->FthCSTStatus == CST_NORMAL)
						szBox4Status = '0';
					else
					if (m_pDevCmn->FthCSTStatus == CST_NEAR)
						szBox4Status = '1';
					else
					if (m_pDevCmn->FthCSTStatus == CST_EMPTY)
						szBox4Status = '2';
					else
					if (m_pDevCmn->FthCSTStatus == CST_FULL)
						szBox4Status = '0';
					else
					if (m_pDevCmn->FthCSTStatus == CST_ERROR)
						szBox4Status = '8';
					else
					if (m_pDevCmn->FthCSTStatus == CST_HIGH)
						szBox4Status = '0';
				}

				// reject
				if (m_pDevCmn->RejectCSTLoc != NORMAL_POS)
					szRejectStatus = '9';
				else
				if (m_pDevCmn->RejectCSTStatus == CST_NORMAL)
					szRejectStatus = '0';
				else
				if (m_pDevCmn->RejectCSTStatus == CST_NEAR)
					szRejectStatus = '1';
				else
				if (m_pDevCmn->RejectCSTStatus == CST_EMPTY)
					szRejectStatus = '0';
				else
				if (m_pDevCmn->RejectCSTStatus == CST_FULL)
					szRejectStatus = '4';
				else
				if (m_pDevCmn->RejectCSTStatus == CST_HIGH)
					szRejectStatus = '3';

				//更新Agent保存标准状态位
				if((nAppRunType == AGENT_ERRORMODE && m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN)) ||
					(nAppRunType == AGENT_ERRORMODE && !m_pDevCmn->fnAPL_CheckError())               ||
				    (nAppRunType == AGENT_ERRORMODE && m_pDevCmn->RejectCSTStatus == CST_FULL)       ||
 					(nAppRunType == AGENT_ERRORMODE && nAgentAtmDvrErrorFlag == TRUE)                ||
					(nAppRunType == AGENT_ERRORMODE && nAgentAtmAfterOpenFistFailFlag == TRUE))		// 2008.11.18
				{
     	            memcpy(&szAgentOldErrorStatus,GetSprintf("%c%c%c%c%c%c%c",szPinStatus,szSprStatus,szJprStatus,szMcuStatus,szPbStatus,szPinStatus,szDvrStatus),7);
					memcpy(&szAgentOldStatus[0],GetSprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c",szPinStatus,szSprStatus,szJprStatus,szMcuStatus,szAtmStatus,szCSTStatus,szSprPaperStatus,szJprPaperStatus,szBox1Status,szBox2Status,szBox3Status,szBox4Status,szRejectStatus),13);
				}
                else
					memcpy(&szAgentOldStatus[0],GetSprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c",szPinStatus,szSprStatus,szJprStatus,szMcuStatus,szAtmStatus,szCSTStatus,szSprPaperStatus,szJprPaperStatus,szBox1Status,szBox2Status,szBox3Status,szBox4Status,szRejectStatus),13);

				iDevNum = 12;
				if(InitInfo.TranDvrMode == 1)		
					iDevNum++;
				
				memcpy(&szAgentIdxTranData[nTempLen+30],GetSprintf("%02d",iDevNum),2);
				
				memcpy(&szAgentIdxTranData[nTempLen+32],"1",1);

				//增加取款锁定 added by liuxl 20110610
				int icwdmaxshutter = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM, 1);
				if ( icwdmaxshutter > 0 &&
					IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CWDCURSHUTTERNUM, 0) == icwdmaxshutter)
				{
					szCduStatus = '8';
 				}

                memcpy(&szAgentIdxTranData[nTempLen+33],&szCduStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen+34],"3",1);
                memcpy(&szAgentIdxTranData[nTempLen+35],&szMcuStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen+36],"4",1);
                memcpy(&szAgentIdxTranData[nTempLen+37],&szJprStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen+38],"5",1);
                memcpy(&szAgentIdxTranData[nTempLen+39],&szSprStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen+40],"6",1);
                memcpy(&szAgentIdxTranData[nTempLen+41],&szPinStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen+42],"A",1);
                memcpy(&szAgentIdxTranData[nTempLen+43],&szCommStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen+44],"C",1);
                memcpy(&szAgentIdxTranData[nTempLen+45],&szTTUStatus,1);
				nTempLen1 = nTempLen+46;

                if(InitInfo.TranDvrMode == 1)
				{
				    memcpy(&szAgentIdxTranData[nTempLen+46],"B",1);// 
                    memcpy(&szAgentIdxTranData[nTempLen+47],GetSprintf("%c",szDvrStatus),1);
					nTempLen1 = nTempLen+48;
				}

				memcpy(&szAgentIdxTranData[nTempLen1],"F",1);
                memcpy(&szAgentIdxTranData[nTempLen1+1],&szBox1Status,1);

				memcpy(&szAgentIdxTranData[nTempLen1+2],"G",1);
                memcpy(&szAgentIdxTranData[nTempLen1+3],&szBox2Status,1);

				memcpy(&szAgentIdxTranData[nTempLen1+4],"H",1);
                memcpy(&szAgentIdxTranData[nTempLen1+5],&szBox3Status,1);

				memcpy(&szAgentIdxTranData[nTempLen1+6],"I",1);
                memcpy(&szAgentIdxTranData[nTempLen1+7],&szBox4Status,1);

				memcpy(&szAgentIdxTranData[nTempLen1+8],"M",1);
                memcpy(&szAgentIdxTranData[nTempLen1+9],&szRejectStatus,1);

				memcpy(&szAgentIdxTranData[nTempLen1+10],&szAtmStatus,1);
				memcpy(&szAgentIdxTranData[nTempLen1+11],GetSprintf("%c",szMaterialStatus),1);
				
				memcpy(&szAgentIdxTranData[nTempLen1+12],GetSprintf("%010d",tempSPRPrintCnt),10);
				memcpy(&szAgentIdxTranData[nTempLen1+22],GetSprintf("%010d",tempJPRPrintCnt),10);
				memcpy(&szAgentIdxTranData[nTempLen1+32],GetSprintf("%010d",tempINKPrintCnt),10);
				
                //memcpy(&szAgentIdxTranData[nTempLen1],GetSprintf("%-10s","          "),10);	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "###TranData: return[%s]",szAgentIdxTranData);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "###Status: [%s]",szAgentOldStatus);
			}
			else
			if(nAppRunType == AGENT_ATMPTOTAL || nAppRunType == AGENT_ADDCASHTOTAL ||nAppRunType == AGENT_ATMCTOTAL)
			{
				iTranCode = '9';
				nTempLen =0;
				szTranCode = '6';                                     //日志种类区分 4:对账
				if(nAppRunType == AGENT_ATMPTOTAL)
				{
					memcpy(&szAgentIdxTranData[nTempLen],"1",1);
					memcpy(&szAgentIdxTranData[nTempLen+1],GetSprintf("%d",bCommRecvFlag),1);
					memcpy(&szAgentIdxTranData[nTempLen+2],GetSprintf("%d",iTranOkFlag),1);

				}
				else
                if(nAppRunType == AGENT_ADDCASHTOTAL)
				{
					memcpy(&szAgentIdxTranData[nTempLen],"2",1);       //1:对账，2：本机加钞统计 3:操作员统计  
					memcpy(&szAgentIdxTranData[nTempLen+1],"1",1);
					memcpy(&szAgentIdxTranData[nTempLen+2],"1",1);
				}
				else
                if(nAppRunType == AGENT_ATMCTOTAL)
				{
					memcpy(&szAgentIdxTranData[nTempLen],"3",1);       //1:对账，2：本机加钞统计 3:操作员统计  
					memcpy(&szAgentIdxTranData[nTempLen+1],"1",1);
					memcpy(&szAgentIdxTranData[nTempLen+2],"1",1);
				}

				memcpy(&szAgentIdxTranData[nTempLen+3],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+7],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+11],&szErrorMsg,40);
				
				memcpy(&szAgentIdxTranData[nTempLen+51],"03",2);   //对账种类个数
                //需要增加存款模块 by hyl

				//现金存款
				memcpy(&szAgentIdxTranData[nTempLen+53],"02",2);
				memcpy(&szAgentIdxTranData[nTempLen+55],GetSprintf("%-4.4s",GetSprintf("%d",Asc2Int(&m_pProfile->WITHTOTAL.DATA[0].Count[2], 4))),4);
				memcpy(&szAgentIdxTranData[nTempLen+59],GetSprintf("00%-14.14s",&m_pProfile->WITHTOTAL.DATA[0].Amount[0]),16);
				//转账  
				memcpy(&szAgentIdxTranData[nTempLen+75],"05",2);
				memcpy(&szAgentIdxTranData[nTempLen+77],GetSprintf("%-4.4s",GetSprintf("%d",Asc2Int(&m_pProfile->TRANSTOTAL.DATA[0].Count[2],4))),4);
				memcpy(&szAgentIdxTranData[nTempLen+81],GetSprintf("00%-14.14s",&m_pProfile->TRANSTOTAL.DATA[0].Amount[0]),16);
				//冲正
				memcpy(&szAgentIdxTranData[nTempLen+97],"06",2);
				memcpy(&szAgentIdxTranData[nTempLen+99],GetSprintf("%-4.4s",GetSprintf("%d",Asc2Int(&m_pProfile->WITHTOTAL.DATA[5].Count[2],4))),4);
				memcpy(&szAgentIdxTranData[nTempLen+103],GetSprintf("00%-14.14s",&m_pProfile->WITHTOTAL.DATA[5].Amount[0]),16);

				memcpy(&szAgentIdxTranData[nTempLen+119],GetSprintf("%-10s","          "),10);	
        
			}
			else
			if(nAppRunType == AGENT_PAY  ||
			   nAppRunType == AGENT_AC_PAY )
			{
				if(nAppRunType == AGENT_PAY )
				{   
					if( PayType == S_PAY1)
					{
                	    memcpy(&szTranUseCode,"140200",6);
						iTranCode = 'A';
					}
					else
					if( PayType == S_PAY2)
					{
						memcpy(&szTranUseCode,"140400",6);
						iTranCode = 'I';
					}
				}
				else
				if(nAppRunType == AGENT_AC_PAY)
                    memcpy(&szTranUseCode,"000000",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],"0000",4);                  //转入银行代码
                
				if(nAppRunType == AGENT_PAY )
				{
				    memcpy(&szAgentIdxTranData[nTempLen+110],GetSprintf("%-28.28s",Accept.InAccountNum),28);
			    	memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d",Asc2Int(Accept.Money,sizeof(Accept.Money))),16);
				}
				else
				if(nAppRunType == AGENT_AC_PAY)
				{
				    memcpy(&szAgentIdxTranData[nTempLen+110],GetSprintf("%-28.28s",ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount),28);
				    memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d", atof(ACHostSend.TranMoney)*100),16);
				}
				memcpy(&szAgentIdxTranData[nTempLen+154],"                ",16);    //转账后余额不清，不填
				memcpy(&szAgentIdxTranData[nTempLen+170],GetSprintf("%8.8s",szFee),8);
				memcpy(&szAgentIdxTranData[nTempLen+178],"          ", 10);        //预留
			}
			else
			if(nAppRunType == AGENT_REMIT_ADD ||
			   nAppRunType == AGENT_REMIT_PWD ||
			   nAppRunType == AGENT_AC_REMIT  ||
			   nAppRunType == AGENT_REMIT_IN)
			{
				if(nAppRunType == AGENT_REMIT_ADD)
				{
					memcpy(&szTranUseCode,"180200",6);
					iTranCode = 'J';
				}
				else
				if(nAppRunType == AGENT_REMIT_PWD)
				{
					memcpy(&szTranUseCode,"180200",6);
					iTranCode = 'K';
				}
				else
				if(nAppRunType == AGENT_REMIT_IN)
				{
					memcpy(&szTranUseCode,"180600",6);
					iTranCode = 'L';
				}
				else
				if(nAppRunType == AGENT_AC_REMIT)
					memcpy(&szTranUseCode,"000000",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],"0000",4);                  //转入银行代码
                if(nAppRunType == AGENT_REMIT_ADD || nAppRunType == AGENT_REMIT_PWD || nAppRunType == AGENT_REMIT_IN)
				{
				    memcpy(&szAgentIdxTranData[nTempLen+110],GetSprintf("%-28.28s",Accept.InAccountNum),28);
			    	memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d",Asc2Int(Accept.Money,sizeof(Accept.Money))),16);
				}
				else
				if(nAppRunType == AGENT_AC_REMIT)
				{
				    memcpy(&szAgentIdxTranData[nTempLen+110],GetSprintf("%-28.28s",ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount),28);
				    memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d", atof(ACHostSend.TranMoney)*100),16);
				}			    
				memcpy(&szAgentIdxTranData[nTempLen+154],"                ",16);    //转账后余额不清，不填
				memcpy(&szAgentIdxTranData[nTempLen+170],GetSprintf("%8.8s",szFee),8);
				memcpy(&szAgentIdxTranData[nTempLen+178],"          ", 10);        //预留
			}
			else
			if(nAppRunType == AGENT_AC_TRANSFER)
			{
				iTranCode = 'C';
				memcpy(&szTranUseCode,"000000",6);
				nTempLen =0;
				szTranCode = '4';   //日志种类区分 4:交易
				memcpy(&szAgentIdxTranData[nTempLen],&szTranUseCode,6);
				memcpy(&szAgentIdxTranData[nTempLen+6],GetSprintf("%d",LocalCardFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+7],&cMediaKind,1);
				memcpy(&szAgentIdxTranData[nTempLen+8],GetSprintf("%d",bCommRecvFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+9],GetSprintf("%d",iTranOkFlag),1);
				memcpy(&szAgentIdxTranData[nTempLen+10],&szErrorCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+14],&szReturnCode,4);
				memcpy(&szAgentIdxTranData[nTempLen+18],&szErrorMsg,40);
				memcpy(&szAgentIdxTranData[nTempLen+58],"0100",4);                 //银行代码
				memcpy(&szAgentIdxTranData[nTempLen+62],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+84],GetSprintf("%-22.22s",szCardAccountNo),22);
				memcpy(&szAgentIdxTranData[nTempLen+106],"0000",4);                  //转入银行代码
				memcpy(&szAgentIdxTranData[nTempLen+110],GetSprintf("%-28.28s",ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount),28);
				memcpy(&szAgentIdxTranData[nTempLen+138],GetSprintf("%016d", atof(ACHostSend.TranMoney)*100),16);
				memcpy(&szAgentIdxTranData[nTempLen+154],"                ",16);    //转账后余额不清，不填
				memcpy(&szAgentIdxTranData[nTempLen+170],GetSprintf("%8.8s",szFee),8);
				memcpy(&szAgentIdxTranData[nTempLen+178],"          ", 10);        //预留
			}
			else
			{	
				//其他交易
				iTranCode = '9';
				nTempLen = 0;
				szTranCode = 'Z';
				memcpy(&szTranUseCode, "000000", 6);
				memcpy(&szAgentIdxTranData[nTempLen],"1",2);
				memcpy(&szAgentIdxTranData[nTempLen+7],GetSprintf("%d",bCommRecvFlag),1);
				return FALSE;
				
			}
			
	}
	
	if (!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR)||          
		m_pDevCmn->JnlStatus == JNL_EMPTY_PAPER)				// 流水运行模式
        JprMode = 2;
	else   
		JprMode = 3;
 
    iLineCnt = 0;                                              // 日志行数 在写IDX时再计算

	szAgentIdxCommonData.Format("%16.16s%8.8s%8.8s%1.1s%1.1s%03d%1.1d%1.1s%10.10s",
		GetSprintf("0100A%3.3sA%3.3sA%3.3s",&m_pProfile->NETWORK.BranchNum[0],&m_pProfile->NETWORK.BranchNum[3],&m_pProfile->NETWORK.BranchNum[6]),
		&m_pProfile->NETWORK.AtmSerialNum[0],
		m_pProfile->TRANS.SerialNo,
		&szTranCode,
		&iTranCode,
		iLineCnt,
		JprMode,
		"0",                                                      //凭条FORM形态  
		"002       ");                                            //预留

    szAgentTranData.Format("%s%s",szAgentIdxCommonData,&szAgentIdxTranData);
    m_pDevCmn->fnJPR_AgentIdxPrintFile(szAgentTranData);
    return TRUE;
}


int CTranCmn::fnAPP_AgentCompareDeviceStatus(int StatusType)
{
	char szCommStatus;
	char szAtmStatus;
	int bAgentMsgSendFlg = 0;

	szCommStatus = '0';
	szAtmStatus = '0';
	if(!m_pDevCmn->fnAPL_CheckHostLine())
		szCommStatus = '8';

	if(m_pDevCmn->AtmStatus == ATM_ERROR)
		szAtmStatus = '2';
	else
	if(m_pDevCmn->AtmStatus == ATM_CUSTOM || m_pDevCmn->AtmStatus == ATM_TRAN)
		szAtmStatus = '0';
	else
		szAtmStatus = '1';

	if(szAtmStatus == '0' && szAgentCheckErrorCode[1] != '0')
		bAgentMsgSendFlg = 2;

	if(szCommStatus == '8' && szAgentCheckErrorCode[0] == '0')
		bAgentMsgSendFlg = 1;
	else
	if(szCommStatus == '0' && szAgentCheckErrorCode[0] == '8')
		bAgentMsgSendFlg = 2;

	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "###szAgentCheckErrorCode[%c],szAtmStatus[%c],bAgentMsgSendFlg[%d]",szAgentCheckErrorCode[1],szAtmStatus,bAgentMsgSendFlg);
	szAgentCheckErrorCode[0] = szCommStatus;
	szAgentCheckErrorCode[1] = szAtmStatus;

	return bAgentMsgSendFlg;
}


// Agent Get Cdu Status 
// return 0:正常 1：钞少 2:空 8:故障
char CTranCmn::fnAPP_GetCstStatus()		// 2008.11.18
{
	int i_m_box1 = 9;
	int i_m_box2 = 9;
	int i_m_box3 = 9;
	int i_m_box4 = 9;
	int i_m_devstatus = 9;

	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_GetCstStatus()");
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FstCSTLoc[%d]",m_pDevCmn->FstCSTLoc);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FstCSTStatus[%d]",m_pDevCmn->FstCSTStatus);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SndCSTLoc[%d]",m_pDevCmn->SndCSTLoc);
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SndCSTStatus[%d]",m_pDevCmn->SndCSTStatus);
	if(m_pDevCmn->CSTCnt >= 1)
	{
		if (m_pDevCmn->FstCSTLoc != NORMAL_POS)
		   i_m_box1 = 9;
		else
		if (m_pDevCmn->FstCSTStatus == CST_NORMAL)
		   i_m_box1 = 0;
		else
		if (m_pDevCmn->FstCSTStatus == CST_NEAR)
		   i_m_box1 = 1;
		else
		if (m_pDevCmn->FstCSTStatus == CST_EMPTY)
		   i_m_box1 = 2;
		else
		if (m_pDevCmn->FstCSTStatus == CST_FULL)
		   i_m_box1 = 0;
		else
		if (m_pDevCmn->FstCSTStatus == CST_ERROR)
			i_m_box1 = 8;
		else
		if (m_pDevCmn->FstCSTStatus == CST_HIGH)
			i_m_box1 = 0;
	}
		
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "i_m_box1[%d]",i_m_box1);

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->FstCSTStatus i_m_box1[%d]",i_m_box1);
	if(m_pDevCmn->CSTCnt >= 2)
	{
		if (m_pDevCmn->SndCSTLoc != NORMAL_POS)
		   i_m_box2 = 9;
		else
		if (m_pDevCmn->SndCSTStatus == CST_NORMAL)
		   i_m_box2 = 0;
		else
		if (m_pDevCmn->SndCSTStatus == CST_NEAR)
		   i_m_box2 = 1;
		else
		if (m_pDevCmn->SndCSTStatus == CST_EMPTY)
		   i_m_box2 = 2;
		else
		if (m_pDevCmn->SndCSTStatus == CST_FULL)
		   i_m_box2 = 0;
		else
		if (m_pDevCmn->SndCSTStatus == CST_ERROR)
			i_m_box2 = 8;
		else
		if (m_pDevCmn->SndCSTStatus == CST_HIGH)
			i_m_box2 = 0;
	}
		
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "i_m_box2[%d]",i_m_box2);

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->SndCSTStatus i_m_box2[%d]",i_m_box2);
	if(m_pDevCmn->CSTCnt >= 3)
	{
		if (m_pDevCmn->TrdCSTLoc != NORMAL_POS)
		   i_m_box3 = 9;
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NORMAL)
		   i_m_box3 = 0;
		else
		if (m_pDevCmn->TrdCSTStatus == CST_NEAR)
		   i_m_box3 = 1;
		else
		if (m_pDevCmn->TrdCSTStatus == CST_EMPTY)
		   i_m_box3 = 2;
		else
		if (m_pDevCmn->TrdCSTStatus == CST_FULL)
		   i_m_box3 = 0;
		else
		if (m_pDevCmn->TrdCSTStatus == CST_ERROR)
			i_m_box3 = 8;
		else
		if (m_pDevCmn->TrdCSTStatus == CST_HIGH)
			i_m_box3 = 0;
	}

	if(m_pDevCmn->CSTCnt >= 4)
	{
		if (m_pDevCmn->FthCSTLoc != NORMAL_POS)
		   i_m_box4 = 9;
		else
		if (m_pDevCmn->FthCSTStatus == CST_NORMAL)
		   i_m_box4 = 0;
		else
		if (m_pDevCmn->FthCSTStatus == CST_NEAR)
		   i_m_box4 = 1;
		else
		if (m_pDevCmn->FthCSTStatus == CST_EMPTY)
		   i_m_box4 = 2;
		else
		if (m_pDevCmn->FthCSTStatus == CST_FULL)
		   i_m_box4 = 0;
		else
		if (m_pDevCmn->FthCSTStatus == CST_ERROR)
			i_m_box4 = 8;
		else
		if (m_pDevCmn->FthCSTStatus == CST_HIGH)
			i_m_box4 = 0;
	}

	if(m_pDevCmn->CSTCnt >= 1)
	{
		i_m_devstatus = i_m_box1;
	}

	if(m_pDevCmn->CSTCnt >= 2)
	{
		if(i_m_box1 > i_m_box2)
			i_m_devstatus = i_m_box2;
		else
			i_m_devstatus = i_m_box1;
	}
	
	if(m_pDevCmn->CSTCnt >= 3)
	{
		i_m_devstatus = i_m_box1;
		if(i_m_box1 > i_m_box2)
			i_m_devstatus = i_m_box2;
		if(i_m_devstatus > i_m_box3)
			i_m_devstatus = i_m_box3;
	}

	if(m_pDevCmn->CSTCnt >= 4)
	{
		i_m_devstatus = i_m_box1;
		if(i_m_box1 > i_m_box2)
			i_m_devstatus = i_m_box2;
		if(i_m_devstatus > i_m_box3)
			i_m_devstatus = i_m_box3;
		if(i_m_devstatus > i_m_box4)
			i_m_devstatus = i_m_box4;
	}
	
	//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "i_m_devstatus[%d]",i_m_devstatus);
	
	if(i_m_devstatus == 2)
	   return '2';
	else
	if(i_m_devstatus == 1)
	   return '1';
	else
	if(i_m_devstatus == 8)
	   return '8';
	else
	if(i_m_devstatus == 9)
	   return '9';
	else
       return '0';

}