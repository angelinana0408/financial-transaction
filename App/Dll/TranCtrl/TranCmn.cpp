     /////////////////////////////////////////////////////////////////////////////
// TranCmn.cpp : implementation file
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
// CTranCmn
/////////////////////////////////////////////////////////////////////////////
CTranCmn::CTranCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CTranCmn():return");
}

CTranCmn::~CTranCmn()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CTranCmn()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CTranCmn():return");
}


BEGIN_MESSAGE_MAP(CTranCmn, CWnd)
	//{{AFX_MSG_MAP(CTranCmn)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// 空闲视频心跳信号 2008.05.09
UINT fnAPL_ADCOMMCommand(LPVOID pParam)
{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ADCOMMCommand()return TRUE()");
	CWinAtmCtrl *pWinAtm = (CWinAtmCtrl*)pParam;
	int ret;
	BYTE    Idlesign[] = {0x24, 0x31, 0x30, 0x85, 0x0d, 0x0a};

	if ( pWinAtm == NULL )
		return FALSE;

	CDevCmn* pDevCmn = pWinAtm->m_pDevCmn;
	while ( TRUE )
	{
		Delay_Msg( 1000 );
		
		//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "pDevCmn->IdleADFlag=%d",pDevCmn->IdleADFlag);
		if ( pDevCmn->IdleADFlag )
		{
			//发送心跳
			EnterCriticalSection(&(pDevCmn->ADMutex));
			ret = pDevCmn->fnAPP_WriteADComm(Idlesign,6);
			//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_WriteADComm=%d,Idlesign",ret);
			LeaveCriticalSection(&(pDevCmn->ADMutex));
		}
	}
}

// Response Atmp Command Thread for Transaction   by zjd
UINT fnAPL_AtmpCommand(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AtmpCommand()return TRUE()");	

	CWinAtmCtrl *pWinAtm = (CWinAtmCtrl*)pParam;

	if ( pWinAtm == NULL )
		return FALSE;

	CDevCmn* pDevCmn = pWinAtm->m_pDevCmn;
	CTranCmn* pTranCmn = pWinAtm->m_pTranCmn;

	CString strRecvData = "";
	char Buffer[1024];
	char SendBuffer[1024];
	int	nRecvResult = 0 ;
	int	NetMessage = 0 ;
	int	nLength = 0 ;
	bool	bRejectFlag;
	char MessageKind[6];
	
	while ( TRUE )
	{

		Delay_Msg( 10 );   //by hyl

		if (pDevCmn->fnNET_GetDeviceStatus3() != LINE_OPEN)
			continue;

		nRecvResult = pDevCmn->fnNET_RecvData3( strRecvData , 30 );
		if ( !nRecvResult )
			continue;

		memset(Buffer, 0, sizeof(Buffer));
		memset(SendBuffer, 0, sizeof(SendBuffer));
		
		MakePack(strRecvData.GetBuffer(0), Buffer , strRecvData.GetLength() ); 
		nLength = pTranCmn->fnAPL_UnpackAtmpCmd( Buffer, MessageKind );
		if ( nLength == 0 )             // 解包过程出现失败  by zjd
			continue;


		if ((pDevCmn->AtmStatus == ATM_TRAN) || (pTranCmn->nAgentOperatorMode == TRUE))
			bRejectFlag = TRUE;
		else
			bRejectFlag = FALSE;
		
//		(memcmp(HMEATMPRecv.Data[3], "200100", 6) == 0)				//TC_ATMOPEN
//		(memcmp(HMEATMPRecv.Data[3], "200200", 6) == 0)				//TC_ATMCLOSE
//		(memcmp(HMEATMPRecv.Data[3], "200300", 6) == 0)				//TC_ATMREBOOT
//		(memcmp(HMEATMPRecv.Data[3], "200400", 6) == 0)				//TC_ATMSTATUS
//		(memcmp(HMEATMPRecv.Data[3], "200500", 6) == 0) 			//TC_ATMRESETALL
//		(memcmp(HMEATMPRecv.Data[3], "200600", 6) == 0)				//TC_ATMBIDADDOWNLOAD  实际上是通知  by zjd
//		(memcmp(HMEATMPRecv.Data[3], "200700", 6) == 0)				//TC_ATMBIDADDOWNLOAD
//		(memcmp(HMEATMPRecv.Data[3], "200800", 6) == 0)				//TC_ATMBIDSENDDETAIL  实际上是通知  by zjd
//		(memcmp(HMEATMPRecv.Data[3], "200900", 6) == 0)				//TC_ATMBIDSENDDETAIL
//		(memcmp(HMEATMPRecv.Data[3], "201000", 6) == 0)				//TC_ATMBIDSOFTUPDATE  实际上是通知  by zjd
//		(memcmp(HMEATMPRecv.Data[3], "201100", 6) == 0) 			//TC_ATMBIDSOFTUPDATE

		if (memcmp(MessageKind, "200100", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  OPEN ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  逻辑开机",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}

			if ((pDevCmn->AtmStatus != ATM_TRAN) && (pTranCmn->InitInfo.BidAtmColseFlag))					// 2006.03.21
			{
				pDevCmn->Timer.HostWaitLinkTime = 0;		// BCTC : 2005.09.01
				pDevCmn->HostOpenRetryCnt = 0;			// Clear
				pDevCmn->Timer.HostOpenRetryTime = 0;		// Clear
				pDevCmn->fnAPL_CloseHost();				// 2005.09.11 : DO NOT MODIFY
				pDevCmn->OpenKey = TRUE;
				pTranCmn->InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11
			}
		}
		else
		if (memcmp(MessageKind, "200200", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  CLOSE ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  逻辑关机",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}

			if (bRejectFlag == FALSE)
			{
				pDevCmn->OpenKey = FALSE;
				pTranCmn->InitInfo.BidAtmColseFlag = TRUE;
			}
		}
		else
		if (memcmp(MessageKind, "200300", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  RESTART ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  热启动",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}
			
			if (bRejectFlag == FALSE)
			{
				pTranCmn->HostNms.RebootFlag = TRUE;
				pDevCmn->fnAPL_SetClerkPowerOff();
			}
		}
		else
		if (memcmp(MessageKind, "200400", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  GET ATM STATUS",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  运行状态检测",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
			}
		}
		else
		if (memcmp(MessageKind, "200500", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM  RESET ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理  强制检测",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}

			if (bRejectFlag == FALSE)
				pTranCmn->HostNms.HostResetAll = TRUE;
		}
		else
		if (memcmp(MessageKind, "200600", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM Download PIC INFO",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 广告画面、公告下发通知",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
			}

			bRejectFlag = FALSE;
			if (pTranCmn->fnAPP_TranATMPAdDownloadNotice() == 1)	//处理失败,不再继续
				continue;
		}
		else
		if (memcmp(MessageKind, "200700", 6) == 0)
		{
			bRejectFlag = FALSE;
			if (pTranCmn->fnAPP_TranATMPAdDownload() == 1)			//处理失败,不再继续
				continue;
		}
		else
		if (memcmp(MessageKind, "200800", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM UPLOAD JOURNAL INFO",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 提取远程流水通知",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
			}

			bRejectFlag = FALSE;
			if (pTranCmn->fnAPP_TranATMPSendDetailNotice() == 1)	//处理失败,不再继续
				bRejectFlag = TRUE;
		}
		else
		if (memcmp(MessageKind, "200900", 6) == 0)
		{
			bRejectFlag = FALSE;
			if (pTranCmn->fnAPP_TranATMPSendDetail() == 1)			//处理失败,不再继续
				continue;
		}
		else
		if (memcmp(MessageKind, "201000", 6) == 0)
		{
			if (pTranCmn->LangMode)
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s ATMP_ADM UPDATE SOFTWARE INFO",
											GetTime().GetBuffer(0),
											&(GetTime().GetBuffer(0)[2]),
											&(GetTime().GetBuffer(0)[4])));
			}
			else
			{
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s 监控管理 软件更新通知",
											GetTime().GetBuffer(0),
											&(GetTime().GetBuffer(0)[2]),
											&(GetTime().GetBuffer(0)[4])));
			}

			bRejectFlag = FALSE;
			if (pTranCmn->fnAPP_TranATMPSoftUpdateNotice() == 1)	//处理失败,不再继续
				continue;
		}
		else
		if (memcmp(MessageKind, "201100", 6) == 0)
		{
			bRejectFlag = FALSE;
			if (pTranCmn->fnAPP_TranATMPSoftUpdate() == 1)			//处理失败,不再继续
				continue;
		}

		nLength = pTranCmn->fnAPL_PackAtmpCmd( SendBuffer, bRejectFlag );
		if ( nLength == 0 )											// 打包过程出现失败  by zjd
			continue;
		
		CString strTmp("");

		strTmp.Format("%2.2s:%2.2s:%2.2s [TCPIP BID SENDDATA], Length : [%d]",
									GetTime().GetBuffer(0),
									&(GetTime().GetBuffer(0)[2]),
									&(GetTime().GetBuffer(0)[4]),
									nLength);
		pDevCmn->fnCMN_SaveLineTrace(strTmp.GetBuffer(0), strTmp.GetLength());
		pDevCmn->fnCMN_SaveLineTrace(SendBuffer, nLength, 1);

		pDevCmn->fnNET_SendData3( MakeUnPack(SendBuffer, nLength) , 10 );
        

	}
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AtmpCommand()return TRUE");
	return TRUE;
}

