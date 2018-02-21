 /////////////////////////////////////////////////////////////////////////////
// TranOutput.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "iostream.h"
#include "fstream.h" 


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
//	After Recv Function
/////////////////////////////////////////////////////////////////////////////
// Parameter Slip Print Procedure
int CTranCmn::fnAPP_PSPProc(int PrintDevice, int WaitFlag)
{
	int		HiddenFlag = TRUE;
	int		nJnlIndex = 0;
	int		nSlipIndex = 0;

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
	{
	}

/////////////////////////////////////////////////////////////////////////////
	if (PrintDevice & (DEV_SPR | DEV_JPR))
	{
		if (!Accept.SlipSelectFlag)								// Slip Skip
			PrintDevice &= ~DEV_SPR;
		
		if (PrintDevice == DEV_NONE)
			return T_OK;
	}

	if (MaterialRetractFlag & DEV_MCU)							// 2004.11.05
		HiddenFlag = FALSE;
	else
	if (HostResp.CardRetractFlag)
		HiddenFlag = FALSE;

	if (PrintDevice & DEV_SPR)
	{
		memset(ShuData, 0, sizeof(ShuData));
	
	
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex +  8],  8, HiddenFlag); // by linji 20070720
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 11], 11, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 13], 13, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 16], 16, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 18], 18, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 21], 21, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 24], 24, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 26], 26, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 28], 28, HiddenFlag);

//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex +  8],  8, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 10], 10, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 13], 13, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 16], 16, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 18], 18, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 20], 20, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 23], 23, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 25], 25, HiddenFlag);

		fnAPD_ShuPrint(DEV_SPR, WaitFlag);
	}

//	if (PrintDevice & DEV_JPR) 
//	{
//		memset(ShuData, 0, sizeof(ShuData));
//	
//		HiddenFlag = FALSE;										// 2005.03.10
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex +  7],  7, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 10], 10, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 13], 13, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 15], 15, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 18], 18, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 20], 20, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 23], 23, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 24], 24, HiddenFlag);
//
//		fnAPD_ShuPrint(DEV_JPR, WaitFlag);
//	}

	return T_OK;
}

// Parameter Passbook Print Procedure
int CTranCmn::fnAPP_PPPProc(int WaitFlag)
{
	return T_OK;
}

// Parameter Passbook Write Procedure
int CTranCmn::fnAPP_PPWProc(int WaitFlag)
{
	return T_OK;
}

// Parameter Card Write Procedure
int CTranCmn::fnAPP_PCWProc(int WaitFlag)
{
	return T_OK;
}

// Parameter Message Display Procedure
int CTranCmn::fnAPP_PMDProc()
{
	int		ScrNum = 702;

	if (!PMDRecvData.ParaLength)
		return T_OK;

	for (int i = 0; i <= PMDRecvData.Ey; i++)
		m_pDevCmn->fnSCR_DisplayString(i + 1, PMDRecvData.Data[i]);

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
	if ((HostResp.CardRetractFlag)	||							// Card Message Delay
		(HostResp.CardEjectFlag))
		Delay_Msg(5000);

	return T_OK;
}

// PME Accept Ng Guide Confirm
int CTranCmn::fnAPP_PMEAcceptNgGuideConfirm()
{
	int		ScrNum = 403;
	CString strReturn("");

	if (!PMDRecvData.ParaLength)
		return T_OK;

	for (int i = 0; i <= PMDRecvData.Ey; i++)
		m_pDevCmn->fnSCR_DisplayString(i + 1, PMDRecvData.Data[i]);
	
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

	return T_OK;
}

