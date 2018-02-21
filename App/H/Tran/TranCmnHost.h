/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnHost_H_)
#define _TranCmnHost_H_

/////////////////////////////////////////////////////////////////////////////
//	Host Normal Message Send
/////////////////////////////////////////////////////////////////////////////

#define		MAXBITMAP			129		// 2 Bitmap : 129
#define		MAXBITMAP2			65		// 1 Bitmap : 65

typedef struct tagHHMSendHeader			// Host Header Message Send
{
	char	Length[2];					// Length
	char	TPDU[5];				// Message Kind
	char	NetworkFlag[2];				// Version No
	char	TransSerialNo[8];			// Terminal Status
	char	ErrorCode[4];			// Reserved Data
} HHMSendHeader;

typedef struct tagHMESEND				// Host Message Edit Send
{
	int		DataTrnCode[MAXBITMAP];
	int		DataLength[MAXBITMAP];		// Data Length		
	char	Data[MAXBITMAP][1024];		// Data
} HMESEND;

typedef struct tagHMERECV				// Host Message Edit Recv
{
	int		DataRecvBit[MAXBITMAP];
	int		DataLength[MAXBITMAP];		// Data Length		
	char	Data[MAXBITMAP][1024];		// Data
} HMERECV;

typedef struct tagHMERAWSEND			// Host Message Edit Unpack Send
{
	int		DataTrnCode[MAXBITMAP];
	int		DataLength[MAXBITMAP];		// Data Length		
	char	Data[MAXBITMAP][1024];		// Unpack Data
} HMERAWSEND;

typedef struct tagATMSTATUSINFO			// ATM STATUS INFO
{
	char	CduStatus[1];				// Cdu Status			(0:Normal, 8: Error, 9:Miss)
	char	DepStatus[1];				// Deposit Status		(0:Normal, 8: Error, 9:Miss)
	char	ICStatus[1];				// IC Card Status		(0:Normal, 8: Error, 9:Miss)
	char	McuStatus[1];				// Mcu Status			(0:Normal, 8: Error, 9:Miss)
	char	DoorStatus[1];				// Door Status			(0:Normal, 8: Error, 9:Miss)
	char	PinpadStatus[1];			// Pinpad Status		(0:Normal, 8: Error, 9:Miss)
	char	DispStatus[1];				// Display Status		(0:Normal, 8: Error, 9:Miss)
	char	DESStatus[1];				// DES(Pinpad) Status	(0:Normal, 8: Error, 9:Miss)
	char	MBStatus[1];				// MasterBroad Status	(0:Normal, 8: Error, 9:Miss)
	char	TicketStatus[1];			// Ticket Printer Status(0:Normal, 8: Error, 9:Miss)
	char	VoiceStatus[1];				// Voice Status			(0:Normal, 8: Error, 9:Miss)
	char	PassbookStatus[1];			// Passbook Status		(0:Normal, 8: Error, 9:Miss)
	char	DevelopStatus[1];			// Develop Status		(0:Normal, 8: Error, 9:Miss)
	char	DVRStatus[1];				// DVR Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam1Status[1];				// Cam1 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam2Status[1];				// Cam2 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam3Status[1];				// Cam3 Status			(0:Normal, 8: Error, 9:Miss)
	char	JprStatus[1];				// Jpr Status			(0:Normal, 1: Low, 2: Empty, 8: Error, 9:Miss)
	char	SprStatus[1];				// Spr Status			(0:Normal, 1: Low, 2: Empty, 8: Error, 9:Miss)
	char	DvrHDStatus[1];				// DvrHD Status			(0:Normal, 1: Near Full, 2: Full, 8: Error, 9:Miss)
	char	Deno1Status[1];				// Deno1Status			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	Deno2Status[1];				// Deno2Status			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	Deno3Status[1];				// Deno3Status			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	Deno4Status[1];				// Deno4Status			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	Deno5Status[1];				// Deno5Status			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	Deno6Status[1];				// Deno6Status			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	DepBoxStatus[1];			// DepBox Status		(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	RejectStatus[1];			// RejectStatus			(0:Normal, 1: Low, 2: Empty, 3: Full, 4: Near Full, 8: Error, 9:Miss)
	char	Cst1Count[4];				// Cst1 Count
	char	Cst2Count[4];				// Cst2 Count
	char	Cst3Count[4];				// Cst3 Count
	char	Cst4Count[4];				// Cst4 Count
	char	Cst5Count[4];				// Cst5 Count
	char	Cst6Count[4];				// Cst6 Count
	char	RejectCount[4];				// Reject Count
	char	Cst1Value[2];				// Cst1 Value
	char	Cst2Value[2];				// Cst2 Value
	char	Cst3Value[2];				// Cst3 Value
	char	Cst4Value[2];				// Cst4 Value
	char	Cst5Value[2];				// Cst5 Value
	char	Cst6Value[2];				// Cst6 Value
	char	AtmStatus[1];				// Atm Status			(0: Normal, C: Out Of Service, M: Maintain)
	char	CardRetainCnt[2];			// Card Retian Count
	char	Version[4];					// SoftwareVersion
	char	Reverse[5];					// Reverse				(99999)
} ATMSTATUSINFO;

