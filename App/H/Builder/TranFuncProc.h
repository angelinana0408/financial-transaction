////////////////////////////////////////////////////////////////////////////////
// File Name   : TranFuncProc.h
// Description : Funtion Declaration
// Creator     : SE
// History    : 1. 2004-12-22  Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranFuncProc_H_INCLUDED_)
#define _TranFuncProc_H_INCLUDED_

//////////////////////////////////////////////////////////////////////////////
// Attributes
public:

//////////////////////////////////////////////////////////////////////////////
// Operations
public:

	int	fnAPPF_TransactionInitialize();                                             // TransactionInitialize
	int	fnAPPF_TransactionMenuProcedure();                                          // TransactionMenuProcedure
	int	fnAPPF_TransactionGetTransaction();                                         // TransactionGetTransaction
	int	fnAPPF_TransactionGetMenuDoing();                                           // TransactionGetMenuDoing

	int	fnAPPF_SetProcedureCount1();                                                // SetProcedureCount1
	int	fnAPPF_SetProcedureCount3();                                                // SetProcedureCount3
	int	fnAPPF_SetProcedureCount5();                                                // SetProcedureCount5
	int	fnAPPF_SetProcedureCount6();                                                // SetProcedureCount6
	int	fnAPPF_SetProcedureCount9();                                                // SetProcedureCount9
	int	fnAPPF_SendHost();                                                          // SendHost
	int	fnAPPF_RecvHost();                                                          // RecvHost
	int	fnAPPF_GetHostResult();                                                     // GetHostResult

	int	fnAPPF_TransactionWithdrawInitialize();                                     // TransactionWithdrawInitialize
	int	fnAPPF_TransactionWithdrawInput();                                          // TransactionWithdrawInput
	int	fnAPPF_TransactionWithdrawOutput();                                         // TransactionWithdrawOutput
	int	fnAPPF_TransactionWithdrawCheckDeviceAction();                              // TransactionWithdrawCheckDeviceAction
	int	fnAPPF_TransactionWithdrawCheckMenuContinue();                              // TransactionWithdrawCheckMenuContinue

	int	fnAPPF_TransactionInquiryInitialize();                                      // TransactionInquiryInitialize
	int	fnAPPF_TransactionInquiryInput();                                           // TransactionInquiryInput
	int	fnAPPF_TransactionInquiryOutput();                                          // TransactionInquiryOutput
	int	fnAPPF_TransactionInquiryCheckDeviceAction();                               // TransactionInquiryCheckDeviceAction
	int	fnAPPF_TransactionInquiryCheckMenuContinue();                               // TransactionInquiryCheckMenuContinue

	int	fnAPPF_TransactionTransferInitialize();                                     // TransactionTransferInitialize
	int	fnAPPF_TransactionTransferInput();                                          // TransactionTransferInput
	int	fnAPPF_TransactionTransferOutput();                                         // TransactionTransferOutput
	int	fnAPPF_TransactionTransferCheckDeviceAction();                              // TransactionTransferCheckDeviceAction
	int	fnAPPF_TransactionTransferCheckMenuContinue();                              // TransactionTransferCheckMenuContinue

	int	fnAPPF_TransactionDepositInitialize();                                      // TransactionDepositInitialize
	int	fnAPPF_TransactionDepositInput();                                           // TransactionDepositInput
	int	fnAPPF_TransactionDepositOutput();                                          // TransactionDepositOutput
	int	fnAPPF_TransactionDepositCheckDeviceAction();                               // TransactionDepositCheckDeviceAction
	int	fnAPPF_TransactionDepositCheckMenuContinue();                               // TransactionDepositCheckMenuContinue

	int	fnAPPF_TransactionPassbookInitialize();                                     // TransactionPassbookInitialize
	int	fnAPPF_TransactionPassbookInput();                                          // TransactionPassbookInput
	int	fnAPPF_TransactionPassbookOutput();                                         // TransactionPassbookOutput
	int	fnAPPF_TransactionPassbookCheckDeviceAction();                              // TransactionPassbookCheckDeviceAction
	int	fnAPPF_TransactionPassbookCheckMenuContinue();                              // TransactionPassbookCheckMenuContinue

	int	fnAPPF_TransactionIcInitialize();                                           // TransactionIcInitialize
	int	fnAPPF_TransactionIcInput();                                                // TransactionIcInput
	int	fnAPPF_TransactionIcOutput();                                               // TransactionIcOutput
	int	fnAPPF_TransactionIcCheckDeviceAction();                                    // TransactionIcCheckDeviceAction
	int	fnAPPF_TransactionIcCheckMenuContinue();                                    // TransactionIcCheckMenuContinue



/////////////////////////////////////////////////////////////////////////////
#endif