// Response Agent Command Thread for Transaction   by hyl
UINT fnAPL_AgentCommand(LPVOID pParam)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentCommand()return TRUE()");	

	CWinAtmCtrl *pWinAtm = (CWinAtmCtrl*)pParam;
	if ( pWinAtm == NULL )
		return FALSE;

	int iShrRetry;
	iShrRetry = IniGetInt(_AGENT_ATMS,"COMMON","SHRRETRY",1);
	
	CDevCmn* pDevCmn = pWinAtm->m_pDevCmn;
	CTranCmn* pTranCmn = pWinAtm->m_pTranCmn;
	
	pTranCmn->fnAPP_AgentLoadLibrary();
	pTranCmn->bAgentRebootFlag = FALSE;
	pTranCmn->nAgentBidAtmColseFlag = FALSE;

	while ( TRUE )
	{
		memset(pTranCmn->szAtmRcvBuff,0,10014);
		memset(pTranCmn->szAtmSndBuff,0,1014);
		memset(&pTranCmn->szAtmStatus,0,210);
		Delay_Msg( iShrRetry*1000 );  
		
        if(pTranCmn->bAgentRebootFlag == TRUE && pDevCmn->AtmStatus != ATM_INIT && pDevCmn->AtmStatus != ATM_CLERK && pDevCmn->AtmStatus != ATM_TRAN )
		{
			pTranCmn->bAgentRebootFlag = FALSE;
 			pTranCmn->HostNms.RebootFlag = TRUE;
			pDevCmn->fnAPL_SetClerkPowerOff();
		}		
		
	   if (pTranCmn->pGetSharedMessage("AGT2ATM_SHAREMESSAGE", pTranCmn->szAtmRcvBuff, 1014) == (UINT)0)
	   {        
		    //共享内存数据不存在									         
			DWORD dwErr = GetLastError();
			if (dwErr == ERROR_SUCCESS)
			{
			//共享内存读取成功及没有数据
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentCommand()ERROR_SUCCESS");
				continue;		 
			}												 
			else
			{
			//共享内存数据读取失败(内存未生成及其他事由)
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentCommand()ERROR_FAIL");
				continue;
			}
	   }
	   else
	   {    

		    CTranCmn::LPSndShareMessage pRcvSM = (CTranCmn::LPSndShareMessage)&pTranCmn->szAtmRcvBuff[0];
	        CTranCmn::LPSndShareMessage pSndSM = (CTranCmn::LPSndShareMessage)&pTranCmn->szAtmSndBuff[0]; 

//			pTranCmn->fnAPP_TranAgentCmd();
///////////////////////////////////////////////////////////////////////////////////////////////////////////
			//命令处理
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd()");
	
			bool bAgentRejectFlag;
 			bool bAgentSndFlag = FALSE;
			char sAgentFileType;
			char sAgentFileDate[8];
			CString sAgentFilePath;
			char sAgentReturn;
			char sAgentReturnCDU;
			char sAgentReturnMCU;
			char sAgentReturnSPR;
			char sAgentReturnJPR;
			char sAgentReturnPBM;
            pTranCmn->bAgentRebootFlag = FALSE;

		   //共享内存数据读取成功
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd()szAtmRcvBuff[%s]",pTranCmn->szAtmRcvBuff);           
		   memset(pTranCmn->szAtmStatus.sMTCCode,' ',10);	
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "pTranCmn->nAgentOperatorMode[%d]",pTranCmn->nAgentOperatorMode);           

			if ((pDevCmn->AtmStatus == ATM_TRAN) || pTranCmn->nAgentOperatorMode == TRUE)
			{
				bAgentRejectFlag = TRUE;
				if(pDevCmn->AtmStatus == ATM_TRAN)
				{
					sAgentReturn = AGENT_INTRAN;
				}
				else
//				if(pDevCmn->AtmStatus == ATM_CLERK)
                if(pTranCmn->nAgentOperatorMode == TRUE)
				{
					sAgentReturn = AGENT_CLERK;
				}
			}
			else
			if(pTranCmn->InitInfo.BidAtmColseFlag)
			{
// 				sAgentReturn = AGENT_PAUSE;
				bAgentRejectFlag = FALSE;               //在暂停模式下允许执行管理命令
			}
			else
			{
				bAgentRejectFlag = FALSE;
			
			}

			//取款机命令处理开始//取款机命令处理结束// 给AGENT设定处理结果
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "1030", 4) == 0 )  //ATM STATUS
			{	
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  GET ATM STATUS",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

				if (!pDevCmn->fnAPL_CheckError() || (pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN)))
				{
					memcpy(pTranCmn->szAtmStatus.sMTCCode,GetSprintf("%-10.10s",GetSprintf("%7.7s",pDevCmn->CurErrBuff.ErrorCode)),10);
				   if(pDevCmn->fnAPL_GetDownErrorDevice(DEV_MAIN))
				   {
						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))				// CDU Status
							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","CDU ERROR"),9);	
