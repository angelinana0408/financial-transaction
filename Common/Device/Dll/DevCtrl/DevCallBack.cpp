/////////////////////////////////////////////////////////////////////////////
//	DevCallBack.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"

#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CALLBACK
/////////////////////////////////////////////////////////////////////////////
	CDevCmn*	m_pDevCmn;										// 2010.10.20

/////////////////////////////////////////////////////////////////////////////
// Set Owner
/////////////////////////////////////////////////////////////////////////////
void fnAPC_SetOwner(CDevCmn* pDevice)
{
	m_pDevCmn = pDevice;

	return;
}

/////////////////////////////////////////////////////////////////////////////
//	EMV CALLBACK
/////////////////////////////////////////////////////////////////////////////
// Ic Send Apdu
int	CALLBACK fnAPC_IcSendApdu(BYTE btPrtCode, BYTE* btpApdu, UINT nApduLen, BYTE* btpResp, int* npRespLen)
{
	int		nReturn = FALSE;

	nReturn = m_pDevCmn->fnAPL_IcSendApdu(m_pDevCmn->McuProtocolCode, btpApdu, nApduLen, btpResp, npRespLen);
	if (nReturn)												// Ok
		nReturn = EMV_RSLT_OK;
	else														// Error
		nReturn = EMV_RSLT_ERROR;

	return nReturn;
}

/////////////////////////////////////////////////////////////////////////////
//	PBOC CALLBACK
/////////////////////////////////////////////////////////////////////////////
// Pboc Account Type Select
int fnAPC_PbcAccTypeSelect(BYTE* btpAccountType)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcAccTypeSelect()");
	int		nReturn = FALSE;
	
	nReturn = EMV_OK;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcAccTypeSelect(btpAccountType[%d]):return(%d)", *btpAccountType, nReturn);
	return nReturn;
}

// Pboc Input Amount
int	fnAPC_PbcInputAmt(DWORD* dwpAuthAmt, DWORD* dwpCashBackAmt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcInputAmt()");
	int		nReturn = FALSE;
	
	*dwpAuthAmt = m_pDevCmn->Pbc.TranAmount;

	nReturn = EMV_OK;
	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcInputAmt(dwpAuthAmt[%d]):return(%d)", *dwpAuthAmt, nReturn);
	return nReturn;
}

// Pboc Get Date Time
int	fnAPC_PbcGetDateTime(BYTE* btpDateTime)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetDateTime()");
	int		nReturn = FALSE;
	BYTE	btTempDate[256];
	BYTE	btTempTime[256];
	BYTE	btTempDataTime[256];

	memset(btTempDate, 0, sizeof(btTempDate));
	memset(btTempTime, 0, sizeof(btTempTime));
	memset(btTempDataTime, 0, sizeof(btTempDataTime));

	GetDateTime(btTempDate, btTempTime);
	memcpy(btTempDataTime, &btTempDate[2], 6);
	memcpy(&btTempDataTime[6], btTempTime, 6);
	MakePack(btTempDataTime, btpDateTime, 12);

	nReturn = EMV_OK;

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetDateTime(btpDateTime):return", btpDateTime, 6);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetDateTime():return(%d)", nReturn);
	return nReturn;
}

// Pboc Get Holder Password
int	fnAPC_PbcGetHolderPwd(int nTryFlag, int nRemainCnt, BYTE* nPin)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetHolderPwd(nTryFlag[%d], nRemainCnt[%d])", nTryFlag, nRemainCnt);
	int		nReturn = FALSE;

	nReturn = EMV_OK;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetHolderPwd():return(%d)", nReturn);
	return nReturn;
}

// Pboc Get Unknown TLV
int	fnAPC_PbcGetUnknownTLV(WORD wTag, BYTE* btpData, int nDataLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetUnknownTLV(wTag[%x])", wTag);
	int		nReturn = FALSE;

	nReturn = ERR_NODATA;										// No Specified Special TLV Data

HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetUnknownTLV(btpData):return", btpData, nDataLen);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcGetUnknownTLV():return(%d)", nReturn);
	return nReturn;
}

// Pboc Io Ctrl
void fnAPC_PbcIoCtrl(BYTE btIoName, BYTE* btpIoValue)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcIoCtrl(btIoName[%x])", btIoName);

	if (btIoName == EMV_GET_POSENTRYMODE)
	{
		*btpIoValue = m_pDevCmn->Pbc.POSEntryCode;
	}
	else
		*btpIoValue = 0x00;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcIoCtrl():return()");
	return;														// Not Support Return Directly
}

// Pboc Advice Proc
void fnAPC_PbcAdviceProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcAdviceProc()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcAdviceProc():return()");
	return;														// Not Support Return Directly
}

