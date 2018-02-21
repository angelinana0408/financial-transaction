/////////////////////////////////////////////////////////////////////////////
#if !defined(_Profiledef_H_)
#define _Profiledef_H_

/////////////////////////////////////////////////////////////////////////////
// Initial Profile¿« Argument
#define DEV_INI 		 1
#define NETWORK_INI 	 2
#define TRANS_INI  		 3

// Section Name
#define DEV_SEC			"DEVICE"
#define NET_SEC			"NETWORK"
#define TRANS_SEC		"TRANS"

#define MAIN_SEC		"MAIN_DATA"
#define ERR_SEC			"ERROR_STACK"

#define TRANMAIN_SEC	"MAIN_TRAN"
#define TRANERR_SEC		"ERROR_TRAN"
#define CANCELMAIN_SEC	"MAIN_CANCEL"
#define CANCELERR_SEC	"ERROR_CANCEL"
#define HOSTMAIN_SEC	"MAIN_HOST"
#define HOSTERR_SEC		"ERROR_HOST"

#define WITHTOTAL_SEC	"WITHTOTAL"
#define INQTOTAL_SEC	"INQTOTAL"
#define TRANSTOTAL_SEC	"TRANSTOTAL"
#define DEPTOTAL_SEC	"DEPTOTAL"

// Machine Type
#define ATM				1			// ATM
#define CDP				2			// CDP
#define CD				3			// CD

// Machine Kind
#define NANOCASH		1			// NANOCASH
#define MCD3			2			// MCD3
#define TTW				3			// TTW
#define MCD4			4			// MCD4
#define TTW2			5			// TTW2
#define EATMGBA1		6			// EATMGBA1
#define EATMULL1		7			// EATMULL1
#define EATMTTW			8			// EATMTTW
#define TTW2E			9			// TTW2E
#define MCD4E			10			// MCD4E
#define MX8000T			11			// MX8000T
#define MX8000L			12			// MX8000L
#define MX5600T			13			// MX5600T
#define MX5600L			14			// MX5600L

// Spr Type
#define SPR_T_TYPE		1			// THERMAL TYPE
#define SPR_D_TYPE		2			// DOT     TYPE
#define SPR_TF_TYPE		3			// THERMAL(FORM) TYPE
#define SPR_DF_TYPE		4			// DOT    (FORM) TYPE

// Jpr Type
#define JPR_T_TYPE		1			// THERMAL TYPE
#define JPR_D_TYPE		2			// DOT     TYPE

// Mcu Type
#define MCU_M_TYPE		1			// MOTOR TYPE
#define MCU_S_TYPE		2			// SWIPE TYPE
#define MCU_D_TYPE		3			// DIP   TYPE
#define MCU_M_NS_TYPE	4			// MOTOR TYPE(NO SHUTTER)

// Device
#define NOT_SET				0		// NOT SET

#define BRM_SET				1		// BRM				(MX8000T, MX8000L)
#define CDU_SHUTTER1510_SET	2		// CDU_SHUTTER1510	(NANOCASH)
#define CDU_SHUTTER20_SET	3		// CDU_SHUTTER20	(EATMGBA1)
#define CDU_SHUTTER30_SET	4		// CDU_SHUTTER30
#define CDU_RETRACT20_SET	5		// CDU_RETRACT20	(MCD3, EATMULL1)
#define CDU_RETRACT30_SET	6		// CDU_RETRACT30
#define CDU_PRESENT20_SET	7		// CDU_PRESENT20
#define CDU_PRESENT30_SET	8		// CDU_PRESENT30	(TTW)
#define CDU_GPRESENT20_SET	9		// CDU_GPRESENT20	(EATMTTW)
#define CDU_GPRESENT30_SET	10		// CDU_GPRESENT30	(MCD4, TTW2, TTW2E, MCD4E, MX5600T, MX5600L)
#define CDU_TRAY20_SET		11		// CDU_TRAY20
#define CDU_TRAY30_SET		12		// CDU_TRAY30

#define PBM_SET		 		1		// AUTO TURN
#define PBM_NAT_SET 		2		// NO	AUTO	TURN

#define SOUND_SET			1		// SOUND
#define PC_SOUND_SET		2		// PC SPEAKER