//						else
//						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_BRM))
//							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","BRM ERROR"),9);	
						else
						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU) ||
							(!pDevCmn->fnAPL_GetDefineDevice(DEV_MCU)))				// Mcu Status
							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","MCU ERROR"),9);	
						else
						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR) ||
							(!pDevCmn->fnAPL_GetDefineDevice(DEV_JPR)))				// JPR Status
							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","JPR ERROR"),9);	
						else
						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR) ||
							(!pDevCmn->fnAPL_GetDefineDevice(DEV_SPR)))				// SPR Status
							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","SPR ERROR"),9);	
						else
						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN) ||
							(!pDevCmn->fnAPL_GetDefineDevice(DEV_PIN)))				// PIN Status
							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","PIN ERROR"),9);	
						else
						if (pDevCmn->fnAPL_GetDownErrorDevice(DEV_PBM))				// PBM Status
							memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-9.9s","PBM ERROR"),9);	
				   }
				   else
					   memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%s",GetSprintf("%s",pDevCmn->CurErrBuff.ErrorString)),GetSprintf("%s",pDevCmn->CurErrBuff.ErrorString).GetLength());	
				}
				else
				{
					memcpy(pTranCmn->szAtmStatus.sMTCCode,"0000000000",10);
					if(pDevCmn->AtmStatus == 0)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_INIT"),30);
					else
					if(pDevCmn->AtmStatus == 1)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_ERROR"),30);
					else
					if(pDevCmn->AtmStatus == 2)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_CLERK"),30);
					else
					if(pDevCmn->AtmStatus == 3)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_READY"),30);
					else
					if(pDevCmn->AtmStatus == 4)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_CUSTOM"),30);
					else
					if(pDevCmn->AtmStatus == 5)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_TRAN"),30);
					else
					if(pDevCmn->AtmStatus == 6)
						 memcpy(pTranCmn->szAtmStatus.sStatusDesc,GetSprintf("%-30.30s","AtmStatus: ATM_DOWN"),30);

				}
 				bAgentSndFlag = TRUE; 
				sAgentReturn = AGENT_SUCCCESS;
				memcpy(pSndSM->strData,pTranCmn->szAtmStatus.sMTCCode,10);
				memcpy(&pSndSM->strData[10],pTranCmn->szAtmStatus.sStatusDesc,200);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd()pTranCmn->szAtmStatus.sMTCCode[%s]",pTranCmn->szAtmStatus.sMTCCode);
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd()pTranCmn->szAtmStatus.sStatusDesc[%s]",pTranCmn->szAtmStatus.sStatusDesc);


			} 
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "1040", 4) == 0 )  //
			{	
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  GET ATM REMAIN",
												GetTime().GetBuffer(0),
												&(GetTime().GetBuffer(0)[2]),
												&(GetTime().GetBuffer(0)[4])));
        
				memcpy(pTranCmn->szRemainMedia.card,Int2Asc(pDevCmn->fnMCU_GetMaterialInfo()),1);
				memcpy(pTranCmn->szRemainMedia.cash,Int2Asc(pDevCmn->fnCDU_GetMaterialInfo()),1);
				memcpy(pTranCmn->szRemainMedia.reciept,Int2Asc(pDevCmn->fnSPR_GetMaterialInfo()),1);
 				memcpy(pTranCmn->szRemainMedia.pb,Int2Asc(pDevCmn->fnPBM_GetMaterialInfo()),1);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnMCU_GetMaterialInfo[%d]",pDevCmn->fnMCU_GetMaterialInfo());

                if(pDevCmn->fnMCU_GetMaterialInfo()!=0)      //不为0则为残留
					memcpy(pTranCmn->szRemainMedia.card,"1",1);
				if(pDevCmn->fnCDU_GetMaterialInfo()!=0)
					memcpy(pTranCmn->szRemainMedia.cash,"1",1);
				if(pDevCmn->fnSPR_GetMaterialInfo()!=0)
					memcpy(pTranCmn->szRemainMedia.reciept,"1",1);
				if(pDevCmn->fnPBM_GetMaterialInfo()!=0)
					memcpy(pTranCmn->szRemainMedia.pb,"1",1);

				if(!pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))
				{
					memcpy(pTranCmn->szRemainMedia.card,"2",1);
				}
				if(!pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))
				{
					memcpy(pTranCmn->szRemainMedia.cash,"2",1);
				}
				if(!pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))
				{
					memcpy(pTranCmn->szRemainMedia.reciept,"2",1);
				}
				if(!pDevCmn->fnAPL_GetDefineDevice(DEV_PBM))
				{
					memcpy(pTranCmn->szRemainMedia.pb,"2",1);
				}
				

 				bAgentSndFlag = TRUE; 
				sAgentReturn = AGENT_SUCCCESS;
				memcpy(pSndSM->strData,&pTranCmn->szRemainMedia,4);
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "2010", 4) == 0 )
			{	
				if (bAgentRejectFlag == FALSE)
				{
					pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  POWEROFF ATM",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));

					bAgentSndFlag = FALSE;
					pDevCmn->fnAPL_SetClerkPowerOff();
				}
				else
 				if(pDevCmn->AtmStatus == ATM_TRAN)
				{
					bAgentSndFlag = TRUE;
					sAgentReturn = AGENT_INTRAN;
				}
				else
				if(pTranCmn->nAgentOperatorMode == TRUE)
				{
					bAgentSndFlag = TRUE;
					sAgentReturn = AGENT_CLERK;
				}
				
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "2020", 4) == 0 )
			{	
				if (bAgentRejectFlag == FALSE)
				{
					pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESTART ATM",
															GetTime().GetBuffer(0),
															&(GetTime().GetBuffer(0)[2]),
															&(GetTime().GetBuffer(0)[4])));
					
					bAgentSndFlag = FALSE;
					pTranCmn->HostNms.RebootFlag = TRUE;
                    pDevCmn->fnAPL_SetClerkPowerOff();

				}
				else
 				if(pDevCmn->AtmStatus == ATM_TRAN)
				{
					bAgentSndFlag = TRUE;
					sAgentReturn = AGENT_INTRAN;
				}
				else
				if(pTranCmn->nAgentOperatorMode == TRUE)
				{
					bAgentSndFlag = TRUE;
					sAgentReturn = AGENT_CLERK;
				}
				
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "2030", 4) == 0 )
			{	
				
				if ((pDevCmn->AtmStatus != ATM_TRAN) && (pTranCmn->InitInfo.BidAtmColseFlag))					// 2006.03.21
				{
					pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  OPEN ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
					pDevCmn->Timer.HostWaitLinkTime = 0;		// BCTC : 2005.09.01
					pDevCmn->HostOpenRetryCnt = 0;		    	// Clear
					pDevCmn->Timer.HostOpenRetryTime = 0;		// Clear
					pDevCmn->fnAPL_CloseHost();				    // 2005.09.11 : DO NOT MODIFY
					pDevCmn->OpenKey = TRUE;
					pTranCmn->InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11

					sAgentReturn = AGENT_SUCCCESS;
				}
				else 
				if(pTranCmn->InitInfo.BidAtmColseFlag == FALSE)
				{
					sAgentReturn = AGENT_FAIL;
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "BidAtmColseFlag[%d]",pTranCmn->InitInfo.BidAtmColseFlag);
 				}	

 				bAgentSndFlag = TRUE; 
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "2040", 4) == 0 )
			{				
				if (bAgentRejectFlag == FALSE)
				{
					pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  CLOSE ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));

					if(pTranCmn->InitInfo.BidAtmColseFlag)
					{
						sAgentReturn = AGENT_PAUSE;
					}
					else
					{
						pDevCmn->OpenKey = FALSE;
						pTranCmn->InitInfo.BidAtmColseFlag = TRUE;
						sAgentReturn = AGENT_SUCCCESS;
					}
				

				} 
  				bAgentSndFlag = TRUE; 
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "2050", 4) == 0 )
			{			
				//获取日志运行模式  
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  SJRNLMODE",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

				memcpy(pSndSM->strData,GetSprintf("%d",IniGetInt(_AGENT_ATMS,_AGENT_ATM_AP,"JRNLOPERMODE",3)),1);
     
				sAgentReturn = AGENT_SUCCCESS;
 				bAgentSndFlag = TRUE; 
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "3010", 4) == 0 )
			{
				pTranCmn->HostNms.ModuleResetFlag = 0;
				
				memcpy(&pTranCmn->szDevRest,pRcvSM->strData,6);
				
				if(bAgentRejectFlag == FALSE)
				{
					if(memcmp(&pRcvSM->strData[0],"1",1) == 0)
					{
						pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESET ALL",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
						memset(&pRcvSM->strData[1],'1',5);
					}

					//所有设备有一个成功就算成功，全部失败才失败
					if(memcmp(&pRcvSM->strData[1],"1",1) == 0)
					{
						pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESET MCU",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

						if (!pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))
						{
							memcpy(pTranCmn->szDevRest.dev_card,"3",1);
							sAgentReturnCDU = AGENT_FAIL;
						}
						else
						{
							pTranCmn->bAgentRestResultFlag = 2;
                            pTranCmn->HostNms.ModuleResetFlag = 1;
							if(pDevCmn->fnAPL_AgentRestWaitResult())
							{
								memcpy(pTranCmn->szDevRest.dev_card,"1",1);
								sAgentReturnCDU = AGENT_SUCCCESS; 
							}
							else
							{
								memcpy(pTranCmn->szDevRest.dev_card,"2",1);
								sAgentReturnCDU = AGENT_FAIL; 
							}
						}
						
					}
					
					if(memcmp(&pRcvSM->strData[2],"1",1) == 0)
					{
						pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESET CDU",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

						if (!pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))
						{
							memcpy(pTranCmn->szDevRest.dev_cash,"3",1);
							sAgentReturnMCU = AGENT_FAIL;
						}
						else
						{
							pTranCmn->bAgentRestResultFlag = 2;
                            pTranCmn->HostNms.ModuleResetFlag = 2;
							if(pDevCmn->fnAPL_AgentRestWaitResult())
							{
								memcpy(pTranCmn->szDevRest.dev_cash,"1",1);
								sAgentReturnMCU = AGENT_SUCCCESS;
							}
							else
							{
								memcpy(pTranCmn->szDevRest.dev_cash,"2",1);
								sAgentReturnMCU = AGENT_FAIL;
							}
						}
						
					}
					
					if(memcmp(&pRcvSM->strData[3],"1",1) == 0)
					{
						pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESET SPR",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

						if (!pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))
						{
							memcpy(pTranCmn->szDevRest.dev_reciept,"3",1);
							sAgentReturnSPR = AGENT_FAIL;
						}
						else
						{
							pTranCmn->bAgentRestResultFlag = 2;
                            pTranCmn->HostNms.ModuleResetFlag = 3;
							if(pDevCmn->fnAPL_AgentRestWaitResult())
							{
								memcpy(pTranCmn->szDevRest.dev_reciept,"1",1);
								sAgentReturnSPR = AGENT_SUCCCESS;
							}
							else
							{
								memcpy(pTranCmn->szDevRest.dev_reciept,"2",1);
								sAgentReturnSPR = AGENT_FAIL;
							}
						}
					}
				
					if(memcmp(&pRcvSM->strData[4],"1",1) == 0)
					{
						pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESET JPR",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

						if (!pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))
						{
							memcpy(pTranCmn->szDevRest.dev_journal,"3",1);
							sAgentReturnJPR = AGENT_FAIL;
						}
						else
						{
							pTranCmn->bAgentRestResultFlag = 2;
                            pTranCmn->HostNms.ModuleResetFlag = 4;
							if(pDevCmn->fnAPL_AgentRestWaitResult())
							{
								memcpy(pTranCmn->szDevRest.dev_journal,"1",1);
								sAgentReturnJPR = AGENT_SUCCCESS;
							}
							else
							{
								memcpy(pTranCmn->szDevRest.dev_journal,"2",1);
								sAgentReturnJPR = AGENT_FAIL;
							}
						}
					}
					
					if(memcmp(&pRcvSM->strData[5],"1",1) == 0)
					{
						pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESET PBM",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));

						if (!pDevCmn->fnAPL_GetDefineDevice(DEV_PBM))
						{
							memcpy(pTranCmn->szDevRest.dev_pb,"3",1);
							sAgentReturnPBM = AGENT_FAIL;
						}
						else
						{
							pTranCmn->bAgentRestResultFlag = 2;
                            pTranCmn->HostNms.ModuleResetFlag = 5;
							if(pDevCmn->fnAPL_AgentRestWaitResult())
							{
								memcpy(pTranCmn->szDevRest.dev_pb,"1",1);
								sAgentReturnPBM = AGENT_SUCCCESS;
							}
							else
							{
								memcpy(pTranCmn->szDevRest.dev_pb,"2",1);
								sAgentReturnPBM = AGENT_FAIL;
							}
						}

 					}
            		sAgentReturn = AGENT_SUCCCESS;
				}
 				bAgentSndFlag = TRUE; 
				memcpy(&pSndSM->strData,&pTranCmn->szDevRest,6);
				
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "8010", 4) == 0 )
			{	
				CString    sFilePathName;
				FILETIME   filecreattime;   
				FILETIME   fileaccesstime;   
				FILETIME   filewritetime;  
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  SEND LOG",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));

				memcpy(&sAgentFileType,pRcvSM->strData,1);
				memcpy(&sAgentFileDate,&pRcvSM->strData[1],8);
				
				sFilePathName.Format("%s%02d%02d%s","D:\\trace\\Log",Asc2Int(&pRcvSM->strData[5],2),Asc2Int(&pRcvSM->strData[7],2),".Txt");			   // modify by yusy 2008.11.18
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd()sFilePathName[%s]",sFilePathName);

				HANDLE hFile = CreateFile (sFilePathName, GENERIC_READ,
								FILE_SHARE_READ|FILE_SHARE_DELETE,
								NULL, OPEN_EXISTING,
								FILE_FLAG_BACKUP_SEMANTICS, NULL);	 
				
				GetFileTime(hFile,&filecreattime,&fileaccesstime,&filewritetime);
                CloseHandle(hFile); 
				CTime  tmcreat(filecreattime);

   				int bFileExist = memcmp(&pRcvSM->strData[1],GetSprintf("%4d%02d%02d",tmcreat.GetYear(),tmcreat.GetMonth(),tmcreat.GetDay()),8);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd()FileCreateTime[%s]",GetSprintf("%4d%02d%02d%",tmcreat.GetYear(),tmcreat.GetMonth(),tmcreat.GetDay()));

			   if(bFileExist == 0)
			   {
					sAgentFilePath.Format("%s%02d%02d%s","D:\\Trace\\Log",Asc2Int(&pRcvSM->strData[5],2),Asc2Int(&pRcvSM->strData[7],2),".Txt");
                    memcpy(pSndSM->strData,pRcvSM->strData,1);
					memcpy(&pSndSM->strData[1],sAgentFilePath,sAgentFilePath.GetLength());
				}
				else
					sAgentReturn = 'B';

				bAgentSndFlag = TRUE; 

				
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "9010", 4) == 0 )
			{					

				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  DOWNLOAD READY",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4])));
				sAgentReturn = AGENT_SUCCCESS;
			}
			else
			if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && memcmp(pRcvSM->strTrID, "9020", 4) == 0 )
			{			 
				pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s AGENT_ADM  RESTART ATM",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
 				pDevCmn->OpenKey = FALSE;
 				pTranCmn->InitInfo.BidAtmColseFlag = TRUE;  

                pTranCmn->bAgentRebootFlag = TRUE;
                bAgentSndFlag = TRUE;    //下载重启成功失败都给应答
										
			}
			else
			if(pRcvSM->strTxID != "")
			{
				bAgentSndFlag = TRUE;
				sAgentReturn = AGENT_NOSUPPORT;
			}

			if(bAgentSndFlag == TRUE)
			{
				memcpy(&pSndSM->strTxID,&pRcvSM->strTxID,4);
				memcpy(&pSndSM->strTrID,&pRcvSM->strTrID,4);
				memcpy(&pSndSM->strResp,&sAgentReturn,1);
				memcpy(&pSndSM->strMsgLen,GetSprintf("%-5.5s",GetSprintf("%d",GetSprintf("%s",pSndSM->strData).GetLength())),5);
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd() strMsgLen[%5.5s]",pSndSM->strMsgLen);				
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd() strTxID[%4.4s]",pRcvSM->strTxID);				
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd() strTrID[%4.4s]",pRcvSM->strTrID);				
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd() strData[%s]",pSndSM->strData);				
//		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd() pSndSM[%s]",pSndSM);		
				

///////////////////////////////////////////////////////////////////////////////////////////////////////////
				if (pTranCmn->pPutSharedMessage("ATM2AGT_SHAREMESSAGE", pTranCmn->szAtmSndBuff, 10014))				
				{	
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentCommand():szAtmSndBuff[%s]",pTranCmn->szAtmSndBuff);				
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentCommand():Data Wite Success");				
				}												
				else												
				{
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentCommand():Data Wite Fail");
				}	
                if(memcmp(pRcvSM->strTxID, "0600", 4) == 0 && 
					memcmp(pRcvSM->strTrID, "2040", 4) == 0 && 
					sAgentReturn == AGENT_SUCCCESS)
//                   pTranCmn->fnAPP_AgentIdxGetData(CLERK_OTHER);         //暂停服务后发一个运行日志
                     pTranCmn->nAgentBidAtmColseFlag = TRUE;
				
			}
			

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_TranAgentCmd() return Success");
									   
	   }
	   	   
       if(pTranCmn->pDataInitSharedMessage("AGT2ATM_SHAREMESSAGE"))
	   {
		   MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "pDataInitSharedMessage Success");

	   }
	   else
	   {
		   MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "pDataInitSharedMessage Fail");

	   }
		
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTranCmn message handlers
/////////////////////////////////////////////////////////////////////////////
// Set Owner
void CTranCmn::SetOwner(CWinAtmCtrl *pOwner)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner()");

