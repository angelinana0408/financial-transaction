////////////////////////////////////////////////////////////////////////////////
// File Name   : TranCtrlProc.h
// Description : ControlFunction Declaration
// Creator     : SE
// History    : 1. 2004-12-22  Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCtrlProc_H_INCLUDED_)
#define _TranCtrlProc_H_INCLUDED_

//////////////////////////////////////////////////////////////////////////////
// Attributes
public:

//////////////////////////////////////////////////////////////////////////////
// Operations
public:

	int	fnAPPC_TranBeginProc();                                                     // TransactionBegin
	int	fnAPPC_TranEndProc();                                                       // TransactionEnd
	int	fnAPPC_TranWithProc();                                                      // Withdraw
	int	fnAPPC_TranInqProc();                                                       // Inquiry
	int	fnAPPC_TranTransProc();                                                     // Transfer
	int	fnAPPC_TranDepProc();                                                       // Deposit
	int	fnAPPC_TranPbProc();                                                        // Passbook
	int	fnAPPC_TranIcProc();                                                        // Ic




/////////////////////////////////////////////////////////////////////////////
#endif