// PME Accept Slip Select
int CTranCmn::fnAPP_PMEAcceptSlipSelect()
{

	int		ScrNum = 404;
	CString strReturn("");
	CString cstrTemp("");

/////////////////////////////////////////////////////////////////////////////
	Accept.SlipSelectFlag = TRUE;								// Slip Select Flag
/////////////////////////////////////////////////////////////////////////////
	
	if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||		// 2005.04.19
		(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
		(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
		(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
	{
		Accept.SlipSelectFlag = FALSE;
		return T_OK;
	}
	
	if (TranCode == TC_HZT)										//liyi add 绿卡通
	{
		m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%4.4s",&HMERecv.Data[103][2]));
		m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f",Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
		if (m_pTranCmn->strTFRSubType == "7")
		{
			cstrTemp = "整整";
		}
		else if (m_pTranCmn->strTFRSubType == "8")
		{
			cstrTemp = "定活";
		}
		else if (m_pTranCmn->strTFRSubType == "9")
		{
			cstrTemp = "通知";
		}
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s",cstrTemp));
		if (m_pTranCmn->strTFRInData == "03")
		{
			cstrTemp = "三个月";
		}
		else if (m_pTranCmn->strTFRInData == "06")
		{
			cstrTemp = "半年";
		}
		else if (m_pTranCmn->strTFRInData == "12")
		{
			cstrTemp = "一年";
		}
		else if (m_pTranCmn->strTFRInData == "24")
		{
			cstrTemp = "二年";
		}
		else if (m_pTranCmn->strTFRInData == "36")
		{
			cstrTemp = "三年";
		}
		else if (m_pTranCmn->strTFRInData == "60")
		{
			cstrTemp = "五年";
		}
		else if (m_pTranCmn->strTFRInData == "01")
		{
			cstrTemp = "一天";
		}
		else if (m_pTranCmn->strTFRInData == "07")
		{
			cstrTemp = "七天";
		}
		else if (m_pTranCmn->strTFRInData == "00")
		{
			cstrTemp = "";
		}
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%s",cstrTemp));
		m_pDevCmn->fnSCR_DisplayScreen(48, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
	}
	else if (TranCode == TC_TZH)
	{
		m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s",m_pTranCmn->strTFRSubNo));
		m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f",Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f",Asc2Float(&HMERecv.Data[123][3],8)/100));
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%.2f",Asc2Float(&HMERecv.Data[123][11],8)/100));
//		if (m_pTranCmn->strTFRSubType == "7")
//		{
//			cstrTemp = "整整";
//		}
//		else if (m_pTranCmn->strTFRSubType == "8")
//		{
//			cstrTemp = "定活";
//		}
//		else if (m_pTranCmn->strTFRSubType == "9")
//		{
//			cstrTemp = "通知";
// 		}
		if (m_pTranCmn->strTFRSubType == "1")
		{
			cstrTemp = "整整";
		}
		else if (m_pTranCmn->strTFRSubType == "3")
		{
			cstrTemp = "定活";
		}
		else if (m_pTranCmn->strTFRSubType == "2")
		{
			cstrTemp = "通知";
 		}
		m_pDevCmn->fnSCR_DisplayString(5, GetSprintf("%s",cstrTemp));
		if (m_pTranCmn->strTFRInData == "03")
		{
			cstrTemp = "三个月";
		}
		else if (m_pTranCmn->strTFRInData == "06")
		{
			cstrTemp = "半年";
		}
		else if (m_pTranCmn->strTFRInData == "12")
		{
			cstrTemp = "一年";
		}
		else if (m_pTranCmn->strTFRInData == "24")
		{
			cstrTemp = "二年";
		}
		else if (m_pTranCmn->strTFRInData == "36")
		{
			cstrTemp = "三年";
		}
		else if (m_pTranCmn->strTFRInData == "60")
		{
			cstrTemp = "五年";
		}
		else if (m_pTranCmn->strTFRInData == "01")
		{
			cstrTemp = "一天";
		}
		else if (m_pTranCmn->strTFRInData == "07")
		{
			cstrTemp = "七天";
		}
		else if (m_pTranCmn->strTFRInData == "00")
		{
			cstrTemp = "";
		}
		m_pDevCmn->fnSCR_DisplayString(6, GetSprintf("%s",cstrTemp));			
		m_pDevCmn->fnSCR_DisplayScreen(51, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
		
		
	}
	else
	{
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
	}
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_YES)
		Accept.SlipSelectFlag = TRUE;
	else
	if (strReturn == S_NO)
		Accept.SlipSelectFlag = FALSE;
	else
	if (strReturn == S_EXIT)
		fnAPP_CancelProc(T_EXIT);
	else
	if (strReturn == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);							// 2005.08.22
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
		fnAPP_CancelProc(T_PROGRAM);		

	return T_OK;
}

// PME Display Transfer Procedure
int CTranCmn::fnAPP_PMEDisplayTransferProc()
{
	int		ScrNum = 0;

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
	Delay_Msg(3000);

	return T_OK;
}

// PME Accept Transaction Continue
int CTranCmn::fnAPP_PMEAcceptTranContinue()
{
	int		ScrNum = 410;		// 是否继续交易提示  by zjd
	CString strReturn("");
	
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
	
	return T_OK;
}

// PME Accept A/C MANAGER Remit Continue
int CTranCmn::fnAPP_PMEAcceptACMangerContinue()
{
	int	ScrNum;
	CString strReturn("");
		
	if( ACTranCode == TC_AC_ASSU_TRAN || ACTranCode == TC_AC_NOASSU_PAY)
	{
		if(TranCode == TC_AC_REMIT_PWD)
		   ScrNum = 432;                    // 密码汇款提示继续第12屏
		else
		   ScrNum = 431;	             	// 汇款提示是否继续第12屏
		
	/////////////////////////////////////////////////////////////////////////////
		Accept.SlipSelectFlag = TRUE;								// Slip Select Flag
	/////////////////////////////////////////////////////////////////////////////
		
		if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||		// 2005.04.19
			(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
			(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
			(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
		{
			Accept.SlipSelectFlag = FALSE;
		//	return T_OK;
		}
		
		if( ACHostResp.ReturnCode == 0 )                 //交易成功
 		{
			if(ScrNum == 431)
			{
			    m_pDevCmn->fnSCR_DisplayString(1,"1");   //成功或失败标志
				
			}
			else
			if(ScrNum == 432)
			{
			   m_pDevCmn->fnSCR_DisplayString(2,"1");   //成功或失败标志
			   m_pDevCmn->fnSCR_DisplayString(1,ACHostResp.PostalOrderNo); 
			   
			}	

			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		    if (strReturn == S_YES )
			{
	//			Accept.SlipSelectFlag = TRUE;
				fnAPP_PSPProc(DEV_SPR);								// Spr Print
				fnAPD_CheckDeviceAction(DEV_SPR);
				fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);  // Spr Out
				fnAPD_MaterialOutCheckRetract(DEV_SPR);				   // Spr Out Check & Retract
				
				MainTranReturnFlag = TRUE;
				return T_OK;			
				   	
			}
			else
			if (strReturn == S_NO)
			{
				Accept.SlipSelectFlag = FALSE;	
				
				MainTranReturnFlag = TRUE;
				return T_OK;
			}
			else
			if (strReturn == S_EXIT)
				fnAPP_CancelProc(T_EXIT);
			else
			if (strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else
			if (strReturn == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);							// 2005.08.22
			else
			if (strReturn == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);
			else
				fnAPP_CancelProc(T_PROGRAM);
		
		}
		else                                   //交易失败   
		{
			m_pDevCmn->fnSCR_DisplayString(1,"0");   //成功或失败标志  
			m_pDevCmn->fnSCR_DisplayString(3,ACHostResp.ReturnMess);   //显视账户管家返回错误信息 
    		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		    if (strReturn == S_YES )
			{
	//			Accept.SlipSelectFlag = TRUE;
				fnAPP_PSPProc(DEV_SPR);								// Spr Print
				fnAPD_CheckDeviceAction(DEV_SPR);
				fnAPD_MaterialOut(DEV_SPR, InitInfo.TakeSlipTimeOut);  // Spr Out
				fnAPD_MaterialOutCheckRetract(DEV_SPR);				   // Spr Out Check & Retract

				MainTranReturnFlag = TRUE;
				return T_OK;
				   	
			}
			else
			if (strReturn == S_NO)
			{
				Accept.SlipSelectFlag = FALSE;

				MainTranReturnFlag = TRUE;
				return T_OK;
			}
			else
			if (strReturn == S_EXIT)
				fnAPP_CancelProc(T_EXIT);
			else
			if (strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else
			if (strReturn == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);							// 2005.08.22
			else
			if (strReturn == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);
			else
				fnAPP_CancelProc(T_PROGRAM);

 		}
		
	}
	else    //若不是约定交易5317和非约定缴费5327，则为交易返回错误提示
	{
		if(ACTranCode == TC_AC_CUSMESS_INQ && ACHostResp.ReturnCode == 165)   //5505查询    返回165账户管家未开通  by hyl
		{
			ScrNum = 426; 
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	//第6屏提示管家婆业务未开通
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
			
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
			if (strReturn == S_CONTINUE)  //处理返回，返回主交易画面  
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
		}
		else
		{
            ScrNum = 703;
			m_pDevCmn->fnSCR_DisplayString(1,ACHostResp.ReturnMess);       
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();

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
			if (strReturn == S_CONTINUE)  //处理返回，返回主交易画面  
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
			
		}
	}

	return T_OK;
}


// PME Accept Inquiry Confirm
int CTranCmn::fnAPP_PMEAcceptInquiryConfirm()
{
//	int		ScrNum = 400;                            //20080906 by yaokq
	int		ScrNum;
	CString	strCurrency("");
	CString	strReturn("");
	
	if (m_pDevCmn->HostLineMode	== HOST_LOCAL_MODE)
	{
		m_pDevCmn->fnSCR_DisplayString(1, "CNY1234567890.00");
		m_pDevCmn->fnSCR_DisplayString(2, "CNY1234567890.00");
		m_pDevCmn->fnSCR_DisplayString(3, "CNY1234567890.00");
	}
	else
	{
		if (memcmp(HMERecv.Data[49], "156", 3) == 0)
			strCurrency = "RMB ";
		else
		if (memcmp(HMERecv.Data[49], "840", 3) == 0)
			strCurrency = "USD ";
		else
		if (memcmp(HMERecv.Data[49], "978", 3) == 0)
			strCurrency = "EUR ";
		else
		if (memcmp(HMERecv.Data[49], "724", 3) == 0)
			strCurrency = "ESP ";
		else
		if (memcmp(HMERecv.Data[49], "250", 3) == 0)
			strCurrency = "FRF ";
		else
		if (memcmp(HMERecv.Data[49], "380", 3) == 0)
			strCurrency = "ITL ";
		else
		if (memcmp(HMERecv.Data[49], "810", 3) == 0)
			strCurrency = "RUR ";
		else
		if (memcmp(HMERecv.Data[49], "826", 3) == 0)
			strCurrency = "GBP ";
		else
		if (memcmp(HMERecv.Data[49], "036", 3) == 0)
			strCurrency = "AUD ";
		else
		if (memcmp(HMERecv.Data[49], "392", 3) == 0)
			strCurrency = "JPY ";
		else
		if (memcmp(HMERecv.Data[49], "410", 3) == 0)
			strCurrency = "KRW ";

		if ((HMERecv.Data[54][0] == 'D') ||
			(HMERecv.Data[54][0] == 'd'))
		{
			m_pDevCmn->fnSCR_DisplayString(1, 
				GetSprintf("%s%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][1], 12) / 100));
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(1, 
				GetSprintf("%s-%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][1], 12) / 100));
		}

//		if ((HMERecv.Data[54][13] == 'D') ||
//			(HMERecv.Data[54][13] == 'd'))
//		{
//			m_pDevCmn->fnSCR_DisplayString(2, 
//				GetSprintf("%s-%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][14], 12) / 100));
//		}
//		else
//		{
//			m_pDevCmn->fnSCR_DisplayString(2, 
//				GetSprintf("%s%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][14], 12) / 100));
//		}
//
		if ((HMERecv.Data[54][26] == 'D') ||
			(HMERecv.Data[54][26] == 'd'))
		{
			m_pDevCmn->fnSCR_DisplayString(2, 
				GetSprintf("%s%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][27], 12) / 100));
		}
		else
		{
			m_pDevCmn->fnSCR_DisplayString(2, 
				GetSprintf("%s-%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][27], 12) / 100));
		}

		if ((Asc2Float(&HMERecv.Data[54][27], 12) / 100) > InitInfo.WithDayMaxAmt)
			m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s%.2f", strCurrency, InitInfo.WithDayMaxAmt));		// 单日最大取款限额  by zjd
		else
			m_pDevCmn->fnSCR_DisplayString(3, 
			GetSprintf("%s%.2f", strCurrency, Asc2Float(&HMERecv.Data[54][27], 12) / 100));
	}

	//20080906 by yaokq
	if(LocalCardFlag == 7)   //本省绿卡通副卡
		ScrNum  = 434;				
	else if(LocalCardFlag == 4)						 //本行信用卡
		ScrNum = 435;
	else											 //其它的卡
		ScrNum = 400;

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

	return T_OK;
}

// PME Accept Detail Confirm
int CTranCmn::fnAPP_PMEAcceptDetailConfirm()
{
	int		ScrNum = 401;
	int		nDetailCnt = 0;
	CString	strReturn("");
	CString strCurrency("");
	char	szTempBuff[256];
	double	fTmp = 0.0;

	memset(szTempBuff, 0, sizeof(szTempBuff));

	if (m_pDevCmn->HostLineMode	== HOST_LOCAL_MODE)
	{
//		m_pDevCmn->fnSCR_DisplayString(1,  GetSprintf("%19.19s", CardAccountNo));
		m_pDevCmn->fnSCR_DisplayString(1,  "01020050801CNYC000001000000D1234567890000200000000");
		m_pDevCmn->fnSCR_DisplayString(2,  "00120050810CNYC000000100000D1234567890000200111111");
		m_pDevCmn->fnSCR_DisplayString(3,  "00220050809CNYC000000200000D1234567890000200222222");
		m_pDevCmn->fnSCR_DisplayString(4,  "00320050808CNYC000000300000D1234567890000200333333");
		m_pDevCmn->fnSCR_DisplayString(5,  "00420050807CNYC000000400000D1234567890000200444444");
		m_pDevCmn->fnSCR_DisplayString(6,  "00520050806CNYC000000500000D1234567890000200555555");
		m_pDevCmn->fnSCR_DisplayString(7,  "00620050805CNYC000000600000D1234567890000200666666");
		m_pDevCmn->fnSCR_DisplayString(8,  "00720050804CNYC000000700000D1234567890000200777777");
		m_pDevCmn->fnSCR_DisplayString(9,  "00820050803CNYC000000800000D1234567890000200888888");
		m_pDevCmn->fnSCR_DisplayString(10, "00920050802CNYC000000900000D1234567890000200999999");
	}
	else
	{
		nDetailCnt = Asc2Int(&HMERecv.Data[123][6], 2);
		if (nDetailCnt > 10)
			nDetailCnt = 10;

		for (int i = 0; i < nDetailCnt; i++)
		{


			m_pDevCmn->fnSCR_DisplayString(i*4 + 1, GetSprintf("%8.8s", HMERecv.Data[123] + 8 + (i * 82)).GetBuffer(0), 8);
			m_pDevCmn->fnSCR_DisplayString(i*4 + 2, GetSprintf("%4.4s", HMERecv.Data[123] + 8 + (i * 82) + 45).GetBuffer(0), 8);
		
			fTmp = Asc2Float(HMERecv.Data[123] + 8 + (i * 82) + 50, 11) / 100;
			if (HMERecv.Data[123][8 + (i * 82) + 49] == '-')
							fTmp *= -1;
			m_pDevCmn->fnSCR_DisplayString(i*4 + 3, GetSprintf("%13.2f", fTmp).GetBuffer(0), 13);

			fTmp = Asc2Float(HMERecv.Data[123] + 8 + (i * 82) + 62, 11) / 100;
			if (HMERecv.Data[123][8 + (i * 82) + 61] == '-')
							fTmp *= -1;
			m_pDevCmn->fnSCR_DisplayString(i*4 + 4, GetSprintf("%13.2f", fTmp).GetBuffer(0), 13);

		
			           
		}
	}
	
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

	return T_OK;
}


//PME Accept OTHER_INQ Confirm   //20080910 by yaokq
int CTranCmn::fnAPP_PMEAcceptOtherConfirm()
{
	int		ScrNum = 433;
	int		nOtherCurCnt = 0;
	CString	strReturn("");

	char	OtherInq[999*36];
	memset(OtherInq,0,sizeof(OtherInq));

	
	if (m_pDevCmn->HostLineMode	== HOST_LOCAL_MODE)
	{	
		while(TRUE)
		{
			m_pDevCmn->fnSCR_DisplayString(1,  "001");
			m_pDevCmn->fnSCR_DisplayString(2,  "整整");
			m_pDevCmn->fnSCR_DisplayString(3,  "1000.00");
			m_pDevCmn->fnSCR_DisplayString(4,  "1年");
			m_pDevCmn->fnSCR_DisplayString(5,  "20070101");
			m_pDevCmn->fnSCR_DisplayString(6,  "002");
			m_pDevCmn->fnSCR_DisplayString(7,  "定活");
			m_pDevCmn->fnSCR_DisplayString(8,  "2000.00");
			m_pDevCmn->fnSCR_DisplayString(9,  "");
			m_pDevCmn->fnSCR_DisplayString(10,  "20070101");

			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
			if(strReturn == S_EXIT)
				fnAPP_CancelProc(T_EXIT);
			else if(strReturn == S_CONTINUE)
				return T_OK;
			else if(strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else if(strReturn == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);
			else if (strReturn == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);
			else if(strReturn == S_Previous)
				continue;
			else if(strReturn == S_NEXT)
				continue;
			else
				fnAPP_CancelProc(T_PROGRAM);
			return T_OK;
		}
	}
	else
	{

		ZzhTotalNum = Asc2Int(&HMERecv.Data[123][3],3);
		nOtherCurCnt = Asc2Int(&HMERecv.Data[123][6],2);
		

		memcpy(&OtherInq[ZzhCurNum*27],&(HMERecv.Data[123][8]),nOtherCurCnt*27);
		ZzhCurNum += nOtherCurCnt;
		while(ZzhCurNum < ZzhTotalNum)
		{
				fnAPPF_SendHost();
				if(m_nRetCode != RET_NEXT_STEP)
					fnAPP_CancelProc(T_PROGRAM);

				fnAPPF_RecvHost();
				if(m_nRetCode != RET_NEXT_STEP)
					fnAPP_CancelProc(T_PROGRAM);
			
				fnAPPF_GetHostResult();
				if(m_nRetCode == RET_HOST_OK)	
				{
					fnAPPF_SetProcedureCount5();
					if(m_nRetCode != RET_NEXT_STEP)
						fnAPP_CancelProc(T_PROGRAM);	
				}
				else if(m_nRetCode == RET_HOST_NG)
				{
					fnAPPF_SetProcedureCount9();
					if(m_nRetCode != RET_NEXT_STEP)
						fnAPP_CancelProc(T_PROGRAM);
				}
				else
					fnAPP_CancelProc(T_PROGRAM);
								
			nOtherCurCnt = Asc2Int(&HMERecv.Data[123][6],2);
			memcpy(&OtherInq[ZzhCurNum*27],&(HMERecv.Data[123][8]),nOtherCurCnt*27);
			ZzhCurNum += nOtherCurCnt;
		}

		int j = 0;
		int	nCurNum = ZzhCurNum;

		while (TRUE)
		{	

			for(int i = 0; i<__min(5,nCurNum);i++)
			{
					m_pDevCmn->fnSCR_DisplayString(i*5 + 1,&OtherInq[(i+j) * 27],4);

/************tmp invalidate by yaokq for test 20080912 **************************
					if(OtherInq[(i+j)*27 + 4] == '7')
						m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"整整");
					else if(OtherInq[(i+j)*27 + 4] == '8')
						m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"定活");
					else if(OtherInq[(i+j)*27 + 4] == '9')
						m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"通知");
					else
							m_pDevCmn->fnSCR_DisplayString(i*5 + 2,&OtherInq[(i+j) * 27 + 4],1);
********************************************************************************/
// tmp add by yaokq for test 20080912
					if(OtherInq[(i+j)*27 + 4] == '1')
						m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"整整");
					else if(OtherInq[(i+j)*27 + 4] == '3')
						m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"定活");
					else if(OtherInq[(i+j)*27 + 4] == '2')
						m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"通知");
					else
						m_pDevCmn->fnSCR_DisplayString(i*5 + 2,&OtherInq[(i+j) * 27 + 4],1);					


/////////////////////////////////////////////////////////////////////////////////////


//					m_pDevCmn->fnSCR_DisplayString(i*5 + 2,&OtherInq[(i+j) * 27 + 4],1);
					//20080909 by yaokq
					m_pDevCmn->fnSCR_DisplayString(i*5 + 3,GetSprintf("%d.%.2d",Asc2Int(&OtherInq[(i+j) * 27 + 5],12)/100,Asc2Int(&OtherInq[(i+j) * 27 + 5],12)%100));
				//	m_pDevCmn->fnSCR_DisplayString(i*5 + 3,&OtherInq[(i+j) * 27 + 5],12);
					if(memcmp(&OtherInq[(i+j) * 27 + 17],"03",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"三个月");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"06",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"六个月");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"12",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"一年");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"24",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"二年");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"36",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"三年");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"60",2) == 0)				
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"五年");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"01",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"一天");
					else if(memcmp(&OtherInq[(i+j) * 27 + 17],"07",2) == 0)
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"七天");
					else
						m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"");

//					m_pDevCmn->fnSCR_DisplayString(i*5 + 4,&OtherInq[(i+j) * 27 + 17],2);
					m_pDevCmn->fnSCR_DisplayString(i*5 + 5,&OtherInq[(i+j) * 27 + 19],8);

			}
			j += 5;
			nCurNum -= 5;

			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
			if(strReturn == S_EXIT)
				fnAPP_CancelProc(T_EXIT);
			else if(strReturn == S_CONTINUE)
				return T_OK;
			else if(strReturn == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else if(strReturn == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);
			else if (strReturn == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);
			else if(strReturn == S_Previous)
			{
				if(j == 5)
				{
					j -= 5;
					nCurNum += 5;
					continue;
				}
				else
				{
					j -= 10;
					nCurNum += 10;
					continue;
				}
			}
			else if(strReturn == S_NEXT)
			{
				if(nCurNum <= 0)
				{
					j -= 5;
					nCurNum += 5;
					continue;
				}
				else
					continue;
			}
			else
				fnAPP_CancelProc(T_PROGRAM);
		}

	}

	return T_OK;	
}


// PME Accept Change PassWord
int CTranCmn::fnAPP_PMEAcceptChangePassWord()
{
	int		ScrNum = 412;
//	CString	strReturn("");

/////////////////////////////////////////////////////////////////////////////
	memcpy(Accept.PassWord, Accept.NewPassWord, sizeof(Accept.PassWord));		
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum);
	Delay_Msg(5000);
	fnAPP_CancelProc(T_EXIT);
	
//	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
//	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
//	if (strReturn == S_CONTINUE)
//		;
//	else
//	if (strReturn == S_EXIT)
//		fnAPP_CancelProc(T_EXIT);
//	else
//	if (strReturn == S_CANCEL)
//		fnAPP_CancelProc(T_CANCEL);
//	else
//	if (strReturn == S_TIMEOVER)
//		fnAPP_CancelProc(T_TIMEOVER);
//	else
//	if (strReturn == S_INPUTOVER)
//		fnAPP_CancelProc(T_INPUTOVER);
//	else
//		fnAPP_CancelProc(T_PROGRAM);		

	return T_OK;
}

// PME Accept PassWord
int CTranCmn::fnAPP_PMEAcceptPassWord()
{
	int		ScrNum = 102;
	int		iReloadTimes = 0;
	CString	strReturn("");
	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.PassWord, 'F', sizeof(Accept.PassWord));		// PassWord
/////////////////////////////////////////////////////////////////////////////
    //20070816 add by hyl
	if(CardBank == 1)
	{
		m_pDevCmn->fnSCR_DisplayString(1, "1");
	}
	
	while (TRUE)
	{
//		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE5, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_TRUE, PIN_PASSWORD_TERM);
//		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		//EPP ADD BY HYL 20070902

		m_pDevCmn->fnSCR_DisplayNumeric(2, iReloadTimes);

		if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_TRUE, PIN_PASSWORD_TERM);
		else 
		    m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_INPUT_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
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
		if (strReturn == "ENTER")
		{
			iReloadTimes ++;
			if (iReloadTimes >= 3)
				fnAPP_CancelProc(T_INPUTOVER);
			else
				continue;
		}
		else 
		if (strReturn == S_CLEAR)
		{
			iReloadTimes = 0;
			continue;
		}
		else
		{

			if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
            { 
			   CString PinKeyData = m_pDevCmn->fstrPIN_GetPinKeyData();
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
				if (strReturn.GetLength() >= 6)
				{
					memcpy(Accept.PassWord, "06", 2);
					memcpy(Accept.PassWord + 2, strReturn.GetBuffer(0), 6);
				}
				else
				{
					memcpy(Accept.PassWord, GetSprintf("02d",strReturn.GetLength(),2), 2);
					memcpy(Accept.PassWord + 2, strReturn.GetBuffer(0), strReturn.GetLength());
				}
			}
			
		}
		break;
	}
		
	return T_OK;
}

//  Set Host Time Procedure
int CTranCmn::fnAPP_SetHostTimeProc()
{
	char	Pdate[256];
	char	Ptime[256];

	memset(Pdate, 0, sizeof(Pdate));
	memset(Ptime, 0, sizeof(Ptime));

	if (IsNum(HMERecv.Data[13], 4))
	{
		GetDate(Pdate);
		memcpy(Pdate, HMERecv.Data[13], 8);
		PutDate(Pdate);
		m_pDevCmn->fnAPL_SetDate(Pdate);						// Transaction Date Set : 2004.05.08
	}

	if (IsNum(HMERecv.Data[12], 6))
	{
		GetTime(Ptime);
		memcpy(Ptime, HMERecv.Data[12], 6);
		PutTime(Ptime);											// Time Setting
	}

	return T_OK;
}

// Ready Procedure
int CTranCmn::fnAPP_ReadyProc()
{
	return T_OK;
}

// Ready Print Result
int CTranCmn::fnAPP_ReadyPrintResult(int PrintDevice, int WaitFlag)
{
	return T_OK;
}

// Open Procedure
int CTranCmn::fnAPP_OpenProc()
{
	int		i = 0;
	char	Pdate[256];
//	char	Ptime[256];

	if ((m_pDevCmn->FstCSTValue == CASH_CHINA_100) ||
		(m_pDevCmn->FstCSTValue == CASH_CHINA_50))
	{
		InitInfo.WithMaxAmt = CSH_M10050_MAXOUT;
		InitInfo.WithMinAmt = CSH_M10050_MINOUT;

		InitInfo.WithMaxAmt = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, CSH_M10050_MAXOUT);
		if ((InitInfo.WithMaxAmt < CSH_M10050_MINOUT) ||
			(InitInfo.WithMaxAmt > CSH_M10050_MAXOUT))
			InitInfo.WithMaxAmt = CSH_M10050_MAXOUT;
		IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, InitInfo.WithMaxAmt);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_WITHMAXAMOUNT[%d]", InitInfo.WithMaxAmt);


		InitInfo.WithMinAmt = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMINAMOUNT, CSH_M10050_MINOUT);
		if (InitInfo.WithMinAmt < CSH_M10050_MINOUT)
		{
            InitInfo.WithMinAmt = CSH_M10050_MINOUT;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_WITHMINAMOUNT[%d]", InitInfo.WithMinAmt);
		}
		else
	    if (InitInfo.WithMinAmt > CSH_M10050_MAXOUT)	
			InitInfo.WithMinAmt = CSH_M10050_MAXOUT;
		IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMINAMOUNT, InitInfo.WithMinAmt);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_WITHMINAMOUNT[%d]", InitInfo.WithMinAmt);

	}

	if ((m_pDevCmn->FstCSTValue == CASH_CHINA_20) ||
		(m_pDevCmn->FstCSTValue == CASH_CHINA_10))
	{
		InitInfo.WithMaxAmt = CSH_M2010_MAXOUT;
		InitInfo.WithMinAmt = CSH_M2010_MINOUT;

		InitInfo.WithMaxAmt = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, CSH_M2010_MAXOUT);
		if ((InitInfo.WithMaxAmt < CSH_M2010_MINOUT) ||
			(InitInfo.WithMaxAmt > CSH_M2010_MAXOUT))
			InitInfo.WithMaxAmt = CSH_M2010_MAXOUT;
		IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, InitInfo.WithMaxAmt);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_WITHMAXAMOUNT[%d]", InitInfo.WithMaxAmt);

	}

