/////////////////////////////////////////////////////////////////////////////
// TranInput.cpp : implementation file
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
//	Calculate Money Function
/////////////////////////////////////////////////////////////////////////////
// Calculate Money Max Out
int CTranCmn::fnAPP_CalcMoneyMaxOut()
{
/////////////////////////////////////////////////////////////////////////////
	MoneyMaxOut = fnAPP_CalcMoneyMaxOut10050();
/////////////////////////////////////////////////////////////////////////////

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOut:MoneyMaxOut[%d]", MoneyMaxOut);
	return MoneyMaxOut;
}

// Calculate Money Max Out 100 50
int CTranCmn::fnAPP_CalcMoneyMaxOut10050()
{
	int		MONEY_MAXOUT = InitInfo.WithMaxAmt;					// Withdraw Max Amount

	if ((MONEY_MAXOUT >= 1)	&&
		(MONEY_MAXOUT <= CSH_M10050_MAXOUT))					// 2004.11.05
		;
	else 
		MONEY_MAXOUT = CSH_M10050_MAXOUT;
	
	MoneyMaxOut = 0;
	switch (WithAvail)
	{
		case TRAN_WITH_100:
		case TRAN_WITH_BOTH:
			MoneyMaxOut = m_pDevCmn->fnCSH_GetAmountOfCST();	// 2004.11.05
			break;

		case TRAN_WITH_50:
			MoneyMaxOut = m_pDevCmn->fnCSH_GetAmountOfCST();	// 2004.11.05
			if (MoneyMaxOut <= (CASH_CHINA_50 * CSH_EMPTYCASHCNT2))
			{
				if (MoneyMaxOut > (CASH_CHINA_50 * m_pDevCmn->CashFixCashCnt))
					  MoneyMaxOut -= (CASH_CHINA_50 * m_pDevCmn->CashFixCashCnt);
			}

			if (MoneyMaxOut > (CASH_CHINA_50 * CSH_MAXCASHOUTCNT2))
			{
				MoneyMaxOut = (CASH_CHINA_50 * CSH_MAXCASHOUTCNT2);		
			}
			break;

		case TRAN_WITH_NOT:
		default: 
			MoneyMaxOut = 0;
			break;
	}

	if (MoneyMaxOut > MONEY_MAXOUT)
		MoneyMaxOut = MONEY_MAXOUT;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_CalcMoneyMaxOut10050:MoneyMaxOut[%d]", MoneyMaxOut);
	return MoneyMaxOut;
}

// Calculate Transfer Money Max Out
double CTranCmn::fdAPP_CalcTMoneyMaxOut()
{
	TMoneyMaxOut = InitInfo.TransferMaxAmt;						// Transfer Max Amount

	if ((TMoneyMaxOut >= 1)	&&
		(TMoneyMaxOut <= CSH_MONEY_TMAXOUT))
		;
	else 
		TMoneyMaxOut = 0;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fdAPP_CalcTMoneyMaxOut:TMoneyMaxOut[%.2f]", TMoneyMaxOut);
	return TMoneyMaxOut;
}

/////////////////////////////////////////////////////////////////////////////
//	Input function
/////////////////////////////////////////////////////////////////////////////
// Accept Withdraw
int CTranCmn::fnAPP_AcceptWith()
{
	fnAPP_AcceptMoney();										// Accept Money
	if (MainTranReturnFlag)
		return T_OK;
	return T_OK;
}

// Accept Inquiry
int CTranCmn::fnAPP_AcceptInq()
{
	CString strReturn;

	if (TranCode == TC_CHANGEPIN)								// Accept New PassWord
	{
		fnAPP_AcceptNewPassWord(0);
		if (MainTranReturnFlag)
			return T_OK;
	}
	else
	{															//��Ҫ���ж��Ƿ��Ǳ����̿���Ȼ����ʾ��ѯ�Ӳ˵�   by zjd
		if ((LocalCardFlag  == 1)||(LocalCardFlag == 7))		//20080904 by yaokq �����̿�ͨ�����ж�
		{
			while (TRUE)
			{
				m_pDevCmn->fnSCR_DisplayScreen(16, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				else
				if (strReturn == "DETAIL")
				{
					TranCode = TC_DETAIL;
					break;
				}
				else
					break;								//Continue   by zjd
			}
		}
		else if(LocalCardFlag == 5)      // 20080904 by yaokq for �̿�ͨ����
		{
			while(TRUE)
			{
				m_pDevCmn->fnSCR_DisplayScreen(36, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_TIMEOVER)
					fnAPP_CancelProc(T_TIMEOVER);
				else if (strReturn == S_EXIT)
					fnAPP_CancelProc(T_EXIT);
				else if (strReturn == S_CANCEL)
					fnAPP_CancelProc(T_CANCEL);
				else if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else if(strReturn == "DETAIL")
				{
					TranCode = TC_DETAIL;
					break;
				}
				else if(strReturn == "OTHER")
				{
					TranCode = TC_OTHER_INQ;
					break;
				}
				else
					break;
			}
		}
	}
	return T_OK;
}

int	CTranCmn::fnAPP_Trans_ToOtherMoney()						//����ת����Ʒ�ֽ��  liyi add �̿�ͨ
{
	int				iNGCnt = 0;
	int				ScrNum = 0;
	int				ReTryCnt = 0;
	CString			strReturn("");
	CString			strReturn1("");
	CString			strInAmount("");
	CString			strLeft("");
	CString			strRight("");
	CStringArray	strReturnArray;
	CString			strFee("");
	CString			strTFRType("");
	CString			strTFRData("");


/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
/////////////////////////////////////////////////////////////////////////////

	ReTryCnt = 3;
	while (ReTryCnt)
	{
		TranCode = TC_INQUIRY;
		TranCode2 = 0;

		if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
		{
			m_pDevCmn->TranResult = TRUE;
		}
		else
		{
			fnAPP_SendHost();
 			fnAPP_RecvHost();
//			m_pDevCmn->TranResult = TRUE;
		}

		TranCode = TC_HZT;
		if (m_pDevCmn->TranResult)
		{
			ScrNum = 46;
			if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)
			{
				m_pDevCmn->fnSCR_DisplayString(4, "500000.00");
			}
			else
			{
				if ((HMERecv.Data[54][0] == 'D') ||
					(HMERecv.Data[54][0] == 'd'))
				{
					m_pDevCmn->fnSCR_DisplayString(4, 
						GetSprintf("%.2f", Asc2Float(&HMERecv.Data[54][1], 12) / 100));			//��ʾ���
				}
				else
				{
					m_pDevCmn->fnSCR_DisplayString(4, 
						GetSprintf("-%.2f", Asc2Float(&HMERecv.Data[54][1], 12) / 100));
 				}
//				m_pDevCmn->fnSCR_DisplayString(4, 
// 										GetSprintf("%.2f", Asc2Float("000000500000") / 100));			//��ʾ���
			}

			m_pDevCmn->fnSCR_DisplayString(2, 
				IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMINNUM,"50"));		//������С���
			m_pDevCmn->fnSCR_DisplayString(3, 
				IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMINNUM,"50000"));	//֪ͨ��С���
			m_pDevCmn->fnSCR_DisplayString(5, m_pTranCmn->strTFRSubType);									//���ݸ�flash����Ϊ�����ж�
			m_pDevCmn->fnSCR_DisplayString(6, 
							IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMAXNUM,"50000"));
			m_pDevCmn->fnSCR_DisplayString(7, 
							IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMAXNUM,"50000"));
			m_pDevCmn->fnSCR_DisplayString(8, 
							IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM,"50000"));


			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

			strReturn = m_pDevCmn->fstrSCR_GetKeyString();				
			if (strReturn == S_EXIT)
			{
//				fnAPP_CancelProc(T_EXIT);
				HZTFlag = S_EXIT;						// added by yusy 20110526
				return T_OK;
			}
			else 
			if (strReturn == S_CANCEL)
			{
//				fnAPP_CancelProc(T_CANCEL);
				HZTFlag = S_CANCEL;						// added by yusy 20110526
				return T_OK;
			}
			else
			if (strReturn == S_TIMEOVER)
			{
//				fnAPP_CancelProc(T_TIMEOVER);
				HZTFlag = S_TIMEOVER;					// added by yusy 20110526
				return T_OK;
			}
			else
			if (strReturn == S_INPUTOVER)
			{
//				fnAPP_CancelProc(T_INPUTOVER);
				HZTFlag = S_INPUTOVER;					// added by yusy 20110526
				return T_OK;
			}
			else
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;											//�����أ����������׻���  by zjd
			}
			
			strInAmount = strReturn;

			SplitString(strReturn, ".", strReturnArray);
			if (strReturnArray.GetSize() == 1)
			{
				if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
					fnAPP_CancelProc(T_INPUTERROR);

				strLeft = ZERO10 + strReturnArray[0];
				strLeft = strLeft.Right(10);
				strRight = ZERO2;										// 2004.11.05
			}
			else
			if (strReturnArray.GetSize() == 2)
			{
				if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
					fnAPP_CancelProc(T_INPUTERROR);
				if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
					fnAPP_CancelProc(T_INPUTERROR);

				strLeft = ZERO10 + strReturnArray[0];
				strLeft = strLeft.Right(10);
				strRight = strReturnArray[1] + ZERO2;
				strRight = strRight.Left(2);
			}
			else
				fnAPP_CancelProc(T_INPUTERROR);

			if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
				(Asc2Int(strRight) <= 0))
				fnAPP_CancelProc(T_INPUTERROR);
//			else
//			if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > Asc2Float(&HMERecv.Data[54][1], 12) / 100) ||
//				((HMERecv.Data[54][0] != 'D') && (HMERecv.Data[54][0] != 'd')))
//				fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			else
			if (m_pTranCmn->strTFRSubType == "7")						//���� amt check
			{
				if ((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > 
					Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMAXNUM,"50000")))
					fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}
			else
			if (m_pTranCmn->strTFRSubType == "8")						//���� amt check
			{
				if ((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > 
					Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMAXNUM,"50000")))
					fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}
			else
			if (m_pTranCmn->strTFRSubType == "9")						//֪ͨ amt check
			{
				if ((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > 
					Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM,"50000")))
					fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			}

			memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
			memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
			break;

			if (!--ReTryCnt)
			{
				fnAPP_CancelProc(T_INPUTOVER);
			}
		}
		else 
		if (memcmp(HostResp.RejectCode, "26", 2) == 0)		// ��ʾ��������������ҳ��  by zjd
		{
			if (iNGCnt <3)
			{
				iNGCnt ++;
				fnAPP_PMEAcceptPassWord();						// Accept PassWord
				continue;
			}
			else
				fnAPP_CancelProc(T_CANCEL);
		}
		else														// ��ʾ������Ϣ��������Ӧ����  by zjd
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
				fnAPP_CancelProc(T_CANCEL);
			}
			else
			if (HostResp.CardEjectFlag)								// Card Eject Flag
			{
				fnAPP_PMDProc();									// Message Display
				fnAPD_CheckDeviceAction(DEV_MCU);
				fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
				fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
				fnAPP_CancelProc(T_CANCEL);
			}	
			else
			{
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();							// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																// Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
				{
					fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
					MainTranReturnFlag = TRUE;
					return T_OK;								//�����أ����������׻���  by zjd
				}
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	//���Կͻ�����Ľ��
	ScrNum = 47;
	m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%.2f",Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));

	if (m_pTranCmn->strTFRSubType == "7")
	{
		strTFRType = "����";
	}
	else if (m_pTranCmn->strTFRSubType == "8")
	{
		strTFRType = "����";
	}
	else if (m_pTranCmn->strTFRSubType == "9")
	{
		strTFRType = "֪ͨ";
	}
	m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s",strTFRType));

	if (m_pTranCmn->strTFRInData == "03")
	{
		strTFRData = "������";
	}
	else if (m_pTranCmn->strTFRInData == "06")
	{
		strTFRData = "����";
	}
	else if (m_pTranCmn->strTFRInData == "12")
	{
		strTFRData = "һ��";
	}
	else if (m_pTranCmn->strTFRInData == "24")
	{
		strTFRData = "����";
	}
	else if (m_pTranCmn->strTFRInData == "36")
	{
		strTFRData = "����";
	}
	else if (m_pTranCmn->strTFRInData == "60")
	{
		strTFRData = "����";
	}
	else if (m_pTranCmn->strTFRInData == "01")
	{
		strTFRData = "һ��";
	}
	else if (m_pTranCmn->strTFRInData == "07")
	{
		strTFRData = "����";
	}
	else if (m_pTranCmn->strTFRInData == "00")
	{
		strTFRData = "";
	}
	m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s",strTFRData));

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

	strReturn1 = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn1 == S_EXIT)
		fnAPP_CancelProc(T_EXIT);
	else 
	if (strReturn1 == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn1 == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn1 == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if (strReturn1 == S_RETURN)
	{
		MainTranReturnFlag = TRUE;
		return T_OK;				//�����أ����������׻���  by zjd
	}
	return T_OK;
}