#define PCI_CAMERA1_SET		1		// PCI TYPE CAMERA1
#define USB_CAMERA1_SET		2		// USB TYPE CAMERA1
#define PCI_CAMERA2_SET		3		// PCI TYPE CAMERA2
#define USB_CAMERA2_SET		4		// USB TYPE CAMERA2
#define USB_AVICAMERA1_SET	5		// USB TYPE AVI CAMERA1
#define USB_AVICAMERA2_SET	6		// USB TYPE AVI CAMERA2

#define ICCARD_EMV			1		// IC CARD EMV
#define ICCARD_PBOC			2		// IC CARD PBOC

#define SPL_SET				1		// SPL
#define VFD_SET				1		// VFD

// Key type
#define KEYMGR_EPP		1
#define SINGLE_EPP		2
#define TRIPLE_EPP		3
#define TRIPLE_NDC		4

// Sound Volume
#define SOUND_OFF		0
#define SOUND_ONE		1
#define SOUND_TWO		2
#define SOUND_THREE		3
#define SOUND_MAX		4

// Power type
#define PNC_TYPE		1
#define UPS_TYPE		2
#define EUL_TYPE		3
#define EUC_TYPE		4

// Spl Device
#define SPL_TOGGLESW	1
#define SPL_ONOFFSW		2

// Op Device
#define OP_TOUCH		1
#define OP_TSPL			2
#define OP_TSPLCHN		3
#define OP_HSPL			4

// Mcu Device
#define MCU_M_OMRON		1
#define MCU_M_KDE		2
#define MCU_M_HYOSUNG	3
#define MCU_M_SANKYO	4
#define MCU_D_MAGTEK	5
#define MCU_M_SANKYO_AS	6

// Mw Device
#define MW_KAL			0
#define MW_NEXTWARE		1
#define MW_KALWOSA		2
#define MW_NEXTWOSA		3

// Nms Device
#define NMS_ZL			1
#define NMS_DC			2
#define NMS_HX			3
#define NMS_BOCOM		4
#define NMS_ICBC		5
#define NMS_RCBFZ		6
#define NMS_CBHEB		7

// Ejr Device
#define EJR_ZL			1
#define EJR_DC			2
#define EJR_HX			3
#define EJR_CUPJN		4

// Ric Device
#define RIC_CBTS		1

// Ird Device
#define IRD_CBTS		1

// protocol type
#define NET_IOC			1
#define NET_SWP			2
#define NET_CCL			3
#define NET_SDLC		4
#define NET_TCPIP		5
#define NET_HDLC		6
#define NET_BAS			7

// Network
#define L1200			0			// LINE SPEED : 0:1200,1:2400,2:4800,3:9600,4:19200
#define L2400			1	
#define L4800			2	
#define L9600			3	
#define L19200			4	
#define POLL_ON			0
#define	POLL_OFF		0x01
#define PTP				0
#define MTP				0x80
#define HALF_DUPLEX		0
#define FULL_DUPLEX		0x40
#define MTF				0
#define FTF				0x10
#define RS422			0
#define RS232C			0x04
#define NRZ				0
#define NRZI			0x02
#define INCLK			0
#define EXCLK			0x01