#include "..\..\H\Common\ClassSetOwner.h"

/////////////////////////////////////////////////////////////////////////////
//	Version & Operation Information Setting
/////////////////////////////////////////////////////////////////////////////
	m_pProfile->GetTransProfile();
	Strcpy(m_pProfile->TRANS.ApVersion, VERSION, 9);			// Version Register : VNN-NN-NN(9 Move)
	m_pProfile->PutTransProfile();

//	m_pDevCmn->HostLineMode		= HOST_LOCAL_MODE;				// Oline/Local
	m_pDevCmn->HostLineMode		= HOST_ONLINE_MODE;				// Oline/Local
	m_pDevCmn->CashCycleMode	= CSH_RECYCLE_MODE;				// Recycle/Uncycle

	if (IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, "TESTCASHMODE") == "fake")
		m_pDevCmn->CashCashMode		= CSH_TESTCASH_MODE;		// RealCash/TestCash
	else
		m_pDevCmn->CashCashMode		= CSH_REALCASH_MODE;		// RealCash/TestCash
																// 2009.03.25 通过配置文件判断
//	if ((m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)   ||
//		(m_pDevCmn->CashCashMode == CSH_TESTCASH_MODE))
// 		iDEBUGFLAG = 1;
//	else
		iDEBUGFLAG = 0;

	char szCduStatus;   //20080321 
	char szSprStatus;
	char szJprStatus;
	char szMcuStatus;
	char szPbStatus;
	char szPinStatus;
	char szDvrStatus;
	char szBrmStatus;
	CString cDvrstatus("");

	if (m_pProfile->NETWORK.InterfaceKind == 20)				// Cup(SHGD) :2006.09.22
		m_pDevCmn->fnAPL_SetAutoOffBankDevice(DEV_SPR | DEV_JPR | DEV_CSH);
	else
	if (m_pProfile->NETWORK.InterfaceKind == 17)				// Cup(BJHX) :2006.05.20
		m_pDevCmn->fnAPL_SetAutoOffBankDevice(DEV_SPR | DEV_JPR);
	else
        m_pDevCmn->fnAPL_SetAutoOffBankDevice(DEV_SPR | DEV_CSH);		// 2007.07.08 定义哪些模块故障还可以进服务   by zjd
////////////////////////////////////////////////////////////////// 第一次开机初始化批次号   by zjd
	if (IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0000") == "0000")
		IniSetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001");

/////////////////////////////////////////////////////////////////////////////
//	Builder Information Variable
/////////////////////////////////////////////////////////////////////////////
	m_nScrNum = 0;												// Screen Number : 2004.11.05
	m_nRetCode = RET_ABNORMAL_STEP;								// Return Code
	setAtmStatus = 1;
/////////////////////////////////////////////////////////////////////////////
//	Ftp Information Variable
/////////////////////////////////////////////////////////////////////////////
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPUSERNAME,	"user");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPPASSWORD,	"pass");
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_FTPDIRNAME,	"/ATMClient/");
	
/////////////////////////////////////////////////////////////////////////////
//	Initialize Information Setting
/////////////////////////////////////////////////////////////////////////////需要处理下载的控制参数中的相关变量，从INI中读取   by zjd
	memset(&CwcInfo, 0, sizeof(CwcInfo));		// added by liuxl 20110714