int	CTranCmn::fnAPP_Trans_LiveToOther()							//����ת����Ʒ�� liyi add �̿�ͨ
{
	CString strReturn;
	CString	strPara;
	int		ScrNum;

	strPara=IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SAVEKINDPARA,"00000000");
	
	while (TRUE)
	{															//ѡ��������������
		m_pDevCmn->fnSCR_DisplayScreen(43, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;										
		}
		else
		if (strReturn == "7")									//������ȡ
		{
			m_pTranCmn->strTFRSubType = strReturn;
			if (strPara.GetAt(1) == '0')						//�˽��ױ���ֹ
			{
				ScrNum = 53;											
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_CONTINUE)
				{
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
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
					fnAPP_CancelProc(T_PROGRAM);
			}
			while (TRUE) 
			{									
				m_pDevCmn->fnSCR_DisplayScreen(44, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;									
				}
				else
				{
					m_pTranCmn->strTFRInData = strReturn;
				}
				break;
			}	
		}
		else
		if (strReturn == "9")									//֪ͨ���
		{
			m_pTranCmn->strTFRSubType = strReturn;
			if (strPara.GetAt(2) == '0')						//�˽��ױ���ֹ
			{
				ScrNum = 53;											
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_CONTINUE)
				{
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
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
					fnAPP_CancelProc(T_PROGRAM);
			}
			while (TRUE) 
			{													
				m_pDevCmn->fnSCR_DisplayScreen(45, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;									
				}
				else
				{
					m_pTranCmn->strTFRInData = strReturn;
				}
				break;
			}	
		}
		else
		if (strReturn == "8")									//��������
		{
			if (strPara.GetAt(0) == '0')						//�˽��ױ���ֹ
			{
				ScrNum = 53;											
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_CONTINUE)
				{
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
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
					fnAPP_CancelProc(T_PROGRAM);
			}
			m_pTranCmn->strTFRSubType = strReturn;
			m_pTranCmn->strTFRInData = "00";
			break;						
		}
		break;
	}

	fnAPP_Trans_ToOtherMoney();
	if (MainTranReturnFlag)
		return T_OK;

	return T_OK;
}
int CTranCmn::fnAPP_Trans_OtherToLive()						//����Ʒ��ת���� liyi add �̿�ͨ
{
	int				iNGCnt = 0;
	int				ScrNum = 0;
	int				ReTryCnt = 0;
	CString			strReturn("");
	CString			strReturn1("");
	CString			strLeft("");
	CString			strRight("");
	CStringArray	strReturnArray;
	CString			strFee("");
	int				iChoiceNo = -1;

	// 20080908 by yaokq 
	int				nAvailNum = 0;            //��Ч�����˻���Ŀ
	char			OtherInq[999*36];		  //�˻�����
	memset(OtherInq,0,sizeof(OtherInq));
	int				nOtherCurCnt = 0;			//һ������õ����˻���Ŀ
	CString			StrSaveKindPara("");		//�˻����Ͳ���

	StrSaveKindPara = GetSprintf("%s",IniGetStr(_TIMEDEFINE_INI,_INIKEY_TIMEDEFINE,_INIKEY_TIMEDEFINE_SAVEKINDPARA,"00000000"));
	
	ZzhTotalNum = 0;				//�ܵ����˻���Ŀ
	ZzhCurNum = 0;					//�Ѿ�ȡ�õ����˻���Ŀ

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
/////////////////////////////////////////////////////////////////////////////

	ReTryCnt = 3;
	while (ReTryCnt)
	{
		TranCode = TC_OTHER_INQ;
		TranCode2 = 0;

		if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
		{
			m_pDevCmn->TranResult = TRUE;
		}
		else
		{
			ZzhCurNum = 0;
			fnAPP_SendHost();
			fnAPP_RecvHost();
		}
// 20080908 by yaokq ���������˻���Ϣȡ����

		if (m_pDevCmn->TranResult)
		{
			ZzhTotalNum = Asc2Int(&HMERecv.Data[123][3],3);
			nOtherCurCnt = Asc2Int(&HMERecv.Data[123][6],2);

			if (ZzhTotalNum == 0	&&
				nOtherCurCnt == 0)						//liyi add �̿�ͨ ���������ʻ� 2008.09.19
			{
				ScrNum = 54;											
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_CONTINUE)
				{
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
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
					fnAPP_CancelProc(T_PROGRAM);			
			}		

			ZzhCurNum += nOtherCurCnt;
		
			for(int i = 0;i<nOtherCurCnt;i++)
			{
				if(((HMERecv.Data[123][8+4+i*27] == '1')&&(StrSaveKindPara.GetAt(4) != '0'))	||
					((HMERecv.Data[123][8+4+i*27] == '3')&&(StrSaveKindPara.GetAt(3) != '0'))	||
					((HMERecv.Data[123][8+4+i*27] == '2')&&(StrSaveKindPara.GetAt(5) != '0')))
					{
						memcpy(&OtherInq[nAvailNum*27],&(HMERecv.Data[123][8+i*27]),27);
						nAvailNum++;
					}
			}				
		
//			while(ZzhCurNum < ZzhTotalNum)
			while (ZzhTotalNum)
			{
				fnAPP_SendHost();
				fnAPP_RecvHost();
				
				if (m_pDevCmn->TranResult)
				{
					ZzhTotalNum = Asc2Int(&HMERecv.Data[123][3],3);
					nOtherCurCnt = Asc2Int(&HMERecv.Data[123][6],2);
					
					ZzhCurNum += nOtherCurCnt;

					for(int i = 0;i<nOtherCurCnt;i++)
					{
						if(((HMERecv.Data[123][8+4+i*27] == '1')&&(StrSaveKindPara.GetAt(4) != '0'))	||
							((HMERecv.Data[123][8+4+i*27] == '3')&&(StrSaveKindPara.GetAt(3) != '0'))	||
 							((HMERecv.Data[123][8+4+i*27] == '2')&&(StrSaveKindPara.GetAt(5) != '0')))
						{
							memcpy(&OtherInq[nAvailNum*27],&(HMERecv.Data[123][8+i*27]),27);
							nAvailNum++;
						}
					}
				}
				else
				{
					break;
				}
			}

			if (nAvailNum == 0)						//liyi add �̿�ͨ ����������ת�ʴ��ֵ����ʻ� 2008.09.19
			{
				ScrNum = 55;											
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				if (strReturn == S_CONTINUE)
				{
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
					fnAPP_CancelProc(T_TIMEOVER);
				else
				if (strReturn == S_INPUTOVER)
					fnAPP_CancelProc(T_INPUTOVER);
				else
					fnAPP_CancelProc(T_PROGRAM);			
			}
////////////////////////////////////////////////////////////////////////////////////////////
			TranCode = TC_TZH;
			ScrNum = 49;
			if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
			{
				while(TRUE)
				{
					m_pDevCmn->fnSCR_DisplayString(1, "00001");
					m_pDevCmn->fnSCR_DisplayString(2, "����");
					m_pDevCmn->fnSCR_DisplayString(3, "1000.00");
					m_pDevCmn->fnSCR_DisplayString(4, "1��");
					m_pDevCmn->fnSCR_DisplayString(5, "20070101");
					m_pDevCmn->fnSCR_DisplayString(6, "00002");
					m_pDevCmn->fnSCR_DisplayString(7, "����");
					m_pDevCmn->fnSCR_DisplayString(8, "2000.00");
					m_pDevCmn->fnSCR_DisplayString(9, "2��");
					m_pDevCmn->fnSCR_DisplayString(10, "20080101");
		
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																
				
					strReturn = m_pDevCmn->fstrSCR_GetKeyString();				
					if (strReturn == S_EXIT)
						fnAPP_CancelProc(T_EXIT);
					else if (strReturn == S_CANCEL)
						fnAPP_CancelProc(T_CANCEL);
					else if (strReturn == S_TIMEOVER)
						fnAPP_CancelProc(T_TIMEOVER);
					else if (strReturn == S_INPUTOVER)
						fnAPP_CancelProc(T_INPUTOVER);
					else if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;											//�����أ����������׻���  by zjd
					}
					else if(strReturn == S_Previous)
						continue;
					else if(strReturn == S_NEXT)
						continue;
					else if(strReturn == S_ENTER)
					{
						break;
				
					}
				}
			}
			else
			{
				int j = 0;
				int nCurNum = nAvailNum;
				while(TRUE)
				{
					for(int i = 0; i<__min(5,nCurNum);i++)
					{
						m_pDevCmn->fnSCR_DisplayString(i*5 + 1,&OtherInq[(i+j) * 27],4);

						if(OtherInq[(i+j)*27 + 4] == '1')
							m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"����");
						else if(OtherInq[(i+j)*27 + 4] == '3')
							m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"����");
						else if(OtherInq[(i+j)*27 + 4] == '2')
							m_pDevCmn->fnSCR_DisplayString(i*5 +2 ,"֪ͨ");
						else
							m_pDevCmn->fnSCR_DisplayString(i*5 + 2,&OtherInq[(i+j) * 27 + 4],1);

						m_pDevCmn->fnSCR_DisplayString(i*5 + 3,GetSprintf("%d.%.2d",Asc2Int(&OtherInq[(i+j) * 27 + 5],12)/100,Asc2Int(&OtherInq[(i+j) * 27 + 5],12)%100));
						
						if(memcmp(&OtherInq[(i+j) * 27 + 17],"03",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"������");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"06",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"������");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"12",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"һ��");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"24",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"����");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"36",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"����");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"60",2) == 0)				
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"����");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"01",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"һ��");
						else if(memcmp(&OtherInq[(i+j) * 27 + 17],"07",2) == 0)
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"����");
						else
							m_pDevCmn->fnSCR_DisplayString(i*5 + 4,"");
						
						m_pDevCmn->fnSCR_DisplayString(i*5 + 5,&OtherInq[(i+j) * 27 + 19],8);
					}
					j += 5;
					nCurNum -= 5;

					// added by liuxl 20110926
					if (j <= 5)
						m_pDevCmn->fnSCR_DisplayImage(5, FALSE);
					if (nCurNum <= 0)
						m_pDevCmn->fnSCR_DisplayImage(6, FALSE);
			
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;											//�����أ����������׻���
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
					else
					{
						for (int k=0;k<=nAvailNum;k++)
						{
							if (memcmp(strReturn.GetBuffer(0),&OtherInq[k*27],4) == 0)
							{
								DOUBLE Money = Asc2Float(&OtherInq[k*27+5], 12)/100;
								switch (OtherInq[k*27 + 4])
								{
									case '1':
										if (Money < Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMINNUM,"50")) ||
											Money > Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMAXNUM,"50000")))
											iChoiceNo = -2;
										break;
									case '3':
										if (Money < Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMINNUM,"50")) ||
											Money > Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMAXNUM,"50000")))
											iChoiceNo = -2;
										break;
									case '2':
										if (Money < Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMINNUM,"50000")) ||
											Money > Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMAXNUM,"100000")))
											iChoiceNo = -2;
										break;
									default:
										break;
								}

								if (iChoiceNo == -2)
									break;

								m_pTranCmn->strTFRSubNo=strReturn;
								memcpy(Accept.Money, &OtherInq[k*27+5], 12);

								iChoiceNo = k;
								break;
							}
						}
						if (iChoiceNo == -1)
						{
							ScrNum = 52;											//�޴����ʺ�   by liyi
							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
							strReturn = m_pDevCmn->fstrSCR_GetKeyString();
							if (strReturn == S_CONTINUE)
							{
// 								MainTranReturnFlag = TRUE;
// 								return T_OK;
								j = 0;
								nCurNum = nAvailNum;
								ScrNum = 49;
								continue;
							}
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
						}
						else if (iChoiceNo == -2)
						{
							ScrNum = 62;											//���ʺŽ���   by liuxl
							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
							strReturn = m_pDevCmn->fstrSCR_GetKeyString();
							if (strReturn == S_CONTINUE)
							{
								j = 0;
								nCurNum = nAvailNum;
								ScrNum = 49;
								iChoiceNo = -1;
								continue;
							}
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
						}
					}
					break;
				}
				break;
			}
			if (!--ReTryCnt)
			{
				fnAPP_CancelProc(T_INPUTOVER);
			}
		}
		else 
		if (memcmp(HostResp.RejectCode, "26", 2) == 0)		// ��ʾ��������������ҳ��  by zjd
		{
			if (iNGCnt <3)
			{
				iNGCnt ++;
				fnAPP_PMEAcceptPassWord();						// Accept PassWord
				continue;
			}
			else
				fnAPP_CancelProc(T_CANCEL);
		}
		else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
				fnAPP_CancelProc(T_CANCEL);
			}
			else
			if (HostResp.CardEjectFlag)								// Card Eject Flag
			{
				fnAPP_PMDProc();									// Message Display
				fnAPD_CheckDeviceAction(DEV_MCU);
				fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
				fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
				fnAPP_CancelProc(T_CANCEL);
			}	
			else
			{
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();							// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																// Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
				{
					fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
					MainTranReturnFlag = TRUE;
					return T_OK;								//�����أ����������׻���  by zjd
				}
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	//���Կͻ�����Ľ��
	ScrNum = 50;
	if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
	{
		m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s","0001"));
		m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s","1000.00"));
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s","����"));
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%s","һ��"));

	}
	else
	{
		CString cstrTemp;
		char strTemp[16];
		memset(strTemp,0,sizeof(strTemp));

		memcpy(strTemp,&OtherInq[iChoiceNo*27],4);
		m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s",strTemp));
		
		memset(strTemp,0,sizeof(strTemp));
		memcpy(strTemp,&OtherInq[iChoiceNo*27+4],1);
		m_pTranCmn->strTFRSubType=GetSprintf("%s",strTemp);
		if (memcmp(strTemp, "1", 1) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "3", 1) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "2", 1) == 0)
		{
			cstrTemp = "֪ͨ";
 		}
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s",cstrTemp));

		memset(strTemp,0,sizeof(strTemp));
		memcpy(strTemp,&OtherInq[iChoiceNo*27+5],12);
		m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f",Asc2Float(strTemp)/100));

		memset(strTemp,0,sizeof(strTemp));
		memcpy(strTemp,&OtherInq[iChoiceNo*27+17],2);
		m_pTranCmn->strTFRInData=GetSprintf("%s",strTemp);
		if (memcmp(strTemp, "03", 2) == 0)
		{
			cstrTemp = "������";
		}
		else if (memcmp(strTemp, "06", 2) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "12", 2) == 0)
		{
			cstrTemp = "һ��";
		}
		else if (memcmp(strTemp, "24", 2) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "36", 2) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "60", 2) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "01", 2) == 0)
		{
			cstrTemp = "һ��";
		}
		else if (memcmp(strTemp, "07", 2) == 0)
		{
			cstrTemp = "����";
		}
		else if (memcmp(strTemp, "00", 2) == 0)
		{
			cstrTemp = "";
		}
		m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%s",cstrTemp));
	}

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

	strReturn1 = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn1 == S_EXIT)
		fnAPP_CancelProc(T_EXIT);
	else 
	if (strReturn1 == S_CANCEL)
		fnAPP_CancelProc(T_CANCEL);
	else
	if (strReturn1 == S_TIMEOVER)
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn1 == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	else
	if (strReturn1 == S_RETURN)
	{
		MainTranReturnFlag = TRUE;
		return T_OK;				//�����أ����������׻���  by zjd
	}
	return T_OK;
}