/////////////////////////////////////////////////////////////////////////////
typedef struct tagDeviceInfo{
	int		MachineType;			// 0:NOT_SET, 1:ATM, 2:CDP, 3:CD
	int		MachineKind;			// 0:NOT_SET, 1:NANOCASH, 2:MCD3, 3:TTW, 4:MCD4, 5:TTW2, 6:EATMGBA1, 7:EATMULL1, 8:EATMTTW, 9:TTW2E, 10:MCD4E, 11:MX8000T, 12:MX8000L, 13:MX5600T, 14:MX5600L
	int		SprHandler;				// 0:NOT_SET, 1:THERMAL, 2:DOT, 3:THERMAL(FORM), 4:DOT(FORM)
	int		JprHandler;				// 0:NOT_SET, 1:THERMAL, 2:DOT
	int		McuHandler;				// 0:NOT_SET, 1:MCU_M_TYPE, 2:MCU_S_TYPE, 3:MCU_D_TYPE, 4:MCU_M_NS_TYPE
	int		CashHandler;			// 0:NOT_SET, 1:BRM, 2:CDU_SHUTTER1510, 3:CDU_SHUTTER20, 4:CDU_SHUTTER30, 5:CDU_RETRACT20, 6:CDU_RETRACT30, 7:CDU_PRESENT20, 8:CDU_PRESENT30, 9:CDU_GPRESENT20, 10:CDU_GPRESENT30, 11:CDU_TRAY20, 12:CDU_TRAY30
	int		PBHandler;				// 0:NOT_SET, 1:PBM, 2:PBM(NO AUTO TURN)
	int		KeyHandler;				// 0:NOT_SET, 1:KEYMGR_EPP, 2:SINGLE_EPP, 3:TRIPLE_EPP, 4:TRIPLE_NDC
	int		SoundDevice;			// 0:NOT_SET, 1:INSTALL
	int		SoundVolume;			// 0:NOT SET, 1, 2, 3, MAX
	int		CameraDevice;			// 0:NOT_SET, 1:PCI TYPE CAMERA1, 2:USB TYPE CAMERA1, 3:PCI TYPE CAMERA2, 4:USB TYPE CAMERA2, 5:USB_AVICAMERA1_SET, 6:USB_AVICAMERA2_SET
	int		ICCardDevice;			// 0:NOT_SET, 1:ICCARD_EMV, 2:ICCARD_PBOC
	int		PowerHandler;			// 0:NOT_SET, 1:PNC_TYPE, 2:UPS_TYPE, 3:EUL_TYPE, 4: EUC_TYPE
	
	int		SplDevice;				// 0:NOT_SET, 1:SPL_TOGGLESW, 2:SPL_ONOFFSW
	int		VfdDevice;				// 0:NOT_SET, 1:INSTALL
	int		DualDevice;				// 0:NOT_SET, 1:DUAL MONITOR
	int		DvrDevice;				// 0:NOT_SET, N:PORT_NUMBER
	int		OpDevice;				// 0:NOT_SET, 1:OP_TOUCH, 2:OP_TSPL, 3:OP_TSPLCHN, 4:OP_HSPL
	int		McuDevice;				// 0:NOT_SET, 1:MCU_M_OMRON, 2:MCU_M_KDE, 3:MCU_M_HYOSUNG, 4:MCU_M_SANKYO, 5:MCU_D_MAGTEK, 6:MCU_M_SANKYO_AS
	int		MwDevice;				// 0:MW_KAL,  1:MW_NEXTWARE, 2:MW_KALWOSA, 3:MW_NEXTWOSA
	int		NmsDevice;				// 0:NOT_SET, 1:NMS_ZL, 2:NMS_DC, 3:NMS_HX, 4:NMS_BOCOM, 5:NMS_ICBC, 6:NMS_RCBFZ, 7:NMS_CBHEB
	int		EjrDevice;				// 0:NOT_SET, 1:EJR_ZL, 2:EJR_DC, 3:EJR_HX, 4:EJR_CUPJN
	int		EtcComKind;				// 0:NOT_SET, N:PORT_NUMBER

	int		RicDevice;				// 0:NOT_SET, 1:RIC_CBTS
	int		RicComPort;				// 0:NOT_SET, N:PORT_NUMBER
	int		IrdDevice;				// 0:NOT_SET, 1:IRD_CBTS
	int		IrdComPort;				// 0:NOT_SET, N:PORT_NUMBER
	int		ETCDevice14;			// 0:NOT_SET
	int		ETCDevice15;			// 0:NOT_SET
	int		ETCDevice16;			// 0:NOT_SET
	int		ETCDevice17;			// 0:NOT_SET
	int		ETCDevice18;			// 0:NOT_SET
	int		ETCDevice19;			// 0:NOT_SET
	int		ETCDevice20;			// 0:NOT_SET
	int		ETCDevice21;			// 0:NOT_SET
	int		ETCDevice22;			// 0:NOT_SET
	int		ETCDevice23;			// 0:NOT_SET
	int		ETCDevice24;			// 0:NOT_SET
	int		ETCDevice25;			// 0:NOT_SET
	int		ETCDevice26;			// 0:NOT_SET
	int		ETCDevice27;			// 0:NOT_SET
	int		ETCDevice28;			// 0:NOT_SET
	int		ETCDevice29;			// 0:NOT_SET
	int		ETCDevice30;			// 0:NOT_SET
	int		ETCDevice31;			// 0:NOT_SET
} DEVICEINFO;

