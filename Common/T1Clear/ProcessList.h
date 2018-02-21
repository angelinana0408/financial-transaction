#ifndef _KILL_LIST
#define _KILL_LIST

#define itemof(item)		(sizeof(item) / sizeof(item[0]))

static char KillList[][80] = 
{
											// 중국 AP
	"loader.exe"			,				// Winatm Loader
	"netftpd.exe"			,				// ftpD
	"nhvdm.exe"				,				// 단체테스트(nano)
	"operation.exe"			,				// 단체테스트(mcd3,ttw)
	"devman.exe"			,				// 단체테스트(mcd3,ttw)

	"rear.exe"				,				// OP Loader
	"rearap.exe"			,				// OM Loader
	"omdemon.exe"			,				// OM <-> AP Interface Module
	"devtest.exe"			,				// 단체테스트
	"KeyMgr.exe"			,				// Key Manager
	
											// SP관련	
	"Kxevtvwr.exe"			,				// 
	"DEP_SIM.exe"			,				// 
	"KXWatchDogSvr.exe"		,				// 
	"KXSysMonSvr.exe"		,				// 
	"KXTraceViewerSvr.exe"	,				// 
	"AtmBattery.exe"		,				// 
	"AtmUps.exe"			,				// 
	"vdm.exe"				,				// 
	"IDC_SIM.exe"			,				// 
	"Pin_sim.exe"			,				// 
	"siu_sim.exe"			,				// 
	"TTU_SIM.exe"			,				// 
	"CameraSimulator.exe"	,				// 
	"CDM_Sim.exe"			,				// 
	"SimDocumentPtr.exe"	,				// 
	"SimJournalPtr.exe"		,				// 
	"SimReceiptPtr.exe"		,				// 
	"SimCashAcceptor.exe"	,				// 
	"KXPrinterErrors.exe"	,				// 
	"PTRScanner_Sim.exe"	,				// 
	"SimPassbookPtr.exe"	,				// 
	"CommIF.exe"			,				// 
	"ErrorInfoView.exe"		,				// 
	"HsCamera.exe"			,				// 
	"PtrApp.exe"			,				// 
	"SerialDevice.exe"		,				// 
	"siu.exe"				,				// 
	"T1_Jptr.exe"			,				// 
	"T1_rptr.exe"			,				// 
	"KXTraceViewer.exe"		,				// 
	"T1_idc.exe"			,				// 
	"KXReboot.exe"			,				// 
	"kxspregister.exe"		,				// 
	
											// 중국 NANOCASH
	"IDC_NANO.exe"			,				// 
	"MySiu.exe"				,				// 
	"MyPinPad.exe"			,				// 
	"HSCAMSp.exe"			,				// 
	"ComMonitor.exe"		,				// 
	"DcpEmul.exe"			,				// 
	"DotJournalPrinter.exe"	,				// 
	"ComMonitor4DJPR.exe"	,				// 
	"DotJPR.exe"			,				// 

	"HcCamera.exe"			,				// 중국 MCD3
	"Siu.exe"				,				// 
	"MyPinPad.exe"			,				// 
	"RPtrHCShu.exe"			,				// 
	"Idchcshu.exe"			,				// 
	"Ups.exe"				,				// 
	"DotJournalPrinter.exe"	,				// 
	"ComMonitor4DJPR.exe"	,				// 
	"DotJPR.exe"			,				// 

	"NHMCD4IDC.exe"			,				// 중국 MCD4 & TTW2 : 2005.04.19
	"COMMONITOR4CDM.exe"	,				// 
	"COMMONITOR4DJPR.exe"	,				// 
	"DOTJPR.exe"			,				// 
	"COMMONITOR4PNC.exe"	,				//
	"NH_SIU.exe"			,				// 
	"HsCamera.exe"			,				//
	"RPLVFDCTRL.exe"		,				// 
	"TSPL.exe"				,				// 중국 TSPL : 2005.12.01
	"ComMonitor4TSPL.exe"	,				//
	
	"HcCamera.exe"			,				// 중국 TTW
	"Siu.exe"				,				// 
	"MyPinPad.exe"			,				// 
	"RPtrHCShu.exe"			,				// 
	"TTW_EX_MCU.exe"		,				// 
	"TTW_EX_SPR.exe"		,				// 
	"Ups.exe"				,				// 
	"DotJournalPrinter.exe"	,				// 
	"ComMonitor4DJPR.exe"	,				// 
	"DotJPR.exe"			,				// 

	"ComMonitor4DJPR.exe"	,				// 중국 MB2500N : 2005.11.11
	"ComMonitor4PNC.exe"	,				//
	"DcpEmul.exe"			,				//
	"DotJPR.exe"			,				//
	"HsCamera.exe"			,				//
	"KIOSK_MCU.exe"			,				//
	"MAGTEK.exe"			,				//
	"MyPinPad.exe"			,				//
	"NH_SIU.exe"			,				//
	"NHCdm.exe"				,				// 2006.03.09
	"NW_DotJPR.exe"			,				//
	"NW_KSPR.exe"			,				//
	"SANKYO4XFS30.exe"		,				//

	"CDMCIMSP.exe"			,				// MM8000T : 2008.12.09
	"Dipl.exe"				,				//
	"DiplWait.exe"			,				//
	"Router.exe"			,				//
	"NHCDM4CRM.exe"			,				//
	"NHCIM4CRM.exe"			,				//

	"NHPTR4PRT.exe"			,				// MX5600T : 2009.08.25
	"NHPIN4EPP.exe"			,				//

	"demon.exe"				,				// 
	"main.exe"				,				// 
	"vdmrun.exe"			,				// 
	"Opmain.exe"			,				// 
	"MyPinpad.exe"			,				// 
	"DcpEmul.exe"			,				// 
	"IEXPLORE.exe"			,				// 
	"CameraSimulator.exe"	,				// 
	"CDM_Sim.exe"			,				// 
	"DEP_SIM.exe"			,				// 
	"HSSiu.exe"				,				// 
	"IDC_SIM.exe"			,				// 
	"commonitor.exe"		,				// 
	"Kxevtvwr.exe"			,				// 
	"KXPrinterErrors.exe"	,				// 
	"KXReboot.exe"			,				// 
	"kxspregister.exe"		,				// 
	"KXSysMonSvr.exe"		,				// 
	"KXTraceViewerSvr.exe"	,				// 
	"KXWatchDogSvr.exe"		,				// 
	"Pin_sim.exe"			,				// 
	"PTRScanner_Sim.exe"	,				// 
	"SimCashAcceptor.exe"	,				// 
	"SimDocumentPtr.exe"	,				// 
	"SimJournalPtr.exe"		,				// 
	"SimPassbookPtr.exe"	,				// 
	"SimReceiptPtr.exe"		,				// 
	"siu_sim.exe"			,				// 
	"TTU_SIM.exe"			,				// 
	"vdm.exe"				,				// 

											// 전기종 통합 Kill : 2006.06.02
											// CDU
	"ComMonitor4CDM.exe"	,				//
	"ComMonitor4TSCDM.exe"	,				// 2007.05.31
	"NHCDM4GCDU.exe"		,				//
	"NHCDM.exe"				,				//
	"NHCDM4TSCDU.exe"		,				//
											// IDC
	"NHMCD4IDC.exe"			,				//
	"TTW_EX_MCU.exe"		,				//
	"IDC_NANO.exe"			,				//
	"SANKYO4XFS30.exe"		,				//
	"KMCU4DCP30.exe"		,				//
	"OMRON4IDC30.exe"		,				//
	"Idchcshu.exe"			,				//
	"KIOSK_MCU.exe"			,				//
	"MAGTEK.exe"			,				//
											// SPR
	"TTW_EX_SPR.exe"		,				//
	"NW_KSPR.exe"			,				//
	"NW_NC2.exe"			,				//
	"RPtrHCShu.exe"			,				//
											// JPR
	"ComMonitor4DJPR.exe"	,				//
	"DotJPR.exe"			,				//
	"NW_DotJPR.exe"			,				//
	"DotJournalPrinter.exe"	,				//
											// SIU
	"ComMonitor4PNC.exe"	,				//
	"NH_SIU.exe"			,				//
	"HSSiu.exe"				,				//
	"DcpEmul.exe"			,				//
	"MySiu.exe"				,				//
	"SIU.exe"				,				//
											// PIN
	"MyPinPad.exe"			,				//
											// SPL
	"ComMonitor4TSPL.exe"	,				//
	"TSPL.exe"				,				//
	"NXTTU4SPL.exe"			,				//
											// CAMERA
	"HsCamera.exe"			,				//
	"Nx2Camera.exe"			,				//
	"HSCAMSp.exe"			,				//
	"HcCamera.exe"			,				//
											// OTHER
	"ComMonitor.exe"		,				//
	"Ups.exe"				,				//
	"RPLVFDCTRL.exe"		,				//
	"3270w32.exe"			,				//
	
											// 별도로 확인할 사항
	"TstCon32.exe"			,				// Test Containner
	"MwiTM.exe"				,				// MWITM
	"AgentService.exe"		,				// Cup(SH) : 2006.04.19
	"Agent.exe"				,				// DC NMS  : 2006.07.03
	"Excute.exe"			,				// Cup(DL) : 2006.11.02
	"Monitor2.exe"			,				// AVI	   : 2006.11.02
	"AviPlayer.exe"			,				// AVI     : 2008.04.10
	"ServiceBotton.exe"		,				// OpSwitch: 2008.08.18
	""						,				// 
	""						,				// 
	""						,				// 


	"t1clear.exe"							// Myself
};


#endif