// Accept Transfer
int CTranCmn::fnAPP_AcceptTrans()								//liyi add �̿�ͨת��			
{
	CString strReturn;
	int ScrNum;
	int iNGCnt = 0;

	if (TranCode == TC_TRANSFER)								// Accept Transfer
	{
		
		ScrNum = 514;											//ת�˷�թƭ��ʾ
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;										//�����أ����������׻���
		}
		else
		if (strReturn == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);


		if (LocalCardFlag  == 5)
		{
			while (TRUE)
			{														//ѡ��ת������
				m_pDevCmn->fnSCR_DisplayScreen(41, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;									//�����أ����������׻���  by zjd
				}
				else
				if (strReturn == "1")								//�������ʻ�ת��
				{
					break;
				}
				else
				if (strReturn == "2")								//�̿�ͨ����ת��
				{
					while (TRUE)
					{												//ѡ����ת������
						m_pDevCmn->fnSCR_DisplayScreen(42, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
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
						if (strReturn == S_RETURN)
						{
							MainTranReturnFlag = TRUE;
							return T_OK;							//�����أ����������׻���  by zjd
						}
						else
						if (strReturn == "1")						//����ת��������
						{
							fnAPP_Trans_LiveToOther();
							//MainTranReturnFlag = TRUE;

							if (HZTFlag == S_EXIT)					// added by yusy 20110526
								fnAPP_CancelProc(T_EXIT);
							else 
							if (HZTFlag == S_CANCEL)
								fnAPP_CancelProc(T_CANCEL);
							else
							if (HZTFlag == S_TIMEOVER)
								fnAPP_CancelProc(T_TIMEOVER);
							else
							if (HZTFlag == S_INPUTOVER)
								fnAPP_CancelProc(T_INPUTOVER);

							return T_OK;
						}
						else
						if (strReturn == "2")						//��������ת����
						{
							fnAPP_Trans_OtherToLive();
							//MainTranReturnFlag = TRUE;
							return T_OK;								
						}
					}
				}
			}
		}
	}
	else
	if (TranCode == TC_REMIT)																	// Accept REMIT
	{
		iNGCnt = 0;
		while (TRUE)
		{
			TranCode = TC_INQUIRY;
			TranCode2 = 0;

			fnAPP_SendHost();
			fnAPP_RecvHost();
			
			TranCode = TC_REMIT;
			if (m_pDevCmn->TranResult)															//ѡ��������
			{
				if ((!m_pDevCmn->fnAPL_GetAvailDevice(DEV_SPR))		||							
					(m_pDevCmn->fnAPL_GetDownErrorDevice(DEV_SPR))	||
					(m_pDevCmn->SlipHeadStatus != HEADER_NORMAL)	||
					(m_pDevCmn->SlipStatus == SLIP_EMPTY_PAPER))
				{
					ScrNum = 19;																// �������ť
				}
				else
				{
					ScrNum = 14;
				}
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	// Confirm Input Account Screen
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
				if (strReturn == S_RETURN)  //�����أ����������׻���  
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else
                    RemitType = strReturn;
				break;
			}
			else 
			if (memcmp(HostResp.RejectCode, "26", 2) == 0)			// ��ʾ��������������ҳ��  by zjd
			{
				if (iNGCnt <3)
				{
					iNGCnt ++;
					fnAPP_PMEAcceptPassWord();						// Accept PassWord
					continue;
				}
				else
					fnAPP_CancelProc(T_CANCEL);
			}
			else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (HostResp.CardEjectFlag)								// Card Eject Flag
				{
					fnAPP_PMDProc();									// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																		// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}	
				else
				{
					if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
						(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					{
						fnAPP_PMDProc();							// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																	// Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					{
						fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
						MainTranReturnFlag = TRUE;
						return T_OK;								//�����أ����������׻���  by zjd
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
			}
		}
	}
	else
	if (TranCode == TC_PAY)										// Accept Pay
	{
		iNGCnt = 0;
		while (TRUE)
		{
			TranCode = TC_INQUIRY;
			TranCode2 = 0;
			
			fnAPP_SendHost();
			fnAPP_RecvHost();
			
			TranCode = TC_PAY;
			if (m_pDevCmn->TranResult)                                  //ѡ��ɷ�����
			{
				if (iPayMenuNumCZ == 0)						// added by liuxl 20111021
					PayType = S_PAY1;
				else
				if (iPayMenuNumJF == 0)
					PayType = S_PAY2;
				else
				{
					m_pDevCmn->fnSCR_DisplayScreen(13, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	// Confirm Input Account Screen
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
					if (strReturn == S_RETURN)  //�����أ����������׻���  
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}
					else
					    PayType = strReturn;
				}
				break;
			}
			else 
			if (memcmp(HostResp.RejectCode, "26", 2) == 0)		// ��ʾ��������������ҳ��  by zjd
			{
				if (iNGCnt <3)
				{
					iNGCnt ++;
					fnAPP_PMEAcceptPassWord();						// Accept PassWord
					continue;
				}
				else
					fnAPP_CancelProc(T_CANCEL);
			}
			else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (HostResp.CardEjectFlag)								// Card Eject Flag
				{
					fnAPP_PMDProc();									// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																		// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}	
				else
				{
					if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
						(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					{
						fnAPP_PMDProc();							// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																	// Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					{
						fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
						MainTranReturnFlag = TRUE;
						return T_OK;								//�����أ����������׻���  by zjd
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
			}
		}
	}
	else
	if (TranCode == TC_ECHANNEL)									// E-CHANNEL
	{
		int			ScrNum = 0;
		int			displayNum = 0;
//		int			i, j, k;
		int			i, j;
		CString		strReturn("");
		CString		strTemp("");
		
		TranCode = TC_ECHANNEL_INQ;
		TranCode2 = 0;
		memset(ECHANNELInq_POST, 0, sizeof(ECHANNELInq_POST));

		Accept.InAccountNumSize = strlen(CardAccountNo);
		memcpy(Accept.InAccountNum, CardAccountNo, Accept.InAccountNumSize);
		
		fnAPP_SendHost();
		fnAPP_RecvHost();
		
		if (m_pDevCmn->TranResult)
		{			
			ScrNum = 500;
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%-28.28s", Accept.InAccountNum));
			m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%-40.40s", &HMERecv.Data[61][16]));
			memcpy(Accept.RecvData61, HMERecv.Data[61], Asc2Int(HMERecv.Data[61], 2) + 2);
			m_pDevCmn->fnSCR_DisplayString(3, "��ǩԼ�ͻ�");
			strTemp.Format("%-20.20s", &HMERecv.Data[63][4]);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			m_pDevCmn->fnSCR_DisplayString(5, strTemp);
			memcpy(Accept.RecvData63, HMERecv.Data[63], Asc2Int(HMERecv.Data[63], 2) + 2);
			strTemp.Format("%-11.11s", &HMERecv.Data[123][3]);
			strTemp.TrimLeft();
			strTemp.TrimRight();
			m_pDevCmn->fnSCR_DisplayString(6, strTemp);
			memcpy(Accept.CellPhoneNo, &HMERecv.Data[123][3], 11);
			if (memcmp(&HMERecv.Data[63][2], "01", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "���֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "02", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "���ڲ�");
			else
			if (memcmp(&HMERecv.Data[63][2], "03", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "����֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "04", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "����֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "05", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "�۰ľ��������ڵ�ͨ��֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "06", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "̨�����֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "07", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "��������");
			else
			if (memcmp(&HMERecv.Data[63][2], "08", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "�������");
			else
			if (memcmp(&HMERecv.Data[63][2], "09", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "��ʱ���֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "10", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "����ʿ��֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "11", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "������ְ�ɲ�֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "12", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "�侯ʿ��֤");
			else
			if (memcmp(&HMERecv.Data[63][2], "13", 2) == 0)
				m_pDevCmn->fnSCR_DisplayString(4, "����");
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
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_CANCEL;											
			}

			ScrNum = 200;
			memset(ECHANNELSEL, 0, sizeof(ECHANNELSEL));
			CStringArray strEchannelItemsArray;
			CStringArray strEchannelInqArray;
			for (i = 0; i < itemof(ECHANNELContent_POST); i++)
			{
				if (memcmp(ECHANNELContent_POST[i], "AAAAAA", 6) == 0)
					break;
				SplitString(CString(ECHANNELContent_POST[i]), ",", strEchannelItemsArray);
				
				for (j = 0; j < itemof(ECHANNELInq_POST); j++)
				{
					if (memcmp(ECHANNELContent_POST[j], "AAAAAA", 6) == 0)
						break;
					SplitString(CString(ECHANNELInq_POST[j]), ",", strEchannelInqArray);
					
					if (Asc2Int(strEchannelItemsArray[0], 3) == Asc2Int(strEchannelInqArray[0], 3)	&&
						Asc2Int(strEchannelItemsArray[4], 1))
					{
						displayNum++;
						strTemp = strEchannelItemsArray[1];
						strTemp.TrimLeft();
						strTemp.TrimRight();
						m_pDevCmn->fnSCR_DisplayString(displayNum * 2 - 1, strTemp);
						if (Asc2Int(strEchannelInqArray[1], 1) == 1 || Asc2Int(strEchannelInqArray[1], 1) == 2)
							m_pDevCmn->fnSCR_DisplayString(displayNum * 2, "1");
						else
							m_pDevCmn->fnSCR_DisplayString(displayNum * 2, "0");

						ECHANNELSEL[i] = 1;

						break;
					}
				}

				if (displayNum == 4)
					break;
			}
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
			CStringArray strTempArray;
			SplitString(strReturn, ":", strTempArray);
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
			if ((strTempArray.GetSize() >= 5) &&					// 2005.05.26
				(strTempArray[0] == "ECHANNELSEL"))
			{
				TransPassWordFlag = 0;
				BOOL ProtReadOnce = FALSE;
				j = 1;
				for (i = 0; i < itemof(ECHANNELContent_POST); i++)
				{
					if (memcmp(ECHANNELContent_POST[i], "AAAAAA", 6) == 0)
						break;

					if (!ECHANNELSEL[i])
						continue;

					SplitString(CString(ECHANNELContent_POST[i]), ",", strEchannelItemsArray);

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strTempArray[%d] = %d", j, Asc2Int(strTempArray[j]));

					if (Asc2Int(strTempArray[j]))
					{
						if (Asc2Int(strEchannelItemsArray[5], 1))
							TransPassWordFlag = 1;

						if (ProtReadOnce)
						{
							j++;
							continue;
						}

// 						CString strTempRow;
// 						int m = 0;
// 						int rowPage = 8;
// //						int	nCurNum = IniGetInt(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dRowNUM", i + 1), 0);
// 						int	nCurNum = IniGetInt(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, "ECP01RowNUM", 0);
// 						ScrNum = 450;
// 						
// 						while (TRUE)
// 						{
// 							for(k = 0; k<__min(rowPage, nCurNum); k++)
// 							{
// //								strTempRow = IniGetStr(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP%02dRow%04d", i + 1, k + m + 1), "");
// 								strTempRow = IniGetStr(_ECHANNELPROTOCOL_INI, _INIKEY_TRANSACTION_ECHANNELPROT, GetSprintf("ECP01Row%04d", k + m + 1), "");
// 								m_pDevCmn->fnSCR_DisplayString(k + 1, strTempRow);
// 							}
// 							m += rowPage;
// 							nCurNum -= rowPage;
// 							
// 							if (m <= rowPage)
// 								m_pDevCmn->fnSCR_DisplayImage(5, FALSE);
// 							if (nCurNum <= 0)
// 								m_pDevCmn->fnSCR_DisplayImage(6, FALSE);
// 							
// //							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
// 							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, 60, PIN_MENU_MODE);
// 							strReturn = m_pDevCmn->fstrSCR_GetKeyString();
// 							if(strReturn == S_EXIT)
// 								fnAPP_CancelProc(T_EXIT);
// 							else if(strReturn == S_CONTINUE)
// 								break;
// 							else if(strReturn == S_CANCEL)
// 								fnAPP_CancelProc(T_CANCEL);
// 							else if(strReturn == S_TIMEOVER)
// 								fnAPP_CancelProc(T_TIMEOVER);
// 							else if (strReturn == S_INPUTOVER)
// 								fnAPP_CancelProc(T_INPUTOVER);
// 							else if(strReturn == S_Previous)
// 							{
// 								if(m == rowPage)
// 								{
// 									m -= rowPage;
// 									nCurNum += rowPage;
// 									continue;
// 								}
// 								else
// 								{
// 									m -= rowPage * 2;
// 									nCurNum += rowPage * 2;
// 									continue;
// 								}
// 							}
// 							else if(strReturn == S_NEXT)
// 							{
// 								if(nCurNum <= 0)
// 								{
// 									m -= rowPage;
// 									nCurNum += rowPage;
// 									continue;
// 								}
// 								else
// 									continue;
// 							}
// 							else
// 								fnAPP_CancelProc(T_PROGRAM);
// 						}

						ScrNum = 24;
						int EPNum = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ECHANNELPSETNUM, 25);
						if (EPNum)
						{
							m_pDevCmn->fnSCR_DisplayNumeric(1, EPNum);
							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, 60, PIN_MENU_MODE);

							strReturn = m_pDevCmn->fstrSCR_GetKeyString();
							if(strReturn == S_EXIT)
								fnAPP_CancelProc(T_EXIT);
							else if(strReturn == S_CONTINUE);
							else if(strReturn == S_CANCEL)
								fnAPP_CancelProc(T_CANCEL);
							else if(strReturn == S_TIMEOVER)
								fnAPP_CancelProc(T_TIMEOVER);
							else if (strReturn == S_INPUTOVER)
								fnAPP_CancelProc(T_INPUTOVER);
							else
								fnAPP_CancelProc(T_PROGRAM);
						}

						ScrNum = 451;
						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
						strReturn = m_pDevCmn->fstrSCR_GetKeyString();
						if(strReturn == S_EXIT)
							fnAPP_CancelProc(T_EXIT);
						else if(strReturn == S_CONTINUE)
							ProtReadOnce = TRUE;
						else if(strReturn == S_CANCEL)
							fnAPP_CancelProc(T_CANCEL);
						else if(strReturn == S_TIMEOVER)
							fnAPP_CancelProc(T_TIMEOVER);
						else if (strReturn == S_INPUTOVER)
							fnAPP_CancelProc(T_INPUTOVER);
					}
					else
					{
						ECHANNELSEL[i] = 0;
					}

					j++;
				}

				for (i = 0; i < sizeof(ECHANNELSEL); i++)
				{
					if (ECHANNELSEL[i])
						break;
					else
					if (i == sizeof(ECHANNELSEL) - 1)
						fnAPP_CancelProc(T_EXIT);
				}
			}
/////////////////////////////////////////////////////////////////////////////
			memset(Accept.LogInPassWord, 'F', sizeof(Accept.LogInPassWord));	// New PassWord
/////////////////////////////////////////////////////////////////////////////
			
			int RetryCnt = 3;
			int iReloadTimes = 0;
			CString	strFirstPin("");
			CString	strSecondPin("");
			
			while (RetryCnt)
			{
				ScrNum = 114;										// Input First New PassWord Screen
				iReloadTimes = 0;
				while(TRUE)
				{
					//EPP ADD BY HYL 20070902
					m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
					   m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				//�����أ����������׻���  by zjd
					}
					else
					{
						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
						{
						   strFirstPin = m_pDevCmn->fstrPIN_GetPinKeyData();     // Save PassWord First 
						   //�޸�FLASH��λ������ʱ������ȷ��,����������������
						   if  (strFirstPin.GetLength() == 0)
							   fnAPP_CancelProc(T_ERROR);
						}
						else
						   strFirstPin = strReturn;								// Save PassWord First 
					}
					break;
				}

				
				ScrNum = 115; // Input Second New PassWord Screen
				RetryCnt = 0;
				iReloadTimes = 0;
				while(TRUE)
				{
					 //EPP ADD BY HYL 20070902
					m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				//�����أ����������׻���  by zjd
					}
					else
					{   
						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
						{
						   strSecondPin = m_pDevCmn->fstrPIN_GetPinKeyData();   // Save PassWord Second
						   if  (strSecondPin.GetLength() == 0)					// liyi chang 2008.10.05
							   fnAPP_CancelProc(T_ERROR);
						}
						else
						   strSecondPin = strReturn;							// Save PassWord Second
					}
					break;
				}
				

				if (strFirstPin == strSecondPin)							// Check PassWord 
				{
					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
					{ 
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData3[%s]", strFirstPin);
						if (strFirstPin.GetLength() >= sizeof(Accept.LogInPassWord))
							memcpy(Accept.LogInPassWord, strFirstPin.GetBuffer(0), sizeof(Accept.LogInPassWord));
						else
							memcpy(Accept.LogInPassWord, strFirstPin.GetBuffer(0), strFirstPin.GetLength());
					}
					else
					if (strFirstPin.GetLength() == 6)
					{
						memcpy(Accept.LogInPassWord, "06", 2);
						memcpy(Accept.LogInPassWord + 2, strFirstPin.GetBuffer(0), 6);
					}
					else
						fnAPP_CancelProc(T_PROGRAM);
					break;
				}

				if (!--RetryCnt)
				{
					fnAPP_CancelProc(T_INPUTOVER);
				}

				ScrNum = 113;											// Confirm New PassWord Screen
				iReloadTimes = 0;
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
			}

			if (TransPassWordFlag)
			{
				memset(Accept.TransPassWord, 'F', sizeof(Accept.TransPassWord));	// New PassWord				
				RetryCnt = 3;
				while (RetryCnt)
				{
					ScrNum = 116;										// Input First New PassWord Screen
					iReloadTimes = 0;
					while(TRUE)
					{
						//EPP ADD BY HYL 20070902
						m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);
						
						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
						if (strReturn == S_RETURN)
						{
							MainTranReturnFlag = TRUE;
							return T_OK;				//�����أ����������׻���  by zjd
						}
						else
						{
							if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
							{
								strFirstPin = m_pDevCmn->fstrPIN_GetPinKeyData();     // Save PassWord First 
								//�޸�FLASH��λ������ʱ������ȷ��,����������������
								if  (strFirstPin.GetLength() == 0)
									fnAPP_CancelProc(T_ERROR);
							}
							else
								strFirstPin = strReturn;								// Save PassWord First 
						}
						break;
					}
				
					ScrNum = 117; // Input Second New PassWord Screen
					RetryCnt = 0;
					iReloadTimes = 0;
					while(TRUE)
					{
						 //EPP ADD BY HYL 20070902
						m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
						if (strReturn == S_RETURN)
						{
							MainTranReturnFlag = TRUE;
							return T_OK;				//�����أ����������׻���  by zjd
						}
						else
						{   
							if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
							{
							   strSecondPin = m_pDevCmn->fstrPIN_GetPinKeyData();   // Save PassWord Second
							   if  (strSecondPin.GetLength() == 0)					// liyi chang 2008.10.05
								   fnAPP_CancelProc(T_ERROR);
							}
							else
							   strSecondPin = strReturn;							// Save PassWord Second
						}
						break;
					}
				
					if (strFirstPin == strSecondPin)							// Check PassWord 
					{
						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
						{ 
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData3[%s]", strFirstPin);
							if (strFirstPin.GetLength() >= sizeof(Accept.TransPassWord))
								memcpy(Accept.TransPassWord, strFirstPin.GetBuffer(0), sizeof(Accept.TransPassWord));
							else
								memcpy(Accept.TransPassWord, strFirstPin.GetBuffer(0), strFirstPin.GetLength());
						}
						else
						if (strFirstPin.GetLength() == 6)
						{
							memcpy(Accept.TransPassWord, "06", 2);
							memcpy(Accept.TransPassWord + 2, strFirstPin.GetBuffer(0), 6);
						}
						else
							fnAPP_CancelProc(T_PROGRAM);
						break;
					}

					if (!--RetryCnt)
					{
						fnAPP_CancelProc(T_INPUTOVER);
					}

					ScrNum = 113;											// Confirm New PassWord Screen
					iReloadTimes = 0;
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
				}
			}

			TranCode = TC_SMSPARITY;
			TranCode2 = 0;
			
			fnAPP_SendHost();
			fnAPP_RecvHost();
			
			if (m_pDevCmn->TranResult)
			{
				TranCode = TC_ECHANNEL;
				TranCode2 = 0;
				TranSend = 0;
				
				ScrNum = 320;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, 90, PIN_INPUT_MODE);																
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				else
				if (strReturn.GetLength() > sizeof(Accept.SMSParityCode))
						fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					memset(Accept.SMSParityCode, ' ', sizeof(Accept.SMSParityCode));
					memcpy(Accept.SMSParityCode, strReturn.GetBuffer(0), strReturn.GetLength());
				}
			}
			else														// ��ʾ������Ϣ��������Ӧ����  by zjd
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
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (HostResp.CardEjectFlag)								// Card Eject Flag
				{
					fnAPP_PMDProc();									// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
					// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}	
				else
				{
					if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
						(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					{
						fnAPP_PMDProc();								// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
						// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);	
						// Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (fnAPP_MainMenuContinueCheck() == T_OK)			// 2004.08.23
					{
						fnAPP_PMEAcceptNgGuideConfirm();				// Accept Ng Guide Confirm
						MainTranReturnFlag = TRUE;
						return T_CANCEL;								// �����أ����������׻���  by zjd
					}
					else
					fnAPP_CancelProc(T_CANCEL);
				}
			}
		}
		else														// ��ʾ������Ϣ��������Ӧ����  by zjd
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
				fnAPP_CancelProc(T_CANCEL);
			}
			else
			if (HostResp.CardEjectFlag)								// Card Eject Flag
			{
				fnAPP_PMDProc();									// Message Display
				fnAPD_CheckDeviceAction(DEV_MCU);
				fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
				// Card Out
				fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
				fnAPP_CancelProc(T_CANCEL);
			}	
			else
			{
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();								// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
					// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);	
					// Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)			// 2004.08.23
				{
					fnAPP_PMEAcceptNgGuideConfirm();				// Accept Ng Guide Confirm
					MainTranReturnFlag = TRUE;
					return T_CANCEL;								// �����أ����������׻���  by zjd
				}
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
		return T_OK;
	}
	else
    if (TranCode == TC_ACMANAGER)								// Accept A/C MANAGER
	{ 
	    memset(&ACHostResp,0,sizeof(ACHostResp));
		memset(&ACHostSend,0,sizeof(ACHostSend));
		memset(ACHostRespACManager,0, sizeof(ACHostRespACManager));
		memset(ACHostRespRemitAssu,0, sizeof(ACHostRespRemitAssu));
		memset(ACHostRespPayUnAssu,0, sizeof(ACHostRespPayUnAssu));
		memset(ACHostRespPayAssu,0, sizeof(ACHostRespPayAssu));

		if ( ACInqFlag == 0 )
		{
			TranCode = TC_INQUIRY;              //�ȷ�һ����ѯ���׽���������֤
			fnAPP_SendHost();
			fnAPP_RecvHost();
		}
		else
		{
			m_pDevCmn->TranResult = TRUE;
		}

		if ( m_pDevCmn->TranResult )
		{
			ACInqFlag = 1;
//			while (TRUE)
// 			{
			m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
            Delay_Msg(1000);
			ACTranCode = TC_AC_CUSMESS_INQ;          //�û��Ƿ񿪰����˻��ܼ�ҵ��   �������Ƿ�Ϊ0   
			fnAPP_ACSendHost();
			fnAPP_ACRecvHost();
			if( ACHostResp.ReturnCode !=0)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;		
			}
            

			ACTranCode = TC_AC_CUSASSU_DETAIL;
			fnAPP_ACSendHost();
			fnAPP_ACRecvHost();
			if( ACHostResp.ReturnCode !=0)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;		
			}

			ACTranCode = TC_AC_TRAN_INQ;
			fnAPP_ACSendHost();
			fnAPP_ACRecvHost();
			if( ACHostResp.ReturnCode !=0)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;		
			}

			ScrNum = 32;
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	// Confirm Input Account Screen
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
			if (strReturn == S_RETURN)  //�����أ����������׻���  
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
		   else
		   {
				if( strReturn == S_ACREMIT)
				{
					TranCode = TC_AC_REMIT;
				}
				else 
				if(strReturn == S_ACPAY)
					TranCode = TC_AC_PAY;	
// 				break;
		   }

// 			}
		}
		else 
		if (memcmp(HostResp.RejectCode, "26", 2) == 0)		// ��ʾ��������������ҳ��  by zjd
		{
			if (iNGCnt <3)
			{
				iNGCnt ++;
				fnAPP_PMEAcceptPassWord();						// Accept PassWord
// 				continue;
			}
			else
				fnAPP_CancelProc(T_CANCEL);
		}
		else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
				fnAPP_CancelProc(T_CANCEL);
			}
			else
			if (HostResp.CardEjectFlag)								// Card Eject Flag
			{
				fnAPP_PMDProc();									// Message Display
				fnAPD_CheckDeviceAction(DEV_MCU);
				fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
				fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
				fnAPP_CancelProc(T_CANCEL);
			}	
			else
			{
				if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
					(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
				{
					fnAPP_PMDProc();							// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																// Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
				{
					fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
					MainTranReturnFlag = TRUE;
					return T_OK;								//�����أ����������׻���  by zjd
				}
				else
					fnAPP_CancelProc(T_CANCEL);
			}
		}
	}
	else
    if (TranCode == TC_OTHER)									// Accept Other
	{ 
		// ������δ��ͨ
		ScrNum = 53;											
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_CONTINUE)
		{
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
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
			fnAPP_CancelProc(T_PROGRAM);		
	}
	fnAPP_AcceptInAccountNum();	// Accept In Account Number
	if (MainTranReturnFlag)
		return T_OK;	
	fnAPP_AcceptTMoney();										// Accept Transfer Money
	if (MainTranReturnFlag)
		return T_OK;
	return T_OK;
}

// Accept Deposit
int CTranCmn::fnAPP_AcceptDep()
{
	return T_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Accept Money
int CTranCmn::fnAPP_AcceptMoney()
{
//////////////////////////////////////////////�ͻ�ѡ��ȡ���ʻ����ͺ�����   by zjd
	
	hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 3,
		m_pDevCmn->iMonitorCS_Custom, CardAccNoForMonitor,"SELECT CWD","NO");
/////////////////////////////////////////////////////////////////////////////
	fnAPP_AcceptMoney10050();
	if (MainTranReturnFlag)
		return T_OK;
/////////////////////////////////////////////////////////////////////////////
//	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <AMOUNT INPUT : %10.10s.%2.2s>",
//											GetTime().GetBuffer(0),
//											&(GetTime().GetBuffer(0)[2]),
//											&(GetTime().GetBuffer(0)[4]),
//											Accept.Money,
//											&Accept.Money[10]));// 2005.03.22
/////////////////////////////////////////////////////////////////////////////
	return T_OK;
}

// Accept Money 100 50
int CTranCmn::fnAPP_AcceptMoney10050()
{
	int		ScrNum = 0;
	CString strReturn("");
	int		GetCase = 0;										// 2004.11.05
	CString	WithMsg("");

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
/////////////////////////////////////////////////////////////////////////////

	if (!fnAPP_CalcMoneyMaxOut())
		fnAPP_CancelProc(T_MSG, TranMsgTbl[19][m_pDevCmn->fnSCR_GetCurrentLangMode()]);

	GetCase = 0;												// Select Amount : 2004.11.05
	if (MoneyMaxOut < CSH_M10050_SELECT)						// 2005.08.22
		GetCase = 1;											// Input Amount

	switch (WithAvail)
	{
		case TRAN_WITH_100:
			WithMsg = "000100";
			break;
		case TRAN_WITH_50:
			WithMsg = "050000";
			break;
		case TRAN_WITH_BOTH:
			WithMsg = "050100";
			break;

		case TRAN_WITH_NOT:
		default: 
			fnAPP_CancelProc(T_MSG, TranMsgTbl[19][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			break;
	}

	while (GetCase <= 2)
	{
		switch (GetCase)
		{
			case 0:												// Select Amount
				ScrNum = 201;									// Select Amount Screen(100 Only)
				m_pDevCmn->fnSCR_DisplayNumeric(1, MoneyMaxOut);
				m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.0f", InitInfo.WithDayMaxAmt));
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyString();
				GetCase = 2;									// Check Amount
				if (strReturn == "100")							// 2004.11.05
					memcpy(Accept.Money, "000000010000", 12);
				else
				if (strReturn == "200")
					memcpy(Accept.Money, "000000020000", 12);
				else
				if (strReturn == "300")
					memcpy(Accept.Money, "000000030000", 12);
				else
				if (strReturn == "500")
					memcpy(Accept.Money, "000000050000", 12);
				else
				if (strReturn == "1000")
					memcpy(Accept.Money, "000000100000", 12);
				else
				if (strReturn == "2000")
					memcpy(Accept.Money, "000000200000", 12);
				else
				if (strReturn == "1500")
					memcpy(Accept.Money, "000000150000", 12);
				else
				if (strReturn == "3000")
					memcpy(Accept.Money, "000000300000", 12);
				else
				if (strReturn == S_OTHER)
					GetCase = 1;								// Input Amount
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
				if (strReturn == S_RETURN)			// liyi add �̿�ͨ
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				else
					fnAPP_CancelProc(T_PROGRAM);		
				break;

			case 1:												// Input Amount
				ScrNum = 202;									// Input Amount Screen
				m_pDevCmn->fnSCR_DisplayString(1, WithMsg);
 				m_pDevCmn->fnSCR_DisplayNumeric(2, MoneyMaxOut);
//              m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f", FLOAT(MoneyMaxOut)));
				m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.0f", InitInfo.WithDayMaxAmt));		// �������ȡ���޶�  by zjd
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
				strReturn = m_pDevCmn->fstrSCR_GetKeyNumeric(6);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				else
				if ((strReturn.GetLength() != 6)		||		// Input Data Check
					(!IsNum(strReturn.GetBuffer(0), 6))	||		
					(IsZero(strReturn.GetBuffer(0), 6))	||
					(!IsZero(&(strReturn.GetBuffer(0)[5]), 1)))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				if ((WithAvail == TRAN_WITH_100)		&&		// Input Data 100 Only
					(Asc2Int(strReturn.GetBuffer(0), 6) % CASH_CHINA_100))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				if ((WithAvail == TRAN_WITH_50)			&&		// Input Data 50 Only
					(Asc2Int(strReturn.GetBuffer(0), 6) % CASH_CHINA_50))
					fnAPP_CancelProc(T_INPUTERROR);
				else											// 2005.04.19
				if ((Asc2Int(strReturn.GetBuffer(0), 6) > InitInfo.WithMaxAmt)	||
					(Asc2Int(strReturn.GetBuffer(0), 6) < InitInfo.WithMinAmt)	||
					(Asc2Int(strReturn.GetBuffer(0), 6) > MoneyMaxOut))
				{												// Amount Over Check
					fnAPP_CancelProc(T_MSG, TranMsgTbl[20][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				}
				else
				{
					memset(Accept.Money, '0', sizeof(Accept.Money));
					memcpy(&Accept.Money[4], strReturn.GetBuffer(0), 6);
					GetCase = 2;								// Check Amount
				}
				break;

			case 2:												// Check Amount
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_AcceptMoney:"		\
													"Accept.Money[%12.12s], "	\
													"MoneyMaxOut[%d]",
													Accept.Money,
													MoneyMaxOut);
				if (!IsNum(Accept.Money, 12)	||				// Money Data Check
					IsZero(Accept.Money, 12)	||
					!IsZero(&Accept.Money[9], 3))
					fnAPP_CancelProc(T_PROGRAM);

				if ((WithAvail == TRAN_WITH_100)		&&		// Money Data 100 Only
					(Asc2Int(&Accept.Money[4], 6) % CASH_CHINA_100))
					fnAPP_CancelProc(T_PROGRAM);

				if ((WithAvail == TRAN_WITH_50)			&&		// Money Data 50 Only
					(Asc2Int(&Accept.Money[4], 6) % CASH_CHINA_50))
					fnAPP_CancelProc(T_PROGRAM);
																// 2005.04.19
				if ((Asc2Int(&Accept.Money[4], 6) > InitInfo.WithMaxAmt)	||
					(Asc2Int(&Accept.Money[4], 6) < InitInfo.WithMinAmt)	||
					(Asc2Int(&Accept.Money[4], 6) > MoneyMaxOut)			||
					(Asc2Int(&Accept.Money[4], 6) > CSH_M10050_MAXOUT))							
					fnAPP_CancelProc(T_PROGRAM);

				GetCase = 9;									// Terminate
				break;

			default: 
				break;
		}
	}

	return T_OK;
}

// Accept Transfer Money
int CTranCmn::fnAPP_AcceptTMoney()
{
	int				ScrNum = 0;
	int				ReTryCnt = 0;
//	bool			bAgain = false;
	CString			strReturn("");
	CString			strReturn1("");
	CString			strInAmount("");
	CString			strLeft("");
	CString			strRight("");
	CStringArray	strReturnArray;
	CString			strFee("");
	CString			strFirstRemitPin("");			// 2008.10.05
	CString			strSecondRemitPin("");
	int				iReloadTimes = 0;


/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
/////////////////////////////////////////////////////////////////////////////

	if (TranCode == TC_TRANSFER)								// Accept Transfer
	{
		fdAPP_CalcTMoneyMaxOut();

		ReTryCnt = 3;
		while (ReTryCnt)
		{
			ScrNum = 204;												// Input Transfer Amount Screen
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%22.22s", Accept.InAccountNum));
			m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.0f", TMoneyMaxOut));
			InitInfo.TransferMinAmt = Asc2Float(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMINAMOUNT,"50"));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "InitInfo.TransferMinAmt[%f]", InitInfo.TransferMinAmt);
			m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.0f", InitInfo.TransferMinAmt));
	//		if (bAgain == true)
	//			m_pDevCmn->fnSCR_DisplayString(8, "1");
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

			strReturn = m_pDevCmn->fstrSCR_GetKeyString();				// 2004.11.05
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
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				//�����أ����������׻���  by zjd
			}
			
			strInAmount = strReturn;

			SplitString(strReturn, ".", strReturnArray);
			if (strReturnArray.GetSize() == 1)
			{
				if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
					fnAPP_CancelProc(T_INPUTERROR);

				strLeft = ZERO10 + strReturnArray[0];
				strLeft = strLeft.Right(10);
				strRight = ZERO2;										// 2004.11.05
			}
			else
			if (strReturnArray.GetSize() == 2)
			{
				if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
					fnAPP_CancelProc(T_INPUTERROR);
				if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
					fnAPP_CancelProc(T_INPUTERROR);

				strLeft = ZERO10 + strReturnArray[0];
				strLeft = strLeft.Right(10);
				strRight = strReturnArray[1] + ZERO2;
				strRight = strRight.Left(2);
			}
			else
				fnAPP_CancelProc(T_INPUTERROR);

			if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
				(Asc2Int(strRight) <= 0))
				fnAPP_CancelProc(T_INPUTERROR);
			else														// Amount Over Check
			if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
				((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
				fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
			else
			{
				memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
				memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
				break;
			}

			if (!--ReTryCnt)
			{
				fnAPP_CancelProc(T_INPUTOVER);
			}

		}
		//���Կͻ�����Ľ��   by zjd
		// added by yusy 2010.03.22
		if(fnAPL_checkLocalCard(CString(Accept.InAccountNum)))
		{
			ScrNum = 437;
			int templen = Asc2Int(&HMERecv.Data[2], 2);
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s", &HMERecv.Data[61][16]));
			m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s", &HMERecv.Data[2][2], templen));
			m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f",Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
		}
		else
		{
			ScrNum = 408;
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%22.22s", Accept.InAccountNum));
			m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f",Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
		}
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

		strReturn1 = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn1 == S_EXIT)
			fnAPP_CancelProc(T_EXIT);
		else 
		if (strReturn1 == S_CANCEL)
			fnAPP_CancelProc(T_CANCEL);
		else
		if (strReturn1 == S_TIMEOVER)
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn1 == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
		else
		if (strReturn1 == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				//�����أ����������׻���  by zjd
		}
	}
	else
	if (TranCode == TC_REMIT)
	{
		if (RemitType == S_REMIT_ADD)
		{
			TranCode = TC_REMIT_ADD;
			ReTryCnt = 3;
			while (ReTryCnt)
			{
				ScrNum = 203;
				m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.0f", InitInfo.TranMaxRemittance));
				m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.0f", InitInfo.TranMaxRemittance));
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;											
				}
				
				strInAmount = strReturn;

				SplitString(strReturn, ".", strReturnArray);
				if (strReturnArray.GetSize() == 1)
				{
					if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);

					strLeft = ZERO10 + strReturnArray[0];
					strLeft = strLeft.Right(10);
					strRight = ZERO2;										// 2004.11.05
				}
				else
				if (strReturnArray.GetSize() == 2)
				{
					if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);
					if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);

					strLeft = ZERO10 + strReturnArray[0];
					strLeft = strLeft.Right(10);
					strRight = strReturnArray[1] + ZERO2;
					strRight = strRight.Left(2);
				}
				else
					fnAPP_CancelProc(T_INPUTERROR);

				if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
					(Asc2Int(strRight) <= 0))
					fnAPP_CancelProc(T_INPUTERROR);
				else														// Amount Over Check
				if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > InitInfo.TranMaxRemittance) ||
					((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
					fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				else
				{
					memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
					memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
					break;
				}

				if (!--ReTryCnt)
				{
					fnAPP_CancelProc(T_INPUTOVER);
				}
			}
			//���Կͻ�����Ľ��
			ScrNum = 402;
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s", m_pTranCmn->strRemitAddNo));
			m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s", m_pTranCmn->strRemitAddName));
			m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s", m_pTranCmn->strRemitAddAdd));
			m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

			strReturn1 = m_pDevCmn->fstrSCR_GetKeyString();
			if (strReturn1 == S_EXIT)
				fnAPP_CancelProc(T_EXIT);
			else 
			if (strReturn1 == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else
			if (strReturn1 == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);
			else
			if (strReturn1 == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);
			else
			if (strReturn1 == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				//�����أ����������׻���  by zjd
			}
		}
		else if (RemitType == S_REMIT_PWD)
		{
			TranCode = TC_REMIT_PWD;
			ReTryCnt = 3;
			while (ReTryCnt)
			{
				ScrNum = 203;												// Input Transfer Amount Screen
				m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.0f", InitInfo.TranMaxRemittance));
				m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.0f", InitInfo.TranMaxRemittance));
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

				strReturn = m_pDevCmn->fstrSCR_GetKeyString();				// 2004.11.05
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				
				strInAmount = strReturn;

				SplitString(strReturn, ".", strReturnArray);
				if (strReturnArray.GetSize() == 1)
				{
					if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);

					strLeft = ZERO10 + strReturnArray[0];
					strLeft = strLeft.Right(10);
					strRight = ZERO2;										// 2004.11.05
				}
				else
				if (strReturnArray.GetSize() == 2)
				{
					if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);
					if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);

					strLeft = ZERO10 + strReturnArray[0];
					strLeft = strLeft.Right(10);
					strRight = strReturnArray[1] + ZERO2;
					strRight = strRight.Left(2);
				}
				else
					fnAPP_CancelProc(T_INPUTERROR);

				if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
					(Asc2Int(strRight) <= 0))
					fnAPP_CancelProc(T_INPUTERROR);
				else														// Amount Over Check
				if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > InitInfo.TranMaxRemittance) ||
					((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
					fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				else
				{
					memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
					memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
					break;
				}

				if (!--ReTryCnt)
				{
					fnAPP_CancelProc(T_INPUTOVER);
				}
			}

			
			memset(Accept.NewPassWord, 'F', sizeof(Accept.NewPassWord));	

			ReTryCnt = 3;
			while (ReTryCnt)
			{
				ScrNum = 306;
				iReloadTimes = 0;
				while(TRUE)
				{
					m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
					   m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				//�����أ����������׻���  by zjd
					}
					else
					{
						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
						{
						   strFirstRemitPin = m_pDevCmn->fstrPIN_GetPinKeyData();     // Save PassWord First 
						   //�޸�FLASH��λ������ʱ������ȷ��,����������������
						   if  (strFirstRemitPin.GetLength() == 0)
							   fnAPP_CancelProc(T_ERROR);
						}
						else
						   strFirstRemitPin = strReturn;								// Save PassWord First 
					}
					break;
				}

				
				ScrNum = 307;															// Input Second New PassWord Screen
				ReTryCnt = 0;
				iReloadTimes = 0;
				while(TRUE)
				{
					m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
					if (strReturn == S_CLEAR)
					{
						iReloadTimes = 0;
						continue;
					}
					else
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				//�����أ����������׻���  by zjd
					}
					else
					{   
						if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
						{
						   strSecondRemitPin = m_pDevCmn->fstrPIN_GetPinKeyData();   // Save PassWord Second
						   if  (strSecondRemitPin.GetLength() == 0)
							   fnAPP_CancelProc(T_ERROR);
						}
						else
						   strSecondRemitPin = strReturn;							// Save PassWord Second
					}
					break;
				}
				

				if (strFirstRemitPin == strSecondRemitPin)							// Check PassWord 
				{
					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
					{ 
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData3[%s]", strFirstRemitPin);
						if (strFirstRemitPin.GetLength() >= sizeof(Accept.NewPassWord))
							memcpy(Accept.NewPassWord, strFirstRemitPin.GetBuffer(0), sizeof(Accept.NewPassWord));
						else
							memcpy(Accept.NewPassWord, strFirstRemitPin.GetBuffer(0), strFirstRemitPin.GetLength());
					}
					else
					if (strFirstRemitPin.GetLength() == 6)
					{
						memcpy(Accept.NewPassWord, "06", 2);
						memcpy(Accept.NewPassWord + 2, strFirstRemitPin.GetBuffer(0), 6);
					}
					else
						fnAPP_CancelProc(T_PROGRAM);
					break;
				}

				if (!--ReTryCnt)
				{
					fnAPP_CancelProc(T_INPUTOVER);
				}

				ScrNum = 308;											// Confirm New PassWord Screen
				iReloadTimes = 0;
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
			}
			//���Կͻ�����Ľ��
			ScrNum = 409;
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

			strReturn1 = m_pDevCmn->fstrSCR_GetKeyString();
			if (strReturn1 == S_EXIT)
				fnAPP_CancelProc(T_EXIT);
			else 
			if (strReturn1 == S_CANCEL)
				fnAPP_CancelProc(T_CANCEL);
			else
			if (strReturn1 == S_TIMEOVER)
				fnAPP_CancelProc(T_TIMEOVER);
			else
			if (strReturn1 == S_INPUTOVER)
				fnAPP_CancelProc(T_INPUTOVER);
			else
			if (strReturn1 == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;
			}
		}
		else if (RemitType == S_REMIT_IN)
		{
			TranCode = TC_REMIT_IN;
			ScrNum = 319;
			iReloadTimes = 0;
	
			memset(Accept.NewPassWord, 'F', sizeof(Accept.NewPassWord));	
  
			while (TRUE)
			{
				// liyi add 2008.10.17 ��Ʊ��Ϣ��ѯ���ף�������ܷ�ʽ
				char RemitPan[256];	
				char sRemitInNo[256];
				memset(RemitPan,0,sizeof(RemitPan));
				memcpy(sRemitInNo,m_pTranCmn->strRemitInNo.GetBuffer(0),m_pTranCmn->strRemitInNo.GetLength());
				int Result = FindNotNum(sRemitInNo, 20);	
				if (Result <= 12)
				{
					memset(&RemitPan[0], '0', 1);
					memcpy(&RemitPan[1], sRemitInNo, 11);
				}
				else
				if (Result <= 19)
					memcpy(&RemitPan[0], &sRemitInNo[Result - 13], 12);
				else 
					memcpy(&RemitPan[0], &sRemitInNo[7], 12);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "S_REMIT_IN:RemitInNo[%s],RemitPan[%s]",sRemitInNo,RemitPan);


				m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, RemitPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				else
				{
					if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
					{ 
						CString PinKeyData = m_pDevCmn->fstrPIN_GetPinKeyData();
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData3[%s]", PinKeyData);
						if  (PinKeyData.GetLength() == 0)
							fnAPP_CancelProc(T_ERROR);
						else if (PinKeyData.GetLength() >= sizeof(Accept.NewPassWord))
							memcpy(Accept.NewPassWord, PinKeyData.GetBuffer(0), sizeof(Accept.NewPassWord));
						else
							memcpy(Accept.NewPassWord, PinKeyData.GetBuffer(0), PinKeyData.GetLength());
					}
					else
					{
						if (strReturn.GetLength() >= 6)
						{
							memcpy(Accept.NewPassWord, "06", 2);
							memcpy(Accept.NewPassWord + 2, strReturn.GetBuffer(0), 6);
						}
						else
						{
							memcpy(Accept.NewPassWord, GetSprintf("02d",strReturn.GetLength(),2), 2);
							memcpy(Accept.NewPassWord + 2, strReturn.GetBuffer(0), strReturn.GetLength());
						}
					}
				}
				break;
			}
			//��Ʊ��Ϣ��ѯ
			TranCode = TC_REMIT_INF_INQ;
			
			fnAPP_SendHost();
			fnAPP_RecvHost();
			if(m_pDevCmn->bAgentMode == 1)						// added by yusy 2009.05.13
			{
				fnAPP_AgentIdxGetData(AGENT_REMIT_INF_INQ);
			}
			
			TranCode = TC_REMIT_IN;
			if (m_pDevCmn->TranResult)
			{
				memcpy(Accept.Money, &HMERecv.Data[123][5], 12);

				ScrNum = 418;
				m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s",m_pTranCmn->strRemitInNo));
				m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s",GetSprintf("%.2f", Asc2Float(&HMERecv.Data[123][3], 14) / 100)));
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	// Confirm Input Account Screen
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
			}
			else 
			if (memcmp(HostResp.RejectCode, "26", 2) == 0)		// ��ʾ��������������ҳ��  by zjd
			{
				fnAPP_PMEAcceptPassWord();						// Accept PassWord
			}
			else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (HostResp.CardEjectFlag)								// Card Eject Flag
				{
					fnAPP_PMDProc();									// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																		// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}	
				else
				{
					if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
						(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					{
						fnAPP_PMDProc();							// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																	// Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					{
						fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
						MainTranReturnFlag = TRUE;
						return T_OK;								//�����أ����������׻���  by zjd
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
			}
		}
	}
	else
	if (TranCode == TC_PAY)										// Accept Pay  ��Ҫ�����Ƿ������������뽻�ѽ��  by zjd
	{   
		fdAPP_CalcTMoneyMaxOut();
		if(PayType == S_PAY1 && stPayJF[PayNum].szPayContral[1]=='1' ||
		   PayType == S_PAY2 && stPayCZ[PayNum].szPayContral[1]=='1')       //�Ƿ���ҪԤ��ѯ   by hyl
		{  
			TranCode = TC_PAY_INQ;

			if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
			{
				m_pDevCmn->TranResult = TRUE;
			}
			else
			{
				fnAPP_SendHost();
				fnAPP_RecvHost();
			}

			TranCode = TC_PAY;
            if(m_pDevCmn->TranResult)
			{
				/////////////////////////
				// liyi ���������� 20110926
				strPayInqRefNo.Format("%12.12s", HostResp.ReferenceNo);
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strPayInqRefNo[%s]",strPayInqRefNo);

// 				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayJF[%d].szPayContral[2][%c]",PayNum,stPayJF[PayNum].szPayContral[2]);
				if(PayType == S_PAY1 && stPayJF[PayNum].szPayContral[2]=='1')     //��Ҫ��������ɷѽ��  by hyl            
				{       		        
			    	ScrNum = 210;
					
					m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%03d-%-10.10s", stPayMenuJF[PayNum].iPayNum,stPayMenuJF[PayNum].szPayName));
//					m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.f",Asc2Float(Accept.InAccountNum)));
					m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s",Accept.InAccountNum));
					if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
						m_pDevCmn->fnSCR_DisplayString(4, "50");
					else
						m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%.2f",Asc2Float(&HMERecv.Data[123][6], 12)/ 100));
					if (strVersion == VERSIONBEIJING)			// 060411 �����汾
					{
						if (stPayMenuJF[PayNum].iPayNum == 6)
						{
							m_pDevCmn->fnSCR_DisplayString(5, "�����˶Խ��ѽ����֪ͨ������Ƿ�һ�¡�");
							m_pDevCmn->fnSCR_DisplayString(6, "�粻�������˳�������̨���ѡ�");
						}
					}
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}

					strInAmount = strReturn;
					
					SplitString(strReturn, ".", strReturnArray);
					if (strReturnArray.GetSize() == 1)
					{
						if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
							fnAPP_CancelProc(T_INPUTERROR);
						
						strLeft = ZERO10 + strReturnArray[0];
						strLeft = strLeft.Right(10);
						strRight = ZERO2;										// 2004.11.05
					}
					else
						if (strReturnArray.GetSize() == 2)
						{
							if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
								fnAPP_CancelProc(T_INPUTERROR);
							if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
								fnAPP_CancelProc(T_INPUTERROR);
							
							strLeft = ZERO10 + strReturnArray[0];
							strLeft = strLeft.Right(10);
							strRight = strReturnArray[1] + ZERO2;
							strRight = strRight.Left(2);
						}
						else
							fnAPP_CancelProc(T_INPUTERROR);
						
						if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
							(Asc2Int(strRight) <= 0))
							fnAPP_CancelProc(T_INPUTERROR);
						else														// Amount Over Check
						if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
							((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
							fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
						else
						{
							memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
							memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
						}
					
				}
				else
				if( PayType == S_PAY2 )
				{
					ScrNum = 206;
					m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%03d-%-10.10s", stPayMenuCZ[PayNum].iPayNum,stPayMenuCZ[PayNum].szPayName));
					m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s",Accept.InAccountNum));
					if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
						m_pDevCmn->fnSCR_DisplayString(3, "50");
					else															// added by liuxl 20111012
					if (memcmp(&m_pProfile->NETWORK.BranchNum[0], "13", 2) == 0 &&
						stPayMenuCZ[PayNum].iPayNum == 101						&&
						PayType == S_PAY2)
						ScrNum = 205;
					else
						m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f",Asc2Float(&HMERecv.Data[123][8]+Asc2Int(&HMERecv.Data[123][6],2),12) / 100));
					if (strVersion == VERSIONBEIJING)			// 060411 �����汾
					{
						if ( stPayMenuCZ[PayNum].iPayNum == 1)
						{
							m_pDevCmn->fnSCR_DisplayString(5, "��ֵ�������Ϊ300Ԫ");
							m_pDevCmn->fnSCR_DisplayString(6, "�ұ���Ϊ��Ԫ��������");
						}
					}
					
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
					strReturn = m_pDevCmn->fstrSCR_GetKeyString();

                    if (strReturn == "30")							
				        memcpy(Accept.Money, "000000003000", 12);
					else
					if (strReturn == "50")
						memcpy(Accept.Money, "000000005000", 12);
					else
					if (strReturn == "200")
						memcpy(Accept.Money, "000000020000", 12);
					else
					if (strReturn == "300")
						memcpy(Accept.Money, "000000030000", 12);
					else
					if (strReturn == "500")
						memcpy(Accept.Money, "000000050000", 12);
					else
					if (strReturn == "2000")
						memcpy(Accept.Money, "000000200000", 12);
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
					if (strReturn == S_RETURN)
					{
					  MainTranReturnFlag = TRUE;
					  return T_OK;	
					}
					if (strReturn == S_OTHER)
					{
						ScrNum = 208;
						m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%03d-%-10.10s", stPayMenuCZ[PayNum].iPayNum,stPayMenuCZ[PayNum].szPayName));
						m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s",Accept.InAccountNum));
						if (m_pDevCmn->HostLineMode == HOST_LOCAL_MODE)	
							m_pDevCmn->fnSCR_DisplayString(4, "50");
						else															// added by liuxl 20111012
						if (memcmp(&m_pProfile->NETWORK.BranchNum[0], "13", 2) == 0 &&
							stPayMenuCZ[PayNum].iPayNum == 101						&&
							PayType == S_PAY2)
							ScrNum = 207;
						else
							m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%.2f",Asc2Float(&HMERecv.Data[123][8]+Asc2Int(&HMERecv.Data[123][6],2),12) / 100));
						
						if (strVersion == VERSIONBEIJING)		// 060411 �����汾
						{
							if ( stPayMenuCZ[PayNum].iPayNum == 1)
							{
								m_pDevCmn->fnSCR_DisplayString(5, "��ֵ�������Ϊ300Ԫ");
								m_pDevCmn->fnSCR_DisplayString(6, "�ұ���Ϊ��Ԫ��������");
							}
						}

						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
						if (strReturn == S_RETURN)
						{
						  MainTranReturnFlag = TRUE;
						  return T_OK;	
						}
					    else 
						{
							SplitString(strReturn, ".", strReturnArray);
							if (strReturnArray.GetSize() == 1)
							{
								if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
									fnAPP_CancelProc(T_INPUTERROR);
								
								strLeft = ZERO10 + strReturnArray[0];
								strLeft = strLeft.Right(10);
								strRight = ZERO2;										// 2004.11.05
							}
							else
							if (strReturnArray.GetSize() == 2)
							{
								if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
									fnAPP_CancelProc(T_INPUTERROR);
								if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
									fnAPP_CancelProc(T_INPUTERROR);
								
								strLeft = ZERO10 + strReturnArray[0];
								strLeft = strLeft.Right(10);
								strRight = strReturnArray[1] + ZERO2;
								strRight = strRight.Left(2);
							}
							else
								fnAPP_CancelProc(T_INPUTERROR);
							
							if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
								(Asc2Int(strRight) <= 0))
								fnAPP_CancelProc(T_INPUTERROR);
							else														// Amount Over Check
							if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
								((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
								fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
							else
							{
								memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
								memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
							}

						}
				
					}
					else
					{
						SplitString(strReturn, ".", strReturnArray);
						if (strReturnArray.GetSize() == 1)
						{
							if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
								fnAPP_CancelProc(T_INPUTERROR);
							
							strLeft = ZERO10 + strReturnArray[0];
							strLeft = strLeft.Right(10);
							strRight = ZERO2;										// 2004.11.05
						}
						else
							if (strReturnArray.GetSize() == 2)
							{
								if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
									fnAPP_CancelProc(T_INPUTERROR);
								if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
									fnAPP_CancelProc(T_INPUTERROR);
								
								strLeft = ZERO10 + strReturnArray[0];
								strLeft = strLeft.Right(10);
								strRight = strReturnArray[1] + ZERO2;
								strRight = strRight.Left(2);
							}
							else
								fnAPP_CancelProc(T_INPUTERROR);
							
							if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
								(Asc2Int(strRight) <= 0))
								fnAPP_CancelProc(T_INPUTERROR);
							else														// Amount Over Check
								if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
									((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
									fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
								else
								{
									memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
									memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
									MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
								}

					}
				}

				else
				if(PayType == S_PAY1 && stPayJF[PayNum].szPayContral[2]=='0')    //����Ҫ��������ɷѽ�� by hyl
				{
					if(Asc2Int(&HMERecv.Data[123][6], 12) == 0 )                     //��Ƿ����Ϣ�����Ƿ����Ϣ���˳�
					{   				
						ScrNum = 406;
						m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%03d-%-10s", stPayMenuJF[PayNum].iPayNum,stPayMenuJF[PayNum].szPayName));
						m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s",Accept.InAccountNum));
						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
						if (strReturn == S_RETURN)
						{
						  MainTranReturnFlag = TRUE;
						  return T_OK;	
						}		
					}
					else
					{
                        ScrNum=413;
						m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%03d-%-10.10s", stPayMenuJF[PayNum].iPayNum,stPayMenuJF[PayNum].szPayName));
						m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s",Accept.InAccountNum));
						m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f", Asc2Float(&HMERecv.Data[123][6],12) / 100));
						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);

						if (strVersion == VERSIONBEIJING)		// 060411 �����汾
						{
							if (stPayMenuJF[PayNum].iPayNum == 6)
							{
								m_pDevCmn->fnSCR_DisplayString(5, "�����˶Խ��ѽ����֪ͨ������Ƿ�һ�¡�");
								m_pDevCmn->fnSCR_DisplayString(6, "�粻�������˳�������̨���ѡ�");
							}
						}

						strReturn = m_pDevCmn->fstrSCR_GetKeyString();
						if (strReturn == S_CONTINUE)
						   memcpy(Accept.Money, &HMERecv.Data[123][6], 12);
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
						if (strReturn == S_RETURN)
						{
						  MainTranReturnFlag = TRUE;
						  return T_OK;	
						}
						
					}
					
				}
							
			}
			else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (HostResp.CardEjectFlag)								// Card Eject Flag
				{
					fnAPP_PMDProc();									// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																		// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}	
				else
				{
					if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
						(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					{
						fnAPP_PMDProc();							// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																	// Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					{
						fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
						MainTranReturnFlag = TRUE;
						return T_OK;								//�����أ����������׻���  by zjd
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
			}/////////////////////////
		}
		else
		if(PayType == S_PAY1 && stPayJF[PayNum].szPayContral[1]=='0')    ////����ҪԤ��ѯ   by hyl
		{
			ScrNum = 209;
			
			if (strVersion == VERSIONBEIJING)					// 060411 �����汾
			{
				if (stPayMenuJF[PayNum].iPayNum == 6)
				{
					m_pDevCmn->fnSCR_DisplayString(5, "�����˶Խ��ѽ����֪ͨ������Ƿ�һ�¡�");
					m_pDevCmn->fnSCR_DisplayString(6, "�粻�������˳�������̨���ѡ�");
				}
			}

			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
			if (strReturn == S_RETURN)
			{
			  MainTranReturnFlag = TRUE;
			  return T_OK;	
			}
			else 
			{
				SplitString(strReturn, ".", strReturnArray);
				if (strReturnArray.GetSize() == 1)
				{
					if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);
					
					strLeft = ZERO10 + strReturnArray[0];
					strLeft = strLeft.Right(10);
					strRight = ZERO2;										// 2004.11.05
				}
				else
					if (strReturnArray.GetSize() == 2)
					{
						if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
							fnAPP_CancelProc(T_INPUTERROR);
						if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
							fnAPP_CancelProc(T_INPUTERROR);
						
						strLeft = ZERO10 + strReturnArray[0];
						strLeft = strLeft.Right(10);
						strRight = strReturnArray[1] + ZERO2;
						strRight = strRight.Left(2);
					}
					else
						fnAPP_CancelProc(T_INPUTERROR);
					
					if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
						(Asc2Int(strRight) <= 0))
						fnAPP_CancelProc(T_INPUTERROR);
					else														// Amount Over Check
						if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
							((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
							fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
						else
						{
							memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
							memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
						}

			}
		}	
		else 
		if(PayType == S_PAY2 && stPayCZ[PayNum].szPayContral[1]=='0')
		{  
			/////////////////////////////////////////////////////////////////////////////
			memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
            /////////////////////////////////////////////////////////////////////////////
			ScrNum = 205;
			m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%03d-%-10.10s", stPayMenuCZ[PayNum].iPayNum,stPayMenuCZ[PayNum].szPayName));
			m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s",Accept.InAccountNum));
			
			if (strVersion == VERSIONBEIJING)					// 060411 �����汾
			{
				if ( stPayMenuCZ[PayNum].iPayNum == 1)
				{
					m_pDevCmn->fnSCR_DisplayString(5, "��ֵ�������Ϊ300Ԫ");
					m_pDevCmn->fnSCR_DisplayString(6, "�ұ���Ϊ��Ԫ��������");
				}
			}

			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
			strReturn = m_pDevCmn->fstrSCR_GetKeyString();

			if (strReturn == "30")							
				memcpy(Accept.Money, "000000003000", 12);
			else
			if (strReturn == "50")
				memcpy(Accept.Money, "000000005000", 12);
			else
			if (strReturn == "200")
				memcpy(Accept.Money, "000000020000", 12);
			else
			if (strReturn == "300")
				memcpy(Accept.Money, "000000030000", 12);
			else
			if (strReturn == "500")
				memcpy(Accept.Money, "000000050000", 12);
			else
			if (strReturn == "2000")
				memcpy(Accept.Money, "000000200000", 12);
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
			if (strReturn == S_RETURN)
			{
			  MainTranReturnFlag = TRUE;
			  return T_OK;	
			}
			else
			if (strReturn == S_OTHER)
			{
				ScrNum = 207;
                m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%03d-%-10.10s", stPayMenuCZ[PayNum].iPayNum,stPayMenuCZ[PayNum].szPayName));
				m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%s",Accept.InAccountNum));
				
				if (strVersion == VERSIONBEIJING)				// 060411 �����汾
				{
					if ( stPayMenuCZ[PayNum].iPayNum == 1)
					{
						m_pDevCmn->fnSCR_DisplayString(5, "��ֵ�������Ϊ300Ԫ");
						m_pDevCmn->fnSCR_DisplayString(6, "�ұ���Ϊ��Ԫ��������");
					}
				}

				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
				if (strReturn == S_RETURN)
				{
				   MainTranReturnFlag = TRUE;
				   return T_OK;	
				}
				else 
				{
					SplitString(strReturn, ".", strReturnArray);
					if (strReturnArray.GetSize() == 1)
					{
						if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
							fnAPP_CancelProc(T_INPUTERROR);
						
						strLeft = ZERO10 + strReturnArray[0];
						strLeft = strLeft.Right(10);
						strRight = ZERO2;										// 2004.11.05
					}
					else
						if (strReturnArray.GetSize() == 2)
						{
							if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
								fnAPP_CancelProc(T_INPUTERROR);
							if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
								fnAPP_CancelProc(T_INPUTERROR);
							
							strLeft = ZERO10 + strReturnArray[0];
							strLeft = strLeft.Right(10);
							strRight = strReturnArray[1] + ZERO2;
							strRight = strRight.Left(2);
						}
						else
							fnAPP_CancelProc(T_INPUTERROR);
						
						if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
							(Asc2Int(strRight) <= 0))
							fnAPP_CancelProc(T_INPUTERROR);
						else														// Amount Over Check
							if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
								((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
								fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
							else
							{
								memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
								memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
								MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
							}

				}
			}
			else 
			{
				SplitString(strReturn, ".", strReturnArray);
				if (strReturnArray.GetSize() == 1)
				{
					if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
						fnAPP_CancelProc(T_INPUTERROR);
					
					strLeft = ZERO10 + strReturnArray[0];
					strLeft = strLeft.Right(10);
					strRight = ZERO2;										// 2004.11.05
				}
				else
					if (strReturnArray.GetSize() == 2)
					{
						if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
							fnAPP_CancelProc(T_INPUTERROR);
						if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
							fnAPP_CancelProc(T_INPUTERROR);
						
						strLeft = ZERO10 + strReturnArray[0];
						strLeft = strLeft.Right(10);
						strRight = strReturnArray[1] + ZERO2;
						strRight = strRight.Left(2);
					}
					else
						fnAPP_CancelProc(T_INPUTERROR);
					
					if ((Asc2Float(strLeft) <= 0)	&&							// Input Data Check
						(Asc2Int(strRight) <= 0))
						fnAPP_CancelProc(T_INPUTERROR);
					else														// Amount Over Check
						if (((Asc2Float(strLeft) + (Asc2Float(strRight) / 100)) > TMoneyMaxOut) ||
							((Asc2Float(strLeft) + (Asc2Float(strRight) / 100))> CSH_MONEY_TMAXOUT))
							fnAPP_CancelProc(T_MSG, TranMsgTbl[21][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
						else
						{
							memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
							memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
							MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.Money[%s]", Accept.Money);
						}

			}
		}
// 		}
// 		ReTryCnt = 3;
//		while (ReTryCnt)
//		{
//			ReTryCnt --;
// 		}
	}
    else
	if(TranCode == TC_AC_REMIT)
	{

        // ����5407��ѯ�˻���� ȡ������� BY HYL
		m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
        Delay_Msg(1000);
		ACTranCode = TC_AC_BALANCE_INQ;
		fnAPP_ACSendHost();
		fnAPP_ACRecvHost();
		if( ACHostResp.ReturnCode !=0)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;		
		}

		ScrNum = 222;			 //A/C MANAGERת�˻�� ��8��								
		m_pDevCmn->fnSCR_DisplayNumeric(2, ACHostRespRemitAssu[ACHostResp.RecNum].ArrayNum);

		
		if((Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 4) ||
			(Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 5 )) 
		{ 
			 m_pDevCmn->fnSCR_DisplayString(3,  GetSprintf("%6.6s",ACHostRespRemitAssu[ACHostResp.RecNum].PayeePostCode));
			 m_pDevCmn->fnSCR_DisplayString(4, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAdd);
		}
		else
		if((Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 6)  ||
		   (Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 7)  ||
		   (Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 8)  ||
		   (Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 9)  ||
		   (Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 10))
		{
			 m_pDevCmn->fnSCR_DisplayString(3, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount);
			 m_pDevCmn->fnSCR_DisplayString(4, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeBankName);
		}
		
		m_pDevCmn->fnSCR_DisplayString(5, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeName);			//����Ӧ��������������  by zjd
		m_pDevCmn->fnSCR_DisplayString(6, ACHostRespRemitAssu[ACHostResp.RecNum].TranName);
 
		m_pDevCmn->fnSCR_DisplayString(7, ACHostResp.AcBalance);           //�������
		m_pDevCmn->fnSCR_DisplayString(8, ACHostRespRemitAssu[ACHostResp.RecNum].SumLimit);            //����޶�
		


		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();				// 2004.11.05
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				//�����أ����������׻���  by zjd
		}
		
		memcpy(ACHostSend.TranMoney,strReturn.GetBuffer(0),strReturn.GetLength());



        //���Կͻ�����Ľ��
		
		memcpy(ACHostSend.TranType, ACHostRespRemitAssu[ACHostResp.RecNum].TranType,sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].TranType));    //�趨��������뽻�׽��
		m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
        Delay_Msg(1000);
		ACTranCode = TC_AC_CHARGE;
		fnAPP_ACSendHost();
        fnAPP_ACRecvHost();
        if( ACHostResp.ReturnCode !=0)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;		
		}

		ScrNum =  421;                     //A/C MANAGER��9��
		m_pDevCmn->fnSCR_DisplayString(1, ACHostSend.TranMoney);                      //���׽��
		m_pDevCmn->fnSCR_DisplayString(2, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeName);                      //�տ�������
		m_pDevCmn->fnSCR_DisplayString(3, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeBankName);                      //ת������
		m_pDevCmn->fnSCR_DisplayString(4, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAdd);                      //�տ����˺� ��ַ
		m_pDevCmn->fnSCR_DisplayString(5, ACHostResp.CommCharge);                      //������
		m_pDevCmn->fnSCR_DisplayString(6, ACHostRespRemitAssu[ACHostResp.RecNum].TranName);             			//��������
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				//�����أ����������׻���  by zjd
		}
		else
		if (strReturn == S_ENTER)
		{
			if( Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 5 )                            //�ж��Ƿ�Ϊ������
			{
				TranCode = TC_AC_REMIT_PWD;
				ScrNum =  120;                 //A/C MANAGER �������10��

                while(TRUE)
				{
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
					if (strReturn == S_CLEAR)
						continue; 
					else
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;			
					}
					else
					{
						memcpy(ACHostSend.ReservePin, strReturn.GetBuffer(0), strReturn.GetLength());
// 						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "strReturn ReservePin[%s]", ACHostSend.ReservePin);    //Ҫȥ�� by hyl
						ScrNum =  422;               //A/C MANAGER �������11����
						m_pDevCmn->fnSCR_DisplayString(1, ACHostSend.TranMoney);
						m_pDevCmn->fnSCR_DisplayString(2, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeName);
						m_pDevCmn->fnSCR_DisplayString(3, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAdd);
						m_pDevCmn->fnSCR_DisplayString(4, ACHostResp.CommCharge);
// 							m_pDevCmn->fnSCR_DisplayString(5, ACHostResp.TranName);

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
						if (strReturn == S_RETURN)
						{
							MainTranReturnFlag = TRUE;
							return T_OK;				
						}
						else
						if (strReturn == S_ENTER)
						{

							memcpy(ACHostSend.ReserveNum,ACHostRespRemitAssu[ACHostResp.RecNum].ReserveNum,sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].ReserveNum));
							
							m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
                            Delay_Msg(1000);
							ACTranCode = TC_AC_ASSU_TRAN;
							fnAPP_ACSendHost();
							fnAPP_ACRecvHost();
							if(ACHostResp.ReturnCode == 0)
							{
								fnAPP_PMEAcceptACMangerContinue();
								MainTranReturnFlag = TRUE;
							    return T_OK;
							}
							else
							{
								MainTranReturnFlag = TRUE;
								return T_OK; 
							}
						}
					}
				}
			}
			else
			if( Asc2Int(ACHostRespRemitAssu[ACHostResp.RecNum].TranType) == 9 )                            //�ж��Ƿ�Ϊһ��ͨת��
			{
				TranCode = TC_AC_REMIT_FIX;              
				
				ScrNum = 33;                 //һ��ͨ��10��
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;			
				}
				else
				{
					ACHostSend.DepTime = Asc2Int(strReturn);
					ScrNum = 423;                 //һ��ͨ��11��
					m_pDevCmn->fnSCR_DisplayString(1, ACHostSend.TranMoney);     //��������ȡ������
					m_pDevCmn->fnSCR_DisplayString(2, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeName);
					m_pDevCmn->fnSCR_DisplayString(3, ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount);
					m_pDevCmn->fnSCR_DisplayString(4, strReturn);   //���ڴӺζ���
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);																

					strReturn1 = m_pDevCmn->fstrSCR_GetKeyString();
					if (strReturn1 == S_EXIT)
						fnAPP_CancelProc(T_EXIT);
					else 
					if (strReturn1 == S_CANCEL)
						fnAPP_CancelProc(T_CANCEL);
					else
					if (strReturn1 == S_TIMEOVER)
						fnAPP_CancelProc(T_TIMEOVER);
					else
					if (strReturn1 == S_INPUTOVER)
						fnAPP_CancelProc(T_INPUTOVER);
					else
					if (strReturn1 == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}
					else
					if (strReturn1 == S_ENTER)
					{
						memcpy(ACHostSend.ReserveNum,ACHostRespRemitAssu[ACHostResp.RecNum].ReserveNum,sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].ReserveNum));
						m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
                        Delay_Msg(1000);
						ACTranCode = TC_AC_ASSU_TRAN;
						fnAPP_ACSendHost();
						fnAPP_ACRecvHost();
						
						if(ACHostResp.ReturnCode == 0)
						{
							fnAPP_PMEAcceptACMangerContinue();
							MainTranReturnFlag = TRUE;
							return T_OK;

						}
						else
						{
							MainTranReturnFlag = TRUE;
							return T_OK; 
						}
					}
				}
				 
					
				
			}
			else                  //�������һ��ͨ����Ļ��
			{
				
                memcpy(ACHostSend.ReserveNum,ACHostRespRemitAssu[ACHostResp.RecNum].ReserveNum,sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].ReserveNum));
				TranCode = TC_AC_REMIT;
				m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
                Delay_Msg(1000);
				ACTranCode = TC_AC_ASSU_TRAN;
				fnAPP_ACSendHost();
				fnAPP_ACRecvHost();

				if(ACHostResp.ReturnCode == 0)
				{
					fnAPP_PMEAcceptACMangerContinue();
					MainTranReturnFlag = TRUE;
					return T_OK;

				}
				else
				{
					MainTranReturnFlag = TRUE;
					return T_OK; 
				}
			}

		}
		                          
	}
	else
	if(TranCode == TC_AC_PAY_ASSU)
	{	
		ScrNum = 429;			//֧��Ԥ�� Լ���ɷѵ�9,10��		
		m_pDevCmn->fnSCR_DisplayString(1, ACHostRespPayAssu[ACHostResp.RecNum].JHNumber);	

		if( ACHostRespPayAssu[ACHostResp.RecNum].TranTypeAttri == 0 ||
		    ACHostRespPayAssu[ACHostResp.RecNum].TranTypeAttri == 1)                      //֧��Ԥ�ɷ� 
		{
		   m_pDevCmn->fnSCR_DisplayString(2, ACHostSend.TranMoney);
		}
		else
		{
           m_pDevCmn->fnSCR_DisplayString(2, ACHostResp.JHAmount);
		   memcpy(ACHostSend.TranMoney,ACHostResp.JHAmount,sizeof(ACHostResp.JHAmount));
		}

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_INPUT_MODE);																
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();				// 2004.11.05
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				
		}
		else
		if (strReturn == S_ENTER)
		{   
	        memcpy(ACHostSend.ReserveNum,ACHostRespPayAssu[ACHostResp.RecNum].ReserveNum,sizeof(ACHostRespPayAssu[ACHostResp.RecNum].ReserveNum));
			MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.ReserveNum[%s]", ACHostSend.ReserveNum);
			
			m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
            Delay_Msg(1000);
			ACTranCode = TC_AC_ASSU_TRAN;
			fnAPP_ACSendHost();
			fnAPP_ACRecvHost();

			if(ACHostResp.ReturnCode == 0)
			{
				fnAPP_PMEAcceptACMangerContinue();
				MainTranReturnFlag = TRUE;
				return T_OK;

			}
			else
			{
				MainTranReturnFlag = TRUE;
				return T_OK; 
			}
				
		}		 
        
	}
    else
	if( TranCode == TC_AC_PAY_NOASSU)      //��Լ���ɷ�
	{
  
		memcpy(ACHostSend.TranType,ACHostRespPayUnAssu[ACHostResp.RecNum].TranType,sizeof(ACHostRespPayUnAssu[ACHostResp.RecNum].TranType));
		m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
        Delay_Msg(1000);
		ACTranCode = TC_AC_PAY_INQ;           //��������ɷѺ����ѯӦ�ɷѽ��
		fnAPP_ACSendHost();
		fnAPP_ACRecvHost();
		if( ACHostResp.ReturnCode !=0)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;		
		}
		

       if( ACHostRespPayUnAssu[ACHostResp.RecNum].TranTypeAttri == 0 ||
		   ACHostRespPayUnAssu[ACHostResp.RecNum].TranTypeAttri == 1)   //֧��Ԥ�ɷ�                 
	   { 
			ScrNum = 223;					//��Ԥ�ɷ�       ��Լ���ɷѵ�10��	
			m_pDevCmn->fnSCR_DisplayString(2, Accept.InAccountNum);			
			m_pDevCmn->fnSCR_DisplayString(3, ACHostResp.JHAmount);
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

			strReturn = m_pDevCmn->fstrSCR_GetKeyString();				// 2004.11.05
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
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
			else
			{ 
         		memcpy(ACHostSend.TranMoney,strReturn.GetBuffer(0),strReturn.GetLength());			
			}	
	   }

		ScrNum = 429;			//֧��Ԥ�� ��Լ���ɷѵ�9,10��		
		m_pDevCmn->fnSCR_DisplayString(1, Accept.InAccountNum);	

		if( ACHostRespPayUnAssu[ACHostResp.RecNum].TranTypeAttri == 0 ||
			ACHostRespPayUnAssu[ACHostResp.RecNum].TranTypeAttri == 1)                      //֧��Ԥ�ɷ� 
		{
		   m_pDevCmn->fnSCR_DisplayString(2, ACHostSend.TranMoney);
		}
		else
		{
		   m_pDevCmn->fnSCR_DisplayString(2, ACHostResp.JHAmount);
		   memcpy(ACHostSend.TranMoney,ACHostResp.JHAmount,sizeof(ACHostResp.JHAmount));
		}

		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_INPUT_MODE);																
		strReturn = m_pDevCmn->fstrSCR_GetKeyString();				// 2004.11.05
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
		if (strReturn == S_RETURN)  //�����أ����������׻���  
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				
		}
		else
		if (strReturn == S_ENTER)
		{
			m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
            Delay_Msg(1000);
			ACTranCode = TC_AC_NOASSU_PAY;
			fnAPP_ACSendHost();
			fnAPP_ACRecvHost();

			if(ACHostResp.ReturnCode == 0)
			{
				fnAPP_PMEAcceptACMangerContinue();
				MainTranReturnFlag = TRUE;
				return T_OK;

			}
			else
			{
				MainTranReturnFlag = TRUE;
				return T_OK; 
			}
				
		}
	
		

	}
	else		// added by yusy ic ָ���˻�Ȧ��������
	if (TranCode == TC_LOAD)
	{
		fnAPP_AcceptIcLoadAccountMoney();						// Accept Ic Load Account Money
		if (MainTranReturnFlag)
			return T_OK;
		fnAPP_AcceptIcLoadAccountMoneyConfirm();				// Accept Ic Load Account Money Confirm
	}
	else		// added by yusy ic �޸�С���˻��������
	if (TranCode == TC_BOUNDMDOIFY)
	{
		fnAPP_AcceptIcLimitConfirm();
		if (MainTranReturnFlag)
			return T_OK;
		fnAPP_AcceptIcLimitMoney();
	}