//  InitInfo.TransferMaxAmt = CSH_MONEY_TMAXOUT;   
    InitInfo.TransferMaxAmt = Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXAMOUNT,"50000"));
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXAMOUNT, GetSprintf("%.0f", InitInfo.TransferMaxAmt));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_TRANMAXAMOUNT[%s]", GetSprintf("%.2f", InitInfo.TransferMaxAmt));

// 	InitInfo.TransferMinAmt = CSH_MONEY_TMINOUT;
    InitInfo.TransferMinAmt = Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINAMOUNT,"50"));
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINAMOUNT, GetSprintf("%.0f", InitInfo.TransferMinAmt));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_TRANMINAMOUNT[%s]", GetSprintf("%.2f", InitInfo.TransferMinAmt));

    if(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHDAYMAXAMOUNT) == "0")
		  InitInfo.WithDayMaxAmt = 20000 ;
    else
	      InitInfo.WithDayMaxAmt = Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHDAYMAXAMOUNT, "20000"));
	IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHDAYMAXAMOUNT, GetSprintf("%.0f", InitInfo.WithDayMaxAmt));
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_WITHDAYMAXAMOUNT[%s]", GetSprintf("%.2f", InitInfo.WithDayMaxAmt));


   //广告时间
   if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADTIME) == 0)
	   InitInfo.ADTime = 5;
   else
       InitInfo.ADTime = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADTIME,5);
   IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADTIME,InitInfo.ADTime);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() _INIKEY_TIMEDEFINE_ADTIME[%s]", GetSprintf("%d", InitInfo.ADTime));


   //初始化广告图片张数并标记为永久广告 by hyl 
   
   if(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM) == 0)
   {
	   CFileFind   TempFp;
	   BOOL        bFind;
	   int ADCount=1;
	   char ADFileNamePath[256];
	   while(TRUE)
	   {   
		   
           sprintf(ADFileNamePath, "%s\\%s%s%s", _AP_ADUPDATE_DIR, "AD",Int2Asc(ADCount),".jpg");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ADFileNamePath[%s]", ADFileNamePath);
		   bFind = TempFp.FindFile(ADFileNamePath);
		   if(!bFind)
		   {			 
			 break;
		   }
		   else
		   {   
 			   IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,GetSprintf("%s%d","ADPICTUREUSETIME",ADCount),"");
			   IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO,GetSprintf("%s%d","ADPICTUREENDTIME",ADCount),"999999999999");
			   ADCount++;
		   }   
	   }
       IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM,ADCount -1);
	   
	   if(IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM)==0)       //若已有值则不修改它 by hyl
	       IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM,ADCount -1);
// 	   IniSetStr(_TRANSACTION_INI, _INIKEY_BIDDOWNLOADINFO, _INIKEY_TRANSACTION_ADPICTUREENDTIME,"999999999999");
   }
    
   //当广告图片大于本地设置时,以本地张数为准
   InitInfo.ADSetNum = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ADSETNUM);
   if(InitInfo.ADSetNum != 0 &&  InitInfo.ADSetNum < IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM))    
        IniSetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_ADPICTURENUM,InitInfo.ADSetNum);
   

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)				// HOST LOCAL MODE	
	{
		GetDate(Pdate);
		m_pDevCmn->fnAPL_SetDate(Pdate);						// Transaction Date Set
		
		//EPP ADD BY HYL 20070902
		if ((m_pProfile->DEVICE.MwDevice == MW_KALWOSA)	||
			(m_pProfile->DEVICE.MwDevice == MW_NEXTWOSA))
		{
			if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
			{
				m_pDevCmn->fnPIN_InitializeEPP();
				m_pDevCmn->fnPIN_LoadIV("Vector");
				m_pDevCmn->fnPIN_ImportKey(TMASTERKEY, "", GetSprintf("%s%s", ZERO16, ZERO16));
				m_pDevCmn->fnPIN_ImportKey(TPINKEY, TMASTERKEY, GetSprintf("%s%s", ZERO16, ZERO16));
				m_pDevCmn->fnPIN_ImportKey(TMACKEY, TMASTERKEY, GetSprintf("%s%s", ZERO16, ZERO16));
				m_pDevCmn->fnPIN_SetPinKeyName(TPINKEY);
				m_pDevCmn->fnPIN_SetMacKeyName(TMACKEY);
			}
			else
			{
				m_pDevCmn->fnPIN_InitializeEPP();
				m_pDevCmn->fnPIN_LoadIV("Vector");
				m_pDevCmn->fnPIN_ImportKey(SMASTERKEY, "", ZERO16);
				m_pDevCmn->fnPIN_ImportKey(SPINKEY, SMASTERKEY, ZERO16);
				m_pDevCmn->fnPIN_ImportKey(SMACKEY, SMASTERKEY, ZERO16);
				m_pDevCmn->fnPIN_SetPinKeyName(SPINKEY);
				m_pDevCmn->fnPIN_SetMacKeyName(SMACKEY);
			}
		}

		else
		{
			if (m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)						// 2006.04.26
			{
				m_pDevCmn->fnPIN_LoadIV("Vector");
				m_pDevCmn->fnPIN_LoadKey("K4", GetSprintf("%16.16s", "FFFFFFFFFFFFFFFF"));
				m_pDevCmn->fnPIN_LoadKey("K3", GetSprintf("%16.16s", "FFFFFFFFFFFFFFFF"));
				m_pDevCmn->fnPIN_LoadKey("K2", GetSprintf("%16.16s", "FFFFFFFFFFFFFFFF"));
				m_pDevCmn->fnPIN_LoadKey("K1", GetSprintf("%16.16s", "FFFFFFFFFFFFFFFF"));
			}
			else
			{
				m_pDevCmn->fnPIN_LoadIV("Vector");
				m_pDevCmn->fnPIN_LoadKey("K4", GetSprintf("%16.16s", "FFFFFFFFFFFFFFFF"));
				m_pDevCmn->fnPIN_LoadKey("K1", GetSprintf("%16.16s", "FFFFFFFFFFFFFFFF"));
			}	
		}
		return T_OK;
	}
/////////////////////////////////////////////////////////////////////////////

//	InitInfo.UserWithAvail = TRUE;
//	InitInfo.UserInquiryAvail = TRUE;
//	InitInfo.UserDetailAvail = FALSE;
//	InitInfo.UserTransAvail = TRUE;
//	InitInfo.UserChangePinAvail = FALSE;
//	if (m_pProfile->NETWORK.InterfaceKind == 18)				// Cup(BJGD) :2006.07.07
//		InitInfo.UserTransAvail = FALSE;						// 2007.01.18
	

//	PINKEY和MACKEY的处理在申请密钥通讯包返回时做，使用软加密   by zjd
//	char	PinKey[256];
//	char	PinKeyRight[256];
//	char	MacKey[256];
//	char	szTemp[256];
//	char	szCheckValue[256];
//	CString	strMacData("");
//
//	memset(PinKey, 0, sizeof(PinKey));
//	memset(PinKeyRight, 0, sizeof(PinKeyRight));
//	memset(MacKey, 0, sizeof(MacKey));
//	memset(szTemp, 0, sizeof(szTemp));
//	memset(szCheckValue, 0, sizeof(szCheckValue));
//
//	if (InitInfo.KeyMode == TRIPLE_DES)							// 2006.03.21
//	{
//		MakeUnPack(&HMERecv.Data[48][5],  PinKey, 8, 1);		// Pin Key
//		MakeUnPack(&HMERecv.Data[48][13], PinKeyRight, 8, 1);	// Pin Key Right
//		MakeUnPack(&HMERecv.Data[48][37], MacKey, 8, 1);		// Mac Key
//		
//		m_pDevCmn->fnPIN_LoadIV("Vector");
//		m_pDevCmn->fnPIN_LoadKey("K4", GetSprintf("%16.16s", MacKey));
//		m_pDevCmn->fnPIN_LoadKey("K3", GetSprintf("%16.16s", PinKey));
//		m_pDevCmn->fnPIN_LoadKey("K2", GetSprintf("%16.16s", PinKeyRight));
//		m_pDevCmn->fnPIN_LoadKey("K1", GetSprintf("%16.16s", PinKey));
//		
//		m_pDevCmn->fnPIN_MacingData(MakeUnPack(szTemp, 8));
//		strMacData = m_pDevCmn->fstrPIN_GetMacingData();
//
//		if (memcmp(&HMERecv.Data[48][53], strMacData.GetBuffer(0), 4) == 0)
//			;													// Mac Key Check
//		else
//		{
//			m_pDevCmn->TranResult = FALSE;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() MacKey CheckValue[%16.16s]", &HMERecv.Data[48][53]);
//			return T_OK;
//		}
//	}
//	else
//	{
//		MakeUnPack(&HMERecv.Data[48][5],  PinKey, 8, 1);		// Pin Key
//		MakeUnPack(&HMERecv.Data[48][37], MacKey, 8, 1);		// Mac Key
//
//		m_pDevCmn->fnPIN_LoadIV("Vector");
//		m_pDevCmn->fnPIN_LoadKey("K4", GetSprintf("%16.16s", MacKey));
//		m_pDevCmn->fnPIN_LoadKey("K1", GetSprintf("%16.16s", PinKey));
//
//		m_pDevCmn->fnPIN_MacingData(MakeUnPack(szTemp, 8));
//		strMacData = m_pDevCmn->fstrPIN_GetMacingData();
//
//		if (memcmp(&HMERecv.Data[48][53], strMacData.GetBuffer(0), 4) == 0)
//			;													// Mac Key Check
//		else
//		{
//			m_pDevCmn->TranResult = FALSE;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() MacKey CheckValue[%16.16s]", &HMERecv.Data[48][53]);
//			return T_OK;
//		}
//
//		m_pDevCmn->fnPIN_LoadIV("Vector");
//		m_pDevCmn->fnPIN_LoadKey("K4", GetSprintf("%16.16s", PinKey));
//		m_pDevCmn->fnPIN_LoadKey("K1", GetSprintf("%16.16s", MacKey));
//
//		m_pDevCmn->fnPIN_MacingData(MakeUnPack(szTemp, 8));
//		strMacData = m_pDevCmn->fstrPIN_GetMacingData();
//
//		if (memcmp(&HMERecv.Data[48][21], strMacData.GetBuffer(0), 4) == 0)
//			;													// Pin Key Check
//		else
//		{
//			m_pDevCmn->TranResult = FALSE;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_OpenProc() PinKey CheckValue[%16.16s]", &HMERecv.Data[48][21]);
//			return T_OK;
//		}
//		
//		memset(PinKey, 0, sizeof(PinKey));
//		memset(MacKey, 0, sizeof(MacKey));
//
//		MakeUnPack(&HMERecv.Data[48][5],  PinKey, 8, 1);		// Pin Key
//		MakeUnPack(&HMERecv.Data[48][37], MacKey, 8, 1);		// Mac Key
//
//		m_pDevCmn->fnPIN_LoadIV("Vector");
//		m_pDevCmn->fnPIN_LoadKey("K4", GetSprintf("%16.16s", MacKey));
//		m_pDevCmn->fnPIN_LoadKey("K1", GetSprintf("%16.16s", PinKey));
//	}

	return T_OK;
}

// Open Print Result
int CTranCmn::fnAPP_OpenPrintResult(int PrintDevice, int WaitFlag)
{
//	int		HiddenFlag = FALSE;
//
//	memset(ShuData, 0, sizeof(ShuData));
//
//	fnAPP_EditPrintLine(DEV_JPR, ShuData[7], 7, HiddenFlag);
//	fnAPP_EditPrintLine(DEV_JPR, ShuData[10], 10, HiddenFlag);
//	fnAPP_EditPrintLine(DEV_JPR, ShuData[11], 11, HiddenFlag);	// 2005.09.11
//	fnAPP_EditPrintLine(DEV_JPR, ShuData[20], 20, HiddenFlag);
//
//	fnAPD_ShuPrint(DEV_JPR, WaitFlag);
	return T_OK;
}

