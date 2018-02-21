/////////////////////////////////////////////////////////////////////////////
//	Profile.cpp: implementation of the CProfile class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "stdio.h"
#include "stdlib.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Profile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

iDkeyDEVICE DEVKEY[]={
	"MachineType"  				,
	"MachineKind"  				,
	"SprHandler"  				,
	"JprHandler"  				,
	"McuHandler"  				,
	"CashHandler"				,
	"PBHandler"    				,
	"KeyHandler" 				,
	"SoundDevice"  				,
	"SoundVolume"  				,
	"CameraDevice" 				,
	"ICCardDevice"				,
	"PowerHandler"				,
	"ETCDevice0"				,								// DO NOT CHANGE !!! : 2005.04.19
	"ETCDevice1"				,
	"ETCDevice2"				,
	"ETCDevice3"				,
	"ETCDevice4"				,
	"ETCDevice5"				,
	"ETCDevice6"				,
	"ETCDevice7"				,
	"ETCDevice8"				,
	"ETCDevice9"				,
	"ETCDevice10"				,
	"ETCDevice11"				,
	"ETCDevice12"				,
	"ETCDevice13"				,
	"ETCDevice14"				,
	"ETCDevice15"				,
	"ETCDevice16"				,
	"ETCDevice17"				,
	"ETCDevice18"				,
	"ETCDevice19"				,
	"ETCDevice20"				,
	"ETCDevice21"				,
	"ETCDevice22"				,
	"ETCDevice23"				,
	"ETCDevice24"				,
	"ETCDevice25"				,
	"ETCDevice26"				,
	"ETCDevice27"				,
	"ETCDevice28"				,
	"ETCDevice29"				,
	"ETCDevice30"				,
	"ETCDevice31"
};

iDkeyNET NETKEY[]={
	"AtmSerialNum"				,
	"BranchNum"					,
	"SubBranchNum"				,
	"RoomNum"					,
	"MachineMakeDate"			,
	"MachineMakeNum"			,
	"BranchInOut"				,
	"AutoBranchOnOff"			,
	"All24OnOff"				,
	"All365OnOff"				,

	"Interface"					,
	"InterfaceKind"				,
	"BpIpAddress"				,
	"BpPortNo"					,
	"BpIpAddress2"				,
	"BpPortNo2"					,
	"BpIpAddress3"				,
	"BpPortNo3"					,
	"BpIpAddress4"				,
	"BpPortNo4"					,
    "AtmIPAddress"				,
    "AtmSubnetMask"				,
    "AtmGateWay"				,
	"AtmPortNo"					,

	"DeviceAddr"				,
	"SelectAddr"				,
	"SendRecvSize"				,
	"LineSpeed"					,
	"PollCheck"					,
	"Point"						,
	"Duplex"					,
	"TimeFiller"				,
	"RsType"					,
	"Nrz"						,
	"Clk"
};

iDkeyTRANS TRANKEY[]={
	"ApVersion"					,		
	"YYYYMMDD"					,			
	"SerialNo"					,			
	"ClerkTransOnOff"			,	
	"ProcCount"					,		
	"TransPowerOnOff"			,
	"EjrSerialNo"				,
	"DeviceExecOff"				,
	"DeviceYYYYMMDD"			,
	"DeviceSprCnt"				,
	"DeviceJprCnt"				,
	"DeviceMcuCnt"				,
	"DeviceCshCnt"				,
	"DevicePbmCnt"				,
	"DeviceSprRetractCnt"		,
	"DeviceJprRetractCnt"		,
	"DeviceMcuRetractCnt"		,
	"DeviceCshRetractCnt"		,
	"DevicePbmRetractCnt"		,
	"AsPassWord"				,
	"OpPassWord"				,
	"SPRPrintCnt"				,						// added by yusy 08.09.09
	"JPRPrintCnt"				,
	"INKPrintCnt"
};

CRITICAL_SECTION CProfile::m_csLock;

/////////////////////////////////////////////////////////////////////////////
//	Construction/Destruction
/////////////////////////////////////////////////////////////////////////////
CProfile::CProfile()
{
	InitializeCriticalSection(&m_csLock);
}