/////////////////////////////////////////////////////////////////////////////
//	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <TAMOUNT INPUT : %10.10s.%2.2s>",
//											GetTime().GetBuffer(0),
//											&(GetTime().GetBuffer(0)[2]),
//											&(GetTime().GetBuffer(0)[4]),
//											Accept.Money,
//											&Accept.Money[10]));// 2005.03.22
/////////////////////////////////////////////////////////////////////////////
	return T_OK;
}

// Accept Account Deposit Money
int CTranCmn::fnAPP_AcceptIcLoadAccountMoney()
{
	int				ScrNum = 0;
	CString			strReturn("");
	CString			strLeft("");
	CString			strRight("");
	CStringArray	strReturnArray;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
	FirstPbcECInq = Asc2Float(fstrAPD_PbcECInq()) / 100;
/////////////////////////////////////////////////////////////////////////////

	if ((Asc2Float(fstrAPD_PbcECLimit()) / 100) - (Asc2Float(fstrAPD_PbcECInq()) / 100) <= 0)
	{
		ScrNum = 68;
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_RETURN)
		{
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
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
	}
	else
	if ((Asc2Float(fstrAPD_PbcECLimit()) / 100) - (Asc2Float(fstrAPD_PbcECInq()) / 100) < InitInfo.nIcLoadMinNum)
	{
		ScrNum = 69;
		m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%.2f", InitInfo.nIcLoadMinNum));
		m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

		strReturn = m_pDevCmn->fstrSCR_GetKeyString();
		if (strReturn == S_RETURN)
		{
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
			fnAPP_CancelProc(T_TIMEOVER);
		else
		if (strReturn == S_INPUTOVER)
			fnAPP_CancelProc(T_INPUTOVER);
	}

	ScrNum = 224;												// Input Pay Amount Screen
	m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f", InitInfo.nIcLoadMinNum));
	m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f", (Asc2Float(fstrAPD_PbcECLimit()) / 100) - (Asc2Float(fstrAPD_PbcECInq()) / 100)));
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_RETURN)
	{
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
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	
	SplitString(strReturn, ".", strReturnArray);
	if (strReturnArray.GetSize() == 1)
	{
		if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
			fnAPP_CancelProc(T_INPUTERROR);

		strLeft = ZERO10 + strReturnArray[0];
		strLeft = strLeft.Right(10);
		strRight = ZERO2;
	}
	else
	if (strReturnArray.GetSize() == 2)
	{
		if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
			fnAPP_CancelProc(T_INPUTERROR);
		if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
			fnAPP_CancelProc(T_INPUTERROR);

		strLeft = ZERO10 + strReturnArray[0];
		strLeft = strLeft.Right(10);
		strRight = strReturnArray[1] + ZERO2;
		strRight = strRight.Left(2);
	}
	else
		fnAPP_CancelProc(T_INPUTERROR);

	if ((Asc2Int(strLeft) <= 0)	&&								// Input Data Check
		(Asc2Int(strRight) <= 0))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
		memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
	}

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <ICAMOUNT INPUT : %10.10s.%2.2s>",
											GetTime().GetBuffer(0),
											&(GetTime().GetBuffer(0)[2]),
											&(GetTime().GetBuffer(0)[4]),
											Accept.Money,
											&Accept.Money[10]));
