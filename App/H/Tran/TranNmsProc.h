/////////////////////////////////////////////////////////////////////////////
#if !defined(_TranNmsProc_H_INCLUDED_)
#define _TranNmsProc_H_INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Attributes
public:
/////////////////////////////////////////////////////////////////////////////
//	Nms Variable
/////////////////////////////////////////////////////////////////////////////
struct	tagHostNms						// Host Nms
{
	int		FirstOpenFlag;				// First Open Flag
	int		OpenFlag;					// Open Flag
	int		PowerOffFlag;				// Power Off Flag
	int		RebootFlag;					// Reboot Flag
	int		ErrorAutoOffSave;			// Error Auto Off Save
	int		ErrorKind;					// Error Kind
	int		ErrorFlag;					// Error Flag
	int		HostResetAll;				// Host Reset All Devices
	int		ModuleResetFlag;			// Host Reset Devices 1:MCU 2:CDU 3:SPR 4:JPR 5:PB

	int		RecycleBoxStatus;			// Recycle Box Status
	int		FstCSTStatus;				// First CST Status
	int		SndCSTStatus;				// Second CST Status
	int		TrdCSTStatus;				// Third CST Status
	int		FthCSTStatus;				// Fourth CST Status
	int		WithAvail;					// Withdraw Avail
	int		SlipStatus;					// Slip Status
	int		JnlStatus;					// Journal Status
	int		DoorStatus;					// Door	Status
	int		DvrStatus;					// Dvr Status
	int		ServiceStatus;				// Service Status
	int		ErrorStatus;				// Error Status
	int		CloseStatus;				// Close Status
	int		CduDeviceStatus;			// Cdu Device Status
	int		SprDeviceStatus;			// Spr Device Status
	int		JprDeviceStatus;			// Jpr Device Status
	int		McuDeviceStatus;			// Mcu Device Status
	int		PinDeviceStatus;			// Pin Device Status

	int		DeviceErrorSendFlag;		// Device Error Send Flag

	int		RqkSendFlag;				// Rqk Send Flag
	int		CwcSendFlag;				// Cwc Send Flag
	int		OexSendFlag;				// Oex Send Flag
	int		AexCustomerCardTimeOutSendFlag;	// Customer Time Out Send Flag
	int		AexCustomerCashTimeOutSendFlag;	// Customer Cancel Send Flag
	int		AexCustomerHostRetractSendFlag;	// Customer Cancel Send Flag
	int		OexBothDenoNotesSendFlag;	// Both Denoination Notes Send Flag
	int		OexPowerOnSendFlag;			// Power On Send Flag
	int		OexCloseBySpvSendFlag;		// Close By Supervisor Send Flag
	int		OexOpenBySpvSendFlag;		// Open By Supervisor Send Flag
	int		OexSlipPaperOutSendFlag;	// Slip Paper Out Send Flag
	int		OexJnlPaperOutSendFlag;		// Journal Paper Out Send Flag
	int		OexDoorOpenErrorFlag;		// Door Open Error Flag
	int		OexDoorOpenSendFlag;		// Door Open Send Flag
	int		OexServiceModeSendFlag;		// Service Mode Send Flag
	int		MsrSendFlag;				// Msr Send Flag

	int			SaveErrCnt;
	ERRSTACK	SaveErrBuff;
} HostNms;
	
	int		NmsSendLength;										// Nms Send Length
	int		NmsRecvLength;										// Nms Recv Length
	int		NmsRecvLengthTbl[NMSRECVTBL];						// Nms Recv Length Table

	int		NmsRecvInTbl;										// Nms Recv In Table
	int		NmsRecvOutTbl;										// Nms Recv Out Table
	BYTE	NmsSendBuffer[NETBUFFSIZE];							// Nms Send Buffer
	BYTE	NmsRecvBuffer[NETBUFFSIZE];							// Nms Recv Buffer
	BYTE	NmsRecvBufferTbl[NMSRECVTBL][NETBUFFSIZE];			// Nms Recv Buffer Table

/////////////////////////////////////////////////////////////////////////////
// Operations
public:
/////////////////////////////////////////////////////////////////////////////
//	Nms Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_NmsProc(int InitFlag = FALSE);				// Nms Procedure

/////////////////////////////////////////////////////////////////////////////
//	Send/Recv Function
/////////////////////////////////////////////////////////////////////////////
	int		fnAPP_LineOpen(int OpenSec, int nAsyncFlag = FALSE);
																// Line Open
	int		fnAPP_LineClose(int CloseSec, int nAsyncFlag = FALSE);
																// Line Close
	int		fnAPP_SendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlag = TRUE);
																// Send Data
	int		fnAPP_RecvDataClear();								// Recv Data Clear
	int		fnAPP_RecvData(BYTE* RecvBuff, int* Length, int RecvSec);
	int		fnAPP_RecvInTbl(BYTE* Buff, int Length);			// Recv In Table
	int		fnAPP_RecvOutTbl(BYTE* Buff, int* Length);			// Recv Out Table
	int		fnAPP_NmsSendData(BYTE* SendBuff, int Length, int SendSec, int RecvBuffClearFlag = TRUE);
																// Nms Send Data
	int		fnAPP_NmsRecvData(BYTE* RecvBuff, int* Length, int RecvSec);
/////////////////////////////////////////////////////////////////////////////
#endif