CProfile::~CProfile()
{
	DeleteCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	INTERNAL FUNCTION
/////////////////////////////////////////////////////////////////////////////
#define DefSize		32768
int CProfile::GetPInt(char* SectName, char* KeyName, int nDef, char* FileName)
{
	return (GetPrivateProfileInt(SectName, KeyName, nDef, FileName));
}

void CProfile::GetPStr(char* SectName, char* KeyName, char sDef, void *cDest, int nSize, char* FileName)
{
	char	DefTemp[DefSize];
	char	Temp[DefSize];
	char*	Dest = (char*)cDest;

	DefTemp[0] = '(';
	memset(&DefTemp[1], sDef, nSize - 1);
	DefTemp[nSize] = ')';
	DefTemp[nSize + 1] = NULL;
	GetPrivateProfileString(SectName, KeyName, DefTemp, Temp, sizeof(Temp), FileName);
	memcpy(Dest, &Temp[1], nSize - 1);
	Dest[nSize - 1] = NULL;
}

void CProfile::GetPByten(char* SectName, char* KeyName, BYTE *Dest, int nSize, char* FileName)
{
	char	DefTemp[DefSize];
	char	Temp[DefSize];

	DefTemp[0] = '(';
	memset(&DefTemp[1], '0', nSize * 2);
	DefTemp[nSize * 2 + 1] = ')';
	DefTemp[nSize * 2 + 2] = NULL;
	GetPrivateProfileString(SectName, KeyName, DefTemp, Temp, sizeof(Temp), FileName);
	Asc2Byten(&Temp[1], Dest, nSize);
}

void CProfile::PutPInt(char* SectName, char* KeyName, int Src, char* FileName, int nFlushFlag)
{
	char	Temp[DefSize];

	sprintf(Temp, "%d", Src);
	WritePrivateProfileString(SectName, KeyName, Temp, FileName);
	if (nFlushFlag)
		IniFlush(FileName);
}

void CProfile::PutPStr(char* SectName, char* KeyName, void *cSrc, int nSize, char* FileName, int nFlushFlag)
{
	char	Temp[DefSize];
	int		sLen;
	char*	Src = (char*)cSrc;
	BOOL    bRet = FALSE;

	Temp[0] = '(';
	memset(&Temp[1], ' ', sizeof(Temp) - 1);

	sLen = strlen(Src);
	if (nSize > sLen)
	{
		memcpy(&Temp[1], Src, sLen);
		Temp[nSize] = ')';
		Temp[nSize + 1] = NULL;
	}
	else
	{
		memcpy(&Temp[1], Src, nSize - 1);
		Temp[nSize] = ')';
		Temp[nSize + 1] = NULL;
	}
	bRet = WritePrivateProfileString(SectName, KeyName, Temp, FileName);
	if (!bRet)
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "WritePrivateProfileString ERR");
	}

	if (nFlushFlag)
		IniFlush(FileName);
}

void CProfile::PutPByten(char* SectName, char* KeyName, BYTE *Src, int nSize, char* FileName, int nFlushFlag)
{
	char	Temp[DefSize];

	Temp[0] = '(';
	Byte2Ascn(Src, &Temp[1], nSize, 16);
	Temp[nSize * 2 + 1] = ')';
	Temp[nSize * 2 + 2] = NULL;
	WritePrivateProfileString(SectName, KeyName, Temp, FileName);
	if (nFlushFlag)
		IniFlush(FileName);
}