/////////////////////////////////////////////////////////////////////////////
	return T_OK;
}

// Accept Ic Limit Money
int CTranCmn::fnAPP_AcceptIcLimitMoney()
{
	int				ScrNum = 225;
	CString			strReturn("");
	CString			strLeft("");
	CString			strRight("");
	CStringArray	strReturnArray;

/////////////////////////////////////////////////////////////////////////////
	memset(Accept.Money, '0', sizeof(Accept.Money));			// Money
/////////////////////////////////////////////////////////////////////////////

	m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f", Asc2Float(fstrAPD_PbcECInq()) / 100));
	if ((Asc2Float(fstrAPD_PbcECInq()) / 100) > InitInfo.nIcLoadMinNum)
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f", Asc2Float(fstrAPD_PbcECInq()) / 100));
	else
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%.2f", InitInfo.nIcLoadMinNum));
	m_pDevCmn->fnSCR_DisplayString(4, GetSprintf("%.2f", InitInfo.nIcLoadMaxNum));
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																
																
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_RETURN)
	{
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
		fnAPP_CancelProc(T_TIMEOVER);
	else
	if (strReturn == S_INPUTOVER)
		fnAPP_CancelProc(T_INPUTOVER);
	
	SplitString(strReturn, ".", strReturnArray);
	if (strReturnArray.GetSize() == 1)
	{
		if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
			fnAPP_CancelProc(T_INPUTERROR);

		strLeft = ZERO10 + strReturnArray[0];
		strLeft = strLeft.Right(10);
		strRight = ZERO2;
	}
	else
	if (strReturnArray.GetSize() == 2)
	{
		if (!IsNum(strReturnArray[0].GetBuffer(0), strReturnArray[0].GetLength()))
			fnAPP_CancelProc(T_INPUTERROR);
		if (!IsNum(strReturnArray[1].GetBuffer(0), strReturnArray[1].GetLength()))
			fnAPP_CancelProc(T_INPUTERROR);

		strLeft = ZERO10 + strReturnArray[0];
		strLeft = strLeft.Right(10);
		strRight = strReturnArray[1] + ZERO2;
		strRight = strRight.Left(2);
	}
	else
		fnAPP_CancelProc(T_INPUTERROR);

	if ((Asc2Int(strLeft) <= 0)	&&								// Input Data Check
		(Asc2Int(strRight) <= 0))
		fnAPP_CancelProc(T_INPUTERROR);
	else
	{
		memcpy(Accept.Money, strLeft.GetBuffer(0), 10);
		memcpy(&Accept.Money[10], strRight.GetBuffer(0), 2);
	}

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s <ICAMOUNT INPUT : %10.10s.%2.2s>",
											GetTime().GetBuffer(0),
											&(GetTime().GetBuffer(0)[2]),
											&(GetTime().GetBuffer(0)[4]),
											Accept.Money,
											&Accept.Money[10]));