// Sum Procedure
int CTranCmn::fnAPP_SumProc(int InputID)
{
	char	iCount[256];
	char	iMoney[256];
	char	iTemp[256];
	CString	strDispCntData("");									// 2004.11.05

	memset(iCount, 0, sizeof(iCount));
	memcpy(iCount, "000001", 6);

	memset(iMoney, 0, sizeof(iMoney));
	memset(iTemp, 0, sizeof(iTemp));

	memset(iMoney, '0', 14);
	memcpy(&iMoney[14 - sizeof(Accept.Money)], Accept.Money, sizeof(Accept.Money));
	
	switch (InputID) 
	{
		case 0:													// Withdraw Host
			m_pProfile->GetWithTotal();

			if (Asc2Float(&iMoney[2], 12))						// 0:CWD
			{
				AddString(m_pProfile->WITHTOTAL.DATA[6].Count, iCount, 6);
				AddString(m_pProfile->WITHTOTAL.DATA[6].Amount, iMoney, 14);
				
				if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST1))
				{
					AddString(m_pProfile->WITHTOTAL.DATA[7].Count, iCount, 6);
					Int2Asc(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST1), iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[7].Amount, iTemp, 14);
				}
				if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST2))
				{
					AddString(m_pProfile->WITHTOTAL.DATA[8].Count, iCount, 6);
					Int2Asc(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST2), iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[8].Amount, iTemp, 14);
				}
				if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST3))
				{
					AddString(m_pProfile->WITHTOTAL.DATA[9].Count, iCount, 6);
					Int2Asc(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST3), iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[9].Amount, iTemp, 14);
				}
				if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST4))
				{
					AddString(m_pProfile->WITHTOTAL.DATA[10].Count, iCount, 6);
					Int2Asc(IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHCST4), iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[10].Amount, iTemp, 14);
				}
			}
			
			m_pProfile->PutWithTotal();
			break;

		case 1:													// Withdraw Atm
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_WITH, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetWithTotal();

			if (Asc2Float(&iMoney[2], 12))						// 0:CWD
			{
				AddString(m_pProfile->WITHTOTAL.DATA[0].Count, iCount, 6);
				AddString(m_pProfile->WITHTOTAL.DATA[0].Amount, iMoney, 14);

				if (CashFstCSTDispCnt)							// 2004.11.05
				{
					AddString(m_pProfile->WITHTOTAL.DATA[1].Count, iCount, 6);
					Int2Asc(CashFstCSTDispCnt, iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[1].Amount, iTemp, 14);
				}
				if (CashSndCSTDispCnt)
				{
					AddString(m_pProfile->WITHTOTAL.DATA[2].Count, iCount, 6);
					Int2Asc(CashSndCSTDispCnt, iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[2].Amount, iTemp, 14);
				}
				if (CashTrdCSTDispCnt)
				{
					AddString(m_pProfile->WITHTOTAL.DATA[3].Count, iCount, 6);
					Int2Asc(CashTrdCSTDispCnt, iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[3].Amount, iTemp, 14);
				}
				if (CashFthCSTDispCnt)
				{
					AddString(m_pProfile->WITHTOTAL.DATA[4].Count, iCount, 6);
					Int2Asc(CashFthCSTDispCnt, iTemp, 14);
					AddString(m_pProfile->WITHTOTAL.DATA[4].Amount, iTemp, 14);
				}

				strDispCntData = "0";							// 2004.11.05
				if (m_pDevCmn->CSTCnt >= CSH_CST_1)
					strDispCntData += "," + Int2Asc(m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1) + CashFstCSTDispCnt);
				if (m_pDevCmn->CSTCnt >= CSH_CST_2)
					strDispCntData += "," + Int2Asc(m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2) + CashSndCSTDispCnt);
				if (m_pDevCmn->CSTCnt >= CSH_CST_3)
					strDispCntData += "," + Int2Asc(m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3) + CashTrdCSTDispCnt);
				if (m_pDevCmn->CSTCnt >= CSH_CST_4)
					strDispCntData += "," + Int2Asc(m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4) + CashFthCSTDispCnt);
				m_pDevCmn->fnCSH_SetNumberOfDispenseCash(strDispCntData);
			}
			
			m_pProfile->PutWithTotal();
			break;

		case 2:													// Inquiry Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_INQ, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetInqTotal();
																
			AddString(m_pProfile->INQTOTAL.DATA[0].Count, iCount, 6);
																// 0:INQ
			m_pProfile->PutInqTotal();
			break;

		case 3:													// Change Pin Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_INQ, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetInqTotal();
			
			AddString(m_pProfile->INQTOTAL.DATA[1].Count, iCount, 6);

			m_pProfile->PutInqTotal();
			break;
			
		case 4:													// Transfer Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_TRANS, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetTransTotal();
			
			if (Asc2Float(&iMoney[2], 12))
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[0].Count, iCount, 6);
				AddString(m_pProfile->TRANSTOTAL.DATA[0].Amount, iMoney, 14);
			}

			m_pProfile->PutTransTotal();
			break;
			
		case 5:													// Detail Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_INQ, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetInqTotal();
																
			AddString(m_pProfile->INQTOTAL.DATA[2].Count, iCount, 6);
																// 0:INQ
			m_pProfile->PutInqTotal();
			break;

		case 6:													// Pay Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_TRANS, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetTransTotal();
																
			if (Asc2Float(&iMoney[2], 12))
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[1].Count, iCount, 6);
				AddString(m_pProfile->TRANSTOTAL.DATA[1].Amount, iMoney, 14);
			}
																// 1:PAY
			m_pProfile->PutTransTotal();
			break;

		case 7:													// Remit_Add Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_TRANS, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetTransTotal();
																
			if (Asc2Float(&iMoney[2], 12))
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[2].Count, iCount, 6);
				AddString(m_pProfile->TRANSTOTAL.DATA[2].Amount, iMoney, 14);
			}
																// 1:Remit_Add
			m_pProfile->PutTransTotal();
			break;

		case 8:													// Remit_IN Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_TRANS, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetTransTotal();
																
			if (Asc2Float(&iMoney[2], 12))
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[3].Count, iCount, 6);
				AddString(m_pProfile->TRANSTOTAL.DATA[3].Amount, iMoney, 14);
			}
																// 1:Remit_IN
			m_pProfile->PutTransTotal();
			break;

		case 9:													// Remit_PWD Host
			m_pDevCmn->fnAPL_AddTransactionStatistics(TRAN_TRANS, Accept.Money, sizeof(Accept.Money));
																// 2005.05.18
			m_pProfile->GetTransTotal();
																
			if (Asc2Float(&iMoney[2], 12))
			{
				AddString(m_pProfile->TRANSTOTAL.DATA[4].Count, iCount, 6);
				AddString(m_pProfile->TRANSTOTAL.DATA[4].Amount, iMoney, 14);
			}
																// 1:Remit_PWD
			m_pProfile->PutTransTotal();
			break;

		case 11:												// Cancel Withdraw
			m_pProfile->GetWithTotal();

			if (Asc2Float(&iMoney[2], 12))						// 1:CWC
			{
				AddString(m_pProfile->WITHTOTAL.DATA[5].Count, iCount, 6);
				AddString(m_pProfile->WITHTOTAL.DATA[5].Amount, iMoney, 14);
			}
			
			m_pProfile->PutWithTotal();
			break;

		default:
			break;
	}
	
	return T_OK;
}

// Sum Clear
int CTranCmn::fnAPP_SumClear()
{
/////////////////////////////////////////////////////////////////////////////
	SumClearFlag = TRUE;										// 2004.08.23
/////////////////////////////////////////////////////////////////////////////
	
	m_pProfile->InitWithTotal();								// Total Clear
	m_pProfile->InitInqTotal();
	m_pProfile->InitTransTotal();
	m_pProfile->InitDepTotal();
	m_pProfile->ClearAllErrTran();
	m_pProfile->ClearAllErrCancel();
//	m_pProfile->ClearAllErrHost();								// Speed Up

	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          *ATMP TOTAL*
// 2:========================================
// 3:DATE & TIME       : 2004.05.01 12:12:12
// 4:BRANCH NUMBER     : NNNNNNNN
// 5:ATM ID            : NNNNNNNN
// 6:BATCH NUMBER      : NNNNNNNNNNNNNN 
// 7:HOST RESULT       : (MATCH)
// 8:CST  DENOMINATION : NNNN,NNNN,NNNN,NNNN	    DENO
// 9:CST  SET     COUNT: NNNN,NNNN,NNNN,NNNN		SET               
//10:CST  REMAIN  COUNT: NNNN,NNNN,NNNN,NNNN		REMAIN	CST       
//11:CST  REJECT  COUNT: NNNN,NNNN,NNNN,NNNN		REJECT	CST       
//12:CST  JAM     COUNT: NNNN,NNNN,NNNN,NNNN		JAM(SHUTTER+PATH) 
//13:WITH(ATM)  CNT AMT: NNNN,NNNNNNNNNNNN.NN                            
//14:WITH(ATM)      CST: NNNN,NNNN,NNNN,NNNN		DISPENSE(CUSTOMER)
//15:WITH(HOST) CNT AMT: NNNN,NNNNNNNNNNNN.NN                            
//16:WITH(HOST)     CST: NNNN,NNNN,NNNN,NNNN		DISPENSE(HOST)    
//17:
//18:CWC  COUNT  AMOUNT: NNNN,NNNNNNNNNNNN.NN    
//19:TFR  COUNT  AMOUNT: NNNN,NNNNNNNNNNNN.NN
//20:PAY  COUNT  AMOUNT: NNNN,NNNNNNNNNNNN.NN
//21:INQUIRY      COUNT: NNNN
//22:DETAIL       COUNT: NNNN
//23:CHANGE PIN   COUNT: NNNN
//24:CARD CAPTURE COUNT: NNNN
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================          
// 1:          *CANCEL(CWC)*
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:BRANCH NUMBER      : NNNNNNNN
// 5:ATM ID             : NNNNNNNN
// 6:TOTAL CANCEL COUNT : NNNN
// 7:
// 8:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
// 9:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//10:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//11:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//12:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//13:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//14:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//15:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//16:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//17:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//18:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//19:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//20:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//21:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//22:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//23:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
//24:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          *ERROR(OEX,AEX)*
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:BRANCH NUMBER      : NNNNNNNN
// 5:ATM ID             : NNNNNNNN
// 6:TOTAL ERROR COUNT  : NNNN
// 7:
// 8:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS)
// 9:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//10:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//11:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//12:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//13:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//14:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//15:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//16:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//17:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//18:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//19:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//20:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS) 
//21:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS)
//22:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS)
//23:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS)
//24:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS)
/////////////////////////////////////////////////////////////////////////////
// Sum Print
int CTranCmn::fnAPP_SumPrint(int PrintDevice, int WaitFlag)
{
	DERRINFO	ErrCancel[ErrHost_MaxNumber];
	DERRINFO	ErrTran[ErrHost_MaxNumber];
//	DERRINFO	ErrHost[ErrHost_MaxNumber];
	int			ErrCancelNum = 0, ErrTranNum = 0;/*, ErrHostNum = 0;*/
	char		CurTime[256];
	int			i = 0, j = 0;
	int			TempRejectCount1 = 0, TempRejectCount2 = 0, TempRejectCount3 = 0, TempRejectCount4 =0;
	int			TempJamCount1 = 0, TempJamCount2 = 0, TempJamCount3 = 0, TempJamCount4 =0;
	CString		strTemp("");
	CString		strTemp2("");
	CString		strTemp3("");
	CString		strTemp4("");
	CString		strTemp5("");
	int			nTempAmount = 0, nTempOutAmount = 0, nTempSetAmount = 0;
	double		fTmpAmt = 0.0;
	double		fTmpAmt1 = 0.0;
	int iCnt;
    int iSuspCnt = 0;

/////////////////////////////////////////////////////////////////////////////
	m_pProfile->GetWithTotal();
	m_pProfile->GetInqTotal();
	m_pProfile->GetTransTotal();
	m_pProfile->GetDepTotal();
	ErrCancelNum = m_pProfile->GetAllErrCancel(ErrCancel);
	ErrTranNum = m_pProfile->GetAllErrTran(ErrTran);
//	ErrHostNum = m_pProfile->GetAllErrHost(ErrHost);

/////////////////////////////////////////////////////////////////////////////
	memset(ShuData, 0, sizeof(ShuData));

	j = 0;

	{
//		sprintf(ShuData[j++], "========================================");
		if (TranCode == TC_TOTAL)
		{
			iSuspCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, "SUSPICIOUSNUM", 0);
			if( iSusTotalNum == 0 )
			{
				if (memcmp(HostResp.RejectCode, "00", 2) == 0)
					sprintf(ShuData[j++], "*****************YES*****************");
				else
					sprintf(ShuData[j++], "*****************NO******************");

				sprintf(ShuData[j++], "%10.10sPrint ATMP TOTAL", "");		// 2005.03.16
				GetTime(CurTime);
				sprintf(ShuData[j++], "DATE : %4.4s-%2.2s-%2.2s ATM ID : %8.8s",
							m_pProfile->TRANS.DeviceYYYYMMDD,
							&m_pProfile->TRANS.DeviceYYYYMMDD[4],
							&m_pProfile->TRANS.DeviceYYYYMMDD[6],
							m_pProfile->NETWORK.AtmSerialNum);
				sprintf(ShuData[j++], "BATCH NO : %4.4s",
							HMERecv.Data[72]);
				sprintf(ShuData[j++], "            ATMP TOTAL          MATCH");
//			fTmpAmt = ((m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1) + m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1)) * m_pDevCmn->FstCSTValue)
//					+ ((m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2) + m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2)) * m_pDevCmn->SndCSTValue)
//					+ ((m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3) + m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_3)) * m_pDevCmn->TrdCSTValue)
//					+ ((m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4) + m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_4)) * m_pDevCmn->FthCSTValue);
//HYL 修改本地REMAIN
				fTmpAmt = Asc2Float(GetSprintf("%12.12s",InputRemainAmount))/100;
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InputRemainAmount[%.0f]", fTmpAmt);
				
				fTmpAmt1 = Asc2Float(HMERecv.Data[121] + 57, 12) / 100;

				
				
				if (fTmpAmt == fTmpAmt1)
					sprintf(ShuData[j++], "RMB Cash Remain Amount:%10.10s.%2.2s Y",
								HMERecv.Data[121] + 57, HMERecv.Data[121] + 67);
				else
					sprintf(ShuData[j++], "RMB Cash Remain Amount:%10.10s.%2.2s N",
								HMERecv.Data[121] + 57, HMERecv.Data[121] + 67);

				if (Asc2Int(HMERecv.Data[121] +3 , 6) == Asc2Int(&m_pProfile->WITHTOTAL.DATA[0].Count[2], 4))
					sprintf(ShuData[j++], "RMB CWD Times     : %6.6s           Y",
								HMERecv.Data[121] + 3);
				else
					sprintf(ShuData[j++], "RMB CWD Times     : %6.6s           N",
								HMERecv.Data[121] + 3);


				if (Asc2Float(HMERecv.Data[121] + 9, 12) == Asc2Float(m_pProfile->WITHTOTAL.DATA[0].Amount, 14))
					sprintf(ShuData[j++], "RMB CWD Amount    : %10.10s.%2.2s    Y",
								HMERecv.Data[121] + 9, HMERecv.Data[121] + 19);
				else
					sprintf(ShuData[j++], "RMB CWD Amount    : %10.10s.%2.2s    N",
								HMERecv.Data[121] + 9, HMERecv.Data[121] + 19);

				if (Asc2Int(HMERecv.Data[121] + 39, 6) == Asc2Int(&m_pProfile->TRANSTOTAL.DATA[0].Count[2], 4))
					sprintf(ShuData[j++], "RMB TFR Times     : %6.6s           Y",
								HMERecv.Data[121] + 39);
				else
					sprintf(ShuData[j++], "RMB TFR Times     : %6.6s           N",
								HMERecv.Data[121] + 39);

				if (Asc2Float(HMERecv.Data[121] + 45, 12) == Asc2Float(m_pProfile->TRANSTOTAL.DATA[0].Amount, 14))
					sprintf(ShuData[j++], "RMB TFR Amount    : %10.10s.%2.2s    Y",
								HMERecv.Data[121] + 45, HMERecv.Data[121] + 55);
				else
					sprintf(ShuData[j++], "RMB TFR Amount    : %10.10s.%2.2s    N",
								HMERecv.Data[121] + 45, HMERecv.Data[121] + 55);

			}
			
			//增加可疑交易处理 BY HYL
 			if (memcmp(HostResp.RejectCode, "00", 2) != 0)  
			{
				if (iSuspCnt > 0)
				{
					if(iSusTotalNum == 0)
					{
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "iSuspCnt[%d]",iSuspCnt);
						sprintf(ShuData[j++],"======(Abnormal Transaction)======");
						
					}
                     
					if(iSuspCnt >= 7*( 1+ iSusTotalNum))
					{
						iCnt = 7;
					}
					else
					{  
						iCnt = iSuspCnt - 7*iSusTotalNum;
					}
					
					for(int jj = 7*iSusTotalNum; jj < 7*iSusTotalNum + iCnt; jj++)
					{
						sprintf(ShuData[j++],IniGetStr(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, GetSprintf("SUSPICIOUSLIST1%03.3d",jj), 0));
						sprintf(ShuData[j++],IniGetStr(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, GetSprintf("SUSPICIOUSLIST2%03.3d",jj), 0));
					}

					if(iSusTotalNum == iSuspCnt /7 )
					{
					    sprintf(ShuData[j++],GetSprintf("Abnormal Num : %3.3d", iSuspCnt));  
				    	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Abnormal CWC");
					}
					
				}

 			}
			
		}
		else
		if (TranCode == TC_SUBTOTAL)
		{
			sprintf(ShuData[j++], "%10.10s*ATMC TOTAL*", "");		// 2005.03.16
//			sprintf(ShuData[j++], "========================================");
			GetTime(CurTime);
			sprintf(ShuData[j++], "DATE : %4.4s-%2.2s-%2.2s ATM ID : %8.8s",
						m_pProfile->TRANS.DeviceYYYYMMDD,
						&m_pProfile->TRANS.DeviceYYYYMMDD[4],
						&m_pProfile->TRANS.DeviceYYYYMMDD[6],
						m_pProfile->NETWORK.AtmSerialNum);
			sprintf(ShuData[j++], "      ATMC TOTAL");
			sprintf(ShuData[j++], "ATM Cassettes Total Number:%d", m_pDevCmn->CSTCnt);
			sprintf(ShuData[j++], "            Cas.1  Cas.2  Cas.3  Cas.4");
			sprintf(ShuData[j++], "Notes     :  CNY    CNY    CNY    CNY");
			sprintf(ShuData[j++], "Value     : %4.4d,  %4.4d,  %4.4d,  %4.4d", 
								m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_1),
								m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_2),
								m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_3),
								m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_4));
			
			sprintf(ShuData[j++], "Last      : %4.4d,  %4.4d,  %4.4d,  %4.4d", 
								m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_1),
								m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_2),
								m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_3),
								m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_4));

			sprintf(ShuData[j++], "Left      : %4.4d,  %4.4d,  %4.4d,  %4.4d", 
								m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1),
								m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2),
								m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3),
								m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4));
																	// 2005.02.22
			sprintf(ShuData[j++], "Disp      : %4.4s,  %4.4s,  %4.4s,  %4.4s", 
								&m_pProfile->WITHTOTAL.DATA[1].Amount[10],
								&m_pProfile->WITHTOTAL.DATA[2].Amount[10],
								&m_pProfile->WITHTOTAL.DATA[3].Amount[10],
								&m_pProfile->WITHTOTAL.DATA[4].Amount[10]);
			
			TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1)	-
						 m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)			-	
						 m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1);
			if (TempRejectCount1 < 0)
				TempRejectCount1 = 0;

			TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2)	-
						 m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)			-	
						 m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2);
			if (TempRejectCount2 < 0)
				TempRejectCount2 = 0;
			
			TempRejectCount3 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3)	-
				m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3)			-	
				m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3);
			if (TempRejectCount3 < 0)
				TempRejectCount3 = 0;

			TempRejectCount4 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_4)	-
							   m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4)			-	
							   m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4);
			if (TempRejectCount4 < 0)
				TempRejectCount4 = 0;

			sprintf(ShuData[j++], "PURG      : %4.4d,  %4.4d,  %4.4d,  %4.4d", 
													TempRejectCount1, TempRejectCount2, TempRejectCount3, TempRejectCount4);
																	// 2005.02.22
			sprintf(ShuData[j++], "RMB Cash Init Amount  : %10.10d.00",	// 2005.03.16
								   ((m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_1) * m_pDevCmn->FstCSTValue)
								+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_2) * m_pDevCmn->SndCSTValue)
								+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_3) * m_pDevCmn->TrdCSTValue)
								+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_4) * m_pDevCmn->FthCSTValue)));
			sprintf(ShuData[j++], "RMB Cash Remain Amount: %10.10d.00",	// 2005.03.16
								   (((m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)) * m_pDevCmn->FstCSTValue)
								+	((m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)) * m_pDevCmn->SndCSTValue)
								+	((m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_3)) * m_pDevCmn->TrdCSTValue)
								+	((m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_4) - m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_4)) * m_pDevCmn->FthCSTValue)));
			sprintf(ShuData[j++], "RMB CWD Times         : %4.4s", &m_pProfile->WITHTOTAL.DATA[0].Count[2]);
			sprintf(ShuData[j++], "RMB CWD Amount        : %12.12s.%2.2s",		
													&m_pProfile->WITHTOTAL.DATA[0].Amount[0],
													&m_pProfile->WITHTOTAL.DATA[0].Amount[12]);
			sprintf(ShuData[j++], "RMB TFR Times         : %4.4s", &m_pProfile->TRANSTOTAL.DATA[0].Count[2]);
			sprintf(ShuData[j++], "RMB TFR Amount        : %12.12s.%2.2s",		
													&m_pProfile->TRANSTOTAL.DATA[0].Amount[0],
													&m_pProfile->TRANSTOTAL.DATA[0].Amount[12]);
			sprintf(ShuData[j++], "RMB CWC TIMES         : %4.4s", &m_pProfile->WITHTOTAL.DATA[5].Count[2]);
			sprintf(ShuData[j++], "RMB CWC AMOUNT        : %12.12s.%2.2s",		
													&m_pProfile->WITHTOTAL.DATA[5].Amount[0],
													&m_pProfile->WITHTOTAL.DATA[5].Amount[12]);
			sprintf(ShuData[j++], "Momeny Retact Times   : %d", 
													IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0));
			sprintf(ShuData[j++], "Momeny Retact Amount  : %d.00",		
													IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0) / 100);
			sprintf(ShuData[j++], "Card Retained         : %4.4d", 
													Asc2Int(m_pDevCmn->fstrMCU_GetRetractCnt()));

		}

