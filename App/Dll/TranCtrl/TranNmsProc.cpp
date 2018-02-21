/////////////////////////////////////////////////////////////////////////////
// TranNms.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include  "shellapi.h" 
#include  "AFXDISP.H"
#include "string.h" 
#include "iostream.h" 

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"
#include "..\..\H\Common\ClassInclude.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\DevCmn.h"
#include "..\..\H\Tran\TranCmnDefine.h"
#include "..\..\H\Tran\TranCmn.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
//	Nms Procedure
/////////////////////////////////////////////////////////////////////////////
// Nms Procedure
int CTranCmn::fnAPP_NmsProc(int InitFlag)								
{
	static	CString	strReBootDate("");
	static	int		ServiceSendFlag = FALSE;
	static  int     DvrIdelFlag = FALSE;
	int				ServiceStatus = FALSE;
	int				ErrorStatus = FALSE;
	int				CloseStatus = FALSE;
	CString			strOpMode = GetShareData("OP_OPSTATUS");	// 2006.01.10
	int				DvrStatus = fnAPL_GetDvrStatus();			// 2006.05.10

//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "---------------fnAPP_NmsProc()-----------------");
    // 2007.06.26  by hyl
	CString NowTime("");
	CString UpdateFileUseTime("");
	CString UpdateFileList;
	CString ADFileList;
	CString NoticeFileList;
	CString HostUpdateVersion("");
	CString NoticeFileUseTime("");
	CString NoticeFileEndTime("");
	CString CautionFileUseTime("");
	CString CautionFileEndTime("");
	CString cDvrstatus("");
	char    ADFileUseTime[24];
	char    ADFileEndTime[24];	
	char    UpdateFileName[12];
// 	char    ADFileName[12];
	char    TempFileName[12];
	int     FNameLength;
	SHFILEOPSTRUCT   FileOp;   
	char fPath[256],tPath[256];
	char DeleFileName[256];
	FileOp.wFunc = FO_COPY;
	FileOp.fFlags= FOF_NOCONFIRMATION;
	CStringArray FileNameList;
	memset(ADFileUseTime,0,sizeof(ADFileUseTime));
	memset(ADFileEndTime,0,sizeof(ADFileEndTime));
	memset(UpdateFileName,0,sizeof(UpdateFileName));
// 	memset(ADFileName,0,sizeof(ADFileName));
	memset(TempFileName,0,sizeof(TempFileName));
//	int ADNum = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM);
    int	ADSetNum = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM);
    
	// 浙江版本抽奖名额重置 20120306
	CString strDate = GetDate();
	if (strVersion == VERSIONZHEJIANG &&
		memcmp(strDate.GetBuffer(0), IniGetStr(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYSTARTDATE, "20120415"), 8) >= 0 &&
		memcmp(strDate.GetBuffer(0), IniGetStr(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYENDDATE, "20120715"), 8) <= 0 &&
		memcmp(strDate.GetBuffer(0), IniGetStr(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYSETDATE, "00000000"), 8) > 0)
	{
		if (IniGetInt(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYWINREMNUM, 2) !=
			IniGetInt(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYWINNUM, 2))
			IniSetInt(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYWINREMNUM, 
				IniGetInt(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYWINNUM, 2));
		IniSetStr(_TRANSACTION_INI, _INIKEY_LOTTERY, _INIKEY_TRANSACTION_LOTTERYSETDATE, strDate.GetBuffer(0));
	}


	//获取广告下载批次启用时间最早的时间
 	Sprintf(ADFileUseTime,12,"%12.12s","999999999999");
	int ADUSECOUNT = 0;	
	for(int USECOUNT = 1;USECOUNT <= ADSetNum;USECOUNT++)
	{
		if(IniGetStr(_TRANSACTION_INI,_INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",USECOUNT)) !="" && 
		ADFileUseTime > IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",USECOUNT)))
		{
		  Sprintf(ADFileUseTime, 12, "%12.12s",IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",USECOUNT)));
		  ADUSECOUNT = USECOUNT;
		}
    }
	
	//获取广告下载批次结束时间最早的时间
	Sprintf(ADFileEndTime, 12, "%12.12s","999999999999");
    int ADENDCOUNT = 0;
	for(int ENDCOUNT = 1;ENDCOUNT <= ADSetNum;ENDCOUNT++)
	{   
		if(	IniGetStr(_TRANSACTION_INI,_INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ENDCOUNT))!=""
			&& ADFileEndTime > IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ENDCOUNT)))
		{
			Sprintf(ADFileEndTime, 12, "%12.12s",IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ENDCOUNT)));
			ADENDCOUNT = ENDCOUNT;
		}
    }
	NowTime = GetSprintf("%6.6s%6.6s",&(GetDate().GetBuffer(0))[2],
		                                GetTime().GetBuffer(0));
	UpdateFileUseTime = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTUPDATEFILEUSETIME);
	UpdateFileList = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTUPDATEFILELIST);

    NoticeFileUseTime = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREUSETIME);
	NoticeFileEndTime = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME);

	CautionFileUseTime = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREUSETIME);
	CautionFileEndTime = IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME);
    
	
	if (m_pDevCmn->AtmStatus != ATM_TRAN)
	{
		if (m_pDevCmn->AtmStatusSave == ATM_ERROR)
		{
			if (HostNms.HostResetAll)
			{
				HostNms.HostResetAll = FALSE;
				m_pDevCmn->fnAPL_ClearError();
				m_pDevCmn->RecoveryType = 1;
				m_pDevCmn->DeviceAutoOff = DEV_NONE;
				m_pDevCmn->DeviceRetract = DEV_NONE;
				m_pDevCmn->DeviceLost = DEV_NONE;
				m_pDevCmn->fnAPL_ResetDevice(DEV_MAIN);
			}
			if(HostNms.ModuleResetFlag != 0)
			{
				m_pDevCmn->fnAPL_ClearError();
				m_pDevCmn->RecoveryType = 1;
				m_pDevCmn->DeviceAutoOff = DEV_NONE;
				m_pDevCmn->DeviceRetract = DEV_NONE;
				m_pDevCmn->DeviceLost = DEV_NONE;
				switch(HostNms.ModuleResetFlag)
				{
					case 1:
						if(m_pDevCmn->fnAPL_ResetDevice(DEV_MCU))
							bAgentRestResultFlag = 1;
                        else 
						    bAgentRestResultFlag = 0;
                        
						break;
					case 2:
						if(m_pDevCmn->fnAPL_ResetDevice(DEV_CSH))
							bAgentRestResultFlag = 1;
                        else 
						    bAgentRestResultFlag = 0;

						break;
					case 3:
						if(m_pDevCmn->fnAPL_ResetDevice(DEV_SPR))
							bAgentRestResultFlag = 1;
                        else 
						    bAgentRestResultFlag = 0;

						break;
					case 4:
						if(m_pDevCmn->fnAPL_ResetDevice(DEV_JPR))
							bAgentRestResultFlag = 1;
                        else 
						    bAgentRestResultFlag = 0;

						break;
					case 5:
					   	if(m_pDevCmn->fnAPL_ResetDevice(DEV_PBM))
							bAgentRestResultFlag = 1;
                        else 
						    bAgentRestResultFlag = 0;

						break;
				}
				m_pDevCmn->fnAPL_ShareResetDeviceResult(bAgentRestResultFlag);
				HostNms.ModuleResetFlag = 0;
			}
		}
		else
		if (HostNms.HostResetAll || HostNms.ModuleResetFlag != 0)
		{
			m_pDevCmn->AtmStatus = ATM_ERROR;
			return T_OK;
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	Init Procedure
/////////////////////////////////////////////////////////////////////////////
	if (InitFlag)		
	{
		strReBootDate = RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_REBOOT);
		m_pDevCmn->fnNET_Initialize3(LINE_BID_OPCL_TIME);		// 2005.05.26
		m_pDevCmn->fnNMS_InitializeIP(InitInfo.NmsIp, InitInfo.NmsPort);
																// 2006.07.03

		memset(&HostNms, 0, sizeof(HostNms));

		HostNms.RecycleBoxStatus	= CST_NORMAL;
		HostNms.FstCSTStatus		= CST_NORMAL;				// 2005.09.07
		HostNms.SndCSTStatus		= CST_NORMAL;
		HostNms.TrdCSTStatus		= CST_NORMAL;
		HostNms.FthCSTStatus		= CST_NORMAL;
		HostNms.WithAvail			= TRAN_WITH_BOTH;
		HostNms.SlipStatus			= SLIP_NORMAL;
		HostNms.JnlStatus			= JNL_NORMAL;
		HostNms.DoorStatus			= DOOR_CLOSED;
		HostNms.ServiceStatus		= FALSE;
		HostNms.ErrorStatus			= FALSE;
		HostNms.CloseStatus			= FALSE;

		NmsSendLength = 0;
		NmsRecvLength = 0;
		memset(NmsRecvLengthTbl, 0, sizeof(NmsRecvLengthTbl));
		
		NmsRecvInTbl = 0;
		NmsRecvOutTbl = 0;
		memset(NmsSendBuffer, 0, sizeof(NmsSendBuffer));
		memset(NmsRecvBuffer, 0, sizeof(NmsRecvBuffer));
		memset(NmsRecvBufferTbl, 0, sizeof(NmsRecvBufferTbl));
	}

/////////////////////////////////////////////////////////////////////////////
//	Skip Procedure
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmStatusSave == ATM_CUSTOM) &&				// Customer Go
		(m_pDevCmn->AtmStatus == ATM_TRAN))
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
//	Recv Data Clear
/////////////////////////////////////////////////////////////////////////////
	fnAPP_RecvDataClear();