/////////////////////////////////////////////////////////////////////////////
	return T_OK;
}

// Accept Account Deposit Confirm
int CTranCmn::fnAPP_AcceptIcLoadAccountMoneyConfirm()
{
	int			ScrNum = 0;
	CString		strReturn("");

	ScrNum = 441;												// Confirm Account Deposit Screen
	m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%.2f", Asc2Float(Accept.Money, sizeof(Accept.Money)) / 100));
	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);		// added by liuxl 20110926
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_ENTER)
		;
	else
	if (strReturn == S_RETURN)
		MainTranReturnFlag = TRUE;
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

// Accept Ic Limit Confirm
int CTranCmn::fnAPP_AcceptIcLimitConfirm()
{
	int			ScrNum = 442;
	CString		strReturn("");
																// Pbc EC Inq First
	m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%.2f", Asc2Float(fstrAPD_PbcECLimit()) / 100));
	m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%.2f", Asc2Float(fstrAPD_PbcECInq()) / 100));

	m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
	strReturn = m_pDevCmn->fstrSCR_GetKeyString();
	if (strReturn == S_CONFIRM)
		;
	else
	if (strReturn == S_RETURN)
		MainTranReturnFlag = TRUE;
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

// Accept New PassWord
int CTranCmn::fnAPP_AcceptNewPassWord(int firstFlag)
{
	int			ScrNum = 0;
	int			RetryCnt = 0;
	int		    iReloadTimes = 0;
	CString		strFirstPin("");
	CString		strSecondPin("");
	CString		strReturn("");

///////////////////////////////////////////////////////////////////////////// added by yusy 2010.03.22
	//memset(Accept.PassWord, 'F', sizeof(Accept.PassWord));	// PassWord
	iReloadTimes = 0;
	ScrNum = 103;
	while (TRUE)
	{
		m_pDevCmn->fnSCR_DisplayString(3, GetSprintf("%d", firstFlag));
		m_pDevCmn->fnSCR_DisplayNumeric(2, iReloadTimes);

		if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, PIN_PASSWORD_MIN, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				//�����أ����������׻���  by zjd
		}
		else
		{
			if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
			{ 
				CString PinKeyData = m_pDevCmn->fstrPIN_GetPinKeyData();
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
/////////////////////////////////////////////////////////////////////////////

	
/////////////////////////////////////////////////////////////////////////////
	memset(Accept.NewPassWord, 'F', sizeof(Accept.NewPassWord));	// New PassWord
/////////////////////////////////////////////////////////////////////////////
	
	RetryCnt = 3;
	
	while (RetryCnt)
	{
		ScrNum = 111;										// Input First New PassWord Screen
	    iReloadTimes = 0;
        while(TRUE)
		{
			//EPP ADD BY HYL 20070902
			m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

			if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
			   m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				//�����أ����������׻���  by zjd
			}
			else
			{
				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				{
				   strFirstPin = m_pDevCmn->fstrPIN_GetPinKeyData();     // Save PassWord First 
				   //�޸�FLASH��λ������ʱ������ȷ��,����������������
				   if  (strFirstPin.GetLength() == 0)
					   fnAPP_CancelProc(T_ERROR);
				}
			    else
				   strFirstPin = strReturn;								// Save PassWord First 
			}
			break;
		}

		
		ScrNum = 112; // Input Second New PassWord Screen
        RetryCnt = 0;
		iReloadTimes = 0;
		while(TRUE)
		{
	         //EPP ADD BY HYL 20070902
            m_pDevCmn->fnSCR_DisplayNumeric(1, iReloadTimes);

			if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_PASSWORD_MODE4, CardPan, 6, PIN_PASSWORD_MAX, PIN_PASSWORD_AUTO_FALSE, PIN_PASSWORD_TERM);
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
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				//�����أ����������׻���  by zjd
			}
			else
			{   
				if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)
				{
				   strSecondPin = m_pDevCmn->fstrPIN_GetPinKeyData();   // Save PassWord Second
				   if  (strSecondPin.GetLength() == 0)					// liyi chang 2008.10.05
					   fnAPP_CancelProc(T_ERROR);
				}
				else
				   strSecondPin = strReturn;							// Save PassWord Second
			}
			break;
		}
		

		if (strFirstPin == strSecondPin)							// Check PassWord 
		{
			if(m_pProfile->DEVICE.KeyHandler == TRIPLE_EPP)   //EPP ADD BY HYL
            { 
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_MenuDeviceProc:PinKeyData3[%s]", strFirstPin);
				if (strFirstPin.GetLength() >= sizeof(Accept.NewPassWord))
					memcpy(Accept.NewPassWord, strFirstPin.GetBuffer(0), sizeof(Accept.NewPassWord));
				else
					memcpy(Accept.NewPassWord, strFirstPin.GetBuffer(0), strFirstPin.GetLength());
			}
			else
			if (strFirstPin.GetLength() == 6)
			{
				memcpy(Accept.NewPassWord, "06", 2);
				memcpy(Accept.NewPassWord + 2, strFirstPin.GetBuffer(0), 6);
			}
			else
				fnAPP_CancelProc(T_PROGRAM);
			break;
		}

		if (!--RetryCnt)
		{
			fnAPP_CancelProc(T_INPUTOVER);
		}

		ScrNum = 113;											// Confirm New PassWord Screen
		iReloadTimes = 0;
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
	}
	
	return T_OK;
}