typedef struct tagATMSTATUSINFO127			// ATM STATUS INFO
{
	char	ProductCode[6];				// Product Code
	char	CduDepStatus[1];			// Cdu/Deposit Status	(0:Normal, 8: Error, 9:Miss)
	char	ICStatus[1];				// IC Card Status		(0:Normal, 8: Error, 9:Miss)
	char	McuStatus[1];				// Mcu Status			(0:Normal, 8: Error, 9:Miss)
	char	McuExitStatus[1];			// Mcu Exit Status		(0:Normal, 8: Error, 9:Miss)
	char	DoorStatus[1];				// Door Status			(0:Normal, 8: Error, 9:Miss)
	char	PinpadStatus[1];			// Pinpad Status		(0:Normal, 8: Error, 9:Miss)
	char	DispStatus[1];				// Display Status		(0:Normal, 8: Error, 9:Miss)
	char	DESStatus[1];				// DES(Pinpad) Status	(0:Normal, 8: Error, 9:Miss)
	char	MBStatus[1];				// MasterBroad Status	(0:Normal, 8: Error, 9:Miss)
	char	TicketStatus[1];			// Ticket Printer Status(0:Normal, 8: Error, 9:Miss)
	char	VoiceStatus[1];				// Voice Status			(0:Normal, 8: Error, 9:Miss)
	char	PassbookStatus[1];			// Passbook Status		(0:Normal, 8: Error, 9:Miss)
	char	DevelopStatus[1];			// Develop Status		(0:Normal, 8: Error, 9:Miss)
	char	DVRStatus[1];				// DVR Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam1Status[1];				// Cam1 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam2Status[1];				// Cam2 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam3Status[1];				// Cam3 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam4Status[1];				// Cam4 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam5Status[1];				// Cam5 Status			(0:Normal, 8: Error, 9:Miss)
	char	Cam6Status[1];				// Cam6 Status			(0:Normal, 8: Error, 9:Miss)
	char	JprStatus[1];				// Jpr Status			(0:Normal, 1: Low, 2: Empty, 8: Error, 9:Miss)
	char	SprStatus[1];				// Spr Status			(0:Normal, 1: Low, 2: Empty, 8: Error, 9:Miss)
	char	DvrHDStatus[1];				// DvrHD Status			(0:Normal, 1: Near Full, 2: Full, 8: Error, 9:Miss)
	char	Deno1Status[1];				// Deno1Status			(0:Normal, 8: Error, 9:Miss)
	char	Deno2Status[1];				// Deno2Status			(0:Normal, 8: Error, 9:Miss)
	char	Deno3Status[1];				// Deno3Status			(0:Normal, 8: Error, 9:Miss)
	char	Deno4Status[1];				// Deno4Status			(0:Normal, 8: Error, 9:Miss)
	char	Deno5Status[1];				// Deno5Status			(0:Normal, 8: Error, 9:Miss)
	char	Deno6Status[1];				// Deno6Status			(0:Normal, 8: Error, 9:Miss)
	char	RejectStatus[1];			// RejectStatus			(0:Normal, 8: Error, 9:Miss)
	char	DepositStatus[1];			// DepositStatus		(0:Normal, 3: Full, 4: Near Full, 7: ReturnOver, 9:Miss)
	char	WithdrawStatus[1];			// DepositStatus		(0:Normal, 1: Low,  2: Empty, 7: ReturnOver, 9:Miss)
	char	RejectOperationStatus[1];	// RejectOperationStatus(0:Normal, 3: Full, 4: Near Full)
	char	Cst1Count[4];				// Cst1 Count
	char	Cst2Count[4];				// Cst2 Count
	char	Cst3Count[4];				// Cst3 Count
	char	Cst4Count[4];				// Cst4 Count
	char	Cst5Count[4];				// Cst5 Count
	char	Cst6Count[4];				// Cst6 Count
	char	RejectCount[4];				// Reject Count
	char	Cst1Value[2];				// Cst1 Value
	char	Cst2Value[2];				// Cst2 Value
	char	Cst3Value[2];				// Cst3 Value
	char	Cst4Value[2];				// Cst4 Value
	char	Cst5Value[2];				// Cst5 Value
	char	Cst6Value[2];				// Cst6 Value
	char	AtmStatus[1];				// Atm Status			(0: Normal, C: Out Of Service, M: Maintain)
	char	CardRetainCnt[2];			// Card Retian Count
	char	Version[4];					// SoftwareVersion
	char	HeadAdVersion[6];			// HeadAdVersion
	char	BranchAdVersion[6];			// BranchAdVersion
	char	PoundageVersion[6];			// PoundageVersion
	char	Reverse[5];					// Reverse				(99999)
} ATMSTATUSINFO127;