// 	CwcInfo.CwcFlag = 0;
// 	CwcInfo.ICCwcDataLen = 0;
// 	CwcInfo.TranExpInfoLen = 0;
// 	CwcInfo.CwcRetryCnt = 0;
// 	CwcInfo.TranAccountLen = 0;
	memset(&CardRetainData, 0, sizeof(CardRetainData));

	memset(&InitInfo, 0, sizeof(InitInfo));
	InitInfo.WithMaxAmt = 0;
	InitInfo.WithMinAmt = 0;
	InitInfo.TransferMaxAmt = 0;
	
	InitInfo.UserWithAvail = TRUE;
	InitInfo.UserInquiryAvail = TRUE;
	InitInfo.UserDetailAvail = TRUE;
	InitInfo.UserTransAvail = TRUE;
	InitInfo.UserChangePinAvail = TRUE;
	InitInfo.UserDepAvail = TRUE;
	InitInfo.UserPbmAvail = TRUE;

	InitInfo.CwcRetryCnt = CWC_RETRY_COUNT;
	InitInfo.KeyInTimeOut = KEYIN_TIME_OUT;    //无操作等待时间
	InitInfo.TakeSlipTimeOut = TAKE_SLIP_TIME_OUT;
 
	if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT) == 0)
       InitInfo.TakeCardTimeOut = TAKE_CARD_TIME_OUT;
    else
	   InitInfo.TakeCardTimeOut = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT, TAKE_CARD_TIME_OUT);

	if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT)==0)
       InitInfo.TakeCashTimeOut = TAKE_CASH_TIME_OUT;
	else
	   InitInfo.TakeCashTimeOut = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT, TAKE_CASH_TIME_OUT);
	
	InitInfo.LineSendTime = LINE_SEND_TIME;
	InitInfo.LineRecvTime = LINE_RECV_TIME;
	InitInfo.OpenRetryTime = LINE_RETRY_TIME;
	InitInfo.LineRetryTime = 30;
	InitInfo.CwcRetryTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWCRETRYTIME, 0);
	InitInfo.TransRetryTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANRETRYTIME, 0);
	InitInfo.MsrSendTime = LINE_MSR_SEND_TIME;

	InitInfo.BidAtmColseFlag = FALSE;
	InitInfo.TotalDisplayNo = 0;
	
	if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT) == 0)
		InitInfo.LineSendTime = LINE_SEND_TIME;
	else
	    InitInfo.LineSendTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT, LINE_SEND_TIME);
	if (InitInfo.LineSendTime < 60)
		InitInfo.LineSendTime = 60;
	else
	if (InitInfo.LineSendTime > 120)
		InitInfo.LineSendTime = 120;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT, InitInfo.LineRecvTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_SENDTIMEOUT[%d]", InitInfo.LineRecvTime);

    if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_RECVTIMEOUT) == 0)
	    InitInfo.LineRecvTime = LINE_RECV_TIME;
	else
	    InitInfo.LineRecvTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_RECVTIMEOUT, LINE_RECV_TIME);
	if (InitInfo.LineRecvTime < 60)
		InitInfo.LineRecvTime = 60;
	else
	if (InitInfo.LineRecvTime > 120)
		InitInfo.LineRecvTime = 120;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_RECVTIMEOUT, InitInfo.LineRecvTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_RECVTIMEOUT[%d]", InitInfo.LineRecvTime);

	if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT) == 0)
	    InitInfo.LineRecvTime = LINE_RECV_TIME;
	else
	    InitInfo.LineRecvTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT, LINE_RECV_TIME);
	if (InitInfo.LineRecvTime < 60)
	    InitInfo.LineRecvTime = 60;
	else
	if (InitInfo.LineRecvTime > 120)
	    InitInfo.LineRecvTime = 120;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SENDTIMEOUT, InitInfo.LineRecvTime);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_SENDTIMEOUT[%d]", InitInfo.LineRecvTime);
    

	InitInfo.OpenRetryTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_OPENRETRYTIME, 60);
	if (InitInfo.OpenRetryTime < 60)
		InitInfo.OpenRetryTime = 60;
	else
	if (InitInfo.OpenRetryTime > 3600)
		InitInfo.OpenRetryTime = 3600;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_OPENRETRYTIME, InitInfo.OpenRetryTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_OPENRETRYTIME[%d]", InitInfo.OpenRetryTime);

	InitInfo.LineRetryTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_LINERETRYTIME, 30);
	if (InitInfo.LineRetryTime < 30)
		InitInfo.LineRetryTime = 30;							// 2005.08.22 : DO NOT MODIFY
	else
	if (InitInfo.LineRetryTime > 300)
		InitInfo.LineRetryTime = 300;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_LINERETRYTIME, InitInfo.LineRetryTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_LINERETRYTIME[%d]", InitInfo.LineRetryTime);

	if (InitInfo.TransRetryTime <= 0)
		InitInfo.TransRetryTime = 3;
	else
	if (InitInfo.TransRetryTime > 10)
		InitInfo.TransRetryTime = 10;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANRETRYTIME, InitInfo.TransRetryTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_TRANRETRYTIME[%d]", InitInfo.TransRetryTime);

    if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWCRETRYTIME) == 0 )
	    InitInfo.CwcRetryTime = 3;
    else
        InitInfo.CwcRetryTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWCRETRYTIME, 3);
	if (InitInfo.CwcRetryTime <= 0)
		InitInfo.CwcRetryTime = 3;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWCRETRYTIME, InitInfo.CwcRetryTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_CWCRETRYTIME[%d]", InitInfo.CwcRetryTime);

    if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME) == 0 )
	    InitInfo.MsrSendTime = 180;
	else
	    InitInfo.MsrSendTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME, 180);
	if (InitInfo.MsrSendTime < 60)								// 2005.12.09
		InitInfo.MsrSendTime = 60;
	else
	if (InitInfo.MsrSendTime > 600)
		InitInfo.MsrSendTime = 600;
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME, InitInfo.MsrSendTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_IDLERETRYTIME[%d]", InitInfo.MsrSendTime);

	// added by yusy 08.09.09
	/***/
    if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_AGENTRETRYTIME) == 0 )
	    InitInfo.AgentMsrSendTime = 3600;
	else
	    InitInfo.AgentMsrSendTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_AGENTRETRYTIME, 3600);

	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_AGENTRETRYTIME, InitInfo.AgentMsrSendTime);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_AGENTRETRYTIME[%d]", InitInfo.AgentMsrSendTime);
	/***/
    
	CString	strTemp("");
	strTemp = IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSIP);
	if (strTemp.GetLength() >= 15)
		memcpy(InitInfo.NmsIp, strTemp, 15);
	else
		memcpy(InitInfo.NmsIp, m_pProfile->NETWORK.BpIpAddress, 15);
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSIP, GetSprintf("%15.15s", InitInfo.NmsIp));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_NMSIP[%15.15s]", InitInfo.NmsIp);

	strTemp = IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSPORT);
	if (strTemp.GetLength() <= 5)
		InitInfo.NmsPort = Asc2Int(strTemp);
	else
		InitInfo.NmsPort = Asc2Int(m_pProfile->NETWORK.BpPortNo);
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_NMSPORT, GetSprintf("%d", InitInfo.NmsPort));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() _INIKEY_TIMEDEFINE_NMSPORT[%d]", InitInfo.NmsPort);

//	InitInfo.KeyMode = RegGetInt(_REGKEY_PINPADSTATUS, _REGKEY_PINPADSTATUS_KEYMODE);
	InitInfo.KeyMode = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "DESMODE", 0);
	if (InitInfo.KeyMode == 0)									// 2006.03.21
	{
		InitInfo.KeyMode = IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "DESMODE", 0);
		InitInfo.KeyMode = TRIPLE_DES;
	}
	else
		InitInfo.KeyMode = SINGLE_DES;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() DESMODE[%d]", InitInfo.KeyMode);


//本地参数设置 by hyl 
	if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_KEYINTIMEOUT) == 0)
        InitInfo.KeyInTimeOut = 30;
	else
        InitInfo.KeyInTimeOut = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_KEYINTIMEOUT,30);
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_KEYINTIMEOUT,InitInfo.KeyInTimeOut);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() KeyInTimeOut[%d]", InitInfo.KeyInTimeOut);
    
    if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DETAILNUM) == 0) 
	    InitInfo.DetailNum = 10;
	else
        InitInfo.DetailNum = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DETAILNUM,10);
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DETAILNUM,InitInfo.DetailNum);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() DetailNum[%d]", InitInfo.DetailNum);

    if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ERRORNOTICETIME) == 0)
        InitInfo.ErrorNoticeTime = 3 ;
	else
        InitInfo.ErrorNoticeTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ERRORNOTICETIME,3);
	IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ERRORNOTICETIME,InitInfo.ErrorNoticeTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() ErrorNoticeTime[%d]", InitInfo.ErrorNoticeTime);
    
    InitInfo.TranMaxRemittance = Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXREMITTANCE,"50000"));
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXREMITTANCE, GetSprintf("%.0f", InitInfo.TranMaxRemittance));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() TranMaxRemittance[%s]", GetSprintf("%.2f", InitInfo.TranMaxRemittance));

    InitInfo.TranMinRemittance = Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINREMITTANCE,"50")) ;
    IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINREMITTANCE, GetSprintf("%.0f", InitInfo.TranMinRemittance));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() TranMinRemittance[%s]", GetSprintf("%.2f",InitInfo.TranMinRemittance));

//A/C账户管家银行柜员号
	strTemp = IniGetStr(_NETWORK_INI, "NETWORK", "ACCounterNumber", m_pProfile->NETWORK.AtmSerialNum);
	memcpy(InitInfo.ACCounterNumber, strTemp, strTemp.GetLength());
    IniSetStr(_NETWORK_INI, "NETWORK", "ACCounterNumber", InitInfo.ACCounterNumber);
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() ACCounterNumber[%s]", InitInfo.ACCounterNumber);
 //20090803 by yaokq for 绿卡通
	InitInfo.ZztohMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_ZZTOHMAXNUM,"50000"));
	InitInfo.ZztohMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_ZZTOHMINNUM,"50"));
	InitInfo.DhtohMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_DHTOHMAXNUM,"50000"));
	InitInfo.DhtohMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_DHTOHMINNUM,"50"));
	InitInfo.TzcktohMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_TZCKTOHMAXNUM,"50000"));
	InitInfo.TzcktohMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_TZCKTOHMINNUM,"50"));
	InitInfo.HtodhMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_HTODHMAXNUM,"50000"));
	InitInfo.HtodhMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_HTODHMINNUM,"50"));
	InitInfo.HtotzcqMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_HTOTZCQMAXNUM,"50000"));
	InitInfo.HtotzcqMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_HTOTZCQMINNUM,"50"));
	InitInfo.HtozzMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_HTOZZMAXNUM,"50000"));
	InitInfo.HtozzMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_HTOZZMINNUM,"50"));
	InitInfo.SaveKindPara = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_SAVEKINDPARA,"11111100"));

	// added by yusy ic
	InitInfo.nIcLoadMinNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_ICLOADMINNUM, "50"));
	InitInfo.nIcLoadMaxNum = Asc2Float(IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_ICLOADMAXNUM, "1000"));

	tacDenial = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TACDENIAL, "");
	tacOnline = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TACONLINE, "");
	tacDefault = IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TACDEFAULT, "");

