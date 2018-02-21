/////////////////////////////////////////////////////////////////////////////
//	DevApl.cpp : implementation file
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "direct.h"
#include "shlwapi.h"

#include "..\..\H\Common\CmnLibIn.h"
#include "..\..\H\Common\ConstDef.h"

#include "..\..\H\Dll\DevDefine.h"
#include "..\..\H\Dll\Profile.h"
#include "..\..\H\Dll\Network.h"

#include "..\..\H\Dll\DevEtc.h"
#include "..\..\H\Dll\DevCmn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	BASIC FUNCTION(APL) : APL SET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	BASIC FUNCTION(APL) : APL GET MODE
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//	BASIC FUNCTION(APL) : APL OPERATION MODE
/////////////////////////////////////////////////////////////////////////////
// Initialize DS(Variable)
int CDevCmn::fnAPL_InitializeDS()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_InitializeDS()");
																// Common Version Setting : 2007.11.01
	RegSetStr(_REGKEY_MWI, "VERSION", RegGetStr(_REGKEY_MWI, "VERSION", "V00.00.00") + "(" + COMMON_VERSION + ")");
	fnAPL_ClerkInitModeSet();									// Clerk Initialize Mode Set
	RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_REBOOT, GetDate());

	_mkdir(_EJR_DIR);											// 2007.08.03
	_mkdir(_EJR_TXT);
	_mkdir(_EJR_IMG);
	_mkdir(_EJR_SEND);
	_mkdir(_EJR_FTP);
	_mkdir(_EJR_DCNMS);
	_mkdir(_EJR_HXNMS);
	_mkdir(_EJR_TOTALDETAIL);
	_mkdir(_EJR_JNL);
	_mkdir(_EJR_JNL_BACKUP);

	_mkdir(_AP_TRACE_DIR);
	_mkdir(_AP_UPDATE_DIR);
	_mkdir(_AP_TEMP_DIR);
	_mkdir(_AP_TEMPFTP_DIR);
	_mkdir(_EJR_JPRDATA);
	_mkdir(_EJR_CUPJNEJR);
	
	DeleteFiles(_EJR_SEND, 7);									// Ejr File Delete(7 Day Standard) : 2004.06.22
	DeleteFiles(_EJR_FTP, 84);									// Ejr Ftp File Delete(12 Week Standard)
	DeleteFiles(_EJR_DCNMS, 180);								// DC Nms Ejr File Delete(180 Day Standard)
	DeleteFiles(_EJR_HXNMS, 180);								// HX Nms Ejr File Delete(180 Day Standard)
	DeleteFiles(_EJR_TOTALDETAIL, 180);							// Total Detail Ejr File Delete(180 Day Standard)
	DeleteFiles(_AP_TRACE_DIR, 180);								// Trace File Delete(4 Week Standard)
	DeleteFiles(_AP_TEMP_DIR, 7);								// Temp File Delete(7 Day Standard)
	DeleteFiles(_AP_TEMPFTP_DIR, 7);							// Temp Ftp File Delete(7 Day Standard)
	DeleteFiles(_EJR_DETAILSEND, 365);							// 删除上传的交易明细和流水纸内容文件，保存180天  by zjd
	fnAPL_EjrCheckFileSize();
	DeleteFiles(_EJR_JNL_BACKUP, 365);							// Backup Journal File Delete(180 Day Standard)
	DeleteFiles(_EJR_JPRDATA, 180);								// Journal File Delete(180 Day Standard)
	DeleteFiles(_EJR_CUPJNEJR, 180);							// Cup(JN) Journal File Delete(180 Day Standard)
	

	switch (AtmDefine.MwDevice)									// 2007.06.06
	{
		case MW_KAL:
			break;

		case MW_NEXTWARE:
		case MW_KALWOSA:
		case MW_NEXTWOSA:
			if (RegGetStr(_REGKEY_SOFTXFSCDU, "ConvPCU2LCU", "0") == "0")
			{
				DeleteFile("C:\\T1ATM\\INI\\CdmInfo.ini");
				DeleteFile("C:\\T1ATM\\INI\\CstInfo.ini");
				DeleteFile("C:\\T1ATM\\INI\\DispInfo.ini");
				DeleteFile("C:\\T1ATM\\INI\\CdmInfo.tmp");
				DeleteFile("C:\\T1ATM\\INI\\CstInfo.tmp");
				DeleteFile("C:\\T1ATM\\INI\\DispInfo.tmp");

				DeleteFile("C:\\T1ATM\\INI\\CdmInfo30.ini");
				DeleteFile("C:\\T1ATM\\INI\\CstInfo30.ini");
				DeleteFile("C:\\T1ATM\\INI\\DispInfo30.ini");
				DeleteFile("C:\\T1ATM\\INI\\CdmInfo30.tmp");
				DeleteFile("C:\\T1ATM\\INI\\CstInfo30.tmp");
				DeleteFile("C:\\T1ATM\\INI\\DispInfo30.tmp");
			}
			break;
	}

	// Default Operation Setting
	hADComm				= INVALID_HANDLE_VALUE;                 // 广告机串口设备句柄 2008.05.09
	IdleADFlag			= 0;									// 空闲广告播放标志					
	InitializeCriticalSection(&ADMutex);						// 广告机写入串口锁

	HostLineMode		= HOST_ONLINE_MODE;						// Oline/Local
	CashCycleMode		= CSH_RECYCLE_MODE;						// Recycle/Uncycle
	CashCashMode		= CSH_REALCASH_MODE;					// RealCash/TestCash
																// 2005.11.11
	CashMaxCashOutCnt	= CSH_MAXCASHOUTCNT;					// Cash Max Cash Out Count
	CashFixCashCnt		= CSH_FIXCASHCNT;						// Cash Fix Cash Count
	CashEmptyCashCnt	= CSH_EMPTYCASHCNT;						// Cash Empty Cash Count
																// 2006.08.22
	ApDeviceLangMode	= CHN_MODE;								// Ap Device Language Mode
	OpDeviceLangMode	= CHN_MODE;								// Op Device Language Mode

	// SP Setting Section
	fnSPR_SetPrintImageMode(TRUE);								// ALL : 2007.05.07

	RegSetStr(_REGKEY_SPRSTATUS, "EnRetractBin", "0");
	RegSetInt(_REGKEY_SPRSTATUS, "MaxRetractBin", 100);

// 	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "SPRJAMSENSOR", 1) == 1)
// 		RegSetStr(_REGKEY_SPRSTATUS, "JamSensor", "1");
// 	else
// 		RegSetStr(_REGKEY_SPRSTATUS, "JamSensor", "0");			// 2009.03.25 SPR卡纸传感器设置


	RegSetStr(_REGKEY_SOFTXFSMANAGER, "MaximumTraceFileSize", "10000000");	// added by yusy
	
	RegSetStr(_REGKEY_SPRSTATUS, "LOWSensor", "1");
	RegSetStr(_REGKEY_SPRSTATUS, "ImageFileName", "C:\\\\T1ATM\\\\APP\\\\FORMS\\\\SPRFORM.BMP");
	RegSetStr(_REGKEY_SPRSTATUS, "PrintImageName1", "C:\\\\T1ATM\\\\APP\\\\FORMS\\\\SPRFORM.BMP");
	RegSetStr(_REGKEY_SPRSTATUS, "PrintImageName2", "C:\\\\T1ATM\\\\APP\\\\FORMS\\\\SPRFORM2.BMP");
	RegSetStr(_REGKEY_SPRSTATUS, "ImageNumber", "1");			// 2009.02.12
	RegSetStr(_REGKEY_SPRSTATUS, "SelectBackGround", "1");		// 2009.02.12
	RegSetInt(_REGKEY_SPRSTATUS, "FontSize", 14);				// 2008.06.24
	RegSetInt(_REGKEY_SPRSTATUS, "MultibyteCharSet", 0);		// 2008.08.05
	RegSetInt(_REGKEY_SPRSTATUS, "EPSupportMultibyte", 0);		// 2008.08.05
	RegSetStr(_REGKEY_SPRSTATUS, "TotalColumn", "40");			// 2008.08.05
	RegSetStr(_REGKEY_SPRSTATUS, "TopMargin", "-50");			// 2008.08.05
	RegSetStr(_REGKEY_SPRSTATUS, "CutMargin", "160");			// 2008.08.05
	RegSetStr(_REGKEY_SPRSTATUS, "LeftMargin", "0");			// 2008.08.05
	RegSetStr(_REGKEY_SPRSTATUS, "SetConf", "1");				// 2009.11.09
	RegSetInt(_REGKEY_SPRSTATUS, "InitAtReset", 1);				// 2010.04.01
	RegSetInt(_REGKEY_SPRSTATUS, "EjectAtClose", 0);			// 2011.09.23
	RegSetInt(_REGKEY_SPRSTATUS, "EPDownloadRetryCount", 3);	// 2011.10.11
	RegSetStr(_REGKEY_SPRSTATUS, "EPDownloadDir", "C:\\Program Files\\Nextware\\Exe");
	RegSetInt(_REGKEY_SPRSTATUS, "EPDownloadOption", 1);		// 2011.10.11
	RegSetInt(_REGKEY_SPRSTATUS, "IsAsyncEPDownload", 0);		// 2011.10.11
	RegSetInt(_REGKEY_SPRCAPSSTATUS, "CAP_UNICODE", 0);
	RegSetInt(_REGKEY_SPRCAPSSTATUS, "CAP_MEDIATAKEN", 1);
	RegSetStr(_REGKEY_SPRDEVSTATUS, "PAPEROUT", "ONLINE");		// 2008.06.24
	RegSetStr(_REGKEY_SPRDEVSTATUS, "MEDIANOTPRESENT", "ONLINE");
																// 2008.06.24
	RegSetStr(_REGKEY_SPRDEVSTATUS, "MEDIANOTPRESENT TO PAPEROUT", "YES");
																// 2008.08.05
	RegSetStr(_REGKEY_SOFTXFSSPR, "FormDirectory", "C:\\T1ATM\\APP\\FORMS\\");
	RegSetStr(_REGKEY_SOFTXFSSPR, "FormExtension", "WFM");
	RegSetStr(_REGKEY_SOFTXFSSPR, "MediaDirectory", "C:\\T1ATM\\APP\\FORMS\\");
	RegSetStr(_REGKEY_SOFTXFSSPR, "MediaExtension", "WFM");

	RegSetStr(_REGKEY_JPRDEVSTATUS, "PAPEROUT", "ONLINE");		// 2008.06.24
	RegSetStr(_REGKEY_JPRDEVSTATUS, "MEDIANOTPRESENT", "ONLINE");
																// 2008.06.24
	RegSetStr(_REGKEY_SOFTXFSJPR, "FormDirectory", "C:\\T1ATM\\APP\\FORMS\\");
	RegSetStr(_REGKEY_SOFTXFSJPR, "FormExtension", "WFM");
	RegSetStr(_REGKEY_SOFTXFSJPR, "MediaDirectory", "C:\\T1ATM\\APP\\FORMS\\");
	RegSetStr(_REGKEY_SOFTXFSJPR, "MediaExtension", "WFM");

	RegSetInt(_REGKEY_MCUSTATUS, "RetainFull_HWError", 1);
	RegSetInt(_REGKEY_MCUSTATUS, "RetainStatus_HWError", 0);
	RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);			// 2008.01.11

	if (IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, "FluxSensor", 0) == 1)
		RegSetInt(_REGKEY_MCUSTATUS, "FluxSensorProgrammable", 1);
	else
		RegSetInt(_REGKEY_MCUSTATUS, "FluxSensorProgrammable", 0);

	RegSetInt(_REGKEY_MCUSTATUS, "EnablePreHeadForChip", 1);	// 2011.01.18
	RegSetInt(_REGKEY_MCUSTATUS, "MSEncryption", 0);			// 2011.01.13
	RegSetInt(_REGKEY_MCUSTATUS, "EPDownloadRetryCount", 3);	// 2011.01.13
	RegSetStr(_REGKEY_MCUSTATUS, "EPDownloadDir", "C:\\Program Files\\Nextware\\Exe");
	RegSetInt(_REGKEY_MCUSTATUS, "EPDownloadOption", 1);		// 2011.01.13
	RegSetInt(_REGKEY_MCUSTATUS, "SupportAircard", 0);			// 2011.01.13
	RegSetInt(_REGKEY_MCUSTATUS, "PowerDownEject", 0);			// 2011.01.13
	RegSetInt(_REGKEY_MCUSTATUS, "ForceRemoveError", 1);		// 2011.08.28
	RegSetInt(_REGKEY_MCUSTATUS, "ReportStatusChangeInIdle", 1);// 2011.08.28

	RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "1");	// G-CDU STANDARD : 2007.05.07
																// 2010.01.06 (0->1)
	RegSetStr(_REGKEY_CDUSTATUS, "AutoReject", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "BillsMinimum", Int2Asc(CSH_NEARCASHCNT));
	RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "CSTMinimum4Mix6", Int2Asc(CashEmptyCashCnt));
	RegSetStr(_REGKEY_CDUSTATUS, "ClearNoteCount", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "DetectShutterNote", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "EmptyEmulation", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "ErrorCode7D", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "Initialize", "0");			// 2009.03.05
	RegSetStr(_REGKEY_CDUSTATUS, "INOPRetry", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "INOPEmulation", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "IntermediateStacker", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "MakeErrorShutterControl", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "MaxBills", Int2Asc(CSH_MAXCASHOUTCNT2));
																// 2010.03.12(40->50)
	RegSetStr(_REGKEY_CDUSTATUS, "MisfeedRetry", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "NoteCounting", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "PartialDispense", "0");
	if (Asc2Int(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2))) > 300)
		IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", "300");
	else
	if (Asc2Int(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2))) < 120)
		IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", "120");
	RegSetStr(_REGKEY_CDUSTATUS, "RejectMaximum", IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2)));
	RegSetStr(_REGKEY_CDUSTATUS, "RetractOnInit", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "Retract4China", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "Shutter", "1");
	RegSetStr(_REGKEY_CDUSTATUS, "TestDispense", "R");
	RegSetStr(_REGKEY_CDUSTATUS, "ReserveOption", "0");
	RegSetStr(_REGKEY_CDUSTATUS, "Log4TransDir", "");			// 2007.06.06
	RegSetStr(_REGKEY_CDUSTATUS, "TraceDir", "D:\\TRACE\\");	// 2007.08.29
	RegSetStr(_REGKEY_CDUSTATUS, "LogBackupSet", "0");			// 2007.12.11
	RegSetInt(_REGKEY_CDUSTATUS, "KeepINOPStatus", 1);			// 2011.02.22 (for CDUM)
	RegSetInt(_REGKEY_CDUSTATUS, "EPDownloadRetryCount", 3);	// 2011.10.11
	RegSetStr(_REGKEY_CDUSTATUS, "EPDownloadDir", "C:\\Program Files\\Nextware\\Exe");
	RegSetInt(_REGKEY_CDUSTATUS, "EPDownloadOption", 1);		// 2011.10.11
																// 2008.07.24
	if (m_pProfile->NETWORK.InterfaceKind == 11)				// 11:CUP(BCTC)
		RegSetStr(_REGKEY_CDUSTATUS, "ExtraDataRead", "1");		// "1" = DATA READ + NO EP LOG
	else
		RegSetStr(_REGKEY_CDUSTATUS, "ExtraDataRead", "3");		// "3" = DATA READ + EP LOG

	RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");			// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20

	RegSetStr(_REGKEY_PINSTATUS, "LoadKeyPath", "C:\\Program Files\\Nextware\\Exe\\LoadKeyInfo.ini");
	RegSetInt(_REGKEY_PINSTATUS, "EnterKeyFlag", 1);
	RegSetInt(_REGKEY_PINSTATUS, "UseFDKeys", 255);
	RegSetInt(_REGKEY_PINSTATUS, "BlockRetryCount", 2);			// 2008.01.11
	RegSetInt(_REGKEY_PINSTATUS, "IsComMonitor", 1);			// 2009.11.04
	RegSetInt(_REGKEY_PINSTATUS, "AdjustCustomerData", 1);		// 2010.06.03
	RegSetInt(_REGKEY_PINSTATUS, "MacByKeyLength", 1);			// 2010.09.02
	RegSetInt(_REGKEY_PINSTATUS, "EPDownloadRetryCount", 3);	// 2011.10.11
	RegSetStr(_REGKEY_PINSTATUS, "EPDownloadDir", "C:\\Program Files\\Nextware\\Exe");
	RegSetInt(_REGKEY_PINSTATUS, "EPDownloadOption", 1);		// 2011.10.11
	RegSetStr(_REGKEY_KEYMGRSTATUS, "SingleKeyName", "SMASTERKEY");
	RegSetStr(_REGKEY_KEYMGRSTATUS, "TripleKeyName", "TMASTERKEY");
	RegSetStr(_REGKEY_JPRSTATUS, "PrinterCodePage", "950");		// 2010.01.21
	RegSetInt(_REGKEY_JPRSTATUS, "CntPrintLine", 20);
	RegSetInt(_REGKEY_JPRSTATUS, "Timeout4PrintEnd", 30);
	RegSetInt(_REGKEY_JPRSTATUS, "LineSpace", 18);				// 2011.03.07
	
	RegSetInt(_REGKEY_SPCONFIGTXTLOG, "DaysToLeft", 0);			// 2011.08.01
	RegSetInt(_REGKEY_SPCONFIGTXTLOG, "MaxLogSize", 10);
	RegSetInt(_REGKEY_SPCONFIGTXTLOG, "NoOfBackup", 10);
	RegSetStr(_REGKEY_SIUSTATUS, "TxtlogPath", " ");
	RegSetStr(_REGKEY_SPRSTATUS, "TxtlogPath", " ");
	RegSetInt(_REGKEY_NEXTWARE, "DaysToLeft", Asc2Int(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCNWLOGSAVEDAY, "30")));
	RegSetInt(_REGKEY_NEXTWARE, "MaximumTraceFileSpace", Asc2Int(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCNWLOGSPACE, "10")));
	RegSetInt(_REGKEY_NEXTWARE, "Compress", 1);
	RegSetStr(_REGKEY_NEXTWARE, "TraceDirectory", "D:\\TRACE_NEXTWARE");
	RegSetStr(_REGKEY_NEXTWARE, "InstallDir", "C:\\Program Files\\Nextware\\");
	
	switch (AtmDefine.KeyHandler)								// 2007.12.11
	{
		case KEYMGR_EPP:
		case SINGLE_EPP:
		case TRIPLE_EPP:
			RegSetInt(_REGKEY_PINSTATUS, "UsePadCharNDCMAC", 0);
			RegSetInt(_REGKEY_PINSTATUS, "UseNDCMAC", 0);
			break;

		case TRIPLE_NDC:
			RegSetInt(_REGKEY_PINSTATUS, "UsePadCharNDCMAC", 0);
			RegSetInt(_REGKEY_PINSTATUS, "UseNDCMAC", 1);
			break;
	}
			
	RegSetInt(_REGKEY_CAMSTATUS, "MaxPictureCount", 65535);
	RegSetInt(_REGKEY_CAMSTATUS, "PictureCount", 0);

	RegSetStr(_REGKEY_TTUSTATUS, "CommType", "SERIAL");
	RegSetInt(_REGKEY_TTUSTATUS, "FDKSupport", 0);
	RegSetInt(_REGKEY_TTUSTATUS, "MaxColumn", 40);
	RegSetInt(_REGKEY_TTUSTATUS, "Sound", 1);
	RegSetInt(_REGKEY_TTUSTATUS, "UserType", 4);
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F1", "A");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F2", "B");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F3", "C");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F4", "D");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F5", "E");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F6", "F");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F7", "G");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "F8", "H");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "LEFT", "I");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "RIGHT", "J");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "AUX", "K");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "RESET", "L");
	RegSetStr(_REGKEY_TTUKEYMAPPING, "CLEAR", "BACKSPACE");

	RegSetInt(_REGKEY_VDMSTATUS, "AlwaysTop", 0);				// 2009.07.28
	RegSetStr(_REGKEY_VDMSTATUS, "OptionOfExit", "POWEROFF");
	RegSetStr(_REGKEY_VDMSTATUS, "VdmPath", "C:\\T1ATM\\NHVdm");

	RegSetStr(_REGKEY_DCPEMULATOR, "ExecutePath", "C:\\Program Files\\Kalignite\\Dll\\");
	RegSetStr(_REGKEY_WATCHDOG, "TracePath", "D:\\\\Trace");

	if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCTAMPERFLAG, "0") == "1")
	{
		RegSetInt(_REGKEY_SIUSTATUS, "NHAntiskimming", 1);
		RegSetInt(_REGKEY_SIUSTATUS, "WaitingTimeOption", 30);
		RegSetStr(_REGKEY_SIUCAPSSTATUS, "CAP_SEN_INTTAMPER", "YES");
		RegSetInt(_REGKEY_MCUSTATUS, "SIUAntiskimming ", 0);
	}
	
	switch (AtmDefine.MachineKind)								// MACHINE KIND : 2007.05.07
	{
		case NANOCASH:
		case MCD3:
		case TTW:
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "2");
			break;
		case MCD4:
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);	// 2005.03.08

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 0);

			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MCD4");		// 2007.04.26
			break;
		case TTW2:
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);	// 2005.03.08

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 0);

			switch (AtmDefine.OpDevice)
			{
				case NOT_SET:
					break;
				case OP_TOUCH:
					RegSetStr(_REGKEY_VDMSTATUS, "Name", "TTW");
					break;
				case OP_TSPL:
					RegSetStr(_REGKEY_VDMSTATUS, "Name", "TTW2TSPL");
					break;
				case OP_TSPLCHN:
					RegSetStr(_REGKEY_VDMSTATUS, "Name", "TTW2TSPL");
					break;
				case OP_HSPL:
					break;
			}
			break;
		case EATMGBA1:											// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 0);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 1);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 0);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);

			switch (AtmDefine.CashHandler)
			{
				case BRM_SET:									// BRM
					break;
				case CDU_SHUTTER1510_SET:						// NANO-CDU
					break;
				case CDU_SHUTTER20_SET:							// CDU-M
				case CDU_SHUTTER30_SET:
					RegSetStr(_REGKEY_VDMSTATUS, "Name", "MB2500N");
					break;
				case CDU_RETRACT20_SET:							// TS-CDU
				case CDU_RETRACT30_SET:
					break;
				case CDU_PRESENT20_SET:							// TTW-CDU
				case CDU_PRESENT30_SET:
					break;
				case CDU_GPRESENT20_SET:						// G-CDU
				case CDU_GPRESENT30_SET:
					RegSetStr(_REGKEY_VDMSTATUS, "Name", "EATMGB");
					break;
				case CDU_TRAY20_SET:							// TRAY-CDU-M
				case CDU_TRAY30_SET:
					break;
			}
			break;
		case EATMULL1:											// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 0);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 1);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 0);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 0);

			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MB2500N");	// 2007.04.26
			break;
		case EATMTTW:											// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 0);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 1);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 0);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);

			RegSetStr(_REGKEY_VDMSTATUS, "Name", "EATMTTW");	// 2007.04.26
			break;
		case TTW2E:												// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 0);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 1);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 0);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);
			
			RegSetStr(_REGKEY_VDMSTATUS, "Name", "TTW3");		// 2007.04.26
			break;
		case MCD4E:												// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 2);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 0);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 2);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);
			
			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MCD4_E");		// 2007.04.26
			break;
		case MX8000T:											// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 2);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 0);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 2);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);
			
			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MX8000T");	// 2007.04.26
			
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNumber", 0);	// 2008.04.10
			RegSetInt(_REGKEY_VDMSTATUS, "IsKillCRMProcess", 0);// 2010.01.12
			RegSetStr(_REGKEY_SIUCAPSSTATUS, "CAP_INDI_OPENCLOSE", "YES");
			RegSetInt(_REGKEY_SIUCONFSTATUS, "INDI_OPENCLOSE", 40);
			break;
		case MX8000L:											// 2005.11.11
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 2);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 0);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 2);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "SERIAL");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);
			
			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MX8000L");	// 2007.04.26
			
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNumber", 0);	// 2008.04.10
			RegSetInt(_REGKEY_VDMSTATUS, "IsKillCRMProcess", 0);// 2010.01.12
			RegSetStr(_REGKEY_SIUCAPSSTATUS, "CAP_INDI_OPENCLOSE", "YES");
			RegSetInt(_REGKEY_SIUCONFSTATUS, "INDI_OPENCLOSE", 40);
			break;
		case MX5600T:
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetStr(_REGKEY_SPRSTATUS, "TopMargin", "-24");	// 2010.05.14
			RegSetStr(_REGKEY_SPRSTATUS, "CutMargin", "188");
			RegSetInt(_REGKEY_SPRSTATUS, "FontHeight", 32);
			RegSetInt(_REGKEY_SPRDEFAULTFONT, "PointSize", 10);
			RegSetStr(_REGKEY_SPRDEFAULTFONT, "Font", "SIMSUN");
	
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 2);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 0);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 2);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "USB");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);
			
			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MX5600T");	// 2009.08.25
			RegSetInt(_REGKEY_GCDUUSB_DRIVER, "dwBulkTimeout", 0x0001d4c0);
			RegSetStr(_REGKEY_CDUSTATUS, "ExtraDataRead", "1");	// "1" = DATA READ + EP LOG
			break;
		case MX5600L:
			RegSetStr(_REGKEY_SPRSTATUS, "Initialize", "0");
			RegSetStr(_REGKEY_SPRSTATUS, "TopMargin", "-24");	// 2010.05.14
			RegSetStr(_REGKEY_SPRSTATUS, "CutMargin", "188");
			RegSetInt(_REGKEY_SPRSTATUS, "FontHeight", 32);
			RegSetInt(_REGKEY_SPRDEFAULTFONT, "PointSize", 10);
			RegSetInt(_REGKEY_SPRDEFAULTFONT, "FontSize_Portrait", 10);
			RegSetStr(_REGKEY_SPRDEFAULTFONT, "Font", "SIMSUN");
			RegSetStr(_REGKEY_SPRDEFAULTFONT, "FontStyle", "NORMAL");
	
			RegSetInt(_REGKEY_UPSSTATUS, "UserPowerOffType", 2);// 2008.10.28
			RegSetInt(_REGKEY_UPSSTATUS, "ACPowerOffType", 0);
			RegSetInt(_REGKEY_UPSSTATUS, "SWPowerOffType", 2);

			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			RegSetStr(_REGKEY_VDMSTATUS, "Type", "USB");
			RegSetInt(_REGKEY_VDMSTATUS, "CduOption", 1);
			
			RegSetStr(_REGKEY_VDMSTATUS, "Name", "MX5600L");	// 2009.08.25
			RegSetInt(_REGKEY_GCDUUSB_DRIVER, "dwBulkTimeout", 0x0001d4c0);
			RegSetStr(_REGKEY_CDUSTATUS, "ExtraDataRead", "1");	// "1" = DATA READ + EP LOG
			break;
	}

	switch (AtmDefine.MwDevice)									// WOSA : 2007.04.18
	{
		case MW_KAL:
			RegSetStr(_REGKEY_CDUSTATUS, "NoteCounting", "1");

			RegSetInt(_REGKEY_PINSTATUS, "IsWOSAEncryption", 0);
			break;

		case MW_NEXTWARE:
			RegSetStr(_REGKEY_SOFTXFSCDU, "ConvPCU2LCU", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "DispType", "0");
			RegSetStr(_REGKEY_SOFTXFSCDU, "CheckDenomination", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "CheckRBNotes", "0");
			RegSetStr(_REGKEY_SOFTXFSCDU, "LCUSortOption", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "LCUComposeType", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "LCUCountMgmt", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "PCUCountUse", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "RetractCountOn", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "NoteCounting", "0");

			RegSetInt(_REGKEY_PINSTATUS, "IsWOSAEncryption", 0);

			RegSetInt(_REGKEY_KEYMGRSTATUS, "BtnCorrection", 53);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsNDC", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "DistanceFromBottom", 5);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "XFSVersion", 1);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "CommKind", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsComMonitor", 1);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "AlwaysTop", 1);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "TraceLevel", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsWriteLog", 0);
			break;

		case MW_KALWOSA:
		case MW_NEXTWOSA:
			RegSetStr(_REGKEY_SOFTXFSCDU, "ConvPCU2LCU", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "DispType", "0");
			RegSetStr(_REGKEY_SOFTXFSCDU, "CheckDenomination", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "CheckRBNotes", "0");
			RegSetStr(_REGKEY_SOFTXFSCDU, "LCUSortOption", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "LCUComposeType", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "LCUCountMgmt", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "PCUCountUse", "1");
			RegSetStr(_REGKEY_SOFTXFSCDU, "RetractCountOn", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "NoteCounting", "0");

			RegSetInt(_REGKEY_PINSTATUS, "IsWOSAEncryption", 1);

			RegSetInt(_REGKEY_KEYMGRSTATUS, "BtnCorrection", 53);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsNDC", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "DistanceFromBottom", 5);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "XFSVersion", 1);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "CommKind", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsComMonitor", 1);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "AlwaysTop", 1);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "TraceLevel", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsWriteLog", 0);
			break;
	}
	if (m_pProfile->NETWORK.InterfaceKind == 103)				// 103:CB(TS)
		RegSetStr(_REGKEY_SOFTXFSCDU, "CheckDenomination", "0");// 2011.08.18
				
	switch (AtmDefine.MachineKind)								// MACHINE KIND : 2009.08.25
	{
		case NANOCASH:
		case MCD3:
		case TTW:
			break;
		case MCD4:
			break;
		case TTW2:
			break;
		case EATMGBA1:											// 2005.11.11
			break;
		case EATMULL1:											// 2005.11.11
			break;
		case EATMTTW:											// 2005.11.11
			break;
		case TTW2E:												// 2005.11.11
			break;
		case MCD4E:												// 2005.11.11
			break;
		case MX8000T:											// 2005.11.11
			break;
		case MX8000L:											// 2005.11.11
			break;
		case MX5600T:
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsComMonitor", 0);
			break;
		case MX5600L:
			RegSetInt(_REGKEY_KEYMGRSTATUS, "IsComMonitor", 0);
			break;
	}

	switch (AtmDefine.SprHandler)								// DEVICE : 2007.05.07
	{
		case SPR_T_TYPE:
		case SPR_D_TYPE:
			RegSetStr(_REGKEY_SPRSTATUS, "ImageDownLoadOn", "1");
			RegSetStr(_REGKEY_SPRSTATUS, "PrintOption", "2");
			break;

		case SPR_TF_TYPE:
		case SPR_DF_TYPE:
			RegSetStr(_REGKEY_SPRSTATUS, "ImageDownLoadOn", "0");
			RegSetStr(_REGKEY_SPRSTATUS, "PrintOption", "0");
			break;
	}

	switch (AtmDefine.McuDevice)								// 2008.01.11
	{
		case MCU_M_OMRON:
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 20);
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptTimeOut", 2);
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOut", 25);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptJamRecover", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "JamTimeOut", 25);
			RegSetInt(_REGKEY_MCUSTATUS, "ShutterErrorCount", 5);
			RegSetInt(_REGKEY_MCUSTATUS, "ShutterTimeOut", 10);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);	// 2008.01.11
			break;
		case MCU_M_KDE:
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 20);
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptTimeOut", 2);
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOut", 25);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptJamRecover", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "JamTimeOut", 25);
			RegSetInt(_REGKEY_MCUSTATUS, "ShutterErrorCount", 5);
			RegSetInt(_REGKEY_MCUSTATUS, "ShutterTimeOut", 10);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);	// 2008.01.11
			break;
		case MCU_M_HYOSUNG:
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 20);
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptTimeOut", 2);
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOut", 25);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportLongCardRecovery", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);	// 2008.01.11
			break;												// 2006.04.20
		case MCU_M_SANKYO:
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 30);
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptTimeOut", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOut", 4);	// 2008.11.10
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOutRetry", 3);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);	// 2008.01.11
			break;
		case MCU_D_MAGTEK:
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 0);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);	// 2008.01.11
			break;
		case MCU_M_SANKYO_AS:									// 2008.01.11
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 30);
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "SupportAcceptTimeOut", 1);
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOut", 4);	// 2008.11.10
			RegSetInt(_REGKEY_MCUSTATUS, "AcceptTimeOutRetry", 3);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 1);	// 2009.04.20 抖动入卡
			break;
		default:												// 2006.08.22
			RegSetInt(_REGKEY_MCUSTATUS, "MaxRetractBin", 20);
			RegSetInt(_REGKEY_MCUSTATUS, "AntiSkimming", 0);	// 2008.01.11
			break;
	}

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:											// BRM SEMI SHUTTER(EVB:1,1,1) : 2008.04.10
			RegSetInt(_REGKEY_OKIPSCIMSPCONFIG, "dwShutterCloseMode", 1);
			RegSetInt(_REGKEY_OKIPSCIMSPCONFIG, "dwCashInMode", 1);
			RegSetInt(_REGKEY_OKIKALCASHACCEPTOR, "AllowCancelAtAllTimes", 1);

																// P=L(EVB:0)
			RegSetInt(_REGKEY_OKISPOKICASHRECYCLER, "PhysicalOneToOneMapping", 1);

																// CST MISSING(EVB:1)
			RegSetInt(_REGKEY_OKIPSCIMSPCONFIG, "dwMissingEnable", 1);
			
																// IF EMPTY CHECK THEN ADD CASH(EVB:1)
			RegSetInt(_REGKEY_OKIPSCIMSPCONFIG, "dwEmptyCheck", 0);
			RegSetInt(_REGKEY_OKIPSCDMSPCONFIG, "dwEmptyCheck", 0);

																// LAST ACCEPT STATUS(EVB:1,1)
			RegSetInt(_REGKEY_OKISPOKICASHRECYCLER, "AllowRollbackWhileNoCashInIsActive", 1);
			RegSetInt(_REGKEY_OKISPOKICASHRECYCLER, "CashInStatusIndependentOfCompletion", 1);

																// CST POSITION CHECK(EVB:1,1)
			RegSetInt(_REGKEY_OKIPSCIMCSTCONFIG, "dwCassErrorDisable", 0);
			RegSetInt(_REGKEY_OKIPSCDMPCSTINFO, "dwCassErrorDisable", 0);

																// CDM,CIM SYNC(EVB:Recycler)
			RegSetStr(_REGKEY_OKIKALCASHACCEPTOR, "CompoundDevice", "Recycler");
			RegSetStr(_REGKEY_OKIKALCASHDISPENSER, "CompoundDevice", "Recycler");

																// BRM MAX CASH IN COUNT : 2008.04.10
			RegSetInt(_REGKEY_OKIPSCIMLIMIT, "dwDepMaxBills", Asc2Int(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMMAXCASHINCNT, Int2Asc(CSH_MAXCASHINCNT))));

			RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwAType", 0);	// BRM CST TYPE : 2008.04.10
			RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwBType", 0);
			RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwCType", 0);
			RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwDType", 0);
			if ((RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE).GetLength() != 4)		||
				(!IsNum(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE).GetBuffer(0), 4)))
				RegSetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE, "0000");
			else
			{
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE).Mid(0, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwAType", 1);
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE).Mid(1, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwBType", 1);
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE).Mid(2, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwCType", 1);
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMCSTTYPE).Mid(3, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCDMCSTTYPE, "dwDType", 1);
			}

			RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType4", 1);	// BRM NOTE TYPE : 2008.04.10
			RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType3", 0);
			RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType2", 0);
			RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType1", 0);
			if ((RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).GetLength() != 4)		||
				(!IsNum(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).GetBuffer(0), 4)))
				RegSetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE, "1000");
			else
			{
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(0, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType4", 1);
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(1, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType3", 1);
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(2, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType2", 1);
				if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(3, 1) == "1")
					RegSetInt(_REGKEY_OKIPSCIMNOTETYPE, "dwNoteType1", 1);
			}
			
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "1");
			RegGetInt(_REGKEY_OKIBRMSHUTTER, "OperationRetryCount", 10);
			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "3");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
		case CDU_SHUTTER1510_SET:								// NANO-CDU
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "1");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "500");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
		case CDU_SHUTTER20_SET:									// CDU-M
		case CDU_SHUTTER30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "1");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
		case CDU_RETRACT20_SET:									// TS-CDU
		case CDU_RETRACT30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "1");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
		case CDU_PRESENT20_SET:									// TTW-CDU
		case CDU_PRESENT30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "1");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "1");		// 2005.04.19 : TTW-CDU ONLY
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
		case CDU_GPRESENT20_SET:								// G-CDU
		case CDU_GPRESENT30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "1");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "1");

			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
		case CDU_TRAY20_SET:									// TRAY-CDU-M
		case CDU_TRAY30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "AutoShutterControl", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "CashTakenCheckingDelay", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EP4China", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "EmptySensor", "0");

			RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", "6");	// DEFAULT(CDU) CDU("6"), BRM("3") : 2010.10.20
			break;
	}

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "MisfeedRetry", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "RetractRetryCnt", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "RejectMaximum", Int2Asc(CSH_FULLREJECTCNT));
			break;
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:								// 2005.12.08
			switch (AtmDefine.MwDevice)
			{
				case MW_KAL:
					RegSetStr(_REGKEY_CDUSTATUS, "MisfeedRetry", "0");
					RegSetStr(_REGKEY_CDUSTATUS, "RetractRetryCnt", "0");
					RegSetStr(_REGKEY_CDUSTATUS, "RejectMaximum", Int2Asc(CSH_FULLREJECTCNT2));
					break;
				case MW_NEXTWARE:
				case MW_KALWOSA:
				case MW_NEXTWOSA:
					RegSetStr(_REGKEY_CDUSTATUS, "MisfeedRetry", "1");
					RegSetStr(_REGKEY_CDUSTATUS, "RetractRetryCnt", "2");
					if (Asc2Int(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2))) > 300)
						IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", "300");
					else
					if (Asc2Int(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2))) < 120)
						IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", "120");
					RegSetStr(_REGKEY_CDUSTATUS, "RejectMaximum", IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2)));
					break;
			}
			break;
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			RegSetStr(_REGKEY_CDUSTATUS, "MisfeedRetry", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "RetractRetryCnt", "0");
			RegSetStr(_REGKEY_CDUSTATUS, "RejectMaximum", Int2Asc(CSH_FULLREJECTCNT));
			break;
	}

	RegSetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMFLAG, "0");		// 2012.08.24

	CString strTemp("");

	strTemp = RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCCDUMIXTYPE);

	if ((Asc2Int(strTemp) >= 1) &&
		(Asc2Int(strTemp) <= 9))
	{
		RegSetStr(_REGKEY_CDUSTATUS, "MixAlgorithm", strTemp);
	}

	RegSetStr(_REGKEY_SOFTXFSCDU, "DispType", "0");
	if ((RegGetStr(_REGKEY_CDUSTATUS, "MixAlgorithm") == "2")	||
		(RegGetStr(_REGKEY_CDUSTATUS, "MixAlgorithm") == "3")	||
		(RegGetStr(_REGKEY_CDUSTATUS, "MixAlgorithm") == "4")	||
		(RegGetStr(_REGKEY_CDUSTATUS, "MixAlgorithm") == "7")	||
		(RegGetStr(_REGKEY_CDUSTATUS, "MixAlgorithm") == "8")	||
		(RegGetStr(_REGKEY_CDUSTATUS, "MixAlgorithm") == "9"))
	{
		RegSetStr(_REGKEY_SOFTXFSCDU, "DispType", "10");		// 2011.01.05
	}

	switch (AtmDefine.OpDevice)
	{
		case NOT_SET:
			OpDeviceLangMode = CHN_MODE;
			RegSetStr(_REGKEY_SIUSTATUS, "ReferInitOperatorState", "NO");
			RegSetStr(_REGKEY_SIUSTATUS, "ReferOperatorSwitch", "NO");
																// 2008.08.05
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionX", 0);	
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionY", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionX", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionY", 0);
			break;
		case OP_TOUCH:
			OpDeviceLangMode = CHN_MODE;
			RegSetStr(_REGKEY_SIUSTATUS, "ReferInitOperatorState", "NO");
			RegSetStr(_REGKEY_SIUSTATUS, "ReferOperatorSwitch", "NO");
																// 2008.08.05
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionX", 0);	
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionY", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionX", 800);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionY", 0);
			break;
		case OP_TSPL:											// 2006.06.02 : TSPL OP S/W EMULATION
			OpDeviceLangMode = ENG_MODE;
			RegSetInt(_REGKEY_TTUSTATUS, "DisplayType", 0);
			RegSetStr(_REGKEY_SIUSTATUS, "ReferInitOperatorState", "YES");
			RegSetStr(_REGKEY_SIUSTATUS, "ReferOperatorSwitch", "YES");
																// 2008.08.05
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionX", 800);	
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionY", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionX", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionY", 0);
			break;
		case OP_TSPLCHN:
			OpDeviceLangMode = CHN_MODE;
			RegSetInt(_REGKEY_TTUSTATUS, "DisplayType", 2);
			RegSetStr(_REGKEY_SIUSTATUS, "ReferInitOperatorState", "YES");
			RegSetStr(_REGKEY_SIUSTATUS, "ReferOperatorSwitch", "YES");
																// 2008.08.05
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionX", 800);	
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionY", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionX", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionY", 0);
			break;
		case OP_HSPL:
			OpDeviceLangMode = ENG_MODE;
			RegSetInt(_REGKEY_TTUSTATUS, "DisplayType", 0);
			RegSetStr(_REGKEY_SIUSTATUS, "ReferInitOperatorState", "NO");
			RegSetStr(_REGKEY_SIUSTATUS, "ReferOperatorSwitch", "NO");
																// 2008.08.05
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionX", 800);	
			RegSetInt(_REGKEY_VDMSTATUS, "ScreenPositionY", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionX", 0);
			RegSetInt(_REGKEY_KEYMGRSTATUS, "ScreenPositionY", 0);
			break;
	}

	if (m_pProfile->NETWORK.InterfaceKind == 111)				// 111:RCB(FZ)
	{
		switch (AtmDefine.MachineKind)
		{
			case NANOCASH:
			case MCD3:
			case TTW:
				break;
			case MCD4:
				break;
			case TTW2:
				break;
			case EATMGBA1:
				break;
			case EATMULL1:
				break;
			case EATMTTW:
				break;
			case TTW2E:											// 2009.07.20
				RegSetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCSPRNORESET, "1");
				break;
			case MCD4E:
				break;
			case MX8000T:
				break;
			case MX8000L:
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
	}

	RegSetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCSPRNORESET, "1");
	if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BHCSPRNORESET, "0") == "1")
	{
		RegSetInt(_REGKEY_SPRSTATUS, "InitAtReset", 0);			// 2010.04.01
	}
	
	// AP Setting Section
	if ((RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRWRITEP).GetLength() != 4)		||
		(!IsNum(RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRWRITEP).GetBuffer(0), 4))	||
		(RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP).GetLength() != 4)			||
		(!IsNum(RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP).GetBuffer(0), 4)))
	{
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRWRITEP,		// 2005.04.19
					GetSprintf("%4.4s", &m_pProfile->TRANS.EjrSerialNo[4]));
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRSENDP, 
					GetSprintf("%4.4s", &m_pProfile->TRANS.EjrSerialNo[4]));
	}
	
	if (RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).GetLength() != 31)
	{
		fnAPL_ClearStatistics();								// 2005.05.18
	}
	
	AtmDoing			= FALSE;								// Atm Doing
	AtmStatus			= ATM_INIT;								// Atm Status 
	AtmStatusSave		= ATM_INIT;								// Atm Status Save 
	AtmStatusReEntryFlag= TRUE;									// Atm Status ReEntry Flag
	HostStatus			= HOST_OFFLINE;							// Host Status 
	HostStatusSave		= HOST_OFFLINE;							// Host Status Save 
	TranStatus			= TRAN_IDLE;							// Transaction Status 
	TranStatusSave		= TRAN_IDLE;							// Transaction Status Save 
	TranProc			= TRAN_NULL;							// Transaction Procedure
	TranResult			= FALSE;								// Transaction Result

	DeviceDefine		= DEV_NONE;								// Defined Device Set
	DeviceNoDevice		= DEV_MAIN;								// Device No Device Set
	DeviceDownDevice	= DEV_MAIN;								// Device Down Device Set
	DeviceStatus		= DEV_NONE;								// Device Status Set
	DeviceAutoOff		= DEV_NONE;								// Device Auto Off Set
	DeviceRetract		= DEV_NONE;								// Device Retract Set
	DeviceTran			= DEV_NONE;								// Device Transaction Set 
	DeviceTimeout		= DEV_NONE;								// Device Tiemout Set
	DeviceLost			= DEV_NONE;								// Device Lost Set
	DeviceEvent			= DEV_NONE;								// Device Event Set

	DeviceSetAutoOffBank= DEV_NONE;								// Device Set Auto Off Bank

	DeviceOperationFlag	= FALSE;								// Device Operation Flag
	
	SprRecvTime			= MID_DEVRSP_TIME;						// Spr Recv Time
	JprRecvTime			= MID_DEVRSP_TIME;						// Jpr Recv Time
	McuRecvTime			= MID_DEVRSP_TIME;						// Mcu Recv Time
	CshRecvTime			= MID_DEVRSP_TIME;						// Csh Recv Time
	PbmRecvTime			= MID_DEVRSP_TIME;						// Pbm Recv Time

	OpenKey				= TRUE;									// Open Key
	PowerOffDetect		= POWER_NORMAL;							// Power Off Detect
	RecoveryType		= 0;									// 0:Auto Recovery,1:Manual Recovery
	TranType			= 0;									// 0:Customer Not Use,1:Customer Use(Error Send)

	CurErrCnt			= 0;									// Current Error Count
	memset(&CurErrBuff, 0, sizeof(CurErrBuff));					// Current Error Buffer
	memset(&FirstErrBuff, 0, sizeof(FirstErrBuff));				// First   Error Buffer
	memset(&LastErrBuff, 0, sizeof(LastErrBuff));				// Last    Error Buffer
	memset(&PrintErrBuff, 0, sizeof(PrintErrBuff));				// Print   Error Buffer

	CSTCnt				= 0;									// CST Count
	FstCSTValue			= 0;									// First CST Value
	SndCSTValue			= 0;									// Second CST Value
	TrdCSTValue			= 0;									// Third CST Value
	FthCSTValue			= 0;									// Fourth CST Value
	FstCSTSetCash		= 0;									// First CST Set Cash
	SndCSTSetCash		= 0;									// Second CST Set Cash
	TrdCSTSetCash		= 0;									// Third CST Set Cash
	FthCSTSetCash		= 0;									// Fourth CST Set Cash
	FstCSTCnt			= 0;									// First CST Count(ClerkInformation)
	SndCSTCnt			= 0;									// Second CST Count(ClerkInformation)
	TrdCSTCnt			= 0;									// Third CST Count(ClerkInformation)
	FthCSTCnt			= 0;									// Fourth CST Count(ClerkInformation)

	RejectCSTCnt		= 0;									// Reject CST Count(ClerkInformation)
	FstCSTRejectCnt		= 0;									// First CST Reject Count
	SndCSTRejectCnt		= 0;									// Second CST Reject Count
	TrdCSTRejectCnt		= 0;									// Third CST Reject Count
	FthCSTRejectCnt		= 0;									// Fourth CST Reject Count

	RetractCSTCnt		= 0;									// Retract CST Count(ClerkInformation)
	Csh100RetractCnt	= 0;									// Csh 100 Retract Count
	Csh50RetractCnt		= 0;									// Csh 50 Retract Count
	Csh20RetractCnt		= 0;									// Csh 20 Retract Count
	Csh10RetractCnt		= 0;									// Csh 10 Retract Count
	CshUnknownRetractCnt= 0;									// Csh Unknown Retract Count

	DepositCSTCnt		= 0;									// Deposit CST Count(ClerkInformation)
	Csh100DepositCnt	= 0;									// Csh 100 Deposit Count
	Csh50DepositCnt		= 0;									// Csh 50 Deposit Count
	Csh20DepositCnt		= 0;									// Csh 20 Deposit Count
	Csh10DepositCnt		= 0;									// Csh 10 Deposit Count
	CshUnknownDepositCnt= 0;									// Csh Unknown Deposit Count

	CshD100DepositCnt	= 0;									// Csh D100 Deposit Count(ClerkInformation)
	CshD50DepositCnt	= 0;									// Csh D50 Deposit Count(ClerkInformation)
	CshC100DepositCnt	= 0;									// Csh C100 Deposit Count
	CshC50DepositCnt	= 0;									// Csh C50 Deposit Count

	CshChangeLoc		= NO_CHANGE;							// Csh Change Location
	CshLoc				= NORMAL_POS;							// Csh Location
	RejectCSTLoc		= NORMAL_POS;							// Reject CST Location(ClerkInformation)
	RetractCSTLoc		= NORMAL_POS;							// Retract CST Location(ClerkInformation)
	DepositCSTLoc		= NORMAL_POS;							// Deposit CST Location(ClerkInformation)
	CshD100DepositLoc	= NORMAL_POS;							// Csh D100 Deposit Location(ClerkInformation)
	CshD50DepositLoc	= NORMAL_POS;							// Csh D50 Deposit Location(ClerkInformation)
	CshC100DepositLoc	= NORMAL_POS;							// Csh C100 Deposit Location
	CshC50DepositLoc	= NORMAL_POS;							// Csh C50 Deposit Location
	ClerkCSTLoc			= NORMAL_POS;							// Clerk CST Location
	FstCSTLoc			= NORMAL_POS;							// First CST Location(Cd)
	SndCSTLoc			= NORMAL_POS;							// Second CST Location(Cd)
	TrdCSTLoc			= NORMAL_POS;							// Third CST Location(Cd)
	FthCSTLoc			= NORMAL_POS;							// Fourth CST Location(Cd)
	DoorStatus			= DOOR_CLOSED;							// Door Status
	InternalTamperStatus= NORMAL_POS;							// Internal Tamper Status

	SlipHeadStatus		= HEADER_NORMAL;						// Slip	Head Status
	JnlHeadStatus		= HEADER_NORMAL;						// Journal Head Status
	SlipStatus			= SLIP_NORMAL;							// Slip Status
	JnlStatus			= JNL_NORMAL;							// Journal Status

	if (m_pProfile->NETWORK.InterfaceKind == 4003)				// 4003:CB(ZG) : 2008.06.04
	{
		CashMaxRetractCnt	= IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CSHMAXRETRACTCNT);
																// Cash Max Retract Count
		CashRetractCnt		= IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CSHRETRACTCNT);
																// Cash Retract Count
	}
	else
	{
		CashMaxRetractCnt	= 0;								// Cash Max Retract Count
		CashRetractCnt		= 0;								// Cash Retract Count
		IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CSHMAXRETRACTCNT, CashMaxRetractCnt);
		IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CSHRETRACTCNT, CashRetractCnt);
	}
	
	SlipMaxRetractCnt	= RegGetInt(_REGKEY_SPRSTATUS, "MaxRetractBin");
																// Slip Max Retract Count
	CardMaxRetractCnt	= RegGetInt(_REGKEY_MCUSTATUS, "MaxRetractBin");
																// Card Max Retract Count
	PBMaxRetractCnt		= 0;									// PassBook Max Retract Count
	SlipRetractCnt		= 0;									// Slip Retract Count
	CardRetractCnt		= 0;									// Card Retract Count
	PBRetractCnt		= 0;									// PassBook Retract Count

	RejectCSTStatus		= CST_NORMAL;							// Reject CST Status(ClerkInformation)
	RetractCSTStatus	= CST_NORMAL;							// Retract CST Status(ClerkInformation)
	DepositCSTStatus	= CST_NORMAL;							// Deposit CST Status(ClerkInformation)
	CshD100DepositStatus= CST_NORMAL;							// Csh D100 Deposit Status(ClerkInformation)
	CshD50DepositStatus = CST_NORMAL;							// Csh D50 Deposit Status(ClerkInformation)
	CshC100DepositStatus= CST_NORMAL;							// Csh C100 Deposit Status
	CshC50DepositStatus = CST_NORMAL;							// Csh C50 Deposit Status
	ClerkCSTStatus		= CST_NORMAL;							// Clerk CST Status
	RecycleBoxStatus	= CST_NORMAL;							// Recycle Box Status
	FstCSTStatus		= CST_NORMAL;							// First CST Status
	SndCSTStatus		= CST_NORMAL;							// Second CST Status
	TrdCSTStatus		= CST_NORMAL;							// Third CST Status
	FthCSTStatus		= CST_NORMAL;							// Fourth CST Status
	
	CshInitAmount		= 0;									// Init Amount
	CshRemainAmount		= 0;									// Remain Amount

	TranAvail			= TRAN_ALL_NOT;							// Transaction Avail 
	WithAvail			= TRAN_WITH_NOT;						// Withdraw Avail 
	DepAvail			= TRAN_DEP_NOT;							// Deposit Avail 
	PbmAvail			= TRAN_PBM_NOT;							// Pbm Avail 
	
	InServiceLED		= LED_STR_OFF;							// In ServiceLED
	SupervisorLED		= LED_STR_OFF;							// Supervisor LED
	ErrorLED			= LED_STR_OFF;							// Error LED
	CommunicationsLED	= LED_STR_OFF;							// Communication LED
	Indicator			= IND_ALLOFF;							// Indicator
	
	HostOpenFlag		= FALSE;								// Host Open Flag
	HostOpenRetryCnt	= 0;									// Host Open Retry Count
	HostCancelRetryCnt	= 0;									// Host Cancel Retry Count
	memset(&Timer, 0, sizeof(Timer));							// Timer

	EjrThreadUse = FALSE;										// Elec Journal Thread Use
	EjrDoing = FALSE;											// Elec Journal Doing
	memset(&Ejr, 0, sizeof(Ejr));								// Elec Journal
	memset(&EjrSpr, 0, sizeof(EjrSpr));							// Elec Journal Spr

	CaptureSerialNo = ZERO6;									// Capture Serial No

	MonitorCustomNo	 = -1;										// Monitor Custom Number
	MonitorAddCashNo = -1;										// Monitor Add Cash Number
	FaceCamNo		 = 0;										// Camera No for Face
	HandCamNo		 = 0;										// Camera No for Hand
	AddCashCamNo	 = 0;										// Camera No for AddCash

	VfdThreadUse = FALSE;										// Vfd Thread Use
	VfdDoing = FALSE;											// Vfd Doing

	FtpThreadUse = FALSE;										// Ftp Thread Use
	FtpDoing = FALSE;											// Ftp Doing
	
//////////////////////////////////////////////////////////////////////////
	McuReadMSOnlyFlag = FALSE;									// Mcu Read Only MS Flag:2010.10.20
	McuProtocolCode = 0;										// Mcu Protocol Code:2010.10.20
//////////////////////////////////////////////////////////////////////////

	memset(&Pbc, 0, sizeof(Pbc));								// Pboc Call Back Send Data:2010.10.20

	memset(&Scr, 0, sizeof(Scr));								// Screen

//////////////////////////////////////////////////////////////////////////
	// OP Variable Initialize
	Op.SendRecvStatus	= "OFF";								// SendRecvStatus(ON,OFF)
	Op.TransationStatus	= "OFF";								// TransationStatus(ON,OFF)
	
/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
	// Agent Initialize
	bAgentMode = IniGetInt("D:\\INI\\TRANSACTION.INI", "TRANSACTION","AGENTOPEN", 0);
	// DeviceDefine Create Section
	switch (AtmDefine.SprHandler)								// 0:NOT_SET, 1:THERMAL, 2:DOT, 3:THERMAL(FORM), 4:DOT(FORM)
	{
		case SPR_T_TYPE:
		case SPR_D_TYPE:
		case SPR_TF_TYPE:
		case SPR_DF_TYPE:
			DeviceDefine |= DEV_SPR;
			break;
	}

	switch (AtmDefine.JprHandler)								// 0:NOT_SET, 1:THERMAL, 2:DOT
	{
		case JPR_T_TYPE:
		case JPR_D_TYPE:
			DeviceDefine |= DEV_JPR;
			break;
	}

	switch (AtmDefine.McuHandler)								// 0:NOT_SET, 1:MOTOR, 2:SWIPE, 3:DIP
	{
		case MCU_M_TYPE:
		case MCU_S_TYPE:
		case MCU_D_TYPE:
		case MCU_M_NS_TYPE:
			DeviceDefine |= DEV_MCU;
			break;
	}

	switch (AtmDefine.CashHandler)								// 0:NOT_SET, 1:BRM, 2:CDU_SHUTTER1510, 3:CDU_SHUTTER20, 4:CDU_SHUTTER30, 5:CDU_RETRACT20, 6:CDU_RETRACT30, 7:CDU_PRESENT20, 8:CDU_PRESENT30, 9:CDU_GPRESENT20, 10:CDU_GPRESENT30, 11:CDU_TRAY20, 12:CDU_TRAY30
	{
		case BRM_SET:
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			DeviceDefine |= DEV_CSH;
			break;
	}

	switch (AtmDefine.PBHandler)								// 0:NOT_SET, 1:PBM, 2:PBM(NO AUTO TURN) 
	{
		case PBM_SET:
		case PBM_NAT_SET:
			DeviceDefine |= DEV_PBM;
			break;
	}

	switch (AtmDefine.KeyHandler)								// 0:NOT_SET, 1:KEYMGR_EPP, 2:SINGLE_EPP, 3:TRIPLE_EPP, 4:TRIPLE_NDC
	{
		case KEYMGR_EPP:
		case SINGLE_EPP:
		case TRIPLE_EPP:
		case TRIPLE_NDC:
			break;
	}
		
	switch (AtmDefine.CameraDevice)								// 0:NOT_SET, 1:PCI TYPE CAMERA1, 2:USB TYPE CAMERA1, 3:PCI TYPE CAMERA2, 4:USB TYPE CAMERA2, 5:USB_AVICAMERA1_SET, 6:USB_AVICAMERA2_SET
	{
		case PCI_CAMERA1_SET:
		case USB_CAMERA1_SET:
		case PCI_CAMERA2_SET:
		case USB_CAMERA2_SET:
			DeviceDefine |= DEV_CMR;
			break;
	}

	switch (AtmDefine.ICCardDevice)								// 0:NOT_SET, 1:ICCARD_EMV, 2:ICCARD_PBOC
	{
		case ICCARD_EMV:
		case ICCARD_PBOC:
			DeviceDefine |= DEV_MCU;
			break;
	}

	switch (AtmDefine.PowerHandler)								// 0:NOT_SET, 1:PNC_TYPE, 2:UPS_TYPE
	{
		case PNC_TYPE:
		case UPS_TYPE:
		case EUL_TYPE:											// 2005.12.23
		case EUC_TYPE:
			DeviceDefine |= DEV_UPS;
			break;
	}

	switch (AtmDefine.SplDevice)								// 0:NOT_SET, 1:SPL_TOGGLESW, 2:SPL_ONOFFSW
	{
		case SPL_TOGGLESW:										// 2007.04.03
		case SPL_ONOFFSW:
			break;
	}
	
	switch (AtmDefine.VfdDevice)								// 0:NOT_SET, 1:INSTALL
	{
		case VFD_SET:
			DeviceDefine |= DEV_VFD;							// 2005.04.19
			break;
	}

	switch (AtmDefine.RicDevice)								// 0:NOT_SET, 1:RIC_CBTS
	{
		case RIC_CBTS:
			DeviceDefine |= DEV_RIC;
			break;
	}
																// 2007.09.07
	switch (AtmDefine.IrdDevice)								// 0:NOT_SET, 1:IRD_CBTS
	{
		case IRD_CBTS:
			DeviceDefine |= DEV_IRD;
			break;
	}

	// DeviceDefine Create(Miscellaneous) Section
	DeviceDefine |= DEV_SCR;									// SCREEN
	DeviceDefine |= DEV_DOR;									// DOOR
	DeviceDefine |= DEV_LGT;									// LIGHT
	DeviceDefine |= DEV_PIN;									// PIN : 2004.04.08
	DeviceDefine |= DEV_SNS;									// SENSOR : 2004.04.14

	switch (AtmDefine.MachineKind)								// 2008.01.31
	{
		case NANOCASH:
			break;
		case MCD3:
			break;
		case TTW:
			break;
		case MCD4:
			DeviceDefine |= DEV_SPL;
			break;
		case TTW2:
			break;
		case EATMGBA1:											// 2005.11.11
			break;
		case EATMULL1:											// 2005.11.11
			break;
		case EATMTTW:											// 2005.11.11
			break;
		case TTW2E:												// 2005.11.11
			break;
		case MCD4E:												// 2005.11.11
			DeviceDefine |= DEV_SPL;
			break;
		case MX8000T:											// 2005.11.11
			break;
		case MX8000L:											// 2005.11.11
			break;
		case MX5600T:
			break;
		case MX5600L:
			DeviceDefine |= DEV_SPL;
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_InitializeDS():return");
	return TRUE;
}

// Set Cash Information
int CDevCmn::fnAPL_SetCashInfo(int nCashMaxOutAmt, int nUserMaxOutAmt)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetCashInfo(nCashMaxOutAmt[%d], nUserMaxOutAmt[%d])", nCashMaxOutAmt, nUserMaxOutAmt);

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			if (nUserMaxOutAmt >= nCashMaxOutAmt)				// 2007.06.06
				CashMaxCashOutCnt = nCashMaxOutAmt / CASH_CHINA_100;
			else
				CashMaxCashOutCnt = nUserMaxOutAmt / CASH_CHINA_100;

			if (CashMaxCashOutCnt > CSH_MAXCASHOUTCNT4BRM)		// Cash Max Cash Out Count
				CashMaxCashOutCnt = CSH_MAXCASHOUTCNT4BRM;
			else
			if (CashMaxCashOutCnt < CSH_FIXCASHCNT)
				CashMaxCashOutCnt = CSH_FIXCASHCNT;

			CashFixCashCnt = CSH_FIXCASHCNT;					// Cash Fix Cash Count
			CashEmptyCashCnt = CashMaxCashOutCnt + CSH_FIXCASHCNT;
																// Cash Empty Cash Count : 2008.04.10
			RegSetStr(_REGKEY_CDUSTATUS, "CSTMinimum4Mix6", Int2Asc(CashEmptyCashCnt));
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			if (nUserMaxOutAmt >= nCashMaxOutAmt)				// 2007.06.06
				CashMaxCashOutCnt = nCashMaxOutAmt / CASH_CHINA_100;
			else
				CashMaxCashOutCnt = nUserMaxOutAmt / CASH_CHINA_100;

			if (CashMaxCashOutCnt > CSH_MAXCASHOUTCNT2)			// Cash Max Cash Out Count
				CashMaxCashOutCnt = CSH_MAXCASHOUTCNT2;
			else
			if (CashMaxCashOutCnt < CSH_FIXCASHCNT)
				CashMaxCashOutCnt = CSH_FIXCASHCNT;

			CashFixCashCnt = CSH_FIXCASHCNT;					// Cash Fix Cash Count
			CashEmptyCashCnt = CashMaxCashOutCnt + CSH_FIXCASHCNT;
																// Cash Empty Cash Count : 2008.04.10
			RegSetStr(_REGKEY_CDUSTATUS, "CSTMinimum4Mix6", Int2Asc(CashEmptyCashCnt));
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_SetCashInfo():return");
	return TRUE;
}

// Get Cash Max Information
int CDevCmn::fnAPL_GetCashMaxInfo(int nTranProc)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetCashMaxInfo(nTranProc[%d])", nTranProc);

	int		nCashMaxCount = 0;

	switch (nTranProc)
	{
		case TRAN_WITH:											// WITHDRAW
			break;

		case TRAN_INQ:											// INQUIRY
			break;

		case TRAN_TRANS:										// TRANSFER
			break;

		case TRAN_DEP:											// DEPOSIT
			nCashMaxCount = Asc2Int(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMMAXCASHINCNT, Int2Asc(CSH_MAXCASHINCNT)));
			break;

		case TRAN_PB:											// PASSBOOK
			break;

		case TRAN_IC:											// IC
			break;

		default: 
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetCashMaxInfo():return");
	return nCashMaxCount;
}

// Check Reboot Device
int CDevCmn::fnAPL_CheckRebootDevice()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckRebootDevice()");

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Os Reboot Check");
																// 2005.11.07 : Os Reboot Check
	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_OSREBOOT).CompareNoCase("OSREBOOTOK") != 0)
	{
		for (int i = 30; i > 0; i--)
		{
			fnSCR_DisplayString(4, GetSprintf("PLEASE WAIT !!! (%d SEC)", i));
			fnSCR_DisplayScreen(701);							// Display Wait Message
			Delay_Msg(1000);
		}

		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_OSREBOOT, "OSREBOOTOK");
		WinExecWaitEnd("C:\\T1ATM\\APP\\EXERELE\\GOMA.EXE TPDUAL_UTL.EXE", SW_HIDE);
		return FALSE;
	}

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Ap Install Check");
																// 2006.02.23 : Ap Install Check
	if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_APINSTALL).CompareNoCase("APINSTALLOK") != 0)
	{
		fnSCR_DisplayString(4, "PLEASE AP INSTALL !!! (CD)");
		fnSCR_DisplayScreen(701);								// Display Ap Install Message

		while (TRUE)
		{
			Delay_Msg(1000);
		}

		WinExecWaitEnd("C:\\T1ATM\\APP\\EXERELE\\GOMA.EXE TPDUAL_UTL.EXE", SW_HIDE);
		return FALSE;
	}

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Kal Trace Backup");
																// 2005.12.19 : Kal Trace Backup
	fnCMN_ProcBackupKalTrace(GetSprintf("%s\\BOOT%s%s",
													_AP_TRACE_DIR, 
													GetDate().GetBuffer(0),
													GetTime().GetBuffer(0)));

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Camera Check[%s]", RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_CAMERACHECK));
																// 2005.12.19 : Camera Check
	fnCMR_Initialize();
	fnAPL_CaptureFaceSave(1, TRUE, TRUE);
	fnAPL_CaptureFaceSave(2);
	fnAPL_CaptureHandSave(3);
	fnAPL_CaptureHandSave(4);
	if ((fnCMR_GetDeviceStatus() == NODEVICE)	||				// 2005.04.19
		(fnCMR_GetDeviceStatus() == DOWN))
	{	
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_CAMERACHECK).CompareNoCase("CAMERANG") != 0)
		{
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_CAMERACHECK, "CAMERANG");

			fnCMR_Deinitialize();
			WinExecWaitEnd("C:\\T1ATM\\APP\\EXERELE\\GOMA.EXE TPDUAL_UTL.EXE", SW_HIDE);
			return FALSE;
		}
	}
	else
	{
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_CAMERACHECK, "CAMERAOK");
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Camera Check End[%s]", RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_CAMERACHECK));

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Pin Check[%s]", RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_PINCHECK));
																// 2006.04.19 : Pin Check
	fnPIN_Initialize();
	fnAPL_CheckDeviceAction(DEV_PIN);
	if ((fnPIN_GetDeviceStatus() == NODEVICE)	||				// 2006.04.19
		(fnPIN_GetDeviceStatus() == DOWN))
	{	
		if (RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_PINCHECK).CompareNoCase("PINNG") != 0)
		{
			RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_PINCHECK, "PINNG");

			fnPIN_Deinitialize();
			WinExecWaitEnd("C:\\T1ATM\\APP\\EXERELE\\GOMA.EXE TPDUAL_UTL.EXE", SW_HIDE);
			return FALSE;
		}
	}
	else
	{
		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_PINCHECK, "PINOK");
	}
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "Pin Check End[%s]", RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_PINCHECK));

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CheckRebootDevice():return");
	return TRUE;
}

// Load Device
int CDevCmn::fnAPL_LoadDevice()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice()");

	if (Asc2Int(RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMPLAYMOVIE)))
	{															// PLAY MOVIE BEGIN : 2008.04.10
		HWND hAviWnd = ::FindWindow(NULL, "AviPlayer");
		if (!hAviWnd)
		{
			WinExec("C:\\T1ATM\\CFMONITOR\\AVIPLAYER.EXE", SW_SHOW);
			if (GetShareData("OP_PLAYMOVIE").CompareNoCase("OFF") == 0)
				::PostMessage(hAviWnd, WUM_SOUND_OFF, 0, 0);	// 2008.09.22
		}
	}

	if (m_pProfile->NETWORK.InterfaceKind == 101)				// RCB(HF) : 2007.10.30
		memcpy(&GuideMsgTbl[1][ApDeviceLangMode][25], "********", 8);
	else
		memcpy(&GuideMsgTbl[1][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
	memcpy(&GuideMsgTbl[1][ApDeviceLangMode][48], m_pProfile->TRANS.ApVersion, 9);
	fnSCR_DisplayStringArray(1, GuideMsgTbl[1][ApDeviceLangMode]);
	fnSCR_DisplayScreen(701);									// Display Version

	// Miscellaneous Device Initialize
	fnDOR_Initialize();											// First Create
	fnLGT_Initialize();
	fnSPL_Initialize();
	fnPIN_Initialize();
	fnSNS_Initialize();
	fnCMR_Initialize();
	fnUPS_Initialize();
	fnVFD_Initialize();											// 2005.04.19

	fnDVR_Initialize();											// 2005.06.15
	fnETC_Initialize();											// 2006.08.22

	fnRIC_Initialize();											// 2008.07.01
	fnIRD_Initialize();											// 2008.07.01

//	fnJPR_Initialize();											// AtmHostCtrl Create
//	fnSPR_Initialize();											// AtmHostCtrl Create
//	fnMCU_Initialize();											// AtmHostCtrl Create
//	fnCSH_Initialize();											// AtmHostCtrl Create
//	fnPBM_Initialize();											// AtmHostCtrl Create
	
	Delay_Msg(1000);
	// Load Switch Procedure Section
	if (fnSPL_GetLoadSwitch())
	{
	}

//	fnNET_Initialize(LINE_OPCL_TIME);							// AtmHostCtrl Create
	fnSCR_Initialize();
	fnSCR_SetCurrentLangMode(CHN_MODE);

	// Vfd Control Thread Start
	VfdDoing = TRUE;											// 2005.04.19

	// Elec Journal Send Thread Start
	EjrDoing = TRUE;
//	AfxBeginThread(fnAPT_EjrSendThread, this, THREAD_PRIORITY_NORMAL);

	// Ftp Send Thread Start
	FtpDoing = TRUE;
//	AfxBeginThread(fnAPT_FtpSendThread, this, THREAD_PRIORITY_NORMAL);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_LoadDevice():return");
	return TRUE;
}

// Unload Device
int CDevCmn::fnAPL_UnLoadDevice()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnLoadDevice()");

	// Elec Journal Send Thread Stop
	EjrDoing = FALSE;											// Elec Journal Doing
	while (EjrThreadUse)										// End Check : 2005.04.19
		Delay_Msg(1000);
	
	// Vfd Control Thread Stop
	VfdDoing = FALSE;											// Vfd Doing
	while (VfdThreadUse)										// End Check : 2005.04.19
		Delay_Msg(1000);

	// Ftp Send Thread Stop
	FtpDoing = FALSE;											// Ftp Doing
	while (FtpThreadUse)										// End Check : 2005.04.19
		Delay_Msg(1000);
	
	switch (AtmDefine.MachineKind)								// MACHINE KIND : 2007.05.07
	{
		case NANOCASH:
		case MCD3:
		case TTW:
			break;
		case MCD4:
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			break;
		case TTW2:
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			break;
		case EATMGBA1:											// 2005.11.11
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case EATMULL1:											// 2005.11.11
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 0);
			RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 0);
			break;
		case EATMTTW:											// 2005.11.11
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case TTW2E:												// 2005.11.11
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case MCD4E:												// 2005.11.11
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case MX8000T:											// 2005.11.11
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case MX8000L:											// 2005.11.11
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case MX5600T:
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
		case MX5600L:
			if (CSTCnt >= 4)
			{
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst1", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst2", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst3", 1);
				RegSetInt(_REGKEY_VDMSTATUS, "DispenseNotesCst4", 1);
			}
			break;
	}

	fnSCR_Deinitialize();
//	fnNET_DeInitialize(LINE_OPCL_TIME);							// AtmModeCtrl Delete

	fnPBM_Deinitialize();
	fnCSH_Deinitialize();
	fnMCU_Deinitialize();
	fnSPR_Deinitialize();
	fnJPR_Deinitialize();
	fnAPL_CheckDeviceAction(DEV_MAIN);

	fnDVR_Deinitialize();										// 2005.06.15
	fnETC_Deinitialize();										// 2005.08.22
	fnRIC_Deinitialize();										// 2008.07.01
	fnIRD_Deinitialize();										// 2008.07.01

	fnVFD_Deinitialize();										// 2005.04.19
	fnUPS_Deinitialize();
	fnCMR_Deinitialize();
	fnSNS_Deinitialize();										// Last Delete
	fnPIN_Deinitialize();
	fnSPL_Deinitialize();
	fnLGT_Deinitialize();
	fnDOR_Deinitialize();
	
	Delay_Msg(3000);
	switch (AtmDefine.MachineKind)
	{															// 2006.06.02
		case NANOCASH:
		case MCD3:
		case TTW:
		case MCD4:
		case TTW2:
		case EATMGBA1:
		case EATMULL1:
		case EATMTTW:
		case TTW2E:
		case MCD4E:
		case MX8000T:
		case MX8000L:											// CDU
		case MX5600T:
		case MX5600L:
			WinExecWaitEnd("C:\\T1ATM\\APP\\EXERELE\\GOMA.EXE    ComMonitor4CDM.exe "		\
																"ComMonitor4TSCDM.exe "		\
																"NHCDM4GCDU.exe "			\
																"NHCDM.exe "				\
																"NHCDM4TSCDU.exe "			\
																// BRM : 2008.12.09
																"CDMCIMSP.exe "				\
																"Dipl.exe "					\
																"DiplWait.exe "				\
																"Router.exe "				\
																// NH BRM SP: 2009.11.18
																"NHCDM4CRM.exe "			\
																"NHCIM4CRM.exe "			\
																// IDC
																"NHMCD4IDC.exe "			\
																"TTW_EX_MCU.exe "			\
																"IDC_NANO.exe "				\
																"SANKYO4XFS30.exe "			\
																"KMCU4DCP30.exe "			\
																"OMRON4IDC30.exe "			\
																"Idchcshu.exe "				\
																"KIOSK_MCU.exe "			\
																"MAGTEK.exe "				\
																// SPR
																"TTW_EX_SPR.exe "			\
																"NW_KSPR.exe "				\
																"NW_NC2.exe "				\
																"RPtrHCShu.exe "			\
																"NHPTR4PRT.exe "			\
																// JPR
																"ComMonitor4DJPR.exe "		\
																"DotJPR.exe "				\
																"NW_DotJPR.exe "			\
																"DotJournalPrinter.exe "	\
																// SIU
																"ComMonitor4PNC.exe "		\
																"NH_SIU.exe "				\
																"HSSiu.exe "				\
																"DcpEmul.exe "				\
																"MySiu.exe "				\
																"SIU.exe "					\
																// PIN
																"MyPinPad.exe "				\
																"NHPIN4EPP.exe "			\
																// SPL
																"ComMonitor4TSPL.exe "		\
																"TSPL.exe "					\
																"NXTTU4SPL.exe "			\
																// CAMERA
																"HsCamera.exe "				\
																"Nx2Camera.exe "			\
																"HSCAMSp.exe "				\
																"HcCamera.exe "				\
																// OTHER
//																"ComMonitor.exe "
																"Ups.exe "					\
																"RPLVFDCTRL.exe "			\
																"3270w32.exe",
																SW_HIDE);
			break;
	}
	Delay_Msg(2000);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_UnLoadDevice():return");
	return TRUE;
}

// Get Define Device
int	CDevCmn::fnAPL_GetDefineDevice(int nDevId)
{
	return (DeviceDefine & nDevId);
}

// Get Yes Device
int	CDevCmn::fnAPL_GetYesDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (~(fnAPL_GetNoDevice(nDevId))));
}

// Get No Device
int	CDevCmn::fnAPL_GetNoDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (DeviceNoDevice & nDevId));
}

// Get Down Device
int	CDevCmn::fnAPL_GetDownDevice(int nDevId)
{
	return (fnAPL_GetDefineDevice(nDevId) & (DeviceDownDevice & nDevId));
}

// Get Avail Device
int	CDevCmn::fnAPL_GetAvailDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_BOTH | TRAN_DEP_CASH)))
		nDevId &= ~DEV_CSH;
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;
	return (nDevId & (~(fnAPL_GetErrorDevice(nDevId))));
}

// Get Avail Error Device
int	CDevCmn::fnAPL_GetAvailErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_BOTH | TRAN_DEP_CASH)))
		nDevId &= ~DEV_CSH;
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;
	return (fnAPL_GetErrorDevice(nDevId));
}

// Get Error Device
int	CDevCmn::fnAPL_GetErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	DeviceStatus |= m_pProfile->TRANS.DeviceExecOff;
	return (DeviceStatus & nDevId);
}

// Get Down Error Device
int	CDevCmn::fnAPL_GetDownErrorDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);
																// 2007.04.16
	return (fnAPL_GetDownDevice(nDevId) | fnAPL_GetErrorDevice(nDevId) | (DeviceAutoOff & nDevId));
}

// Get Use Device
int	CDevCmn::fnAPL_GetUseDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_WITH_BOTH | TRAN_DEP_CASH)))
		nDevId &= ~DEV_CSH;
	if (!(m_pProfile->TRANS.ClerkTransOnOff & (TRAN_PBM_OK)))
		nDevId &= ~DEV_PBM;

	nDevId &= ~DeviceAutoOff;
	nDevId &= ~m_pProfile->TRANS.DeviceExecOff;
	return (nDevId);
}

// Get Auto Off Bank Device
int CDevCmn::fnAPL_GetAutoOffBankDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	return (DeviceSetAutoOffBank & nDevId);
}

// Set Auto Off Bank Device
int CDevCmn::fnAPL_SetAutoOffBankDevice(int nDevId)
{
	nDevId = fnAPL_GetDefineDevice(nDevId);

	DeviceSetAutoOffBank = nDevId;
	return (DeviceSetAutoOffBank);
}

// Get Avail Transaction
int CDevCmn::fnAPL_GetAvailTrans()
{
	WithAvail	= fnAPL_GetAvailWithdraw();
	DepAvail	= fnAPL_GetAvailDeposit();
	PbmAvail	= fnAPL_GetAvailPassbook();
	TranAvail	= fnAPL_GetClerkTransOnOff();
	return TranAvail;
}

// Get Avail Withdraw
int CDevCmn::fnAPL_GetAvailWithdraw()
{
	int		nTempWithAvail = TRAN_WITH_NOT;

	if ((fnAPL_GetAvailDevice(DEV_CSH))		&&					// 2012.08.24
		(CshLoc == NORMAL_POS)				&&
		(RejectCSTLoc == NORMAL_POS)		&&
		(RejectCSTStatus != CST_FULL)		&&
		(RejectCSTStatus != CST_ERROR)		&&
		(RejectCSTStatus != CST_MANIP)		&&
		(RetractCSTLoc == NORMAL_POS)		&&
		(RetractCSTStatus != CST_FULL)		&&
		(RetractCSTStatus != CST_ERROR)		&&
		(RetractCSTStatus != CST_MANIP))
	{
		switch (fnCSH_GetAvailWithdraw()) 
		{
			case WITH_TYPE1_ONLY:
				nTempWithAvail |= TRAN_WITH_100;
				break;
			case WITH_TYPE2_ONLY:
				nTempWithAvail |= TRAN_WITH_50;
				break;
			case WITH_BOTH_AVAILABLE:
				nTempWithAvail |= TRAN_WITH_BOTH;
				break;

			case WITH_NOT_AVAILABLE:
			default:
				break;
		}
	}

	return nTempWithAvail;
}

// Get Avail Deposit
int CDevCmn::fnAPL_GetAvailDeposit()
{
	int		nTempDepAvail = TRAN_DEP_NOT;
	int		nTempGetDepAvail = DEP_NOT_AVAILABLE;

	if ((fnAPL_GetAvailDevice(DEV_CSH))		&&					// 2008.04.10
		(CshLoc == NORMAL_POS)				&&
		(RetractCSTLoc == NORMAL_POS)		&&
		(RetractCSTStatus != CST_FULL)		&&
		(RetractCSTStatus != CST_ERROR)		&&
		(RetractCSTStatus != CST_MANIP))

	{
		nTempGetDepAvail = fnCSH_GetAvailDeposit();
		if (nTempGetDepAvail & DEP_TYPE100_ONLY)
			nTempDepAvail |= TRAN_DEP_100;
		if (nTempGetDepAvail & DEP_TYPE50_ONLY)
			nTempDepAvail |= TRAN_DEP_50;
		if (nTempGetDepAvail & DEP_TYPE20_ONLY)
			nTempDepAvail |= TRAN_DEP_20;
		if (nTempGetDepAvail & DEP_TYPE10_ONLY)
			nTempDepAvail |= TRAN_DEP_10;
	}

	return nTempDepAvail;
}

// Get Avail Passbook
int CDevCmn::fnAPL_GetAvailPassbook()
{
	int		nTempPbmAvail = TRAN_PBM_NOT;

	if (fnAPL_GetAvailDevice(DEV_PBM))							
	{
		nTempPbmAvail |= TRAN_PBM_OK;
	}

	return nTempPbmAvail;
}

// Get Clerk Transaction OnOff
int CDevCmn::fnAPL_GetClerkTransOnOff()
{
	int		nTempWithAvail	= WithAvail;
	int		nTempDepAvail	= DepAvail;
	int		nTempPbmAvail	= PbmAvail;

// Withdraw Status Procedure									// 2008.04.10
	if (nTempWithAvail & TRAN_WITH_BOTH)
	{
		if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_BOTH))	
			nTempWithAvail = TRAN_WITH_NOT;
	}
	else
	{
		nTempWithAvail = TRAN_WITH_NOT;
	}

// Deposit Status Procedure										// 2008.04.10
	if (nTempDepAvail & TRAN_DEP_CASH)
	{
		if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_DEP_CASH))
			nTempDepAvail = TRAN_DEP_NOT;
	}
	else
	{
		nTempDepAvail = TRAN_DEP_NOT;
	}
	
// Pbm Status Procedure											// 2008.04.10
	if (nTempPbmAvail & TRAN_PBM_OK)
	{
		if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_PBM_OK))
			nTempPbmAvail = TRAN_PBM_NOT;
	}
	else
	{
		nTempPbmAvail =TRAN_PBM_NOT;
	}

	WithAvail	= nTempWithAvail;
	DepAvail	= nTempDepAvail;
	PbmAvail	= nTempPbmAvail;
	return (nTempWithAvail | nTempDepAvail | nTempPbmAvail);
}

// Set Sensor Information
int CDevCmn::fnAPL_SetSensorInfo()
{
	int			nEventDevice = DEV_NONE;						// 2008.04.10
	int			nStatusDevice = DEV_NONE;
	int			nResultDevice = DEV_NONE;
	CString		strTempSensor("");
	CString		strReturn("");
	CStringArray strReturnArray;
																
	nEventDevice = fnCMN_GetEventDevice(DEV_MAIN | DEV_DOR | DEV_SNS);	// 2012.08.24
	nStatusDevice = fnCMN_GetStatusDevice(DEV_MAIN | DEV_DOR | DEV_SNS);
	nResultDevice = DeviceEvent | nEventDevice | nStatusDevice;	// 2004.08.23
	DeviceEvent = DEV_NONE;										// 2004.08.23

	if (nResultDevice & DEV_CSH)
	{
		if (fnAPL_GetYesDevice(DEV_CSH))
		{
			FstCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_1);		// First CST Count(ClerkInformation)
			SndCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_2);		// Second CST Count(ClerkInformation)
			TrdCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_3);		// Third CST Count(ClerkInformation)
			FthCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_4);		// Fourth CST Count(ClerkInformation)

			RejectCSTCnt  = fnCSH_GetNumberOfReject(CSH_CST_REJECT);
			FstCSTRejectCnt = fnCSH_GetNumberOfReject(CSH_CST_1);
			SndCSTRejectCnt = fnCSH_GetNumberOfReject(CSH_CST_2);
			TrdCSTRejectCnt = fnCSH_GetNumberOfReject(CSH_CST_3);
			FthCSTRejectCnt = fnCSH_GetNumberOfReject(CSH_CST_4);

			strReturn = fstrCSH_GetNumberOfRetract();
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				RetractCSTCnt = Asc2Int(strReturnArray[0]);
			if (strReturnArray.GetSize() >= 2)
				Csh100RetractCnt = Asc2Int(strReturnArray[1]);
			if (strReturnArray.GetSize() >= 3)
				Csh50RetractCnt = Asc2Int(strReturnArray[2]);
			if (strReturnArray.GetSize() >= 4)
				Csh20RetractCnt = Asc2Int(strReturnArray[3]);
			if (strReturnArray.GetSize() >= 5)
				Csh10RetractCnt = Asc2Int(strReturnArray[4]);
			if (strReturnArray.GetSize() >= 6)
				CshUnknownRetractCnt = Asc2Int(strReturnArray[5]);

			strReturn = fstrCSH_GetNumberOfDeposit(CASH_TYPE_DEPOSIT);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				DepositCSTCnt = Asc2Int(strReturnArray[0]);
			if (strReturnArray.GetSize() >= 2)
				Csh100DepositCnt = Asc2Int(strReturnArray[1]);
			if (strReturnArray.GetSize() >= 3)
				Csh50DepositCnt = Asc2Int(strReturnArray[2]);
			if (strReturnArray.GetSize() >= 4)
				Csh20DepositCnt = Asc2Int(strReturnArray[3]);
			if (strReturnArray.GetSize() >= 5)
				Csh10DepositCnt = Asc2Int(strReturnArray[4]);
			if (strReturnArray.GetSize() >= 6)
				CshUnknownDepositCnt = Asc2Int(strReturnArray[5]);

			strReturn = fstrCSH_GetNumberOfDeposit(CASH_TYPE_DEPOSIT100);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				CshD100DepositCnt = Asc2Int(strReturnArray[0]);
			strReturn = fstrCSH_GetNumberOfDeposit(CASH_TYPE_DEPOSIT50);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				CshD50DepositCnt = Asc2Int(strReturnArray[0]);

			strReturn = fstrCSH_GetNumberOfDeposit(CASH_TYPE_RECYCLE100);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				CshC100DepositCnt = Asc2Int(strReturnArray[0]);
			strReturn = fstrCSH_GetNumberOfDeposit(CASH_TYPE_RECYCLE50);
			SplitString(strReturn, ",", strReturnArray);
			if (strReturnArray.GetSize() >= 1)
				CshC50DepositCnt = Asc2Int(strReturnArray[0]);
						
			CshChangeLoc	= fnCSH_GetChangeUnitLock();
			CshLoc			= fnCSH_GetPosition();
			RejectCSTLoc	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			RetractCSTLoc	= (fnCSH_GetRetractCSTStatus() == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			DepositCSTLoc	 = (fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT)	== CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			CshD100DepositLoc= (fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT100) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			CshD50DepositLoc = (fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT50)	== CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			CshC100DepositLoc= (fnCSH_GetDepositCSTStatus(CASH_TYPE_RECYCLE100) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			CshC50DepositLoc = (fnCSH_GetDepositCSTStatus(CASH_TYPE_RECYCLE50)	== CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			ClerkCSTLoc		= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ABNORMAL_POS : NORMAL_POS;
			if (CSTCnt >= 1)
				FstCSTLoc	= (fnCSH_GetCSTStatus(CSH_CST_1) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			if (CSTCnt >= 2)
				SndCSTLoc	= (fnCSH_GetCSTStatus(CSH_CST_2) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			if (CSTCnt >= 3)
				TrdCSTLoc	= (fnCSH_GetCSTStatus(CSH_CST_3) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;
			if (CSTCnt >= 4)
				FthCSTLoc	= (fnCSH_GetCSTStatus(CSH_CST_4) == CST_SET_NG)	? ABNORMAL_POS : NORMAL_POS;

			RejectCSTStatus	= (fnCSH_GetRejectCSTStatus() == CST_SET_NG)	? RejectCSTStatus : fnCSH_GetRejectCSTStatus();
			RetractCSTStatus= (fnCSH_GetRetractCSTStatus() == CST_SET_NG)	? RetractCSTStatus : fnCSH_GetRetractCSTStatus();
			DepositCSTStatus	= (fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT)		== CST_SET_NG)	? DepositCSTStatus : fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT);
			CshD100DepositStatus= (fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT100)	== CST_SET_NG)	? CshD100DepositStatus : fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT100);
			CshD50DepositStatus	= (fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT50)	== CST_SET_NG)	? CshD50DepositStatus : fnCSH_GetDepositCSTStatus(CASH_TYPE_DEPOSIT50);
			CshC100DepositStatus= (fnCSH_GetDepositCSTStatus(CASH_TYPE_RECYCLE100)	== CST_SET_NG)	? CshC100DepositStatus : fnCSH_GetDepositCSTStatus(CASH_TYPE_RECYCLE100);
			CshC50DepositStatus	= (fnCSH_GetDepositCSTStatus(CASH_TYPE_RECYCLE50)	== CST_SET_NG)	? CshC50DepositStatus : fnCSH_GetDepositCSTStatus(CASH_TYPE_RECYCLE50);
			ClerkCSTStatus	= (fnCSH_GetClerkCSTStatus() == CST_SET_NG)		? ClerkCSTStatus : fnCSH_GetClerkCSTStatus();
			RecycleBoxStatus= (fnCSH_GetRecycleBoxStatus() == CST_SET_NG)	? RecycleBoxStatus : fnCSH_GetRecycleBoxStatus();
			if (CSTCnt >= 1)
				FstCSTStatus= (fnCSH_GetCSTStatus(CSH_CST_1) == CST_SET_NG)	? FstCSTStatus : fnCSH_GetCSTStatus(CSH_CST_1);
			if (CSTCnt >= 2)
				SndCSTStatus= (fnCSH_GetCSTStatus(CSH_CST_2) == CST_SET_NG)	? SndCSTStatus : fnCSH_GetCSTStatus(CSH_CST_2);
			if (CSTCnt >= 3)
				TrdCSTStatus= (fnCSH_GetCSTStatus(CSH_CST_3) == CST_SET_NG)	? TrdCSTStatus : fnCSH_GetCSTStatus(CSH_CST_3);
			if (CSTCnt >= 4)
				FthCSTStatus= (fnCSH_GetCSTStatus(CSH_CST_4) == CST_SET_NG)	? FthCSTStatus : fnCSH_GetCSTStatus(CSH_CST_4);

			CshInitAmount = fnCSH_GetNumberOfTotalSetCash(CSH_CST_1) * FstCSTValue +
							fnCSH_GetNumberOfTotalSetCash(CSH_CST_2) * SndCSTValue +
							fnCSH_GetNumberOfTotalSetCash(CSH_CST_3) * TrdCSTValue +
							fnCSH_GetNumberOfTotalSetCash(CSH_CST_4) * FthCSTValue;
			CshRemainAmount = ((fnCSH_GetNumberOfTotalSetCash(CSH_CST_1) - fnCSH_GetNumberOfDispenseCash(CSH_CST_1)) * FstCSTValue)
							+	((fnCSH_GetNumberOfTotalSetCash(CSH_CST_2) - fnCSH_GetNumberOfDispenseCash(CSH_CST_2)) * SndCSTValue)
							+	((fnCSH_GetNumberOfTotalSetCash(CSH_CST_3) - fnCSH_GetNumberOfDispenseCash(CSH_CST_3)) * TrdCSTValue)
							+	((fnCSH_GetNumberOfTotalSetCash(CSH_CST_4) - fnCSH_GetNumberOfDispenseCash(CSH_CST_4)) * FthCSTValue);
			fnCMN_ClearEventDevice(~DEV_CSH);
			fnCMN_ClearStatusDevice(~DEV_CSH);
		}
	}

	if (nResultDevice & DEV_DOR)
	{
		if (fnAPL_GetYesDevice(DEV_DOR))
		{
			DoorStatus		= fnDOR_GetDoorStatus();

			fnCMN_ClearEventDevice(~DEV_DOR);
			fnCMN_ClearStatusDevice(~DEV_DOR);
		}
	}

	if (nResultDevice & DEV_SNS)								// 2012.08.24
	{
		if (fnAPL_GetYesDevice(DEV_SNS))
		{
			InternalTamperStatus = fnSNS_GetInternalTamper();

			fnCMN_ClearEventDevice(~DEV_SNS);
			fnCMN_ClearStatusDevice(~DEV_SNS);
		}
	}

	if (nResultDevice & DEV_SPR)
	{
		if (fnAPL_GetYesDevice(DEV_SPR))
		{
			SlipHeadStatus	= HEADER_NORMAL;					// Coding?
			SlipStatus		= fnSPR_GetPaperStatus();
			SlipRetractCnt	= Asc2Int(fstrSPR_GetRetractCnt());	// 2006.08.22

			fnCMN_ClearEventDevice(~DEV_SPR);
			fnCMN_ClearStatusDevice(~DEV_SPR);
		}
	}

	if (nResultDevice & DEV_JPR)
	{
		if (fnAPL_GetYesDevice(DEV_JPR))
		{
			JnlHeadStatus	= HEADER_NORMAL;					// Coding?
			JnlStatus		= fnJPR_GetPaperStatus();

			fnCMN_ClearEventDevice(~DEV_JPR);
			fnCMN_ClearStatusDevice(~DEV_JPR);
		}
	}

	if (nResultDevice & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))
		{
			CardRetractCnt	= Asc2Int(fstrMCU_GetRetractCnt());	// 2006.08.22

			fnCMN_ClearEventDevice(~DEV_MCU);
			fnCMN_ClearStatusDevice(~DEV_MCU);
		}
	}

	if (nResultDevice & DEV_PBM)
	{
		if (fnAPL_GetYesDevice(DEV_PBM))
		{
			PBRetractCnt	= Asc2Int(fstrPBM_GetRetractCnt());	// 2006.08.22

			fnCMN_ClearEventDevice(~DEV_PBM);
			fnCMN_ClearStatusDevice(~DEV_PBM);
		}
	}

	return TRUE;
}

// Set Date
int CDevCmn::fnAPL_SetDate(LPCTSTR szDate)
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.YYYYMMDD, szDate, 8);
	m_pProfile->TRANS.YYYYMMDD[8] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// Clear Serial No
int CDevCmn::fnAPL_ClearSerialNo()
{
	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, "00000000", 8);
	m_pProfile->TRANS.SerialNo[8] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// Add Serial No
int CDevCmn::fnAPL_AddSerialNo(int nSerialLength)
{
	char	szTempSerialNo[256];
	
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo, 8);

	if (!IsNum(szTempSerialNo, 8))
	{
		memset(szTempSerialNo, '0', 8);
	}

	if (nSerialLength == 4)										// 2004.11.05
	{
		if (memcmp(&szTempSerialNo[4], "9999", 4) == 0)
			memset(szTempSerialNo, '0', 8);
		AddString(szTempSerialNo, "00000001", 8);
	}
	else
	if (nSerialLength == 5)										// 2004.11.05
	{
		if (memcmp(&szTempSerialNo[3], "99999", 5) == 0)
			memset(szTempSerialNo, '0', 8);
		AddString(szTempSerialNo, "00000001", 8);
	}
	else
	if (nSerialLength == 6)										// 2004.11.05
	{
		if (memcmp(&szTempSerialNo[2], "999999", 6) == 0)
			memset(szTempSerialNo, '0', 8);
		AddString(szTempSerialNo, "00000001", 8);
	}
	else
	if (nSerialLength == 7)										// 2004.11.05
	{
		if (memcmp(&szTempSerialNo[1], "9999999", 7) == 0)
			memset(szTempSerialNo, '0', 8);
		AddString(szTempSerialNo, "00000001", 8);
	}
	else
	{
		if (memcmp(szTempSerialNo, "99999999", 8) == 0)
			memset(szTempSerialNo, '0', 8);
		AddString(szTempSerialNo, "00000001", 8);
	}

	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, szTempSerialNo, 8);
	m_pProfile->TRANS.SerialNo[8] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// Set Serial No
int CDevCmn::fnAPL_SetSerialNo(LPCTSTR szSerialNo, int nSerialLength)
{
	if (!IsNum((LPSTR)szSerialNo, nSerialLength))				// 2005.11.11
		return FALSE;

	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.SerialNo, "00000000", 8);
	memcpy(&m_pProfile->TRANS.SerialNo[8 - nSerialLength], szSerialNo, nSerialLength);
	m_pProfile->TRANS.SerialNo[8] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// Set Process Count
int CDevCmn::fnAPL_SetProcCount(char chProcCount)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[PROCESS COUNT] : %1.1c", chProcCount);

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.ProcCount[0] = chProcCount;
	m_pProfile->TRANS.ProcCount[1] = NULL;
	m_pProfile->PutTransProfile();

	fnAPL_DisplayProcCountSegment();
	return TRUE;
}

// Set On Transaction
int CDevCmn::fnAPL_SetOnTransaction(int nTransDoing)
{
	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.TransPowerOnOff = nTransDoing; 
	m_pProfile->PutTransProfile();
	return TRUE;
}

// Set Tranaction Start
int CDevCmn::fnAPL_SetTranStart()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[TRAN START]");

	m_pProfile->GetTransProfile();
	m_pProfile->TRANS.ProcCount[0] = '1';
	m_pProfile->TRANS.ProcCount[1] = NULL;
	m_pProfile->TRANS.TransPowerOnOff = ON; 
	m_pProfile->PutTransProfile();

	fnAPL_DisplayProcCountSegment();
	return TRUE;
}

// Clear Device Execute Off Information
int CDevCmn::fnAPL_ClearDeviceExecOffInfo(int nInitFlag)
{
	if ((nInitFlag) ||
		((memcmp(m_pProfile->TRANS.YYYYMMDD, m_pProfile->TRANS.DeviceYYYYMMDD, 8) != 0)))
	{
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.DeviceExecOff = DEV_NONE;
		memcpy(m_pProfile->TRANS.DeviceYYYYMMDD, m_pProfile->TRANS.YYYYMMDD, 8);
		m_pProfile->TRANS.DeviceSprCnt = 0;						
		m_pProfile->TRANS.DeviceJprCnt = 0;
		m_pProfile->TRANS.DeviceMcuCnt = 0;
		m_pProfile->TRANS.DeviceCshCnt = 0;
		m_pProfile->TRANS.DevicePbmCnt = 0;
		
		m_pProfile->TRANS.DeviceSprRetractCnt = 0;				// 2004.11.05
		m_pProfile->TRANS.DeviceJprRetractCnt = 0;
		m_pProfile->TRANS.DeviceMcuRetractCnt = 0;
		m_pProfile->TRANS.DeviceCshRetractCnt = 0;
		m_pProfile->TRANS.DevicePbmRetractCnt = 0;
		m_pProfile->PutTransProfile();
	}

	return TRUE;
}

// Add Device Execute Off Information
int CDevCmn::fnAPL_AddDeviceExecOffInfo(LPCTSTR szHsErrorCode)
{
	int		DeviceMcuCnt = m_pProfile->TRANS.DeviceMcuCnt;
	int		DeviceCshCnt = m_pProfile->TRANS.DeviceCshCnt;
	int		DevicePbmCnt = m_pProfile->TRANS.DevicePbmCnt;
	int		DeviceExecOff = m_pProfile->TRANS.DeviceExecOff;

	if (szHsErrorCode[0] == '3')								// 2004.08.13
		DeviceMcuCnt++;
	else
	if ((szHsErrorCode[0] == '4') ||
		((szHsErrorCode[0] == '5')))
		DeviceCshCnt++;
	else
	if (szHsErrorCode[0] == '6')
		DevicePbmCnt++;

//	if (DeviceMcuCnt >= 3)										// 2005.04.14
//	{
//		DeviceExecOff |= DEV_MCU;
//	}
//	else 
//		DeviceExecOff &= ~DEV_MCU;

	if (DeviceCshCnt >= 3)										// 2004.11.05
	{
		DeviceExecOff |= DEV_CSH;
	}
	else 
		DeviceExecOff &= ~DEV_CSH;

	if (DevicePbmCnt >= 3)
	{
		DeviceExecOff |= DEV_PBM;
	}
	else
		DeviceExecOff &= ~DEV_PBM;

if (HostLineMode == HOST_LOCAL_MODE)							// Host Local Mode
{
DeviceMcuCnt = 0;
DeviceCshCnt = 0;
DevicePbmCnt = 0;
DeviceExecOff = 0;
}

	if (m_pProfile->NETWORK.InterfaceKind == 11)				// 11:CUP(BCTC)	: 2005.08.31
	{
		DeviceMcuCnt = 0;
		DeviceCshCnt = 0;
		DevicePbmCnt = 0;
		DeviceExecOff = 0;
	}

	if ((DeviceMcuCnt == m_pProfile->TRANS.DeviceMcuCnt)	&&
		(DeviceCshCnt == m_pProfile->TRANS.DeviceCshCnt)	&&
		(DevicePbmCnt == m_pProfile->TRANS.DevicePbmCnt)	&&
		(DeviceExecOff == m_pProfile->TRANS.DeviceExecOff))
		;
	else
	{
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.DeviceMcuCnt = DeviceMcuCnt;
		m_pProfile->TRANS.DeviceCshCnt = DeviceCshCnt;
		m_pProfile->TRANS.DevicePbmCnt = DevicePbmCnt;
		m_pProfile->TRANS.DeviceExecOff = DeviceExecOff;
		m_pProfile->PutTransProfile();
	}

	return T_OK;
}

// Add Device Retract Count
int CDevCmn::fnAPL_AddDeviceRetractCnt(int nDevId)
{
	int		DeviceSprRetractCnt = m_pProfile->TRANS.DeviceSprRetractCnt;
	int		DeviceJprRetractCnt = m_pProfile->TRANS.DeviceJprRetractCnt;
	int		DeviceMcuRetractCnt = m_pProfile->TRANS.DeviceMcuRetractCnt;
	int		DeviceCshRetractCnt = m_pProfile->TRANS.DeviceCshRetractCnt;
	int		DevicePbmRetractCnt = m_pProfile->TRANS.DevicePbmRetractCnt;

	if (nDevId & DEV_SPR)										// 2004.11.05
		DeviceSprRetractCnt++;
	if (nDevId & DEV_JPR)
		DeviceJprRetractCnt++;
	if (nDevId & DEV_MCU)
		DeviceMcuRetractCnt++;
	if (nDevId & DEV_CSH)
		DeviceCshRetractCnt++;
	if (nDevId & DEV_PBM)
		DevicePbmRetractCnt++;

if (HostLineMode == HOST_LOCAL_MODE)							// Host Local Mode
{
DeviceSprRetractCnt = 0;
DeviceJprRetractCnt = 0;
DeviceMcuRetractCnt = 0;
DeviceCshRetractCnt = 0;
DevicePbmRetractCnt = 0;
}

	if ((DeviceSprRetractCnt == m_pProfile->TRANS.DeviceSprRetractCnt)	&&
		(DeviceJprRetractCnt == m_pProfile->TRANS.DeviceJprRetractCnt)	&&
		(DeviceMcuRetractCnt == m_pProfile->TRANS.DeviceMcuRetractCnt)	&&
		(DeviceCshRetractCnt == m_pProfile->TRANS.DeviceCshRetractCnt)	&&
		(DevicePbmRetractCnt == m_pProfile->TRANS.DevicePbmRetractCnt))
		;
	else
	{
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.DeviceSprRetractCnt = DeviceSprRetractCnt;
		m_pProfile->TRANS.DeviceJprRetractCnt = DeviceJprRetractCnt;
		m_pProfile->TRANS.DeviceMcuRetractCnt = DeviceMcuRetractCnt;
		m_pProfile->TRANS.DeviceCshRetractCnt = DeviceCshRetractCnt;
		m_pProfile->TRANS.DevicePbmRetractCnt = DevicePbmRetractCnt;
		m_pProfile->PutTransProfile();
	}

	return T_OK;
}

// Clear Statistics
int CDevCmn::fnAPL_ClearStatistics()
{
	StatErrorDbTbl	StatErrorDbData[ERROR_DB_MAX_COUNT];
	char			szTemp[256];
	CString			strErrorDbData("");

	// Transaction Statistics Clear								// 2005.05.18
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE,		"00000000 000000 00000000 000000");
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANWITHDRAW,	"00000000000000000000 00000000000000000000");
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANINQUIRY,	"00000000000000000000 00000000000000000000");
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANTRANSFER,	"00000000000000000000 00000000000000000000");
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDEPOSIT,	"00000000000000000000 00000000000000000000");
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANPASSBOOK,	"00000000000000000000 00000000000000000000");
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANIC,		"00000000000000000000 00000000000000000000");

	// Error Statistics Clear									// 2005.05.18
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDATE,		"00000000 000000 00000000 000000");
	for (int i = 0; i < ERROR_DB_MAX_COUNT; i++)
	{
		memset(&StatErrorDbData[i],						' ', sizeof(StatErrorDbData[i]));

		memset(&StatErrorDbData[i].ProcCount,			' ', sizeof(StatErrorDbData[i].ProcCount));
		memset(&StatErrorDbData[i].Fill1,				' ', sizeof(StatErrorDbData[i].Fill1));
		memset(&StatErrorDbData[i].ErrorCode[0],		' ', sizeof(StatErrorDbData[i].ErrorCode));
		memset(&StatErrorDbData[i].Fill2,				' ', sizeof(StatErrorDbData[i].Fill2));
		memset(&StatErrorDbData[i].ErrorCount[0],		'0', sizeof(StatErrorDbData[i].ErrorCount));
		memset(&StatErrorDbData[i].Fill3,				' ', sizeof(StatErrorDbData[i].Fill3));
		memset(&StatErrorDbData[i].ErrorBeginDate[0],	'0', sizeof(StatErrorDbData[i].ErrorBeginDate));
		memset(&StatErrorDbData[i].Fill4,				' ', sizeof(StatErrorDbData[i].Fill4));
		memset(&StatErrorDbData[i].ErrorEndDate[0],		'0', sizeof(StatErrorDbData[i].ErrorEndDate));
		memset(&StatErrorDbData[i].Fill5,				' ', sizeof(StatErrorDbData[i].Fill5));
		memset(&StatErrorDbData[i].Dummy[0],			' ', sizeof(StatErrorDbData[i].Dummy));

		sprintf(szTemp, "%%%d.%ds", sizeof(StatErrorDbData[i]), sizeof(StatErrorDbData[i]));
		strErrorDbData += GetSprintf(szTemp, (LPSTR)&StatErrorDbData[i]);
	}
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDB, strErrorDbData);
	return T_OK;
}

// Add Transaction Statistics
int CDevCmn::fnAPL_AddTransactionStatistics(int nTranProc, LPCTSTR szAmount, int nAmountLength)
{
	char	szCount[256] = { "00000000000000000001" };
	char	szMoney[256];
	char	szTemp[256];
	CString	strTemp("");

	memset(szMoney, 0, sizeof(szMoney));
	memset(szTemp, 0, sizeof(szTemp));

	memset(szMoney, '0', 20);
	if (nAmountLength <= 20)									// 2005.08.25
		memcpy(&szMoney[20 - nAmountLength], szAmount, nAmountLength);
	else
		memcpy(&szMoney[0], szAmount, 20);
	
	switch (nTranProc)											// 2005.05.18
	{
		case TRAN_WITH:											// WITHDRAW
			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANWITHDRAW);
			
			sprintf(szTemp, "%s", strTemp.GetBuffer(0));
			AddString(&szTemp[0], szCount, 20);
			AddString(&szTemp[21], szMoney, 20);
			strTemp.Format("%s", szTemp);			

			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANWITHDRAW,	strTemp);

			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(8);
			if (strTemp == ZERO8)
			{
				strTemp = GetDate() + " " + GetTime() + " ";
				strTemp += GetDate() + " " + GetTime();
			}
			else
			{
				strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(16);
				strTemp += GetDate() + " " + GetTime();
			}
			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE, strTemp);
			break;

		case TRAN_INQ:											// INQUIRY
			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANINQUIRY);
			
			sprintf(szTemp, "%s", strTemp.GetBuffer(0));
			AddString(&szTemp[0], szCount, 20);
			AddString(&szTemp[21], szMoney, 20);
			strTemp.Format("%s", szTemp);			

			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANINQUIRY,	strTemp);

			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(8);
			if (strTemp == ZERO8)
			{
				strTemp = GetDate() + " " + GetTime() + " ";
				strTemp += GetDate() + " " + GetTime();
			}
			else
			{
				strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(16);
				strTemp += GetDate() + " " + GetTime();
			}
			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE, strTemp);
			break;

		case TRAN_TRANS:										// TRANSFER
			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANTRANSFER);
			
			sprintf(szTemp, "%s", strTemp.GetBuffer(0));
			AddString(&szTemp[0], szCount, 20);
			AddString(&szTemp[21], szMoney, 20);
			strTemp.Format("%s", szTemp);			

			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANTRANSFER,	strTemp);

			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(8);
			if (strTemp == ZERO8)
			{
				strTemp = GetDate() + " " + GetTime() + " ";
				strTemp += GetDate() + " " + GetTime();
			}
			else
			{
				strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(16);
				strTemp += GetDate() + " " + GetTime();
			}
			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE, strTemp);
			break;

		case TRAN_DEP:											// DEPOSIT
			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDEPOSIT);
			
			sprintf(szTemp, "%s", strTemp.GetBuffer(0));
			AddString(&szTemp[0], szCount, 20);
			AddString(&szTemp[21], szMoney, 20);
			strTemp.Format("%s", szTemp);			

			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDEPOSIT,	strTemp);

			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(8);
			if (strTemp == ZERO8)
			{
				strTemp = GetDate() + " " + GetTime() + " ";
				strTemp += GetDate() + " " + GetTime();
			}
			else
			{
				strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(16);
				strTemp += GetDate() + " " + GetTime();
			}
			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE, strTemp);
			break;

		case TRAN_PB:											// PASSBOOK
			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANPASSBOOK);
			
			sprintf(szTemp, "%s", strTemp.GetBuffer(0));
			AddString(&szTemp[0], szCount, 20);
			AddString(&szTemp[21], szMoney, 20);
			strTemp.Format("%s", szTemp);			

			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANPASSBOOK,	strTemp);

			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(8);
			if (strTemp == ZERO8)
			{
				strTemp = GetDate() + " " + GetTime() + " ";
				strTemp += GetDate() + " " + GetTime();
			}
			else
			{
				strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(16);
				strTemp += GetDate() + " " + GetTime();
			}
			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE, strTemp);
			break;

		case TRAN_IC:											// IC
			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANIC);
			
			sprintf(szTemp, "%s", strTemp.GetBuffer(0));
			AddString(&szTemp[0], szCount, 20);
			AddString(&szTemp[21], szMoney, 20);
			strTemp.Format("%s", szTemp);			

			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANIC,	strTemp);

			strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(8);
			if (strTemp == ZERO8)
			{
				strTemp = GetDate() + " " + GetTime() + " ";
				strTemp += GetDate() + " " + GetTime();
			}
			else
			{
				strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE).Left(16);
				strTemp += GetDate() + " " + GetTime();
			}
			RegSetStr(_REGKEY_STAT, _REGKEY_STAT_TRANDATE, strTemp);
			break;

		default:
			break;
	}
	
	return T_OK;
}

// Add Error Statistics
int CDevCmn::fnAPL_AddErrorStatistics(LPCTSTR szErrorCode)
{
	StatErrorDbTbl	StatErrorDbData[ERROR_DB_MAX_COUNT];
	StatErrorDbTbl	StatErrorDbTemp;
	char			szTemp[256];
	CString			strErrorDbData("");
	CString			strTemp("");
	int				i = 0, j = 0;
	int				nFindFlag = FALSE;

	if (AtmStatus == ATM_TRAN)									// 2005.10.09
		;
	else
	if ((m_pProfile->TRANS.ProcCount[0] != ' ')	&&				// 2005.10.09
		(memcmp(szErrorCode, "991@@", 5) == 0))
		;
	else
		return TRUE;

	// Reg Get													// 2005.05.18
	strErrorDbData = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDB);

	// Reg => StatErrorDbData
	for (i = 0; i < ERROR_DB_MAX_COUNT; i++)
	{
		memcpy(&StatErrorDbData[i], 
			strErrorDbData.Mid(i * sizeof(StatErrorDbData[i]), sizeof(StatErrorDbData[i])).GetBuffer(0),
					sizeof(StatErrorDbData[i]));
	}

	// Error Code Add
	nFindFlag = FALSE;
	// Add Count
	for (i = 0; i < ERROR_DB_MAX_COUNT; i++)
	{
		if ((memcmp(&StatErrorDbData[i].ProcCount, &m_pProfile->TRANS.ProcCount[0], 1) == 0)	&&
			(memcmp(&StatErrorDbData[i].ErrorCode[0], szErrorCode, 7) == 0))
		{
			nFindFlag = TRUE;
			AddString(&StatErrorDbData[i].ErrorCount[0], "000001", 6);
			memcpy(&StatErrorDbData[i].ErrorEndDate[0], GetDate().GetBuffer(0), 8);
			break;
		}
	}

	// Add Error Code
	if (!nFindFlag)
	{
		for (i = 0; i < ERROR_DB_MAX_COUNT; i++)
		{
			if (memcmp(&StatErrorDbData[i].ErrorCode[0], SPACE7, 7) == 0)
			{
				nFindFlag = TRUE;
				memset(&StatErrorDbData[i],	' ', sizeof(StatErrorDbData[i]));
				memcpy(&StatErrorDbData[i].ProcCount, &m_pProfile->TRANS.ProcCount[0], 1);
				memcpy(&StatErrorDbData[i].ErrorCode[0], szErrorCode, 7);
				memcpy(&StatErrorDbData[i].ErrorCount[0], "000001", 6);
				memcpy(&StatErrorDbData[i].ErrorBeginDate[0], GetDate().GetBuffer(0), 8);
				memcpy(&StatErrorDbData[i].ErrorEndDate[0], GetDate().GetBuffer(0), 8);
				break;
			}
		}
	}

	// Last Position Add Error Code
	if (!nFindFlag)
	{
		nFindFlag = TRUE;
		memset(&StatErrorDbData[ERROR_DB_MAX_COUNT - 1], ' ', sizeof(StatErrorDbData[ERROR_DB_MAX_COUNT - 1]));
		memcpy(&StatErrorDbData[ERROR_DB_MAX_COUNT - 1].ProcCount, &m_pProfile->TRANS.ProcCount[0], 1);
		memcpy(&StatErrorDbData[ERROR_DB_MAX_COUNT - 1].ErrorCode[0], szErrorCode, 7);
		memcpy(&StatErrorDbData[ERROR_DB_MAX_COUNT - 1].ErrorCount[0], "000001", 6);
		memcpy(&StatErrorDbData[ERROR_DB_MAX_COUNT - 1].ErrorBeginDate[0], GetDate().GetBuffer(0), 8);
		memcpy(&StatErrorDbData[ERROR_DB_MAX_COUNT - 1].ErrorEndDate[0], GetDate().GetBuffer(0), 8);
	}

	// Sort
	for (i = 0; i < (ERROR_DB_MAX_COUNT - 1); i++)
	{
		for (j = (i + 1); j < ERROR_DB_MAX_COUNT; j++)
		{
			// Compare ErrorCount(Descending)
			if (memcmp(&StatErrorDbData[i].ErrorCount[0], &StatErrorDbData[j].ErrorCount[0], 6) > 0)
				;
			else
			if (memcmp(&StatErrorDbData[i].ErrorCount[0], &StatErrorDbData[j].ErrorCount[0], 6) < 0)
			{
				StatErrorDbTemp = StatErrorDbData[i];
				StatErrorDbData[i] = StatErrorDbData[j];
				StatErrorDbData[j] = StatErrorDbTemp;
			}
			else
			{	// Compare ErrorBeginDate(ascending)
				if (memcmp(&StatErrorDbData[i].ErrorBeginDate[0], &StatErrorDbData[j].ErrorBeginDate[0], 8) > 0)
				{
					StatErrorDbTemp = StatErrorDbData[i];
					StatErrorDbData[i] = StatErrorDbData[j];
					StatErrorDbData[j] = StatErrorDbTemp;
				}
				else
				if (memcmp(&StatErrorDbData[i].ErrorBeginDate[0], &StatErrorDbData[j].ErrorBeginDate[0], 8) < 0)
					;
				else
				{	// Compare ProcCount(ascending)
					if (memcmp(&StatErrorDbData[i].ProcCount, &StatErrorDbData[j].ProcCount, 1) > 0)
					{
						StatErrorDbTemp = StatErrorDbData[i];
						StatErrorDbData[i] = StatErrorDbData[j];
						StatErrorDbData[j] = StatErrorDbTemp;
					}
					else
					if (memcmp(&StatErrorDbData[i].ProcCount, &StatErrorDbData[j].ProcCount, 1) < 0)
						;
					else
					{
						// Compare ProcCount(ascending)
						if (memcmp(&StatErrorDbData[i].ErrorCode[0], &StatErrorDbData[j].ErrorCode[0], 7) > 0)
						{
							StatErrorDbTemp = StatErrorDbData[i];
							StatErrorDbData[i] = StatErrorDbData[j];
							StatErrorDbData[j] = StatErrorDbTemp;
						}
						else
						if (memcmp(&StatErrorDbData[i].ErrorCode[0], &StatErrorDbData[j].ErrorCode[0], 7) < 0)
							;
						else
						{
						}
					}
				}
			}
		}
	}

	// Reg Set
	strErrorDbData = "";
	for (i = 0; i < ERROR_DB_MAX_COUNT; i++)
	{
		sprintf(szTemp, "%%%d.%ds", sizeof(StatErrorDbData[i]), sizeof(StatErrorDbData[i]));
		strErrorDbData += GetSprintf(szTemp, (LPSTR)&StatErrorDbData[i]);
	}
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDB, strErrorDbData);

	// Date Update
	strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDATE).Left(8);
	if (strTemp == ZERO8)
	{
		strTemp = GetDate() + " " + GetTime() + " ";
		strTemp += GetDate() + " " + GetTime();
	}
	else
	{
		strTemp = RegGetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDATE).Left(16);
		strTemp += GetDate() + " " + GetTime();
	}
	RegSetStr(_REGKEY_STAT, _REGKEY_STAT_ERRORDATE, strTemp);
	return T_OK;
}

// Set Clerk Power Off
int CDevCmn::fnAPL_SetClerkPowerOff()
{
//	fnAPL_StackError("POWERCL", "CLERK POWER OFF !!!");
	PowerOffDetect |= CLERK_POWER_OFF;

	switch (AtmDefine.MachineKind)
	{
		case NANOCASH:
			break;
		case MCD3:
			break;
		case TTW:
			break;
		case MCD4:												// 2005.03.08(KAL)
			RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
			break;
		case TTW2:												// 2005.03.08(KAL)
			RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
			break;
		case EATMGBA1:											// 2005.11.11
			break;
		case EATMULL1:											// 2005.11.11
			break;
		case EATMTTW:											// 2005.11.11
			break;
		case TTW2E:												// 2005.11.11
			break;
		case MCD4E:												// 2005.11.11
			break;
		case MX8000T:											// 2005.11.11
			break;
		case MX8000L:											// 2005.11.11
			break;
		case MX5600T:
			break;
		case MX5600L:
			break;
	}
	return TRUE;
}

// Check Power Status
int CDevCmn::fnAPL_CheckPowerStatus(int nRealFlag)
{
	int		nTempPowerOffDetect;

	if (!nRealFlag)												// 2004.08.23
	{
		if (AtmStatus != AtmStatusSave)							// Difference Mode
			return TRUE;

		if (AtmStatus == ATM_TRAN)
			return TRUE;
	}
	
	if (PowerOffDetect & (EXT_AC_OFF | INT_BATTERY_LOW | CLERK_POWER_OFF | POWER_ABNORMAL))
		return FALSE;

	nTempPowerOffDetect = fnUPS_GetPowerStatus();

	if (nTempPowerOffDetect & EXT_AC_OFF)
	{
		fnAPL_StackError("POWERAC", "POWER OUT OR AC OFF !!!");
		PowerOffDetect |= EXT_AC_OFF;

		switch (AtmDefine.MachineKind)
		{
			case NANOCASH:
				break;
			case MCD3:
				break;
			case TTW:
				break;
			case MCD4:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);
				break;
			case TTW2:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 0);
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
		return FALSE;
	}

	if (nTempPowerOffDetect & INT_BATTERY_LOW)
	{
		fnAPL_StackError("POWERBA", "BATTERY LOW !!!");
		PowerOffDetect |= INT_BATTERY_LOW;

		switch (AtmDefine.MachineKind)
		{
			case NANOCASH:
				break;
			case MCD3:
				break;
			case TTW:
				break;
			case MCD4:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
				break;
			case TTW2:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
		return FALSE;
	}

	if (nTempPowerOffDetect & CLERK_POWER_OFF)
	{
//		fnAPL_StackError("POWERCL", "CLERK POWER OFF !!!");
		PowerOffDetect |= CLERK_POWER_OFF;

		switch (AtmDefine.MachineKind)
		{
			case NANOCASH:
				break;
			case MCD3:
				break;
			case TTW:
				break;
			case MCD4:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
				break;
			case TTW2:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
		return FALSE;
	}

	if (nTempPowerOffDetect & POWER_ABNORMAL)
	{
		fnAPL_StackError("POWERAB", "UPS ABNORMAL !!!");
		PowerOffDetect |= POWER_ABNORMAL;

		switch (AtmDefine.MachineKind)
		{
			case NANOCASH:
				break;
			case MCD3:
				break;
			case TTW:
				break;
			case MCD4:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
				break;
			case TTW2:											// 2005.03.08(KAL)
				RegSetInt(_REGKEY_UPSSTATUS, "PowerOffType", 2);
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
		return FALSE;
	}

	return TRUE;
}

// Check Power Off Error
int CDevCmn::fnAPL_CheckPowerOffError()
{
	if (m_pProfile->TRANS.TransPowerOnOff == ON) 
	{
		fnAPL_StackError("991@@91", "TRANSACTION POWER OFF !!!");
		m_pProfile->GetTransProfile();
		m_pProfile->TRANS.TransPowerOnOff = OFF; 
		m_pProfile->PutTransProfile();
		return FALSE;
	}

	return TRUE;
}

// Clear Error
int CDevCmn::fnAPL_ClearError()
{
	CurErrCnt = 0;
	m_pProfile->GetCrtErrStack(&CurErrBuff, TRUE);

	memset(&PrintErrBuff, 0, sizeof(PrintErrBuff));				// Print   Error Buffer

/////////////////////////////////////////////////////////////////////////////
	fnAPL_ClerkInformation();									// Clerk Information
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// Check Error
int CDevCmn::fnAPL_CheckError()
{
	CurErrCnt = m_pProfile->GetCrtErrStack(&CurErrBuff, FALSE);
	if (CurErrCnt)
	{
		m_pProfile->GetFirstErrStack(&FirstErrBuff);
		m_pProfile->GetLastErrStack(&LastErrBuff);
		return FALSE;
	}
	else 
		return TRUE;
}

// Check ErrorCode Valid
int CDevCmn::fnAPL_CheckErrorCodeValid(LPCTSTR szErrorCode)
{
	if ((IsSpace((LPSTR)szErrorCode, 7))				||		// 2005.05.18
		(IsZero((LPSTR)szErrorCode, 7))					||
		(FindChar((LPSTR)szErrorCode, 7, NULL) != 7))
		return FALSE;
	else 
		return TRUE;
}

// Stack Error													
int CDevCmn::fnAPL_StackError(LPCTSTR szErrorCode, LPCTSTR szErrorString, int nDevId)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[ERROR CODE] : %1.1c-%7.7s %s", m_pProfile->TRANS.ProcCount[0], szErrorCode, szErrorString);

	CString	strTempErrorcode(szErrorCode);
		
	strTempErrorcode += "0000000";
	strTempErrorcode = strTempErrorcode.Left(7);

	if (!fnAPL_CheckErrorCodeValid(szErrorCode))
	{	
		m_pProfile->ErrStack(m_pProfile->TRANS.ProcCount[0], "9999999", "INTERNAL CHECK !!!");
		fnAPL_AddErrorStatistics("9999999");					// 2005.05.18
	}
	else
	{
		m_pProfile->ErrStack(m_pProfile->TRANS.ProcCount[0], strTempErrorcode.GetBuffer(0), (LPSTR)szErrorString);
		fnAPL_AddErrorStatistics(strTempErrorcode.GetBuffer(0));// 2005.05.18
	}

	fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], szErrorCode);

/////////////////////////////////////////////////////////////////////////////
	fnAPL_ClerkInformation();									// Clerk Information
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// Check Host Line
int CDevCmn::fnAPL_CheckHostLine()
{
	if ((HostStatus == HOST_OFFLINE) ||
		(HostStatus == HOST_WAITLINK))
		return FALSE;
	return TRUE;
}

// Check Host Open
int CDevCmn::fnAPL_CheckHostOpen()
{
	if (HostStatus != HOST_ONLINE)
		return FALSE;
	return TRUE;
}

// Close Line
int CDevCmn::fnAPL_CloseLine()
{
	HostStatus = HOST_OFFLINE;

	if (AtmStatus == ATM_READY)									// Ready Mode
		;
	else														// Other Mode : 2004.06.30
	{
		HostOpenRetryCnt = 0;
		Timer.HostOpenRetryTime = 5;
	}

	HostOpenFlag = FALSE;										// 2004.06.22
	return TRUE;
}

// Close Host
int CDevCmn::fnAPL_CloseHost()
{
	if ((HostStatus == HOST_READY)		||						// 2007.06.06
		(HostStatus == HOST_WAITONLINE)	||
		(HostStatus == HOST_ONLINE))
		HostStatus = HOST_WAITREADY;

	if (AtmStatus == ATM_READY)									// Ready Mode
		;
	else														// Other Mode : 2004.06.30
	{
		HostOpenRetryCnt = 0;
		Timer.HostOpenRetryTime = 5;
	}
	
	HostOpenFlag = FALSE;										// 2004.06.22
	return TRUE;
}

// Initialize Device
int CDevCmn::fnAPL_InitializeDevice(int nDevId, int nInitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[INITIALIZE] : %x,%d", nDevId, nInitFlag);

	int		nTempRecoveryDevice = DEV_NONE;
	int		nCardEjectFlag = FALSE;								// 2005.09.11

	nDevId = fnAPL_GetDefineDevice(nDevId);
	DeviceEvent |= nDevId;										// 2004.08.23
	nTempRecoveryDevice = fnAPL_GetAvailErrorDevice(nDevId);

	fnSPL_SetSegment("88888888");
	fnAPL_DeviceSetFlicker(DEV_MAIN, FLICKER_OFF);

/////////////////////////////////////////////////////////////////////////////
	if (nDevId & DEV_SPR)										// 2005.04.03
	{
		fnAPL_CheckDeviceAction(DEV_SPR);
	}

	if (nDevId & DEV_JPR)										// 2005.04.03
	{
		fnAPL_CheckDeviceAction(DEV_JPR);
	}

	if (nDevId & DEV_MCU)										// 2005.04.03
	{
		fnAPL_CheckDeviceAction(DEV_MCU);
	}

	if (nDevId & DEV_CSH)										// 2005.04.03
	{
		fnAPL_CheckDeviceAction(DEV_CSH);
	}

	if (nDevId & DEV_PBM)										// 2005.04.03
	{
		fnAPL_CheckDeviceAction(DEV_PBM);
	}
	
	if (nDevId & DEV_MAIN)										// 2005.04.03
	{
		fnAPL_ClearError();										// 2005.04.03
	}

/////////////////////////////////////////////////////////////////////////////
	if ((fnDOR_GetDeviceStatus() == NODEVICE)	||				// 2005.04.19
		(fnDOR_GetDeviceStatus() == DOWN))
	{
		fnDOR_ClearErrorCode();
		fnDOR_Initialize();
	}
	if ((fnLGT_GetDeviceStatus() == NODEVICE)	||
		(fnLGT_GetDeviceStatus() == DOWN))
	{
		fnLGT_ClearErrorCode();
		fnLGT_Initialize();
	}
	if ((fnSPL_GetDeviceStatus() == NODEVICE)	||
		(fnSPL_GetDeviceStatus() == DOWN))
	{
		fnSPL_ClearErrorCode();
		fnSPL_Initialize();
	}
	if ((fnPIN_GetDeviceStatus() == NODEVICE)	||
		(fnPIN_GetDeviceStatus() == DOWN))
	{
		fnPIN_ClearErrorCode();
		fnPIN_Initialize();
	}
	if ((fnSNS_GetDeviceStatus() == NODEVICE)	||
		(fnSNS_GetDeviceStatus() == DOWN))
	{
		fnSNS_ClearErrorCode();
		fnSNS_Initialize();
	}

	if ((fnCMR_GetDeviceStatus() == NODEVICE)	||				// 2005.04.19
		(fnCMR_GetDeviceStatus() == DOWN))
	{
		fnCMR_ClearErrorCode();
		fnCMR_Initialize();
	}
	if ((fnUPS_GetDeviceStatus() == NODEVICE)	||
		(fnUPS_GetDeviceStatus() == DOWN))
	{
		fnUPS_ClearErrorCode();
		fnUPS_Initialize();
	}
	if ((fnVFD_GetDeviceStatus() == NODEVICE)	||				// 2005.04.19
		(fnVFD_GetDeviceStatus() == DOWN))
	{
		fnVFD_ClearErrorCode();
		fnVFD_Initialize();
	}
	if ((fnSCR_GetDeviceStatus() == NODEVICE)	||
		(fnSCR_GetDeviceStatus() == DOWN))
	{
		fnSCR_ClearErrorCode();
		fnSCR_Initialize();
	}
	if ((fnRIC_GetDeviceStatus() == NODEVICE)	||
		(fnRIC_GetDeviceStatus() == DOWN))
	{
		fnRIC_ClearErrorCode();
		fnRIC_Initialize();
	}
	if ((fnIRD_GetDeviceStatus() == NODEVICE)	||
		(fnIRD_GetDeviceStatus() == DOWN))
	{
		fnIRD_ClearErrorCode();
		fnIRD_Initialize();
	}
/////////////////////////////////////////////////////////////////////////////
	if (nDevId & DEV_SPR)										// 2004.11.05
	{
		DeviceTimeout &= ~DEV_SPR;								// 2004.11.05
		fnSPR_ClearErrorCode();
		fnSPR_Initialize();
		switch (AtmDefine.MachineKind)							// 2006.02.09
		{
			case NANOCASH:
				break;
			case MCD3:
				fnAPL_CheckDeviceAction(DEV_SPR);
				break;
			case TTW:
				fnAPL_CheckDeviceAction(DEV_SPR);
				break;
			case MCD4:
				break;
			case TTW2:
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
	}

	if (nDevId & DEV_JPR)										// 2004.11.05
	{
		DeviceTimeout &= ~DEV_JPR;								// 2004.11.05
		fnJPR_ClearErrorCode();
		fnJPR_Initialize();
		switch (AtmDefine.MachineKind)							// 2006.02.09
		{
			case NANOCASH:
				break;
			case MCD3:
				fnAPL_CheckDeviceAction(DEV_JPR);
				break;
			case TTW:
				fnAPL_CheckDeviceAction(DEV_JPR);
				break;
			case MCD4:
				break;
			case TTW2:
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
	}

	if (nDevId & DEV_MCU)										// 2004.11.05
	{
		if (AtmDefine.McuDevice == MCU_M_OMRON)					// 2005.11.11
		{
			if (fnAPL_GetDownErrorDevice(DEV_MCU))
			{
				if (fstrMCU_GetErrorCode().Left(5) == "30020")
					nCardEjectFlag = TRUE;
			}
		}

		DeviceTimeout &= ~DEV_MCU;								// 2004.11.05
		fnMCU_ClearErrorCode();

		if (nCardEjectFlag)										// 2005.09.11
		{
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 0);
			fnMCU_Initialize();
			switch (AtmDefine.MachineKind)						// 2006.02.09
			{
				case NANOCASH:
					break;
				case MCD3:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case TTW:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case MCD4:
					break;
				case TTW2:
					break;
				case EATMGBA1:									// 2005.11.11
					break;
				case EATMULL1:									// 2005.11.11
					break;
				case EATMTTW:									// 2005.11.11
					break;
				case TTW2E:										// 2005.11.11
					break;
				case MCD4E:										// 2005.11.11
					break;
				case MX8000T:									// 2005.11.11
					break;
				case MX8000L:									// 2005.11.11
					break;
				case MX5600T:
					break;
				case MX5600L:
					break;
			}
		}
		else
		{
			switch (AtmDefine.McuDevice)						// 2005.11.11
			{
				case MCU_M_OMRON:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_M_KDE:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_M_HYOSUNG:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_M_SANKYO:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_D_MAGTEK:
					break;
				case MCU_M_SANKYO_AS:							// 2008.01.11
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
			}

			fnMCU_Initialize();
			switch (AtmDefine.MachineKind)						// 2006.02.09
			{
				case NANOCASH:
					break;
				case MCD3:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case TTW:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case MCD4:
					break;
				case TTW2:
					break;
				case EATMGBA1:									// 2005.11.11
					break;
				case EATMULL1:									// 2005.11.11
					break;
				case EATMTTW:									// 2005.11.11
					break;
				case TTW2E:										// 2005.11.11
					break;
				case MCD4E:										// 2005.11.11
					break;
				case MX8000T:									// 2005.11.11
					break;
				case MX8000L:									// 2005.11.11
					break;
				case MX5600T:
					break;
				case MX5600L:
					break;
			}
		}
	}

	if (nDevId & DEV_CSH)										// 2004.11.05
	{
		DeviceTimeout &= ~DEV_CSH;								// 2004.11.05
		fnCSH_ClearErrorCode();
		fnCSH_Initialize();
		switch (AtmDefine.MachineKind)							// 2006.02.09
		{
			case NANOCASH:
				break;
			case MCD3:
				fnAPL_CheckDeviceAction(DEV_CSH);
				break;
			case TTW:
				fnAPL_CheckDeviceAction(DEV_CSH);
				break;
			case MCD4:
				break;
			case TTW2:
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
	}

	if (nDevId & DEV_PBM)										// 2004.11.05
	{
		DeviceTimeout &= ~DEV_PBM;								// 2004.11.05
		fnPBM_ClearErrorCode();
		fnPBM_Initialize();
		switch (AtmDefine.MachineKind)							// 2006.02.09
		{
			case NANOCASH:
				break;
			case MCD3:
				fnAPL_CheckDeviceAction(DEV_PBM);
				break;
			case TTW:
				fnAPL_CheckDeviceAction(DEV_PBM);
				break;
			case MCD4:
				break;
			case TTW2:
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
	}

/////////////////////////////////////////////////////////////////////////////
	if (nDevId & DEV_SPR)
	{
		fnAPL_CheckDeviceAction(DEV_SPR);						// 2004.07.26
	}

	if (nDevId & DEV_JPR)
	{
		fnAPL_CheckDeviceAction(DEV_JPR);						// 2004.07.26
	}

	if (nDevId & DEV_MCU)
	{
		fnAPL_CheckDeviceAction(DEV_MCU);						// 2005.09.11
		switch (AtmDefine.McuDevice)							// 2005.11.11
		{
			case MCU_M_OMRON:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_M_KDE:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_M_HYOSUNG:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_M_SANKYO:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_D_MAGTEK:
				break;
			case MCU_M_SANKYO_AS:								// 2008.01.11
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
		}

//		if ((!nInitFlag) &&										// Not First
//			(RecoveryType == 0))								// Auto Recovery
//		{
//			if (fnAPL_CheckDeviceAction(DEV_MCU))
//			{
//				if (fnMCU_GetMaterialInfo())					// Card Eject
//				{
//					DeviceTimeout &= ~DEV_MCU;
//					fnMCU_Eject();
//					if (fnAPL_CheckDeviceAction(DEV_MCU))
//						fnMCU_WaitTaken();
//					fnAPL_CheckDeviceAction(DEV_MCU);
//				}	
//			}
//			fnMCU_Initialize();
//		}

		if (fnAPL_CheckDeviceAction(DEV_MCU))
		{
//			if (fnMCU_GetMaterialInfo() & ST_SENSOR2)			// Card Retract
//				fnMCU_Retract();
			if (fnMCU_GetMaterialInfo())						// 2004.03.18
			{
				if (nCardEjectFlag)								// 2005.09.11
					Delay_Msg(5000);

				fnMCU_Retract();
			}
		}
		if (fnAPL_CheckDeviceAction(DEV_MCU))
		{
			if ((!(DeviceTimeout & DEV_MCU))	&&
				(nTempRecoveryDevice == DEV_MCU))
				fnAPL_ClearError();
		}
	}

	if (nDevId & DEV_CSH)
	{
		fnAPL_CheckDeviceAction(DEV_CSH);						// 2004.07.26
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
				fnAPL_CheckDeviceAction(DEV_CSH);
				Delay_Msg(2000);								// Sensor Wait(2000ms)
				fnAPL_CheckDeviceAction(DEV_CSH);

				if (fnCSH_GetDeviceStatus() != NODEVICE)		// 2008.04.10
				{
					if (fnCSH_GetMaterialInfo())
					{
						fnCSH_Retract();
						fnAPL_CheckDeviceAction(DEV_CSH);
					}
				}
				break;
			case CDU_SHUTTER1510_SET:
			case CDU_SHUTTER20_SET:
			case CDU_SHUTTER30_SET:
				break;
			case CDU_RETRACT20_SET:
			case CDU_RETRACT30_SET:
			case CDU_PRESENT20_SET:
			case CDU_PRESENT30_SET:
			case CDU_GPRESENT20_SET:
			case CDU_GPRESENT30_SET:
				fnAPL_CheckDeviceAction(DEV_CSH);
				Delay_Msg(2000);								// Sensor Wait(2000ms)
				fnAPL_CheckDeviceAction(DEV_CSH);

				if (fnCSH_GetDeviceStatus() != NODEVICE)		// 2005.04.19
				{
//					if (fnCSH_GetMaterialInfo())				// 2010.10.20
//					{
						fnCSH_Retract();
						fnAPL_CheckDeviceAction(DEV_CSH);
// 						fnCSH_Initialize();						// G-CDU : 2005.04.19
// 						fnAPL_CheckDeviceAction(DEV_CSH);		// 2006.02.09
//					}
				}
				break;
			case CDU_TRAY20_SET:
			case CDU_TRAY30_SET:
				break;
		}
	}

	if (nDevId & DEV_PBM)
	{
//		if ((!nInitFlag) &&										// Not First
//			(RecoveryType == 0))								// Auto Recovery
//		{
//			if (fnAPL_CheckDeviceAction(DEV_PBM))
//			{
//				if (fnPBM_GetMaterialInfo())					// Pbm Eject
//				{
//					DeviceTimeout &= ~DEV_PBM;
//					fnPBM_Eject();
//					if (fnAPL_CheckDeviceAction(DEV_PBM))
//						fnPBM_WaitTaken();
//					fnAPL_CheckDeviceAction(DEV_PBM);
//				}	
//			}
//			fnPBM_Initialize();
//		}

		if (fnAPL_CheckDeviceAction(DEV_PBM))
		{
//			if (fnPBM_GetMaterialInfo() & ST_SENSOR2)			// Pbm Retract
//				fnPBM_Retract();
			if (fnPBM_GetMaterialInfo())						// 2004.03.18
				fnPBM_Retract();								
		}
		if (fnAPL_CheckDeviceAction(DEV_PBM))
		{
			if ((!(DeviceTimeout & DEV_PBM))	&&
				(nTempRecoveryDevice == DEV_PBM))
				fnAPL_ClearError();
		}
	}

	fnAPL_CheckDeviceAction(nDevId);

	Delay_Msg(2000);											// Dummy Device Execute Wait(2000ms) : 2004.07.26
	fnAPL_CheckDevice();										// Check Device
	fnAPL_CheckRetractDevice(nDevId, nInitFlag);				// Check Retract Device

	if (fnAPL_GetErrorDevice(nDevId))
		return FALSE;
	else
		return TRUE;
}

// Initialize Device Mcu
int CDevCmn::fnAPL_InitializeDeviceMcu(int nDevId, int nInitFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[MCUINITIALIZE] : %x,%d", nDevId, nInitFlag);

	int		nTempRecoveryDevice = DEV_NONE;
	int		nCardEjectFlag = FALSE;								// 2005.09.11

	nDevId = fnAPL_GetDefineDevice(nDevId);
	DeviceEvent |= nDevId;										// 2004.08.23
	nTempRecoveryDevice = fnAPL_GetAvailErrorDevice(nDevId);

	fnSPL_SetSegment("88888888");
	fnAPL_DeviceSetFlicker(DEV_MAIN, FLICKER_OFF);

/////////////////////////////////////////////////////////////////////////////
	if (nDevId & DEV_MCU)										// 2005.04.03
	{
		fnAPL_CheckDeviceAction(DEV_MCU);
	}

	if (nDevId & DEV_MAIN)										// 2005.04.03
	{
		fnAPL_ClearError();										// 2005.04.03
	}

/////////////////////////////////////////////////////////////////////////////
	if (nDevId & DEV_MCU)										// 2004.11.05
	{
		if (AtmDefine.McuDevice == MCU_M_OMRON)					// 2005.11.11
		{
			if (fnAPL_GetDownErrorDevice(DEV_MCU))
			{
				if (fstrMCU_GetErrorCode().Left(5) == "30020")
					nCardEjectFlag = TRUE;
			}
		}

		DeviceTimeout &= ~DEV_MCU;								// 2004.11.05
		fnMCU_ClearErrorCode();

		if (nCardEjectFlag)										// 2005.09.11
		{
			RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 0);
			fnMCU_Initialize();
			switch (AtmDefine.MachineKind)						// 2006.02.09
			{
				case NANOCASH:
					break;
				case MCD3:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case TTW:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case MCD4:
					break;
				case TTW2:
					break;
				case EATMGBA1:									// 2005.11.11
					break;
				case EATMULL1:									// 2005.11.11
					break;
				case EATMTTW:									// 2005.11.11
					break;
				case TTW2E:										// 2005.11.11
					break;
				case MCD4E:										// 2005.11.11
					break;
				case MX8000T:									// 2005.11.11
					break;
				case MX8000L:									// 2005.11.11
					break;
				case MX5600T:
					break;
				case MX5600L:
					break;
			}
		}
		else
		{
			switch (AtmDefine.McuDevice)						// 2005.11.11
			{
				case MCU_M_OMRON:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_M_KDE:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_M_HYOSUNG:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_M_SANKYO:
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
				case MCU_D_MAGTEK:
					break;
				case MCU_M_SANKYO_AS:							// 2008.01.11
					RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
					break;
			}

			fnMCU_Initialize();
			switch (AtmDefine.MachineKind)						// 2006.02.09
			{
				case NANOCASH:
					break;
				case MCD3:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case TTW:
					fnAPL_CheckDeviceAction(DEV_MCU);
					break;
				case MCD4:
					break;
				case TTW2:
					break;
				case EATMGBA1:									// 2005.11.11
					break;
				case EATMULL1:									// 2005.11.11
					break;
				case EATMTTW:									// 2005.11.11
					break;
				case TTW2E:										// 2005.11.11
					break;
				case MCD4E:										// 2005.11.11
					break;
				case MX8000T:									// 2005.11.11
					break;
				case MX8000L:									// 2005.11.11
					break;
				case MX5600T:
					break;
				case MX5600L:
					break;
			}
		}
	}


/////////////////////////////////////////////////////////////////////////////
	if (nDevId & DEV_MCU)
	{
		fnAPL_CheckDeviceAction(DEV_MCU);						// 2005.09.11
		switch (AtmDefine.McuDevice)							// 2005.11.11
		{
			case MCU_M_OMRON:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_M_KDE:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_M_HYOSUNG:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_M_SANKYO:
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
			case MCU_D_MAGTEK:
				break;
			case MCU_M_SANKYO_AS:								// 2008.01.11
				RegSetInt(_REGKEY_MCUSTATUS, "InitPosition", 1);
				break;
		}

		if (fnAPL_CheckDeviceAction(DEV_MCU))
		{
//			if (fnMCU_GetMaterialInfo() & ST_SENSOR2)			// Card Retract
//				fnMCU_Retract();
			if (fnMCU_GetMaterialInfo())						// 2004.03.18
			{
				if (nCardEjectFlag)								// 2005.09.11
					Delay_Msg(5000);
			}
		}
		if (fnAPL_CheckDeviceAction(DEV_MCU))
		{
			if ((!(DeviceTimeout & DEV_MCU))	&&
				(nTempRecoveryDevice == DEV_MCU))
				fnAPL_ClearError();
		}
	}

	fnAPL_CheckDeviceAction(nDevId);

	Delay_Msg(2000);											// Dummy Device Execute Wait(2000ms) : 2004.07.26
	fnAPL_CheckDevice();										// Check Device
	fnAPL_CheckRetractDevice(nDevId, nInitFlag);				// Check Retract Device

	if (fnAPL_GetErrorDevice(nDevId))
		return FALSE;
	else
		return TRUE;
}

// Check Device
int CDevCmn::fnAPL_CheckDevice()
{
//	int		nDevId;
	CString	strMinCntData("");									// 2004.11.05
	CString	strMaxCntData("");
	int		nDepType = DEP_NOT_AVAILABLE;						// 2008.04.10

/////////////////////////////////////////////////////////////////////////////
	// No Device Check
	if (fnDOR_GetDeviceStatus() == NODEVICE)					// 2004.11.05
		DeviceNoDevice |= DEV_DOR;
	else
		DeviceNoDevice &= ~DEV_DOR;

	if (fnLGT_GetDeviceStatus() == NODEVICE)					// 2005.03.30
		DeviceNoDevice |= DEV_LGT;
	else
		DeviceNoDevice &= ~DEV_LGT;

	if (fnSPL_GetDeviceStatus() == NODEVICE)					// 2005.03.30
		DeviceNoDevice |= DEV_SPL;
	else
		DeviceNoDevice &= ~DEV_SPL;

	if (fnPIN_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_PIN;
	else
		DeviceNoDevice &= ~DEV_PIN;

	if (fnSNS_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_SNS;
	else
		DeviceNoDevice &= ~DEV_SNS;

	if (fnCMR_GetDeviceStatus() == NODEVICE)					// 2005.03.30
		DeviceNoDevice |= DEV_CMR;
	else
		DeviceNoDevice &= ~DEV_CMR;

	if (fnUPS_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_UPS;
	else
		DeviceNoDevice &= ~DEV_UPS;
	
	if (fnVFD_GetDeviceStatus() == NODEVICE)					// 2005.04.19
		DeviceNoDevice |= DEV_VFD;
	else
		DeviceNoDevice &= ~DEV_VFD;

	if (fnRIC_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_RIC;
	else
		DeviceNoDevice &= ~DEV_RIC;

	if (fnIRD_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_IRD;
	else
		DeviceNoDevice &= ~DEV_IRD;

	if (fnSCR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_SCR;
	else
		DeviceNoDevice &= ~DEV_SCR;

/////////////////////////////////////////////////////////////////////////////
	if (fnSPR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_SPR;
	else
		DeviceNoDevice &= ~DEV_SPR;

	if (fnJPR_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_JPR;
	else
		DeviceNoDevice &= ~DEV_JPR;

	if (fnMCU_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_MCU;
	else
		DeviceNoDevice &= ~DEV_MCU;

	if (fnCSH_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_CSH;
	else
		DeviceNoDevice &= ~DEV_CSH;
	
	if (fnPBM_GetDeviceStatus() == NODEVICE)
		DeviceNoDevice |= DEV_PBM;
	else
		DeviceNoDevice &= ~DEV_PBM;

/////////////////////////////////////////////////////////////////////////////
	// Down Device Check
	if (fnDOR_GetDeviceStatus() == DOWN)						// 2004.11.05
		DeviceDownDevice |= DEV_DOR;
	else
		DeviceDownDevice &= ~DEV_DOR;

	if (fnLGT_GetDeviceStatus() == DOWN)						// 2005.03.30
		;
	else
		DeviceDownDevice &= ~DEV_LGT;

	if (fnSPL_GetDeviceStatus() == DOWN)						// 2005.03.30
		;
	else
		DeviceDownDevice &= ~DEV_SPL;

	if (fnPIN_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_PIN;
	else
		DeviceDownDevice &= ~DEV_PIN;

	if (fnSNS_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_SNS;
	else
		DeviceDownDevice &= ~DEV_SNS;

	if (fnCMR_GetDeviceStatus() == DOWN)						// 2005.03.30
		DeviceDownDevice |= DEV_CMR;
	else
		DeviceDownDevice &= ~DEV_CMR;

	if (fnUPS_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_UPS;
	else
		DeviceDownDevice &= ~DEV_UPS;
	
	if (fnVFD_GetDeviceStatus() == DOWN)						// 2005.04.19
		DeviceDownDevice |= DEV_VFD;
	else
		DeviceDownDevice &= ~DEV_VFD;

	if (fnRIC_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_RIC;
	else
		DeviceDownDevice &= ~DEV_RIC;

	if (fnIRD_GetDeviceStatus() == DOWN)						// 2007.09.07
		DeviceDownDevice |= DEV_IRD;
	else
		DeviceDownDevice &= ~DEV_IRD;

	if (fnSCR_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_SCR;
	else
		DeviceDownDevice &= ~DEV_SCR;

/////////////////////////////////////////////////////////////////////////////
	if (fnSPR_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_SPR;
	else
		DeviceDownDevice &= ~DEV_SPR;

	if (fnJPR_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_JPR;
	else
		DeviceDownDevice &= ~DEV_JPR;

	if (fnMCU_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_MCU;
	else
		DeviceDownDevice &= ~DEV_MCU;

	if (fnCSH_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_CSH;
	else
		DeviceDownDevice &= ~DEV_CSH;
	
	if (fnPBM_GetDeviceStatus() == DOWN)
		DeviceDownDevice |= DEV_PBM;
	else
		DeviceDownDevice &= ~DEV_PBM;

/////////////////////////////////////////////////////////////////////////////
	fnAPL_CheckError();											// 2005.04.03
/////////////////////////////////////////////////////////////////////////////
	// CST Information Procedure
	if (fnCSH_GetDeviceStatus() != NODEVICE)					// 2004.04.19
	{
		if (!CSTCnt)											// CST Information Setting
		{
			fnAPL_CheckDeviceAction(DEV_CSH);					// 2004.08.13
			CSTCnt		= fnCSH_GetNumberOfCST();
			if (CSTCnt >= 1)									// 2004.08.23
			{
				FstCSTValue	= fnCSH_GetValueOfCash(CSH_CST_1);
				FstCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_1);	// First CST Count(ClerkInformation)
			}
			if (CSTCnt >= 2)									// 2004.08.23
			{
				SndCSTValue	= fnCSH_GetValueOfCash(CSH_CST_2);
				SndCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_2);	// Second CST Count(ClerkInformation)
			}
			if (CSTCnt >= 3)									// 2004.08.23
			{
				TrdCSTValue	= fnCSH_GetValueOfCash(CSH_CST_3);
				TrdCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_3);	// Third CST Count(ClerkInformation)
			}
			if (CSTCnt >= 4)									// 2004.08.23
			{
				FthCSTValue	= fnCSH_GetValueOfCash(CSH_CST_4);
				FthCSTCnt = fnCSH_GetNumberOfCash(CSH_CST_4);	// Fourth CST Count(ClerkInformation)
			}

			fnAPL_CheckDeviceAction(DEV_CSH);					// 2004.11.05

			strMinCntData = "0";								// 2004.11.05
			switch (AtmDefine.CashHandler)						// 2006.06.02
			{
				case BRM_SET:
					strMaxCntData = Int2Asc(CSH_FULLREJECTCNT);
					break;
				case CDU_SHUTTER1510_SET:
				case CDU_SHUTTER20_SET:
				case CDU_SHUTTER30_SET:
				case CDU_RETRACT20_SET:
				case CDU_RETRACT30_SET:
				case CDU_PRESENT20_SET:
				case CDU_PRESENT30_SET:
					strMaxCntData = Int2Asc(CSH_FULLREJECTCNT);
					break;
				case CDU_GPRESENT20_SET:
				case CDU_GPRESENT30_SET:						// 2006.10.09 : LCU COUNT USE
					switch (AtmDefine.MwDevice)
					{
						case MW_KAL:
							strMaxCntData = Int2Asc(CSH_FULLREJECTCNT2);
							break;
						case MW_NEXTWARE:
						case MW_KALWOSA:
						case MW_NEXTWOSA:
							if (Asc2Int(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2))) > 300)
								IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", "300");
							else
							if (Asc2Int(IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2))) < 120)
								IniSetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", "120");
							strMaxCntData = IniGetStr(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CSHMAXREJECTCNT", Int2Asc(CSH_MAXREJECTCNT2));
							break;
					}
					break;
				case CDU_TRAY20_SET:
				case CDU_TRAY30_SET:
					strMaxCntData = Int2Asc(CSH_FULLREJECTCNT);
					break;
			}
			if (CSTCnt >= 1)									// 2004.11.05
			{
				strMinCntData += "," + Int2Asc(CSH_NEARCASHCNT);
				strMaxCntData += ",0";
			}
			if (CSTCnt >= 2)
			{
				strMinCntData += "," + Int2Asc(CSH_NEARCASHCNT);
				strMaxCntData += ",0";
			}
			if (CSTCnt >= 3)
			{
				strMinCntData += "," + Int2Asc(CSH_NEARCASHCNT);
				strMaxCntData += ",0";
			}
			if (CSTCnt >= 4)
			{
				strMinCntData += "," + Int2Asc(CSH_NEARCASHCNT);
				strMaxCntData += ",0";
			}
			
			if (CSTCnt)											// 2006.03.28
			{
				fnCSH_SetMinMaxCST(strMinCntData, strMaxCntData);
				fnAPL_CheckDeviceAction(DEV_CSH);
			}
																// BRM NOTE TYPE SET : 2008.04.10
			if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(0, 1) == "1")
				nDepType = DEP_TYPE100_ONLY;
			if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(1, 1) == "1")
				nDepType |= DEP_TYPE50_ONLY;
			if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(2, 1) == "1")
				nDepType |= DEP_TYPE20_ONLY;
			if (RegGetStr(_REGKEY_OPTION, _REGKEY_OPTION_BRMNOTETYPE).Mid(3, 1) == "1")
				nDepType |= DEP_TYPE10_ONLY;
			fnCSH_SetAvailDeposit(nDepType);
			fnAPL_CheckDeviceAction(DEV_CSH);
		}
	}
	if (!CSTCnt)												// CST No Information : 2005.04.03
	{
		DeviceDownDevice |= DEV_CSH;
		DeviceStatus |= DEV_CSH;
		if (!CurErrCnt)
			fnAPL_StackError("9799499", "CASH INFO CHECK !!!", DEV_CSH);
	}

	DeviceDownDevice |= DeviceNoDevice;							// 2004.11.05

/////////////////////////////////////////////////////////////////////////////
	// Down Device Procedure(~DEV_MAIN)							// 2005.04.03
	if (DeviceDownDevice & DEV_DOR)								
	{
		DeviceStatus |= DEV_DOR;
		if (!CurErrCnt)
			fnAPL_StackError("9799901", "DOOR CHECK !!!", DEV_DOR);
	}
	else
	{
		DeviceStatus &= ~DEV_DOR;
	}

	if (DeviceDownDevice & DEV_LGT)
	{
		DeviceStatus |= DEV_LGT;
		if (!CurErrCnt)
			fnAPL_StackError("9799902", "LIGHT CHECK !!!", DEV_LGT);
	}
	else
	{
		DeviceStatus &= ~DEV_LGT;
	}

	if (DeviceDownDevice & DEV_SPL)
	{
		DeviceStatus |= DEV_SPL;
		if (!CurErrCnt)
			fnAPL_StackError("9799903", "SPL CHECK !!!", DEV_SPL);
	}
	else
	{
		DeviceStatus &= ~DEV_SPL;
	}

	if (DeviceDownDevice & DEV_PIN)
	{
		DeviceStatus |= DEV_PIN;
		if (!CurErrCnt)
			fnAPL_StackError("9799904", "PIN CHECK !!!", DEV_PIN);
	}
	else
	{
		DeviceStatus &= ~DEV_PIN;
	}

	if (DeviceDownDevice & DEV_SNS)
	{
		DeviceStatus |= DEV_SNS;
		if (!CurErrCnt)
			fnAPL_StackError("9799905", "SENSOR CHECK !!!", DEV_SNS);
	}
	else
	{
		DeviceStatus &= ~DEV_SNS;
	}

	if (DeviceDownDevice & DEV_CMR)
	{
//		DeviceStatus |= DEV_CMR;								// 2005.04.07
//		if (!CurErrCnt)
//			fnAPL_StackError("9799906", "CAMERA CHECK !!!", DEV_CMR);
	}
	else
	{
		DeviceStatus &= ~DEV_CMR;
	}

	if (DeviceDownDevice & DEV_UPS)
	{
		DeviceStatus |= DEV_UPS;
		if (!CurErrCnt)
			fnAPL_StackError("9799907", "UPS CHECK !!!", DEV_UPS);
	}
	else
	{
		DeviceStatus &= ~DEV_UPS;
	}

	if (DeviceDownDevice & DEV_VFD)								// 2005.04.19
	{
//		DeviceStatus |= DEV_VFD;
//		if (!CurErrCnt)
//			fnAPL_StackError("9799909", "VFD CHECK !!!", DEV_VFD);
	}
	else
	{
		DeviceStatus &= ~DEV_VFD;
	}

	if (DeviceDownDevice & DEV_RIC)
	{
//		DeviceStatus |= DEV_RIC;
//		if (!CurErrCnt)
//			fnAPL_StackError("979990A", "RIC CHECK !!!", DEV_RIC);
	}
	else
	{
		DeviceStatus &= ~DEV_RIC;
	}

	if (DeviceDownDevice & DEV_IRD)
	{
//		DeviceStatus |= DEV_IRD;
//		if (!CurErrCnt)
//			fnAPL_StackError("9799911", "IRD CHECK !!!", DEV_IRD);
	}
	else
	{
		DeviceStatus &= ~DEV_IRD;
	}

	if (DeviceDownDevice & DEV_SCR)
	{
		DeviceStatus |= DEV_SCR;
		if (!CurErrCnt)
			fnAPL_StackError("9799908", "SCREEN CHECK !!!", DEV_SCR);
	}
	else
	{
		DeviceStatus &= ~DEV_SCR;
	}

/////////////////////////////////////////////////////////////////////////////
	// Down Device Procedure(DEV_MAIN)							// 2005.04.03
//	nDevId = fnAPL_GetAvailDevice(DeviceDownDevice & DEV_MAIN);	
//	if (nDevId & DEV_SPR)
//	{
//		fnAPL_StackError("9799200", "SPR CHECK !!!", DEV_SPR);
//		DeviceStatus |= DEV_SPR;
//	}
//	if (nDevId & DEV_JPR)
//	{
//		fnAPL_StackError("9799100", "JPR CHECK !!!", DEV_JPR);
//		DeviceStatus |= DEV_JPR;
//	}
//	if (nDevId & DEV_MCU)
//	{
//		fnAPL_StackError("9799300", "MCU CHECK !!!", DEV_MCU);
//		DeviceStatus |= DEV_MCU;
//	}
//	if (nDevId & DEV_CSH)
//	{
//		fnAPL_StackError("9799400", "CASH CHECK !!!", DEV_CSH);
//		DeviceStatus |= DEV_CSH;
//	}
//	if (nDevId & DEV_PBM)
//	{
//		fnAPL_StackError("9799600", "PBM CHECK !!!", DEV_PBM);
//		DeviceStatus |= DEV_PBM;
//	}

/////////////////////////////////////////////////////////////////////////////
	if (DeviceDownDevice)
		return FALSE;
	else
		return TRUE;
}

// Check Retract Device
int CDevCmn::fnAPL_CheckRetractDevice(int nDevId, int nInitFlag)
{
	int		nDeviceRetractDevice = DEV_NONE;
	int		nTempDevId = DEV_NONE;

	if (m_pProfile->TRANS.DeviceMcuRetractCnt >= 3)				// 2005.04.14
		nDeviceRetractDevice |= DEV_MCU;
	if (m_pProfile->TRANS.DevicePbmRetractCnt >= 3)
		nDeviceRetractDevice |= DEV_PBM;

	// Retract Procedure
	if ((!nInitFlag) &&											// Not First
		(RecoveryType == 0))									// Auto Recovery
		nTempDevId = fnAPL_GetAvailDevice(nDeviceRetractDevice & nDevId);
	else 
		nTempDevId = DEV_NONE;

	if (nTempDevId & DEV_MCU)
	{
		fnAPL_StackError("9799301", "MCU RETRACT OVER !!!", DEV_MCU);
		DeviceStatus |= DEV_MCU;
	}
	if (nTempDevId & DEV_PBM)
	{
		fnAPL_StackError("9799601", "PBM RETRACT OVER !!!", DEV_PBM);
		DeviceStatus |= DEV_PBM;
	}

	if (nDeviceRetractDevice)
		return FALSE;
	else
		return TRUE;
}

// Check Device Action
int CDevCmn::fnAPL_CheckDeviceAction(int nDevId)
{
	int		nMaxDevRspTime = MID_DEVRSP_TIME;					// 2005.04.19
	CString	strTempAction("");
	int		nTempTimeoutDevice = DEV_NONE;
	int		nTempErrorDevice = DEV_NONE;
	int		nTempErrorDeviceSave = DEV_NONE;
	int		CheckDev = DEV_NONE;
	int		CheckDev2 = DEV_NONE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	if (nDevId == DEV_NONE)										// 2007.09.06
		return TRUE;

	CheckDev  = nDevId & (~(DEV_RIC | DEV_IRD));
	CheckDev2 = nDevId & (DEV_RIC | DEV_IRD);

	if (nDevId & DEV_SPR)
		nMaxDevRspTime = __max(nMaxDevRspTime, SprRecvTime);
	if (nDevId & DEV_JPR)
		nMaxDevRspTime = __max(nMaxDevRspTime, JprRecvTime);
	if (nDevId & DEV_MCU)
		nMaxDevRspTime = __max(nMaxDevRspTime, McuRecvTime);
	if (nDevId & DEV_CSH)
		nMaxDevRspTime = __max(nMaxDevRspTime, CshRecvTime);
	if (nDevId & DEV_PBM)
		nMaxDevRspTime = __max(nMaxDevRspTime, PbmRecvTime);
	if (nDevId & DEV_RIC)										// 2008.07.01
		nMaxDevRspTime = __max(nMaxDevRspTime, RicRecvTime);
	if (nDevId & DEV_IRD)
		nMaxDevRspTime = __max(nMaxDevRspTime, IrdRecvTime);

	strTempAction = fstrCMN_CheckDeviceAction(CheckDev, nMaxDevRspTime);

	if (strTempAction == CHK_NORMAL)
	{
		if (CheckDev & DEV_SPR)
			DeviceStatus &= ~DEV_SPR;
		if (CheckDev & DEV_JPR)
			DeviceStatus &= ~DEV_JPR;
		if (CheckDev & DEV_MCU)
			DeviceStatus &= ~DEV_MCU;
		if (CheckDev & DEV_CSH)
			DeviceStatus &= ~DEV_CSH;
		if (CheckDev & DEV_PBM)									// 2007.09.07
			DeviceStatus &= ~DEV_PBM;

		if (CheckDev2 & DEV_RIC)
		{
			strTempAction = fstrRIC_CheckDeviceAction(nMaxDevRspTime);
			if (strTempAction == CHK_NORMAL)
				DeviceStatus &= ~DEV_RIC;
			else
			{
				DeviceStatus |= DEV_RIC;
				fnAPL_StackError(fstrRIC_GetErrorCode(), "RIC CHECK !!!", DEV_RIC);
			}
		}
		if (CheckDev2 & DEV_IRD)
		{
			strTempAction = fstrIRD_CheckDeviceAction(nMaxDevRspTime);
			if (strTempAction == CHK_NORMAL)
				DeviceStatus &= ~DEV_IRD;
			else
			{
				DeviceStatus |= DEV_IRD;
				fnAPL_StackError(fstrIRD_GetErrorCode(), "IRD CHECK !!!", DEV_IRD);
			}
		}
	}
	else
	if (strTempAction == CHK_TIMEOUT)
	{
		nTempTimeoutDevice = fnCMN_GetTimeoutDevice(CheckDev);
		DeviceTimeout |= (nTempTimeoutDevice & DEV_MAIN);
	}
	else														// CHK_TIMEOVER
	{															// CHK_ERROR
		nTempErrorDevice = fnCMN_GetErrorDevice(CheckDev);
		DeviceStatus |= (nTempErrorDevice & DEV_MAIN);

		if (DeviceOperationFlag)								// 2005.05.26
		{
			DeviceOperationFlag = FALSE;

			if (nTempErrorDevice & DEV_SPR)
				fnAPL_StackError(fstrSPR_GetErrorCode(), "SPR CHECK !!!", DEV_SPR);
			if (nTempErrorDevice & DEV_JPR)
				fnAPL_StackError(fstrJPR_GetErrorCode(), "JPR CHECK !!!", DEV_JPR);
			if (nTempErrorDevice & DEV_MCU)
				fnAPL_StackError(fstrMCU_GetErrorCode(), "MCU CHECK !!!", DEV_MCU);
			if (nTempErrorDevice & DEV_CSH)
				fnAPL_StackError(fstrCSH_GetErrorCode(), "CASH CHECK !!!", DEV_CSH);
			if (nTempErrorDevice & DEV_PBM)
				fnAPL_StackError(fstrPBM_GetErrorCode(), "PBM CHECK !!!", DEV_PBM);

			nTempErrorDeviceSave = nTempErrorDevice;			// 2007.09.06
			nTempErrorDeviceSave &= ~DeviceAutoOff;
			if (nTempErrorDeviceSave)
			{
				fnCMN_ProcBackupKalTrace(GetSprintf("%s\\ERROR%s%s", _AP_TRACE_DIR, 
																  GetDate().GetBuffer(0),
																  GetTime().GetBuffer(0)));
			}
		}
	}

	if (fnAPL_GetErrorDevice(nDevId))
		return FALSE;
	else
		return TRUE;
}

// Check Device Action AutoOff
int CDevCmn::fnAPL_CheckDeviceActionAutoOff(int nDevId)
{																// 2007.09.06
	nDevId = fnAPL_GetDefineDevice(nDevId & (~DeviceAutoOff));
	return (fnAPL_CheckDeviceAction(nDevId));
}

// Check Material
int CDevCmn::fnAPL_CheckMaterial(int nDevId)
{
	int		nTempReturn = TRUE;

	nDevId = fnAPL_GetAvailDevice(nDevId);

	if (nDevId & DEV_MCU)
	{
		switch (AtmDefine.McuDevice)							// 2005.11.11
		{
			case MCU_M_OMRON:
				if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
				{												// 2007.11.01
					fnAPL_StackError("8217091", "MCU EXIST !!!", DEV_MCU);
					DeviceStatus |= DEV_MCU;
					nTempReturn = FALSE;
				}
				break;
			case MCU_M_KDE:
				if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
				{												// 2007.11.01
					fnAPL_StackError("8217091", "MCU EXIST !!!", DEV_MCU);
					DeviceStatus |= DEV_MCU;
					nTempReturn = FALSE;
				}
				break;
			case MCU_M_HYOSUNG:
				if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
				{												// 2007.11.01
					fnAPL_StackError("8217091", "MCU EXIST !!!", DEV_MCU);
					DeviceStatus |= DEV_MCU;
					nTempReturn = FALSE;
				}
				break;
			case MCU_M_SANKYO:
				if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
				{												// 2007.11.01
					fnAPL_StackError("8217091", "MCU EXIST !!!", DEV_MCU);
					DeviceStatus |= DEV_MCU;
					nTempReturn = FALSE;
				}
				break;
			case MCU_D_MAGTEK:
				break;
			case MCU_M_SANKYO_AS:								// 2008.01.11
				if (fnMCU_GetMaterialInfo() & ST_SENSOR2)
				{												// 2007.11.01
					fnAPL_StackError("8217091", "MCU EXIST !!!", DEV_MCU);
					DeviceStatus |= DEV_MCU;
					nTempReturn = FALSE;
				}
				break;
		}
	}

	if (nDevId & DEV_CSH)
	{
		if (fnCSH_GetMaterialInfo()) 
		{														// 2007.11.01
			fnAPL_StackError("8216091", "CASH EXIST !!!", DEV_CSH);
			DeviceStatus |= DEV_CSH;
			nTempReturn = FALSE;
		}
	}

	if (nDevId & DEV_PBM)
	{
		if (fnPBM_GetMaterialInfo() & ST_SENSOR2)
		{														// 2007.11.01
			fnAPL_StackError("8218091", "PBM EXIST !!!", DEV_PBM);
			DeviceStatus |= DEV_PBM;
			nTempReturn = FALSE;
		}
	}

	return nTempReturn;
}

// Clear Retract Count
int	CDevCmn::fnAPL_ClearRetractCnt(int nDevId)
{
	if (nDevId & DEV_SPR)
	{
		if (fnAPL_GetYesDevice(DEV_SPR))
			fnSPR_ClearRetractCnt();
	}
	
	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))
			fnMCU_ClearRetractCnt();
	}
	
	if (nDevId & DEV_PBM)
	{
		if (fnAPL_GetYesDevice(DEV_PBM))
			fnPBM_ClearRetractCnt();
	}
	
	return TRUE;
}

// Retract Material
int	CDevCmn::fnAPL_RetractMaterial(int nDevId)
{
	int		SprRetractCnt = -1;									// Default -1
	int		McuRetractCnt = -1;
	int		PbmRetractCnt = -1;
	int		CshRetractCnt = -1;									// 2005.04.19
	int		CshRetractFlag = FALSE;
	CStringArray strTempRetractArray;
	
	if (fnAPL_GetYesDevice(DEV_SPR))
		SprRetractCnt = Asc2Int(fstrSPR_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(fstrMCU_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(fstrPBM_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_CSH))							// 2005.04.19
		CshRetractCnt = fnCSH_GetNumberOfCash(CSH_CST_REJECT);
	
	nDevId = fnAPL_GetAvailDevice(nDevId);
	
	if (nDevId & DEV_SPR)
	{
		if (fnSPR_GetMaterialInfo())
			fnSPR_Retract();
	}
	
	if (nDevId & DEV_MCU)
	{
		if (fnMCU_GetMaterialInfo())
			fnMCU_Retract();
	}
	
	if (nDevId & DEV_PBM)
	{
		if (fnPBM_GetMaterialInfo())
			fnPBM_Retract();
	}
	
	if (nDevId & DEV_CSH)										// 2004.06.30
	{
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
				if (fnCSH_GetMaterialInfo())
					fnCSH_Retract();							// 2008.04.10
				break;
			case CDU_SHUTTER1510_SET:
			case CDU_SHUTTER20_SET:
			case CDU_SHUTTER30_SET:
				break;
			case CDU_RETRACT20_SET:
			case CDU_RETRACT30_SET:
			case CDU_PRESENT20_SET:
			case CDU_PRESENT30_SET:
			case CDU_GPRESENT20_SET:
			case CDU_GPRESENT30_SET:
				if (fnCSH_GetMaterialInfo())
				{
					CshRetractFlag = TRUE;						// 2008.09.01

					CashRetractCnt++;							// 2008.06.04
					IniSetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CSHRETRACTCNT, CashRetractCnt);

					fnCSH_Retract();
				}
				break;
			case CDU_TRAY20_SET:
			case CDU_TRAY30_SET:
				break;
		}
	}
	
	fnAPL_CheckDeviceAction(nDevId);

	if (nDevId & DEV_CSH)
	{
		fnAPL_CheckDeviceAction(DEV_CSH);
		Delay_Msg(2000);										// Sensor Wait(2000ms)
		fnAPL_CheckDeviceAction(DEV_CSH);
	}

	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))
		{
			if ((McuRetractCnt >= 0)	&&
				(McuRetractCnt < Asc2Int(fstrMCU_GetRetractCnt())))
			{
				DeviceRetract |= DEV_MCU;
//				fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [LOST : CARD RETRACT]",
//												GetTime().GetBuffer(0),
//												&(GetTime().GetBuffer(0)[2]),
//												&(GetTime().GetBuffer(0)[4])));
			}
		}
	}
	
	if (nDevId & DEV_PBM)
	{
		if (fnAPL_GetYesDevice(DEV_PBM))
		{
			if ((PbmRetractCnt >= 0)	&&
				(PbmRetractCnt < Asc2Int(fstrPBM_GetRetractCnt())))
			{
				DeviceRetract |= DEV_PBM;
//				fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [LOST : PBM RETRACT]",
//												GetTime().GetBuffer(0),
//												&(GetTime().GetBuffer(0)[2]),
//												&(GetTime().GetBuffer(0)[4])));
			}
		}
	}
	
	if (nDevId & DEV_CSH)
	{
		if (fnAPL_GetYesDevice(DEV_CSH))
		{
			switch (AtmDefine.CashHandler)
			{
				case BRM_SET:
					SplitString(fstrCSH_GetRetractResult(), ",", strTempRetractArray);
					if ((Asc2Int(strTempRetractArray[0]) == 0)	&&			// Retract Total Amount
						(Asc2Int(strTempRetractArray[1]) == 0)	&&			// Retract Hundred Count
						(Asc2Int(strTempRetractArray[2]) == 0)	&&			// Retract Fifty Count
						(Asc2Int(strTempRetractArray[3]) == 0)	&&			// Retract Twenty Count
						(Asc2Int(strTempRetractArray[4]) == 0)	&&			// Retract Ten Count
						(Asc2Int(strTempRetractArray[5]) == 0));			// Retract Unknown Count
					else 
					{
						DeviceRetract |= DEV_CSH;
						fnAPL_PrintCashMoveInfo(CSH_LOST,					// Lost
										Asc2Int(strTempRetractArray[0]),	// Retract Total Amount
										Asc2Int(strTempRetractArray[1]),	// Retract Hundred Count
										Asc2Int(strTempRetractArray[2]),	// Retract Fifty Count
										Asc2Int(strTempRetractArray[3]),	// Retract Twenty Count
										Asc2Int(strTempRetractArray[4]),	// Retract Ten Count
										Asc2Int(strTempRetractArray[5]));	// Retract Unknown Count
					}
					break;
				case CDU_SHUTTER1510_SET:
				case CDU_SHUTTER20_SET:
				case CDU_SHUTTER30_SET:
					break;
				case CDU_RETRACT20_SET:
				case CDU_RETRACT30_SET:
				case CDU_PRESENT20_SET:
				case CDU_PRESENT30_SET:
				case CDU_GPRESENT20_SET:
				case CDU_GPRESENT30_SET:
//					if ((CshRetractCnt >= 0)	&&				// 2008.09.01
//						(CshRetractCnt < fnCSH_GetNumberOfCash(CDU_CST_REJECT)))
//					{
//						DeviceRetract |= DEV_CSH;
//						fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [LOST : CASH RETRACT]",
//														GetTime().GetBuffer(0),
//														&(GetTime().GetBuffer(0)[2]),
//														&(GetTime().GetBuffer(0)[4])));
//					}

					if (CshRetractFlag == TRUE)					// 2008.09.01
					{
						DeviceRetract |= DEV_CSH;
						fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [LOST : CASH RETRACT]",
														GetTime().GetBuffer(0),
														&(GetTime().GetBuffer(0)[2]),
														&(GetTime().GetBuffer(0)[4])));
					}
					break;
				case CDU_TRAY20_SET:
				case CDU_TRAY30_SET:
					break;
			}
		}
	}
	
	return TRUE;
}

// Auto Off Device													
int CDevCmn::fnAPL_AutoOffDevice(int nDevId)
{
	int		nTempAutoOffDevice = DEV_NONE;

	if (nDevId & fnAPL_GetAutoOffBankDevice(DEV_SPR))			// 2005.11.07
	{
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_SPR)
			nTempAutoOffDevice |= DEV_SPR;
	}

	if (nDevId & fnAPL_GetAutoOffBankDevice(DEV_JPR))			// 2005.11.07
	{
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_JPR)
			nTempAutoOffDevice |= DEV_JPR;
	}

	if (nDevId & fnAPL_GetAutoOffBankDevice(DEV_CSH))			// 2005.11.07
	{
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_CSH)
			nTempAutoOffDevice |= DEV_CSH;
	}

	if (nDevId & fnAPL_GetAutoOffBankDevice(DEV_PBM))			// 2005.11.07
	{
		if (fnAPL_GetAvailErrorDevice(DeviceStatus) & DEV_PBM)
			nTempAutoOffDevice |= DEV_PBM;
	}

	if ((nTempAutoOffDevice)	&&								// 2005.11.07
		(nTempAutoOffDevice == fnAPL_GetAvailErrorDevice(DeviceStatus)))
		fnAPL_ClearError();
	
	if (nTempAutoOffDevice)										// 2005.11.07
		DeviceAutoOff |= nTempAutoOffDevice;

	return TRUE;
}

// Auto Recover Check
int CDevCmn::fnAPL_AutoRecoverCheck(int nDevId)
{
	CString	strErrorCode("");
	
	switch (AtmDefine.McuDevice)								// 2005.11.11
	{
		case MCU_M_OMRON:
			if (nDevId & DEV_MCU)
			{
				if (fnAPL_GetDeviceEvent(DEV_MCU))
				{
					strErrorCode = fstrMCU_GetErrorCode();
					if (strErrorCode == "9723111")				// 2008.08.05
					{
						fnMCU_ClearErrorCode();
						return FALSE;
					}
				}
			}
			break;
		case MCU_M_KDE:
			if (nDevId & DEV_MCU)
			{
				if (fnAPL_GetDeviceEvent(DEV_MCU))
				{
					strErrorCode = fstrMCU_GetErrorCode();
					if (strErrorCode == "9723111")				// 2008.08.05
					{
						fnMCU_ClearErrorCode();
						return FALSE;
					}
				}
			}
			break;
		case MCU_M_HYOSUNG:
			break;
		case MCU_M_SANKYO:
			if (nDevId & DEV_MCU)								// 2005.04.06
			{
				if (fnAPL_GetDeviceEvent(DEV_MCU))
				{
					strErrorCode = fstrMCU_GetErrorCode();
					if ((strErrorCode == "9723111")	||			// 2008.08.05
						(strErrorCode == "9723018"))			// 2010.03.12
					{
						fnMCU_ClearErrorCode();
						return FALSE;
					}
				}
			}
			break;
		case MCU_D_MAGTEK:
			break;
		case MCU_M_SANKYO_AS:									// 2008.01.11
			if (nDevId & DEV_MCU)								// 2005.04.06
			{
				if (fnAPL_GetDeviceEvent(DEV_MCU))
				{
					strErrorCode = fstrMCU_GetErrorCode();
					if ((strErrorCode == "9723111")	||			// 2008.08.05
						(strErrorCode == "9723018"))			// 2010.03.12
					{
						fnMCU_ClearErrorCode();
						return FALSE;
					}
				}
			}
			break;
	}

	return TRUE;
}

// Auto Recover Device
int CDevCmn::fnAPL_AutoRecoverDevice(int nDevId)
{
	switch (AtmDefine.McuDevice)								// 2005.11.11
	{
		case MCU_M_OMRON:
			if (nDevId & DEV_MCU)
			{
				fnSCR_DisplayScreen(601);
				fnAPL_DeviceEnDisable(DEV_MCU, DISABLE);
				fnAPL_CheckDeviceAction(DEV_MCU);
				fnAPL_InitializeDevice(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
		case MCU_M_KDE:
			if (nDevId & DEV_MCU)
			{
				fnSCR_DisplayScreen(601);
				fnAPL_DeviceEnDisable(DEV_MCU, DISABLE);
				fnAPL_CheckDeviceAction(DEV_MCU);
				fnAPL_InitializeDevice(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
		case MCU_M_HYOSUNG:
			break;
		case MCU_M_SANKYO:
			if (nDevId & DEV_MCU)								// 2005.04.06
			{
				fnSCR_DisplayScreen(601);

//				while (TRUE)
				for (int i=0; i<3; i++)
				{
					DeviceTimeout &= ~DEV_MCU;					// 2005.04.08
					fnMCU_Eject(K_10_WAIT);						// 2008.08.05
					if (fnAPL_CheckDeviceAction(DEV_MCU))
					{
						fnMCU_WaitTaken();
						if (fnAPL_CheckDeviceAction(DEV_MCU))
						{
							if (!(DeviceTimeout & DEV_MCU))
								break;
						}
					}
					Delay_Msg(5000);							// 2005.04.19
				}

				fnAPL_InitializeDevice(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
		case MCU_D_MAGTEK:
			break;
		case MCU_M_SANKYO_AS:									// 2008.01.11
			if (nDevId & DEV_MCU)								// 2005.04.06
			{
				fnSCR_DisplayScreen(601);

//				while (TRUE)
				for (int i=0; i<3; i++)
				{
					DeviceTimeout &= ~DEV_MCU;					// 2005.04.08
					fnMCU_Eject(K_10_WAIT);						// 2008.08.05
					if (fnAPL_CheckDeviceAction(DEV_MCU))
					{
						fnMCU_WaitTaken();
						if (fnAPL_CheckDeviceAction(DEV_MCU))
						{
							if (!(DeviceTimeout & DEV_MCU))
								break;
						}
					}
					Delay_Msg(5000);							// 2005.04.19
				}

				fnAPL_InitializeDevice(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
	}

	return TRUE;
}

// Auto Recover Mcu
int CDevCmn::fnAPL_AutoRecoverDeviceMcu(int nDevId)
{
	switch (AtmDefine.McuDevice)								// 2005.11.11
	{
		case MCU_M_OMRON:
			if (nDevId & DEV_MCU)
			{
				fnSCR_DisplayScreen(601);
				fnAPL_DeviceEnDisable(DEV_MCU, DISABLE);
				fnAPL_CheckDeviceAction(DEV_MCU);
				fnAPL_InitializeDevice(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
		case MCU_M_KDE:
			if (nDevId & DEV_MCU)
			{
				fnSCR_DisplayScreen(601);
				fnAPL_DeviceEnDisable(DEV_MCU, DISABLE);
				fnAPL_CheckDeviceAction(DEV_MCU);
				fnAPL_InitializeDevice(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
		case MCU_M_HYOSUNG:
			break;
		case MCU_M_SANKYO:
			if (nDevId & DEV_MCU)								// 2005.04.06
			{
				fnAPL_InitializeDeviceMcu(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
		case MCU_D_MAGTEK:
			break;
		case MCU_M_SANKYO_AS:									// 2008.01.11
			if (nDevId & DEV_MCU)								// 2005.04.06
			{

				fnAPL_InitializeDeviceMcu(DEV_MCU);
				fnAPL_CheckDeviceAction(DEV_MCU);
				if (fnAPL_GetAvailErrorDevice(DEV_MCU))
					return FALSE;
			}
			break;
	}

	return TRUE;
}



// Display Normal Segment
int CDevCmn::fnAPL_DisplayNormalSegment()
{
	CString	strTempNormalSegment("");

	// Normal Mode : Cash Count & Check Count Display
	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
			strTempNormalSegment.Format("%4.4d%4.4d", FstCSTCnt, 0);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			strTempNormalSegment.Format("%4.4d%4.4d", FstCSTCnt	+ SndCSTCnt + TrdCSTCnt + FthCSTCnt, 0);
			break;
	}

	fnSPL_SetSegment(strTempNormalSegment);
	return TRUE;
}

// Display Error Segment
int CDevCmn::fnAPL_DisplayErrorSegment(char chProcCount, LPCTSTR szErrorCode)
{
	CString	strTempErrorSegment("");

	// Error Mode : Procedure Count & Error Code Display
	strTempErrorSegment.Format("%1.1c%7.7s", chProcCount, szErrorCode);

	fnSPL_SetSegment(strTempErrorSegment);
	return TRUE;
}

// Display ProcCount Segment													
int CDevCmn::fnAPL_DisplayProcCountSegment()
{
	if (!fnAPL_CheckError())
	{
		fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], CurErrBuff.ErrorCode);
/////////////////////////////////////////////////////////////////////////////
		fnAPL_ClerkInformation();								// Clerk Information
/////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		if ((m_pProfile->TRANS.ProcCount[0] == ' ') ||			// Clerk Operation
			(m_pProfile->TRANS.ProcCount[0] == '0'))			// Custom Wait
		{
/////////////////////////////////////////////////////////////////////////////
			fnAPL_ClerkInformation();							// Clerk Information
/////////////////////////////////////////////////////////////////////////////
			fnAPL_DisplayNormalSegment();
		}
		else
		{
			fnAPL_DisplayErrorSegment(m_pProfile->TRANS.ProcCount[0], "0000000");
/////////////////////////////////////////////////////////////////////////////
			fnAPL_ClerkInformation(FALSE, TRUE);				// Clerk Information
/////////////////////////////////////////////////////////////////////////////
		}
	}

	return TRUE;
}

// Display Information Led
int CDevCmn::fnAPL_DisplayInformationLed(int nInitFlag)
{
	CString	strTempInServiceLED			= LED_STR_OFF;			// 2005.12.01
	CString	strTempSupervisorLED		= LED_STR_OFF;
	CString	strTempErrorLED				= LED_STR_OFF;
	CString	strTempCommunicationsLED	= LED_STR_OFF;

	if (AtmStatus == ATM_INIT)									// 2005.04.19
		strTempSupervisorLED		= LED_STR_ON;
	else
	if (AtmStatus == ATM_ERROR)
		strTempErrorLED				= LED_STR_ON;
	else
	if (AtmStatus == ATM_CLERK)
		strTempSupervisorLED		= LED_STR_ON;
	else
	if (AtmStatus == ATM_READY)
		strTempSupervisorLED		= LED_STR_ON;
	else
	if (AtmStatus == ATM_CUSTOM)
		strTempInServiceLED			= LED_STR_ON;
	else
	if (AtmStatus == ATM_TRAN)
		strTempInServiceLED			= LED_STR_ON;
	else
	if (AtmStatus == ATM_DOWN)
		strTempSupervisorLED		= LED_STR_ON;

	if (fnAPL_CheckHostLine())									// 2005.04.19
		strTempCommunicationsLED	= LED_STR_ON;
	else
		strTempCommunicationsLED	= LED_STR_OFF;

	if ((nInitFlag)												||
		(strTempInServiceLED		!= InServiceLED)			||
		(strTempSupervisorLED		!= SupervisorLED)			||
		(strTempErrorLED			!= ErrorLED)				||
		(strTempCommunicationsLED	!= CommunicationsLED))
	{
		InServiceLED		= strTempInServiceLED;
		SupervisorLED		= strTempSupervisorLED;
		ErrorLED			= strTempErrorLED;
		CommunicationsLED	= strTempCommunicationsLED;
		switch (AtmDefine.MachineKind)
		{
			case NANOCASH:
				break;
			case MCD3:
				break;
			case TTW:
				break;
			case MCD4:											// 2005.04.19
				fnSPL_SetInServiceLED(strTempInServiceLED);
				fnSPL_SetSuperviosrLED(strTempSupervisorLED);
				fnSPL_SetErrorLED(strTempErrorLED);
				fnSPL_SetCommunicationsLED(strTempCommunicationsLED);
				break;
			case TTW2:											// 2005.04.19
				break;
			case EATMGBA1:										// 2005.11.11
				break;
			case EATMULL1:										// 2005.11.11
				break;
			case EATMTTW:										// 2005.11.11
				break;
			case TTW2E:											// 2005.11.11
				break;
			case MCD4E:											// 2005.11.11
				fnSPL_SetInServiceLED(strTempInServiceLED);
				fnSPL_SetSuperviosrLED(strTempSupervisorLED);
				fnSPL_SetErrorLED(strTempErrorLED);
				fnSPL_SetCommunicationsLED(strTempCommunicationsLED);
				break;
			case MX8000T:										// 2005.11.11
				break;
			case MX8000L:										// 2005.11.11
				break;
			case MX5600T:
				break;
			case MX5600L:
				fnSPL_SetCommunicationsLED(strTempCommunicationsLED);
				break;
		}
	}

	return TRUE;
}

// Display Information Indicator
int CDevCmn::fnAPL_DisplayInformationIndicator(int nInitFlag)
{
	int		nTempIndicator = IND_ALLOFF;						// 2005.12.01

	if ((AtmStatus == ATM_CUSTOM) ||
		(AtmStatus == ATM_TRAN))
		nTempIndicator = IND_TRANENABLE;
	else
		nTempIndicator = IND_ALLOFF;

	if ((nInitFlag) ||
		(nTempIndicator != Indicator))
	{
		Indicator = nTempIndicator;
		switch (AtmDefine.MachineKind)							// 2004.06.30
		{
			case NANOCASH:
				if (nTempIndicator == IND_TRANENABLE)
					fnLGT_SetFlicker(PIN_FLICKER, FLICKER_ON);
				else
					fnLGT_SetFlicker(PIN_FLICKER, FLICKER_OFF);
				break;
			case MCD3:											// 2004.06.30
				break;
			case TTW:
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case MCD4:											// 2004.06.30
				break;
			case TTW2:
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case EATMGBA1:										// 2005.11.11
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case EATMULL1:										// 2005.11.11
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case EATMTTW:										// 2005.11.11
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case TTW2E:											// 2005.11.11
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case MCD4E:											// 2005.11.11
				break;
			case MX8000T:										// 2005.11.11
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case MX8000L:										// 2005.11.11
				fnLGT_SetIndicator(nTempIndicator);
				break;
			case MX5600T:
				break;
			case MX5600L:
				break;
		}
	}

	return TRUE;
}

// Send Information Rpl
int CDevCmn::fnAPL_SendInformationRpl(int nInitFlag)
{
	return TRUE;
}

// Send Information Inform
int CDevCmn::fnAPL_SendInformationInform(int nInitFlag)
{
	return TRUE;
}

// Display & Print Error
int CDevCmn::fnAPL_DisplayPrintError(int nInitFlag)
{
	int		nTempReturn = TRUE;
	static	CString	strSaveProcCount("");
	static	CString	strSaveErrorCode("");
	static	int		nSaveClerkErrorMsgNo = 0;
	int		nTempClerkErrorMsgNo = 0;
	static	CString	strTempErrorString("");
	CString	strTempCmpErrorString("");

	if (nInitFlag)
		;
	else
	if (AtmStatus != AtmStatusSave)								// Difference Mode
		return TRUE;

	switch (AtmStatus)
	{
		case ATM_ERROR: 										// Error Mode
			if (nInitFlag)
			{
				strSaveProcCount = "";
				strSaveErrorCode = "";
			}

			fnAPL_CheckError();

			if ((AtmDefine.OpDevice == NOT_SET)		&&			// 2006.01.10
				(GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0))
				;												// Clerk Doing
			else
			if (fnSCR_GetCurrentScreenNo() != 27)
				fnSCR_DisplayScreen(27);					// 显示暂停服务   by zjd

//			if ((fnSCR_GetCurrentScreenNo() != 701)			||
//				(strSaveProcCount != CurErrBuff.ProcCount)	||
//				(strSaveErrorCode != CurErrBuff.ErrorCode))
//			{
//				if (!fnAPL_CheckError())	
//				{												// Display Error
//					memcpy(&GuideMsgTbl[2][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
//					memcpy(&GuideMsgTbl[2][ApDeviceLangMode][46], &CurErrBuff.ProcCount[0], 1);
//					memcpy(&GuideMsgTbl[2][ApDeviceLangMode][48], &CurErrBuff.ErrorCode[0], 5);
//					memcpy(&GuideMsgTbl[2][ApDeviceLangMode][54], &CurErrBuff.ErrorCode[5], 2);
//					strTempErrorString = GetString(CurErrBuff.ErrorString, 28, 28);
//					strTempErrorString.TrimLeft();
//					strTempErrorString.TrimRight();
//
//					strTempCmpErrorString = strTempErrorString;	// 2004.05.08
//					strTempCmpErrorString.MakeUpper();			// 2004.06.30
//					if ((strTempCmpErrorString.Find("CASH") >= 0)		||
//						(strTempCmpErrorString.Find("CSH") >= 0)		||
//						(strTempCmpErrorString.Find("CDU") >= 0))
//						strTempErrorString = "Stop Service !!!";
//
//					strcpy(&GuideMsgTbl[2][ApDeviceLangMode][69], strTempErrorString.GetBuffer(0));
//					fnSCR_DisplayStringArray(1, GuideMsgTbl[2][ApDeviceLangMode]);
//					fnSCR_DisplayScreen(701);					// Display Error Information
//				}
//				else
//				{
//					memcpy(&GuideMsgTbl[3][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
//					fnSCR_DisplayStringArray(1, GuideMsgTbl[3][ApDeviceLangMode]);
//					fnSCR_DisplayScreen(701);					// Display Error Information
//				}
//			}

			if ((memcmp(PrintErrBuff.ProcCount, CurErrBuff.ProcCount, 1) != 0)	||
				(memcmp(PrintErrBuff.ErrorCode, CurErrBuff.ErrorCode, 7) != 0))
			{													// 2004.07.05
				memcpy(&PrintErrBuff, &CurErrBuff, sizeof(PrintErrBuff));
				if (!fnAPL_CheckError())
				{												
					fnAPL_DisplayErrorSegment(CurErrBuff.ProcCount[0], CurErrBuff.ErrorCode);
					nTempReturn = fnAPL_PrintError(DEV_JPR);
/////////////////////////////////////////////////////////////////////////////
					fnAPL_ClerkInformation();					// Clerk Information
/////////////////////////////////////////////////////////////////////////////
				}
			}

			strSaveProcCount = CurErrBuff.ProcCount;			// Save
			strSaveErrorCode = CurErrBuff.ErrorCode;
			break;

		case ATM_CLERK:											// Clerk Mode
		case ATM_READY:											// Ready Mode
			if (nInitFlag)
				nSaveClerkErrorMsgNo = 0;

			nTempClerkErrorMsgNo = 4;							// Stop Service
			if (!OpenKey)
				nTempClerkErrorMsgNo = 5;						// Clerk Processing
			else
			if (SlipHeadStatus != HEADER_NORMAL)
				nTempClerkErrorMsgNo = 6;						// Spr Cover Opened
			else 
			if (JnlHeadStatus != HEADER_NORMAL)
				nTempClerkErrorMsgNo = 7;						// Jpr Cover Opened
			else 
			if (SlipStatus == SLIP_EMPTY_PAPER)
				nTempClerkErrorMsgNo = 8;						// Spr Empty
			else 
			if (JnlStatus == JNL_EMPTY_PAPER)
				nTempClerkErrorMsgNo = 9;						// Jpr Empty
			else 
			if ((CashMaxRetractCnt > 0)	&&
				(CashRetractCnt >= CashMaxRetractCnt))
				nTempClerkErrorMsgNo = 34;						// Cash Retract Over : 2008.06.04
			else 
			if ((CardMaxRetractCnt > 0)	&&
				(CardRetractCnt >= CardMaxRetractCnt))
				nTempClerkErrorMsgNo = 33;						// Card Retract Over : 2006.08.22
			else 
			if (CshLoc != NORMAL_POS)
				nTempClerkErrorMsgNo = 10;						// Cash Abnormal Position
			else 
			if (RejectCSTLoc != NORMAL_POS)	
				nTempClerkErrorMsgNo = 11;						// Reject Cassette Abnormal Position
			else 
			if (DoorStatus == DOOR_OPENED)
				nTempClerkErrorMsgNo = 12;						// Door Opened
			else 
			if (InternalTamperStatus == ABNORMAL_POS)			// 2012.08.24
				nTempClerkErrorMsgNo = 36;						// Status ON
			else 
			if (fnAPL_GetDownDevice(DEV_SPR))
				nTempClerkErrorMsgNo = 17;						// Spr Check
			else
			if (fnAPL_GetDownDevice(DEV_JPR))
				nTempClerkErrorMsgNo = 18;						// Jpr Check
			else
			if (fnAPL_GetDownDevice(DEV_MCU))
				nTempClerkErrorMsgNo = 19;						// Mcu Check
			else
			if (fnAPL_GetDownDevice(DEV_CSH))
				nTempClerkErrorMsgNo = 20;						// Csh Check
			else
			if (fnAPL_GetDownDevice(DEV_PBM))
				nTempClerkErrorMsgNo = 21;						// Pbm Check
			else
			if (!fnAPL_CheckHostLine())
			{
				nTempClerkErrorMsgNo = 13;						// Line Connecting
			}
			else
			if (!fnAPL_CheckHostOpen())
			{
				nTempClerkErrorMsgNo = 14;						// Host Connecting
			}
				
			if ((AtmDefine.OpDevice == NOT_SET)		&&			// 2006.01.10
				(GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0))
				;												// Clerk Doing
			else
			if (fnSCR_GetCurrentScreenNo() != 27)
				fnSCR_DisplayScreen(27);					// 显示暂停服务   by zjd

//			if ((fnSCR_GetCurrentScreenNo() != 701)				||
//				(nTempClerkErrorMsgNo != nSaveClerkErrorMsgNo))
//			{
//				memcpy(&GuideMsgTbl[nTempClerkErrorMsgNo][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
//				fnSCR_DisplayStringArray(1, GuideMsgTbl[nTempClerkErrorMsgNo][ApDeviceLangMode]);
//				fnSCR_DisplayScreen(701);						// Display Clerk Information
//			}

			nSaveClerkErrorMsgNo = nTempClerkErrorMsgNo;		// Save
			break;

		default:
			break;
	}

	return nTempReturn;
}

// Print Error
int CDevCmn::fnAPL_PrintError(int nDevId)
{
	if (!fnAPL_CheckError())
	{
		if (m_pProfile->NETWORK.InterfaceKind == 1002)			// CB(LZ) : 2008.03.13
		{
			if ((memcmp(CurErrBuff.ErrorCode, "77", 2) == 0) ||
				(memcmp(CurErrBuff.ErrorCode, "88", 2) == 0))
				return TRUE;
		}

		fnAPL_EjrSpoolPrintStart(nDevId);
		fnAPL_EjrSpoolData(nDevId, 0, 0, GetSprintf("%2.2s:%2.2s:%2.2s [MTC:%1.1c-%5.5s(%2.2s)]",
													GetTime().GetBuffer(0),
													&(GetTime().GetBuffer(0)[2]),
													&(GetTime().GetBuffer(0)[4]),
													CurErrBuff.ProcCount[0],
													&CurErrBuff.ErrorCode[0],
													&CurErrBuff.ErrorCode[5]));
		fnAPL_EjrSpoolData(nDevId, 1, 0, GetSprintf("%40.40s", 
													GetString(CurErrBuff.ErrorString, sizeof(CurErrBuff.ErrorString), 40).GetBuffer(0)));
		fnAPL_EjrSpoolEnd(nDevId);
		return (fnAPL_CheckDeviceAction(nDevId));
	}

	return TRUE;
}

// Check Print Device Position
int CDevCmn::fnAPL_CheckPrintDevicePosition()
{
	int		nTempReturn = TRUE;

	if (!fnAPL_CheckPrintCshPosition())
		nTempReturn = FALSE;
	if (!fnAPL_CheckPrintRearDoorStatus())
		nTempReturn = FALSE;

	return nTempReturn;
}

// Check Print Csh Position
int CDevCmn::fnAPL_CheckPrintCshPosition()
{
	int		nTempReturn = TRUE;
	static	int nSaveCshLoc				= NORMAL_POS;
	static	int	nSaveRejectCSTLoc		= NORMAL_POS;
	static	int	nSaveRetractCSTLoc		= NORMAL_POS;
	static	int	nSaveDepositCSTLoc		= NORMAL_POS;
	static	int	nSaveCshD100DepositLoc	= NORMAL_POS;
	static	int	nSaveCshD50DepositLoc	= NORMAL_POS;
	static	int	nSaveClerkCSTLoc		= NORMAL_POS;
	static	int	nSaveFstCSTLoc			= NORMAL_POS;
	static	int	nSaveSndCSTLoc			= NORMAL_POS;
	static	int	nSaveTrdCSTLoc			= NORMAL_POS;
	static	int	nSaveFthCSTLoc			= NORMAL_POS;

								  //0123456789012345678901234567890123456789
	char	szCshPrint[][256]	= {"  :  :   CASH NORMAL POSITION  ",			   
								   "  :  :   CASH ABNORMAL POSITION"};
	char	szRjtPrint[][256]	= {"  :  :   RJTC NORMAL POSITION  ",
								   "  :  :   RJTC ABNORMAL POSITION"};
	char	szRttPrint[][256]	= {"  :  :   RTTC NORMAL POSITION  ",
								   "  :  :   RTTC ABNORMAL POSITION"};
	char	szDepPrint[][256]	= {"  :  :   DEPC NORMAL POSITION  ",
								   "  :  :   DEPC ABNORMAL POSITION"};
	char	szD100Print[][256]	= {"  :  :   D100 NORMAL POSITION  ",
								   "  :  :   D100 ABNORMAL POSITION"};
	char	szD50Print[][256]	= {"  :  :   D50  NORMAL POSITION  ",
								   "  :  :   D50  ABNORMAL POSITION"};
	char	szCctPrint[][256]	= {"  :  :   CCST NORMAL POSITION  ",
								   "  :  :   CCST ABNORMAL POSITION"};
	char	szFstPrint[][256]	= {"  :  :   CST1 NORMAL POSITION  ",
								   "  :  :   CST1 ABNORMAL POSITION"};
	char	szSndPrint[][256]	= {"  :  :   CST2 NORMAL POSITION  ",
								   "  :  :   CST2 ABNORMAL POSITION"};
	char	szTrdPrint[][256]	= {"  :  :   CST3 NORMAL POSITION  ",
								   "  :  :   CST3 ABNORMAL POSITION"};
	char	szFthPrint[][256]	= {"  :  :   CST4 NORMAL POSITION  ",
								   "  :  :   CST4 ABNORMAL POSITION"};

	if (fnAPL_GetDefineDevice(DEV_CSH))
	{
		switch (AtmDefine.CashHandler)
		{
			case BRM_SET:
				if (nSaveCshLoc != CshLoc)
				{
					nSaveCshLoc = CshLoc;
					if (!fnAPL_PrintPosition(szCshPrint[CshLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveRejectCSTLoc != RejectCSTLoc)
				{
					nSaveRejectCSTLoc = RejectCSTLoc;
					if (!fnAPL_PrintPosition(szRjtPrint[RejectCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveRetractCSTLoc != RetractCSTLoc)
				{
					nSaveRetractCSTLoc = RetractCSTLoc;
					if (!fnAPL_PrintPosition(szRttPrint[RetractCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveDepositCSTLoc != DepositCSTLoc)
				{
					nSaveDepositCSTLoc = DepositCSTLoc;
					if (!fnAPL_PrintPosition(szDepPrint[DepositCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveCshD100DepositLoc != CshD100DepositLoc)
				{
					nSaveCshD100DepositLoc = CshD100DepositLoc;
					if (!fnAPL_PrintPosition(szD100Print[CshD100DepositLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveCshD50DepositLoc != CshD50DepositLoc)
				{
					nSaveCshD50DepositLoc = CshD50DepositLoc;
					if (!fnAPL_PrintPosition(szD50Print[CshD50DepositLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveClerkCSTLoc != ClerkCSTLoc)
				{
					nSaveClerkCSTLoc = ClerkCSTLoc;
					if (!fnAPL_PrintPosition(szCctPrint[ClerkCSTLoc]))
						nTempReturn = FALSE;
				}
				break;

			case CDU_SHUTTER1510_SET:
			case CDU_SHUTTER20_SET:
			case CDU_SHUTTER30_SET:
			case CDU_RETRACT20_SET:
			case CDU_RETRACT30_SET:
			case CDU_PRESENT20_SET:
			case CDU_PRESENT30_SET:
			case CDU_GPRESENT20_SET:
			case CDU_GPRESENT30_SET:
			case CDU_TRAY20_SET:
			case CDU_TRAY30_SET:
				if (nSaveCshLoc != CshLoc)
				{
					nSaveCshLoc = CshLoc;
					if (!fnAPL_PrintPosition(szCshPrint[CshLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveRejectCSTLoc != RejectCSTLoc)
				{
					nSaveRejectCSTLoc = RejectCSTLoc;
					if (!fnAPL_PrintPosition(szRjtPrint[RejectCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveFstCSTLoc != FstCSTLoc)
				{
					nSaveFstCSTLoc = FstCSTLoc;
					if (!fnAPL_PrintPosition(szFstPrint[FstCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveSndCSTLoc != SndCSTLoc)
				{
					nSaveSndCSTLoc = SndCSTLoc;
					if (!fnAPL_PrintPosition(szSndPrint[SndCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveTrdCSTLoc != TrdCSTLoc)
				{
					nSaveTrdCSTLoc = TrdCSTLoc;
					if (!fnAPL_PrintPosition(szTrdPrint[TrdCSTLoc]))
						nTempReturn = FALSE;
				}
				if (nSaveFthCSTLoc != FthCSTLoc)
				{
					nSaveFthCSTLoc = FthCSTLoc;
					if (!fnAPL_PrintPosition(szFthPrint[FthCSTLoc]))
						nTempReturn = FALSE;
				}
				break;
		}
	}

	return nTempReturn;
}

// Check Print RearDoor Status
int CDevCmn::fnAPL_CheckPrintRearDoorStatus()
{
	int		nTempReturn = TRUE;
	static	int nSaveDoorStatus	= DOOR_CLOSED;					// DoorStatus惑怕

								  //0123456789012345678901234567890123456789
	char	szDoorPrint[][256]	= {"  :  :   DOOR OPENED",			   
								   "  :  :   DOOR CLOSED"};

	if (nSaveDoorStatus != DoorStatus) 
	{
		nSaveDoorStatus = DoorStatus;
		if (!fnAPL_PrintPosition(szDoorPrint[DoorStatus]))
			nTempReturn = FALSE;
	}

	return nTempReturn;
}

// Print Position
int CDevCmn::fnAPL_PrintPosition(LPCTSTR szPrintData)
{
	char	szTempPrintBuffer[256];
	char	szTempTimeBuffer[256];

	strcpy(szTempPrintBuffer, szPrintData);
	GetTime(szTempTimeBuffer);
	Sprintf(szTempPrintBuffer, 8, "%2.2s:%2.2s:%2.2s", &szTempTimeBuffer[0], &szTempTimeBuffer[2], &szTempTimeBuffer[4]);

	fnAPL_EjrSpoolPrintStart(DEV_JPR);
	fnAPL_EjrSpoolData(DEV_JPR, 0, 0, szTempPrintBuffer);
	fnAPL_EjrSpoolEnd(DEV_JPR);
	return (fnAPL_CheckDeviceActionAutoOff(DEV_JPR));			// 2007.09.06
}

// Print Cash Move Information
int CDevCmn::fnAPL_PrintCashMoveInfo(int nMoveFlag, int nRealMove, int nHundredRJT, int nFiftyRJT, int nTwentyRJT, int nTenRJT, int nUnknownRJT)
{
	char	szTempPrintBuffer[256];

	if ((nRealMove == 0)		&&
		(nHundredRJT == 0)		&&
		(nFiftyRJT == 0)		&&
		(nTwentyRJT == 0)		&&
		(nTenRJT == 0)			&&
		(nUnknownRJT == 0))
	   return TRUE;

	fnAPL_EjrSpoolPrintStart(DEV_JPR);

	// line 1
	if (nMoveFlag & CSH_AUTOLOAD)
	{
		if (nMoveFlag & REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[AUTO REFILL]");
		else fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[AUTO RETRACT]");
	}
	else 
	if (nMoveFlag & CSH_MANUALLOAD)
	{
		if (nMoveFlag & REFILL_MOVE)
			fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[MANUAL REFILL]");
		else fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[MANUAL RETRACT]");
	}
	else 
	if (nMoveFlag & CSH_RESET)
		fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[RESET RETRACT]");
	else 
		fnAPL_EjrSpoolData(DEV_JPR, 0, 0, "[LOST RETRACT]");

	// line2
	if (nMoveFlag & (CSH_AUTOLOAD | CSH_MANUALLOAD))
	{
		if ((nMoveFlag & REFILL_MOVE) == REFILL_MOVE)
			sprintf(szTempPrintBuffer,  "REFILL  COUNT : %5d", nRealMove);
		else 
			sprintf(szTempPrintBuffer, "RETRACT COUNT : %5d", nRealMove);
	}
	else 
		sprintf(szTempPrintBuffer, "RETRACT AMOUNT : %10d", nRealMove);

	fnAPL_EjrSpoolData(DEV_JPR, 1, 0, szTempPrintBuffer);
																// 2008.04.10
	// line 3						   0123456789012345678901234567890123456789
	fnAPL_EjrSpoolData(DEV_JPR, 2, 0, "----- RETRACT STATUS -----");
	// line 4
	fnAPL_EjrSpoolData(DEV_JPR, 3, 0, GetSprintf("  100 RETRACT : %5d", nHundredRJT));
	// line 5
	fnAPL_EjrSpoolData(DEV_JPR, 4, 0, GetSprintf("   50 RETRACT : %5d", nFiftyRJT));
	// line 6
	fnAPL_EjrSpoolData(DEV_JPR, 5, 0, GetSprintf("   20 RETRACT : %5d", nTwentyRJT));
	// line 7
	fnAPL_EjrSpoolData(DEV_JPR, 6, 0, GetSprintf("   10 RETRACT : %5d", nTenRJT));
	// line 8
	fnAPL_EjrSpoolData(DEV_JPR, 7, 0, GetSprintf("OTHER RETRACT : %5d", nUnknownRJT));

	fnAPL_EjrSpoolEnd(DEV_JPR);
	return (fnAPL_CheckDeviceActionAutoOff(DEV_JPR));			// 2007.09.06
}

// Print Journal Information
int CDevCmn::fnAPL_PrintJnlInfo(LPCTSTR szJnlInfoData, int nWaitFlag)
{
	if (!strlen(szJnlInfoData))
		return TRUE;

	fnAPL_EjrSpoolPrintStart(DEV_JPR);
	fnAPL_EjrSpoolData(DEV_JPR, 0, 0, szJnlInfoData);
	fnAPL_EjrSpoolEnd(DEV_JPR);
	
	if (nWaitFlag)
		return (fnAPL_CheckDeviceActionAutoOff(DEV_JPR));		// 2007.09.06
	else
		return TRUE;
}

int  CDevCmn::fnAPL_AgentLineCount()   
  {   
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentLineCount()");
      FILE *fp;
	       
	  int   nCount = 0;
	  fp=fopen(szAgentCreateFilePathName,"r");
      CStdioFile   f=fp; 
	  CString      str="\n";  
	  if(fp)
	  {
		  while(f.ReadString(str))
		  {              
				nCount++;   
		  }

	      fclose(fp);
	  }
	  else 
	  {
		  //新建一个文件
		  nCount = 0;
	  }
	  
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentLineCount()return[%d]",nCount);
      return   nCount;   
 }   

//int  CDevCmn::fnAPL_AgentRunIdxGetData(LPCTSTR nAppRunType)
//{
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentRunIdxGetData()");
//    CString szAgentIdxCommonData("");
//	char szAgentIdxTranData[1024];
//	CString szAgentTranData("");
//    int iTranCode;
//	int iLineCnt;
//	int JprMode;
//	int nTempLen;
//	char szTranCode;
//
//	memset(szAgentIdxTranData,0,512);
//	szTranCode ='5';
//	iTranCode = 9;
//	nTempLen = 0;
//   	iLineCnt = fnAPL_AgentLineCount();
//	if (!fnAPL_GetDefineDevice(DEV_JPR)||          
//		JnlStatus == JNL_EMPTY_PAPER)				// 流水运行模式
//        JprMode = 2;
//	else   
//		JprMode = 3;
//
//	szAgentIdxCommonData.Format("%17.17s%1.1s%16.16s%8.8s%8.8s%1.1s%1.1d%03d%d%s%10.10s",
//		&(szAgentCreateFilePathName.GetBuffer(0)[28]), 
//	    &(szAgentCreateFilePathName.GetBuffer(0)[45]),
//		GetSprintf("0100A%3.3sA%3.3sA%3.3s",&m_pProfile->NETWORK.BranchNum[0],&m_pProfile->NETWORK.BranchNum[3],&m_pProfile->NETWORK.BranchNum[6]),
//		&m_pProfile->NETWORK.AtmSerialNum[0],
//		m_pProfile->TRANS.SerialNo,
//		&szTranCode,
//		iTranCode,
//		iLineCnt,
//		JprMode,
//		"0",                //凭条FORM形态  
//		"          ");      //预留
//
//	memcpy(&szAgentIdxTranData[nTempLen],nAppRunType,2);
//	memcpy(&szAgentIdxTranData[nTempLen+2],"          ",10);
//
//    szAgentTranData.Format("%s%s",szAgentIdxCommonData,&szAgentIdxTranData);
//MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_AgentRunIdxGetData()return[%s]",szAgentTranData);
//    fnJPR_AgentIdxPrintFile(szAgentTranData);
//
//	fnAPL_AgentCreateFile();          //生成新的日志流水文件
//
//    return TRUE;
//
//}

// Get Net MAC  20080320 BY HYL 
//void CDevCmn::fnAPL_GetNetMAC(char *pRetVal)
//{	
//	typedef struct _ASTAT_ 
//    { 
//		ADAPTER_STATUS adapt; 
//		NAME_BUFFER NameBuff [30]; 
//    }ASTAT, * PASTAT; 
//	
//    ASTAT Adapter; 
//	
//	NCB ncb; 
//	UCHAR uRetCode; 
//	
//	memset( &ncb, 0, sizeof(ncb) ); 
//	ncb.ncb_command = NCBRESET; 
//	ncb.ncb_lana_num = 0; 
//	
//	uRetCode = Netbios( &ncb ); 
//	
//	memset( &ncb, 0, sizeof(ncb) ); 
//	ncb.ncb_command = NCBASTAT; 
//	ncb.ncb_lana_num = 0; 
//	
//	strcpy( (LPTSTR)ncb.ncb_callname, "* " ); 
//	ncb.ncb_buffer = (unsigned char *) &Adapter; 
//	ncb.ncb_length = sizeof(Adapter); 
//	
//	uRetCode = Netbios( &ncb ); 
//	
//	if ( uRetCode == 0 ) 
//	{ 
//		sprintf( pRetVal,"%02x%02x%02x%02x%02x%02x", 
//			Adapter.adapt.adapter_address[0], 
//			Adapter.adapt.adapter_address[1], 
//			Adapter.adapt.adapter_address[2], 
//			Adapter.adapt.adapter_address[3], 
//			Adapter.adapt.adapter_address[4], 
//			Adapter.adapt.adapter_address[5] ); 
//	} 
//	return;
// }

// Print Journal Information Direct
int CDevCmn::fnAPL_PrintJnlInfoDirect(LPCTSTR szJnlInfoData, int nWaitFlag)
{
	if (!strlen(szJnlInfoData))
		return TRUE;

	CString		strTempPrintData("");
	CString		strTempJprPrintData("");
																// Data Make	
	strTempPrintData = GetString((LPSTR)szJnlInfoData, strlen(szJnlInfoData), JPR_MAX_COL);
	strTempPrintData.TrimRight();
	if (strTempPrintData.GetLength())
		strTempJprPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
	if (!strTempJprPrintData.GetLength())						// Jpr No Data
		strTempJprPrintData = "20";

	fnJPR_PrintFile(strTempJprPrintData);
	fnAPL_CheckDeviceActionAutoOff(DEV_JPR);					// 2007.09.06
	if ((!fnAPL_GetAvailDevice(DEV_JPR))	||					// 2005.04.19
		(fnAPL_GetDownErrorDevice(DEV_JPR))	||
		(JnlHeadStatus != HEADER_NORMAL)	||
		(JnlStatus == JNL_EMPTY_PAPER))
		fnJPR_PrintLog(strTempJprPrintData);
	else
		fnJPR_Print(strTempJprPrintData);

	if (nWaitFlag)
		return (fnAPL_CheckDeviceActionAutoOff(DEV_JPR));		// 2007.09.06
	else
		return TRUE;
}

// Auto Refill Csh
int CDevCmn::fnAPL_AutoRefillCsh(int nCshRefillMode)
{
	CStringArray strTempRefillArray;
	CStringArray strTempTakeupArray;

	if (!fnAPL_GetAvailDevice(DEV_CSH))
		return TRUE;

	switch (nCshRefillMode) 
	{
		case NORMAL:
			return TRUE;
			break;

		case NEED_REFILL:
			if (m_pProfile->NETWORK.InterfaceKind == 101)		// RCB(HF) : 2007.10.30
				memcpy(&GuideMsgTbl[15][ApDeviceLangMode][25], "********", 8);
			else
				memcpy(&GuideMsgTbl[15][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
			fnSCR_DisplayStringArray(1, GuideMsgTbl[15][ApDeviceLangMode]);
			fnSCR_DisplayScreen(701);							// Display Refilling
			fnAPL_CheckDeviceAction(DEV_CSH);
			fnCSH_RefillMoney(0);
			if (!fnAPL_CheckDeviceAction(DEV_CSH))
				return FALSE;
			fnAPL_DisplayNormalSegment();
			SplitString(fstrCSH_GetRefillResult(), ",", strTempRefillArray);
			return (fnAPL_PrintCashMoveInfo(CSH_AUTOLOAD | REFILL_MOVE,
							Asc2Int(strTempRefillArray[1]),		// Refill Act Num
							Asc2Int(strTempRefillArray[3]),		// Refill Hundred RJT Num
							Asc2Int(strTempRefillArray[4]),		// Refill Fifty RJT Num
							Asc2Int(strTempRefillArray[5]),		// Refill Twenty RJT Num
							Asc2Int(strTempRefillArray[6]),		// Refill Ten RJT Num
							Asc2Int(strTempRefillArray[7])));	// Refill Unknown RJT Num
			break;

		case NEED_TAKEUP:
			if (m_pProfile->NETWORK.InterfaceKind == 101)		// RCB(HF) : 2007.10.30
				memcpy(&GuideMsgTbl[16][ApDeviceLangMode][25], "********", 8);
			else
				memcpy(&GuideMsgTbl[16][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
			fnSCR_DisplayStringArray(1, GuideMsgTbl[16][ApDeviceLangMode]);
			fnSCR_DisplayScreen(701);							// Display Take Up
			fnAPL_CheckDeviceAction(DEV_CSH);
			fnCSH_TakeupMoney(0);
			if (!fnAPL_CheckDeviceAction(DEV_CSH))
				return FALSE;
			fnAPL_DisplayNormalSegment();
			SplitString(fstrCSH_GetTakeupResult(), ",", strTempTakeupArray);
			return (fnAPL_PrintCashMoveInfo(CSH_AUTOLOAD | TAKEUP_MOVE,
							Asc2Int(strTempTakeupArray[1]),		// Take Up Act Num
							Asc2Int(strTempTakeupArray[3]),		// Take Up Hundred RJT Num
							Asc2Int(strTempTakeupArray[4]),		// Take Up Fifty RJT Num
							Asc2Int(strTempTakeupArray[5]),		// Take Up Twenty RJT Num
							Asc2Int(strTempTakeupArray[6]),		// Take Up Ten RJT Num
							Asc2Int(strTempTakeupArray[7])));	// Take Up Unknown RJT Num
			break;

		default:
			break;
	}

	return TRUE;
}

// Auto Reset Csh
int CDevCmn::fnAPL_AutoResetCsh()
{
	if (!fnAPL_GetAvailDevice(DEV_CSH))
		return TRUE;
	fnAPL_CheckDeviceAction(DEV_CSH);

	fnCSH_Initialize();
	return (fnAPL_CheckDeviceAction(DEV_CSH));
}

// Auto Ready Csh
int CDevCmn::fnAPL_AutoReadyCsh()
{	
	if (!fnAPL_GetAvailDevice(DEV_CSH))
		return TRUE;
	fnAPL_CheckDeviceAction(DEV_CSH);

	fnCSH_Ready();
	return (fnAPL_CheckDeviceAction(DEV_CSH));
}

// Reset Device
int CDevCmn::fnAPL_ResetDevice(int nDevId, int nInitFlag)				
{
	int		SprRetractCnt = -1;									// Default -1
	int		McuRetractCnt = -1;
	int		PbmRetractCnt = -1;
	int		CshRetractCnt = -1;									// 2005.04.19
	CStringArray strTempRetractArray;

	if (fnAPL_GetYesDevice(DEV_SPR))
		SprRetractCnt = Asc2Int(fstrSPR_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_MCU))
		McuRetractCnt = Asc2Int(fstrMCU_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_PBM))
		PbmRetractCnt = Asc2Int(fstrPBM_GetRetractCnt());
	if (fnAPL_GetYesDevice(DEV_CSH))							// 2005.04.19
		CshRetractCnt = fnCSH_GetNumberOfCash(CSH_CST_REJECT);

	nDevId = fnAPL_GetDefineDevice(nDevId);

	fnAPL_InitializeDevice(nDevId, nInitFlag);

	if (nDevId & DEV_CSH)
	{
		fnAPL_CheckDeviceAction(DEV_CSH);
		Delay_Msg(2000);										// Sensor Wait(2000ms)
		fnAPL_CheckDeviceAction(DEV_CSH);
	}

	fnAPL_CheckMaterial();

	if (nDevId & DEV_MCU)
	{
		if (fnAPL_GetYesDevice(DEV_MCU))
		{
			if ((McuRetractCnt >= 0)	&&
				(McuRetractCnt < Asc2Int(fstrMCU_GetRetractCnt())))
			{
				fnAPL_AddDeviceRetractCnt(DEV_MCU);				// 2004.11.05
				DeviceRetract |= DEV_MCU;
//				fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RESET : CARD RETRACT]",
//												GetTime().GetBuffer(0),
//												&(GetTime().GetBuffer(0)[2]),
//												&(GetTime().GetBuffer(0)[4])));
			}
		}
	}

	if (nDevId & DEV_PBM)
	{
		if (fnAPL_GetYesDevice(DEV_PBM))
		{
			if ((PbmRetractCnt >= 0)	&&
				(PbmRetractCnt < Asc2Int(fstrPBM_GetRetractCnt())))
			{
				fnAPL_AddDeviceRetractCnt(DEV_PBM);				// 2004.11.05
				DeviceRetract |= DEV_PBM;
//				fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RESET : PBM RETRACT]",
//												GetTime().GetBuffer(0),
//												&(GetTime().GetBuffer(0)[2]),
//												&(GetTime().GetBuffer(0)[4])));
			}
		}
	}

	if (nDevId & DEV_CSH)										// 2004.06.30
	{
		if (fnAPL_GetYesDevice(DEV_CSH))
		{
			switch (AtmDefine.CashHandler)
			{
				case BRM_SET:
					SplitString(fstrCSH_GetRetractResult(), ",", strTempRetractArray);
					if ((Asc2Int(strTempRetractArray[0]) == 0)	&&			// Retract Total Amount
						(Asc2Int(strTempRetractArray[1]) == 0)	&&			// Retract Hundred Count
						(Asc2Int(strTempRetractArray[2]) == 0)	&&			// Retract Fifty Count
						(Asc2Int(strTempRetractArray[3]) == 0)	&&			// Retract Twenty Count
						(Asc2Int(strTempRetractArray[4]) == 0)	&&			// Retract Ten Count
						(Asc2Int(strTempRetractArray[5]) == 0));			// Retract Unknown Count
					else 
					{
						DeviceRetract |= DEV_CSH;							
						fnAPL_PrintCashMoveInfo(CSH_RESET,					// Reset
										Asc2Int(strTempRetractArray[0]),	// Retract Total Amount
										Asc2Int(strTempRetractArray[1]),	// Retract Hundred Count
										Asc2Int(strTempRetractArray[2]),	// Retract Fifty Count
										Asc2Int(strTempRetractArray[3]),	// Retract Twenty Count
										Asc2Int(strTempRetractArray[4]),	// Retract Ten Count
										Asc2Int(strTempRetractArray[5]));	// Retract Unknown Count
					}
					break;
				case CDU_SHUTTER1510_SET:
				case CDU_SHUTTER20_SET:
				case CDU_SHUTTER30_SET:
					break;
				case CDU_RETRACT20_SET:
				case CDU_RETRACT30_SET:
				case CDU_PRESENT20_SET:
				case CDU_PRESENT30_SET:
				case CDU_GPRESENT20_SET:
				case CDU_GPRESENT30_SET:
					if ((CshRetractCnt >= 0)	&&				// 2007.11.01
						(CshRetractCnt < fnCSH_GetNumberOfCash(CDU_CST_REJECT)))
					{
						DeviceRetract |= DEV_CSH;
//						fnAPL_PrintJnlInfo(GetSprintf("%2.2s:%2.2s:%2.2s [RESET : CASH RETRACT]",
//														GetTime().GetBuffer(0),
//														&(GetTime().GetBuffer(0)[2]),
//														&(GetTime().GetBuffer(0)[4])));
					}
					break;
				case CDU_TRAY20_SET:
				case CDU_TRAY30_SET:
					break;
			}
		}
	}

	if (fnAPL_GetAvailErrorDevice(nDevId))
		return FALSE;
	else
	{
		fnAPL_DisplayNormalSegment();
		return TRUE;
	}
}

// Un Used
int CDevCmn::fnAPL_UnUsed()
{
	fnAPL_SetOnTransaction(OFF);
	fnAPL_SetProcCount(' ');
	fnSCR_SetCurrentLangMode(CHN_MODE);

	switch (AtmDefine.OpDevice)									// 2005.11.11
	{
		case NOT_SET:
			if (GetShareData("OP_OPSTATUS").CompareNoCase("OPDO") == 0)
				fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM, DISABLE);
			else
				fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);
			break;
		case OP_TOUCH:
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);
			break;
		case OP_TSPL:
		case OP_TSPLCHN:
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);
			break;
		case OP_HSPL:
			fnAPL_DeviceEnDisable(DEV_MCU | DEV_PBM | DEV_PIN, DISABLE);
			break;
	}

	fnAPL_DeviceSetFlicker(DEV_MAIN, FLICKER_OFF);
	fnAPL_DisplayInformationLed(TRUE);
	fnAPL_DisplayInformationIndicator(TRUE);

	if (fnAPL_CheckDeviceAction(DEV_MCU))						// 2004.08.23
	{
		if (fnMCU_GetMaterialInfo())							// Card Eject
		{
			DeviceTimeout &= ~DEV_MCU;
			fnMCU_Eject(K_NO_WAIT);
			fnAPL_CheckDeviceAction(DEV_MCU);
		}	
	}
	return TRUE;
}

// Device EnDisable
int CDevCmn::fnAPL_DeviceEnDisable(int nDevId, int nAction, int nPinInputMode, LPCTSTR szCardData,  int nPinPassWordMin, int nPinPassWordMax, int nPinPassWordAuto, LPCTSTR szPinPassWordTerm, int nWaitFlag)
{
	int		nTempDevId = DEV_NONE;
	CString	strPinPassWordTerm(szPinPassWordTerm);

/////////////////////////////////////////////////////////////////////////////
	strPinPassWordTerm.Replace(",F1", "");						// TOUCH ONLY : 2005.06.29
	strPinPassWordTerm.Replace(",F2", "");
	strPinPassWordTerm.Replace(",F3", "");
	strPinPassWordTerm.Replace(",F4", "");
	strPinPassWordTerm.Replace(",F5", "");
	strPinPassWordTerm.Replace(",F6", "");
	strPinPassWordTerm.Replace(",F7", "");
	strPinPassWordTerm.Replace(",F8", "");
	strPinPassWordTerm.Replace("F1,", "");
	strPinPassWordTerm.Replace("F2,", "");
	strPinPassWordTerm.Replace("F3,", "");
	strPinPassWordTerm.Replace("F4,", "");
	strPinPassWordTerm.Replace("F5,", "");
	strPinPassWordTerm.Replace("F6,", "");
	strPinPassWordTerm.Replace("F7,", "");
	strPinPassWordTerm.Replace("F8,", "");
	strPinPassWordTerm.Replace("F1", "");
	strPinPassWordTerm.Replace("F2", "");
	strPinPassWordTerm.Replace("F3", "");
	strPinPassWordTerm.Replace("F4", "");
	strPinPassWordTerm.Replace("F5", "");
	strPinPassWordTerm.Replace("F6", "");
	strPinPassWordTerm.Replace("F7", "");
	strPinPassWordTerm.Replace("F8", "");
/////////////////////////////////////////////////////////////////////////////

	nDevId = fnAPL_GetAvailDevice(nDevId);
	
	nTempDevId = nDevId & DEV_MAIN;
	fnAPL_CheckDeviceAction(nTempDevId);

	if (nDevId & DEV_MCU)										// 2010.10.20
	{
		if ((AtmDefine.ICCardDevice == NOT_SET) ||
			(McuReadMSOnlyFlag))
		{
			if (nAction == ENABLE)
				fnMCU_EntryEnable();
			else 
				fnMCU_EntryDisable();
		}
		else
		{
			if (nAction == ENABLE)
				fnMCU_ICEntryEnable();
			else 
				fnMCU_ICEntryDisable();
		}
	}

	if (nDevId & DEV_PBM)
	{
		if (nAction == ENABLE)
			fnPBM_EntryEnable();
		else 
			fnPBM_EntryDisable();
	}

	if (nDevId & DEV_RIC)
	{
		if (nAction == ENABLE)
			fnRIC_EntryEnable();
		else 
			fnRIC_EntryDisable();
	}

	if (nDevId & DEV_IRD)
	{
		if (nAction == ENABLE)
			fnIRD_EntryEnable();
		else 
			fnIRD_EntryDisable();
	}

	if (nDevId & DEV_PIN)
	{
		if (nAction == ENABLE)
		{
			switch (nPinInputMode)
			{
				case PIN_DISABLE_MODE:
					fnPIN_EntryDisable();
					break;
				case PIN_MENU_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_MENU_TOUCH,		// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_MENU_PAD,		// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_NUM_AMT_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_NUM_AMT_TOUCH,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_NUM_AMT_PAD,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_NUMERIC_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_NUMERIC_TOUCH,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_NUMERIC_PAD,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_NUM_ALL_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_NUM_ALL_TOUCH,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_NUM_ALL_PAD,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE2:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD2,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE3:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD3,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE4:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD4,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE5:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD5,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE6:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD6,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_PASSWORD_MODE7:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_TOUCH,	// szActiveKeys
											strPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_EPP,			// nEnableMode
											nPinPassWordMin,	// nMinKeyCount
											nPinPassWordMax,	// nMaxKeyCount
											nPinPassWordAuto,	// bAutoEnd
											PIN_PASSWORD_PAD7,	// szActiveKeys
											szPinPassWordTerm,	// szTerminatorKeys
											szCardData,			// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_AMOUNT_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_AMOUNT_TOUCH,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_AMOUNT_PAD,		// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_INPUT_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_INPUT_TOUCH,	// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_INPUT_PAD,		// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;
				case PIN_ALL_MODE:
					if (AtmDefine.MachineKind == MCD3)
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_ALL_TOUCH,		// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					else
					{
						fnPIN_EntryEnable(PINMODE_NORMAL,		// nEnableMode
											0,					// nMinKeyCount
											0,					// nMaxKeyCount : 2004.11.05
											FALSE,				// bAutoEnd
											PIN_ALL_PAD,		// szActiveKeys
											"",					// szTerminatorKeys
											"",					// szCardData
											K_NO_WAIT);			// nTimeout
					}
					break;

				default:
					break;
			}
		}
		else 
		{
			fnPIN_EntryDisable();
		}
	}

	if (nWaitFlag)
		return (fnAPL_CheckDeviceAction(nTempDevId));
	else 
		return TRUE;
}

// Device Set Flicker
int CDevCmn::fnAPL_DeviceSetFlicker(int nDevId, int nAction)
{
	switch (AtmDefine.MachineKind)								// 2004.06.30
	{
		case NANOCASH:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			break;
		case MCD3:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case TTW:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case MCD4:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);			// 2004.11.05
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case TTW2:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case EATMGBA1:											// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case EATMULL1:											// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case EATMTTW:											// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case TTW2E:												// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case MCD4E:												// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case MX8000T:											// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case MX8000L:											// 2005.11.11
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case MX5600T:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
		case MX5600L:
			if (nDevId & DEV_SPR)
				fnLGT_SetFlicker(SPR_FLICKER, nAction);
			if (nDevId & DEV_MCU)
				fnLGT_SetFlicker(MCU_FLICKER, nAction);
			if (nDevId & DEV_CSH)
				fnLGT_SetFlicker(CDU_FLICKER, nAction);
			break;
	}

	return TRUE;
}

// Get Device Event
int CDevCmn::fnAPL_GetDeviceEvent(int nDevId, int nEventKind)
{
	int TranDev = DEV_NONE;
	int	ScanDev = DEV_NONE;

	nDevId = fnAPL_GetDefineDevice(nDevId);
	ScanDev = nDevId & (~(DEV_RIC | DEV_IRD));

	TranDev |= fnCMN_ScanDeviceAction(ScanDev, 0, nEventKind);
	if (TranDev)
		;
	else
	{
		if (nDevId & DEV_RIC)
		{
			if (fnRIC_GetDataStatus())
				TranDev |= DEV_RIC;
		}
		if (nDevId & DEV_IRD)
		{
			if (fnIRD_GetDataStatus())
				TranDev |= DEV_IRD;
		}
	}

	if (nDevId & TranDev)
	{
		DeviceTran = TranDev;
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_GetDeviceEvent(nDevid[%x]) DeviceTran[%x]", nDevId, DeviceTran);
		return TRUE;
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//
//      FILE    D:\EJR\TXTMMDD\DD.TXT(1 YEAR)
//              D:\EJR\IMGMMDD\DDEJRSEQNN0.JPG
//                             DDEJRSEQNN1.JPG(0-9:WITHDRAW,DEPOSIT)
//
//		RECORD	2048 BYTE(EjrTbl)
/////////////////////////////////////////////////////////////////////////////
// Elec Journal Spool Print Start
int CDevCmn::fnAPL_EjrSpoolPrintStart(int nDevId, EjrTbl* pEjrData)
{
	memset(&Ejr, ' ', sizeof(Ejr));
	memset(&EjrSpr, ' ', sizeof(EjrSpr));

	if (pEjrData)												// Key Area Setting
		memcpy(&Ejr, pEjrData, sizeof(Ejr));

	Ejr.Dummy[itemof(Ejr.Dummy) - 1] = '|';
	Ejr.Cr = A_CR;
	Ejr.Lf = A_LF;

	for (int i = 0; i < JPR_MAX_ROW; i++)						// Data Area Setting
	{
		Ejr.Data[i][JPR_MAX_COL]		= '|';
		Ejr.Data[i][JPR_MAX_COL + 1]	= A_CR;
		Ejr.Data[i][JPR_MAX_COL + 2]	= A_LF;
	}
	Ejr.DataDummy[itemof(Ejr.DataDummy) - 1] = '|';
	Ejr.DataDummyCr = A_CR;
	Ejr.DataDummyLf = A_LF;
	return TRUE;
}

// Elec Journal Spool Data
int CDevCmn::fnAPL_EjrSpoolData(int nDevId, int nLine, int nSize, LPCTSTR szSpoolData)
{
	if (nDevId & DEV_SPR)
	{
		if (nLine < 0)											// Line Adjust
			nLine = 0;
		if (nLine >= SPR_MAX_ROW)
			nLine = SPR_MAX_ROW - 1;
		
		if (nSize <= 0)											// Size Adjust
			nSize = strlen(szSpoolData);
		if (nSize > SPR_MAX_COL)
			nSize = SPR_MAX_COL;

		memcpy(EjrSpr.Data[nLine], GetString((LPSTR)szSpoolData, nSize, nSize).GetBuffer(0), nSize);
																// Data Adjust
	}

	if (nDevId & DEV_JPR)
	{
		if (nLine < 0)											// Line Adjust
			nLine = 0;
		if (nLine >= JPR_MAX_ROW)
			nLine = JPR_MAX_ROW - 1;
		
		if (nSize <= 0)											// Size Adjust
			nSize = strlen(szSpoolData);
		if (nSize > JPR_MAX_COL)
			nSize = JPR_MAX_COL;

		memcpy(Ejr.Data[nLine], GetString((LPSTR)szSpoolData, nSize, nSize).GetBuffer(0), nSize);
																// Data Adjust
	}
	return TRUE;
}

// Elec Journal Spool End & Save & Print
int CDevCmn::fnAPL_EjrSpoolEnd(int nDevId, int nWaitFlag)
{
	CString		strTempPrintData("");
	CString		strTempUnPackPrintData("");
	CString		strTempSprPrintData("");
	CString		strTempJprPrintData("");
	int			i = 0;
	int			nActDevId = DEV_NONE;							// 2005.02.28

//	if (nDevId & DEV_JPR)										// Jpr Exist
//	{
//		fnAPL_EjrAddSerialNo();
//		fnAPL_EjrSave();
//	}

	for (i = 0; i < SPR_MAX_ROW; i++)							// Data Make
	{
		if (nDevId & DEV_SPR)									// Spr Data Make
		{
			strTempPrintData = GetString(EjrSpr.Data[i], SPR_MAX_COL, SPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempSprPrintData.GetLength())
					strTempSprPrintData = strTempUnPackPrintData;
				else
					strTempSprPrintData += "," + strTempUnPackPrintData;
			}
			else
			{
				if (!strTempSprPrintData.GetLength())
					strTempSprPrintData = "20";
				else
					strTempSprPrintData += ",20";
			}
		}
	}

	for (i = 0; i < JPR_MAX_ROW; i++)							// Data Make
	{
		if (nDevId & DEV_JPR)									// Jpr Data Make
		{
			strTempPrintData = GetString(Ejr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempJprPrintData.GetLength())
					strTempJprPrintData = strTempUnPackPrintData;
				else
					strTempJprPrintData += "," + strTempUnPackPrintData;
			}
		}
	}

	if (nDevId & DEV_SPR)										// Spr Print
	{
		if (!strTempSprPrintData.GetLength())					// Spr No Data 
			strTempSprPrintData = "20";

		fnSPR_PrintFile(strTempSprPrintData);
		if ((!fnAPL_GetAvailDevice(DEV_SPR))	||				// 2005.04.19
			(fnAPL_GetDownErrorDevice(DEV_SPR))	||
			(SlipHeadStatus != HEADER_NORMAL)	||
			(SlipStatus == SLIP_EMPTY_PAPER))
			;
		else
		{
			nActDevId |= DEV_SPR;								// 2005.02.28
			fnAPL_CheckDeviceActionAutoOff(DEV_SPR);			// 2007.09.06
			fnSPR_Print(strTempSprPrintData);
		}
	}

	if (nDevId & DEV_JPR)										// Jpr Print
	{
		if (!strTempJprPrintData.GetLength())					// Jpr No Data
			strTempJprPrintData = "20";
//		if (!IsSpace(Ejr.TranSerialNo, 8))						// 2004.06.30
//		{
//								   //01234567890123456789012345678901234567890123456789012345678901234567890123456789
//			strTempJprPrintData += ",2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A";
//		}
																// '*' Print
		fnJPR_PrintFile(strTempJprPrintData);
		if ((!fnAPL_GetAvailDevice(DEV_JPR))	||				// 2005.04.19
			(fnAPL_GetDownErrorDevice(DEV_JPR))	||
			(JnlHeadStatus != HEADER_NORMAL)	||
			(JnlStatus == JNL_EMPTY_PAPER))
		{
			fnAPL_EjrSaveHistory();								// 2006.05.20
			fnJPR_PrintLog(strTempJprPrintData);
		}
		else
		{
			nActDevId |= DEV_JPR;								// 2005.02.28
			fnAPL_CheckDeviceActionAutoOff(DEV_JPR);			// 2007.09.06

			if ((!fnAPL_GetAvailDevice(DEV_JPR))	||			// 2005.04.19
				(fnAPL_GetDownErrorDevice(DEV_JPR))	||
				(JnlHeadStatus != HEADER_NORMAL)	||
				(JnlStatus == JNL_EMPTY_PAPER))
			{
				fnAPL_EjrSaveHistory();							// 2006.05.20
				fnJPR_PrintLog(strTempJprPrintData);
			}
			else
			{
				fnAPL_EjrSaveHistoryTemp();						// 2006.05.20
				fnJPR_Print(strTempJprPrintData);
			}
		}
	}

	if (nWaitFlag)												// 2007.09.06
		return (fnAPL_CheckDeviceActionAutoOff(nDevId & nActDevId));
	else 
		return TRUE;
}

// Elec Journal Spool End & Save & Print 2					// djp 20090213
int CDevCmn::fnAPL_EjrSpoolEnd2(int nDevId, char cPrintChar, int nWaitFlag)
{
	CString		strTempPrintData("");
	CString		strTempUnPackPrintData("");
	CString		strTempSprPrintData("");
	CString		strTempJprPrintData("");
	int			i = 0;
	int			nActDevId = DEV_NONE;							// 2005.02.28

	if (nDevId & DEV_JPR)										// Jpr Exist
	{
		fnAPL_EjrAddSerialNo();
		fnAPL_EjrSave();
	}

	for (i = 0; i < SPR_MAX_ROW; i++)							// Data Make
	{
		if (nDevId & DEV_SPR)									// Spr Data Make
		{
			strTempPrintData = GetString(EjrSpr.Data[i], SPR_MAX_COL, SPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempSprPrintData.GetLength())
					strTempSprPrintData = strTempUnPackPrintData;
				else
					strTempSprPrintData += "," + strTempUnPackPrintData;
			}
			else
			{
				if (!strTempSprPrintData.GetLength())
					strTempSprPrintData = "20";
				else
					strTempSprPrintData += ",20";
			}
		}
	}

	for (i = 0; i < JPR_MAX_ROW; i++)							// Data Make
	{
		if (nDevId & DEV_JPR)									// Jpr Data Make
		{
			strTempPrintData = GetString(Ejr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempJprPrintData.GetLength())
					strTempJprPrintData = strTempUnPackPrintData;
				else
					strTempJprPrintData += "," + strTempUnPackPrintData;
			}
		}
	}

	if (nDevId & DEV_SPR)										// Spr Print
	{
		if (!strTempSprPrintData.GetLength())					// Spr No Data 
			strTempSprPrintData = "20";
		if ((!fnAPL_GetAvailDevice(DEV_SPR))	||				// 2005.04.19
			(fnAPL_GetDownErrorDevice(DEV_SPR))	||
			(SlipHeadStatus != HEADER_NORMAL)	||
			(SlipStatus == SLIP_EMPTY_PAPER))
			;
		else
		{
			nActDevId |= DEV_SPR;								// 2005.02.28
			fnAPL_CheckDeviceActionAutoOff(DEV_SPR);			// 2007.09.06
			fnSPR_Print(strTempSprPrintData);
		}
	}

	if (nDevId & DEV_JPR)										// Jpr Print
	{
		if (!strTempJprPrintData.GetLength())					// Jpr No Data
			strTempJprPrintData = "20";
		if (!IsSpace(Ejr.TranSerialNo, 8))						// 2004.06.30
		{
			char achPrintChar[64];
			if (cPrintChar != ' ')								// if cPrintChar is BankSpace, so not print splite signe.  djp 20090223
			{
				memset(achPrintChar, cPrintChar, sizeof(achPrintChar));
				char PrintChar[33];
				memset(PrintChar, 0, sizeof(PrintChar));
				memcpy(&PrintChar[0], ",", 1);
				memcpy(&PrintChar[1], achPrintChar, 32);
				//01234567890123456789012345678901234567890123456789012345678901234567890123456789
				//strTempJprPrintData += ",2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A2A";		// djp, 20090213  2A = '*'
				strTempJprPrintData += PrintChar;
			}
		}
																// '*' Print
		if ((!fnAPL_GetAvailDevice(DEV_JPR))	||				// 2005.04.19
			(fnAPL_GetDownErrorDevice(DEV_JPR))	||
			(JnlHeadStatus != HEADER_NORMAL)	||
			(JnlStatus == JNL_EMPTY_PAPER))
		{
			fnAPL_EjrSaveHistory();								// 2006.05.20
			fnJPR_PrintLog(strTempJprPrintData);
		}
		else
		{
			nActDevId |= DEV_JPR;								// 2005.02.28
			fnAPL_CheckDeviceActionAutoOff(DEV_JPR);			// 2007.09.06

			if ((!fnAPL_GetAvailDevice(DEV_JPR))	||			// 2005.04.19
				(fnAPL_GetDownErrorDevice(DEV_JPR))	||
				(JnlHeadStatus != HEADER_NORMAL)	||
				(JnlStatus == JNL_EMPTY_PAPER))
			{
				fnAPL_EjrSaveHistory();							// 2006.05.20
				fnJPR_PrintLog(strTempJprPrintData);
			}
			else
			{
				fnAPL_EjrSaveHistoryTemp();						// 2006.05.20
				fnJPR_Print(strTempJprPrintData);
			}
		}
	}

	if (nWaitFlag)												// 2007.09.06
		return (fnAPL_CheckDeviceActionAutoOff(nDevId & nActDevId));
	else 
		return TRUE;
}

// Elec Journal Direct Print
int CDevCmn::fnAPL_EjrDirectPrint(int nDevId, int nWaitFlag)
{
	CString		strTempPrintData("");
	CString		strTempUnPackPrintData("");
	CString		strTempJprPrintData("");
	int			i = 0;
	int			nActDevId = DEV_NONE;

	if (nDevId & DEV_JPR)										// Jpr Data Make
	{
		for (i = 0; i < JPR_MAX_ROW; i++)
		{
			strTempPrintData = GetString(Ejr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
			strTempPrintData.TrimRight();
			if (strTempPrintData.GetLength())
			{
				strTempUnPackPrintData = MakeUnPack(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength());
				if (!strTempJprPrintData.GetLength())
					strTempJprPrintData = strTempUnPackPrintData;
				else
					strTempJprPrintData += "," + strTempUnPackPrintData;
			}
		}
	}

	if (nDevId & DEV_JPR)										// Jpr Print
	{
		if (!strTempJprPrintData.GetLength())					// Jpr No Data
			strTempJprPrintData = "20";
		fnJPR_PrintFile(strTempJprPrintData);
		if ((!fnAPL_GetAvailDevice(DEV_JPR))	||				// 2005.04.19
			(fnAPL_GetDownErrorDevice(DEV_JPR))	||
			(JnlHeadStatus != HEADER_NORMAL)	||
			(JnlStatus == JNL_EMPTY_PAPER))
			fnJPR_PrintLog(strTempJprPrintData);
		else
		{
			nActDevId |= DEV_JPR;								// 2005.02.28
			fnAPL_CheckDeviceActionAutoOff(DEV_JPR);			// 2007.09.06
			fnJPR_Print(strTempJprPrintData);
		}
	}

	if (nWaitFlag)												// 2007.09.06
		return (fnAPL_CheckDeviceActionAutoOff(nDevId & nActDevId));
	else 
		return TRUE;
}

// Elec Journal Add Serial No
int CDevCmn::fnAPL_EjrAddSerialNo()
{
	char	szTempEjrSerialNo[256];										

	memcpy(szTempEjrSerialNo, m_pProfile->TRANS.EjrSerialNo, 8);

	if (memcmp(szTempEjrSerialNo, "99999999", 8) == 0)
		memset(szTempEjrSerialNo, '0', 8);
	AddString(szTempEjrSerialNo, "00000001", 8);

	m_pProfile->GetTransProfile();
	memcpy(m_pProfile->TRANS.EjrSerialNo, szTempEjrSerialNo, 8);
	m_pProfile->TRANS.EjrSerialNo[8] = NULL;
	m_pProfile->PutTransProfile();
	return TRUE;
}

// Elec Journal Save
int CDevCmn::fnAPL_EjrSave()
{
	char		szTempFileName[256];
	CFileStatus	TempFileStatus;
	FILE*		TempFp;
	EjrTbl		TempEjr;
	int			nTempFReadNum = 0;
																// Key Area Setting
	memcpy(Ejr.SerialNo, m_pProfile->TRANS.EjrSerialNo, sizeof(Ejr.SerialNo));
	memcpy(Ejr.Date, m_pProfile->TRANS.YYYYMMDD, sizeof(Ejr.Date));
	memcpy(Ejr.Time, GetTime().GetBuffer(0), sizeof(Ejr.Time));

	_mkdir(_EJR_DIR);											// Dir Create
	_mkdir(GetSprintf("%s%4.4s", _EJR_TXT, &m_pProfile->TRANS.YYYYMMDD[4]));

	sprintf(szTempFileName, "%s%4.4s\\%4.4s.TXT", 
							_EJR_TXT, 
							&m_pProfile->TRANS.YYYYMMDD[4], 
							&m_pProfile->TRANS.YYYYMMDD[4]);
	if (CFile::GetStatus(szTempFileName, TempFileStatus))		// Date Confirm
	{
		TempFp = fopen(szTempFileName, "rb");
		if (TempFp)
		{
			nTempFReadNum = fread(&TempEjr , sizeof(TempEjr), 1, TempFp);
			fclose(TempFp);
			if (nTempFReadNum)
			{													// Delete(1 Year Ago Data)
				if (memcmp(TempEjr.Date, m_pProfile->TRANS.YYYYMMDD, sizeof(TempEjr.Date)) != 0)
				{
					DeleteFile(szTempFileName);
					DeleteFiles(GetSprintf("%s%4.4s", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4]), -1);
				}
			}
			else 
			{
				DeleteFile(szTempFileName);						// No Data
				DeleteFiles(GetSprintf("%s%4.4s", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4]), -1);
			}
		}
	}

	TempFp = fopen(szTempFileName, "abc");						// Journaling
	if (TempFp)
	{
		fwrite(&Ejr, sizeof(Ejr), 1, TempFp);
		fflush(TempFp);
		fclose(TempFp);
	}

//	if (!IsSpace(Ejr.TranSerialNo, 8))							// ALL SEND : 2004.08.11
	{
		_mkdir(_EJR_SEND);
		sprintf(szTempFileName, "%s\\%4.4s.TXT",				// 2005.04.19
								_EJR_SEND, 
								&m_pProfile->TRANS.EjrSerialNo[4]);
		TempFp = fopen(szTempFileName, "wbc");					// Journaling Send
		if (TempFp)
		{
			fwrite(&Ejr, sizeof(Ejr), 1, TempFp);
			fflush(TempFp);
			fclose(TempFp);
		}

		RegSetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_EJRWRITEP,		// 2005.04.19
					GetSprintf("%4.4s", &m_pProfile->TRANS.EjrSerialNo[4]));
	}
	return TRUE;
}

// Elec Journal Save History
int CDevCmn::fnAPL_EjrSaveHistory()
{
	char		szBuff[256];
	char		szTempFileName[256];
	char		szHistoryFileName[256];
	CFileStatus	TempFileStatus;
	FILE*		TempFp;
	FILE*		HistoryFp;
	CString		strTempPrintData("");

	if (fnAPL_GetAutoOffBankDevice(DEV_JPR))
		;
	else
		return TRUE;

	_mkdir(_EJR_JNL);											// Dir Create

	sprintf(szTempFileName, "%s\\%s", _EJR_JNL, _EJR_JNL_TEMP);
	sprintf(szHistoryFileName, "%s\\%s", _EJR_JNL, _EJR_JNL_HISTORY);

	if (CFile::GetStatus(szTempFileName, TempFileStatus))		// Temp File Check
	{
		TempFp = fopen(szTempFileName, "rb");
		HistoryFp = fopen(szHistoryFileName, "ac");
		if (TempFp && HistoryFp)
		{
			while (TRUE)
			{
				memset(szBuff, 0, sizeof(szBuff));
				if (fgets(szBuff, 250, TempFp))
					fprintf(HistoryFp, "%-40.40s\n", szBuff);
				else
					break;
			}
		}

		if (TempFp)
			fclose(TempFp);
		if (HistoryFp)
		{
			fflush(HistoryFp);
			fclose(HistoryFp);
		}

		DeleteFile(szTempFileName);
	}
	
	HistoryFp = fopen(szHistoryFileName, "ac");
	if (HistoryFp)												// Write History
	{
		for (int i = 0; i < JPR_MAX_ROW; i++)
		{
			strTempPrintData = GetString(Ejr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
			if (IsSpace(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength()))
				;
			else
				fprintf(HistoryFp, "%-40.40s\n", strTempPrintData);
		}

		if (!IsSpace(Ejr.TranSerialNo, 8))
		{									//0123456789012345678901234567890123456789
			fprintf(HistoryFp, "%-40.40s\n", "********************************");
		}

		fflush(HistoryFp);
		fclose(HistoryFp);
	}

	return TRUE;
}

// Elec Journal Save Temp
int CDevCmn::fnAPL_EjrSaveHistoryTemp()
{
	char		szTempFileName[256];
	CFileStatus	TempFileStatus;
	FILE*		TempFp;
	CString		strTempPrintData("");

	if (fnAPL_GetAutoOffBankDevice(DEV_JPR))
		;
	else
		return TRUE;

	_mkdir(_EJR_JNL);											// Dir Create

	sprintf(szTempFileName, "%s\\%s", _EJR_JNL, _EJR_JNL_TEMP);

	TempFp = fopen(szTempFileName, "wc");
	if (TempFp)													// Write Temp File
	{
		for (int i = 0; i < JPR_MAX_ROW; i++)
		{
			strTempPrintData = GetString(Ejr.Data[i], JPR_MAX_COL, JPR_MAX_COL);
			if (IsSpace(strTempPrintData.GetBuffer(0), strTempPrintData.GetLength()))
				;
			else
				fprintf(TempFp, "%-40.40s\n", strTempPrintData);
		}

		if (!IsSpace(Ejr.TranSerialNo, 8))
		{
			fprintf(TempFp, "%-40.40s\n", "********************************");
		}

		fflush(TempFp);
		fclose(TempFp);
	}

	return TRUE;
}

// Elec Journal Check File Size
int CDevCmn::fnAPL_EjrCheckFileSize()
{
	char		szTempFileName[256];
	CFileStatus	TempFileStatus;
	FILE*		TempFp;
	LONG		nFileSize = 0;
	CString		strFilePath("");

	sprintf(szTempFileName, "%s\\%s", _EJR_JNL, _EJR_JNL_HISTORY);
	if (CFile::GetStatus(szTempFileName, TempFileStatus))		// File Check
	{
		TempFp = fopen(szTempFileName, "rb");
		if (TempFp)
		{
			fseek(TempFp, 0, SEEK_END);
			nFileSize = ftell(TempFp);
			fclose(TempFp);
			
			if (nFileSize > (1024 * 1024 * 4))					// File Size Check(4Mega) And Delete
			{
				_mkdir(_EJR_JNL_BACKUP);						// Dir Create
				strFilePath.Format("%s\\%s.TXT", _EJR_JNL_BACKUP, GetDate());
				CopyFile(szTempFileName, strFilePath, TRUE);
				DeleteFile(szTempFileName);
				RegSetInt(_REGKEY_DEVICE, _REGKEY_DEVICE_JNLPOSITION, 0);
			}
		}
	}

	return TRUE;
}

// Capture File Clear
int CDevCmn::fnAPL_CaptureFileClear(int nAddSerialFlag, int nSerialLength)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureFileClear(nAddSerialFlag[%d], nSerialLength[%d])", nAddSerialFlag, nSerialLength);

	char	szTempSerialNo[256];

	memset(szTempSerialNo, 0, sizeof(szTempSerialNo));
	memcpy(szTempSerialNo, m_pProfile->TRANS.SerialNo + 2, 6);
	if (nAddSerialFlag)
	{
		if (nSerialLength == 4)									// 2005.11.11
		{
			if (memcmp(&szTempSerialNo[2], "9999", 4) == 0)
				memset(szTempSerialNo, '0', 6);
			AddString(szTempSerialNo, "000001", 6);
		}
		else
		if (nSerialLength == 5)									// 2005.11.11
		{
			if (memcmp(&szTempSerialNo[1], "99999", 5) == 0)
				memset(szTempSerialNo, '0', 6);
			AddString(szTempSerialNo, "000001", 6);
		}
		else
		{
			if (memcmp(szTempSerialNo, "999999", 6) == 0)
				memset(szTempSerialNo, '0', 6);
			AddString(szTempSerialNo, "000001", 6);
		}
	}

	CaptureSerialNo.Format("%6.6s", szTempSerialNo);			// 2005.11.11

	_mkdir(_EJR_DIR);											// Dir Create
	_mkdir(GetSprintf("%s%4.4s", _EJR_IMG, &m_pProfile->TRANS.YYYYMMDD[4]));

	for (int i = 0; i < 10; i++)								// Delete
	{
		DeleteFile(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d.JPG", 
							_EJR_IMG, 
							&m_pProfile->TRANS.YYYYMMDD[4], 
							&m_pProfile->TRANS.YYYYMMDD[4], 
							CaptureSerialNo.GetBuffer(0),
							i));
	}
	return TRUE;
}

// Capture Face Save
int CDevCmn::fnAPL_CaptureFaceSave(int nIndex, int nFirstFlag, int nAddSerialFlag, int nSerialLength)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureFaceSave(nIndex[%d], nFirstFlag[%d], nAddSerialFlag[%d], nSerialLength[%d])", nIndex, nFirstFlag, nAddSerialFlag, nSerialLength);

	if (nFirstFlag)												// Delete : 2005.11.11
		fnAPL_CaptureFileClear(nAddSerialFlag, nSerialLength);
	
	switch (AtmDefine.MachineKind)								// 2004.06.24
	{
		case NANOCASH:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d.JPG", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MCD3:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case TTW:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MCD4:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case TTW2:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case EATMGBA1:											// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case EATMULL1:											// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case EATMTTW:											// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case TTW2E:												// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MCD4E:												// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX8000T:											// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX8000L:											// 2005.11.11
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX5600T:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX5600L:
			fnCMR_CaptureFace(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
	}
	return TRUE;
}

// Capture Hand Save
int CDevCmn::fnAPL_CaptureHandSave(int nIndex, int nFirstFlag, int nAddSerialFlag, int nSerialLength)
{
MsgDump(TRACE_DUMP_MODE, "Log", __FILE__, __LINE__, "fnAPL_CaptureHandSave(nIndex[%d], nFirstFlag[%d], nAddSerialFlag[%d], nSerialLength[%d])", nIndex, nFirstFlag, nAddSerialFlag, nSerialLength);

	if (nFirstFlag)												// Delete : 2005.11.11
		fnAPL_CaptureFileClear(nAddSerialFlag, nSerialLength);
	
	switch (AtmDefine.MachineKind)								// 2004.06.24
	{
		case NANOCASH:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d.JPG", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MCD3:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case TTW:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MCD4:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case TTW2:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case EATMGBA1:											// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case EATMULL1:											// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case EATMTTW:											// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case TTW2E:												// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MCD4E:												// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX8000T:											// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX8000L:											// 2005.11.11
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX5600T:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
		case MX5600L:
			fnCMR_CaptureHand(GetSprintf("%s%4.4s\\%4.4s%6.6s%1.1d", 
									_EJR_IMG, 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									&m_pProfile->TRANS.YYYYMMDD[4], 
									CaptureSerialNo.GetBuffer(0),
									nIndex));
			break;
	}
	return TRUE;
}

// Clerk Initialize Mode Set
int	CDevCmn::fnAPL_ClerkInitModeSet()
{
	CString			strOpInformation("");
	CStringArray	strOpInformationArray;
	CString			strTemp("");
	int				i = 0;

/////////////////////////////////////////////////////////////////////////////
	strOpInformation = GetShareData("OP_STATUS");				// Status Information Setting
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:OP_STATUS[%s]", strOpInformation);

	SplitString(strOpInformation, "||", strOpInformationArray);
	if (strOpInformationArray.GetSize() >= 20)					// 2005.11.11
		;
	else
	{					  //0123456789012345678901234567890123456789
		strOpInformation = "||||||||||||||||||||||||||||||||||||||";// 2005.11.11
		SplitString(strOpInformation, "||", strOpInformationArray);
	}

	strOpInformationArray[0] = "INIT";							// INIT Setting
	strOpInformationArray[4] = "ON";							// ON Setting
	strOpInformationArray[7].Format("%3.3s", &TranDefine.ApVersion[10]);
																// SCR BANK NAME Setting
	strOpInformationArray[8] = fstrAPL_ClerkInformationMake(8);	// MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L) Setting
	for (i = 0; i < strOpInformationArray.GetSize(); i++)
	{
		if (i == 0)
			strTemp = strOpInformationArray[i];
		else
			strTemp += "||" + strOpInformationArray[i];
	}
	SetShareData("OP_STATUS", strTemp);
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:OP_STATUS[%s]", strTemp);
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	SetShareData("OP_RESULT", OP_NORMAL);						// Default Normal Setting
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
	switch (AtmDefine.OpDevice)									// OP_MODE Setting : 2005.11.11
	{
		case NOT_SET:											// 2005.12.16 : FIRST AP SCREEN
			SetShareData("OP_MODE", "AP");
			break;
		case OP_TOUCH:
			SetShareData("OP_MODE", "");						// 2006.01.09
			break;
		case OP_TSPL:
		case OP_TSPLCHN:
			SetShareData("OP_MODE", "AP");
			break;
		case OP_HSPL:
			SetShareData("OP_MODE", "AP");
			break;
	}
	SetShareData("OP_OPSTATUS", "AP");							// 2006.01.10

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:OP_APSTATUS[%s]", GetShareData("OP_APSTATUS"));
	SetShareData("OP_APSTATUS", "APINIT");						// OP_APSTATUS Clear
	SetShareData("OP_APSTATUS", "APINIT");						// 2005.03.30
	SetShareData("OP_APSTATUS", "APINIT");						// 2005.03.30
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInitModeSet:OP_APSTATUS[%s]", GetShareData("OP_APSTATUS"));
/////////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// Clerk Information
int CDevCmn::fnAPL_ClerkInformation(int nInitFlag, int nOnlyHeaderUpdateFlag)
{
	static int		nFirstFlag = TRUE;
	static int		nDoingFlag = FALSE;
	int				i = 0;

/////////////////////////////////////////////////////////////////////////////
	if (nDoingFlag)												// 2004.08.05
		return TRUE;
	nDoingFlag = TRUE;											// 2004.08.05
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
if (!nOnlyHeaderUpdateFlag)
{
	// Status Information
	Op.Status = "";												// OP_STATUS
	for (i = 0; i <= 18; i++)									// 2006.06.02
		Op.Status += fstrAPL_ClerkInformationMake(i) + "||";
	Op.Status += fstrAPL_ClerkInformationMake(19);				// 2006.06.02
}

	// Header Information			
	Op.Header = "";												// OP_HEADER
	switch (AtmDefine.CashHandler)								// 2008.04.10
	{
		case BRM_SET:
			for (i = 100; i <= 125; i++)
				Op.Header += fstrAPL_ClerkInformationMake(i) + "||";
			for (i = 1600; i <= 1607; i++)
				Op.Header += fstrAPL_ClerkInformationMake(i) + "||";
			Op.Header += fstrAPL_ClerkInformationMake(1608);
			break;
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			for (i = 100; i <= 124; i++)
				Op.Header += fstrAPL_ClerkInformationMake(i) + "||";
			Op.Header += fstrAPL_ClerkInformationMake(125);
			break;
	}

if (!nOnlyHeaderUpdateFlag)
{
	// ApUpdate Information			
	Op.ApUpdate = "";											// OP_APUPDATE
	for (i = 200; i <= 200; i++)
		Op.ApUpdate += fstrAPL_ClerkInformationMake(i) + "||";
	Op.ApUpdate += fstrAPL_ClerkInformationMake(201);
	
	// TraceCopy Information			
	Op.TraceCopy = "";											// OP_TRACECOPY
	for (i = 300; i <= 301; i++)
		Op.TraceCopy += fstrAPL_ClerkInformationMake(i) + "||";
	Op.TraceCopy += fstrAPL_ClerkInformationMake(302);
	
	// Version Information			
	Op.Version = "";											// OP_VERSION
	for (i = 400; i <= 420; i++)
		Op.Version += fstrAPL_ClerkInformationMake(i) + "||";
	Op.Version += fstrAPL_ClerkInformationMake(421);
	
	// WorkParam Information			
	Op.WorkParam = "";											// OP_WORKPARAM
	for (i = 500; i <= 513; i++)
		Op.WorkParam += fstrAPL_ClerkInformationMake(i) + "||";
	Op.WorkParam += fstrAPL_ClerkInformationMake(514);
	
	// AddCash Information			
	Op.AddCash = "";											// OP_ADDCASH
	for (i = 600; i <= 618; i++)
		Op.AddCash += fstrAPL_ClerkInformationMake(i) + "||";
	Op.AddCash += fstrAPL_ClerkInformationMake(619);
	
	// EjmView Information			
	Op.EjmView = "";											// OP_EJMVIEW
	for (i = 700; i <= 701; i++)
		Op.EjmView += fstrAPL_ClerkInformationMake(i) + "||";
	Op.EjmView += fstrAPL_ClerkInformationMake(702);
	
	// EjmResult Information			
	Op.EjmResult = "";											// OP_EJMRESULT
	for (i = 800; i <= 814; i++)
		Op.EjmResult += fstrAPL_ClerkInformationMake(i) + "||";
	Op.EjmResult += fstrAPL_ClerkInformationMake(815);
	
	// EjmCopy Information			
	Op.EjmCopy = "";											// OP_EJMCOPY
	for (i = 900; i <= 902; i++)								// 2005.02.24
		Op.EjmCopy += fstrAPL_ClerkInformationMake(i) + "||";
	Op.EjmCopy += fstrAPL_ClerkInformationMake(903);

	// EjmSend Information			
	Op.EjmSend = "";											// OP_EJMSEND
	for (i = 1000; i <= 1002; i++)
		Op.EjmSend += fstrAPL_ClerkInformationMake(i) + "||";
	Op.EjmSend += fstrAPL_ClerkInformationMake(1003);

	// EjmPrint Information			
	Op.EjmPrint = "";											// OP_EJMPRINT
	for (i = 1100; i <= 1102; i++)
		Op.EjmPrint += fstrAPL_ClerkInformationMake(i) + "||";
	Op.EjmPrint += fstrAPL_ClerkInformationMake(1103);

	// CashValue Information			
	Op.CashValue = "";											// OP_CASHVALUE
	for (i = 1200; i <= 1202; i++)								// 2005.02.24
		Op.CashValue += fstrAPL_ClerkInformationMake(i) + "||";
	Op.CashValue += fstrAPL_ClerkInformationMake(1203);

	// DbInitail Information			
	Op.DbInitial = "";											// OP_DBINITIAL
	for (i = 1300; i <= 1300; i++)
		Op.DbInitial += fstrAPL_ClerkInformationMake(i) + "||";
	Op.DbInitial += fstrAPL_ClerkInformationMake(1301);

	// Total Information
	Op.Total = "";												// OP_TOTAL
	for (i = 1400; i <= 1400; i++)								// 2005.08.31
		Op.Total += fstrAPL_ClerkInformationMake(i) + "||";
	Op.Total += fstrAPL_ClerkInformationMake(1401);

	// SetParam Information
	Op.SetParam = "";											// OP_SETPARAM
	for (i = 1500; i <= 1528; i++)								// 2005.10.25
		Op.SetParam += fstrAPL_ClerkInformationMake(i) + "||";	// liyi add 2008.09.23
	Op.SetParam += fstrAPL_ClerkInformationMake(1529);

	// View Atmc Total information
	Op.ViewAtmcTotal= "";
	for (i = 1600; i <= 1630; i++)								
		Op.ViewAtmcTotal += fstrAPL_ClerkInformationMake(i) + "||";	// yaokq add 20081031
	Op.ViewAtmcTotal += fstrAPL_ClerkInformationMake(1631);
}

//////////////////////////////////////////////////////////////////////////
	if ((nInitFlag)	||											// Status Information
		(Op.Status			!= OpSave.Status))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_STATUS[%s]", Op.Status);
		OpSave.Status		=	Op.Status;
		SetShareData("OP_STATUS", Op.Status);
	}

	if ((nInitFlag) ||											// Header Information
		(Op.Header			!= OpSave.Header))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_HEADER[%s]", Op.Header);
		OpSave.Header		=	Op.Header;
		SetShareData("OP_HEADER", Op.Header);
	}

	if ((nInitFlag) ||											// ApUpdate Information
		(Op.ApUpdate		!= OpSave.ApUpdate))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_APUPDATE[%s]", Op.ApUpdate);
		OpSave.ApUpdate		=	Op.ApUpdate;
		SetShareData("OP_APUPDATE", Op.ApUpdate);
	}

	if ((nInitFlag) ||											// TraceCopy Information
		(Op.TraceCopy		!= OpSave.TraceCopy))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_TRACECOPY[%s]", Op.TraceCopy);
		OpSave.TraceCopy	=	Op.TraceCopy;
		SetShareData("OP_TRACECOPY", Op.TraceCopy);
	}

	if ((nInitFlag) ||											// Version Information
		(Op.Version			!= OpSave.Version))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_VERSION[%s]", Op.Version);
		OpSave.Version		=	Op.Version;
		SetShareData("OP_VERSION", Op.Version);
	}

	if ((nInitFlag) ||											// WorkParam Information
		(Op.WorkParam		!= OpSave.WorkParam))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_WORKPARAM[%s]", Op.WorkParam);
		OpSave.WorkParam		=	Op.WorkParam;
		SetShareData("OP_WORKPARAM", Op.WorkParam);
	}

	if ((nInitFlag) ||											// AddCash Information
		(Op.AddCash			!= OpSave.AddCash))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_ADDCASH[%s]", Op.AddCash);
		OpSave.AddCash		=	Op.AddCash;
		SetShareData("OP_ADDCASH", Op.AddCash);
	}

	if ((nInitFlag) ||											// EjmView Information
		(Op.EjmView			!= OpSave.EjmView))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_EJMVIEW[%s]", Op.EjmView);
		OpSave.EjmView		=	Op.EjmView;
		SetShareData("OP_EJMVIEW", Op.EjmView);
	}

	if ((nInitFlag) ||											// EjmResult Information
		(Op.EjmResult		!= OpSave.EjmResult))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_EJMRESULT[%s]", Op.EjmResult);
		OpSave.EjmResult		=	Op.EjmResult;
		SetShareData("OP_EJMRESULT", Op.EjmResult);
	}

	if ((nInitFlag) ||											// EjmCopy Information
		(Op.EjmCopy			!= OpSave.EjmCopy))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_EJMCOPY[%s]", Op.EjmCopy);
		OpSave.EjmCopy	=	Op.EjmCopy;
		SetShareData("OP_EJMCOPY", Op.EjmCopy);
	}

	if ((nInitFlag) ||											// EjmSend Information
		(Op.EjmSend			!= OpSave.EjmSend))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_EJMSEND[%s]", Op.EjmSend);
		OpSave.EjmSend	=	Op.EjmSend;
		SetShareData("OP_EJMSEND", Op.EjmSend);
	}

	if ((nInitFlag) ||											// EjmPrint Information
		(Op.EjmPrint			!= OpSave.EjmPrint))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_EJMPRINT[%s]", Op.EjmPrint);
		OpSave.EjmPrint	=	Op.EjmPrint;
		SetShareData("OP_EJMPRINT", Op.EjmPrint);
	}

	if ((nInitFlag) ||											// CashValue Information
		(Op.CashValue		!= OpSave.CashValue))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_CASHVALUE[%s]", Op.CashValue);
		OpSave.CashValue	=	Op.CashValue;
		SetShareData("OP_CASHVALUE", Op.CashValue);
	}

	if ((nInitFlag) ||											// DbInitial Information
		(Op.DbInitial			!= OpSave.DbInitial))		
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_DBINITIAL[%s]", Op.DbInitial);
		OpSave.DbInitial	=	Op.DbInitial;
		SetShareData("OP_DBINITIAL", Op.DbInitial);
	}

	if ((nInitFlag) ||											// Total Information
		(Op.Total				!= OpSave.Total))				// 2005.08.31
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_TOTAL[%s]", Op.Total);
		OpSave.Total		=	Op.Total;
		SetShareData("OP_TOTAL", Op.Total);
	}

	if ((nInitFlag) ||											// Set Param Information
		(Op.SetParam				!= OpSave.SetParam))		// 2005.10.25
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_SETPARAM[%s]", Op.SetParam);
		OpSave.SetParam		=	Op.SetParam;
		SetShareData("OP_SETPARAM", Op.SetParam);
	}

	if ((nInitFlag) ||											// Set Atmc Total Information
		(Op.ViewAtmcTotal				!= OpSave.ViewAtmcTotal))		// yaokq
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_View_Total[%s]", Op.ViewAtmcTotal);
		OpSave.ViewAtmcTotal		=	Op.ViewAtmcTotal;
		SetShareData("OP_VIEW_ATMCTOTALS", Op.ViewAtmcTotal);
	}

/////////////////////////////////////////////////////////////////////////////
	if (nFirstFlag)												// 2004.06.26
	{
		nFirstFlag = FALSE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_APSTATUS[%s]", GetShareData("OP_APSTATUS"));
		SetShareData("OP_APSTATUS", "APSTART");					// OP_APSTATUS Setting
		SetShareData("OP_APSTATUS", "APSTART");					// 2005.03.30
		SetShareData("OP_APSTATUS", "APSTART");					// 2005.03.30
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_ClerkInformation:OP_APSTATUS[%s]", GetShareData("OP_APSTATUS"));
	}
/////////////////////////////////////////////////////////////////////////////
	
/////////////////////////////////////////////////////////////////////////////
	nDoingFlag = FALSE;											// 2004.08.05
	return TRUE;
/////////////////////////////////////////////////////////////////////////////
}

// Clerk Information Make
CString	CDevCmn::fstrAPL_ClerkInformationMake(int nIndex)
{
	CString	strTempClerkInformation("");
	CString	strTemp("");

	int	TempRejectCount1 = 0;       //yaokq add 20081031
	int	TempRejectCount2 = 0; 
	int	TempRejectCount3 = 0; 
	int	TempRejectCount4 = 0; 

	switch (nIndex)
	{
/////////////////////////////////////////////////////////////////////////////
		case 0:
			// 0:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
			if (AtmStatus == ATM_INIT)
				strTempClerkInformation = "INIT";
			else
			if (AtmStatus == ATM_ERROR)
				strTempClerkInformation = "ERROR";
			else
			if (AtmStatus == ATM_CLERK)
				strTempClerkInformation = "CLERK";
			else
			if (AtmStatus == ATM_READY)
				strTempClerkInformation = "READY";
			else
			if (AtmStatus == ATM_CUSTOM)
				strTempClerkInformation = "CUSTOM";
			else
			if (AtmStatus == ATM_TRAN)
				strTempClerkInformation = "TRAN";
			else
			if (AtmStatus == ATM_DOWN)
				strTempClerkInformation = "DOWN";
			break;

		case 1:
			// 1:OPEN STATUS(ON,OFF)
			if (OpenKey)
				strTempClerkInformation = "ON";
			else
				strTempClerkInformation = "OFF";
			break;
		
		case 2:
			// 2:SEND RECV STATUS(ON,OFF)
			strTempClerkInformation = Op.SendRecvStatus;
			break;

		case 3:
			// 3:TRANSACTION STATUS(ON,OFF)
			strTempClerkInformation = Op.TransationStatus;
			break;

		case 4:
			// 4:POWER STATUS(ON,OFF)
			if (fnAPL_CheckPowerStatus())
				strTempClerkInformation = "ON";
			else
				strTempClerkInformation = "OFF";
			break;

		case 5:
			// 5:AS PASSWORD
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.AsPassWord);
			strTempClerkInformation.TrimRight();
			break;

		case 6:
			// 6:OP PASSWORD
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.OpPassWord);
			strTempClerkInformation.TrimRight();
			break;

		case 7:
			// 7:SCR BANK NAME
			strTempClerkInformation.Format("%3.3s", &TranDefine.ApVersion[10]);
			break;

		case 8:
			// 8:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
			switch (AtmDefine.MachineKind)
			{
				case NANOCASH:
					strTempClerkInformation = "NANOCASH";
					break;
				case MCD3:
					strTempClerkInformation = "MCD3";
					break;
				case TTW:
					strTempClerkInformation = "TTW";
					break;
				case MCD4:
					strTempClerkInformation = "MCD4";
					break;
				case TTW2:
					strTempClerkInformation = "TTW2";
					break;
				case EATMGBA1:									// 2005.11.11
					strTempClerkInformation = "EATMGBA1";
					break;
				case EATMULL1:									// 2005.11.11
					strTempClerkInformation = "EATMULL1";
					break;
				case EATMTTW:									// 2005.11.11
					strTempClerkInformation = "EATMTTW";
					break;
				case TTW2E:										// 2005.11.11
					strTempClerkInformation = "TTW2E";
					break;
				case MCD4E:										// 2005.11.11
					strTempClerkInformation = "MCD4E";
					break;
				case MX8000T:									// 2005.11.11
					strTempClerkInformation = "MX8000T";
					break;
				case MX8000L:									// 2005.11.11
					strTempClerkInformation = "MX8000L";
					break;
				case MX5600T:
					strTempClerkInformation = "MX5600T";
					break;
				case MX5600L:
					strTempClerkInformation = "MX5600L";
					break;
			}
			break;

		case 9:
			// 9:OPERATOR SWITCH STATUS(NONE,SUPERVISOR,RUN,SUPERVISORCHANGED,RUNCHANGED)
			switch (fnSNS_GetOperatorSwitch())
			{
				case OPER_NONE:
					strTempClerkInformation = "NONE";
					break;
				case OPER_SUPERVISOR:
					strTempClerkInformation = "SUPERVISOR";
					break;
				case OPER_RUN:
					strTempClerkInformation = "RUN";
					break;
				case OPER_SUPERVISORCHANGED:
					strTempClerkInformation = "SUPERVISORCHANGED";
					break;
				case OPER_RUNCHANGED:
					strTempClerkInformation = "RUNCHANGED";
					break;
			}
			break;

		case 10:
			// 10:FTP IP(NNNNNNNNNNNN)
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.BpIpAddress4,
											&m_pProfile->NETWORK.BpIpAddress4[4],
											&m_pProfile->NETWORK.BpIpAddress4[8],
											&m_pProfile->NETWORK.BpIpAddress4[12]);
			break;

		case 11:
			// 11:FTP PORT(NNNNN)
			strTempClerkInformation.Format("%5.5s", m_pProfile->NETWORK.BpPortNo4);
			break;

		case 12:
			// 12:ATM SERIAL NUMBER(NNNNNNNN)
			strTempClerkInformation.Format("%8.8s", m_pProfile->NETWORK.AtmSerialNum);
			break;

		case 13:
			// 13:BRANCH NUMBER(NNNNNNNN)
			strTempClerkInformation.Format("%9.9s", m_pProfile->NETWORK.BranchNum);
			break;

		case 14:
			// 14:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 15:
			// 15:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 16:
			// 16:INTERFACE KIND(N)								// 2005.08.31
			strTempClerkInformation.Format("%d", m_pProfile->NETWORK.InterfaceKind);
			break;

		case 17:												// 2005.11.11
			// 17:OP DEVICE(NOT_SET, OP_TOUCH, OP_TSPL, OP_TSPLCHN, OP_HSPL)
			switch (AtmDefine.OpDevice)
			{
				case NOT_SET:
					strTempClerkInformation = "NOT_SET";
					break;
				case OP_TOUCH:
					strTempClerkInformation = "OP_TOUCH";
					break;
				case OP_TSPL:
					strTempClerkInformation = "OP_TSPL";
					break;
				case OP_TSPLCHN:
					strTempClerkInformation = "OP_TSPLCHN";
					break;
				case OP_HSPL:
					strTempClerkInformation = "OP_HSPL";
					break;
			}
			break;

		case 18:												// 2005.12.01
			// 18:OP LED(POWERLED,COMMLED,INSERVICELED,SUPERVISORLED,ERRORLED,CARDLED,REPORTLED,NOTELED)
			strTempClerkInformation = LED_STR_ON;
			strTempClerkInformation += ",";
			strTempClerkInformation += CommunicationsLED;
			strTempClerkInformation += ",";
			strTempClerkInformation += InServiceLED;
			strTempClerkInformation += ",";
			strTempClerkInformation += SupervisorLED;
			strTempClerkInformation += ",";
			strTempClerkInformation += ErrorLED;
			strTempClerkInformation += ",";

			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if ((fstrAPL_ClerkInformationMake(111) != "NORMAL")	||	// 111:MCU(NORMAL, ERROR)
					(Asc2Int(fstrAPL_ClerkInformationMake(119))))		// 119:MCU(C)
					strTempClerkInformation += LED_STR_ON;
				else
					strTempClerkInformation += LED_STR_OFF;
				strTempClerkInformation += ",";

				if ((fstrAPL_ClerkInformationMake(112) != "NORMAL")	||	// 112:SPR(NORMAL, ERROR)
					(fstrAPL_ClerkInformationMake(113) != "NORMAL")	||	// 113:JPR(NORMAL, ERROR)
					(fstrAPL_ClerkInformationMake(120) != "NORMAL")	||	// 120:SPR(MISSING, NORMAL, LOW, EMPTY)
					(fstrAPL_ClerkInformationMake(121) != "NORMAL"))	// 121:JPR(MISSING, NORMAL, LOW, EMPTY)
					strTempClerkInformation += LED_STR_ON;
				else
					strTempClerkInformation += LED_STR_OFF;
				strTempClerkInformation += ",";

				if ((fstrAPL_ClerkInformationMake(110) != "NORMAL")	||	// 110:CSH(NORMAL, ERROR)
					(WithAvail == TRAN_WITH_NOT))
					strTempClerkInformation += LED_STR_ON;
				else
					strTempClerkInformation += LED_STR_OFF;
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if ((fstrAPL_ClerkInformationMake(111) != "正常")	||	// 111:MCU(NORMAL, ERROR)
					(Asc2Int(fstrAPL_ClerkInformationMake(119))))		// 119:MCU(C)
					strTempClerkInformation += LED_STR_ON;
				else
					strTempClerkInformation += LED_STR_OFF;
				strTempClerkInformation += ",";

				if ((fstrAPL_ClerkInformationMake(112) != "正常")	||	// 112:SPR(NORMAL, ERROR)
					(fstrAPL_ClerkInformationMake(113) != "正常")	||	// 113:JPR(NORMAL, ERROR)
					(fstrAPL_ClerkInformationMake(120) != "正常")	||	// 120:SPR(MISSING, NORMAL, LOW, EMPTY)
					(fstrAPL_ClerkInformationMake(121) != "正常"))		// 121:JPR(MISSING, NORMAL, LOW, EMPTY)
					strTempClerkInformation += LED_STR_ON;
				else
					strTempClerkInformation += LED_STR_OFF;
				strTempClerkInformation += ",";

				if ((fstrAPL_ClerkInformationMake(110) != "正常")	||	// 110:CSH(NORMAL, ERROR)
					(WithAvail == TRAN_WITH_NOT))
					strTempClerkInformation += LED_STR_ON;
				else
					strTempClerkInformation += LED_STR_OFF;
			}
			break;

		case 19:												// 2006.06.02
			// 19:SPL DEVICE(NOT_SET, SPL_TOGGLESW, SPL_ONOFFSW)
			switch (AtmDefine.SplDevice)
			{
				case NOT_SET:
					strTempClerkInformation = "NOT_SET";
					break;
				case SPL_TOGGLESW:
					strTempClerkInformation = "SPL_TOGGLESW";
					break;
				case SPL_ONOFFSW:
					strTempClerkInformation = "SPL_ONOFFSW";
					break;
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		case 100:
			// 100:MACHINE KIND(NANOCASH, MCD3, TTW, MCD4, TTW2, EATMGBA1, EATMULL1, EATMTTW, TTW2E, MCD4E, MX8000T, MX8000L, MX5600T, MX5600L)
			strTempClerkInformation = fstrAPL_ClerkInformationMake(8);
			break;

		case 101:
			// 101:ATM STATUS(INIT, ERROR, CLERK, READY, CUSTOM, TRAN, DOWN)
			strTempClerkInformation = fstrAPL_ClerkInformationMake(0);
			break;

		case 102:
			// 102:DOOR STATUS(DOOROPEN, DOORCLOSE)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (DoorStatus == DOOR_OPENED)
					strTempClerkInformation = "DOOROPEN";
				else
				if (DoorStatus == DOOR_CLOSED)
					strTempClerkInformation = "DOORCLOSE";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (DoorStatus == DOOR_OPENED)
					strTempClerkInformation = "保险柜门开";
				else
				if (DoorStatus == DOOR_CLOSED)
					strTempClerkInformation = "保险柜门关";
			}
			break;

		case 103:
			// 103:LINE STATUS(HOSTONLINE, HOSTOFFLINE)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (fnAPL_CheckHostLine())
					strTempClerkInformation = "HOSTONLINE";
				else
					strTempClerkInformation = "HOSTOFFLINE";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (fnAPL_CheckHostLine())
					strTempClerkInformation = "通讯正常";
				else
					strTempClerkInformation = "通讯异常";
			}
			break;

		case 104:
			// 104:MESSAGE
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (!fnAPL_CheckError())
				{
					if (fnCMR_GetDeviceStatus() == NORMAL)		// 2004.07.26
						strTempClerkInformation = "* ERROR *";
					else
						strTempClerkInformation = "* CHECK *(CAM)";
				}
				else
				{
					if (fnCMR_GetDeviceStatus() == NORMAL)
						strTempClerkInformation = "NORMAL";
					else
						strTempClerkInformation = "* CHECK *(CAM)";
				}
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (!fnAPL_CheckError())
				{
					if (fnCMR_GetDeviceStatus() == NORMAL)		// 2004.07.26
						strTempClerkInformation = "* 故障 *";
					else
						strTempClerkInformation = "* 异常 *(CAM)";
				}
				else
				{
					if (fnCMR_GetDeviceStatus() == NORMAL)
						strTempClerkInformation = "正常";
					else
						strTempClerkInformation = "* 异常 *(CAM)";
				}
			}
			break;

		case 105:
			// 105:ERROR CODE([P] - [EEEEE(SS)])
			if (!fnAPL_CheckError())
				strTempClerkInformation.Format("[%1.1c] - [%5.5s(%2.2s)]",
												CurErrBuff.ProcCount[0],
												&CurErrBuff.ErrorCode[0],
												&CurErrBuff.ErrorCode[5]);
			else
				strTempClerkInformation.Format("[%1.1c] - [%5.5s(%2.2s)]",
												m_pProfile->TRANS.ProcCount[0],
												"00000",
												"00");
			break;

		case 106:
			// 106:CST1(CCCC(GGG))
			if (CSTCnt >= 1)
				strTempClerkInformation.Format("%d(%d)", 
												FstCSTCnt,
												FstCSTValue);
			break;

		case 107:
			// 107:CST2(CCCC(GGG))
			if (CSTCnt >= 2)
				strTempClerkInformation.Format("%d(%d)", 
												SndCSTCnt,
												SndCSTValue);
			break;

		case 108:
			// 108:CST3(CCCC(GGG))
			if (CSTCnt >= 3)
				strTempClerkInformation.Format("%d(%d)", 
												TrdCSTCnt,
												TrdCSTValue);
			break;

		case 109:
			// 109:CST4(CCCC(GGG))
			if (CSTCnt >= 4)
				strTempClerkInformation.Format("%d(%d)", 
												FthCSTCnt,
												FthCSTValue);
			break;

		case 110:
			// 110:CSH(NORMAL, ERROR)							// 2004.06.29
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_BOTH))
					strTempClerkInformation = "USER-OFF";
				else
				if ((CashMaxRetractCnt > 0)	&&
					(CashRetractCnt >= CashMaxRetractCnt))
					strTempClerkInformation = "RTT-OVER";
				else
				if (DeviceAutoOff & DEV_CSH)					// 2005.02.28
					strTempClerkInformation = "AUTO-OFF";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_CSH)	// 2004.11.05
					strTempClerkInformation = "OFF";
				else
				if (fnAPL_GetDownErrorDevice(DEV_CSH))
					strTempClerkInformation = "ERROR";
				else
				if (CshLoc != NORMAL_POS)						// 2005.05.26
					strTempClerkInformation = "MISSING";
				else
					strTempClerkInformation = "NORMAL";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				//增加取款锁定 added by liuxl 20110609
				int icwdmaxshutter = IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_CWDMAXSHUTTERNUM, 1);

				if (!(m_pProfile->TRANS.ClerkTransOnOff & TRAN_WITH_BOTH))
					strTempClerkInformation = "人为关闭";
				else
				if ((CashMaxRetractCnt > 0)	&&
					(CashRetractCnt >= CashMaxRetractCnt))
					strTempClerkInformation = "回收超限";
				else
				if (DeviceAutoOff & DEV_CSH)					// 2005.02.28
					strTempClerkInformation = "屏蔽";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_CSH)	// 2004.11.05
					strTempClerkInformation = "关闭";
				else
				if (fnAPL_GetDownErrorDevice(DEV_CSH))
					strTempClerkInformation = "故障";
				else
				if (CshLoc != NORMAL_POS)						// 2005.05.26
					strTempClerkInformation = "无";
//				else
//					strTempClerkInformation = "正常";
				else											// added by liuxl 20110609
				if (icwdmaxshutter > 0 &&
					IniGetInt("D:\\INI\\TRANSACTION.INI", "TRANSACTION", "CWDCURSHUTTERNUM", 0) == icwdmaxshutter)
				{
					strTempClerkInformation = "锁定";
 				}
				else
					strTempClerkInformation = "正常";
			}
			break;

		case 111:
			// 111:MCU(NORMAL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (DeviceAutoOff & DEV_MCU)					// 2005.02.28
					strTempClerkInformation = "AUTO-OFF";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_MCU)	// 2004.11.05
					strTempClerkInformation = "OFF";
				else
				if (fnAPL_GetDownErrorDevice(DEV_MCU))
					strTempClerkInformation = "ERROR";
				else
					strTempClerkInformation = "NORMAL";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (DeviceAutoOff & DEV_MCU)					// 2005.02.28
					strTempClerkInformation = "屏蔽";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_MCU)	// 2004.11.05
					strTempClerkInformation = "关闭";
				else
				if (fnAPL_GetDownErrorDevice(DEV_MCU))
					strTempClerkInformation = "故障";
				else
					strTempClerkInformation = "正常";
			}
			break;

		case 112:
			// 112:SPR(NORMAL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (DeviceAutoOff & DEV_SPR)					// 2005.02.28
					strTempClerkInformation = "AUTO-OFF";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_SPR)	// 2004.11.05
					strTempClerkInformation = "OFF";
				else
				if (fnAPL_GetDownErrorDevice(DEV_SPR))
					strTempClerkInformation = "ERROR";
				else
					strTempClerkInformation = "NORMAL";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (DeviceAutoOff & DEV_SPR)					// 2005.02.28
					strTempClerkInformation = "屏蔽";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_SPR)	// 2004.11.05
					strTempClerkInformation = "关闭";
				else
				if (fnAPL_GetDownErrorDevice(DEV_SPR))
					strTempClerkInformation = "故障";
				else
					strTempClerkInformation = "正常";
			}
			break;

		case 113:
			// 113:JPR(NORMAL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (DeviceAutoOff & DEV_JPR)					// 2005.02.28
					strTempClerkInformation = "AUTO-OFF";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_JPR)	// 2004.11.05
					strTempClerkInformation = "OFF";
				else
				if (fnAPL_GetDownErrorDevice(DEV_JPR))
					strTempClerkInformation = "ERROR";
				else
					strTempClerkInformation = "NORMAL";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (DeviceAutoOff & DEV_JPR)					// 2005.02.28
					strTempClerkInformation = "屏蔽";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_JPR)	// 2004.11.05
					strTempClerkInformation = "关闭";
				else
				if (fnAPL_GetDownErrorDevice(DEV_JPR))
					strTempClerkInformation = "故障";
				else
					strTempClerkInformation = "正常";
			}
			break;

		case 114:
			// 114:CST1(MISSING, NORMAL, LOW, EMPTY, FULL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (CSTCnt >= 1)
				{
					if (FstCSTLoc != NORMAL_POS)
						strTempClerkInformation = "MISSING";
					else
					if (FstCSTStatus == CST_NORMAL)
						strTempClerkInformation = "NORMAL";
					else
					if (FstCSTStatus == CST_NEAR)
						strTempClerkInformation = "LOW";
					else
					if (FstCSTStatus == CST_EMPTY)
						strTempClerkInformation = "EMPTY";
					else
					if (FstCSTStatus == CST_FULL)
						strTempClerkInformation = "FULL";
					else
					if (FstCSTStatus == CST_ERROR)
						strTempClerkInformation = "ERROR";
					else
					if (FstCSTStatus == CST_MANIP)
						strTempClerkInformation = "MANIP";
					else
					if (FstCSTStatus == CST_HIGH)
						strTempClerkInformation = "HIGH";
				}
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (CSTCnt >= 1)
				{
					if (FstCSTLoc != NORMAL_POS)
						strTempClerkInformation = "无";
					else
					if (FstCSTStatus == CST_NORMAL)
						strTempClerkInformation = "正常";
					else
					if (FstCSTStatus == CST_NEAR)
						strTempClerkInformation = "钞少";
					else
					if (FstCSTStatus == CST_EMPTY)
						strTempClerkInformation = "空";
					else
					if (FstCSTStatus == CST_FULL)
						strTempClerkInformation = "满";
					else
					if (FstCSTStatus == CST_ERROR)
						strTempClerkInformation = "故障";
					else
					if (FstCSTStatus == CST_MANIP)
						strTempClerkInformation = "管理";
					else
					if (FstCSTStatus == CST_HIGH)
						strTempClerkInformation = "将满";
				}
			}
			break;

		case 115:
			// 115:CST2(MISSING, NORMAL, LOW, EMPTY, FULL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (CSTCnt >= 2)
				{
					if (SndCSTLoc != NORMAL_POS)
						strTempClerkInformation = "MISSING";
					else
					if (SndCSTStatus == CST_NORMAL)
						strTempClerkInformation = "NORMAL";
					else
					if (SndCSTStatus == CST_NEAR)
						strTempClerkInformation = "LOW";
					else
					if (SndCSTStatus == CST_EMPTY)
						strTempClerkInformation = "EMPTY";
					else
					if (SndCSTStatus == CST_FULL)
						strTempClerkInformation = "FULL";
					else
					if (SndCSTStatus == CST_ERROR)
						strTempClerkInformation = "ERROR";
					else
					if (SndCSTStatus == CST_MANIP)
						strTempClerkInformation = "MANIP";
					else
					if (SndCSTStatus == CST_HIGH)
						strTempClerkInformation = "HIGH";
				}
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (CSTCnt >= 2)
				{
					if (SndCSTLoc != NORMAL_POS)
						strTempClerkInformation = "无";
					else
					if (SndCSTStatus == CST_NORMAL)
						strTempClerkInformation = "正常";
					else
					if (SndCSTStatus == CST_NEAR)
						strTempClerkInformation = "钞少";
					else
					if (SndCSTStatus == CST_EMPTY)
						strTempClerkInformation = "空";
					else
					if (SndCSTStatus == CST_FULL)
						strTempClerkInformation = "满";
					else
					if (SndCSTStatus == CST_ERROR)
						strTempClerkInformation = "故障";
					else
					if (SndCSTStatus == CST_MANIP)
						strTempClerkInformation = "管理";
					else
					if (SndCSTStatus == CST_HIGH)
						strTempClerkInformation = "将满";
				}
			}
			break;

		case 116:
			// 116:CST3(MISSING, NORMAL, LOW, EMPTY, FULL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (CSTCnt >= 3)
				{
					if (TrdCSTLoc != NORMAL_POS)
						strTempClerkInformation = "MISSING";
					else
					if (TrdCSTStatus == CST_NORMAL)
						strTempClerkInformation = "NORMAL";
					else
					if (TrdCSTStatus == CST_NEAR)
						strTempClerkInformation = "LOW";
					else
					if (TrdCSTStatus == CST_EMPTY)
						strTempClerkInformation = "EMPTY";
					else
					if (TrdCSTStatus == CST_FULL)
						strTempClerkInformation = "FULL";
					else
					if (TrdCSTStatus == CST_ERROR)
						strTempClerkInformation = "ERROR";
					else
					if (TrdCSTStatus == CST_MANIP)
						strTempClerkInformation = "MANIP";
					else
					if (TrdCSTStatus == CST_HIGH)
						strTempClerkInformation = "HIGH";
				}
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (CSTCnt >= 3)
				{
					if (TrdCSTLoc != NORMAL_POS)
						strTempClerkInformation = "无";
					else
					if (TrdCSTStatus == CST_NORMAL)
						strTempClerkInformation = "正常";
					else
					if (TrdCSTStatus == CST_NEAR)
						strTempClerkInformation = "钞少";
					else
					if (TrdCSTStatus == CST_EMPTY)
						strTempClerkInformation = "空";
					else
					if (TrdCSTStatus == CST_FULL)
						strTempClerkInformation = "满";
					else
					if (TrdCSTStatus == CST_ERROR)
						strTempClerkInformation = "故障";
					else
					if (TrdCSTStatus == CST_MANIP)
						strTempClerkInformation = "管理";
					else
					if (TrdCSTStatus == CST_HIGH)
						strTempClerkInformation = "将满";
				}
			}
			break;

		case 117:
			// 117:CST4(MISSING, NORMAL, LOW, EMPTY, FULL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (CSTCnt >= 4)
				{
					if (FthCSTLoc != NORMAL_POS)
						strTempClerkInformation = "MISSING";
					else
					if (FthCSTStatus == CST_NORMAL)
						strTempClerkInformation = "NORMAL";
					else
					if (FthCSTStatus == CST_NEAR)
						strTempClerkInformation = "LOW";
					else
					if (FthCSTStatus == CST_EMPTY)
						strTempClerkInformation = "EMPTY";
					else
					if (FthCSTStatus == CST_FULL)
						strTempClerkInformation = "FULL";
					else
					if (FthCSTStatus == CST_ERROR)
						strTempClerkInformation = "ERROR";
					else
					if (FthCSTStatus == CST_MANIP)
						strTempClerkInformation = "MANIP";
					else
					if (FthCSTStatus == CST_HIGH)
						strTempClerkInformation = "HIGH";
				}
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (CSTCnt >= 4)
				{
					if (FthCSTLoc != NORMAL_POS)
						strTempClerkInformation = "无";
					else
					if (FthCSTStatus == CST_NORMAL)
						strTempClerkInformation = "正常";
					else
					if (FthCSTStatus == CST_NEAR)
						strTempClerkInformation = "钞少";
					else
					if (FthCSTStatus == CST_EMPTY)
						strTempClerkInformation = "空";
					else
					if (FthCSTStatus == CST_FULL)
						strTempClerkInformation = "满";
					else
					if (FthCSTStatus == CST_ERROR)
						strTempClerkInformation = "故障";
					else
					if (FthCSTStatus == CST_MANIP)
						strTempClerkInformation = "管理";
					else
					if (FthCSTStatus == CST_HIGH)
						strTempClerkInformation = "将满";
				}
			}
			break;

		case 118:
			// 118:REJECT(MISSING, NORMAL, LOW, EMPTY, FULL, ERROR)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (RejectCSTLoc != NORMAL_POS)
					strTempClerkInformation = "MISSING";
				else
				if (RejectCSTStatus == CST_NORMAL)
					strTempClerkInformation = "NORMAL";
				else
				if (RejectCSTStatus == CST_NEAR)
					strTempClerkInformation = "LOW";
				else
				if (RejectCSTStatus == CST_EMPTY)
					strTempClerkInformation = "EMPTY";
				else
				if (RejectCSTStatus == CST_FULL)
					strTempClerkInformation = "FULL";
				else
				if (RejectCSTStatus == CST_ERROR)
					strTempClerkInformation = "ERROR";
				else
				if (RejectCSTStatus == CST_MANIP)
					strTempClerkInformation = "MANIP";
				else
				if (RejectCSTStatus == CST_HIGH)
					strTempClerkInformation = "HIGH";
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (RejectCSTLoc != NORMAL_POS)
					strTempClerkInformation = "无";
				else
				if (RejectCSTStatus == CST_NORMAL)
					strTempClerkInformation = "正常";
				else
				if (RejectCSTStatus == CST_NEAR)
					strTempClerkInformation = "少";
				else
				if (RejectCSTStatus == CST_EMPTY)
					strTempClerkInformation = "空";
				else
				if (RejectCSTStatus == CST_FULL)
					strTempClerkInformation = "满";
				else
				if (RejectCSTStatus == CST_ERROR)
					strTempClerkInformation = "故障";
				else
				if (RejectCSTStatus == CST_MANIP)
					strTempClerkInformation = "管理";
				else
				if (RejectCSTStatus == CST_HIGH)
					strTempClerkInformation = "将满";
			}
			break;

		case 119:
			// 119:MCU(C)
			strTempClerkInformation = Int2Asc(CardRetractCnt);	// 2006.08.22
			break;

		case 120:
			// 120:SPR(MISSING, NORMAL, LOW, EMPTY)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (DeviceAutoOff & DEV_SPR)					// 2005.02.28
					strTempClerkInformation = "AUTO-OFF";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_SPR)	// 2004.11.05
					strTempClerkInformation = "OFF";
				else
				if (fnAPL_GetDownErrorDevice(DEV_SPR))
					strTempClerkInformation = "ERROR";
				else
				{
					if (SlipHeadStatus != HEADER_NORMAL)
						strTempClerkInformation = "MISSING";
					else
					if (SlipStatus == SLIP_NORMAL)
						strTempClerkInformation = "NORMAL";
					else
					if (SlipStatus == SLIP_LOW_END)
						strTempClerkInformation = "LOW";
					else
					if (SlipStatus == SLIP_EMPTY_PAPER)
						strTempClerkInformation = "EMPTY";
				}


			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (DeviceAutoOff & DEV_SPR)					// 2005.02.28
					strTempClerkInformation = "屏蔽";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_SPR)	// 2004.11.05
					strTempClerkInformation = "关闭";
				else
				if (fnAPL_GetDownErrorDevice(DEV_SPR))
					strTempClerkInformation = "故障";
				else
				{
					if (SlipHeadStatus != HEADER_NORMAL)
						strTempClerkInformation = "无纸";
					else
					if (SlipStatus == SLIP_NORMAL)
						strTempClerkInformation = "正常";
					else
					if (SlipStatus == SLIP_LOW_END)
						strTempClerkInformation = "少纸";
					else
					if (SlipStatus == SLIP_EMPTY_PAPER)
						strTempClerkInformation = "无纸";
				}
			}
			break;

		case 121:
			// 121:JPR(MISSING, NORMAL, LOW, EMPTY)
			if ((AtmDefine.OpDevice == OP_TSPL)		||
				(AtmDefine.OpDevice == OP_HSPL))
			{
				if (DeviceAutoOff & DEV_JPR)					// 2005.02.28
					strTempClerkInformation = "AUTO-OFF";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_JPR)	// 2004.11.05
					strTempClerkInformation = "OFF";
				else
				if (fnAPL_GetDownErrorDevice(DEV_JPR))
					strTempClerkInformation = "ERROR";
				else
				{
					if (JnlHeadStatus != HEADER_NORMAL)
						strTempClerkInformation = "MISSING";
					else
					if (JnlStatus == JNL_NORMAL)
						strTempClerkInformation = "NORMAL";
					else
					if (JnlStatus == JNL_LOW_END)
						strTempClerkInformation = "LOW";
					else
					if (JnlStatus == JNL_EMPTY_PAPER)
						strTempClerkInformation = "EMPTY";
				}
			}
			else
			if ((AtmDefine.OpDevice == NOT_SET)		||
				(AtmDefine.OpDevice == OP_TOUCH)	||
				(AtmDefine.OpDevice == OP_TSPLCHN))
			{
				if (DeviceAutoOff & DEV_JPR)					// 2005.02.28
					strTempClerkInformation = "屏蔽";
				else
				if (m_pProfile->TRANS.DeviceExecOff & DEV_JPR)	// 2004.11.05
					strTempClerkInformation = "关闭";
				else
				if (fnAPL_GetDownErrorDevice(DEV_JPR))
					strTempClerkInformation = "故障";
				else
				{
					if (JnlHeadStatus != HEADER_NORMAL)
						strTempClerkInformation = "无纸";
					else
					if (JnlStatus == JNL_NORMAL)
						strTempClerkInformation = "正常";
					else
					if (JnlStatus == JNL_LOW_END)
						strTempClerkInformation = "纸少";
					else
					if (JnlStatus == JNL_EMPTY_PAPER)
						strTempClerkInformation = "无纸";
				}
			}
			break;

		case 122:
			// 122:ATM SERIAL NUMBER(NNNNNNNN)
			strTempClerkInformation.Format("%8.8s", m_pProfile->NETWORK.AtmSerialNum);
			break;

		case 123:
			// 123:BRANCH NUMBER(NNNNNNNN)
			strTempClerkInformation.Format("%9.9s", m_pProfile->NETWORK.BranchNum);
			break;

		case 124:
			// 124:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 125:
			// 125:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 200:
			// 200:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 201:
			// 201:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 300:
			// 300:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 301:
			// 301:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 302:
			// 302:TRACE DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 400:
			// 400:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 401:
			// 401:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;
			
		case 402:
			// 402:CDU SP VERSION
			if (fnAPL_GetDefineDevice(DEV_CSH))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "CDM", "V00.00.00");
			break;

		case 403:
			// 403:JPR SP VERSION
			if (fnAPL_GetDefineDevice(DEV_JPR))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "JPR", "V00.00.00");
			break;

		case 404:
			// 404:SPR SP VERSION
			if (fnAPL_GetDefineDevice(DEV_SPR))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "SPR", "V00.00.00");
			break;

		case 405:
			// 405:MCU SP VERSION
			if (fnAPL_GetDefineDevice(DEV_MCU))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "IDC", "V00.00.00");
			break;

		case 406:
			// 406:PIN SP VERSION
			if (fnAPL_GetDefineDevice(DEV_PIN))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "PINPAD", "V00.00.00");
			break;

		case 407:
			// 407:CAM SP VERSION
			if (fnAPL_GetDefineDevice(DEV_CMR))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "CAM", "V00.00.00");
			break;

		case 408:
			// 408:SIU SP VERSION
			if (fnAPL_GetDefineDevice(DEV_SNS))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "SIU", "V00.00.00");
			break;

		case 409:
			// 409:UPS SP VERSION
			if (fnAPL_GetDefineDevice(DEV_UPS))
			{
				switch (AtmDefine.PowerHandler)					// 2005.12.23
				{
					case NOT_SET:
						break;
					case PNC_TYPE:
						strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "UPS", "V00.00.00");
						break;
					case UPS_TYPE:
						strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "UPS", "V00.00.00");
						break;
					case EUL_TYPE:
						break;
					case EUC_TYPE:
						break;
				}
			}
			break;

		case 410:
			// 410:TTU SP VERSION
			switch (AtmDefine.OpDevice)							// 2005.12.23
			{
				case NOT_SET:
					break;
				case OP_TOUCH:
					break;
				case OP_TSPL:
				case OP_TSPLCHN:
					strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "TTU", "V00.00.00");
					break;
				case OP_HSPL:
					strTempClerkInformation = RegGetStr(_REGKEY_SPVERSION, "TTU", "V00.00.00");
					break;
			}
			break;

		case 411:
			// 411:MWI SP VERSION
			strTempClerkInformation = RegGetStr(_REGKEY_MWI, "VERSION", "V00.00.00");
			break;

		case 412:
			// 412:COMMONITOR SP VERSION
			break;

		case 413:
			// 413:CDU EP VERSION
			if (fnAPL_GetDefineDevice(DEV_CSH))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "CDM", "V00.00.00");
			break;

		case 414:
			// 414:JPR EP VERSION
			if (fnAPL_GetDefineDevice(DEV_JPR))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "JPR", "V00.00.00");
			break;

		case 415:
			// 415:SPR EP VERSION
			if (fnAPL_GetDefineDevice(DEV_SPR))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "SPR", "V00.00.00");
			break;

		case 416:
			// 416:MCU EP VERSION
			if (fnAPL_GetDefineDevice(DEV_MCU))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "IDC", "V00.00.00");
			break;

		case 417:
			// 417:PIN EP VERSION
			if (fnAPL_GetDefineDevice(DEV_PIN))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "PINPAD", "V00.00.00");
			break;

		case 418:
			// 418:CAM EP VERSION
			if (fnAPL_GetDefineDevice(DEV_CMR))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "CAM", "V00.00.00");
			break;

		case 419:
			// 419:SIU EP VERSION
			if (fnAPL_GetDefineDevice(DEV_SNS))					// 2004.10.22
				strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "SIU", "V00.00.00");
			break;

		case 420:
			// 420:UPS EP VERSION
			if (fnAPL_GetDefineDevice(DEV_UPS))
			{
				switch (AtmDefine.PowerHandler)					// 2005.12.23
				{
					case NOT_SET:
						break;
					case PNC_TYPE:
						strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "UPS", "V00.00.00");
						break;
					case UPS_TYPE:
						strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "UPS", "V00.00.00");
						break;
					case EUL_TYPE:
						break;
					case EUC_TYPE:
						break;
				}
			}
			break;

		case 421:
			// 421:TTU EP VERSION
			switch (AtmDefine.OpDevice)							// 2005.12.23
			{
				case NOT_SET:
					break;
				case OP_TOUCH:
					break;
				case OP_TSPL:
				case OP_TSPLCHN:
					strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "TTU", "V00.00.00");
					break;
				case OP_HSPL:
					strTempClerkInformation = RegGetStr(_REGKEY_EPVERSION, "TTU", "V00.00.00");
					break;
			}
			break;

/////////////////////////////////////////////////////////////////////////////
		case 500:
			// 500:ATM SERIAL NUMBER(NNNNNNNN)
			strTempClerkInformation.Format("%8.8s", m_pProfile->NETWORK.AtmSerialNum);
			break;

		case 501:
			// 501:BRANCH NUMBER(NNNNNNNN)
			strTempClerkInformation.Format("%9.9s", m_pProfile->NETWORK.BranchNum);
			break;

		case 502:
			// 502:RESERVED NUMBER1(NNNNNNNN)
			strTempClerkInformation.Format("%8.8s", m_pProfile->NETWORK.SubBranchNum);
			break;

		case 503:
			// 503:RESERVED NUMBER2(NNNNNNNN)
			strTempClerkInformation.Format("%8.8s", m_pProfile->NETWORK.RoomNum);
			break;

		case 504:
			// 504:HOST IP(NNNnnnNNNnnn)
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.BpIpAddress,
											&m_pProfile->NETWORK.BpIpAddress[4],
											&m_pProfile->NETWORK.BpIpAddress[8],
											&m_pProfile->NETWORK.BpIpAddress[12]);
			break;

		case 505:
			// 505:HOST PORT(NNNNN)
			strTempClerkInformation.Format("%5.5s", m_pProfile->NETWORK.BpPortNo);
			break;

		case 506:
			// 506:HOST IP2(NNNnnnNNNnnn)
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.BpIpAddress2,
											&m_pProfile->NETWORK.BpIpAddress2[4],
											&m_pProfile->NETWORK.BpIpAddress2[8],
											&m_pProfile->NETWORK.BpIpAddress2[12]);
			break;

		case 507:
			// 507:HOST PORT2(NNNNN)
			strTempClerkInformation.Format("%5.5s", m_pProfile->NETWORK.BpPortNo2);
			break;

		case 508:
			// 508:ATM IP(NNNnnnNNNnnn)
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.AtmIPAddress,
											&m_pProfile->NETWORK.AtmIPAddress[4],
											&m_pProfile->NETWORK.AtmIPAddress[8],
											&m_pProfile->NETWORK.AtmIPAddress[12]);
			break;

		case 509:
			// 509:ATM SUBNET MASK(NNNnnnNNNnnn)
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.AtmSubnetMask,
											&m_pProfile->NETWORK.AtmSubnetMask[4],
											&m_pProfile->NETWORK.AtmSubnetMask[8],
											&m_pProfile->NETWORK.AtmSubnetMask[12]);
			break;

		case 510:
			// 510:ATM GATE WAY(NNNnnnNNNnnn)
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.AtmGateWay,
											&m_pProfile->NETWORK.AtmGateWay[4],
											&m_pProfile->NETWORK.AtmGateWay[8],
											&m_pProfile->NETWORK.AtmGateWay[12]);
			break;

		case 511:
			// 511:ATM PORT(NNNNN)
			strTempClerkInformation.Format("%5.5s", m_pProfile->NETWORK.AtmPortNo);
			break;

		case 512:
			// 512:HOST IP3(NNNnnnNNNnnn) : BID					// 2005.04.19
			strTempClerkInformation.Format("%3.3s%3.3s%3.3s%3.3s", 
											m_pProfile->NETWORK.BpIpAddress3,
											&m_pProfile->NETWORK.BpIpAddress3[4],
											&m_pProfile->NETWORK.BpIpAddress3[8],
											&m_pProfile->NETWORK.BpIpAddress3[12]);
			break;

		case 513:
			// 513:HOST PORT3(NNNNN) : BID						// 2005.04.19
			strTempClerkInformation.Format("%5.5s", m_pProfile->NETWORK.BpPortNo3);
			break;

		case 514:
			// 514:ADDCASHMODE									// 2007.09.08	// 2008.11.11 默认改为1:网点加钞
			strTempClerkInformation.Format("%d",Asc2Int(RegGetStr("SOFTWARE\\ATM\\APP\\ADDCASHMODE", ZERO2, "1").GetBuffer(0), 1));
			break;



/////////////////////////////////////////////////////////////////////////////
		case 600:
			// 600:CST1(CCCC(GGG))
			if (CSTCnt >= 1)
				strTempClerkInformation.Format("%d(%d)", 
												FstCSTCnt,
												FstCSTValue);
			break;

		case 601:
			// 601:CST2(CCCC(GGG))
			if (CSTCnt >= 2)
				strTempClerkInformation.Format("%d(%d)", 
												SndCSTCnt,
												SndCSTValue);
			break;

		case 602:
			// 602:CST3(CCCC(GGG))
			if (CSTCnt >= 3)
				strTempClerkInformation.Format("%d(%d)", 
												TrdCSTCnt,
												TrdCSTValue);
			break;

		case 603:
			// 603:CST4(CCCC(GGG))
			if (CSTCnt >= 4)
				strTempClerkInformation.Format("%d(%d)", 
												FthCSTCnt,
												FthCSTValue);
			break;

		case 604:
			// 604:CSH(NORMAL, ERROR)							// 2004.06.29
			strTempClerkInformation = fstrAPL_ClerkInformationMake(110);
			break;

		case 605:
			// 605:MCU(NORMAL, ERROR)
			strTempClerkInformation = fstrAPL_ClerkInformationMake(111);
			break;

		case 606:
			// 606:SPR(NORMAL, ERROR)
			strTempClerkInformation = fstrAPL_ClerkInformationMake(112);
			break;

		case 607:
			// 607:JPR(NORMAL, ERROR)
			strTempClerkInformation = fstrAPL_ClerkInformationMake(113);
			break;
																// CST 1
		case 608:
			// 608:CST1(FstCSTValue)
			if (CSTCnt >= 1)
				strTempClerkInformation.Format("%3.3d", FstCSTValue);
			break;

		case 609:
			// 609:CST1(FstCSTCnt)
			if (CSTCnt >= 1)
				strTempClerkInformation = ZERO4;
			break;

		case 610:
			// 610:CST1(FstCSTValue * FstCSTCnt)
			if (CSTCnt >= 1)
				strTempClerkInformation = ZERO6;
			break;
																// CST 2
		case 611:
			// 611:CST2(SndCSTValue)
			if (CSTCnt >= 2)
				strTempClerkInformation.Format("%3.3d", SndCSTValue);
			break;

		case 612:
			// 612:CST2(SndCSTCnt)
			if (CSTCnt >= 2)
				strTempClerkInformation = ZERO4;
			break;

		case 613:
			// 613:CST2(SndCSTValue * SndCSTCnt)
			if (CSTCnt >= 2)
				strTempClerkInformation = ZERO6;
			break;
																// CST 3
		case 614:
			// 614:CST3(TrdCSTValue)
			if (CSTCnt >= 3)
				strTempClerkInformation.Format("%3.3d", TrdCSTValue);
			break;
			
		case 615:
			// 615:CST3(TrdCSTCnt)
			if (CSTCnt >= 3)
				strTempClerkInformation = ZERO4;
			break;

		case 616:
			// 616:CST3(TrdCSTValue * TrdCSTCnt)
			if (CSTCnt >= 3)
				strTempClerkInformation = ZERO6;
			break;
																// CST 4
		case 617:
			// 617:CST4(FthCSTValue)
			if (CSTCnt >= 4)
				strTempClerkInformation.Format("%3.3d", FthCSTValue);
			break;
			
		case 618:
			// 618:CST4(FthCSTCnt)
			if (CSTCnt >= 4)
				strTempClerkInformation = ZERO4;
			break;
			
		case 619:
			// 619:CST4(FthCSTValue * FthCSTCnt)
			if (CSTCnt >= 4)
				strTempClerkInformation = ZERO6;
			break;

/////////////////////////////////////////////////////////////////////////////
		case 700:
			// 700:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 701:
			// 701:SERIAL NO(X6)
			break;

		case 702:
			// 702:CARD NUMBER(X19)
			break;

/////////////////////////////////////////////////////////////////////////////
		case 800:
			// 800-811:EJR DATA(X40)
			break;

		case 812:
			// 812-815:PIC NAME
			break;

/////////////////////////////////////////////////////////////////////////////
		case 900:
			// 900:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 901:
			// 901:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 902:
			// 902:EJM START DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 903:
			// 903:EJM END DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1000:
			// 900:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 1001:
			// 901:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 1002:
			// 902:EJM START DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 1003:
			// 903:EJM END DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1100:
			// 900:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 1101:
			// 901:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 1102:
			// 902:EJM START DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

		case 1103:
			// 903:EJM END DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1200:
			// 1000:CST1(VVV)
			if (CSTCnt >= 1)
				strTempClerkInformation.Format("%3.3d", FstCSTValue);
			break;

		case 1201:
			// 1001:CST2(VVV)
			if (CSTCnt >= 2)
				strTempClerkInformation.Format("%3.3d", SndCSTValue);
			break;

		case 1202:
			// 1002:CST3(VVV)
			if (CSTCnt >= 3)
				strTempClerkInformation.Format("%3.3d", TrdCSTValue);
			break;
			
		case 1203:
			// 1003:CST4(VVV)
			if (CSTCnt >= 4)
				strTempClerkInformation.Format("%3.3d", FthCSTValue);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1300:
			// 1300:AP VERSION(V00-00-00)
			strTempClerkInformation.Format("%9.9s", m_pProfile->TRANS.ApVersion);
			break;

		case 1301:
			// 1301:TRANSACTION DATE(YYYYMMDD)
			strTempClerkInformation.Format("%8.8s", m_pProfile->TRANS.YYYYMMDD);
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1400:												// 2005.08.31
			// 1400:TRAN BATCH NO(NNNNNNNNNnNNNN)
			strTempClerkInformation.Format("%14.14s", RegGetStr(_REGKEY_DEVICE, _REGKEY_DEVICE_TRANBATCHNO, ""));
			break;

		case 1401:												// 2005.08.31
			// 1401:INPUT BATCH NO(NNNNNNNNNnNNNN)
			strTempClerkInformation = "";
			break;

/////////////////////////////////////////////////////////////////////////////
		case 1500:												// 2007.09.08
			// 1500:_INIKEY_TIMEDEFINE_ADSETNUM
			strTempClerkInformation.Format("%2.2d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "ADSETNUM", 0));
			break;

		case 1501:												// 2007.09.08
			// 1501:_INIKEY_TIMEDEFINE_ADTIME
			strTempClerkInformation.Format("%2.2d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "ADTIME", 0));
			break;

		case 1502:												// 2007.09.08
			// 1502:_INIKEY_TIMEDEFINE_RECVTIMEOUT
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_RECVTIMEOUT, 0));
			break;

		case 1503:												// 2007.09.08
			// 1503:_INIKEY_TIMEDEFINE_KEYINTIMEOUT
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "KEYINTIMEOUT", 0));
			break;

		case 1504:												// 2007.09.08
			// 1504:_INIKEY_TIMEDEFINE_TAKECARDTIMEOUT
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECARDTIMEOUT, 0));
			break;

		case 1505:												// 2007.09.08
			// 1505:_INIKEY_TIMEDEFINE_TAKECASHTIMEOUT
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TAKECASHTIMEOUT, 0));
			break;

		case 1506:												// 2007.09.08
			// 1506:_INIKEY_TIMEDEFINE_IDLERETRYTIME
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_IDLERETRYTIME, 0));
			break;

		case 1507:												// 2007.09.08
			// 1507:_INIKEY_TIMEDEFINE_DETAILNUM
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "DETAILNUM", 0));
			break;

		case 1508:												// 2007.09.08
			// 1508:_INIKEY_TIMEDEFINE_ERRORNOTICETIME
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "ERRORNOTICETIME", 0));
			break;

		case 1509:												// 2007.09.08
			// 1509:_INIKEY_TIMEDEFINE_AUTORERETRYTIME
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "AUTORERETRYTIME", 0));
			break;

		case 1510:												// 2007.09.08
			// 1510:_INIKEY_TIMEDEFINE_CWCRETRYTIME
			strTempClerkInformation.Format("%3.3d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "CWCRETRYTIME", 0));
			break;

		case 1511:												// 2007.09.08
			// 1511:_INIKEY_TIMEDEFINE_WITHMAXAMOUNT
			strTempClerkInformation.Format("%4.4d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT, 0));
			break;

		case 1512:												// 2007.09.08
			// 1512:_INIKEY_TIMEDEFINE_WITHMINAMOUNT
			strTempClerkInformation.Format("%4.4d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMINAMOUNT, 0));
			break;

		case 1513:												// 2007.09.08
			// 1513:_INIKEY_TIMEDEFINE_TRANMAXREMITTANCE
			strTempClerkInformation.Format("%6.6d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "TRANMAXREMITTANCE", 50000));   //转账汇款最大最小值可以为0所以初始值不设为0
			break;

		case 1514:												// 2007.09.08
			// 1514:_INIKEY_TIMEDEFINE_TRANMINREMITTANCE
			strTempClerkInformation.Format("%6.6d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "TRANMINREMITTANCE", 50));
			break;

		case 1515:												// 2007.09.08
			// 1515:_INIKEY_TIMEDEFINE_TRANMAXAMOUNT
			strTempClerkInformation.Format("%6.6d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TRANMAXAMOUNT, 50000));
			break;

		case 1516:												// 2007.09.08
			// 1516:_INIKEY_TIMEDEFINE_TRANMINAMOUNT
			strTempClerkInformation.Format("%4.4d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, "TRANMINAMOUNT", 50));
			break;
//////////////////////////////////////////////////////////////////////////
////////liyi add 2008.09.23
		case 1517:												
			// 1517:_INIKEY_TIMEDEFINE_SAVEKINDPARA
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_SAVEKINDPARA, 50));
			break;
				
		case 1518:												
			// 1518:_INIKEY_TIMEDEFINE_ZZTOHMAXNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMAXNUM, 50000));
			break;

		case 1519:												
			// 1519:_INIKEY_TIMEDEFINE_ZZTOHMINNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_ZZTOHMINNUM, 50));
			break;

		case 1520:												
			// 1520:_INIKEY_TIMEDEFINE_DHTOHMAXNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMAXNUM, 50));
			break;

		case 1521:												
			// 1521:_INIKEY_TIMEDEFINE_DHTOHMINNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_DHTOHMINNUM, 50));
			break;

		case 1522:												
			// 1522:_INIKEY_TIMEDEFINE_TZCKTOHMAXNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMAXNUM, 50));
			break;

		case 1523:												
			// 1523:_INIKEY_TIMEDEFINE_TZCKTOHMINNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_TZCKTOHMINNUM, 50));
			break;

		case 1524:												
			// 1524:_INIKEY_TIMEDEFINE_HTODHMAXNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMAXNUM, 50));
			break;

		case 1525:												
			// 1525:_INIKEY_TIMEDEFINE_HTODHMINNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTODHMINNUM, 50));
			break;

		case 1526:											
			// 1526:_INIKEY_TIMEDEFINE_HTOTZCQMAXNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMAXNUM, 50));
			break;

		case 1527:											
			// 1527:_INIKEY_TIMEDEFINE_HTOTZCQMINNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOTZCQMINNUM, 50));
			break;

		case 1528:												
			// 1528:_INIKEY_TIMEDEFINE_HTOZZMAXNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMAXNUM, 50));
			break;

		case 1529:												
			// 1529:_INIKEY_TIMEDEFINE_HTOZZMINNUM
			strTempClerkInformation.Format("%8.8d", IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_HTOZZMINNUM, 50));
			break;

		// yaokq add 20081031

		case 1600:
			strTempClerkInformation.Format("%d",CSTCnt);
			break;

		case 1601:
			strTempClerkInformation.Format("%4.4d",FstCSTValue);
			break;

		case 1602:
			strTempClerkInformation.Format("%4.4d",SndCSTValue);
			break;

		case 1603:
			strTempClerkInformation.Format("%4.4d",TrdCSTValue);
			break;

		case 1604:
			strTempClerkInformation.Format("%4.4d",FthCSTValue);
			break;

		case 1605:
			strTempClerkInformation.Format("%4.4d",fnCSH_GetNumberOfTotalSetCash(CSH_CST_1));
			break;
		
		case 1606:
			strTempClerkInformation.Format("%4.4d",fnCSH_GetNumberOfTotalSetCash(CSH_CST_2));
			break;

		case 1607:
			strTempClerkInformation.Format("%4.4d",fnCSH_GetNumberOfTotalSetCash(CSH_CST_3));
			break;

		case 1608:
			strTempClerkInformation.Format("%4.4d",fnCSH_GetNumberOfTotalSetCash(CSH_CST_4));
			break;

		case 1609:
			strTempClerkInformation.Format("%4.4d",FstCSTCnt);
			break;

		case 1610:
			strTempClerkInformation.Format("%4.4d",SndCSTCnt);
			break;

		case 1611:
			strTempClerkInformation.Format("%4.4d",TrdCSTCnt);
			break;

		case 1612:
			strTempClerkInformation.Format("%4.4d",FthCSTCnt);
			break;

		case 1613:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->WITHTOTAL.DATA[1].Amount[10]);
			break;

		case 1614:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->WITHTOTAL.DATA[2].Amount[10]);
			break;

		case 1615:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->WITHTOTAL.DATA[3].Amount[10]);
			break;

		case 1616:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->WITHTOTAL.DATA[4].Amount[10]);
			break;

		case 1617:
			TempRejectCount1 = fnCSH_GetNumberOfTotalSetCash(CSH_CST_1)	-
				FstCSTCnt - fnCSH_GetNumberOfDispenseCash(CSH_CST_1);
			if(TempRejectCount1 < 0)
				TempRejectCount1 = 0;
			strTempClerkInformation.Format("%4.4d",TempRejectCount1);
			break;

		case 1618:
			TempRejectCount2 = fnCSH_GetNumberOfTotalSetCash(CSH_CST_2)	-
				SndCSTCnt - fnCSH_GetNumberOfDispenseCash(CSH_CST_2);
			if(TempRejectCount2 < 0)
				TempRejectCount2 = 0;
			strTempClerkInformation.Format("%4.4d",TempRejectCount2);
			break;

		case 1619:
			TempRejectCount3 = fnCSH_GetNumberOfTotalSetCash(CSH_CST_3)	-
				TrdCSTCnt - fnCSH_GetNumberOfDispenseCash(CSH_CST_3);
			if(TempRejectCount3 < 0)
				TempRejectCount3 = 0;
			strTempClerkInformation.Format("%4.4d",TempRejectCount3);
			break;

		case 1620:
			TempRejectCount4 = fnCSH_GetNumberOfTotalSetCash(CSH_CST_4)	-
				FthCSTCnt - fnCSH_GetNumberOfDispenseCash(CSH_CST_4);
			if(TempRejectCount4 < 0)
				TempRejectCount4 = 0;
			strTempClerkInformation.Format("%4.4d",TempRejectCount4);
			break;
		
		case 1621:
			strTempClerkInformation.Format("%10.10d",CshInitAmount);
			break;

		case 1622:
			strTempClerkInformation.Format("%10.10d",CshRemainAmount);
			break;

		case 1623:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->WITHTOTAL.DATA[0].Count[2]);
			break;

		case 1624:
			strTempClerkInformation.Format("%10.10s",&m_pProfile->WITHTOTAL.DATA[0].Amount[2]);
			break;

		case 1625:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->TRANSTOTAL.DATA[0].Count[2]);
			break;

		case 1626:
			strTempClerkInformation.Format("%10.10s",&m_pProfile->TRANSTOTAL.DATA[0].Amount[2]);
			break;

		case 1627:
			strTempClerkInformation.Format("%4.4s",&m_pProfile->WITHTOTAL.DATA[5].Count[2]);	
			break;

		case 1628:
			strTempClerkInformation.Format("%10.10s",&m_pProfile->WITHTOTAL.DATA[5].Amount[2]);
			break;

		case 1629:
			strTempClerkInformation.Format("%4.4d",IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTCNT, 0));
			break;

		case 1630:
			strTempClerkInformation.Format("%10.10d",IniGetInt(_TRANSACTION_INI, _INIKEY_TRANSACTION, _INIKEY_TRANSACTION_WITHRETRACTAMT, 0) / 100);
			break;

		case 1631:
			strTempClerkInformation.Format("%4.4d",Asc2Int(fstrMCU_GetRetractCnt()));
			break;


/////////////////////////////////////////////////////////////////////////////
		default:
			break;
	}
	
	return strTempClerkInformation;
}

// Clerk Exec Check
int	CDevCmn::fnAPL_ClerkExecCheck()
{
	return TRUE;
}

// Ic Send Apdu
int	CDevCmn::fnAPL_IcSendApdu(BYTE btPrtCode, BYTE* btpApdu, UINT nApduLen, BYTE* btpResp, int* npRespLen)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_IcSendApdu(btPrtCode[%x], nApduLen[%d])", btPrtCode, nApduLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_IcSendApdu(btpApdu)", btpApdu, nApduLen);

	int		nReturn = FALSE;
	CString	strReturn("");

	nReturn = fnMCU_ICSendData(btPrtCode, MakeUnPack(btpApdu, nApduLen).GetBuffer(0));
	if (!nReturn)												// Ok
		nReturn = fnAPL_CheckDeviceAction(DEV_MCU);

	strReturn = fstrMCU_GetICData();
	if (nReturn)												// Ok
	{
		*npRespLen = MakePack(strReturn.GetBuffer(0), btpResp, strReturn.GetLength());
		nReturn = TRUE;
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_IcSendApdu():return(npRespLen[%d])", *npRespLen);
HexDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_IcSendApdu(btpResp):return", btpResp, *npRespLen);
	}
	else														// Error
	{
		*npRespLen = 0;
		nReturn = FALSE;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_IcSendApdu():return(%d)", nReturn);
	return nReturn;
}

// Cash Retract : 2011.01.04
int	CDevCmn::fnAPL_CashRetract(int nRetractFlag)
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CashRetract(nRetractFlag[%d])", nRetractFlag);

	if (nRetractFlag)
		;
	else
		return T_OK;

	switch (AtmDefine.CashHandler)
	{
		case BRM_SET:
		case CDU_SHUTTER1510_SET:
		case CDU_SHUTTER20_SET:
		case CDU_SHUTTER30_SET:
		case CDU_RETRACT20_SET:
		case CDU_RETRACT30_SET:
		case CDU_PRESENT20_SET:
		case CDU_PRESENT30_SET:
			break;
		case CDU_GPRESENT20_SET:
		case CDU_GPRESENT30_SET:								// 2005.12.08
// 			memcpy(&GuideMsgTbl[35][ApDeviceLangMode][25], &m_pProfile->NETWORK.AtmSerialNum[0], 8);
// 			fnSCR_DisplayStringArray(1, GuideMsgTbl[35][ApDeviceLangMode]);
			fnSCR_DisplayScreen(711);							// Display Version
			fnAPL_CheckDeviceAction(DEV_CSH);
			fnCSH_Retract();
			fnAPL_CheckDeviceAction(DEV_CSH);
			break;
		case CDU_TRAY20_SET:
		case CDU_TRAY30_SET:
			break;
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPL_CashRetract():return");
	return T_OK;
}

