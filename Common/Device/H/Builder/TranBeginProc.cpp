////////////////////////////////////////////////////////////////////////////////
// File Name   : TranBeginProc.cpp
// Description : TransactionBegin
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
// File Name   : fnAPPC_TranBeginProc
// Description : TransactionBegin
// Parameter   : Nothing
// Return Value: T_OK     - Success
//                T_ERROR  - Failure
//Notice Items : Nothing
// History     : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPC_TranBeginProc()
{
	// Start Value Setting
	int nNextState = TRANSACTIONINITIALIZE_STA;
	
	while (1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,
			"fnAPPC_TranBeginProc:m_nRetCode[%d], nNextState[%d]", m_nRetCode, nNextState);

		// According to nNextState-Value, branch next step
		switch (nNextState)
		{
			// TransactionInitialize
			case TRANSACTIONINITIALIZE_STA:
				fnAPPF_TransactionInitialize();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONMENUPROCEDURE_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionMenuProcedure
			case TRANSACTIONMENUPROCEDURE_STA:
				fnAPPF_TransactionMenuProcedure();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANSACTIONGETTRANSACTION_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionGetTransaction
			case TRANSACTIONGETTRANSACTION_STA:
				fnAPPF_TransactionGetTransaction();
				switch(m_nRetCode)
				{
					case RET_WITHDRAW:                               // Withdraw
						return T_OK;
					case RET_INQUIRY:                                // Inquiry
						return T_OK;
					case RET_TRANSFER:                               // Transfer
						return T_OK;
					case RET_DEPOSIT:                                // Deposit
						return T_OK;
					case RET_PASSBOOK:                               // Passbook
						return T_OK;
					case RET_IC:                                     // Ic
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