/////////////////////////////////////////////////////////////////////////////
//	Device/Network/Trans
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitProfileCheck()
{
	HFILE fi;

	// Device Ini File,If the file doesn't exist then create the file
	fi = _lopen(_DEVICE_INI, OF_READWRITE);
	if (fi < 0)
	{
		_mkdir(_INI);											// \\INI\\ DIRECTORY Create
		InitProfile(DEV_INI);
	}
	else
	{
		_lclose(fi);
	    GetDeviceProfile();
	}

	// Network Ini File,If the file doesn't exist then create the file
	fi = _lopen(_NETWORK_INI, OF_READWRITE);
	if (fi < 0)
		InitProfile(NETWORK_INI);
	else
	{
		_lclose(fi);
		GetNetworkProfile();
	}

	// Trans Ini File,If the file doesn't exist then create the file
	fi = _lopen(_TRANS_INI, OF_READWRITE);
	if (fi < 0)
		InitProfile(TRANS_INI);
	else
	{
		_lclose(fi);
		GetTransProfile();
	}

	// ErrStack Ini File,If the file doesn't exist then create the file
	fi = _lopen(_ERRSTACK_INI, OF_READWRITE);
	if (fi < 0) 
		ClearAllErrStack();
	else _lclose(fi);

	// Error Ini File,If the file doesn't exist then create the file
	fi = _lopen(_ERRHOST_INI, OF_READWRITE);
	if (fi < 0) 
	{
		ClearAllErrTran();										// Clear All Error Tran
		ClearAllErrCancel();									// Clear All Error Cancel
		ClearAllErrHost();										// Clear All Error Host
	}
	else _lclose(fi);

	// Total Ini File,If the file doesn't exist then create the file
	fi = _lopen(_ATMTOTAL_INI, OF_READWRITE);
	if (fi < 0) 
	{
		InitWithTotal();										// Init Withdraw Total
		InitInqTotal();											// Init Inquiry Total
		InitTransTotal();										// Init Transfer Total
		InitDepTotal();											// Init Deposit Total
	}
	else _lclose(fi);

}

/////////////////////////////////////////////////////////////////////////////
//	If the file doesn't exist then create the file
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitProfile(int type)
{
	switch (type) 
	{
		case DEV_INI:
			InitDeviceProfile();								
			break;
		case NETWORK_INI:	
			InitNetworkProfile();
			break;
		case TRANS_INI:
			InitTransProfile();
			break;
		default:
			break;
	}
}

