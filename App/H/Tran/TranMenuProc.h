/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranMenuProc_H_INCLUDED_)
#define _TranMenuProc_H_INCLUDED_

#define itemof(item)	(sizeof(item) / sizeof(item[0]))

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Menu Table
/////////////////////////////////////////////////////////////////////////////
	MENUHeader MenuTbl[itemof(MenuTblConst)];
										// Menu Table 
	MENUHeader MenuTblTemp[itemof(MenuTblConst)];			
										// Menu Table Temp
	MENUHeader MenuTblSave[itemof(MenuTblConst)];			
										// Menu Table Save

/////////////////////////////////////////////////////////////////////////////
//	Transaction Status Variable
/////////////////////////////////////////////////////////////////////////////
	int		TranAvail;					// Transaction Avail
	int		WithAvail;					// Withdraw Avail
	int		DepAvail;					// Deposit Avail
	int		PbmAvail;					// Passbook Avail

	char	CurrTranCanDO[16];			// 1:inq,2:cwd,3:trf,4:pin,5:dep,6:remit,7:pay；当前磁卡可做交易功能数组；
	char	CurrTranINQCanDO[16];

	int		LocalCardFlag;				// 当前磁卡为本地（本省）卡的标志；1:本省,2:外省,3:它行,4:贷记
	int		LowAmountFlag;				// 当前为小额交易流程
	int		CurrTypes;					// 所支持的账户类型集合
	int		CurrTypeFlag;				// 所选账户类型 1：借记 2：贷记 4：小额
	int		POSTICType;					// 邮储ic卡的类型1：I型 2：II型
	char	TempTranCanDO[16];

/////////////////////////////////////////////////////////////////////////////
//	Menu Variable
/////////////////////////////////////////////////////////////////////////////
	int		MenuIndex;					// Menu Index
	int		MenuIndexSave;				// Menu Index Save
	int		MenuIndexSub;				// Menu Index Sub
	int		MenuIndexNext;				// Menu Index Next
	int		MenuAvail;					// Menu Avail
	int		MenuNcTran;					// Menu No Card Transaction
	int		MenuBtnCnt;					// Menu Button Count
	int		MenuBtnIndex;				// Menu Button Index
	int		MenuBeforeInputFlag;		// Menu Before Input Flag
	int		MenuDoingFlag;				// Menu Doing Flag
	int		MenuContinueFlag;			// Menu Continue Flag

	int		TranProc;					// Transaction Procedure
	int		TranCode;					// Transaction Code
	int		TranCode2;					// Transaction Code2
	int		RQKCode;					// Rqk Code,0:CDK,1:MACKEY,2:PINKEY
	int		RQKCode2;					// Check Rqk mode, 0:CONSTMASTERKEY 1:TMARSTERKEY

	int		TranProcSave;				// Transaction Procedure Save
	int		TranCodeSave;				// Transaction Code Save
	int		TranCode2Save;				// Transaction Code2 Save

	int		ACTranCode;					// A/C Transaction Code

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Menu Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MainMenuDisplay(int InitFlag = FALSE);		// Main Menu Display
	int		fnAPP_MainMenuDeviceEvent(int CheckTime = 0);		// Main Menu Device Event
	int		fnAPP_MainMenuContinueCheck();						// Main Menu Continue Check

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MenuMake(void* MenuTblMake);					// Menu Make
	int		fnAPP_MenuClearMenuCode(int Index = 0, int MenuCode = NULL, int MenuValue = NULL);
																// Menu Clear Menu Code
	int		fnAPP_MenuClearTranProc(int Index = 0, int TranValue = NULL, int MenuValue = NULL);
																// Menu Clear Transaction Procedure
	int		fnAPP_MenuClearTranCode(int Index = 0, int TranValue = NULL, int MenuValue = NULL);
																// Menu Clear Transaction Code
	int		fnAPP_MenuAvailCheck(int Index, int SubIndex = 0);	// Menu Avail Check
	int		fnAPP_MenuBtnCheck(int Index);						// Menu Button Check

/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_MenuProc();									// Menu Procedure
	int		fnAPP_MenuBeforeInput();							// Menu Before Input
	int		fnAPP_MenuDisplay();								// Menu Display
	int		fnAPP_MenuDeviceEvent(int CheckTime = K_65_WAIT);	// Menu Device Event
	int		fnAPP_MenuDeviceProc();								// Menu Device Procedure
	int		fnAPP_MenuDeviceCheck();							// Menu Device Check
	int		fnAPP_MenuCardPbNcAccept(int ProcAvail = 0);		// Menu Card Pb No Card Accept

/////////////////////////////////////////////////////////////////////////////
#endif