typedef struct tagATMTOTALINFO			// ATM STATUS INFO
{
	char	WithdrawCount[6];			// Withdraw Count
	char	WithdrawAmount[12];			// Withdraw Amount
	char	DepositCount[6];			// Deposit Count
	char	DepositAmount[12];			// Deposit Amount
	char	TransferCount[6];			// Transfer Count
	char	TransferAmount[12];			// Transfer Amount
	char	RemainAmount[12];			// Remain Amount
	char	DepBoxAmount[12];			// Deposit Box Amount
	char	CashLoadCount[6];
	char	CashLoadAmount[12];
	char	LoadCount[6];
	char	LoadAmount[12];
} ATMTOTALINFO;

typedef struct tagATMADDCASHINFO		// ATM ADDCASH INFO
{
	char	Cst1Count[4];				// Cst1 Count
	char	Cst2Count[4];				// Cst2 Count
	char	Cst3Count[4];				// Cst3 Count
	char	Cst4Count[4];				// Cst4 Count
	char	Cst5Count[4];				// Cst5 Count
	char	Cst6Count[4];				// Cst6 Count
	char	Cst1Value[2];				// Cst1 Value
	char	Cst2Value[2];				// Cst2 Value
	char	Cst3Value[2];				// Cst3 Value
	char	Cst4Value[2];				// Cst4 Value
	char	Cst5Value[2];				// Cst5 Value
	char	Cst6Value[2];				// Cst6 Value
} ATMADDCASHINFO;

/////////////////////////////////////////////////////////////////////////////
//	Ap Device Area
/////////////////////////////////////////////////////////////////////////////
typedef struct tagPSPRecv				// Parameter Slip Print Recv
{
	int		ParaLength;					// PARA Length
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	char	Data[SPR_MAX_ROW][256];		// DATA
} PSPRecv;

typedef struct tagPPPRecv				// Parameter Passbook Print Recv
{
	int		ParaLength;					// PARA Length
	int     Bp;							// Begin Page
	int     Ep;							// End Page
	int		p;							// Position p
	int		y;							// Position y
	int		x;							// Position x
	int		DataLength[10][24];			// DATA Length		
	char	Data[10][24][256];			// DATA
} PPPRecv;

typedef struct tagPPWRecv				// Parameter Passbook Write Recv
{
	int		ParaLength;					// PARA Length
	int		x;							// Position x
	char	Data[256];					// DATA
} PPWRecv;

typedef struct tagPCWRecv				// Parameter Card Write Recv
{
	int		ParaLength;					// PARA Length
	int		x;							// Position x
	char	Data[256];					// DATA
} PCWRecv;

typedef struct tagPMERecv				// Parameter Message Edit Recv
{
	int		ParaLength;					// PARA Length
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	int		DataLength[129];			// DATA Length		
	char	Data[129][1024];			// DATA
} PMERecv;

typedef struct tagPMDRecv				// Parameter Message Display Recv
{
	int		ParaLength;					// PARA Length
	int		By;							// Begin y
	int		Ey;							// End y
	int		y;							// Position y
	int		x;							// Position x
	char	Data[30][256];				// DATA
} PMDRecv;

/////////////////////////////////////////////////////////////////////////////
//	Accepted Response
/////////////////////////////////////////////////////////////////////////////
typedef struct tagTranResponse			// Accepted Response: CWD,CWN,TFR,PIN,INQ
{
	char	Err_code[4];				// Error Code
	char	Card_no[19];				// Card No
	char	Amt[12];					// Amount
	char	VAmt[12];					// V-Amount
	char	Host_ser[6];				// Host Serial No
	char	Host_Date[10];				// Host DateTime
} TRANRESPONSE;

typedef struct tagRQKResponse			// Accepted Response: RQK
{
//	char	Author_Code[4];				// Author Code
	char	Pin_Key[16];				// Pin Key
	char	Mac_Key[16];				// Mac Key
} RQKRESPONSE;

/////////////////////////////////////////////////////////////////////////////
#endif