// 060411 合并版本
	strVersion = IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"VERSIONPATCH","000");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() strVersion[%s]", strVersion);

// VM60704 流水中文化
	LangMode = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_LANGUAGEMODE, CHN_MODE);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() LangMode[%d]", LangMode);

	iFstPIN = 0;

// VM60413 重启时间调整		
	strRebootTimeStart = IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"REBOOTTIME","040000");
	strRebootTime = IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"REBOOTTIMERIGHT","040000");
	int iTemp = Asc2Int(strRebootTimeStart) + 10000;
	strRebootTimeEnd.Format("%06d",iTemp);
		int iHou = Asc2Int(strRebootTimeStart.Left(2));
		int iMin = Asc2Int(strRebootTimeStart.Mid(2, 2));
		int iSec = Asc2Int(strRebootTimeStart.Right(2));
		srand(time(0));
		int iRandMin = rand()%50 + iMin;
		int iRandSec = rand()%50 + iSec;
		if(iRandMin > 60 && iHou == 23) iHou = 0;
		else
		if(iRandMin > 60) iHou = iHou + 1;
		if(iRandMin > 60) iRandMin = iRandMin - 60;
		if(iRandSec > 60) iRandSec = iRandSec - 60;
		strRebootTime.Format("%02d%02d%02d", iHou, iRandMin, iRandSec);
		IniSetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"REBOOTTIMERIGHT",strRebootTime);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() strRebootTimeStart[%s],strRebootTimeEnd[%s]", strRebootTimeStart,strRebootTimeEnd);

// 060413 打印device.ini
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() MachineType[%d]", m_pDevCmn->AtmDefine.MachineType);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() MachineKind[%d]", m_pDevCmn->AtmDefine.MachineKind);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() SprHandler[%d]", m_pDevCmn->AtmDefine.SprHandler);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() JprHandler[%d]", m_pDevCmn->AtmDefine.JprHandler);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() McuHandler[%d]", m_pDevCmn->AtmDefine.McuHandler);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() CashHandler[%d]", m_pDevCmn->AtmDefine.CashHandler);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() PBHandler[%d]", m_pDevCmn->AtmDefine.PBHandler);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() KeyHandler[%d]", m_pDevCmn->AtmDefine.KeyHandler);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() SoundDevice[%d]", m_pDevCmn->AtmDefine.SoundDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() SoundVolume[%d]", m_pDevCmn->AtmDefine.SoundVolume);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() CameraDevice[%d]", m_pDevCmn->AtmDefine.CameraDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() ICCardDevice[%d]", m_pDevCmn->AtmDefine.ICCardDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() PowerHandler[%d]", m_pDevCmn->AtmDefine.PowerHandler);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() SplDevice[%d]", m_pDevCmn->AtmDefine.SplDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() VfdDevice[%d]", m_pDevCmn->AtmDefine.VfdDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() DualDevice[%d]", m_pDevCmn->AtmDefine.DualDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() DvrDevice[%d]", m_pDevCmn->AtmDefine.DvrDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() OpDevice[%d]", m_pDevCmn->AtmDefine.OpDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() McuDevice[%d]", m_pDevCmn->AtmDefine.McuDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() MwDevice[%d]", m_pDevCmn->AtmDefine.MwDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() NmsDevice[%d]", m_pDevCmn->AtmDefine.NmsDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() EjrDevice[%d]", m_pDevCmn->AtmDefine.EjrDevice);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() EtcComKind[%d]", m_pDevCmn->AtmDefine.EtcComKind);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() InitInfo.TakeCashTimeOut[%d]", InitInfo.TakeCashTimeOut);


//Agent 设置设备状态 每次重启都发一个状态包
	memcpy(&szAgentOldStatus[0],GetSprintf("0000000000000"),13);         //资源状态 0:正常 1:少 2:空

//////////////////////////////////////////////////////////////////////////////////
//保存设备状态
		szMcuStatus = '0';        //读卡器  0：正常 1:满
		szPbStatus  = '0';        //存折打印机状态 0：正常 1:满 
		szBrmStatus = '0';
		szPinStatus = '1';         //0: no support 1:ok 9:error
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))				// Mcu Status
//			szMcuStatus= '8';
//		else
//			szMcuStatus= '0';
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))				// PIN Status
//			szPinStatus = '9';
//		else
//			szPinStatus = '1';
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))				// JPR Status
//			szJprStatus= '8';
//		else
//			szJprStatus= '0';
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))				// CDU Status
//			szCduStatus= '8';
//		else
//			szCduStatus= '0';
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_BRM))				// BRM Status
//			szBrmStatus = '8';
//		else
//			szBrmStatus = '0';
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))				// SPR Status
//			szSprStatus = '8';
//		else
//			szSprStatus = '0';
//		if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PBM))				// PBM Status
//			szPbStatus = '8';
//		else
//			szPbStatus = '0';
//
//		if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_CSH))
//			szCduStatus= '9';   
//		if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_MCU))
//			szMcuStatus= '9';  
//		if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_JPR))
//			szJprStatus= '9';  
//		if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_PIN))
//			szPinStatus= '0';  
//		if(!m_pDevCmn->fnAPL_GetDefineDevice(DEV_SPR))
//			szSprStatus= '9';  

		szDvrStatus = '0';                         //管理设备 0:no support 1:ok 9:error
		szJprStatus = '0';
		szSprStatus = '0';
		szCduStatus = '0';
		if(InitInfo.TranDvrMode == 1)
		{
// 			cDvrstatus = IniGetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRSTATUS,"0002");
			if(memcmp(InitInfo.DvrStatus,"0004",4) == 0      ||
				InitInfo.DvrChanStatus[0] == '2'||
				InitInfo.DvrChanStatus[1] == '2'||
				InitInfo.DvrChanStatus[2] == '2'||
				InitInfo.DvrChanStatus[3] == '2')    //by hyl 
				szDvrStatus = '9';
			else
				szDvrStatus = '1';
		}	
		memcpy(&szAgentOldErrorStatus,GetSprintf("7.7s",szCduStatus,szSprStatus,szJprStatus,szMcuStatus,szPbStatus,szPinStatus,szDvrStatus),7);
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	InitInfo.TranDvrMode = IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRMODE,0);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() TranDvrMode[%1.1d]", InitInfo.TranDvrMode);
	 
	CString m_strPort =GetSprintf("\\\\.\\%s",IniGetStr(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_OPENCOMM,"COM10"));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner() DvrCommPort[%s]",m_strPort);


// 状态报文控制参数 added by liuxl 20120719
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction statusflag[%d]", 
		IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_STATUSFLAG, 0));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction timeFilterflag[%d]",
		IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TIMEFILTERFLAG, 0));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction DvrStatusflag[%d]",
		IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA, _INIKEY_TRAN_DVRSTATUSFLAG,0));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction SprSet[%d]",
		Asc2Int(IniGetStr(_TRANSACTION_INI,_INIKEY_TRANSACTION,"SPRSET","0")));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Transaction cashboxopt[%d]",
		IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_CASHBOXOPTFLAG, 0));

   nAgentOperatorMode = FALSE;                   //设置柜员模式为非柜员模式 by hyl
   nAgnetAtmOpenFistFailFlag = TRUE;                  //设置第一次签到失败标识 by hyl
   nAgentAtmAfterOpenFistFailFlag = FALSE;            //签到成功后第一次签到失败标识 by hyl
   memcpy(&szCompanyCode[0],GetSprintf("%-2.2s",IniGetStr(_AGENT_ATMS,_AGENT_ATM_AP,"COMPANYCODE","11")),2);
   memcpy(&szAgentFtpServerIP[0],GetSprintf("%-15.15s",IniGetStr(_AGENT_FTPDOWNINFO,_AGENT_SYSINFO,"IP")),15);
   memcpy(&szAgentMachineCode[0],GetSprintf("%-20.20s",IniGetStr(_AGENT_ATMS,_AGENT_ATM_AP,"MACHINECODE","00000000000000000000")),20);
/////////////////////////////////////////////////////////////////////////////
	IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DETAILSENDCNT, 0);
																// 2006.03.21
	IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_DCEJRSENDP, 0);
																// 2006.07.03

/////////////////////////////////////////////////////////////////////////////
//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	memset(PinKey, 0, sizeof(PinKey));							// Pin Key
	memset(MacKey, 0, sizeof(MacKey));							// Pin Key
//	memset(FitFileContent, 0, sizeof(FitFileContent));			// Fit File Content
//	fnAPL_GetFitFileContentSave(_CARDFIT_DAT);					// Load Fit Table To Memory，结构改变，处理时从INI中读取   by zjd
	memset(FITContent_POST, 0, sizeof(FITContent_POST));			// Fit File Content
	memset(ICFITContent_POST, 0, sizeof(ICFITContent_POST));
	fnAPL_GetFitFileContent();
	fnAPL_GetICFitFileContent();
	fnAPL_GetEchannelFileContent();

	memset(&DetailData, ' ', sizeof(DetailData));				// Detail Data
	strDetailData = "";
	DetailBeginPSetFlag = FALSE;