/////////////////////////////////////////////////////////////////////////////
//	Initial Device Profile 
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitDeviceProfile()
{
	memset(&DEVICE, 0, sizeof(DEVICE));

//	DEVICE.MachineType   = ATM;								
//	DEVICE.MachineType   = CDP;
	DEVICE.MachineType   = CD;
	DEVICE.MachineKind   = NANOCASH;

	switch (DEVICE.MachineType)
	{
		case ATM:
			DEVICE.SprHandler    = SPR_T_TYPE;
			DEVICE.JprHandler    = JPR_D_TYPE;
			DEVICE.McuHandler    = MCU_M_TYPE;
			DEVICE.CashHandler   = BRM_SET;
			DEVICE.PBHandler     = PBM_NAT_SET;
			DEVICE.KeyHandler    = KEYMGR_EPP;
			DEVICE.SoundDevice   = SOUND_SET;
			DEVICE.SoundVolume   = SOUND_ONE;
			DEVICE.CameraDevice  = USB_CAMERA1_SET;
			DEVICE.ICCardDevice  = NOT_SET;
			DEVICE.PowerHandler  = PNC_TYPE;
			DEVICE.SplDevice	 = NOT_SET;						// 2004.11.05
			DEVICE.VfdDevice	 = NOT_SET;
			DEVICE.DualDevice	 = NOT_SET;
			DEVICE.DvrDevice	 = NOT_SET;
			DEVICE.OpDevice		 = NOT_SET;
			DEVICE.McuDevice	 = NOT_SET;
			DEVICE.MwDevice		 = MW_KAL;
			DEVICE.NmsDevice	 = NOT_SET;
			DEVICE.EjrDevice	 = NOT_SET;
			DEVICE.EtcComKind	 = NOT_SET;
			DEVICE.RicDevice	 = NOT_SET;
			DEVICE.RicComPort	 = NOT_SET;
			DEVICE.IrdDevice	 = NOT_SET;
			DEVICE.IrdComPort	 = NOT_SET;
			DEVICE.ETCDevice14	 = NOT_SET;
			DEVICE.ETCDevice15	 = NOT_SET;
			DEVICE.ETCDevice16	 = NOT_SET;
			DEVICE.ETCDevice17	 = NOT_SET;
			DEVICE.ETCDevice18	 = NOT_SET;
			DEVICE.ETCDevice19	 = NOT_SET;
			DEVICE.ETCDevice20	 = NOT_SET;
			DEVICE.ETCDevice21	 = NOT_SET;
			DEVICE.ETCDevice22	 = NOT_SET;
			DEVICE.ETCDevice23	 = NOT_SET;
			DEVICE.ETCDevice24	 = NOT_SET;
			DEVICE.ETCDevice25	 = NOT_SET;
			DEVICE.ETCDevice26	 = NOT_SET;
			DEVICE.ETCDevice27	 = NOT_SET;
			DEVICE.ETCDevice28	 = NOT_SET;
			DEVICE.ETCDevice29	 = NOT_SET;
			DEVICE.ETCDevice30	 = NOT_SET;
			DEVICE.ETCDevice31	 = NOT_SET;
			break;

		case CDP:	
			DEVICE.SprHandler    = SPR_T_TYPE;
			DEVICE.JprHandler    = JPR_D_TYPE;
			DEVICE.McuHandler    = MCU_M_TYPE;
			DEVICE.CashHandler   = CDU_SHUTTER20_SET;
			DEVICE.PBHandler     = PBM_NAT_SET;
			DEVICE.KeyHandler    = KEYMGR_EPP;
			DEVICE.SoundDevice   = SOUND_SET;
			DEVICE.SoundVolume   = SOUND_ONE;
			DEVICE.CameraDevice  = USB_CAMERA1_SET;
			DEVICE.ICCardDevice  = NOT_SET;
			DEVICE.PowerHandler  = PNC_TYPE;
			DEVICE.SplDevice	 = NOT_SET;
			DEVICE.VfdDevice	 = NOT_SET;
			DEVICE.DualDevice	 = NOT_SET;
			DEVICE.DvrDevice	 = NOT_SET;
			DEVICE.OpDevice		 = NOT_SET;
			DEVICE.McuDevice	 = NOT_SET;
			DEVICE.MwDevice		 = MW_KAL;
			DEVICE.NmsDevice	 = NOT_SET;
			DEVICE.EjrDevice	 = NOT_SET;
			DEVICE.EtcComKind	 = NOT_SET;
			DEVICE.RicDevice	 = NOT_SET;
			DEVICE.RicComPort	 = NOT_SET;
			DEVICE.IrdDevice	 = NOT_SET;
			DEVICE.IrdComPort	 = NOT_SET;
			DEVICE.ETCDevice14	 = NOT_SET;
			DEVICE.ETCDevice15	 = NOT_SET;
			DEVICE.ETCDevice16	 = NOT_SET;
			DEVICE.ETCDevice17	 = NOT_SET;
			DEVICE.ETCDevice18	 = NOT_SET;
			DEVICE.ETCDevice19	 = NOT_SET;
			DEVICE.ETCDevice20	 = NOT_SET;
			DEVICE.ETCDevice21	 = NOT_SET;
			DEVICE.ETCDevice22	 = NOT_SET;
			DEVICE.ETCDevice23	 = NOT_SET;
			DEVICE.ETCDevice24	 = NOT_SET;
			DEVICE.ETCDevice25	 = NOT_SET;
			DEVICE.ETCDevice26	 = NOT_SET;
			DEVICE.ETCDevice27	 = NOT_SET;
			DEVICE.ETCDevice28	 = NOT_SET;
			DEVICE.ETCDevice29	 = NOT_SET;
			DEVICE.ETCDevice30	 = NOT_SET;
			DEVICE.ETCDevice31	 = NOT_SET;
			break;

		case CD:
			DEVICE.SprHandler    = SPR_T_TYPE;
			DEVICE.JprHandler    = NOT_SET;
			DEVICE.McuHandler    = MCU_M_TYPE;
			DEVICE.CashHandler   = CDU_SHUTTER1510_SET;
			DEVICE.PBHandler     = NOT_SET;
			DEVICE.KeyHandler    = KEYMGR_EPP;
			DEVICE.SoundDevice   = SOUND_SET;
			DEVICE.SoundVolume   = SOUND_ONE;
			DEVICE.CameraDevice  = USB_CAMERA1_SET;
			DEVICE.ICCardDevice  = NOT_SET;
			DEVICE.PowerHandler  = NOT_SET;
			DEVICE.SplDevice	 = NOT_SET;
			DEVICE.VfdDevice	 = NOT_SET;
			DEVICE.DualDevice	 = NOT_SET;
			DEVICE.DvrDevice	 = NOT_SET;
			DEVICE.OpDevice		 = NOT_SET;
			DEVICE.McuDevice	 = NOT_SET;
			DEVICE.MwDevice		 = MW_KAL;
			DEVICE.NmsDevice	 = NOT_SET;
			DEVICE.EjrDevice	 = NOT_SET;
			DEVICE.EtcComKind	 = NOT_SET;
			DEVICE.RicDevice	 = NOT_SET;
			DEVICE.RicComPort	 = NOT_SET;
			DEVICE.IrdDevice	 = NOT_SET;
			DEVICE.IrdComPort	 = NOT_SET;
			DEVICE.ETCDevice14	 = NOT_SET;
			DEVICE.ETCDevice15	 = NOT_SET;
			DEVICE.ETCDevice16	 = NOT_SET;
			DEVICE.ETCDevice17	 = NOT_SET;
			DEVICE.ETCDevice18	 = NOT_SET;
			DEVICE.ETCDevice19	 = NOT_SET;
			DEVICE.ETCDevice20	 = NOT_SET;
			DEVICE.ETCDevice21	 = NOT_SET;
			DEVICE.ETCDevice22	 = NOT_SET;
			DEVICE.ETCDevice23	 = NOT_SET;
			DEVICE.ETCDevice24	 = NOT_SET;
			DEVICE.ETCDevice25	 = NOT_SET;
			DEVICE.ETCDevice26	 = NOT_SET;
			DEVICE.ETCDevice27	 = NOT_SET;
			DEVICE.ETCDevice28	 = NOT_SET;
			DEVICE.ETCDevice29	 = NOT_SET;
			DEVICE.ETCDevice30	 = NOT_SET;
			DEVICE.ETCDevice31	 = NOT_SET;
			break;
	}

	PutDeviceProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Initial Network Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitNetworkProfile()
{
	memset(&NETWORK, 0, sizeof(NETWORK));

	strcpy(NETWORK.AtmSerialNum,	"12345678");
	strcpy(NETWORK.BranchNum,		"876543210");
	StrAllZero(NETWORK.SubBranchNum);
	StrAllZero(NETWORK.RoomNum);
	strcpy(NETWORK.MachineMakeDate,	"20040101");
	StrAllZero(NETWORK.MachineMakeNum);
	NETWORK.BranchInOut		= OFF; 
	NETWORK.AutoBranchOnOff	= ON;
	NETWORK.All24OnOff		= ON; 
	NETWORK.All365OnOff		= ON; 

	NETWORK.Interface		= NET_TCPIP;
	NETWORK.InterfaceKind	= 0;
	strcpy(NETWORK.BpIpAddress,		"192.168.001.100");
	strcpy(NETWORK.BpPortNo,		"08001");
	strcpy(NETWORK.BpIpAddress2,	"000.000.000.000");
	strcpy(NETWORK.BpPortNo2,		"00000");
	strcpy(NETWORK.BpIpAddress3,	"000.000.000.000");
	strcpy(NETWORK.BpPortNo3,		"00000");
	strcpy(NETWORK.BpIpAddress4,	"000.000.000.000");
	strcpy(NETWORK.BpPortNo4,		"00000");
	switch (DEVICE.MachineType)
	{
		case ATM:
		    strcpy(NETWORK.AtmIPAddress,	"192.168.001.050");
			break;

		case CDP:	
		    strcpy(NETWORK.AtmIPAddress,	"192.168.001.050");
			break;

		case CD:
		    strcpy(NETWORK.AtmIPAddress,	"192.168.001.050");
			break;
	}
    strcpy(NETWORK.AtmSubnetMask,	"255.255.255.000");
    strcpy(NETWORK.AtmGateWay,		"192.168.001.001");
	strcpy(NETWORK.AtmPortNo,		"00000");

	NETWORK.DeviceAddr		= 0x00; 
	NETWORK.SelectAddr		= 0x00; 
	NETWORK.SendRecvSize    = 1024;
	NETWORK.LineSpeed		= L19200;
	NETWORK.PollCheck		= POLL_ON;
	NETWORK.Point			= PTP;
	NETWORK.Duplex			= HALF_DUPLEX;
	NETWORK.TimeFiller		= MTF;
	NETWORK.RsType          = RS422;
	NETWORK.Nrz             = NRZ;
	NETWORK.Clk			    = INCLK;

	PutNetworkProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Initial Trans Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::InitTransProfile()
{
	memset(&TRANS, 0, sizeof(TRANS));

	switch (DEVICE.MachineType)
	{
		case ATM:
			//						 0123456789012345678901234567890123456789
			strcpy(TRANS.ApVersion, "V00-00-00 CUP ATM                       ");
			break;

		case CDP:	
			//						 0123456789012345678901234567890123456789
			strcpy(TRANS.ApVersion, "V00-00-00 CUP CDP                       ");
			break;

		case CD:
			//						 0123456789012345678901234567890123456789
			strcpy(TRANS.ApVersion, "V00-00-00 CUP CD                        ");
			break;
	}
	GetDate(TRANS.YYYYMMDD);
	TRANS.YYYYMMDD[8]			= NULL;
	StrAllZero(TRANS.SerialNo);
	TRANS.ClerkTransOnOff		= TRAN_ALL_OK;
	TRANS.ProcCount[0]			= ' ';
	TRANS.ProcCount[1]			= NULL;
	TRANS.TransPowerOnOff		= OFF;
	StrAllZero(TRANS.EjrSerialNo);
	TRANS.DeviceExecOff			= DEV_NONE;
	GetDate(TRANS.DeviceYYYYMMDD);
	TRANS.DeviceYYYYMMDD[8]		= NULL;
	TRANS.DeviceSprCnt			= 0;
	TRANS.DeviceJprCnt			= 0;
	TRANS.DeviceMcuCnt			= 0;
	TRANS.DeviceCshCnt			= 0;
	TRANS.DevicePbmCnt			= 0;
	TRANS.DeviceSprRetractCnt	= 0;
	TRANS.DeviceJprRetractCnt	= 0;
	TRANS.DeviceMcuRetractCnt	= 0;
	TRANS.DeviceCshRetractCnt	= 0;
	TRANS.DevicePbmRetractCnt	= 0;
	strcpy(TRANS.AsPassWord, "4545    ");
	strcpy(TRANS.OpPassWord, "00000000");

	PutTransProfile();
}

/////////////////////////////////////////////////////////////////////////////
//	Get Device Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetDeviceProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	DEVICE.MachineType   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.MachineKind   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SprHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.JprHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.McuHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.CashHandler   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.PBHandler     = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.KeyHandler    = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SoundDevice   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SoundVolume   = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.CameraDevice  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ICCardDevice  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.PowerHandler  = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.SplDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.VfdDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.DualDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.DvrDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.OpDevice		 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.McuDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.MwDevice		 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.NmsDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.EjrDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.EtcComKind	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.RicDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.RicComPort	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.IrdDevice	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.IrdComPort	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice14	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice15	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice16	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice17	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice18	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice19	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice20	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice21	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice22	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice23	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice24	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice25	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice26	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice27	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice28	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice29	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice30	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);
	DEVICE.ETCDevice31	 = GetPInt(DEV_SEC, DEVKEY[i++].name, 0, _DEVICE_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get Network Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetNetworkProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmSerialNum,	sizeof(NETWORK.AtmSerialNum),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BranchNum,		sizeof(NETWORK.BranchNum),      _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.SubBranchNum,	sizeof(NETWORK.SubBranchNum),   _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.RoomNum,		sizeof(NETWORK.RoomNum),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.MachineMakeDate,sizeof(NETWORK.MachineMakeDate),_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.MachineMakeNum,	sizeof(NETWORK.MachineMakeNum),	_NETWORK_INI);
	NETWORK.BranchInOut		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.AutoBranchOnOff	= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.All24OnOff		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.All365OnOff		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);

	NETWORK.Interface		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.InterfaceKind	= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress,	sizeof(NETWORK.BpIpAddress),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo,		sizeof(NETWORK.BpPortNo),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress2,	sizeof(NETWORK.BpIpAddress2),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo2,		sizeof(NETWORK.BpPortNo2),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress3,	sizeof(NETWORK.BpIpAddress3),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo3,		sizeof(NETWORK.BpPortNo3),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpIpAddress4,	sizeof(NETWORK.BpIpAddress4),	_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.BpPortNo4,		sizeof(NETWORK.BpPortNo4),		_NETWORK_INI);
    GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmIPAddress,	sizeof(NETWORK.AtmIPAddress),	_NETWORK_INI);
    GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmSubnetMask,	sizeof(NETWORK.AtmSubnetMask),	_NETWORK_INI);
    GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmGateWay,		sizeof(NETWORK.AtmGateWay),		_NETWORK_INI);
	GetPStr(NET_SEC, NETKEY[i++].name, '0', NETWORK.AtmPortNo,		sizeof(NETWORK.AtmPortNo),		_NETWORK_INI);

	GetPByten(NET_SEC, NETKEY[i++].name, &NETWORK.DeviceAddr, sizeof(NETWORK.DeviceAddr), _NETWORK_INI);
	GetPByten(NET_SEC, NETKEY[i++].name, &NETWORK.SelectAddr, sizeof(NETWORK.SelectAddr), _NETWORK_INI);
	NETWORK.SendRecvSize	= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.LineSpeed		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.PollCheck  		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Point			= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Duplex			= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.TimeFiller		= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.RsType			= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Nrz				= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);
	NETWORK.Clk				= GetPInt(NET_SEC, NETKEY[i++].name, 0, _NETWORK_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Get Trans Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::GetTransProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	GetPStr(TRANS_SEC, TRANKEY[i++].name, '?', TRANS.ApVersion,		sizeof(TRANS.ApVersion),		_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.YYYYMMDD,		sizeof(TRANS.YYYYMMDD),			_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.SerialNo,		sizeof(TRANS.SerialNo),			_TRANS_INI);
	GetPByten(TRANS_SEC, TRANKEY[i++].name, &TRANS.ClerkTransOnOff, sizeof(TRANS.ClerkTransOnOff),	_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, ' ', TRANS.ProcCount,		sizeof(TRANS.ProcCount),		_TRANS_INI);
	TRANS.TransPowerOnOff = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,								_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.EjrSerialNo,	sizeof(TRANS.EjrSerialNo),		_TRANS_INI);
	TRANS.DeviceExecOff = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, '0', TRANS.DeviceYYYYMMDD,sizeof(TRANS.DeviceYYYYMMDD),	_TRANS_INI);
	TRANS.DeviceSprCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceJprCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceMcuCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceCshCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DevicePbmCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.DeviceSprRetractCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,							_TRANS_INI);
	TRANS.DeviceJprRetractCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,							_TRANS_INI);
	TRANS.DeviceMcuRetractCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,							_TRANS_INI);
	TRANS.DeviceCshRetractCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,							_TRANS_INI);
	TRANS.DevicePbmRetractCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,							_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, ' ', TRANS.AsPassWord,sizeof(TRANS.AsPassWord),			_TRANS_INI);
	GetPStr(TRANS_SEC, TRANKEY[i++].name, ' ', TRANS.OpPassWord,sizeof(TRANS.OpPassWord),			_TRANS_INI);
	// added by yusy 08.09.09
	TRANS.SPRPrintCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.JPRPrintCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);
	TRANS.INKPrintCnt = GetPInt(TRANS_SEC, TRANKEY[i++].name, 0,									_TRANS_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put Device Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutDeviceProfile()
{
	int	i=0;

	EnterCriticalSection(&m_csLock);

	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.MachineType,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.MachineKind,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SprHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.JprHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.McuHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.CashHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.PBHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.KeyHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SoundDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SoundVolume,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.CameraDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ICCardDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.PowerHandler,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.SplDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.VfdDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.DualDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.DvrDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.OpDevice,			_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.McuDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.MwDevice,			_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.NmsDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.EjrDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.EtcComKind,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.RicDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.RicComPort,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.IrdDevice,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.IrdComPort,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice14,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice15,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice16,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice17,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice18,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice19,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice20,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice21,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice22,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice23,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice24,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice25,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice26,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice27,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice28,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice29,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice30,		_DEVICE_INI);
	PutPInt(DEV_SEC, DEVKEY[i++].name, DEVICE.ETCDevice31,		_DEVICE_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put Network Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutNetworkProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmSerialNum,	sizeof(NETWORK.AtmSerialNum),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BranchNum,		sizeof(NETWORK.BranchNum),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.SubBranchNum,	sizeof(NETWORK.SubBranchNum),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.RoomNum,			sizeof(NETWORK.RoomNum),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.MachineMakeDate,	sizeof(NETWORK.MachineMakeDate),_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.MachineMakeNum,	sizeof(NETWORK.MachineMakeNum),	_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.BranchInOut,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.AutoBranchOnOff,	_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.All24OnOff,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.All365OnOff,		_NETWORK_INI);

	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Interface,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.InterfaceKind,	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress,		sizeof(NETWORK.BpIpAddress),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo,		sizeof(NETWORK.BpPortNo),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress2,	sizeof(NETWORK.BpIpAddress2),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo2,		sizeof(NETWORK.BpPortNo2),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress3,	sizeof(NETWORK.BpIpAddress3),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo3,		sizeof(NETWORK.BpPortNo3),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpIpAddress4,	sizeof(NETWORK.BpIpAddress4),	_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.BpPortNo4,		sizeof(NETWORK.BpPortNo4),		_NETWORK_INI);
    PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmIPAddress,	sizeof(NETWORK.AtmIPAddress),	_NETWORK_INI);
    PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmSubnetMask,	sizeof(NETWORK.AtmSubnetMask),	_NETWORK_INI);
    PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmGateWay,		sizeof(NETWORK.AtmGateWay),		_NETWORK_INI);
	PutPStr(NET_SEC, NETKEY[i++].name, NETWORK.AtmPortNo,		sizeof(NETWORK.AtmPortNo),		_NETWORK_INI);

	PutPByten(NET_SEC, NETKEY[i++].name, &NETWORK.DeviceAddr,	sizeof(NETWORK.DeviceAddr),		_NETWORK_INI);
	PutPByten(NET_SEC, NETKEY[i++].name, &NETWORK.SelectAddr,	sizeof(NETWORK.SelectAddr),		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.SendRecvSize,	_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.LineSpeed,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.PollCheck,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Point,			_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Duplex,			_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.TimeFiller,		_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.RsType,			_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Nrz,				_NETWORK_INI);
	PutPInt(NET_SEC, NETKEY[i++].name, NETWORK.Clk,				_NETWORK_INI);

	LeaveCriticalSection(&m_csLock);
}

