////////////////////////////////////////////////////////////////////////////////
// File Name   : TranFuncProc.cpp
// Description : Funtion Declaration
// Creator     : SE
// History     : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
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



////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionInitialize()
// Description  : TransactionInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInitialize()
{
	// To Do
	fnAPP_TranInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionMenuProcedure()
// Description  : TransactionMenuProcedure
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionMenuProcedure()
{
	// To Do
	fnAPP_TranMenuProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionGetTransaction()
// Description  : TransactionGetTransaction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionGetTransaction()
{
	// To Do
	fnAPP_TranGetTranProc();

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionGetMenuDoing()
// Description  : TransactionGetMenuDoing
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionGetMenuDoing()
{
	// To Do
	fnAPP_TranGetMenuDoingProc();
	m_nRetCode = (MenuDoingFlag) ? RET_TRUE : RET_FALSE;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_SetProcedureCount1()
// Description  : SetProcedureCount1
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount1()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('1');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_SetProcedureCount3()
// Description  : SetProcedureCount3
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount3()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('3');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_SetProcedureCount5()
// Description  : SetProcedureCount5
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount5()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('5');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_SetProcedureCount6()
// Description  : SetProcedureCount6
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount6()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('6');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_SetProcedureCount9()
// Description  : SetProcedureCount9
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SetProcedureCount9()
{
	// To Do
	m_pDevCmn->fnAPL_SetProcCount('9');
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_SendHost()
// Description  : SendHost
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_SendHost()
{
	// To Do
	fnAPP_SendHost();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_RecvHost()
// Description  : RecvHost
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_RecvHost()
{
	// To Do
	fnAPP_RecvHost();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_GetHostResult()
// Description  : GetHostResult
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_GetHostResult()
{
	// To Do
	m_nRetCode = (m_pDevCmn->TranResult) ? RET_HOST_OK : RET_HOST_NG;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionWithdrawInitialize()
// Description  : TransactionWithdrawInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawInitialize()
{
	// To Do
	fnAPP_TranWithInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionWithdrawInput()
// Description  : TransactionWithdrawInput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawInput()
{
	// To Do
	fnAPP_TranWithInputProc();
	m_nRetCode = (MainTranReturnFlag) ? RET_RETURN_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionWithdrawOutput()
// Description  : TransactionWithdrawOutput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawOutput()
{
	// To Do
	fnAPP_TranWithOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionWithdrawCheckDeviceAction()
// Description  : TransactionWithdrawCheckDeviceAction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawCheckDeviceAction()
{
	// To Do
	fnAPP_TranWithCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionWithdrawCheckMenuContinue()
// Description  : TransactionWithdrawCheckMenuContinue
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionWithdrawCheckMenuContinue()
{
	// To Do
	fnAPP_TranWithCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionInquiryInitialize()
// Description  : TransactionInquiryInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryInitialize()
{
	// To Do
	fnAPP_TranInqInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionInquiryInput()
// Description  : TransactionInquiryInput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryInput()
{
	// To Do
	fnAPP_TranInqInputProc();
	m_nRetCode = (MainTranReturnFlag) ? RET_RETURN_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionInquiryOutput()
// Description  : TransactionInquiryOutput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryOutput()
{
	// To Do
	fnAPP_TranInqOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionInquiryCheckDeviceAction()
// Description  : TransactionInquiryCheckDeviceAction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryCheckDeviceAction()
{
	// To Do
	fnAPP_TranInqCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionInquiryCheckMenuContinue()
// Description  : TransactionInquiryCheckMenuContinue
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionInquiryCheckMenuContinue()
{
	// To Do
	fnAPP_TranInqCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionTransferInitialize()
// Description  : TransactionTransferInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferInitialize()
{
	// To Do
	fnAPP_TranTransInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionTransferInput()
// Description  : TransactionTransferInput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferInput()
{
	// To Do
	fnAPP_TranTransInputProc();
	m_nRetCode = (MainTranReturnFlag) ? RET_RETURN_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionTransferOutput()
// Description  : TransactionTransferOutput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferOutput()
{
	// To Do
	fnAPP_TranTransOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionTransferCheckDeviceAction()
// Description  : TransactionTransferCheckDeviceAction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferCheckDeviceAction()
{
	// To Do
	fnAPP_TranTransCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionTransferCheckMenuContinue()
// Description  : TransactionTransferCheckMenuContinue
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionTransferCheckMenuContinue()
{
	// To Do
	fnAPP_TranTransCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionDepositInitialize()
// Description  : TransactionDepositInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositInitialize()
{
	// To Do
	fnAPP_TranDepInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionDepositInput()
// Description  : TransactionDepositInput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositInput()
{
	// To Do
	fnAPP_TranDepInputProc();
	m_nRetCode = (MainTranReturnFlag) ? RET_RETURN_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionDepositOutput()
// Description  : TransactionDepositOutput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositOutput()
{
	// To Do
	fnAPP_TranDepOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionDepositCheckDeviceAction()
// Description  : TransactionDepositCheckDeviceAction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositCheckDeviceAction()
{
	// To Do
	fnAPP_TranDepCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionDepositCheckMenuContinue()
// Description  : TransactionDepositCheckMenuContinue
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionDepositCheckMenuContinue()
{
	// To Do
	fnAPP_TranDepCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionPassbookInitialize()
// Description  : TransactionPassbookInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookInitialize()
{
	// To Do
	fnAPP_TranPbInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionPassbookInput()
// Description  : TransactionPassbookInput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookInput()
{
	// To Do
	fnAPP_TranPbInputProc();
	m_nRetCode = (MainTranReturnFlag) ? RET_RETURN_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionPassbookOutput()
// Description  : TransactionPassbookOutput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookOutput()
{
	// To Do
	fnAPP_TranPbOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionPassbookCheckDeviceAction()
// Description  : TransactionPassbookCheckDeviceAction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookCheckDeviceAction()
{
	// To Do
	fnAPP_TranPbCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionPassbookCheckMenuContinue()
// Description  : TransactionPassbookCheckMenuContinue
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionPassbookCheckMenuContinue()
{
	// To Do
	fnAPP_TranPbCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionIcInitialize()
// Description  : TransactionIcInitialize
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionIcInitialize()
{
	// To Do
	fnAPP_TranIcInitProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionIcInput()
// Description  : TransactionIcInput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionIcInput()
{
	// To Do
	fnAPP_TranIcInputProc();
	m_nRetCode = (MainTranReturnFlag) ? RET_RETURN_STEP : RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionIcOutput()
// Description  : TransactionIcOutput
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionIcOutput()
{
	// To Do
	fnAPP_TranIcOutputProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionIcCheckDeviceAction()
// Description  : TransactionIcCheckDeviceAction
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionIcCheckDeviceAction()
{
	// To Do
	fnAPP_TranIcCheckDeviceActionProc();
	m_nRetCode = RET_NEXT_STEP;

	return T_OK;
}

////////////////////////////////////////////////////////////////////////////////
// File Name    : fnAPPF_TransactionIcCheckMenuContinue()
// Description  : TransactionIcCheckMenuContinue
// Parameter    : Nothing
// Return Value : T_OK  - Success
//                T_NG  - Failure
// Notice Items : Nothing
// History      : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                   
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPF_TransactionIcCheckMenuContinue()
{
	// To Do
	fnAPP_TranIcCheckMenuContinueProc();
	m_nRetCode = (MainTranDoingFlag) ? RET_LOOP_STEP : RET_NEXT_STEP;

	return T_OK;
}