/////////////////////////////////////////////////////////////////////////////
		fnAPD_ShuPrint(PrintDevice, TRUE);

		if (TranCode == TC_TOTAL)                 //递归打印异常交易 by hyl
		{
			if( iSusTotalNum < iSuspCnt / 7 )
			{
				iSusTotalNum++;
				fnAPP_SumPrint(PrintDevice);
			}

			iSusTotalNum = 0;
		}
		
        

		if (PrintDevice & DEV_SPR)
		{
			fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
			fnAPD_MaterialOutCheckRetract(DEV_SPR);
		}

//////////////////////////////////////////////////////////////////////
//		j = 0;
//		for (i = 0; i <= ErrCancelNum; i++)
//		{
//			if ((i) && (i == ErrCancelNum))
//				break;
//
//			if (j == 0)
//			{
//				memset(ShuData, 0, sizeof(ShuData));
//				
//				sprintf(ShuData[j++], "========================================");
//				sprintf(ShuData[j++], "%10.10s*CANCEL(CWC)*", "");
//				sprintf(ShuData[j++], "========================================");
//				GetTime(CurTime);
//				sprintf(ShuData[j++], "DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
//							m_pProfile->TRANS.DeviceYYYYMMDD,
//							&m_pProfile->TRANS.DeviceYYYYMMDD[4],
//							&m_pProfile->TRANS.DeviceYYYYMMDD[6],
//							CurTime,
//							&CurTime[2],
//							&CurTime[4]);
//				sprintf(ShuData[j++], "BRANCH NUMBER      : %9.9s", &m_pProfile->NETWORK.BranchNum[0]);
//				sprintf(ShuData[j++], "ATM ID             : %8.8s", &m_pProfile->NETWORK.AtmSerialNum[0]);
//				sprintf(ShuData[j++], "TOTAL CANCEL COUNT : %4.4d", ErrCancelNum);
//				j++;
//			}
//
//			if (ErrCancelNum == 0)
//				sprintf(ShuData[j++], "# NO DATA #");
//
//			if (i == ErrCancelNum)
//				break;
//
//			sprintf(ShuData[j++], "%40.40s", ErrCancel[i].Remark);
//
//			if (j >= 25)
//			{
//				fnAPD_ShuPrint(PrintDevice, TRUE);
//				if (PrintDevice & DEV_SPR)
//				{
//					fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
//					fnAPD_MaterialOutCheckRetract(DEV_SPR);
//				}
//				j = 0;
//			}
//		}
//
//		fnAPD_ShuPrint(PrintDevice, TRUE);
//		if (PrintDevice & DEV_SPR)
//		{
//			fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
//			fnAPD_MaterialOutCheckRetract(DEV_SPR);
//		}
//
////////////////////////////////////////////////////////////////////////
//		j = 0;
//		for (i = 0; i <= ErrTranNum; i++)
//		{
//			if ((i) && (i == ErrTranNum))
//				break;
//
//			if (j == 0)
//			{
//				memset(ShuData, 0, sizeof(ShuData));
//				
//				sprintf(ShuData[j++], "========================================");
//				sprintf(ShuData[j++], "%10.10s*ERROR(OEX,AEX)*", "");
//				sprintf(ShuData[j++], "========================================");
//				GetTime(CurTime);
//				sprintf(ShuData[j++], "DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
//							m_pProfile->TRANS.DeviceYYYYMMDD,
//							&m_pProfile->TRANS.DeviceYYYYMMDD[4],
//							&m_pProfile->TRANS.DeviceYYYYMMDD[6],
//							CurTime,
//							&CurTime[2],
//							&CurTime[4]);
//				sprintf(ShuData[j++], "BRANCH NUMBER      : %9.9s", &m_pProfile->NETWORK.BranchNum[0]);
//				sprintf(ShuData[j++], "ATM ID             : %8.8s", &m_pProfile->NETWORK.AtmSerialNum[0]);
//				sprintf(ShuData[j++], "TOTAL ERROR COUNT  : %4.4d", ErrTranNum);
//				j++;
//			}
//
//			if (ErrTranNum == 0)
//				sprintf(ShuData[j++], "# NO DATA #");
//
//			if (i == ErrTranNum)
//				break;
//
//			sprintf(ShuData[j++], "%40.40s", ErrTran[i].Remark);
//
//			if (j >= 25)
//			{
//				fnAPD_ShuPrint(PrintDevice, TRUE);
//				if (PrintDevice & DEV_SPR)
//				{
//					fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
//					fnAPD_MaterialOutCheckRetract(DEV_SPR);
//				}
//				j = 0;
//			}
//		}
//
//		fnAPD_ShuPrint(PrintDevice, TRUE);
//		if (PrintDevice & DEV_SPR)
//		{
//			fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
//			fnAPD_MaterialOutCheckRetract(DEV_SPR);
//		}
	}

//////////////////////////////////////////////////////////////////////
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          *ATMP TOTAL RESULT*
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:BRANCH NUMBER      : NNNNNNNN
// 5:ATM ID             : NNNNNNNN
// 6:BATCH NUMBER       : NNNNNNNNNNNNNN
// 7:HOST RESULT        : (NET ERROR) 

// Sum Print Result
int CTranCmn::fnAPP_SumPrintResult(int PrintDevice, int WaitFlag)
{
//	int			i = 0, j = 0;
//	char		CurTime[256];

/////////////////////////////////////////////////////////////////////////////
//	memset(ShuData, 0, sizeof(ShuData));
//
//	j = 0;
//	sprintf(ShuData[j++], "========================================");
//	sprintf(ShuData[j++], "%10.10s*ATMP TOTAL RESULT*", "");
//	sprintf(ShuData[j++], "========================================");
//
//	GetTime(CurTime);
//	sprintf(ShuData[j++], "DATE & TIME        : %4.4s.%2.2s.%2.2s %2.2s:%2.2s:%2.2s",
//				m_pProfile->TRANS.DeviceYYYYMMDD,
//				&m_pProfile->TRANS.DeviceYYYYMMDD[4],
//				&m_pProfile->TRANS.DeviceYYYYMMDD[6],
//				CurTime,
//				&CurTime[2],
//				&CurTime[4]);
//	sprintf(ShuData[j++], "BRANCH NUMBER      : %9.9s", &m_pProfile->NETWORK.BranchNum[0]);
//	sprintf(ShuData[j++], "ATM ID             : %8.8s", &m_pProfile->NETWORK.AtmSerialNum[0]);
//	sprintf(ShuData[j++], "BATCH NUMBER       : %04.4s", 
//			GetSprintf("%04.4s", IniGetStr(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_TRANBATCHNO, "0001")));
//	
//	if (m_pDevCmn->TranResult)
//		sprintf(ShuData[j++], "HOST RESULT        : %s", "(NORMAL)");
//	else
//		sprintf(ShuData[j++], "HOST RESULT        : %s", "(NET ERROR)");
//
//	fnAPD_ShuPrint(PrintDevice, TRUE);
//	if (PrintDevice & DEV_SPR)
//	{
//		fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
//		fnAPD_MaterialOutCheckRetract(DEV_SPR);
//	}
//		
	return T_OK;
}

