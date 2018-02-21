////////////////////////////////////////////////////////////////////////////////
// File Name   : TranTranProc.cpp
// Description : Transaction
// Creator     : SE
// History     : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "TranCtrlProc_STA.h"
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
// File Name   : fnAPPM_TranTranProc
// Description : Transaction
// Parameter   : Nothing
// Return Value: T_OK     - Success
//                T_ERROR  - Failure
//Notice Items : Nothing
// History     : 1. 2004-12-22  CUP Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
int CTranCmn::fnAPPM_TranTranProc()
{
	// Start Value Setting
	int nNextState = TRANBEGINPROC_STA;
	
	while (1)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__,
			"fnAPPM_TranTranProc:m_nRetCode[%d], nNextState[%d]", m_nRetCode, nNextState);

		// According to nNextState-Value, branch next step
		switch (nNextState)
		{
			// TransactionBegin
			case TRANBEGINPROC_STA:
				fnAPPC_TranBeginProc();
				switch(m_nRetCode)
				{
					case RET_WITHDRAW:                               // Withdraw
						nNextState = TRANWITHPROC_STA;
						break;
					case RET_INQUIRY:                                // Inquiry
						nNextState = TRANINQPROC_STA;
						break;
					case RET_TRANSFER:                               // Transfer
						nNextState = TRANTRANSPROC_STA;
						break;
					case RET_DEPOSIT:                                // Deposit
						nNextState = TRANDEPPROC_STA;
						break;
					case RET_PASSBOOK:                               // Passbook
						nNextState = TRANPBPROC_STA;
						break;
					case RET_IC:                                     // Ic
						nNextState = TRANICPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Withdraw
			case TRANWITHPROC_STA:
				fnAPPC_TranWithProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANWITHPROC_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						nNextState = TRANENDPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Inquiry
			case TRANINQPROC_STA:
				fnAPPC_TranInqProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANINQPROC_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						nNextState = TRANENDPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Transfer
			case TRANTRANSPROC_STA:
				fnAPPC_TranTransProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANTRANSPROC_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						nNextState = TRANENDPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Deposit
			case TRANDEPPROC_STA:
				fnAPPC_TranDepProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANDEPPROC_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						nNextState = TRANENDPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Passbook
			case TRANPBPROC_STA:
				fnAPPC_TranPbProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANPBPROC_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						nNextState = TRANENDPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// Ic
			case TRANICPROC_STA:
				fnAPPC_TranIcProc();
				switch(m_nRetCode)
				{
					case RET_NEXT_STEP:                              // NextStep
						nNextState = TRANENDPROC_STA;
						break;
					case RET_LOOP_STEP:                              // LoopStep
						nNextState = TRANICPROC_STA;
						break;
					case RET_RETURN_STEP:                            // ReturnStep
						nNextState = TRANENDPROC_STA;
						break;
					default:                                         // abnormal end
						fnAPP_CancelProc(T_PROGRAM);
				}
				break;

			// TransactionEnd
			case TRANENDPROC_STA:
				fnAPPC_TranEndProc();
				switch(m_nRetCode)
				{
					case RET_TRUE:                                   // ReturnTrue
						nNextState = TRANBEGINPROC_STA;
						break;
					case RET_FALSE:                                  // ReturnFalse
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
