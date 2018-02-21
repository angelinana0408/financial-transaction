/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranCmnEtc_H_)
#define _TranCmnEtc_H_

/////////////////////////////////////////////////////////////////////////////
//	Error Conversion Table
/////////////////////////////////////////////////////////////////////////////
typedef struct tagErrorConv
{
	char	MTC[8];
	int		CmpLength;
	char	BankErrorCode[5];
	int		Kind;
} ErrorConv;

static ErrorConv ErrorConvTbl[] = {								// 2004.11.05
/////////////////////////////////////////////////////////////////////////////
//	MTC			CmpLength
//					Bank Error Code
//							0:Not Define,1:Device Error,2:Custom Error
//							3:Clerk Error,4:Communication Error
//									HYOSUNG Remark					Bank Remark
/////////////////////////////////////////////////////////////////////////////
//CONVERSION SEND	"1001"											OEX CARD RETRACT(INSERT)
//CONVERSION SEND	"1002"											OEX CARD RETRACT(OHTER DEVICE ERROR)
//CONVERSION SEND	"1003"											OEX CARD RETRACT(INITIALIZE)
//CONVERSION SEND	"1004"											OEX CARD RETRACT(LOST)
//CONVERSION SEND	"1100"											OEX CARD RETRACT(OTHER)
//CONVERSION SEND	"12AA"											OEX CARD RETRACT(HOST ORDER)
//CONVERSION SEND	"2001"											OEX CASH RETRACT(LOST)
//CONVERSION SEND	"2002"											OEX SPR ERROR
//CONVERSION SEND	"2003"											OEX DISPENSE ERROR(SHUTTER OPEN?)
//CONVERSION SEND	"2100"											OEX DISPENSE ERROR(OTHER)
//CONVERSION SEND	"3001"											OEX DEPOSIT ERROR(REJECT)	: CASH=>ATM
//CONVERSION SEND	"3002"											OEX DEPOSIT ERROR(STACK)	: CASH=>ATM
//CONVERSION SEND	"3003"											OEX DEPOSIT ERROR(STACK)	: CASH=>CUSTOMER
//CONVERSION SEND	"3100"											OEX DEPOSIT ERROR(OTHER)
//
//CONVERSION SEND	"0101"											CANCEL DISPENSE ERROR(SHUTTER NOT OPEN)
//CONVERSION SEND	"0102"											CANCEL DISPENSE ERROR(COUNT ERROR)
//CONVERSION SEND	"0103"											CANCEL DISPENSE ERROR(OTHER)
//CONVERSION SEND	"0201"											CANCEL RECV TIME OUT
//CONVERSION SEND	"0401"											CANCEL MISS MATCH(ATM ID, AMOUNT)
//CONVERSION SEND	"0402"											CANCEL MAC MISS MACTCH
//
	"00000  ",	5,	"????",	0,		// UNKNOWN
	"10000  ",	5,	"????",	0,		// JPR UNKNOWN
	"20000  ",	5,	"????",	0,		// SPR UNKNOWN
	"30000  ",	5,	"????",	0,		// MCU UNKNOWN
	"40000  ",	5,	"4017",	0,		// CDU UNKNOWN
	"50000  ",	5,	"4017",	0,		// BRM UNKNOWN
	"60000  ",	5,	"????",	0,		// PBM UNKNOWN
	"991@@  ",	5,	"????",	3,		// TRANSACTION POWER OFF !!!
	"99999  ",	5,	"????",	0,		// INTERNAL CHECK !!!

	"971    ",	3,	"4017",	1,		// BRM TIMEOVER(COMM)
	"9721   ",	4,	"????",	1,		// JPR TIMEOVER(COMM)
	"9722   ",	3,	"????",	1,		// SPR TIMEOVER(COMM)
	"9723   ",	4,	"????",	1,		// MCU TIMEOVER(COMM)
	"974    ",	3,	"4017",	1,		// CDU TIMEOVER(COMM)
	"975    ",	3,	"????",	1,		// PBM TIMEOVER(COMM)

	"97912  ",	5,	"????",	1,		// SPR TIMEOVER(EVENT)
	"97911  ",	5,	"????",	1,		// JPR TIMEOVER(EVENT)
	"97913  ",	5,	"????",	1,		// MCU TIMEOVER(EVENT)
	"97914  ",	5,	"4017",	1,		// CDU TIMEOVER(EVENT)
	"97915  ",	5,	"4017",	1,		// BRM TIMEOVER(EVENT)
	"97916  ",	5,	"????",	1,		// PBM TIMEOVER(EVENT
	"9791@99",	7,	"????",	1,		// LOCK TIMEOVER

	"97922  ",	5,	"????",	1,		// SPR FATAL(STATUS)
	"97921  ",	5,	"????",	1,		// JPR FATAL(STATUS)
	"97923  ",	5,	"????",	1,		// MCU FATAL(STATUS)
	"97924  ",	5,	"4017",	1,		// CDU FATAL(STATUS)
	"97925  ",	5,	"4017",	1,		// BRM FATAL(STATUS)
	"97926  ",	5,	"????",	1,		// PBM FATAL(STATUS)

	"97992  ",	5,	"????",	1,		// SPR FATAL(STATUS)
	"97991  ",	5,	"????",	1,		// JPR FATAL(STATUS)
	"97993  ",	5,	"????",	1,		// MCU FATAL(STATUS)
	"97994  ",	5,	"4017",	1,		// CDU FATAL(STATUS)
	"97995  ",	5,	"4017",	1,		// BRM FATAL(STATUS)
	"97996  ",	5,	"????",	1,		// PBM FATAL(STATUS)
	"97999  ",	5,	"????",	1,		// ETC FATAL(STATUS)
	
	"7710891",	5,	"4021",	4,		// HOST LINE FAIL
	"7710892",	7,	"4021",	4,		// HOST OPEN FAIL

	"8860291",	5,	"4021",	4,		// SEND TIME OVER
	"8860292",	7,	"4021",	4,		// RECV TIME OVER

	"8850101",	5,	"4003",	4,		// TPC MISMATCH
	"8850102",	7,	"4362",	4,		// MAC MISMATCH
	"8850103",	7,	"4362",	4,		// MAC HOST MISMATCH

	"8850111",	7,	"4003",	4,		// CWD RESP MISMATCH
	"8850112",	7,	"4003",	4,		// SERIAL NO MISMATCH
	"8850113",	7,	"4005",	4,		// AMOUNT MISMATCH
	"8850114",	7,	"4003",	4,		// TFR RESP MISMATCH
	"8850115",	7,	"4003",	4,		// ATM ID MISMATCH

	"8850191",	7,	"4003",	4,		// RECV HEADER FAIL
	"8850192",	7,	"4003",	4,		// RECV DETAIL FAIL
	
	"0110091", 	7,	"????",	2,		// LOST SLIP
	"0110191", 	5,	"1004",	2,		// LOST CARD				// CWC NOT USE : 2005.08.22
	"0110291", 	7,	"????",	2,		// LOST WITHDRAW CASH
	"0110292", 	7,	"????",	2,		// LOST DEPOSIT CASH
	"0110293", 	7,	"????",	2,		// LOST DEPOSIT CASH
	"0110391", 	7,	"1004",	2,		// LOST CARD & SLIP
	"0110591", 	7,	"????",	2,		// LOST PASSBOOK
	"0110691", 	7,	"????",	2,		// LOST PASSBOOK & SLIP
	"0110791", 	7,	"1004",	2,		// LOST PASSBOOK & CARD
	"0110891", 	7,	"1004",	2,		// LOST PASSBOOK & CARD & SLIP
	
	"02100  ",	5,	"????",	1,		// SLIP EXIST !!!
	"82170  ",	5,	"????",	1,		// MCU EXIST !!!
	"82160  ",	5,	"????",	1,		// CASH EXIST !!!
	"82180  ",	5,	"????",	1,		// PBM EXIST !!!

	"1      ",	1,	"????",	1,		// JOURNAL JAM
	"2      ",	1,	"????",	1,		// SLIP JAM
	"3      ",	1,	"????",	1,		// CARD JAM
	"4      ",	1,	"4017",	1,		// CASH JAM(CDU)
	"5      ",	1,	"4017",	1,		// CASH JAM(BRM)
	"6      ",	1,	"????",	1		// PASSBOOK JAM
};

