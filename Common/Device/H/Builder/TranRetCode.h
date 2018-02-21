////////////////////////////////////////////////////////////////////////////////
// File Name   : TranRetCode.h
// Description : ReturnValue Definition
// Creator     : SE
//  History    : 1. 2004-12-22  Ver 01.01.01  AUTO - New Version
//                  
//
// Copyright (c) 2004, Nautilus Hyosung Inc
////////////////////////////////////////////////////////////////////////////////
#if !defined(_TranRetCode_H_INCLUDED_)
#define _TranRetCode_H_INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// Return Code Definition
//////////////////////////////////////////////////////////////////////////////

#define RET_ABNORMAL_STEP                                  0                   // AbnormalStep
#define RET_NEXT_STEP                                      1                   // NextStep
#define RET_BEFORE_STEP                                    2                   // BeforeStep
#define RET_LOOP_STEP                                      3                   // LoopStep
#define RET_RETURN_STEP                                    4                   // ReturnStep

#define RET_TRUE                                           10                  // ReturnTrue
#define RET_FALSE                                          11                  // ReturnFalse

#define RET_WITHDRAW                                       20                  // Withdraw
#define RET_INQUIRY                                        21                  // Inquiry
#define RET_TRANSFER                                       22                  // Transfer
#define RET_DEPOSIT                                        23                  // Deposit
#define RET_PASSBOOK                                       24                  // Passbook
#define RET_IC                                             25                  // Ic

#define RET_HOST_OK                                        30                  // HostOk
#define RET_HOST_NG                                        31                  // HostNg



/////////////////////////////////////////////////////////////////////////////
#endif