/////////////////////////////////////////////////////////////////////////////
//	Make Flag Procedure(Non Condition Procedure)
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)		// Same Mode : 2006.03.21
	{
		if ((m_pDevCmn->AtmStatus == ATM_ERROR)		||
			(m_pDevCmn->AtmStatus == ATM_CLERK)		||
			(m_pDevCmn->AtmStatus == ATM_READY))				// 060412
			ServiceSendFlag = TRUE;
	}

	if ((m_pDevCmn->AtmStatus == ATM_CUSTOM) ||					// Service Mode Setting : 2006.03.21
		(m_pDevCmn->AtmStatus == ATM_TRAN))
	{
		if (ServiceSendFlag == TRUE)
			ServiceStatus = TRUE;

		ServiceSendFlag = FALSE;
		if (InitInfo.BidAtmColseFlag)
		{
			m_pDevCmn->OpenKey = FALSE;
			CloseStatus = TRUE;
		}
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_ERROR) &&				// Error Mode Setting : 2005.09.08
		(m_pDevCmn->AtmStatus == ATM_ERROR))
		ErrorStatus = TRUE;
	else
	if ((InitInfo.BidAtmColseFlag)	&&							// 2005.09.11
		(!m_pDevCmn->OpenKey))
		CloseStatus = TRUE;

	if (m_pDevCmn->AtmStatus == m_pDevCmn->AtmStatusSave)		// Same Mode : 2005.08.22
	{
		if (Asc2Int(m_pDevCmn->fstrMCU_GetRetractCnt()) >= 30)			// 吞卡满了以后自动清除，让机器继续服务  by zjd
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_RETRACTCARD, "RETRACTCARDNUM", 0);
			m_pDevCmn->fnAPL_ClearRetractCnt(DEV_MCU);
		}

		if ((m_pDevCmn->AtmStatus == ATM_ERROR)		||			// Status Send Check : 2005.09.11
			(m_pDevCmn->AtmStatus == ATM_CLERK)		||
			(m_pDevCmn->AtmStatus == ATM_READY)		||
			(m_pDevCmn->AtmStatus == ATM_CUSTOM))
		{
			// added by liuxl 20110622
			int msflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_MAINTAINSTATUSFLAG, 1);
			
			if (strOpMode.CompareNoCase("OPDO") == 0 && msflag == 0)
				m_pDevCmn->Timer.StatusSendDelayTime = InitInfo.MsrSendTime>60 ? InitInfo.MsrSendTime-20 : InitInfo.MsrSendTime;
			else
			if (m_pDevCmn->Timer.StatusSendDelayTime == 0)
			{
				HostNms.MsrSendFlag = TRUE;	
				m_pDevCmn->Timer.StatusSendDelayTime = InitInfo.MsrSendTime>60 ? InitInfo.MsrSendTime-20 : InitInfo.MsrSendTime;
			}
			else
			if (m_pDevCmn->Timer.AgentStatusSendDelayTime == 0)  // added by yusy 08.09.09
			{
				if(m_pDevCmn->bAgentMode == 1)
				{
					fnAPP_AgentIdxGetData(AGENT_STATUS);
				}
				m_pDevCmn->Timer.AgentStatusSendDelayTime = InitInfo.AgentMsrSendTime;
			}
			else