// Cash Procedure
int CTranCmn::fnAPP_CashProc()
{
	CString		strDispCntData("");								// 2004.11.05
	CString		strCashCntData("");
	CString		strTotalCashCntData("");

/////////////////////////////////////////////////////////////////////////////
	AddCashFlag = TRUE;											// 2004.08.23
/////////////////////////////////////////////////////////////////////////////

	if (m_pDevCmn->fnAPL_GetAvailDevice(DEV_CSH))				// 2004.08.23	
	{
		strDispCntData = "0";									// 2004.11.05
		strCashCntData = "0";
		strTotalCashCntData = "0";
		if (m_pDevCmn->CSTCnt >= CSH_CST_1)						// 2004.11.05
		{
			strDispCntData += ",0";
			strCashCntData += "," + Int2Asc(m_pDevCmn->FstCSTSetCash);
			strTotalCashCntData += "," + Int2Asc(m_pDevCmn->FstCSTSetCash);
		}
		if (m_pDevCmn->CSTCnt >= CSH_CST_2)
		{
			strDispCntData += ",0";
			strCashCntData += "," + Int2Asc(m_pDevCmn->SndCSTSetCash);
			strTotalCashCntData += "," + Int2Asc(m_pDevCmn->SndCSTSetCash);
		}
		if (m_pDevCmn->CSTCnt >= CSH_CST_3)
		{
			strDispCntData += ",0";
			strCashCntData += "," + Int2Asc(m_pDevCmn->TrdCSTSetCash);
			strTotalCashCntData += "," + Int2Asc(m_pDevCmn->TrdCSTSetCash);
		}
		if (m_pDevCmn->CSTCnt >= CSH_CST_4)
		{
			strDispCntData += ",0";
			strCashCntData += "," + Int2Asc(m_pDevCmn->FthCSTSetCash);
			strTotalCashCntData += "," + Int2Asc(m_pDevCmn->FthCSTSetCash);
		}
																// 2004.11.05
		m_pDevCmn->fnCSH_SetNumberOfDispenseCash(strDispCntData);	
		m_pDevCmn->fnCSH_SetNumberOfCash(strCashCntData, strTotalCashCntData);
		m_pDevCmn->fnAPL_CheckDeviceAction(DEV_CSH);

		m_pDevCmn->fnCSH_ClearDispenseInfo();					// Clear Dispense Information : 2004.08.23
	}
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          *PRINT CASH*
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:BRANCH NUMBER      : NNNNNNNN
// 5:ATM ID             : NNNNNNNN
// 6:BATCH NUMBER       : NNNNNNNNNNNNNN 
// 7:
// 8:CST  DENOMINATION  : NNNN,NNNN,NNNN,NNNN	    DENO
// 9:CST  SET     COUNT : NNNN,NNNN,NNNN,NNNN		SET               
//10:CST  REMAIN  COUNT : NNNN,NNNN,NNNN,NNNN		REMAIN	CST       
//11:CST  REJECT  COUNT : NNNN,NNNN,NNNN,NNNN		REJECT	CST       
//12:CST  JAM     COUNT : NNNN,NNNN,NNNN,NNNN		JAM(SHUTTER+PATH) 
//13:CST  DISP    COUNT : NNNN,NNNN,NNNN,NNNN		DISPENSE(CUSTOMER)   
//14:CST  SET    AMOUNT : NNNNNNNNNN.NN                               
//15:
//16:
//17:
//18:
//19:
//20:
//21:
//22:
//23:
//24:
/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:========================================
// 1:          *ADD CASH*
// 2:========================================
// 3:DATE & TIME        : 2004.05.01 12:12:12
// 4:BRANCH NUMBER      : NNNNNNNN
// 5:ATM ID             : NNNNNNNN
// 6:BATCH NUMBER       : NNNNNNNNNNNNNN
// 7:HOST RESULT        : NORMAL 
// 8:
// 9:CST  DENOMINATION : NNNN,NNNN,NNNN,NNNN	    DENO
//10:CST  SET     COUNT : NNNN,NNNN,NNNN,NNNN		SET               
//11:CST  REMAIN  COUNT : NNNN,NNNN,NNNN,NNNN		REMAIN	CST       
//12:CST  REJECT  COUNT : NNNN,NNNN,NNNN,NNNN		REJECT	CST       
//13:CST  JAM     COUNT : NNNN,NNNN,NNNN,NNNN		JAM(SHUTTER+PATH) 
//14:CST  DISP    COUNT : NNNN,NNNN,NNNN,NNNN		DISPENSE(CUSTOMER)
//15:CST  SET    AMOUNT : NNNNNNNNNN.NN                                  
//16:
//17:CashNO   VAL      ADD      OUT      REJ 
//18:  1 	  100     1000     1000     1000
//19:  2 	  100     1000     1000     1000
//20:  3 	  100     1000     1000     1000
//21:  4 	  100     1000     1000     1000
//22:SUM ADD  AMOUNT : 
//23:SUM OUT  AMOUNT :
//24:SUM LEFT AMOUNT :
/////////////////////////////////////////////////////////////////////////////
// Cash Print
int CTranCmn::fnAPP_CashPrint(int PrintDevice, int WaitFlag)
{
	char		CurTime[256];
	int			i = 0, j = 0;
	int			TempRejectCount1 = 0, TempRejectCount2 = 0, TempRejectCount3 = 0, TempRejectCount4 =0;
	int			TempJamCount1 = 0, TempJamCount2 = 0, TempJamCount3 = 0, TempJamCount4 =0;

//////////////////////////////////////////////////////////////////////
	memset(ShuData, 0, sizeof(ShuData));

	j = 0;
//	sprintf(ShuData[j++], "========================================");
//	if (TranCode == TC_ADDCASH)
//		sprintf(ShuData[j++], "%10.10s*ADD CASH*", "");
//	else
//	if (TranCode == TC_SUBCASH)
//		sprintf(ShuData[j++], "%10.10s*SUB CASH*", "");
//	else
//	if (TranCode == TC_CLEARCASH)
//		sprintf(ShuData[j++], "%10.10s*CLEAR CASH*", "");
//	else
//	if (TranCode == TC_PRINTCASH)
//		sprintf(ShuData[j++], "%10.10s*PRINT CASH*", "");
//	sprintf(ShuData[j++], "========================================");

	GetTime(CurTime);
	sprintf(ShuData[j++], "DATE : %4.4s-%2.2s-%2.2s     ATM ID : %8.8s",
				m_pProfile->TRANS.DeviceYYYYMMDD,
				&m_pProfile->TRANS.DeviceYYYYMMDD[4],
				&m_pProfile->TRANS.DeviceYYYYMMDD[6],
				&m_pProfile->NETWORK.AtmSerialNum[0]);
	sprintf(ShuData[j++], "Operator ID : %7.7s         Print Cash",
				szADDCashCounterNumber);
																// 2004.08.23
//////////////////////////////////////////////////////////////////////
	sprintf(ShuData[j++], "         Cas.1  Cas.2  Cas.3  Cas.4");
	sprintf(ShuData[j++], "Notes  : CNY    CNY    CNY    CNY ");
	sprintf(ShuData[j++], "Value  : %4.4d   %4.4d   %4.4d   %4.4d", 
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_1),
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_2),
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_3),
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_4));

	sprintf(ShuData[j++], "Load   : %4.4d   %4.4d   %4.4d   %4.4d", 
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_1),
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_2),
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_3),
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_4));

	sprintf(ShuData[j++], "Left   : %4.4d   %4.4d   %4.4d   %4.4d", 
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1),
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2),
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3),
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4));

	TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1)	-
				 m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)			-	
				 m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)	-
				 m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 >= 0)
		TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	else
		TempRejectCount1 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 < 0)
		TempRejectCount1 = 0;

	TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2)	-
				 m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)			-	
				 m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)	-
				 m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 >= 0)
		TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	else
		TempRejectCount2 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 < 0)
		TempRejectCount2 = 0;

	TempRejectCount3 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3)	-
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

	sprintf(ShuData[j++], "Purg   : %4.4d   %4.4d   %4.4d   %4.4d", 
											TempRejectCount1, TempRejectCount2, TempRejectCount3, TempRejectCount4);
	sprintf(ShuData[j++], "RMB Cash Load Amount : %10.10d.00",	// 2005.03.16
										   ((m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_1) * m_pDevCmn->FstCSTValue)
										+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_2) * m_pDevCmn->SndCSTValue)
										+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_3) * m_pDevCmn->TrdCSTValue)
										+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_4) * m_pDevCmn->FthCSTValue)));
		
//////////////////////////////////////////////////////////////////////

	fnAPD_ShuPrint(PrintDevice, TRUE);
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
		fnAPD_MaterialOutCheckRetract(DEV_SPR);
	}

//////////////////////////////////////////////////////////////////////
	return T_OK;
}

// Cash Print Result
int CTranCmn::fnAPP_CashPrintResult(int PrintDevice, int WaitFlag)
{
	char		CurTime[256];
	int			i = 0, j = 0;
	int			TempRejectCount1 = 0, TempRejectCount2 = 0, TempRejectCount3 = 0, TempRejectCount4 =0;
	int			TempJamCount1 = 0, TempJamCount2 = 0, TempJamCount3 = 0, TempJamCount4 =0;

//////////////////////////////////////////////////////////////////////
	memset(ShuData, 0, sizeof(ShuData));

	j = 0;
//	sprintf(ShuData[j++], "========================================");
//	if (TranCode == TC_ADDCASH)
//		sprintf(ShuData[j++], "%10.10s*ADD CASH*", "");
//	else
//	if (TranCode == TC_SUBCASH)
//		sprintf(ShuData[j++], "%10.10s*SUB CASH*", "");
//	else
//	if (TranCode == TC_CLEARCASH)
//		sprintf(ShuData[j++], "%10.10s*CLEAR CASH*", "");
//	else
//	if (TranCode == TC_PRINTCASH)
//		sprintf(ShuData[j++], "%10.10s*PRINT CASH*", "");
//	sprintf(ShuData[j++], "========================================");

	GetTime(CurTime);
	sprintf(ShuData[j++], "DATE : %4.4s-%2.2s-%2.2s     ATM ID : %8.8s",
				m_pProfile->TRANS.DeviceYYYYMMDD,
				&m_pProfile->TRANS.DeviceYYYYMMDD[4],
				&m_pProfile->TRANS.DeviceYYYYMMDD[6],
				&m_pProfile->NETWORK.AtmSerialNum[0]);
	sprintf(ShuData[j++], "Operator ID : %7.7s         Load Cash",
				szADDCashCounterNumber);
																// 2004.08.23
//////////////////////////////////////////////////////////////////////
	sprintf(ShuData[j++], "         Cas.1  Cas.2  Cas.3  Cas.4");
	sprintf(ShuData[j++], "Notes  : CNY    CNY    CNY    CNY ");
	sprintf(ShuData[j++], "Value  : %4.4d   %4.4d   %4.4d   %4.4d", 
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_1),
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_2),
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_3),
											m_pDevCmn->fnCSH_GetValueOfCash(CSH_CST_4));

	sprintf(ShuData[j++], "Load   : %4.4d   %4.4d   %4.4d   %4.4d", 
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_1),
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_2),
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_3),
											m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_4));

	sprintf(ShuData[j++], "Left   : %4.4d   %4.4d   %4.4d   %4.4d", 
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1),
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2),
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_3),
											m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_4));

	TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_1)	-
				 m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_1)			-	
				 m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_1)	-
				 m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 >= 0)
		TempRejectCount1 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	else
		TempRejectCount1 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_1);
	if (TempRejectCount1 < 0)
		TempRejectCount1 = 0;

	TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_2)	-
				 m_pDevCmn->fnCSH_GetNumberOfCash(CSH_CST_2)			-	
				 m_pDevCmn->fnCSH_GetNumberOfDispenseCash(CSH_CST_2)	-
				 m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 >= 0)
		TempRejectCount2 = m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	else
		TempRejectCount2 += m_pDevCmn->fnCSH_GetNumberOfReject(CSH_CST_2);
	if (TempRejectCount2 < 0)
		TempRejectCount2 = 0;

	TempRejectCount3 = m_pDevCmn->fnCSH_GetNumberOfTotalSetCash(CSH_CST_3)	-
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

	sprintf(ShuData[j++], "Purg   : %4.4d   %4.4d   %4.4d   %4.4d", 
											TempRejectCount1, TempRejectCount2, TempRejectCount3, TempRejectCount4);
	sprintf(ShuData[j++], "RMB Cash Load Amount : %10.10d.00",	// 2005.03.16
										   ((m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_1) * m_pDevCmn->FstCSTValue)
										+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_2) * m_pDevCmn->SndCSTValue)
										+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_3) * m_pDevCmn->TrdCSTValue)
										+	(m_pDevCmn->fnCSH_GetNumberOfSetCash(CSH_CST_4) * m_pDevCmn->FthCSTValue)));
	
	if (RegGetStr(_REGKEY_ADDCASHMODE, ZERO2, "0") == "0")
	{
		if (memcmp(HostResp.RejectCode, "00", 2) == 0)
		{
			sprintf(ShuData[j++], "%2.2s:%2.2s:%2.2s   ATMP Accept",
				CurTime,
				&CurTime[2],
				&CurTime[4]);
		}
		else
		{
			sprintf(ShuData[j++], "%2.2s:%2.2s:%2.2s   ATMP Reject <%2.2s>",
				CurTime,
				&CurTime[2],
				&CurTime[4],
				HostResp.RejectCode);
		}
	}
	else
	{
		sprintf(ShuData[j++], "%2.2s:%2.2s:%2.2s   BRANCH ADD CASH",
		CurTime,
		&CurTime[2],
		&CurTime[4]);

//		if(m_pDevCmn->bAgentMode == 1)
//  	     	m_pTranCmn->fnAPP_AgentIdxGetData(AGENT_ADDCASH);

	}
		
//////////////////////////////////////////////////////////////////////

	fnAPD_ShuPrint(PrintDevice, TRUE);
	if (PrintDevice & DEV_SPR)
	{
		fnAPD_MaterialOut(DEV_SPR, K_1_WAIT);
		fnAPD_MaterialOutCheckRetract(DEV_SPR);
	}

//////////////////////////////////////////////////////////////////////
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//   0123456789012345678901234567890123456789
// 6:NNNNNN NNNNNNNNNN.NN P-EEEEE(SS)
/////////////////////////////////////////////////////////////////////////////
// Error Transaction Procedure
int CTranCmn::fnAPP_ErrorTranProc()
{
	DERRINFO	ERR;

	StrAllSpace(ERR.Remark);									// 2004.11.05
	memcpy(ERR.Remark, m_pProfile->TRANS.SerialNo, 8);
	memcpy(&ERR.Remark[9], Accept.Money, 10);					// 2004.11.05
	memset(&ERR.Remark[19], '.', 1);
	memcpy(&ERR.Remark[20], &Accept.Money[10], 2);
	if (!m_pDevCmn->fnAPL_CheckError())
	{
		memcpy(&ERR.Remark[23], m_pDevCmn->CurErrBuff.ProcCount, 1);
		memcpy(&ERR.Remark[24], "-", 1);
		memcpy(&ERR.Remark[25], m_pDevCmn->CurErrBuff.ErrorCode, 5);
		memcpy(&ERR.Remark[30], "(", 1);
		memcpy(&ERR.Remark[31], &m_pDevCmn->CurErrBuff.ErrorCode[5], 2);
		memcpy(&ERR.Remark[33], ")", 1);
	}

	m_pProfile->ErrTran(&ERR);
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//   0123456789012345678901234567890123456789
// 6:NNNNNN GGGGGGGGGGGGGGGGGGG NNNNNN.NN(OK)
/////////////////////////////////////////////////////////////////////////////
// Error Cancel Procedure
int CTranCmn::fnAPP_ErrorCancelProc()
{
	DERRINFO	ERR;
																// 2004.06.30
	StrAllSpace(ERR.Remark);
	memcpy(ERR.Remark, m_pProfile->TRANS.SerialNo, 8);
	memcpy(&ERR.Remark[9], CardAccountNo, strlen(CardAccountNo));
	memcpy(&ERR.Remark[29], &Accept.Money[4], 6);				// 2004.11.05
	memset(&ERR.Remark[35], '.', 1);
	memcpy(&ERR.Remark[36], &Accept.Money[10], 2);
	if (m_pDevCmn->TranResult)
		memcpy(&ERR.Remark[38], "OK", 2);
	else
		memcpy(&ERR.Remark[38], "NG", 2);

	m_pProfile->ErrCancel(&ERR);
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Error Host Procedure
int CTranCmn::fnAPP_ErrorHostProc()
{
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Print Error
int	CTranCmn::fnAPP_PrintError(int PrintDevice, int WaitFlag)
{
	int		HiddenFlag = TRUE;									// 2005.04.19
	int		nJnlIndex = 0;
	int		nSlipIndex = 0;

/////////////////////////////////////////////////////////////////////////////
//	if (MaterialOutFlag & DEV_SPR)								// 1 Transaction N Slip : 2004.07.05
//	{
//		if (PrintDevice & DEV_SPR)
//			PrintDevice &= ~DEV_SPR;
//	}

	if (PrintDevice == DEV_NONE)
		return T_OK;

	if (!AddSerialFlag)
	{
		AddSerialFlag = TRUE;
		m_pDevCmn->fnAPL_AddSerialNo();
	}

	if (MaterialRetractFlag & DEV_MCU)
		HiddenFlag = FALSE;
	else
	if (HostResp.CardRetractFlag)
		HiddenFlag = FALSE;

/////////////////////////////////////////////////////////////////////////////

	if (PrintDevice & DEV_SPR)
	{
		memset(ShuData, 0, sizeof(ShuData));

		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex +  8],  8, HiddenFlag); // by linji 20070720
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 11], 11, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 13], 13, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 16], 16, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 18], 18, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 21], 21, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 24], 24, HiddenFlag);
		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 26], 26, HiddenFlag);

//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex +  8],  8, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 10], 10, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 13], 13, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 16], 16, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 18], 18, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 20], 20, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 23], 23, HiddenFlag);
// 		fnAPP_EditPrintLine(DEV_SPR, ShuData[nSlipIndex + 25], 25, HiddenFlag);

		fnAPD_ShuPrint(DEV_SPR, WaitFlag);
	}