/////////////////////////////////////////////////////////////////////////////
//	Put Trans Profile
/////////////////////////////////////////////////////////////////////////////
void CProfile::PutTransProfile()
{
	int		i=0;

	EnterCriticalSection(&m_csLock);

	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.ApVersion,			sizeof(TRANS.ApVersion),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.YYYYMMDD,			sizeof(TRANS.YYYYMMDD),			_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.SerialNo,			sizeof(TRANS.SerialNo),			_TRANS_INI);
	PutPByten(TRANS_SEC, TRANKEY[i++].name, &TRANS.ClerkTransOnOff, sizeof(TRANS.ClerkTransOnOff),	_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.ProcCount,			sizeof(TRANS.ProcCount),		_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.TransPowerOnOff,									_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.EjrSerialNo,		sizeof(TRANS.EjrSerialNo),		_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceExecOff,										_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceYYYYMMDD,		sizeof(TRANS.DeviceYYYYMMDD),	_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceSprCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceJprCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceMcuCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceCshCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DevicePbmCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceSprRetractCnt,								_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceJprRetractCnt,								_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceMcuRetractCnt,								_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DeviceCshRetractCnt,								_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.DevicePbmRetractCnt,								_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.AsPassWord,			sizeof(TRANS.AsPassWord),		_TRANS_INI);
	PutPStr(TRANS_SEC, TRANKEY[i++].name, TRANS.OpPassWord,			sizeof(TRANS.OpPassWord),		_TRANS_INI);
	// added by yusy 08.09.09
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.SPRPrintCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.JPRPrintCnt,										_TRANS_INI);
	PutPInt(TRANS_SEC, TRANKEY[i++].name, TRANS.INKPrintCnt,										_TRANS_INI);

	LeaveCriticalSection(&m_csLock);
}