//			if (m_pDevCmn->AtmStatus == ATM_ERROR)
//				;
// 			else
//			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH | DEV_MCU | DEV_SPR))
			if (m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH | DEV_MCU))
				;												// 060411 出钞器、读卡器故障不进行重启
			else
			if ((GetTime() > strRebootTime)	&&			// Rebooting Check
				(GetTime() < strRebootTimeEnd))					// 060411 重启时间调整
			{
				if (strReBootDate != GetDate())
				{
					HostNms.RebootFlag = TRUE;
					m_pDevCmn->fnAPL_SetClerkPowerOff();		// Set Clerk Power Off : 2004.06.27
				}
			}
            
            
			//升级文件启用 BY HYL
			else
			if(NowTime > UpdateFileUseTime  && UpdateFileUseTime != "" && UpdateFileList !="")
			{	
				FileOp.wFunc = FO_COPY;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NowTime[%s]",NowTime);
	            MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::UpdateFileUseTime[%s]",UpdateFileUseTime);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::UpdateFileList[%s]",UpdateFileList);
				SplitString(UpdateFileList, "|", FileNameList);
				int i=0;
				while (i < FileNameList.GetSize())
				{	
          
					FNameLength = FileNameList[i].GetLength() - 9;
					memcpy(&TempFileName,GetSprintf("%s",FileNameList[i]),FileNameList[i].GetLength());
	                memcpy(UpdateFileName,GetSprintf("%s",&TempFileName[9]),FNameLength);
					memset(fPath,0,sizeof(fPath));
					memset(tPath,0,sizeof(fPath));
					sprintf(fPath, "%s\\%s", _AP_SOFTBACKUPDATE_DIR,UpdateFileName);
					sprintf(tPath, "%s\\%s", _AP_SOFTUPDATE_DIR, UpdateFileName);

                    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::UpdateFileName[%s]",UpdateFileName);
					FileOp.pFrom = fPath;   
					FileOp.pTo = tPath; 
					FileOp.fFlags= FOF_NOCONFIRMATION;

					if(!SHFileOperation(&FileOp))
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::SoftUpdateFile[%s] Copy Success!",UpdateFileName);  
											
					}
					else  
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::SoftUpdateFile[%s] Copy Failed!",UpdateFileName);   	
				 i++;
				}
				
				HostUpdateVersion = GetSprintf("%04s",IniGetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTDOWNLOADVERSION));
                IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTUPDATEVERSION,HostUpdateVersion);
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_HOSTDOWNLOADVERSION,"");
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTUPDATEFILELIST,"");
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_SOFTUPDATEFILEUSETIME,"");
				m_pTranCmn->HostNms.RebootFlag = TRUE;
			    m_pDevCmn->fnAPL_SetClerkPowerOff();
			}

			///广告图片启用 
            else 
			//if( NowTime > ADFileUseTime && ADFileUseTime!="" && ADUSECOUNT <= ADSetNum)
			if( NowTime > ADFileUseTime && ADFileUseTime!="" )
			{	
		     	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AD%d.jpg  UseTime:%s",ADUSECOUNT,ADFileUseTime);
			    MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADUSECOUNT:%d",ADUSECOUNT);
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADNum:%d",ADNum);
				m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;
				m_pDevCmn->fnSCR_DisplayScreen(27);							// Stop Service
				m_pDevCmn->fnAPL_UnUsed();
                
				memset(DeleFileName,0,sizeof(DeleFileName));
				strcpy(DeleFileName,GetSprintf("%s\\%s%d%s",_AP_ADUPDATE_DIR,"AD",ADUSECOUNT,".jpg"));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "DeleFileName[%s]",DeleFileName);	
				CFileFind   TempFp;
	            BOOL        bFind;
				bFind = TempFp.FindFile(DeleFileName);
	
				memset(fPath,0,sizeof(fPath));
				memset(tPath,0,sizeof(fPath));
				strcpy(fPath,GetSprintf("%s\\%s%d%s",_AP_ADUPDATEDOWNLOAD_DIR,"AD",ADUSECOUNT,".jpg"));   
				strcpy(tPath,_AP_ADUPDATE_DIR);  		
				CFileFind   TempFp1;
				BOOL        bFind1;
				bFind1 = TempFp1.FindFile(fPath);
				
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADNum[%d]",ADNum);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADUSECOUNT[%d]",ADUSECOUNT);
				
				//增加判断文件是不是存在,不存在则不进行原广告删除操作,广告张数加1 存在则广告张数不加1 BY HYL 

// 				if( ADUSECOUNT == ADNum+1 )						// liyi 20111024
// 				{
// 					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM,ADUSECOUNT);	
// 				}	
// 				ADNum = ADNum+1;
// 				IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM,ADNum);
				
				if(bFind && bFind1)
				{ 
					FileOp.wFunc = FO_DELETE;	
					FileOp.fFlags= FOF_NOCONFIRMATION;
					
					FileOp.pFrom = DeleFileName; 
					FileOp.pTo =DeleFileName;
					if(!SHFileOperation(&FileOp))
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE %s Success!",DeleFileName); 
					}
					else
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE %s ADFile Failed!",DeleFileName); 
				}
				
				if(!bFind1)												
				  ;
				else
				{	
					//FileOp.wFunc = FO_MOVE;							// liyi 20111024
					FileOp.wFunc = FO_COPY;
				    FileOp.fFlags= FOF_NOCONFIRMATION;
					FileOp.pFrom = fPath;   
				    FileOp.pTo = tPath; 
					if(!SHFileOperation(&FileOp))
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::ADLoadUpdate %s Move Success!",fPath);
					else 
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::ADLoadUpdate %s Move Failed!",fPath); 		
                }

//				if (!bFind && bFind1)									// added by liuxl 20120208
//				{
//					ADNum = ADNum+1;
//					IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM,ADNum);
//				}
				
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",ADUSECOUNT),"");
				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11

					int nStatus = m_pDevCmn->fnSNS_GetOperatorSwitch();
					if (m_pDevCmn->AtmDefine.SplDevice == SPL_ONOFFSW)
					{
						if ((nStatus == OPER_SUPERVISOR) ||
							(nStatus == OPER_SUPERVISORCHANGED))
						{
							m_pDevCmn->OpenKey = FALSE;
							InitInfo.BidAtmColseFlag = TRUE;	
						}
					}
					else														
					if (m_pDevCmn->AtmDefine.SplDevice == SPL_TOGGLESW)
					{
						if ((nStatus == OPER_SUPERVISORCHANGED) ||
							(nStatus == OPER_RUNCHANGED))
						{
							m_pDevCmn->OpenKey = FALSE;
							InitInfo.BidAtmColseFlag = TRUE;
						}
					}
				} 
			}
			
            //恢愎原来的广告图片//增加判断当有备份文件时ADENDCOUNT <= ADPICTURE 时才让恢复, 无备份文件时ADENDCOUNT = ADPICTURE 时才让ADNUM-1 by hyl 
			else
			//if( NowTime > ADFileEndTime && ADFileEndTime != "" && ADENDCOUNT <= ADSetNum )
			if( NowTime > ADFileEndTime && ADFileEndTime != "" )
			{	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADFileEndTime:%s",ADFileEndTime);
				m_pDevCmn->OpenKey = FALSE;
				InitInfo.BidAtmColseFlag = TRUE;
				m_pDevCmn->fnSCR_DisplayScreen(27);							// Stop Service
				m_pDevCmn->fnAPL_UnUsed();
                			
				CFileFind   TempFp;
				BOOL        bFind;
				CFileFind   TempFp1;
	            BOOL        bFind1;
				memset(DeleFileName,0,sizeof(DeleFileName));
				strcpy(DeleFileName,GetSprintf("%s\\%s%d%s",_AP_ADUPDATE_DIR,"AD",ADENDCOUNT,".jpg"));

				memset(fPath,0,sizeof(fPath));
				memset(tPath,0,sizeof(fPath));
				strcpy(fPath,GetSprintf("%s\\%s%d%s",_AP_ADUPDATEBACK_DIR,"AD",ADENDCOUNT,".jpg"));   
				strcpy(tPath,_AP_ADUPDATE_DIR); 
				
                bFind = TempFp.FindFile(DeleFileName);
				bFind1 = TempFp1.FindFile(fPath);
				//if(!bFind1)                                       // liyi 20111024
				{  
					//if(ADENDCOUNT == ADNum)						
					{                     
					    DeleteFile(GetSprintf("%s\\%s%d%s",_AP_ADUPDATE_DIR,"AD",ADENDCOUNT,".jpg"));	  // BY HYL 
//						ADNum = ADNum-1;
//						IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM,ADNum);
						IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",ADENDCOUNT),"");
					}
					
				    IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ADENDCOUNT),"");