typedef struct tagNetworkInfo{
	char	AtmSerialNum[9];		// Atm Serial Number
	char	BranchNum[10];			// Branch Number
	char	SubBranchNum[9];		// Sub Branch Number		
	char	RoomNum[9];				// Room Number	
	char	MachineMakeDate[9];		// Machine Make Data
	char	MachineMakeNum[17];		// Machine Make Number
	int		BranchInOut;			// Branch In/Out
	int		AutoBranchOnOff;		// Auto Branch On/Off
	int		All24OnOff;				// All 24On/Off
	int		All365OnOff;			// All 365On/Off
	
	int		Interface;				// IOC, HDLC, SWP, SDLC, TCPIP, CCL, BAS
	int		InterfaceKind;			// 0:CUP(WH)				// 2004.05.08
									// 1:BOC(SD)				// 2004.05.08
									// 2:ABC(SD)				// 2004.06.18
									// 3:ABC(FZ,XM,ZJ)			// 2004.06.25
									// 4:RCC(GZ)				// 2004.11.05
									// 5:ABC(ZJG)				// 2004.11.05
									// 6:CCB(JX)				// 2004.11.05
									// 7:ABC(BJ,FZ,XM)			// 2004.10.24
									// 8:POST(SC)				// 2005.02.28
									// 9:SDB(SZ)				// 2005.04.27
									//10:CUP(TJ)				// 2005.05.12 : ZL, YKT, DC
									//11:CUP(BCTC)				// 2005.08.17
									//12:CUP(JN)				// 2005.09.11
									//13:CUP(WH8583)			// 2005.10.20
									//14:CUP(BJ)				// 2006.03.04
									//15:CUP(SH)				// 2006.05.08
									//16:CUP(XM)				// 2006.05.08
									//17:CUP(BJHX)				// 2006.05.20
									//18:CUP(BJGD)				// 2006.07.07
									//19:CUP(DL)				// 2006.07.15
									//20:CUP(SHGD)				// 2006.09.22
									//21:CUP(JX)				// 2007.03.06
									//22:CUP(QD)				// 2009.07.20 : FT
									//23:CUP(YT)				// 2009.07.20 : FT
									//24:CUP(TJ)				// 2009.09.24 : CUP(TJ)
									//25:CUP(XX)				// 2005.09.11
									//26:CUP(XX)				// 2005.09.11
									//27:CUP(XX)				// 2005.09.11
									//28:CUP(XX)				// 2005.09.11
									//29:CUP(XX)				// 2005.09.11
									//30:CUP(XX)				// 2005.09.11

									//100:CUP(DLABC)			// 2006.12.07
									//101:RCB(HF)				// 2007.10.30 : BHC
									//102:RCB(JX)				// 2008.01.16 : BHC
									//103:CB(TS)				// 2008.06.10 : BHC
									//104:RCB(GS)				// 2008.08.29 : BHC
									//105:HYB(BJ)				// 2008.11.05 : BHC
									//106:WRB(BJ)				// 2009.02.12 : BHC
									//107:CB(TYC)				// 2009.04.07 : BHC
									//108:ABC(XM)				// 2009.04.10 : BHC
									//109:CB(ERDS)				// 2009.04.20 : BHC
									//110:SHB(BJ)				// 2009.05.20 : BHC
									//111:RCB(FZ)				// 2009.07.20 : BHC
									//112:CB(TY)				// 2009.07.20 : BHC
									//113:CB(HEB)				// 2009.09.01 : BHC
									//114:IBK(TJ)				// 2011.01.06 : BHC
									//115:CB(AS)				// 2011.01.06 : BHC
									//116:CB(WF)				// 2011.07.25 : BHC

									//1000:CB(JN)				// 2005.10.20 : DC
									//1001:HXB(XA)				// 2006.04.21 : DC
									//1002:CB(LZ)				// 2006.06.01 : DC
									//1003:CB(ZB)				// 2006.06.01 : DC
									//1004:CUP(GD)				// 2006.06.28 : DC
									//1005:CB(XA)				// 2006.08.10 : DC
									//1006:CB(ZH)				// 2006.09.13 : DC
									//1007:CB(SY)				// 2006.09.13 : DC
									//1008:HXB(XA) T-DES		// 2008.02.28 : DC

									//2000:CB(NT)				// 2005.10.20 : FT
									//2001:CB(NTC)				// 2006.02.06 : FT
									//2003:CB(NT) XML			// 2008.03.27 : FT
									//2004:CB(NTC)XML			// 2006.03.27 : FT

									//2500:POST-TY(HNC)			// 2007.05.15 : ATMU
									//2501:POST-TY(HN)			// 2007.05.15 : ATMU

									//3000:HXB(WX)				// 2006.12.07 : YY
									//3001:RCB(SZ)				// 2007.01.05 : YY
									//3002:RCB(SZC)				// 2007.01.05 : YY
									//3003:RCB(KS)				// 2007.03.26 : YY
									//3004:RCB(NB)				// 2007.11.07 : YY
									//3005:HXB(WX) T-DES		// 2008.02.28 : YY
									//3006:RCB(CS)				// 2008.11.26 : YY
									//3007:RCB(XZ)				// 2008.11.26 : YY
									//3008:RCB(SZ) NEW			// 2010.07.08 : YY
									//3009:RCB(KS) TMP			// 2011.03.24 : YY

									//4000:CB(JZ)				// 2006.12.10 : ISO
									//4001:BOCOM(BJ)			// 2007.01.17 : ISO
									//4002:ICBC(SY)				// 2007.04.20 : ISO
									//4003:CB(ZG)				// 2007.06.28 : ISO

									//5000:CB(CZ)				// 2007.04.20 : YKT

									//6000:HXB(DL)				// 2007.12.03 : ZL
	
									//7000:BCTC(PBOC)			// 2010.10.20 : BHC
	
	char	BpIpAddress[16];		// BP Ip Address
	char	BpPortNo[6];			// BP Port No
	char	BpIpAddress2[16];		// BP Ip Address2
	char	BpPortNo2[6];			// BP Port No2
	char	BpIpAddress3[16];		// BP Ip Address3
	char	BpPortNo3[6];			// BP Port No3
	char	BpIpAddress4[16];		// BP Ip Address4
	char	BpPortNo4[6];			// BP Port No4
    char	AtmIPAddress[16];		// Atm IP Address
    char	AtmSubnetMask[16];		// Atm Subnet Mask
    char	AtmGateWay[16];			// Atm GateWay
	char	AtmPortNo[6];			// Atm Port No

	BYTE	DeviceAddr;				// DA
	BYTE	SelectAddr;				// SA 
	int		SendRecvSize;			// Send/Recv SIZE
	int		LineSpeed;				// LINE SPEED
	int		PollCheck;				// POLL CHECK ON/OFF
	int		Point;					// PTP, MTP
	int		Duplex;					// HALF_DUPLEX, FULL_DUPLEX
	int		TimeFiller;				// MTF, FTF
	int		RsType;					// RS-422, RS-232C
	int		Nrz;					// NRZ, NRZI
	int		Clk;					// EXCLK, INCLK
} NETWORKINFO;