//	if (PrintDevice & DEV_JPR) 
//	{
//		memset(ShuData, 0, sizeof(ShuData));
//	
//		HiddenFlag = FALSE;										// 2005.03.10
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex +  7],  7, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 10], 10, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 13], 13, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 15], 15, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 18], 18, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 20], 20, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 23], 23, HiddenFlag);
//		fnAPP_EditPrintLine(DEV_JPR, ShuData[nJnlIndex + 24], 24, HiddenFlag);
//
//		fnAPD_ShuPrint(DEV_JPR, WaitFlag);
//	}

	return T_OK;
}

// Print Cancel
int	CTranCmn::fnAPP_PrintCancel(int PrintDevice, int WaitFlag)
{
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
//	Edit Function
/////////////////////////////////////////////////////////////////////////////
// Edit Account
CString CTranCmn::fstrAPP_EditAcnt(int HiddenFlag)
{
	CString strAcnt("");

	if (HMERecv.DataRecvBit[2])
	{
		int		Length = 0;
		char	szAccountNo[256];

		memset(szAccountNo, 0, sizeof(szAccountNo));

		Length = Asc2Int(HMERecv.Data[2], 2);
		memcpy(szAccountNo, &HMERecv.Data[2][2], Length);

		if (strlen(szAccountNo) >= 19)
			strAcnt = EditString(szAccountNo, 19, "##############$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 18)
			strAcnt = EditString(szAccountNo, 18, "#############$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 17)
			strAcnt = EditString(szAccountNo, 17, "############$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 16)
			strAcnt = EditString(szAccountNo, 16, "###########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 15)
			strAcnt = EditString(szAccountNo, 15, "##########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 14)
			strAcnt = EditString(szAccountNo, 14, "#########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 13)
			strAcnt = EditString(szAccountNo, 13, "########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 12)
			strAcnt = EditString(szAccountNo, 12, "#######$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 11)
			strAcnt = EditString(szAccountNo, 11, "######$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 10)
			strAcnt = EditString(szAccountNo, 10, "#####$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 9)
			strAcnt = EditString(szAccountNo, 9, "####$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 8)
			strAcnt = EditString(szAccountNo, 8, "###$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 7)
			strAcnt = EditString(szAccountNo, 7, "##$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 6)
			strAcnt = EditString(szAccountNo, 6, "#$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(szAccountNo) == 5)
			strAcnt = EditString(szAccountNo, 5, "$$$$#", '#', '$', '*', HiddenFlag);
		else
			strAcnt = "****";
	}
	else
	{
		if (strlen(CardAccountNo) >= 19)
			strAcnt = EditString(CardAccountNo, 19, "##############$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 18)
			strAcnt = EditString(CardAccountNo, 18, "#############$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 17)
			strAcnt = EditString(CardAccountNo, 17, "############$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 16)
			strAcnt = EditString(CardAccountNo, 16, "###########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 15)
			strAcnt = EditString(CardAccountNo, 15, "##########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 14)
			strAcnt = EditString(CardAccountNo, 14, "#########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 13)
			strAcnt = EditString(CardAccountNo, 13, "########$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 12)
			strAcnt = EditString(CardAccountNo, 12, "#######$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 11)
			strAcnt = EditString(CardAccountNo, 11, "######$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 10)
			strAcnt = EditString(CardAccountNo, 10, "#####$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 9)
			strAcnt = EditString(CardAccountNo, 9, "####$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 8)
			strAcnt = EditString(CardAccountNo, 8, "###$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 7)
			strAcnt = EditString(CardAccountNo, 7, "##$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 6)
			strAcnt = EditString(CardAccountNo, 6, "#$$$$#", '#', '$', '*', HiddenFlag);
		else
		if (strlen(CardAccountNo) == 5)
			strAcnt = EditString(CardAccountNo, 5, "$$$$#", '#', '$', '*', HiddenFlag);
		else
			strAcnt = "****";
	}

	return strAcnt;
}

/////////////////////////////////////////////////////////////////////////////
//	 0123456789012345678901234567890123456789
// 0:
// 1:
// 2:
// 3: 
// 4:
// 5:
// 6:    1234567890123456789     BBBBBBBB                       // Card No, Branch Number
// 7:
// 8:     CWD      RMB123456.78      00                         // Tran Kind, Amount, Response No
// 9:
//10:
//11:    1234567890123456789                                    // To Card No
//12:
//13:
//14:
//15:
//16:    123456789012    123456     AAAAAAAA                    // Reference No, Serial No, Atm Serial Number
//17:
//18:    2005.05.26    12:28:30                                 // Date & Time
//19:
//20:
//21:
//22:
//23:
//24:
/////////////////////////////////////////////////////////////////////////////
// Edit Print Line
int	CTranCmn::fnAPP_EditPrintLine(int PrintDevice, void* cData, int LineNum, int HiddenFlag)
{
	CString Data("");
	CString	strTemp("");
	char	CurDate[256];
	char	CurTime[256];
	char	szTempBuff[256];

	memset(szTempBuff, 0, sizeof(szTempBuff));

	if (PrintDevice == (DEV_SPR | DEV_JPR))
		return T_OK;

	if (PrintDevice & (DEV_SPR | DEV_JPR) == DEV_NONE)
		return T_OK;

	GetDate(CurDate);
	GetTime(CurTime);

/////////////////////////////////////////////////////////////////////////////
	if (PrintDevice & DEV_SPR)					// 收条纸打印处理   需要修改   by zjd
	{
		switch (LineNum)
		{
			case 8:												// Date Time
				if ((HMERecv.DataRecvBit[13])	&&
					(HMERecv.DataRecvBit[12]))
				{
					Data = GetSprintf("   %4.4s/%2.2s/%2.2s    %2.2s:%2.2s:%2.2s     %8.8s",
										&HMERecv.Data[13][0],
										&HMERecv.Data[13][4],
										&HMERecv.Data[13][6],
										&HMERecv.Data[12][0],
										&HMERecv.Data[12][2],
										&HMERecv.Data[12][4],
										&m_pProfile->NETWORK.AtmSerialNum[0]);
				}
				else
				{
					Data = GetSprintf("   %4.4s/%2.2s/%2.2s    %2.2s:%2.2s:%2.2s     %8.8s",
										CurDate,
										&CurDate[4],
										&CurDate[6],
										CurTime,
										&CurTime[2],
										&CurTime[4],
										&m_pProfile->NETWORK.AtmSerialNum[0]);
				}
				break;

			case 11:											// Card No, Branch Number
				Data = GetSprintf("          %19.19s",
									GetString(fstrAPP_EditAcnt(HiddenFlag).GetBuffer(0), fstrAPP_EditAcnt(HiddenFlag).GetLength(), 19));
				break;		
			
			case 13:											// Tran Kind, Amount, Response No
				if ( (TranCode == TC_WITHDRAWAL)     ||
					 (TranCode == TC_TRANSFER )	     ||
					 (TranCode == TC_HZT )			 ||			// liyi add 绿卡通
					 (TranCode == TC_TZH )	         ||
					 (TranCode == TC_REMIT_ADD)      ||
					 (TranCode == TC_REMIT_IN)       ||
					 (TranCode == TC_REMIT_PWD)      ||
					 (TranCode == TC_PAY)            ||
					 (TranCode == TC_REMIT) )
				{

//						memset(szTempBuff, ' ', 40);
//						memcpy(szTempBuff, "        RMB             RMB", 27);
						if(TranCode == TC_TRANSFER)
							{
								if(fnAPL_checkLocalCard(CString(Accept.InAccountNum)))///转外行卡提示
								{

								memset(szTempBuff, ' ', 40);
								memcpy(szTempBuff, "        RMB             RMB", 27);
								}
								else
								{
								memset(szTempBuff, ' ', 40);
								memcpy(szTempBuff, "        RMB                ", 27);
								}
					}	

						if (!(MoneyRetractFlag & DEV_CSH) || !(TranCode == TC_WITHDRAWAL))
						{
							strTemp.Format("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100);
							memcpy(&szTempBuff[12], strTemp.GetBuffer(0), strTemp.GetLength());
						}
						else
						{
							memcpy(&szTempBuff[3], "   ", 3);
						}
						
						if ((Asc2Float(HMERecv.Data[28], 8) + Asc2Float(HMERecv.Data[29], 8)) / 100 <= 0.00)
						{
							memcpy(&szTempBuff[24], "   ", 3);
						}
						else
						{
							strTemp.Format("%.2f", (Asc2Float(HMERecv.Data[28], 8) + Asc2Float(HMERecv.Data[29], 8)) / 100);
							memcpy(&szTempBuff[27], strTemp.GetBuffer(0), strTemp.GetLength());
						}
						Data = GetSprintf("%-40.40s", szTempBuff);
					
				}
				if((TranCode == TC_AC_PAY_ASSU)    ||
				   (TranCode == TC_AC_PAY_NOASSU)  ||
				   (TranCode == TC_AC_REMIT_FIX)   ||
				   (TranCode == TC_AC_REMIT_PWD)   ||
				   (TranCode == TC_AC_REMIT))
				{
					CString			strReturn("");
					CString			strLeft("");
					CString			strRight("");
					CStringArray	strReturnArray;

					memset(szTempBuff, ' ', 40);
					memcpy(szTempBuff, "        RMB             RMB ", 28);
					strTemp.Format("%.2f", atof(ACHostSend.TranMoney));
 					memcpy(&szTempBuff[12], strTemp.GetBuffer(0), strTemp.GetLength());


// 					memcpy(&szTempBuff[12], ACHostSend.TranMoney,sizeof(ACHostSend.TranMoney));


					if ( Asc2Float(ACHostResp.CommCharge, sizeof(ACHostResp.CommCharge)) <= 0.00)
					{
						memcpy(&szTempBuff[24], "   ", 3);
					}
					else
					{
						memcpy(&szTempBuff[28], ACHostResp.CommCharge, sizeof(ACHostResp.CommCharge));
					}
					Data = GetSprintf("%-40.40s", szTempBuff);
				}

				break;		

			case 16:											// To Card No
				if (TranCode == TC_TRANSFER)
					Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				else if (TranCode == TC_HZT)					// liyi add 绿卡通
				{
					Data = GetSprintf("    %4.4s", &HMERecv.Data[103][2]);
				}
				else if (TranCode == TC_TZH)
				{
					Data = GetSprintf("    0001");
				}
				else if (TranCode == TC_REMIT_ADD)
					Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				else if (TranCode == TC_REMIT_IN)
					Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				else if (TranCode == TC_REMIT_PWD)
					Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				else if (TranCode == TC_PAY)
					Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				else 
				if (  (TranCode == TC_AC_REMIT)        ||
					  (TranCode == TC_AC_REMIT_FIX)    ||
					  (TranCode == TC_AC_REMIT_PWD)  )
				{
					if( sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount) >0 )
					{
 						 memcpy(&ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount[GetSprintf("%s",ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount).GetLength()-7],"***",3);
                         Data = GetSprintf("    %-28.28s", &ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount); //收款账号要分开处理BY HYL

// 						Data = GetSprintf("    ***%4.4s", &(GetSprintf("%s",ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount).GetBuffer(0)[sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount)-4]),4); //收款账号要分开处理BY HYL
				         MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "payeeaccount[%s]",ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount);
					}
				}
				else
				if((TranCode == TC_AC_PAY_ASSU) )
				{
                   Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				}
				else
				if(TranCode == TC_AC_PAY_NOASSU)
				{
				   Data = GetSprintf("    %-28.28s", Accept.InAccountNum);
				}
				break;

			case 18:											// 交易标志   by zjd
				if (TranCode == TC_WITHDRAWAL)
				{
					Data = GetSprintf("     ***");
				}
				else
				if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)	// liyi add 绿卡通
				{
					Data = GetSprintf("                                ***");
				}
				else
				if ( (TranCode == TC_AC_PAY_ASSU)      ||
					 (TranCode == TC_AC_PAY_NOASSU)    ||
					 (TranCode == TC_AC_REMIT)         ||
					 (TranCode == TC_AC_REMIT_FIX)     ||
					 (TranCode == TC_AC_REMIT_PWD)  )
				{
					Data = GetSprintf("     ***                        ***");
				}
				break;

		
			case 21:											// Reference No, Serial No, Atm Serial Number
				if (TranCode == TC_WITHDRAWAL)
				{
					Data = GetSprintf("                %8.8s       %2.2s",
										WithSerialNo,
										HostResp.RejectCode);
				}
				else
				if (TranCode == TC_TRANSFER || TranCode == TC_HZT || TranCode == TC_TZH)	// liyi add 绿卡通
				{
					Data = GetSprintf("                %8.8s       %2.2s",
										&m_pProfile->TRANS.SerialNo[0],
										HostResp.RejectCode);
				}
				else
				if ( (TranCode == TC_AC_PAY_ASSU)      ||
					 (TranCode == TC_AC_PAY_NOASSU)    ||
				     (TranCode == TC_AC_REMIT)         ||
					 (TranCode == TC_AC_REMIT_FIX)     ||
					 (TranCode == TC_AC_REMIT_PWD))
				{
					//A/C MANAGER 流水号和应答码
					Data = GetSprintf("                %8.8s   %d",
										&m_pProfile->TRANS.SerialNo,
										ACHostResp.ReturnCode);
					
				}
				else
				{
					Data = GetSprintf("     ***        %8.8s       %2.2s",
										&m_pProfile->TRANS.SerialNo[0],
										HostResp.RejectCode);
				}
				break;		
		
			case 24:											// Card Retract
				if ((MaterialRetractFlag & DEV_MCU)	||
					(HostResp.CardRetractFlag))
				{
					Data = GetSprintf("        ***                   ***",
										&HMERecv.Data[13][0],
										&HMERecv.Data[13][4],
										&HMERecv.Data[13][6],
										&HMERecv.Data[12][0],
										&HMERecv.Data[12][2],
										&HMERecv.Data[12][4],
										&m_pProfile->NETWORK.AtmSerialNum[0]);
				}
				break;
		
			case 26:											// Error Code			
				if (HostResp.CardRetractFlag)
					Data = GetSprintf("       CARD RETAIN  %12.12s", HMERecv.Data[37]);
				else
				if (CashDispErrorFlag == 1)
				{
					Data = GetSprintf("    CASH DISPENSE ERROR %12.12s", HMERecv.Data[37]);
				    fnAPP_SuspiciProc();
				}
				else
				if ((MoneyRetractFlag & DEV_CSH) && (TranCode == TC_WITHDRAWAL))
				{
					Data = GetSprintf("   CASH RETRACT(TIME OUT) %12.12s", HMERecv.Data[37]);
				    fnAPP_SuspiciProc();
				}
				else
				if (TransCommTimeOut) 
				{
					Data = GetSprintf("    TIME OUT");
				    fnAPP_SuspiciProc();
				}
				else
				if (CancelPrintFlag)
					Data = GetSprintf("    TRANSACTION FAIL %12.12s", HMERecv.Data[37]);
				else
				if (TranCode == TC_WITHDRAWAL || TranCode == TC_TRANSFER)
					Data = GetSprintf("    TRANSACTION SUCCESS %12.12s", HMERecv.Data[37]);
				else
				if (TranCode == TC_HZT)			// liyi add 绿卡通
				{
					if (m_pTranCmn->strTFRSubType == "7")		//整整
					{
						if (Asc2Float(m_pTranCmn->strTFRInData,2) >= 12)
						{
							Data = GetSprintf("    TRANSACTION SUCCESS HZD %.0f YEAR", Asc2Float(m_pTranCmn->strTFRInData,2)/12);
						}
						else
						{
							Data = GetSprintf("    TRANSACTION SUCCESS HZD %s MONTH", m_pTranCmn->strTFRInData);
						}
						
					}
					else if (m_pTranCmn->strTFRSubType == "8")	//定活
					{
						Data = GetSprintf("    TRANSACTION SUCCESS HZD");
					}
					else if (m_pTranCmn->strTFRSubType == "9")	//通知
					{
						Data = GetSprintf("    TRANSACTION SUCCESS HZD %s DAY", m_pTranCmn->strTFRInData);
					}
					
				}
				else
				if (TranCode == TC_TZH)
				{
					Data = GetSprintf("    TRANSACTION SUCCESS %4.4s DZH", m_pTranCmn->strTFRSubNo);
				}
				else
				if (TranCode == TC_PAY)         //BY HYL 
				{  
				   CString DLPritType;
				   if(PayType == S_PAY1)
					   DLPritType=GetSprintf("DJF");
				   else 
				   if(PayType == S_PAY2)
					   DLPritType=GetSprintf("SAL");
				   Data = GetSprintf("    TRANSACTION SUCCESS %s %12.12s",DLPritType, HMERecv.Data[37]);
				}
				else
				if ((TranCode == TC_AC_PAY_ASSU)  ||
				    (TranCode == TC_AC_PAY_NOASSU)) 	
				{  
				   CString DLPritType;
				   DLPritType=GetSprintf("DJF");

				   if(ACHostResp.ReturnCode == 0)
				     Data = GetSprintf("    TRANSACTION SUCCESS %s %s",DLPritType, ACHostResp.OperServiceNo1);  
				   else
					 Data = GetSprintf("    TRANSACTION FAIL %s %s",DLPritType, ACHostResp.OperServiceNo1);  
					 
				}
				else
				if (TranCode == TC_AC_REMIT) 
				{  
				   CString DLPritType;
				   DLPritType=GetSprintf("AZHK");
				   if(ACHostResp.ReturnCode == 0)
				   {
					   if( Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 4 ||
						   Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 6)           //异地本行 是 汇款业务
					   {
				         Data = GetSprintf("    TRANSACTION SUCCESS %s %s",DLPritType, ACHostResp.OperServiceNo);  
					   }
					   else
					   {
				         Data = GetSprintf("    TRANSACTION SUCCESS %s %s",DLPritType,ACHostResp.HostServiceNo1);  
					   }
				   }
				   else
				   { 
					   if( Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 4 ||
						   Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 6)
					   {
					      Data = GetSprintf("    TRANSACTION FAIL %s %s",DLPritType, ACHostResp.OperServiceNo);  
					   }
					   else
					   {
					      Data = GetSprintf("    TRANSACTION FAIL %s %s",DLPritType, ACHostResp.HostServiceNo1);  
					   }
				   }
				}
				else
				if (TranCode == TC_AC_REMIT_FIX) 
				{  
				   CString DLPritType;
				   DLPritType=GetSprintf("DQZZ");
				   if(ACHostResp.ReturnCode == 0)
				     Data = GetSprintf("    TRANSACTION SUCCESS %s %s",DLPritType, ACHostResp.HostServiceNo1);  
				   else
					 Data = GetSprintf("    TRANSACTION FAIL %s %s",DLPritType, ACHostResp.HostServiceNo1);  
				}
				else
				if (TranCode == TC_AC_REMIT_PWD) 
				{  
				   CString DLPritType;
				   DLPritType=GetSprintf("MMHK");
				   if(ACHostResp.ReturnCode == 0)
				     Data = GetSprintf("    TRANSACTION SUCCESS %s %s",DLPritType, ACHostResp.OperServiceNo);  
				   else
					 Data = GetSprintf("    TRANSACTION FAIL %s %s",DLPritType, ACHostResp.OperServiceNo);  
				}
				else
				if (!m_pDevCmn->fnAPL_CheckError())
				{
//					Data = GetSprintf("           ERROR MTC : %1.1c-%5.5s(%2.2s)",
//										m_pDevCmn->CurErrBuff.ProcCount[0],
//										m_pDevCmn->CurErrBuff.ErrorCode,
//										&m_pDevCmn->CurErrBuff.ErrorCode[5]);
					Data = GetSprintf("       %s", m_pDevCmn->CurErrBuff.ErrorString);
				}
				break;

			case 28:
				if((TranCode == TC_AC_REMIT_PWD) ||
				   (TranCode == TC_AC_REMIT_FIX) ||
				   (TranCode == TC_AC_REMIT))
				{
					if(ACHostResp.ReturnCode == 0 && GetSprintf("%s",ACHostResp.PostalOrderNo).GetLength() >0)
                      Data = GetSprintf("    HKPH: %18.18s ",ACHostResp.PostalOrderNo); 
                    else
                      Data = GetSprintf(""); 
					  
				}
				break;
				
				

			default:
				break;
		}
	}
	else
	if (PrintDevice & DEV_JPR)									// 流水打印处理  需要修改  by zjd
	{
		switch (LineNum)
		{
			case 7:												// Atm Serial Number / Date
				if ((TranCode == TC_OPEN)		||
					(TranCode == TC_WITHDRAWAL))
					;
				else
					break;
				
				if ((HMERecv.DataRecvBit[13])	&&
					(HMERecv.DataRecvBit[12]))
				{
					Data = GetSprintf("ATM ID / DATE : %8.8s %4.4s/%2.2s/%2.2s %2.2s:%2.2s:%2.2s",
										&m_pProfile->NETWORK.AtmSerialNum[0], 
										&HMERecv.Data[13][0],
										&HMERecv.Data[13][4],
										&HMERecv.Data[13][6],
										&HMERecv.Data[12][0],
										&HMERecv.Data[12][2],
										&HMERecv.Data[12][4]);
				}
				else
				{
					Data = GetSprintf("ATM ID / DATE : %8.8s %4.4s/%2.2s/%2.2s %2.2s:%2.2s",
										&m_pProfile->NETWORK.AtmSerialNum[0], 
										&CurDate[0],
										&CurDate[4],
										&CurDate[6],
										CurTime,
										&CurTime[2]);
				}
				break;		
			
			case 10:											// Transaciton Type
				if (m_pDevCmn->TranStatus == TRAN_TRAN)
				{
					Data = GetSprintf("TRAN TYPE     : %16.16s",
										TranCodeName[TranCode]);
				}
				else
				{
					if (m_pDevCmn->TranStatus == TRAN_READY)
						Data = GetSprintf("TRAN TYPE     : READY");
					else
					if (m_pDevCmn->TranStatus == TRAN_OPEN)
						Data = GetSprintf("TRAN TYPE     : RQK");
					else
					if (m_pDevCmn->TranStatus == TRAN_CANCEL)
						Data = GetSprintf("TRAN TYPE     : CWC");    
					else
					if (m_pDevCmn->TranStatus == TRAN_ERROR)
						Data = GetSprintf("TRAN TYPE     : ERROR");
					else
					if (m_pDevCmn->TranStatus == TRAN_AEXSTATUS)
						Data = GetSprintf("TRAN TYPE     : AEXSTATUS");
					else
					if (m_pDevCmn->TranStatus == TRAN_OEXSTATUS)
						Data = GetSprintf("TRAN TYPE     : OEXSTATUS");
					else
					if (m_pDevCmn->TranStatus == TRAN_MSRSTATUS)
						Data = GetSprintf("TRAN TYPE     : MSRSTATUS");
					else
					if (m_pDevCmn->TranStatus == TRAN_BIDCOMMAND)
						Data = GetSprintf("TRAN TYPE     : BIDCOMMAND");
					else
					if (m_pDevCmn->TranStatus == TRAN_TRAN)
						Data = GetSprintf("TRAN TYPE     : TRANSACTION");
					else
					if (m_pDevCmn->TranStatus == TRAN_TOTAL)
						Data = GetSprintf("TRAN TYPE     : TOTAL");
					else
					if (m_pDevCmn->TranStatus == TRAN_SUBTOTAL)
						Data = GetSprintf("TRAN TYPE     : SUBTOTAL");
					else
					if (m_pDevCmn->TranStatus == TRAN_ADDCASH)
						Data = GetSprintf("TRAN TYPE     : ADDCASH");
					else
					if (m_pDevCmn->TranStatus == TRAN_SUBCASH)
						Data = GetSprintf("TRAN TYPE     : SUBCASH");
					else
					if (m_pDevCmn->TranStatus == TRAN_CLEARCASH)
						Data = GetSprintf("TRAN TYPE     : CLEARCASH");
					else
					if (m_pDevCmn->TranStatus == TRAN_PRINTCASH)
						Data = GetSprintf("TRAN TYPE     : PRINTCASH");
					else
					if (m_pDevCmn->TranStatus == TRAN_DOWNFIT)
						Data = GetSprintf("TRAN TYPE     : DOWNFIT");
					else
					if (m_pDevCmn->TranStatus == TRAN_SENDDETAIL)
						Data = GetSprintf("TRAN TYPE     : SENDDETAIL");
					else
					if (m_pDevCmn->TranStatus == TRAN_DETAILEND)
						Data = GetSprintf("TRAN TYPE     : DETAILEND");
					else
					if (m_pDevCmn->TranStatus == TRAN_CONFIRMWITH)
						Data = GetSprintf("TRAN TYPE     : CONFIRMWITH");
					else
					if (m_pDevCmn->TranStatus == TRAN_DCSENDEJR)// 2006.07.03
						Data = GetSprintf("TRAN TYPE     : DCSENDEJR");
					else
					if (m_pDevCmn->TranStatus == TRAN_DCEJREND)	// 2006.07.03
						Data = GetSprintf("TRAN TYPE     : DCEJREND");
				}
				break;		

			case 11:											// Open Parameter
				break;		
			
			case 13:											// To Card No
				if (TranCode == TC_TRANSFER)
					Data = GetSprintf("TO CARD NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_HZT)					// liyi add 绿卡通
					Data = GetSprintf("TO ACC  NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_TZH)
					Data = GetSprintf("TO ACC  NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_REMIT_ADD)
					Data = GetSprintf("TO ACC  NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_REMIT_IN)
					Data = GetSprintf("TO ACC  NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_REMIT_PWD)
					Data = GetSprintf("TO ACC  NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_PAY)
					Data = GetSprintf("TO PAY  NO: %-28.28s", Accept.InAccountNum); 
				else if (TranCode == TC_AC_PAY_ASSU )	
                    Data = GetSprintf("TO A/C PAY  NO: %-28.28s", ACHostRespPayAssu[PayAssuCn].JHNumber);
				else if (TranCode == TC_AC_PAY_NOASSU)	
                    Data = GetSprintf("TO A/C PAY  NO: %-28.28s", Accept.InAccountNum);				
				else if (TranCode == TC_AC_REMIT)  
                    Data = GetSprintf("TO A/C REMIT NO: %-28.28s",ACHostRespRemitAssu[RemitCn].PayeeAccount);
				else if ( TranCode == TC_AC_REMIT_FIX)
					Data = GetSprintf("TO A/C REMIT NO: %-28.28s", ACHostRespRemitAssu[RemitCn].PayeeAccount);
				else if ( TranCode == TC_AC_REMIT_PWD)
                    Data = GetSprintf("TO A/C REMIT NO: %-28.28s", ACHostRespRemitAssu[RemitCn].PayeePostCode);


				break;

			case 15:											// Amount 2005.05.26
				if (TranCode == TC_CANCEL)
				{
					strTemp.Format("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100);
					Data = GetSprintf("AMOUNT        : RMB%s", strTemp);
				}
				break;		
		
			case 18:											// Org Serial No
				if (TranCode == TC_CANCEL)
					Data = GetSprintf("ORG SERIAL NO : %6.6s", OrgSerialNo);
				break;

			case 20:											// Serial No
				if (TranCode == TC_WITHDRAWAL)					// 2006.07.31
				{
					Data = GetSprintf("SER/REF/RES NO: %8.8s %12.12s[%2.2s]", 
										WithSerialNo,
										HMERecv.Data[37],
										HostResp.RejectCode);
				}
				else
				{
					Data = GetSprintf("SER/REF/RES NO: %8.8s %12.12s[%2.2s]", 
										&m_pProfile->TRANS.SerialNo[0],
										HMERecv.Data[37],
										HostResp.RejectCode);
				}
				break;		
		
			case 23:											// Response No : 2004.08.19
				if (m_pDevCmn->TranResult)
					;
				else
					Data = GetSprintf("RESPONSE NO   : >>>>> %2.2s <<<<<",
									HostResp.RejectCode);
				break;		

			case 24:											// Error Code
				if (!m_pDevCmn->fnAPL_CheckError())
				{
					Data = GetSprintf("ERROR MTC     : %1.1c-%5.5s(%2.2s)", 
										m_pDevCmn->CurErrBuff.ProcCount[0],
										m_pDevCmn->CurErrBuff.ErrorCode,
										&m_pDevCmn->CurErrBuff.ErrorCode[5]);
				}
				break;

			default:
				break;
		}
	}

	memcpy((char*)cData, Data.GetBuffer(Data.GetLength()), Data.GetLength());
	return T_OK;
}


int CTranCmn::fnAPP_SuspiciProc()      //可疑交易处理 BY HYL
{
 MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SuspiciProc()");
	CString iData("");
	if ( TranCode == TC_WITHDRAWAL)
	{
		iData = "CWD   CASH ERROR    ";
		if(MoneyRetractFlag & DEV_CSH)  
			iData = "CWD   CASH RETRACT  ";
		if (TransCommTimeOut) 
			iData = "CWD   TIME OUT      ";
		if (memcmp(HostResp.RejectCode, "Z1", 2) == 0)
            iData = "CWD   HOST TIME OUT ";
	}
	else
	if ( TranCode == TC_TRANSFER)
	{
		iData= "TRF   TRF ERROR     ";
		if (TransCommTimeOut) 
			iData = "TRF   TIME OUT      ";
		if (memcmp(HostResp.RejectCode, "Z1", 2) == 0)
			iData = "TRF   HOST TIME OUT ";
	}
	else
	if ( TranCode == TC_HZT)		// liyi add 绿卡通
	{
		iData= "HZT   HZT ERROR     ";
		if (TransCommTimeOut) 
			iData = "HZT   TIME OUT      ";
		if (memcmp(HostResp.RejectCode, "Z1", 2) == 0)
			iData = "HZT   HOST TIME OUT ";
	}
	else
	if ( TranCode == TC_TZH)
	{
		iData= "TZH   TZH ERROR     ";
		if (TransCommTimeOut) 
			iData = "TZH   TIME OUT      ";
		if (memcmp(HostResp.RejectCode, "Z1", 2) == 0)
			iData = "TZH   HOST TIME OUT ";
	}
	else
	if (m_pDevCmn->TranStatus == TRAN_CANCEL)
			  iData= "CWC   CWC ERROR     ";
	int iSuspCnt = IniGetInt(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, "SUSPICIOUSNUM", 0);
	IniSetStr(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, GetSprintf("SUSPICIOUSLIST1%03.3d",iSuspCnt),
		GetSprintf("%03.3d PAN: %19.19s RMB%.2f",
		iSuspCnt + 1,
		CardAccountNo,
		Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100
		));
	IniSetStr(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, GetSprintf("SUSPICIOUSLIST2%03.3d",iSuspCnt),
		GetSprintf("SerialNo: %8.8s %20.20s ", &m_pProfile->TRANS.SerialNo[0],
		iData
		));
	IniSetInt(_TRANSACTION_INI, _INIKEY_SUSPICIOUS, "SUSPICIOUSNUM", iSuspCnt + 1);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_SuspiciProc() RETURN ");
	return T_OK;

  }