// 					IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREUSETIME",ADENDCOUNT),"");
				}
//              else
//              {
// 					if(bFind)
// 					{	
// 						FileOp.wFunc = FO_DELETE;
// 						FileOp.fFlags= FOF_NOCONFIRMATION;
// 						FileOp.pFrom = DeleFileName; 
// 						FileOp.pTo =DeleFileName;
// 						if(!SHFileOperation(&FileOp))
// 						{
// 							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE AD%d.jpg Success!",ADENDCOUNT); 
// 						}
// 						else
// 							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE AD%d.jpg Failed!",ADENDCOUNT); 
// 					}	
// 					
// 					FileOp.wFunc = FO_MOVE;
// 					FileOp.fFlags= FOF_NOCONFIRMATION;  
// 					FileOp.pFrom = fPath;   
// 					FileOp.pTo = tPath;   
// 					if(!SHFileOperation(&FileOp))
// 					{
// 						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::ADRBACK AD%d.jpg Move Success!",ADENDCOUNT);  
// 					}
// 					else  
// 					{
// 						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::ADRBACK AD%d.jpg File Move Failed!",ADENDCOUNT);   			
// 					}
// 
// 					IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, GetSprintf("%s%d","ADPICTUREENDTIME",ADENDCOUNT),"999999999999");   //恢复图片后将图片置于永久广告
// 				}
 			    
				
				
				if (InitInfo.BidAtmColseFlag)					// 2006.03.21
				{
					m_pDevCmn->OpenKey = TRUE;
					InitInfo.BidAtmColseFlag = FALSE;			// 2005.09.11

					int nStatus = m_pDevCmn->fnSNS_GetOperatorSwitch();
					if (m_pDevCmn->AtmDefine.SplDevice == SPL_ONOFFSW)
					{
						if ((nStatus == OPER_SUPERVISOR) ||
							(nStatus == OPER_SUPERVISORCHANGED))
						{
							m_pDevCmn->OpenKey = FALSE;
							InitInfo.BidAtmColseFlag = TRUE;	
						}
					}
					else														
					if (m_pDevCmn->AtmDefine.SplDevice == SPL_TOGGLESW)
					{
						if ((nStatus == OPER_SUPERVISORCHANGED) ||
							(nStatus == OPER_RUNCHANGED))
						{
							m_pDevCmn->OpenKey = FALSE;
							InitInfo.BidAtmColseFlag = TRUE;
						}
					}
				} 
			}
			/////////////////////////////////////////////////////////
			///公告图片启用
            else 
			if( NowTime > NoticeFileUseTime && NoticeFileUseTime!="" 
				&& IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREDOWNLOADNUM) !=0)   
			{	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NoticeFileUseTime:%s",NoticeFileUseTime);
				FileOp.wFunc = FO_DELETE;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				memset(DeleFileName,0,sizeof(DeleFileName));
				strcpy(DeleFileName,GetSprintf("%s\\%s",_AP_NOTICEUPDATE_DIR,"NOTICE*.*"));
				FileOp.pFrom = DeleFileName; 
				FileOp.pTo =DeleFileName;
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE NoticeFile Success!"); 
				}
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE NoticeFile Failed!"); 
				
				
				FileOp.wFunc = FO_MOVE;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				memset(fPath,0,sizeof(fPath));
				memset(tPath,0,sizeof(fPath));
				strcpy(fPath,GetSprintf("%s\\%s",_AP_NOTICEUPDATEDOWNLOAD_DIR,"NOTICE*.*"));   
				strcpy(tPath,_AP_NOTICEUPDATE_DIR);   
				FileOp.pFrom = fPath;   
				FileOp.pTo = tPath;   
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NoticeUpdate File Move Success!");  
					int NoticeNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREDOWNLOADNUM);
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREDOWNLOADNUM,0);            
					IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREUSETIME,"");
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM,NoticeNum);
					
				}
				else  
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NoticeLoadUpdate File Move Failed!");   
			}
            //恢愎原来的公告图片
			else
			if( NowTime > NoticeFileEndTime && NoticeFileEndTime != "" && 
				IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTRUEBACKNUM) !=0)
			{	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "NoticeFileEndTime:%s",NoticeFileEndTime);
				FileOp.wFunc = FO_DELETE;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				strcpy(DeleFileName,GetSprintf("%s\\%s",_AP_NOTICEUPDATE_DIR,"NOTICE*.*"));
				FileOp.pFrom = DeleFileName; 
				FileOp.pTo =DeleFileName;
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE NoticeFile Success!"); 
				}
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE NoticeFile Failed!"); 
				
				FileOp.wFunc = FO_MOVE;	
				FileOp.fFlags= FOF_NOCONFIRMATION;
				strcpy(fPath,GetSprintf("%s\\%s",_AP_NOTICEUPDATEBACK_DIR,"NOTICE*.*"));   
				strcpy(tPath,_AP_NOTICEUPDATE_DIR);   
				FileOp.pFrom = fPath;   
				FileOp.pTo = tPath;   
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NOTICEREBACKFile Move Success!");  
					int NoticeBackNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTRUEBACKNUM);
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM,NoticeBackNum);
					IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME,"999999999999");   //恢复图片后将图片置于永久广告
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTRUEBACKNUM,0);
				}
				else  
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NoticeLoadUpdate File Move Failed!");   
			}
			//公告到期,取消公告
			else
			if( NowTime > NoticeFileEndTime && NoticeFileEndTime != "")
			{
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::NoticeFileEndTime[%s]",NoticeFileEndTime); 
				FileOp.wFunc = FO_DELETE;
				memset(DeleFileName,0,sizeof(DeleFileName));
				strcpy(DeleFileName,GetSprintf("%s\\%s",_AP_NOTICEUPDATE_DIR,"NOTICE*.*"));
				FileOp.pFrom = DeleFileName; 
				FileOp.pTo =DeleFileName;
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::Cancel NoticeFile Success!"); 
				}
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::Cancel NoticeFile Failed!"); 
				
				IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTURENUM,0);
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREUSETIME,"");
				IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_NOTICEPICTUREENDTIME,"");
			}

			// added by yusy 2010.03.22
			/////////////////////////////////////////////////////////
			///安全提示图片启用
            else 
			if( NowTime > CautionFileUseTime && CautionFileUseTime!="" 
				&& IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREDOWNLOADNUM) !=0)   
			{	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CautionFileUseTime:%s",CautionFileUseTime);
				FileOp.wFunc = FO_DELETE;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				memset(DeleFileName,0,sizeof(DeleFileName));
				strcpy(DeleFileName,GetSprintf("%s\\%s",_AP_CAUTIONUPDATE_DIR,"CAUTION*.*"));
				FileOp.pFrom = DeleFileName; 
				FileOp.pTo =DeleFileName;
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE CautionFile Success!"); 
				}
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE CatuionFile Failed!"); 
				
				
				FileOp.wFunc = FO_MOVE;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				memset(fPath,0,sizeof(fPath));
				memset(tPath,0,sizeof(fPath));
				strcpy(fPath,GetSprintf("%s\\%s",_AP_CAUTIONUPDATEDOWNLOAD_DIR,"CAUTION*.*"));   
				strcpy(tPath,_AP_CAUTIONUPDATE_DIR);   
				FileOp.pFrom = fPath;   
				FileOp.pTo = tPath;   
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CautionUpdate File Move Success!");  
					int CautionNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREDOWNLOADNUM);
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREDOWNLOADNUM,0);            
					IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREUSETIME,"");
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURENUM,CautionNum);
					
				}
				else  
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CautionLoadUpdate File Move Failed!");   
			}
			//恢愎原来的安全提示图片
			else
			if( NowTime > CautionFileEndTime && CautionFileEndTime != "" && 
				IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTRUEBACKNUM) !=0)
			{	
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CautionFileEndTime:%s",CautionFileEndTime);
				FileOp.wFunc = FO_DELETE;
				FileOp.fFlags= FOF_NOCONFIRMATION;
				strcpy(DeleFileName,GetSprintf("%s\\%s",_AP_CAUTIONUPDATE_DIR,"CAUTION*.*"));
				FileOp.pFrom = DeleFileName; 
				FileOp.pTo =DeleFileName;
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE CautionFile Success!"); 
				}
				else
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::DELETE CautionFile Failed!"); 
				
				FileOp.wFunc = FO_MOVE;	
				FileOp.fFlags= FOF_NOCONFIRMATION;
				strcpy(fPath,GetSprintf("%s\\%s",_AP_CAUTIONUPDATEBACK_DIR,"CAUTION*.*"));   
				strcpy(tPath,_AP_CAUTIONUPDATE_DIR);   
				FileOp.pFrom = fPath;   
				FileOp.pTo = tPath;   
				if(!SHFileOperation(&FileOp))
				{
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CAUTIONREBACKFile Move Success!");  
					int CautionBackNum=IniGetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTRUEBACKNUM);
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTURENUM,CautionBackNum);
					IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTUREENDTIME,"999999999999");
					IniSetInt(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_CAUTIONPICTRUEBACKNUM,0);
				}
				else  
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "::CautionLoadUpdate File Move Failed!");   
			}
		}
		else
		{
			m_pDevCmn->Timer.StatusSendDelayTime = InitInfo.MsrSendTime>60 ? InitInfo.MsrSendTime-20 : InitInfo.MsrSendTime;
			m_pDevCmn->Timer.AgentStatusSendDelayTime = InitInfo.AgentMsrSendTime;  // added by yusy 08.09.09
		}
	}
	else
	{
		m_pDevCmn->Timer.StatusSendDelayTime = InitInfo.MsrSendTime>60 ? InitInfo.MsrSendTime-20 : InitInfo.MsrSendTime;
		m_pDevCmn->Timer.AgentStatusSendDelayTime = InitInfo.AgentMsrSendTime;  // added by yusy 08.09.09
	}

	if (m_pDevCmn->AtmStatusSave == ATM_TRAN)					// Aex Send Check
	{
		if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM) == 1)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, T_OK);
			HostNms.AexCustomerCardTimeOutSendFlag = TRUE;
		}
		else
		if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM) == 2)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, T_OK);
			HostNms.AexCustomerCashTimeOutSendFlag = TRUE;
		}
		else
		if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM) == 3)
		{
			IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANCUSTOM, T_OK);
			HostNms.AexCustomerHostRetractSendFlag = TRUE;
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	Make Flag Procedure(Condition Procedure)
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmStatusSave == ATM_INIT)					// Power On Send Check
	{
//		HostNms.OexPowerOnSendFlag = TRUE;
// 		if ((IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1) ||
// 			(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3) ||
// 			(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 5))		// 2008.10.13
		if (CwcInfo.CwcFlag != 0)								// added by liuxl 20110714
			HostNms.CwcSendFlag = TRUE;							// Power On Cwc Send Check
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_TRAN)	&&				// Cwc Send Check
		(!m_pDevCmn->fnAPL_CheckError())		&&				// Error
		(m_pDevCmn->TranType == 1))								// Customer Use
	{
		m_pDevCmn->TranType = 0;

// 		if ((IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1) ||
// 			(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3) ||
// 			(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 5))		// 2008.10.13
		if (CwcInfo.CwcFlag != 0)								// added by liuxl 20110714
			HostNms.CwcSendFlag = TRUE;

		HostNms.ErrorAutoOffSave = m_pDevCmn->DeviceAutoOff;
		HostNms.ErrorKind = fnAPL_GetErrorCodeKind(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR));
		HostNms.ErrorFlag = TRUE;
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_TRAN)	&&				// Oex Send Check
		(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_RESPONSENO).GetLength())	&&
		(m_pDevCmn->TranType == 1))								// Customer Use
	{
		m_pDevCmn->TranType = 0;
	}
	else
	if ((m_pDevCmn->AtmStatusSave == ATM_ERROR) &&				// Oex Send Check
		(HostNms.ErrorFlag))									// Error Exist
	{
		HostNms.ErrorFlag = FALSE;

		HostNms.OexSendFlag = TRUE;								// 2005.05.26

		if (m_pDevCmn->DeviceRetract & DEV_MCU)					// 2006.03.21
		{
			if (m_pDevCmn->AtmStatus != ATM_ERROR)				// Auto Recovery Ok
			{
				HostNms.OexSendFlag = FALSE;					// FALSE
			}
		}
		else
		{
			if (m_pDevCmn->AtmStatus != ATM_ERROR)				// Auto Recovery Ok
				HostNms.OexSendFlag = FALSE;					// FALSE
			else
			{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RecoverFail[%12.12s]", szAgentErrorCode);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CurrentErr[%s]", m_pDevCmn->CurErrBuff.ErrorCode);
			}
		}
	}
	else
	if (HostNms.DoorStatus != m_pDevCmn->DoorStatus)			// Door Open Send Check
	{
        if(m_pDevCmn->bAgentMode == 1)
		{
			bAgetDoorFLAG = TRUE;    //保险柜门开关变化标志 by hyl
			if( bAgetDoorFLAG = TRUE )
			{
				if(m_pDevCmn->DoorStatus == DOOR_OPENED)
				{
					fnAPP_AgentIdxGetData(CLERK_DOOR_OPEN);
				}
				else
				if(m_pDevCmn->DoorStatus == DOOR_CLOSED)
				{
					fnAPP_AgentIdxGetData(CLERK_DOOR_CLOSE);
				}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "HostNms.DoorStatus[%d]", HostNms.DoorStatus);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "m_pDevCmn->DoorStatus[%d]", m_pDevCmn->DoorStatus);
			}
		}
		if(InitInfo.TranDvrMode == 1)
		{
			if(m_pDevCmn->DoorStatus == DOOR_OPENED)
			{
				fnAPP_DvrDataComm(CMDCODE_INCARD,DVRCHAN_IDX123,5,"DOOROPEN");
			}
			else
			if(m_pDevCmn->DoorStatus == DOOR_CLOSED)
			{
				fnAPP_DvrDataComm(CMDCODE_TAKECARD,DVRCHAN_IDX123,5,"DOORCLOS");
			}			
		}

		HostNms.DoorStatus = m_pDevCmn->DoorStatus;
		if (HostNms.DoorStatus == DOOR_OPENED)
		{
			HostNms.OexDoorOpenErrorFlag = TRUE;				// 2006.03.21
			HostNms.OexDoorOpenSendFlag = TRUE;
			fnAPL_DvrSendDataProc("OD");						// 2005.06.15
		}
		else
		{
			HostNms.OexDoorOpenErrorFlag = FALSE;				// 2006.03.21
			HostNms.OexDoorOpenSendFlag = FALSE;
			ServiceSendFlag = TRUE;								// 2006.03.21
			fnAPL_DvrSendDataProc("CD");						// 2005.06.15
		}
	}
	else														// Cst Near & Empty Status Send Check
	if ((HostNms.ServiceStatus != ServiceStatus)	||			// Service Status Send Check
		(HostNms.ErrorStatus != ErrorStatus)		||
		(HostNms.CloseStatus != CloseStatus))					// Close Status Send Check : 2005.09.08
	{
		HostNms.ServiceStatus = ServiceStatus;
		HostNms.ErrorStatus = ErrorStatus;
		HostNms.CloseStatus = CloseStatus;						// 2005.09.11

		if ((HostNms.ServiceStatus == TRUE)		||				// 2006.03.21
			(HostNms.ErrorStatus == TRUE)		||
			(HostNms.CloseStatus == TRUE))
			HostNms.OexServiceModeSendFlag = TRUE;
		else
			HostNms.OexServiceModeSendFlag = FALSE;
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (strOpMode.CompareNoCase("OPDO") == 0)
		;
	else
	if (HostNms.DvrStatus != DvrStatus)
	{
		HostNms.DvrStatus = DvrStatus;
		HostNms.MsrSendFlag = TRUE;								// 2006.05.10
	}
	else
	if (HostNms.CduDeviceStatus != m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH))
	{
		HostNms.CduDeviceStatus = m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_CSH);
		if (HostNms.CduDeviceStatus)
			HostNms.DeviceErrorSendFlag = TRUE;
		else
			HostNms.DeviceErrorSendFlag = FALSE;
	}
	else
	if (HostNms.SprDeviceStatus != m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))
	{
		HostNms.SprDeviceStatus = m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR);
		if (HostNms.SprDeviceStatus)
			HostNms.DeviceErrorSendFlag = TRUE;
		else
			HostNms.DeviceErrorSendFlag = FALSE;
	}
	else
	if (HostNms.JprDeviceStatus != m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR))
	{
		HostNms.JprDeviceStatus = m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_JPR);
		if (HostNms.JprDeviceStatus)
			HostNms.DeviceErrorSendFlag = TRUE;
		else
			HostNms.DeviceErrorSendFlag = FALSE;
	}
	else														// 060411 读卡器故障，上送故障报文
	if (HostNms.McuDeviceStatus != m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU))
	{
		HostNms.McuDeviceStatus = m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_MCU);
		if (HostNms.McuDeviceStatus)
			HostNms.DeviceErrorSendFlag = TRUE;
		else
			HostNms.DeviceErrorSendFlag = FALSE;
	}
	else														// 060411 密码键盘故障，上送故障报文
	if (HostNms.PinDeviceStatus != m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN))
	{
		HostNms.PinDeviceStatus = m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_PIN);
		if (HostNms.PinDeviceStatus)
			HostNms.DeviceErrorSendFlag = TRUE;
		else
			HostNms.DeviceErrorSendFlag = FALSE;
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->AtmStatus == ATM_CUSTOM ||
		m_pDevCmn->AtmStatus == ATM_READY)						// 060411 钞票、纸状态改变发送状态报文
	{
		if ((HostNms.RecycleBoxStatus != m_pDevCmn->RecycleBoxStatus)	||
			(HostNms.WithAvail != m_pDevCmn->WithAvail)					||	// Withdraw Not Send Check
			(HostNms.FstCSTStatus != m_pDevCmn->FstCSTStatus)			||	// 2006.03.21
			(HostNms.SndCSTStatus != m_pDevCmn->SndCSTStatus)			||
			(HostNms.TrdCSTStatus != m_pDevCmn->TrdCSTStatus)			||
			(HostNms.FthCSTStatus != m_pDevCmn->FthCSTStatus))
		{
			HostNms.RecycleBoxStatus = m_pDevCmn->RecycleBoxStatus;
			HostNms.WithAvail = m_pDevCmn->WithAvail;
			HostNms.FstCSTStatus = m_pDevCmn->FstCSTStatus;
			HostNms.SndCSTStatus = m_pDevCmn->SndCSTStatus;
			HostNms.TrdCSTStatus = m_pDevCmn->TrdCSTStatus;
			HostNms.FthCSTStatus = m_pDevCmn->FthCSTStatus;
			
			HostNms.OexBothDenoNotesSendFlag = TRUE;
		}
		if (HostNms.SlipStatus != m_pDevCmn->SlipStatus)		
		{
			HostNms.SlipStatus = m_pDevCmn->SlipStatus;
			
			HostNms.OexSlipPaperOutSendFlag = TRUE;
		}
		if (HostNms.JnlStatus != m_pDevCmn->JnlStatus)			
		{
			HostNms.JnlStatus = m_pDevCmn->JnlStatus;

			HostNms.OexJnlPaperOutSendFlag = TRUE;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//	AGETN STATUS CHECK
/////////////////////////////////////////////////////////////////////////////
// 签到后链路断开要发一个通讯故障
	if (m_pDevCmn->bAgentMode == 1)
	{
		if (m_pDevCmn->AtmStatus == ATM_TRAN   ||
			m_pDevCmn->AtmStatus == ATM_CUSTOM ||
			m_pDevCmn->AtmStatus == ATM_READY)
		{
			if (m_pDevCmn->HostOpenFlag == FALSE)
			{
				if (nAgentAtmAfterOpenFistFailFlag == TRUE)
				{
		    		nAgentSaveErrorDeviceNum = nAgentErrorDeviceNum;
					nAgentAtmAfterOpenFistFailFlag = FALSE;
				}
			}
		}
		
		if (m_pDevCmn->AtmStatus == ATM_READY  ||
			m_pDevCmn->AtmStatus == ATM_CUSTOM )
		{
			if (nAgentBidAtmColseFlag == TRUE)                  //暂停服务后发一个运行日志
			{
				fnAPP_AgentIdxGetData(CLERK_OTHER);         
				nAgentBidAtmColseFlag = FALSE;
			}
		}
	}
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->AtmStatus == ATM_READY  ||
		m_pDevCmn->AtmStatus == ATM_CUSTOM )					// added by yusy 09.10.28
	{
		if(m_pDevCmn->bAgentMode == 1)
		{
			int bAgentMsgSendFlg = fnAPP_AgentCompareDeviceStatus(RESOURCE_STATUS);
			if(bAgentMsgSendFlg == 1)
				fnAPP_AgentIdxGetData(AGENT_ERRORMODE);
			else
			if(bAgentMsgSendFlg == 2)
				fnAPP_AgentIdxGetData(AGENT_STATUS);
		}
	}

/////////////////////////////////////////////////////////////////////////////
//	Send Procedure
/////////////////////////////////////////////////////////////////////////////
	if ((m_pDevCmn->AtmStatus == ATM_ERROR)		||				// Send Check : 2005.09.11
		(m_pDevCmn->AtmStatus == ATM_CLERK)		||
		(m_pDevCmn->AtmStatus == ATM_READY)		||
		(m_pDevCmn->AtmStatus == ATM_CUSTOM))
		;
	else
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	if (strOpMode.CompareNoCase("OPDO") == 0)
		;
	else
/////////////////////////////////////////////////////////////////////////////
	if (HostNms.RqkSendFlag)
	{
		HostNms.RqkSendFlag = FALSE;
		m_pDevCmn->fnAPL_CloseHost();							// 2005.09.11 : DO NOT MODIFY
		if (m_pProfile->NETWORK.InterfaceKind == 2500)			// POST-TY(HNC)			// 2007.05.15 : ATMU
			fnAPP_LineClose(LINE_OPCL_TIME);
	}
	else
	if (!m_pDevCmn->HostOpenFlag)								// Open Send
	{
		fnAPP_MainOpenProc();
	}
	else
	if ((HostNms.AexCustomerCardTimeOutSendFlag)	||				// Aex Send
		(HostNms.AexCustomerHostRetractSendFlag)	||
		(HostNms.AexCustomerCashTimeOutSendFlag))
	{
		fnAPP_MainTranProc(TRAN_AEXSTATUS);
	}
	else
/////////////////////////////////////////////////////////////////////////////
	if (HostNms.CwcSendFlag)									// Cwc Send
	{
		if (m_pDevCmn->Timer.HostCancelRetryTime)				// Cancel Wait
			;
		else
		{
			HostNms.CwcSendFlag = FALSE;
// 			if ((IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1) ||
// 				(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3) ||
// 				(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 5))		// 2008.10.13
			if (CwcInfo.CwcFlag != 0)							// added by liuxl 20110714
			{
				fnAPP_MainTranProc(TRAN_CANCEL);
// 				if ((IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 1) ||
// 					(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 3) ||
// 					(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHFLAG) == 5))		// 2008.10.13
				if (CwcInfo.CwcFlag != 0)						// added by liuxl 20110714
				{
					HostNms.CwcSendFlag = TRUE;

					m_pDevCmn->HostCancelRetryCnt++;
//					if (m_pDevCmn->HostCancelRetryCnt >= InitInfo.CwcRetryCnt)
//					{
//						m_pDevCmn->Timer.HostCancelRetryTime = InitInfo.CwcRetryTime;
//						m_pDevCmn->fnAPL_CloseHost();			// 2005.09.11 : DO NOT MODIFY
//					}
//					else
						m_pDevCmn->Timer.HostCancelRetryTime = InitInfo.CwcRetryTime;
				}
				else
				{
					m_pDevCmn->HostCancelRetryCnt = 0;
					m_pDevCmn->Timer.HostCancelRetryTime = InitInfo.CwcRetryTime;
				}
			}
		}
	}
	else
	if (CardRetainData.CardRetFlag != 0)
		fnAPP_MainTranProc(TRAN_CARDRETAIN);
// 	else
//	if (HostNms.OexSendFlag)									// Oex Send
//	{	
//		HostNms.OexSendFlag = FALSE;
//		m_pDevCmn->fnAPL_AddDeviceExecOffInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR));
//		fnAPP_MainTranProc(TRAN_ERROR);
//		if(m_pDevCmn->bAgentMode == 1)
//		{
//			fnAPP_AgentIdxGetData(AGENT_ERRORMODE);
//		}
//		if (m_pDevCmn->DeviceRetract & DEV_MCU)					// 2006.03.21
// 			m_pDevCmn->DeviceRetract &= ~DEV_MCU;
// 	}
	else
	if ((HostNms.OexBothDenoNotesSendFlag)		||				// Oex Send
		(HostNms.OexPowerOnSendFlag)			||
		(HostNms.OexCloseBySpvSendFlag)			||
		(HostNms.OexOpenBySpvSendFlag)			||
		(HostNms.OexSlipPaperOutSendFlag)		||				
		(HostNms.OexJnlPaperOutSendFlag)		||
		(HostNms.OexDoorOpenErrorFlag)			||				// 2006.03.21
		(HostNms.OexDoorOpenSendFlag)			||
		(HostNms.OexServiceModeSendFlag))
	{	
		fnAPP_MainTranProc(TRAN_MSRSTATUS);						// liyi 2009.03.17 上送故障报文改为状态报文
		if(m_pDevCmn->bAgentMode == 1)
		{
			m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_STATUS);
		}
	}
	else
	if (HostNms.MsrSendFlag)									// Msr Send
	{
		fnAPP_MainTranProc(TRAN_MSRSTATUS);
	}
	// added by yusy ic 脚本通知判断
	else
	if (IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM") != "" &&
		Asc2Int(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION_SCRIPTLIST, "SCRIPTNUM")) > 0)
	{
		fnAPP_MainTranProc(TRAN_SCRIPT);
	}
	else
	if (HostNms.DeviceErrorSendFlag)
	{
		HostNms.DeviceErrorSendFlag = FALSE;
		m_pDevCmn->fnAPL_AddDeviceExecOffInfo(IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANERROR));
		fnAPP_MainTranProc(TRAN_ERROR);							
		if(m_pDevCmn->bAgentMode == 1)
		{
			fnAPP_AgentIdxGetData(AGENT_ERRORMODE);
		}
	}