///////////////////////////////////////////////////////////////////////////////下载标识
	DownloadParmFlag = 0;
	memset(NewParmVersion, '0', sizeof(NewParmVersion));
	DownloadFITFlag = 0;
	memset(NewFITVersion, '0', sizeof(NewFITVersion));
	DownloadPayFlag = 0;
	memset(NewPayVersion, '0', sizeof(NewPayVersion));
	DownloadOverFlag = 0;
	memset(EchannelProtContent, '0', sizeof(EchannelProtContent));
	TransPassWordFlag = 0;

	DownloadICFITFlag = 0;										// added by yusy ic
	memset(NewICFITVersion, '0', sizeof(NewICFITVersion));
	DownloadAIDListFlag = 0;
	memset(NewAIDListVersion, '0', sizeof(NewAIDListVersion));
	DownloadPBOCParaFlag = 0;
	memset(NewPBOCParaVersion, '0', sizeof(NewPBOCParaVersion));
	DownloadEchannelParaFlag = 0;
	memset(NewEchannelParaVersion, '0', sizeof(NewEchannelParaVersion));
	DownloadEchannelMtd = 0;
	DownloadEchannelNum = 1;

	memset(IsuAuthData, 0, sizeof(IsuAuthData));
	IsuAuthDataLen = 0;
	memset(IcScriptData, 0, sizeof(IcScriptData));
	IcScriptDataLen = 0;
	memset(IcScriptData2, 0, sizeof(IcScriptData2));
	IcScriptDataLen2 = 0;

/////////////////////////////////////////////////////////////////////////////需要预先处理代交费部分的交易列表，从INI中读取   by zjd

	
/////////////////////////////////////////////////////////////////////////////
	RecvOkFlag = FALSE;											// 2006.07.03
	NmsSendFlag = FALSE;
	DvrStatusReturn = 0;
	StatusEtcLength = 0;
	memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));
	memset(DCNmsDateTime, 0, sizeof(DCNmsDateTime));
	memset(DCBidEjrDate, 0, sizeof(DCBidEjrDate));

	DCEjrWriteFlag = FALSE;
	DCEjrSendCnt = 0;
	DCEjrSendData = "";

	CashRetractFlag = FALSE;									// 2011.01.04

/////////////////////////////////////////////////////////////////////////////
//	Create Menu
/////////////////////////////////////////////////////////////////////////////
	memcpy(MenuTbl, MenuTblConst, sizeof(MenuTbl));				// Initialize Menu
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 1Step)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 2Step)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 3Step)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 4Step)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 5Step)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 6Step)
	fnAPP_MenuMake(MenuTbl);									// Menu Make(Sub Menu 7Step)

    if(m_pDevCmn->HostLineMode != HOST_LOCAL_MODE)
	{
 	    AfxBeginThread(fnAPL_AtmpCommand, pOwner, THREAD_PRIORITY_NORMAL);
		if(m_pDevCmn->bAgentMode == 1)
	         AfxBeginThread(fnAPL_AgentCommand, pOwner, THREAD_PRIORITY_NORMAL);
	}

	//广告机心跳信号线程 2008.05.09
	if(IniGetInt(_ADMACHINE_INI, _INIKEY_ADMACHINE, _INIKEY_ADMACHINE_ADMODE, 0) == 1)
		AfxBeginThread(fnAPL_ADCOMMCommand, pOwner, THREAD_PRIORITY_NORMAL);

/////////////////////////////////////////////////////////////////////////////
//	Pboc Initial Data
/////////////////////////////////////////////////////////////////////////////
	fnAPD_PbcInitialData();										// Pboc Initial Data

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetOwner():return");
	return;
}