typedef struct tagTransInfo{
 	char	ApVersion[41];			// Ap Version
	char	YYYYMMDD[9];			// Date
	char	SerialNo[9];			// Serial No 
	BYTE	ClerkTransOnOff;		// Clerk Transaction On/Off
	char	ProcCount[2];			// Process Count
	int		TransPowerOnOff;		// Transaction Power Off
	char	EjrSerialNo[9];			// Elec Journal Serial No
	int		DeviceExecOff;			// Device Exec Off
	char	DeviceYYYYMMDD[9];		// Device Error Date 
	int		DeviceSprCnt;			// Spr Error Count
	int		DeviceJprCnt;			// Jpr Error Count
	int		DeviceMcuCnt;			// Mcu Error Count
	int		DeviceCshCnt;			// Csh Error Count
	int		DevicePbmCnt;			// Pbm Error Count
	int		DeviceSprRetractCnt;	// Spr Retract Count
	int		DeviceJprRetractCnt;	// Jpr Retract Count
	int		DeviceMcuRetractCnt;	// Mcu Retract Count
	int		DeviceCshRetractCnt;	// Csh Retract Count
	int		DevicePbmRetractCnt;	// Pbm Retract Count
	char	AsPassWord[9];			// As PassWord
	char	OpPassWord[9];			// Op PassWord
	int		SPRPrintCnt;			// SPR Print Count  // added by yusy 08.09.09
	int		JPRPrintCnt;			// JPR Print Count
	int		INKPrintCnt;			// INK Print Count
} TRANSINFO; 