/////////////////////////////////////////////////////////////////////////////
//	Passbook Command
/////////////////////////////////////////////////////////////////////////////
static BYTE	PbFrameSet[]	= { 0x1b, 0x6d, 0x1b, 0x69, 0x01 };
static BYTE	PbFrameNextSet[]= { 0x0d, 0x1b, 0x85, 0x31, 0x0d };
static BYTE	PbAbsRow[]		= { 0x1b, 0x69 };					// Abs Y(1)(>12=>+2)
static BYTE	PbAbsCol[]		= { 0x1b, 0x71 };					// Abs X(0)
static BYTE	PbHanStart[]	= { 0xe0, 0xa2 };					// Han Start
static BYTE	PbHanEnd[]		= { 0xe0, 0xa1 };					// Han End	(Hangul Even)
static BYTE	PbHalfSpace		= { 0x80 };							// ' '		(Line   Even)
static BYTE	PbHalfcomma		= { 0x81 };							// ','
static BYTE	PbHalfperiod	= { 0x82 };							// '.'
static BYTE	PbHalfapostrophe= { 0x83 };							// '''
//static BYTE	PbHalfminuse	= { 0x20 };						// '-'
static BYTE	PbNewLine		= { 0x0a };
	
/////////////////////////////////////////////////////////////////////////////
//	Interface Name
/////////////////////////////////////////////////////////////////////////////
static	char	InterfaceName[][7] = {	
//			 01234567890123456789
			"      ",
			"IOC   ", 
			"SWP   ",
			"CCL   ", 
			"SDLC  ",
			"TCP/IP", 
			"HDLC  ", 
			"BAS   "	
	};

/////////////////////////////////////////////////////////////////////////////
//	Line Speed Name
/////////////////////////////////////////////////////////////////////////////
static	char	LineSpeedName[][6] = {	
//			 01234567890123456789
			"1200 ",
			"2400 ",
			"4800 ",
			"9600 ",
			"19200"			
	};

/////////////////////////////////////////////////////////////////////////////
#endif