/////////////////////////////////////////////////////////////////////////////
//	Initialize Function
/////////////////////////////////////////////////////////////////////////////
// Initialize DS
int CTranCmn::fnAPP_InitializeDS(int SerialTranFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeDS()");

/////////////////////////////////////////////////////////////////////////////
//	Transaction Result
/////////////////////////////////////////////////////////////////////////////
	TranWriteFlag = FALSE;
	m_pDevCmn->TranResult = FALSE;								// Host Result
	m_pDevCmn->DeviceLost = DEV_NONE;							// 2006.08.30
	m_pTranCmn->strTFRInaccountType = "0";
	m_pTranCmn->strTFRSubType = "0";							// liyi add 绿卡通 转入帐户类型，1：定活，2：整整，3：通知
	m_pTranCmn->strTFRInData = "0";								// liyi add 绿卡通 存期
	m_pTranCmn->strTFRSubNo = "0";								// liyi add 绿卡通 其它储种子帐号
    m_pTranCmn->strRemitAddNo = "0";							//收款人序号 2008.10.05
	m_pTranCmn->strRemitAddName = "0";							//收款人姓名
	m_pTranCmn->strRemitAddAdd = "0";							//收款人地址
    m_pTranCmn->strRemitInNo = "0";

	m_pTranCmn->strPayInqRefNo = "000000000000";				// added by liyi 20110926

	HZTFlag = "";												// added by yusy 20110526
	iFstPIN = 0;
	
/////////////////////////////////////////////////////////////////////////////
//	Transaction Avail Variable
/////////////////////////////////////////////////////////////////////////////
if (!SerialTranFlag)
{
	TranAvail = TRAN_ALL_NOT;									// Transaction Avail
	WithAvail = TRAN_WITH_NOT;									// Withdraw Avail
	DepAvail = TRAN_DEP_NOT;									// Deposit Avail
	PbmAvail = TRAN_PBM_NOT;									// Passbook Avail
	memset(CurrTranCanDO, '0',sizeof(CurrTranCanDO));
	memset(TempTranCanDO, '0', sizeof(TempTranCanDO));
	LocalCardFlag = 0;
	LowAmountFlag = 0;
	m_pTranCmn->AccountType = "0";
    ACInqFlag = 0;
	CurrTypeFlag = 0;
	CurrTypes = 0;
	POSTICType = 0;
	ICScriptWriteFlag = 0;
}
	
/////////////////////////////////////////////////////////////////////////////
//	Menu Variable
/////////////////////////////////////////////////////////////////////////////
if (!SerialTranFlag)
{
	MenuIndex = 0;												// Menu Index
	MenuIndexSave = 0;											// Menu Index Save
	MenuIndexSub = 0;											// Menu Index Sub
	MenuIndexNext = 0;											// Menu Index Next
	MenuAvail = MENU_NULL;										// Menu Avail
	MenuNcTran = MENU_NULL;										// Menu No Card Transaction
	MenuBtnCnt = 0;												// Menu Button Count
	MenuBtnIndex = 0;											// Menu Button Index
	MenuBeforeInputFlag	= FALSE;								// Menu Before Input Flag
	MenuDoingFlag = TRUE;										// Menu Doing Flag
	MenuContinueFlag = FALSE;									// Menu Continue Flag

	TranProc = TRAN_NULL;										// Transaction Procedure
	TranCode = 0;												// Transaction Code
	TranCode2 = 0;												// Transaction Code2

	TranProcSave = TRAN_NULL;									// Transaction Procedure Save
	TranCodeSave = 0;											// Transaction Code Save
	TranCode2Save = 0;											// Transaction Code2 Save
}

/////////////////////////////////////////////////////////////////////////////
//	Main Variable
/////////////////////////////////////////////////////////////////////////////
if (!SerialTranFlag)
{
	MainTranDoingFlag = TRUE;									// Main Transaction Doing Flag
	MainTranDoingCount = 0;										// Main Transaction Doing Count
}
	MainTranReturnFlag = FALSE;									// Main Transaction Return Flag : 2006.08.22

/////////////////////////////////////////////////////////////////////////////
//	Library Variable
/////////////////////////////////////////////////////////////////////////////
	memset(&DetailData, ' ', sizeof(DetailData));				// Detail Data
	strDetailData = "";
	DetailBeginPSetFlag = FALSE;
	
/////////////////////////////////////////////////////////////////////////////
	RecvOkFlag = FALSE;											// 2006.07.03
	NmsSendFlag = FALSE;
	StatusEtcLength = 0;
	memset(StatusEtcBuffer, 0, sizeof(StatusEtcBuffer));

	DCEjrWriteFlag = FALSE;
	DCEjrSendCnt = 0;
	DCEjrSendData = "";

/////////////////////////////////////////////////////////////////////////////
//	Limit Amount Variable
/////////////////////////////////////////////////////////////////////////////
	MoneyMaxOut = 0;											// Money Max Out
	TMoneyMaxOut = 0;											// Transfer Money Max Out

/////////////////////////////////////////////////////////////////////////////
//	Input Variable
/////////////////////////////////////////////////////////////////////////////
if (!SerialTranFlag)
{
	memset(&Accept, 0, sizeof(Accept));							// Input Variable
	FirstPbcECInq = -10;
	LastPbcECInq = -10;
}

//	Input(Menu)
if (!SerialTranFlag)
{
	memset(Accept.PassWord, 'F', sizeof(Accept.PassWord));		// PassWord
}

//	Input(Before Send)
//	Input Money(Before Send)
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
	
//	Input Other(Before Send)
	memset(Accept.NewPassWord, 'F', sizeof(Accept.NewPassWord));	// New PassWord
	
	Accept.InAccountNumSize = 0;								// In Account Number Size
	memset(Accept.InAccountNum, 0, sizeof(Accept.InAccountNum));
																// In Account Number
//	Input(After Recv)
	Accept.SlipSelectFlag = TRUE;								// Slip Select Flag
	
/////////////////////////////////////////////////////////////////////////////
//	Send Host/Recv Host Variable
/////////////////////////////////////////////////////////////////////////////
	AddSerialFlag = FALSE;										// Add Serial Flag
	TranSend = 0;												// N Times Transaction Send Flag
	memset(OrgSerialNo, 0, sizeof(OrgSerialNo));				// Org Serial No
	memset(WithSerialNo, 0, sizeof(WithSerialNo));				// Withdrawal Serial No

	strBitMap = "";												// Bit Map
	SendLength = 0;												// Send Length
	MacLength  = 0;												// Mac Length
	RecvLength = 0;												// Recv Length

	memset(SendBuffer, 0, sizeof(SendBuffer));					// Send Buffer
	memset(MacBuffer,  0, sizeof(MacBuffer));					// Mac Buffer
	memset(RecvBuffer, 0, sizeof(RecvBuffer));					// Recv Buffer

	memset(&HMESend, 0, sizeof(HMESend));						// Host Message Edit Send
	memset(&HMERecv, 0, sizeof(HMERecv));						// Host Message Edit Recv
	
	memset(&AtmStatusInfo, 0, sizeof(AtmStatusInfo));			// Atm Status Info
	memset(&AtmTotalInfo, 0, sizeof(AtmTotalInfo));				// Atm Total Info
	memset(&AtmAddCashInfo, 0, sizeof(AtmAddCashInfo));			// Atm AddCash Info
	
	memset(&PSPRecvData, 0, sizeof(PSPRecvData));				// Parameter Slip Print Recv Data
	memset(&PPPRecvData, 0, sizeof(PPPRecvData));				// Parameter Passbook Print Recv Data
	memset(&PPWRecvData, 0, sizeof(PPWRecvData));				// Parameter Passbook Write Recv Data
	memset(&PCWRecvData, 0, sizeof(PCWRecvData));				// Parameter Card Write Recv Data
	memset(&PMERecvData, 0, sizeof(PMERecvData));				// Parameter Message Edit Recv Data
	memset(&PMDRecvData, 0, sizeof(PMDRecvData));				// Parameter Message Display Recv Data

	memset(&TranResponseData, 0, sizeof(TranResponseData));		// CWD,INQ,TFR,PIN,CWN Response
	memset(&RQKResponseData, 0, sizeof(RQKResponseData));		// RQK Response

/////////////////////////////////////////////////////////////////////////////
//	After Recv Host Variable
/////////////////////////////////////////////////////////////////////////////
	memset(&HostResp, 0, sizeof(HostResp));
	
/////////////////////////////////////////////////////////////////////////////
//	Media Variable
/////////////////////////////////////////////////////////////////////////////
if (!SerialTranFlag)
{
	DevEnDisable = 0;											// Device EnDisable
	CardTran = MENU_NULL;										// Card Transaction
	CardBank = 0;												// Card Bank
	CardService = 0;											// Card Service
	PbTran = MENU_NULL;											// Pb Transaction

	memset(&CardData, 0, sizeof(CardData));						// Card Data 
	pCardData2 = (MCISO2*)CardData.ISO2Buff;					// 2TR Pointer
	pCardData3 = (MCISO3*)CardData.ISO3Buff;					// 3TR Pointer
	memset(CardAccountNo, 0, sizeof(CardAccountNo));			// Card Account No
	memset(CardPan, 0, sizeof(CardPan));						// Card Pan
	memset(&PbMsData, 0, sizeof(PbMsData));						// Pb Data
	pPbMsData = (PBMS*)PbMsData.sBuf;							// Pb Pointer
}

	PbDataCnt = 0;												// Pb Data Count
	memset(PbData, 0, sizeof(PbData));							// Pb Data

	EjrIndexFlag = FALSE;										// Elec Journal Index Flag
	memset(&EjrData, ' ', sizeof(EjrData));						// Elec Journal Data

	memset(ShuData, 0, sizeof(ShuData));						// Shu Data
	SlipPrintFlag = FALSE;										// Slip Print Flag
	JnlPrintFlag = FALSE;										// Journal Print Flag
	
	MaterialOutFlag = 0;										// Material Out Flag
	MaterialOutCheckFlag = 0;									// Material Out Check Flag
	MaterialRetractFlag = 0;									// Material Retract Flag
	MoneyOutFlag = 0;											// Money Out Flag
	MoneyOutCheckFlag = 0;										// Money Out Check Flag
	MoneyRetractFlag = 0;										// Money Retract Flag
	TransCommTimeOut = 0;
	SprPrintATCFlag = 0;

/////////////////////////////////////////////////////////////////////////////
//	Cash Variable
/////////////////////////////////////////////////////////////////////////////
	CashFstCSTDispCnt = 0;										// Cash First CST Dispence Count
	CashSndCSTDispCnt = 0;										// Cash Second CST Dispence Count
	CashTrdCSTDispCnt = 0;										// Cash Third CST Dispence Count
	CashFthCSTDispCnt = 0;										// Cash Fourth CST Dispence Count

	CashRejCSTBeforeCnt = 0;									// Cash Reject CST Before Count
	CashFstCSTBeforeCnt = 0;									// Cash First CST Before Count
	CashSndCSTBeforeCnt = 0;									// Cash Second CST Before Count
	CashTrdCSTBeforeCnt = 0;									// Cash Third CST Before Count
	CashFthCSTBeforeCnt = 0;									// Cash Fourth CST Before Count
	
	CashRejCSTAfterCnt = 0;										// Cash Reject CST After Count
	CashFstCSTAfterCnt = 0;										// Cash First CST After Count
	CashSndCSTAfterCnt = 0;										// Cash Second CST After Count
	CashTrdCSTAfterCnt = 0;										// Cash Third CST After Count
	CashFthCSTAfterCnt = 0;										// Cash Fourth CST After Count

	CashRejCSTRejCnt = 0;										// Cash Reject CST Reject Count
	CashFstCSTDispRejCnt = 0;									// Cash First CST Dispense Reject Count
	CashSndCSTDispRejCnt = 0;									// Cash Second CST Dispense Reject Count
	CashTrdCSTDispRejCnt = 0;									// Cash Third CST Dispense Reject Count
	CashFthCSTDispRejCnt = 0;									// Cash Fourth CST Dispense Reject Count
	
	CashDispPrintFlag = FALSE;									// Cash Dispense Print Flag
	CashDispErrorFlag = 0;										// added by liuxl 20110802
	CashCountErrorFlag = 0;
	LotteryWin = FALSE;
	
	CashOutOpenFlag = FALSE;									// Cash Out Open Flag
	CashCountFlag = FALSE;										// Cash Count Flag
	CashCountEjrFlag = FALSE;									// Cash Count Ejr Flag

	CashInOpenFlag = FALSE;										// Cash In Open Flag
	CashInCountFlag = FALSE;									// Cash In Count Flag
	CashRejectFlag = FALSE;										// Cash Reject Flag
	CashRejectAFlag = FALSE;									// Cash Reject A Flag

/////////////////////////////////////////////////////////////////////////////
//	Pbc Variable
/////////////////////////////////////////////////////////////////////////////
if (!SerialTranFlag)											// 2010.10.20
{
	memset(&PbcAppList, 0, sizeof(PbcAppList));					// Pboc App List
	memset(&PbcAppDisp, 0, sizeof(PbcAppDisp));
	memset(&PbcTerm, 0, sizeof(PbcTerm));						// Pboc Term
	
	PbcAppTotal = 0;											// Pboc App Total
	PbcAppIndex = 0;											// Pboc App Index
	PbcAppDispTotal = 0;

	PbcCardData2Length = 0;										// Pboc Card Data 2TR Length
	memset(PbcCardData2, 0, sizeof(PbcCardData2));				// Pboc Card Data 2TR
	
	FallBack = 0;												// added by liuxl 20110511
	m_pDevCmn->Pbc.POSEntryCode = 0x90;							// Set Fall Back Mode added by yusy 20110719
}
	PbcTermFlag = TRUE;											// Pboc Terminate Flag
	memset(&PbcHostData, 0, sizeof(PbcHostData));				// Pboc Host Data
	memset(icdata9F36, 0, sizeof(icdata9F36));

/////////////////////////////////////////////////////////////////////////////
//	Output Variable
/////////////////////////////////////////////////////////////////////////////
	SumClearFlag = FALSE;										// Sum Clear Flag
	AddCashFlag = FALSE;										// Add Cash Flag

/////////////////////////////////////////////////////////////////////////////
//	Terminate Variable
/////////////////////////////////////////////////////////////////////////////
	CancelProcNum = 0;											// Cancel Procedure Number
	CancelProcCount = ' ';										// Cancel Procedure Count 
	CancelDepRejectFlag = FALSE;								// Cancel Deposit Reject Flag
	CancelDepRejectProcFlag = FALSE;							// Cancel Deposit Reject Procedure Flag
	CancelPrintFlag = FALSE;									// Cancel Print Flag
	CancelDepPrintFlag = FALSE;									// Cancel Deposit Print Flag

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	Agent Variable
/////////////////////////////////////////////////////////////////////////////
    bCommRecvFlag = 0;
	memset(szTempAtmStatus,0,512);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeDS():return");
	return T_OK;
}

// Initialize Serial Transaction DS
int CTranCmn::fnAPP_InitializeSerialTranDS()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeSerialTranDS()");

	fnAPP_InitializeDS(TRUE);									// Initialize DS

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_InitializeSerialTranDS():return");
	return T_OK;
}