typedef struct tagiDkeyDEVICE{
	char *name;
}iDkeyDEVICE;

typedef struct tagiDkeyNet{
	char *name;
}iDkeyNET;

typedef struct tagiDkeyTrans{
	char *name;
}iDkeyTRANS;

/////////////////////////////////////////////////////////////////////////////
// ErrStack
/////////////////////////////////////////////////////////////////////////////
#define  ErrStack_MaxNumber   256	// Error Stack Max Number

typedef struct tagMainInfo{			// Error Stack Header
	int  Loop;						// Loop
	int  CrtCount;					// Current Count
	int  StackCount;				// Stack Count
} MAININFO;

typedef struct tagErrorInfo{		// Error Stack Detail
	char Date[11];					// Date
	char Time[9];					// Time
	char ProcCount[2];				// Procedure Count
	char ErrorCode[8];				// Error Code
	char ErrorString[81];			// Error String
} ERRSTACK;

typedef struct tagMainName{
	char *name;
} MainName;

typedef struct tagErrorName{
	char *name;
} ErrorName;

/////////////////////////////////////////////////////////////////////////////
// ErrHost
/////////////////////////////////////////////////////////////////////////////
#define  ErrHost_MaxNumber   256	// Error Host Max Number

typedef struct tagHErrInfo{			// Error Host Header
	int  Loop;						// Loop
	int  Count;						// Count
} HERRINFO;

typedef struct tagDErrInfo{			// Error Host Detail
    char Remark[81];				// Remark
} DERRINFO;

typedef struct tagHErrName{
	char *name;
} HErrName;

typedef struct tagDErrName{
	char *name;
} DErrName;

/////////////////////////////////////////////////////////////////////////////
// Total
/////////////////////////////////////////////////////////////////////////////
typedef struct tagLogWFormat{		// Withdraw
	char	Count[6];				// Count
	char	Amount[14];				// Amount
	char	Dummy;					// Dummy
} LOGW;

typedef struct tagWITHTOTALINFO{	// Withdraw Total/Subtotal Info
	LOGW    DATA[16];				// 0:CWD
									// 1:CWC
									// 2-16:CWC2-16
} WITHTOTALINFO;

typedef struct tagLogIFormat{		// Inquiry
	char	Count[6];				// Count
	char	Filler[4];				// Filler
	char	Dummy;					// Dummy
} LOGI;

typedef struct tagINQTOTALINFO{		// Inquiry Total/Subtotal Info
	LOGI    DATA[16];				// 0:INQ
									// 1-16:INQ1-16
} INQTOTALINFO;


typedef struct tagLogTFormat{		// Transfer
	char	Count[6];				// Count
	char	Amount[14];				// Amount
	char	Dummy;					// Dummy
} LOGT;

typedef struct tagTRANSTOTALINFO{	// Transfer Total/Subtotal Info
	LOGT	DATA[16];				// 0:TFR
									// 1-16:TFR1-16
} TRANSTOTALINFO;


typedef struct tagLogDFormat{		// Deposit 
	char	Count[6];				// Count
	char	Amount[14];				// Amount
	char	Dummy;					// Dummy
} LOGD;

typedef struct tagDEPTOTALINFO{		// Deposit Total/Subtotal Info
	LOGD    DATA[16];				// 0:DEP
									// 1:DPC
									// 2-16:DEP2-16
} DEPTOTALINFO;


typedef struct tagWITHNAME{
	char *name;
} WithName;

typedef struct tagINQNAME{
	char *name;
} InqName;

typedef struct tagTRANSNAME{
	char *name;
} TransName;

typedef struct tagDEPNAME{
	char *name;
} DepName;

/////////////////////////////////////////////////////////////////////////////
#endif