// Accept In Account Number
int CTranCmn::fnAPP_AcceptInAccountNum()
{
	int			ScrNum = 0;
	int			RetryCnt = 0;
	CString		strFirstAccount("");
	CString		strSecondAccount("");
	CString		strReturn("");
																		// 2008.10.05
	int				iChoiceNo = -1;
	int				nAvailNum = 0;				//��Ч�����˻���Ŀ
	char			OtherInq[999*36];			//�˻�����
	memset(OtherInq,0,sizeof(OtherInq));
	int				nOtherCurCnt = 0;			//һ������õ����˻���Ŀ

	ZzhTotalNum = 0;							//�ܵ����˻���Ŀ
	ZzhCurNum = 0;								//�Ѿ�ȡ�õ����˻���Ŀ
	
/////////////////////////////////////////////////////////////////////////////
	Accept.InAccountNumSize = 0;								// In Account Number Size
	memset(Accept.InAccountNum, 0, sizeof(Accept.InAccountNum));
																// In Account Number
/////////////////////////////////////////////////////////////////////////////

	if (TranCode == TC_REMIT)
	{
        MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitType[%s]", RemitType);
		if (RemitType == S_REMIT_ADD || RemitType == S_REMIT_PWD)
		{	
			TranCode = TC_REMIT;
			
			ZzhCurNum = 0;
			fnAPP_SendHost();
			fnAPP_RecvHost();

			if (m_pDevCmn->TranResult)                                  
			{
				ZzhTotalNum = Asc2Int(&HMERecv.Data[123][3],3);
				nOtherCurCnt = Asc2Int(&HMERecv.Data[123][6],3);

				if (ZzhTotalNum == 0)
				{
					ScrNum = 57;											// Ԥ���տ�����Ϣ������
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
					strReturn = m_pDevCmn->fstrSCR_GetKeyString();
					if (strReturn == S_CONTINUE)
					{
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
						fnAPP_CancelProc(T_TIMEOVER);
					else
					if (strReturn == S_INPUTOVER)
						fnAPP_CancelProc(T_INPUTOVER);
					else
						fnAPP_CancelProc(T_PROGRAM);			
				}

				ZzhCurNum += nOtherCurCnt;
			
				for(int i = 0;i<nOtherCurCnt;i++)
				{
					memcpy(&OtherInq[nAvailNum*73],&(HMERecv.Data[123][9+3+i*73]),73);
					nAvailNum++;
				}
			
				while(ZzhCurNum < ZzhTotalNum)
				{
					fnAPP_SendHost();
					fnAPP_RecvHost();
					
					if (m_pDevCmn->TranResult)
					{
						nOtherCurCnt = Asc2Int(&HMERecv.Data[123][6],3);
						
						ZzhCurNum += nOtherCurCnt;

						for(int i = 0;i<nOtherCurCnt;i++)
						{
							memcpy(&OtherInq[nAvailNum*73],&(HMERecv.Data[123][9+3+i*73]),73);
							nAvailNum++;
						}
					}
					else
					{
						break;
					}
				}


				ScrNum = 405;
				int j = 0;
				int nCurNum = nAvailNum;
				while(TRUE)
				{
					for(int i = 0; i<__min(5,nCurNum);i++)
					{
						m_pDevCmn->fnSCR_DisplayString(i*3 + 1,&OtherInq[(i+j) * 73],3);
						m_pDevCmn->fnSCR_DisplayString(i*3 + 2,&OtherInq[(i+j) * 73 + 3],30);
						m_pDevCmn->fnSCR_DisplayString(i*3 + 3,&OtherInq[(i+j) * 73 + 33],40);
					}
					
					j += 5;
					nCurNum -= 5;

					// added by liuxl 20110926
					if (j <= 5)
						m_pDevCmn->fnSCR_DisplayImage(5, FALSE);
					if (nCurNum <= 0)
						m_pDevCmn->fnSCR_DisplayImage(6, FALSE);

					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);																

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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;											//�����أ����������׻���
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
					else
					{
						//m_pTranCmn->strRemitAddNo = strReturn;
						for (int k=0;k<=nAvailNum;k++)
						{
							if (memcmp(strReturn.GetBuffer(0),&OtherInq[k*73],3) == 0)
							{
								m_pTranCmn->strRemitAddNo=strReturn;
			memcpy(Accept.InAccountNum,strReturn.GetBuffer(0),strReturn.GetLength());
			Accept.InAccountNumSize = strReturn.GetLength();
 	        MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.InAccountNum[%s]", Accept.InAccountNum);
								char sTemp[128];
								memset(sTemp,0,sizeof(sTemp));
								memcpy(sTemp,&OtherInq[k*73+3],30);
								m_pTranCmn->strRemitAddName=GetSprintf("%s",sTemp);
								memset(sTemp,0,sizeof(sTemp));
								memcpy(sTemp,&OtherInq[k*73+33],40);
								m_pTranCmn->strRemitAddAdd=GetSprintf("%s",sTemp);
								iChoiceNo = k;
								break;
							}
						}
						if (iChoiceNo == -1)
						{
							ScrNum = 58;											// �տ��˴��벻����
							m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);
							strReturn = m_pDevCmn->fstrSCR_GetKeyString();
							if (strReturn == S_CONTINUE)
							{
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
								fnAPP_CancelProc(T_TIMEOVER);
							else
							if (strReturn == S_INPUTOVER)
								fnAPP_CancelProc(T_INPUTOVER);
							else
								fnAPP_CancelProc(T_PROGRAM);	
						}
					}
					break;
				}
			}
			else 
			if (memcmp(HostResp.RejectCode, "26", 2) == 0)				// ��ʾ��������������ҳ��  by zjd
			{
				fnAPP_PMEAcceptPassWord();								// Accept PassWord
			}
			else														// ��ʾ������Ϣ��������Ӧ����  by zjd
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
					fnAPP_CancelProc(T_CANCEL);
				}
				else
				if (HostResp.CardEjectFlag)								// Card Eject Flag
				{
					fnAPP_PMDProc();									// Message Display
					fnAPD_CheckDeviceAction(DEV_MCU);
					fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																		// Card Out
					fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
					fnAPP_CancelProc(T_CANCEL);
				}	
				else
				{
					if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
						(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
					{
						fnAPP_PMDProc();							// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
																	// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);	
																	// Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
					{
						fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
						MainTranReturnFlag = TRUE;
						return T_OK;								//�����أ����������׻���  by zjd
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
			}
		}   
		else 
		if (RemitType == S_REMIT_IN) 
		{
			ScrNum = 318;
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
			if (strReturn == S_RETURN)  
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
			else
                m_pTranCmn->strRemitInNo = strReturn;			
		}
	}
	else
	if (TranCode == TC_TRANSFER)								// Accept Transfer
	{
		RetryCnt = 3;
		int iNGCnt = 0;
		while (RetryCnt)
		{
			memset(Accept.InAccountNum, 0, sizeof(Accept.InAccountNum));
			ScrNum = 301;											// Input First Account Screen
			m_pDevCmn->fnSCR_DisplayNumeric(1, IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANSDAYMAXAMOUNT, 50000));
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
			if (strReturn == S_RETURN)
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				//�����أ����������׻���  by zjd
			}
			else
			if ((strReturn.GetLength() < 13)	||					// Input Data Check
				(strReturn.GetLength() > sizeof(Accept.InAccountNum)))
				fnAPP_CancelProc(T_INPUTERROR);
			else
			{
				strFirstAccount = strReturn;						// Save First Account
			}

///////////////////////////////////////////////////////////////////////////// added by yusy 2010.03.22
			if(fnAPL_checkLocalCard(strFirstAccount))
			{
				if (strFirstAccount.GetLength() >= sizeof(Accept.InAccountNum))
				{
					Accept.InAccountNumSize = sizeof(Accept.InAccountNum);
					memcpy(Accept.InAccountNum, strFirstAccount.GetBuffer(0), sizeof(Accept.InAccountNum));
				}
				else
				{
					Accept.InAccountNumSize = strFirstAccount.GetLength();
					memcpy(Accept.InAccountNum, strFirstAccount.GetBuffer(0), strFirstAccount.GetLength());
				}

				/*******�շ����Ĵ�*******/
				TranCode = TC_CUS_INF_INQ;
				TranCode2 = 0;

				if(transInqCnt == 3)
					fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				
				fnAPP_SendHost();
				fnAPP_RecvHost();

				if (m_pDevCmn->TranResult)
				{
					TranCode = TC_TRANSFER;
					transInqCnt += 1;
					int templen = Asc2Int(&HMERecv.Data[2], 2);
					
					ScrNum = 436;
					m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s", &HMERecv.Data[61][16]));
					m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s", &HMERecv.Data[2][2], templen));
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;
					}
					else
					if (strReturn == S_MODIFY)
					{
						if (!--RetryCnt)
						{
							fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
						}
						continue;
					}
				}
				else 
				if (memcmp(HostResp.RejectCode, "26", 2) == 0)			// ��ʾ��������������ҳ��  by zjd
				{
					if (iNGCnt <3)
					{
						iNGCnt ++;
						fnAPP_PMEAcceptPassWord();						// Accept PassWord
						continue;
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
				else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (HostResp.CardEjectFlag)								// Card Eject Flag
					{
						fnAPP_PMDProc();									// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
						// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}	
					else
					{
						if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
							(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
						{
							fnAPP_PMDProc();							// Message Display
							fnAPD_CheckDeviceAction(DEV_MCU);
							fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
							// Card Out
							fnAPD_MaterialOutCheckRetract(DEV_MCU);	
							// Card Out Check & Retract
							fnAPP_CancelProc(T_CANCEL);
						}
						else
						if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
						{
							fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
							MainTranReturnFlag = TRUE;
							return T_OK;								//�����أ����������׻���  by zjd
						}
						else
							fnAPP_CancelProc(T_CANCEL);
					}
				}
				break;
			}
//////////////////////////////////////////////////						//jl20140307
/*			fnAPL_checkOBTRANINQ(Accept.InAccountNum);
		//	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CurrTranINQCanDO[%d]",&CurrTranINQCanDO);
		//	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CurrTranINQCanDO[%d]",&CurrTranINQCanDO[7]);
			if((memcmp(&CurrTranINQCanDO[7],"1",1) == 0))
			{
				if (strFirstAccount.GetLength() >= sizeof(Accept.InAccountNum))
				{
					Accept.InAccountNumSize = sizeof(Accept.InAccountNum);
					memcpy(Accept.InAccountNum, strFirstAccount.GetBuffer(0), sizeof(Accept.InAccountNum));
				}
				else
				{
					Accept.InAccountNumSize = strFirstAccount.GetLength();
					memcpy(Accept.InAccountNum, strFirstAccount.GetBuffer(0), strFirstAccount.GetLength());
				}
					TranCode = TC_CUS_INF_INQ_OTHERBANK;
					TranCode2 = 0;

				
				
				//TranCode = TC_CUS_INF_INQ;
				//TranCode2 = 0;

				if(transInqCnt == 3)
					fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				
				fnAPP_SendHost();
				fnAPP_RecvHost();

				if (m_pDevCmn->TranResult)
				{
					TranCode = TC_TRANSFER;
					transInqCnt += 1;
					int templen = Asc2Int(&HMERecv.Data[2], 2);
					
					ScrNum = 436;
					m_pDevCmn->fnSCR_DisplayString(1, GetSprintf("%s", &HMERecv.Data[61][16]));
					m_pDevCmn->fnSCR_DisplayString(2, GetSprintf("%s", &HMERecv.Data[2][2], templen));
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;
					}
					else
					if (strReturn == S_MODIFY)
					{
						if (!--RetryCnt)
						{
							fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
						}
						continue;
					}
				}
				else 
				if (memcmp(HostResp.RejectCode, "26", 2) == 0)			// ��ʾ��������������ҳ��  by zjd
				{
					if (iNGCnt <3)
					{
						iNGCnt ++;
						fnAPP_PMEAcceptPassWord();						// Accept PassWord
						continue;
					}
					else
						fnAPP_CancelProc(T_CANCEL);
				}
				else												// ��ʾ������Ϣ��������Ӧ����  by zjd
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
						fnAPP_CancelProc(T_CANCEL);
					}
					else
					if (HostResp.CardEjectFlag)								// Card Eject Flag
					{
						fnAPP_PMDProc();									// Message Display
						fnAPD_CheckDeviceAction(DEV_MCU);
						fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
						// Card Out
						fnAPD_MaterialOutCheckRetract(DEV_MCU);				//  Card Out Check & Retract
						fnAPP_CancelProc(T_CANCEL);
					}	
					else
					{
						if ((m_pDevCmn->AtmDefine.McuHandler == MCU_S_TYPE)	||
							(m_pDevCmn->AtmDefine.McuHandler == MCU_D_TYPE))
						{
							fnAPP_PMDProc();							// Message Display
							fnAPD_CheckDeviceAction(DEV_MCU);
							fnAPD_MaterialOut(DEV_MCU, InitInfo.TakeCardTimeOut);
							// Card Out
							fnAPD_MaterialOutCheckRetract(DEV_MCU);	
							// Card Out Check & Retract
							fnAPP_CancelProc(T_CANCEL);
						}
						else
						if (fnAPP_MainMenuContinueCheck() == T_OK)		// 2004.08.23
						{
							fnAPP_PMEAcceptNgGuideConfirm();			// Accept Ng Guide Confirm
							MainTranReturnFlag = TRUE;
							return T_OK;								//�����أ����������׻���  by zjd
						}
						else
							fnAPP_CancelProc(T_CANCEL);
					}
				}
				break;
			}
*/
			else
			{
				ScrNum = 302;											// Input Second Account Screen
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				//�����أ����������׻���  by zjd
				}
				else
				if ((strReturn.GetLength() < 13)	||					// Input Data Check
					(strReturn.GetLength() > sizeof(Accept.InAccountNum)))
					fnAPP_CancelProc(T_INPUTERROR);
				else
				{
					strSecondAccount = strReturn;						// Save Second Account
				}
							
				if (strFirstAccount == strSecondAccount)				// Check Account 
				{
					if (strFirstAccount.GetLength() >= sizeof(Accept.InAccountNum))
					{
						Accept.InAccountNumSize = sizeof(Accept.InAccountNum);
						memcpy(Accept.InAccountNum, strFirstAccount.GetBuffer(0), sizeof(Accept.InAccountNum));
					}
					else
					{
						Accept.InAccountNumSize = strFirstAccount.GetLength();
						memcpy(Accept.InAccountNum, strFirstAccount.GetBuffer(0), strFirstAccount.GetLength());
					}
				/////////////////////////////////////////////////////////////////////////////���ӿ��л�����ѯ
/*******�շ����Ĵ�*******/
			
/*	if ((memcmp(&CurrTranCanDO[7],"1",1) == 0)||
		(memcmp(&CurrTranCanDO[13],"5",1) == 0)||
		(memcmp(&CurrTranCanDO[13],"6",1) == 0))
*/
				
///////////////////////////////////////////////////////////////////////////
					break;
				}

				if (!--RetryCnt)
				{
					fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				}

				ScrNum = 303;											// Confirm Input Account Screen
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
			}
		}
	}
	else
	if (TranCode == TC_PAY)										// Accept Pay ��Ҫ���������͵�ѡ��������Լ�Ԥ��ѯ�Ĵ���  by zjd
	{
        MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PayType[%s]", PayType);
		if (PayType == S_PAY1)                                  //���ӽɷ�����
		{
			ScrNum = 407;
			int j=2;
			int i=0;
			for(i = 0;i < (sizeof(stPayMenuJF)/sizeof(stPayMenuJF[0]));i++ )				// added by liuxl 20111013
			{ 
	
			  if(stPayMenuJF[i].iPayNum > 0)
			  {				  
				m_pDevCmn->fnSCR_DisplayString(j, GetSprintf("%03d %-10s", stPayMenuJF[i].iPayNum,stPayMenuJF[i].szPayName));
				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYNUM[%03d]stPayMenuJF[%d][%10s]", stPayMenuJF[i].iPayNum,i,stPayMenuJF[i].szPayName);
				j++;
			  }
			}
		}   
		else 
		if (PayType == S_PAY2) 
		{
			ScrNum = 419;
			int j=2;
			int i=0;
			for(i = 0;i < (sizeof(stPayMenuCZ)/sizeof(stPayMenuCZ[0]));i++ )				// added by liuxl 20111013
			{
				if(stPayMenuCZ[i].iPayNum > 0)
				{
					m_pDevCmn->fnSCR_DisplayString(j, GetSprintf("%03d %-10s", stPayMenuCZ[i].iPayNum,stPayMenuCZ[i].szPayName));
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PAYNUM[%03d]stPayMenuCZ[%d][%10s]", stPayMenuCZ[i].iPayNum,i,stPayMenuCZ[i].szPayName);
					j++;
				
				}
			}
					
		}
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
		if (strReturn == S_RETURN)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;				
		}
		else
		{
// 			PayNum=Asc2Int(strReturn);

			for(int iNum=0;iNum< (sizeof(stPayJF)/sizeof(stPayJF[0]));iNum++)		//�����صĽɷѴ���ת���ɽɷ�����λԪ added by liuxl 20111013
			{
				if(PayType == S_PAY1)
				{
					if(stPayJF[iNum].iPayNum == Asc2Int(strReturn))
					{
						PayNum = iNum;
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayJF[%d][%03d][%s]",PayNum,stPayJF[PayNum].iPayNum,stPayJF[PayNum].szPayContral);
					}

				}
				else
				if(PayType == S_PAY2)
				{
					if(stPayCZ[iNum].iPayNum == Asc2Int(strReturn))
					{
						PayNum = iNum;
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayCZ[%d][%03d][%s]",iNum,stPayCZ[iNum].iPayNum,stPayCZ[iNum].szPayContral);
// MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "stPayCZ[%d][%03d][%s]",PayNum,stPayCZ[PayNum].iPayNum,stPayCZ[PayNum].szPayContral);
					}
				}
			}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "PayType[%s]PayNum[%d]",PayType,PayNum);
		}
		if(PayType == S_PAY1)									// ����ɷѺ���
		    ScrNum = 317;
		else
		if(PayType == S_PAY2)									// �����ֵ����
			ScrNum = 300;
																//added by liuxl 20111011
		if (memcmp(&m_pProfile->NETWORK.BranchNum[0], "13", 2) == 0 &&
			Asc2Int(strReturn) == 101								&&
			PayType == S_PAY2)
		{
			CString		strFirst("");
			CString		strSecond("");
			RetryCnt = 3;
			while (RetryCnt)
			{
				ScrNum = 300;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else
					strFirst = strReturn;

				ScrNum = 304;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else
					strSecond = strReturn;

				if (strFirst == strSecond)				// Check Account 
				{
					if (strFirst.GetLength() >= sizeof(Accept.InAccountNum))
					{
						Accept.InAccountNumSize = sizeof(Accept.InAccountNum);
						memcpy(Accept.InAccountNum, strFirst.GetBuffer(0), sizeof(Accept.InAccountNum));
					}
					else
					{
						Accept.InAccountNumSize = strFirst.GetLength();
						memcpy(Accept.InAccountNum, strFirst.GetBuffer(0), strFirst.GetLength());
					}

					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.InAccountNum[%s]", Accept.InAccountNum);
					break;
				}

				if (!--RetryCnt)
				{
					fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				}
				
				ScrNum = 309;											// Confirm Input Account Screen
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
			}
		}
		else
//		if (Asc2Int(strReturn) == 1		||
//			Asc2Int(strReturn) == 2		||
//			Asc2Int(strReturn) == 3)
		{
			CString		strFirst("");
			CString		strSecond("");
			RetryCnt = 3;
			while (RetryCnt)
			{
//				ScrNum = 300;
				if(PayType == S_PAY1)									// ����ɷѺ���
					ScrNum = 317;
				else
				if(PayType == S_PAY2)									// �����ֵ����
					ScrNum = 300;

				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else
					strFirst = strReturn;
									
//				ScrNum = 304;
				if(PayType == S_PAY1)									// ����ɷѺ���
					ScrNum = 305;
				else
				if(PayType == S_PAY2)									// �����ֵ����
					ScrNum = 304;
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
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
				if (strReturn == S_RETURN)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else
					strSecond = strReturn;
													
				if (strFirst == strSecond)				// Check Account 
				{
					if (strFirst.GetLength() >= sizeof(Accept.InAccountNum))
					{
						Accept.InAccountNumSize = sizeof(Accept.InAccountNum);
						memcpy(Accept.InAccountNum, strFirst.GetBuffer(0), sizeof(Accept.InAccountNum));
					}
					else
					{
						Accept.InAccountNumSize = strFirst.GetLength();
						memcpy(Accept.InAccountNum, strFirst.GetBuffer(0), strFirst.GetLength());
					}
												
					MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.InAccountNum[%s]", Accept.InAccountNum);
					break;
				}
									
				if (!--RetryCnt)
				{
					fnAPP_CancelProc(T_MSG, TranMsgTbl[23][m_pDevCmn->fnSCR_GetCurrentLangMode()]);
				}
												
				ScrNum = 309;											// Confirm Input Account Screen
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
			}
		}
//		else
//		{
//			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
//			strReturn = m_pDevCmn->fstrSCR_GetKeyString();
//			if (strReturn == S_EXIT)
//				fnAPP_CancelProc(T_EXIT);
//			else 
//			if (strReturn == S_CANCEL)
//				fnAPP_CancelProc(T_CANCEL);
//			else 
//			if (strReturn == S_TIMEOVER)
//				fnAPP_CancelProc(T_TIMEOVER);
//			else 
//			if (strReturn == S_INPUTOVER)
//				fnAPP_CancelProc(T_INPUTOVER);
//			else 
//			if (strReturn == S_RETURN)
//			{
//				MainTranReturnFlag = TRUE;
//				return T_OK;				
//			}
//			else
//			{
//				memcpy(Accept.InAccountNum,strReturn.GetBuffer(0),strReturn.GetLength());
//				Accept.InAccountNumSize = strReturn.GetLength();
//				MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.InAccountNum[%s]", Accept.InAccountNum);
//			}
//		}
		
//		RetryCnt = 3;
//		while (RetryCnt)
//		{
//			RetryCnt --;
//			
//		}
	}
	else
	if (TranCode == TC_AC_REMIT)									
	{
//		ACTranCode = TC_AC_CUSASSU_DETAIL;
//		fnAPP_ACSendHost();
//      fnAPP_ACRecvHost();
		int nDetailCnt;
		double	fTmp = 0.0;


		if (m_pDevCmn->HostLineMode	== HOST_LOCAL_MODE)
		{
// 				for(int i = 0 )
			m_pDevCmn->fnSCR_DisplayString(1,  "1234567890123456789");
			m_pDevCmn->fnSCR_DisplayString(2,  "1111111111111111111");
			m_pDevCmn->fnSCR_DisplayString(3,  "green");
			m_pDevCmn->fnSCR_DisplayString(4,  "4");
		}
		else
		{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "RemitCn[%d]",RemitCn);

            nDetailCnt = RemitCn;          //ת�˻��ļ�¼����

            if(nDetailCnt == 0)
			{
				ScrNum =  704;             //��ʾ�˻��ܼ�ҵ����Լ�����׼�¼                                
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
				if (strReturn == S_CONTINUE)  //�����أ����������׻���  
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
 			}
    			

			for (int j = 0; j < ((nDetailCnt + 2) / 3); j++)
			{

				for (int i = 0; (i < 3) && ((j * 3 + i) < nDetailCnt) ; i++)
				{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostRespRemitAssu[%d].[%s]", j * 3 +i,ACHostRespRemitAssu[j * 3 +i].TranType);	
					m_pDevCmn->fnSCR_DisplayNumeric(i * 5 + 2, ACHostRespRemitAssu[j * 3 +i].ArrayNum);//������Ҫ���޸�  by zjd  //��������
					if((Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 4) ||
						(Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 5 )) 
					{
						 m_pDevCmn->fnSCR_DisplayString(i * 5 + 3, GetSprintf("%6.6s",ACHostRespRemitAssu[j * 3 +i].PayeePostCode));
						 m_pDevCmn->fnSCR_DisplayString(i * 5 + 4, ACHostRespRemitAssu[j * 3 +i].PayeeAdd);
					}
					else
					if((Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 6)  ||
					   (Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 7)  ||
					   (Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 8)  ||
					   (Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 9)  ||
					   (Asc2Int(ACHostRespRemitAssu[j * 3 +i].TranType) == 10))
					{
						 m_pDevCmn->fnSCR_DisplayString(i * 5 + 3, ACHostRespRemitAssu[j * 3 +i].PayeeAccount);
						 m_pDevCmn->fnSCR_DisplayString(i * 5 + 4,ACHostRespRemitAssu[j * 3 +i].PayeeBankName);
					}

					m_pDevCmn->fnSCR_DisplayString(i * 5 + 5, ACHostRespRemitAssu[j * 3 +i].PayeeName);
					m_pDevCmn->fnSCR_DisplayString(i * 5 + 6, ACHostRespRemitAssu[j * 3 +i].TranName);

					m_pDevCmn->fnSCR_DisplayNumeric(17, j+1);
					m_pDevCmn->fnSCR_DisplayNumeric(18, (nDetailCnt + 2) / 3);

									
				}
				
				if (j == 0)													//��һҳ
					m_pDevCmn->fnSCR_DisplayNumeric(19, 0);					//��
				else
					m_pDevCmn->fnSCR_DisplayNumeric(19, 1);

				if ((j + 1) * 3 >= nDetailCnt)								//��һҳ
					m_pDevCmn->fnSCR_DisplayNumeric(20, 0);					//��
				else
					m_pDevCmn->fnSCR_DisplayNumeric(20, 1);

				ScrNum = 425;         //���ӿͻ���ϸ��Ϣ ת�˻���7��   //��ĻӦ���ɳ�����Ʒ�ҳ  by zjd
		
				m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);	// Confirm Input Account Screen
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
				if (strReturn == "UP")
				{
					
					j -= 2;
					continue;
				}
				else 
				if (strReturn == "NEXT")
					continue;
				else 
				if (strReturn == S_RETURN)            //�����أ����������׻���  
				{
					MainTranReturnFlag = TRUE;
					return T_OK;				
				}
				else
				{
					ACHostResp.RecNum = Asc2Int(strReturn) - 1;             //Ҫ���صڼ�����¼Ӧ�ü�����ٷ��� BY HYL
					memcpy(ACHostResp.PayeeAccount,ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount,sizeof(ACHostRespRemitAssu[ACHostResp.RecNum].PayeeAccount));
					break;
				}
			}
		}
	}
	else
	if (TranCode == TC_AC_PAY)
	{

        int nPayDetailCnt;
		m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
        Delay_Msg(1000);
		ACTranCode = TC_AC_OPENTRAN_INQ;           //��ѯ���нɷ���Ŀ�Ŀ�ͨ����
        fnAPP_ACSendHost();
        fnAPP_ACRecvHost();
		if( ACHostResp.ReturnCode !=0)
		{
			MainTranReturnFlag = TRUE;
			return T_OK;		
		}
		

		if( UnAssuPayOpenNum >0 )                  //��ͨ�Ľɷ���Ŀ�ĸ���������Լ���ͷ�Լ����
		{
			nPayDetailCnt = PayAssuCn;
			ScrNum = 34;                                 //�ɷѵ�7��
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	// Confirm Input Account Screen
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
			if (strReturn == S_RETURN)  //�����أ����������׻���  
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
			else
			{
				ACPayType = strReturn;
			}

			if( ACPayType == "ASSUMPSITPAY")                       //Լ���ɷ� 
			{
				TranCode = TC_AC_PAY_ASSU;               
			    
				for (int j = 0; j < ((nPayDetailCnt + 2) / 3); j++)
				{

					for (int i = 0; (i < 3) && ((j * 3 + i) < nPayDetailCnt) ; i++)
					{
						m_pDevCmn->fnSCR_DisplayNumeric(i * 3 + 2, GetSprintf("%03d",ACHostRespPayAssu[j * 3 +i].ArrayNum));//������Ҫ���޸�  by zjd  //��������
						m_pDevCmn->fnSCR_DisplayString(i * 3 + 3, ACHostRespPayAssu[j * 3 +i].TranName);
						m_pDevCmn->fnSCR_DisplayString(i * 3 + 4, ACHostRespPayAssu[j * 3 +i].JHNumber);

						m_pDevCmn->fnSCR_DisplayNumeric(17, j+1);
						m_pDevCmn->fnSCR_DisplayNumeric(18, (nPayDetailCnt + 2) / 3);						
					}
					
					if (j == 0)													//��һҳ
						m_pDevCmn->fnSCR_DisplayNumeric(19, 0);					//��
					else
						m_pDevCmn->fnSCR_DisplayNumeric(19, 1);

					if ((j + 1) * 3 >= nPayDetailCnt)								//��һҳ
						m_pDevCmn->fnSCR_DisplayNumeric(20, 0);					//��
					else
						m_pDevCmn->fnSCR_DisplayNumeric(20, 1);

					ScrNum = 424;         //���ӿͻ���ϸ��Ϣ 
			
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);	// Confirm Input Account Screen
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
					if (strReturn == "UP")
					{
						
						j -= 2;
						continue;
					}
					else 
					if (strReturn == "NEXT")
						continue;
					else 
					if (strReturn == S_RETURN)            //�����أ����������׻���  
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}
					else
					{
						ACHostResp.RecNum = Asc2Int(strReturn) - 1;             //Ҫ���صڼ�����¼Ӧ�ü�����ٷ��� BY HYL
						break;
					}
				}

				memcpy(Accept.InAccountNum,ACHostRespPayAssu[ACHostResp.RecNum].JHNumber,sizeof(ACHostRespPayAssu[ACHostResp.RecNum].JHNumber));
				memcpy(ACHostSend.TranType,ACHostRespPayAssu[ACHostResp.RecNum].TranType,sizeof(ACHostRespPayAssu[ACHostResp.RecNum].TranType));

				m_pDevCmn->fnSCR_DisplayScreen(711);					// Processing Screen
                Delay_Msg(1000);
				ACTranCode = TC_AC_PAY_INQ;
				fnAPP_ACSendHost();
				fnAPP_ACRecvHost();
				if( ACHostResp.ReturnCode !=0)
				{
					MainTranReturnFlag = TRUE;
					return T_OK;		
				}

				if((ACHostRespPayAssu[ACHostResp.RecNum].TranTypeAttri == 0) ||
				   (ACHostRespPayAssu[ACHostResp.RecNum].TranTypeAttri == 1))
 				{

					ScrNum= 221; 						 //����Լ���ɷѽ�� ��9��
					m_pDevCmn->fnSCR_DisplayString(2, ACHostResp.JHAmount);
					m_pDevCmn->fnSCR_DisplayNumeric(3, ACHostRespPayAssu[ACHostResp.RecNum].ArrayNum);
					m_pDevCmn->fnSCR_DisplayString(4, ACHostRespPayAssu[ACHostResp.RecNum].TranName);
					m_pDevCmn->fnSCR_DisplayString(5, ACHostRespPayAssu[ACHostResp.RecNum].JHNumber);

					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_ALL_MODE);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}
					else
					{
						
						memcpy(ACHostSend.TranMoney,strReturn.GetBuffer(0),strReturn.GetLength());
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "ACHostSend.TranMoney[%s]", ACHostSend.TranMoney);	
					}
				}
                        										 
			}
			else
			if( ACPayType == "UNASSUMPSITPAY" )                       //��Լ���ɷ� 
			{
				nPayDetailCnt = UnPayAssuCn;
				TranCode = TC_AC_PAY_NOASSU;
				if( nPayDetailCnt == 0 )
				{
					ScrNum =  704;             //��ʾ�˻��ܼ�ҵ����Լ�����׼�¼                                
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
					if (strReturn == S_CONTINUE)  //�����أ����������׻���  
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}
				}
				else
				{
					for (int j = 0; j < ((nPayDetailCnt + 5) / 6); j++)
					{

						for (int i = 0; (i < 6) && ((j * 6 + i) < nPayDetailCnt) ; i++)
						{
							m_pDevCmn->fnSCR_DisplayString(i * 2 + 2, GetSprintf("%03d",ACHostRespPayUnAssu[j * 6 +i].ArrayNum));//������Ҫ���޸�  by zjd  //��������
							m_pDevCmn->fnSCR_DisplayString(i * 2 + 3, ACHostRespPayUnAssu[j * 6 +i].TranName);

							m_pDevCmn->fnSCR_DisplayNumeric(17, j+1);
							m_pDevCmn->fnSCR_DisplayNumeric(18, (nPayDetailCnt + 5) / 6);						
						}
						
						if (j == 0)													//��һҳ
							m_pDevCmn->fnSCR_DisplayNumeric(19, 0);					//��
						else
							m_pDevCmn->fnSCR_DisplayNumeric(19, 1);

						if ((j + 1) * 6 >= nPayDetailCnt)								//��һҳ
							m_pDevCmn->fnSCR_DisplayNumeric(20, 0);					//��
						else
							m_pDevCmn->fnSCR_DisplayNumeric(20, 1);

						ScrNum = 430;         //���ӷ�Լ���ɷ�
				
						m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);	// Confirm Input Account Screen
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
						if (strReturn == "UP")
						{
							
							j -= 2;
							continue;
						}
						else 
						if (strReturn == "NEXT")
							continue;
						else 
						if (strReturn == S_RETURN)            //�����أ����������׻���  
						{
							MainTranReturnFlag = TRUE;
							return T_OK;				
						}
						else
						{
							ACHostResp.RecNum = Asc2Int(strReturn) - 1;             //Ҫ���صڼ�����¼Ӧ�ü�����ٷ��� BY HYL
							break;
						}
					} 	
					
					ScrNum = 428;                            //�����Լ���ɷѺ��� ��9��
					m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_AMOUNT_MODE);
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
					if (strReturn == S_RETURN)
					{
						MainTranReturnFlag = TRUE;
						return T_OK;				
					}
					else
					{
						memset(Accept.InAccountNum,0,sizeof(Accept.InAccountNum));
						memcpy(Accept.InAccountNum,strReturn.GetBuffer(0),strReturn.GetLength());
						MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Accept.InAccountNum[%s]", Accept.InAccountNum);

					}
				}

				

			}

		}
		else
		{
			ScrNum =  427;             //�ɷѵ�7������ʾ�ɷ�ҵ��δ��ͨ                                
			m_pDevCmn->fnSCR_DisplayScreen(ScrNum, InitInfo.KeyInTimeOut, PIN_MENU_MODE);	// Confirm Input Account Screen
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
			if (strReturn == S_CONTINUE)  //�����أ����������׻���  
			{
				MainTranReturnFlag = TRUE;
				return T_OK;				
			}
		}
		

	}
	return T_OK;
}