//	else
/////////////////////////////////////////////////////////////////////////////
//	Nms Procedure
/////////////////////////////////////////////////////////////////////////////
//	if (fnAPP_NmsRecvData(NmsRecvBuffer, &NmsRecvLength, 0) == T_OK)
//	{															// DO NOT MODIFY : 2005.09.11
//		fnAPP_MainTranProc(TRAN_BIDCOMMAND);					// 2009.03.05
//	}
//	else
//	if (memcmp(m_pProfile->TRANS.YYYYMMDD, GetDate().GetBuffer(0), 8) != 0)
//	{															// DO NOT MODIFY : 2005.09.02
//		HostNms.RqkSendFlag = TRUE;
//	}

//////////////////////////////////////////////////////////////////根据下载标识进行相应的下载   by zjd
//	if (DownloadParmFlag)										// 2009.03.18
//	{
//		fnAPP_MainTranProc(TRAN_DOWNPARM);
//	}
//
//	if (DownloadFITFlag)
//	{
//		fnAPP_MainTranProc(TRAN_DOWNFIT);
//	}
//
//	if (DownloadPayFlag)
//	{
//		fnAPP_MainTranProc(TRAN_DOWNPAY);
//	}

	// added by liuxl 20110628
	int msflag = IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_MAINTAINSTATUSFLAG, 1);
	
	if (strOpMode.CompareNoCase("OPDO") == 0 && msflag == 1)
	{
		if (HostNms.MsrSendFlag)									// Msr Send
		{
			fnAPP_MainTranProc(TRAN_MSRSTATUS);
		}
	}

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Send/Recv Procedure
/////////////////////////////////////////////////////////////////////////////
// Line Open
int CTranCmn::fnAPP_LineOpen(int OpenSec, int nAsyncFlag)
{
	if (m_pDevCmn->fnNET_Initialize(OpenSec, nAsyncFlag))
		return T_OK;
	else 
		return T_TIMEOVER;
}