// Pboc Verify Cert
int fnAPC_PbcVerifyCert()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcVerifyCert()");
	int		nReturn = FALSE;
	
	nReturn = EMV_OK;											// Not Support Return Directly

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcVerifyCert():return(%d)", nReturn);
	return nReturn;
}

// Pboc Refer Proc
int	fnAPC_PbcReferProc()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcReferProc()");
	int		nReturn = FALSE;
	
	nReturn = REFER_APPROVE;									// Not Support Return Directly

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcReferProc():return(%d)", nReturn);
	return nReturn;
}

// Pboc Display Verify Pin Result
void fnAPC_PbcDispVerifyPinResult(BYTE btResult)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcDispVerifyPinResult()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcDispVerifyPinResult():return()");
	return;														// Not Support Return Directly
}

// Pboc Read Ifd SN
void fnAPC_PbcReadIfdSN(BYTE* btpIfdSn)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcReadIfdSN()");
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcReadIfdSN():return()");
	return;														// Not Support Return Directly
}

// Pboc Ic Send Recv Apdu
int	fnAPC_PbcIcSendRecvApdu(BYTE btSlot, ICC_APDU_SEND* stpApduSend, ICC_APDU_RESP* stpApduResp)
{
	int		nReturn = FALSE;
	int		nTempApduSendLen = 0;
	int		nTempApduRespLen = 0;
	BYTE	btTempApduSend[1024];
	BYTE	btTempApduResp[1024];
	
	memset(btTempApduSend, 0, sizeof(btTempApduSend));
	memset(btTempApduResp, 0, sizeof(btTempApduResp));
	
	memcpy(&btTempApduSend[nTempApduSendLen], stpApduSend->Command, 4);
	nTempApduSendLen = 4;
	
	if (stpApduSend->Lc > 0)
	{
		btTempApduSend[nTempApduSendLen++] = (BYTE)stpApduSend->Lc;
		memcpy(&btTempApduSend[nTempApduSendLen], stpApduSend->DataIn, (BYTE)stpApduSend->Lc);
		nTempApduSendLen += (BYTE)stpApduSend->Lc;
	}
	
	if (stpApduSend->Le >= 256)
		btTempApduSend[nTempApduSendLen++] = 0;
	else
	{
		if (stpApduSend->Le > 0)
			btTempApduSend[nTempApduSendLen++] = (BYTE)stpApduSend->Le;
	}
		
	nReturn = m_pDevCmn->fnAPL_IcSendApdu(m_pDevCmn->McuProtocolCode, btTempApduSend, nTempApduSendLen, btTempApduResp, &nTempApduRespLen);
	if (nReturn)												// Ok
	{
		if (nTempApduRespLen >= 2)
		{
			stpApduResp->LenOut = nTempApduRespLen - 2;
			memcpy(stpApduResp->DataOut, btTempApduResp, stpApduResp->LenOut);
			stpApduResp->SWA = btTempApduResp[nTempApduRespLen - 2];
			stpApduResp->SWB = btTempApduResp[nTempApduRespLen - 1];
			nReturn = EMV_OK;
		}
		else
		{
			stpApduResp->LenOut = 0;
			nReturn = ERR_ICCCMD;
		}
	}
	else														// Error
	{
		stpApduResp->LenOut = 0;
		nReturn = ERR_ICCCMD;
	}

	return nReturn;
}

// Pboc Regist Function
int	fnAPC_PbcRegistFunc()
{	
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcRegistFunc()");
	int					nReturn = FALSE;
	PBCst_FUNCPTR_PARAM stpFuncPtr;

	stpFuncPtr.PAccountTypeSel = fnAPC_PbcAccTypeSelect;
	stpFuncPtr.pInputAmt = fnAPC_PbcInputAmt;
	stpFuncPtr.pGetDateTime = fnAPC_PbcGetDateTime;
	stpFuncPtr.pGetHolderPwd = fnAPC_PbcGetHolderPwd;
	stpFuncPtr.pGetUnknowTLV = fnAPC_PbcGetUnknownTLV;
	stpFuncPtr.pIoCtrl = fnAPC_PbcIoCtrl;
	stpFuncPtr.pAdviceProc = fnAPC_PbcAdviceProc;
	stpFuncPtr.pVerifyCert = fnAPC_PbcVerifyCert;
	stpFuncPtr.pReferProc = fnAPC_PbcReferProc;
	stpFuncPtr.pDispVerifyPinResult = fnAPC_PbcDispVerifyPinResult;
	stpFuncPtr.pReadIfdSN = fnAPC_PbcReadIfdSN;
	stpFuncPtr.pIccCommand = fnAPC_PbcIcSendRecvApdu;

	EmvLib_InitCallBackFunc(&stpFuncPtr);
	
	nReturn = TRUE;

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPC_PbcRegistFunc():return()");
	return nReturn;
}

