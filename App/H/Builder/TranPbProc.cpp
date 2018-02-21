////////////////////////////////////////////////////////////////////////////////
// File Name   : TranPbProc.cpp
// Description : Passbook
// Creator     : SE
// History     : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TranFuncProc_STA.h"
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
// File Name   : fnAPPC_TranPbProc
// Description : Passbook
// Parameter   : Nothing
// Return Value: T_OK     - Success
//                T_ERROR  - Failure
//Notice Items : Nothing
// History     : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPC_TranPbProc()
{
	// Start Value Setting
	int nNextState = TRANSACTIONPASSBOOKINITIALIZE_STA;
	
	while (1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,
			"fnAPPC_TranPbProc:m_nRetCode[%d], nNextState[%d]", m_nRetCode, nNextState);

		// According to nNextState-Value, branch next step
		switch (nNextState)
		{
			// TransactionPassbookInitialize
			case TRANSACTIONPASSBOOKINITIALIZE_STA:
				fnAPPF_TransactionPassbookInitialize();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = SETPROCEDURECOUNT1_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// SetProcedureCount1
			case SETPROCEDURECOUNT1_STA:
				fnAPPF_SetProcedureCount1();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONPASSBOOKINPUT_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionPassbookInput
			case TRANSACTIONPASSBOOKINPUT_STA:
				fnAPPF_TransactionPassbookInput();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = SETPROCEDURECOUNT3_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						return T_OK;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// SetProcedureCount3
			case SETPROCEDURECOUNT3_STA:
				fnAPPF_SetProcedureCount3();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = SENDHOST_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// SendHost
			case SENDHOST_STA:
				fnAPPF_SendHost();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = RECVHOST_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// RecvHost
			case RECVHOST_STA:
				fnAPPF_RecvHost();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = GETHOSTRESULT_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// GetHostResult
			case GETHOSTRESULT_STA:
				fnAPPF_GetHostResult();
				switch(m_nRetCode)
				{
					case RET_HOST_OK:                                // HostOk
						nNextState = SETPROCEDURECOUNT5_STA;
						break;
					case RET_HOST_NG:                                // HostNg
						nNextState = SETPROCEDURECOUNT9_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// SetProcedureCount5
			case SETPROCEDURECOUNT5_STA:
				fnAPPF_SetProcedureCount5();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONPASSBOOKOUTPUT_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// SetProcedureCount9
			case SETPROCEDURECOUNT9_STA:
				fnAPPF_SetProcedureCount9();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONPASSBOOKOUTPUT_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionPassbookOutput
			case TRANSACTIONPASSBOOKOUTPUT_STA:
				fnAPPF_TransactionPassbookOutput();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONPASSBOOKCHECKDEVICEACTION_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionPassbookCheckDeviceAction
			case TRANSACTIONPASSBOOKCHECKDEVICEACTION_STA:
				fnAPPF_TransactionPassbookCheckDeviceAction();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONPASSBOOKCHECKMENUCONTINUE_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionPassbookCheckMenuContinue
			case TRANSACTIONPASSBOOKCHECKMENUCONTINUE_STA:
				fnAPPF_TransactionPassbookCheckMenuContinue();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						return T_OK;
					case RET_LOOP_STEP:                              // LoopStep
						return T_OK;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			default:                                         // abnormal end
				fnAPP_CancelProc(T_PROGRAM);

		}     // End switch

	}     // End while

	return T_OK;

}