// Line Close
int CTranCmn::fnAPP_LineClose(int CloseSec, int nAsyncFlag)
{
	if (m_pDevCmn->fnNET_Deinitialize(CloseSec, nAsyncFlag))
		return T_OK;
	else 
		return T_TIMEOVER;
}

// Send Data
int CTranCmn::fnAPP_SendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlag)
{
	if (RecvBuffClearFlag)
		fnAPP_RecvDataClear();

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Op.SendRecvStatus = "ON";						// SendRecvStatus(ON)
	m_pDevCmn->fnAPL_ClerkInformation();						// Clerk Information
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->fnNET_SendData(MakeUnPack(SendBuff, Length), SendSec))
		return T_OK;
	else 
		return T_TIMEOVER;
}

// Recv Data Clear
int CTranCmn::fnAPP_RecvDataClear()
{
	BYTE	ClearBuff[NETBUFFSIZE];
	int		ClearLength;

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
		return T_OK;

/////////////////////////////////////////////////////////////////////////////
	while (fnAPP_RecvData(ClearBuff, &ClearLength, 0) != T_TIMEOVER);
	return T_OK;
}
	
// Recv Data
int CTranCmn::fnAPP_RecvData(BYTE* RecvBuff, int* Length, int RecvSec)
{
	CString	strRecvData("");
	int		nRecvResult = 0;
	BYTE	TempBuff[NETBUFFSIZE];
	int		TempLength = 0;

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
	{
		Delay_Msg(500);											// FLASH Modify?
		return T_OK;											// RECV OK
	}

/////////////////////////////////////////////////////////////////////////////
	do
	{
		nRecvResult = m_pDevCmn->fnNET_RecvData(strRecvData, RecvSec);
		TempLength = MakePack(strRecvData.GetBuffer(0), TempBuff, strRecvData.GetLength());

		if (nRecvResult == FALSE)
		{
			*Length = 0;
			return T_TIMEOVER;
		}

		if (!TempLength)
		{
			*Length = 0;
			return T_RECVERROR;
		}

		memcpy(RecvBuff, TempBuff, TempLength);
		*Length = TempLength;

		if ((*Length) > 21)										// Data Size Check(Header Base)
			return T_OK;										// Ap Data Ok
		else TempLength = 0;									// Data Ignore

	} while(TRUE);
		
	return T_OK;
}

// Recv In Tbl
int CTranCmn::fnAPP_RecvInTbl(BYTE* Buff, int Length)
{
	int		NmsRecvInTblTemp;

	NmsRecvInTblTemp = NmsRecvInTbl;
	NmsRecvInTblTemp++;
	if (NmsRecvInTblTemp >= NMSRECVTBL)							// In Tbl Temp Adj.
		NmsRecvInTblTemp = 0;

	if (NmsRecvInTblTemp != NmsRecvOutTbl)						// Data Tbl Ok
	{
		memcpy(NmsRecvBufferTbl[NmsRecvInTbl], Buff, Length);
		NmsRecvLengthTbl[NmsRecvInTbl] = Length;

		NmsRecvInTbl = NmsRecvInTblTemp;						// In Tbl Adj.
	}

	return T_OK;
}

// Recv Out Tbl
int CTranCmn::fnAPP_RecvOutTbl(BYTE* Buff, int* Length)
{
	if (NmsRecvInTbl == NmsRecvOutTbl)							// No Data
	{
		*Length = 0;
		return T_TIMEOVER;
	}

	memcpy(Buff, NmsRecvBufferTbl[NmsRecvOutTbl], NmsRecvLengthTbl[NmsRecvOutTbl]);
	*Length = NmsRecvLengthTbl[NmsRecvOutTbl];

	NmsRecvOutTbl++;
	if (NmsRecvOutTbl >= NMSRECVTBL)							// Out Tbl Adj.
		NmsRecvOutTbl = 0;

	if (!(*Length))
		return T_TIMEOVER;

	return T_OK;
}

// Nms Send Data
int CTranCmn::fnAPP_NmsSendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlag)
{
	CString strTmp("");
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->Op.SendRecvStatus = "ON";						// SendRecvStatus(ON)
	m_pDevCmn->fnAPL_ClerkInformation();						// Clerk Information
/////////////////////////////////////////////////////////////////////////////

	strTmp.Format("%2.2s:%2.2s:%2.2s [TCPIP BID SENDDATA], Length : [%d]",
								GetTime().GetBuffer(0),
								&(GetTime().GetBuffer(0)[2]),
								&(GetTime().GetBuffer(0)[4]),
								Length);
	m_pDevCmn->fnCMN_SaveLineTrace(strTmp.GetBuffer(0), strTmp.GetLength());
	m_pDevCmn->fnCMN_SaveLineTrace(SendBuffer, Length, 1);
	
	if (m_pDevCmn->fnNET_SendData3(MakeUnPack(SendBuff, Length), SendSec))
		return T_OK;
	else 
		return T_TIMEOVER;
}

// Nms Recv Data
int CTranCmn::fnAPP_NmsRecvData(BYTE* RecvBuff, int* Length, int RecvSec)
{
	CString	strRecvData("");
	int		nRecvResult = 0;
	BYTE	TempBuff[NETBUFFSIZE];
	int		TempLength = 0;
	CString strTmp("");

////////////////////////////////////////////////////////主机管理命令单独的线程处理，这里直接就返回了。  by zjd
	return T_RECVERROR;

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// Host Local Mode
		return T_RECVERROR;										// RECV OK

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->fnNET_GetDeviceStatus3() != LINE_OPEN)
		return T_RECVERROR;

/////////////////////////////////////////////////////////////////////////////
	do
	{
		nRecvResult = m_pDevCmn->fnNET_RecvData3(strRecvData, RecvSec);
		TempLength = MakePack(strRecvData.GetBuffer(0), TempBuff, strRecvData.GetLength());

		if (nRecvResult == FALSE)
		{
			*Length = 0;
			return T_TIMEOVER;
		}

		if (!TempLength)
		{
			*Length = 0;
			return T_RECVERROR;
		}

		memcpy(RecvBuff, TempBuff, TempLength);
		*Length = TempLength;

		if ((*Length) >= 4)	   // Data Size Check(Header Base) : 2006.07.03
		{
			strTmp.Format("%2.2s:%2.2s:%2.2s [TCPIP BID RECVDATA], Length : [%d]",
			GetTime().GetBuffer(0),
			&(GetTime().GetBuffer(0)[2]),
			&(GetTime().GetBuffer(0)[4]),
			TempLength);
		    m_pDevCmn->fnCMN_SaveLineTrace(strTmp.GetBuffer(0), strTmp.GetLength());
		    m_pDevCmn->fnCMN_SaveLineTrace(RecvBuff, TempLength, 1);
	
			return T_OK;       // Nms Data Ok
		}
		else TempLength = 0;									// Data Ignore

	} while(TRUE);
		
	return T_OK;
}



