/////////////////////////////////////////////////////////////////////////////
// WinAtmCtl.cpp : Implementation of the CWinAtmCtrl ActiveX Control class.
/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"

#include "..\H\Common\CmnLibIn.h"
#include "..\H\Common\ConstDef.h"
#include "..\H\Common\ClassInclude.h"

#include "..\H\Dll\DevDefine.h"
#include "..\H\Dll\DevCmn.h"
#include "..\H\Tran\TranCmnDefine.h"
#include "..\H\Tran\TranCmn.h"

#include "WinAtm.h"
#include "WinAtmCtl.h"
#include "WinAtmPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CWinAtmCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// AXEvent map : AXINAX

BEGIN_EVENTSINK_MAP(CWinAtmCtrl, COleControl)
    //{{AFX_EVENTSINK_MAP(CWinAtmCtrl)
	ON_EVENT(CWinAtmCtrl, IDC_NHMWI, 3 /* OnDisplayData */, OnDisplayData, VTS_BSTR VTS_BSTR)
	ON_EVENT(CWinAtmCtrl, IDC_NHMWI, 4 /* OnDisplayPinpadData */, OnDisplayPinpadData, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CWinAtmCtrl, COleControl)
	//{{AFX_MSG_MAP(CWinAtmCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CWinAtmCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CWinAtmCtrl)
	DISP_FUNCTION(CWinAtmCtrl, "AtmStart", AtmStart, VT_I4, VTS_NONE)
	DISP_FUNCTION(CWinAtmCtrl, "TraceDump", TraceDump, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CWinAtmCtrl, "SetScreenData", SetScreenData, VT_I4, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CWinAtmCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CWinAtmCtrl, COleControl)
	//{{AFX_EVENT_MAP(CWinAtmCtrl)
	EVENT_CUSTOM("OnDisplayData", FireOnDisplayData, VTS_BSTR  VTS_BSTR)
	EVENT_CUSTOM("OnDisplayPinpadData", FireOnDisplayPinpadData, VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CWinAtmCtrl, 1)
	PROPPAGEID(CWinAtmPropPage::guid)
END_PROPPAGEIDS(CWinAtmCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CWinAtmCtrl, "WINATM.WinAtmCtrl.1",
	0xbba80ecc, 0xeef0, 0x4e7c, 0x8b, 0x5e, 0xfd, 0xee, 0xba, 0x56, 0x8e, 0x1d)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CWinAtmCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DWinAtm =
		{ 0xef0057b, 0x458a, 0x4d2f, { 0xa5, 0x6c, 0x51, 0x67, 0xee, 0x33, 0x90, 0x16 } };
const IID BASED_CODE IID_DWinAtmEvents =
		{ 0xccc22c52, 0x2027, 0x460d, { 0xb0, 0x79, 0x39, 0x32, 0x94, 0xc0, 0x48, 0x3a } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwWinAtmOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CWinAtmCtrl, IDS_WINATM, _dwWinAtmOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::CWinAtmCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CWinAtmCtrl
// À¥ÆäÀÌÁö¿¡¼­ »ç¿ë½Ã º¸¾È¹®Á¦ ÇØ°á¿ë
#include "Helpers.h"
#include "ObjSafe.h"

BOOL CWinAtmCtrl::CWinAtmCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.
	
	if (bRegister)
	{
		int retval = AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_WINATM,
			IDB_WINATM,
			afxRegApartmentThreading,
			_dwWinAtmOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);

		
		HRESULT hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls that are safely scriptable");
		
		
		if (SUCCEEDED(hr))
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForScripting);

		
		hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data");

		if (SUCCEEDED(hr))
			RegisterCLSIDInCategory(m_clsid, CATID_SafeForInitializing);

		return retval;
	}
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::CWinAtmCtrl - Constructor

CWinAtmCtrl::CWinAtmCtrl()
{
	InitializeIIDs(&IID_DWinAtm, &IID_DWinAtmEvents);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "CWinAtmCtrl()");

	m_pOwner	= NULL;

	m_pDevCmn	= NULL;
	m_pTranCmn	= NULL;

	m_pProfile	= NULL;
	m_pNetWork	= NULL;
	m_pComPort	= NULL;											// 2005.06.15
	m_pLanPort	= NULL;											// 2005.12.23
	m_pNmsWork	= NULL;											// 2006.07.03
	m_pEtcWork	= NULL;											// 2006.10.30
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::~CWinAtmCtrl - Destructor

CWinAtmCtrl::~CWinAtmCtrl()
{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "~CWinAtmCtrl()");
}

/////////////////////////////////////////////////////////////////////////////
// Set Owner
void CWinAtmCtrl::SetOwner(CWinAtmCtrl *pOwner)
{
#include "..\H\Common\ClassSetOwner.h"

	return;
}

/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::OnDraw - Drawing function

void CWinAtmCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::DoPropExchange - Persistence support

void CWinAtmCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::OnResetState - Reset control to default state

void CWinAtmCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl::AboutBox - Display an "About" box to the user

void CWinAtmCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_WINATM);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CWinAtmCtrl message handlers

int CWinAtmCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnCreate()");

	// Create Middleware Interface Object for control device
	Mwi.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHMWI);

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnCreate():return");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
// DESCRIPT  : Request event for display(To Flash)
// PARAMETER : Data Name, Data
// RETURN    : 
// REMARK    : This function is event handler
//             Receive display screen event from MWI and do some process
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
{
	FireOnDisplayData(szDataName, szDataValue);
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnDisplayPinpadData(LPCTSTR szDataValue)
// DESCRIPT  : Request event for display(To Flash)
// PARAMETER : Data
// RETURN    : 
// REMARK    : This function is event handler
//             Receive display screen event from MWI and do some process
//			   2004.04.07
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnDisplayPinpadData(LPCTSTR szDataValue)
{
	CString	strDataValue(szDataValue);
	BYTE	VirtualKeyCode = 0;

if ((strDataValue >= "0")	&&									// 0-9
	(strDataValue <= "9")	&&
	(strDataValue.GetLength() == 1))							// 2004.08.19
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnDisplayPinpadData(szDataValue[%s])", "*");
else
	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "OnDisplayPinpadData(szDataValue[%s])", szDataValue);

	switch (m_pDevCmn->AtmDefine.OpDevice)						// 2007.08.13
	{
		case NOT_SET:
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						65535 / 64 * 63,						// Focus x(Mouse)
						65535 / 64,								// Focus y(Mouse)
						NULL, 
						NULL);
			break;
		case OP_TOUCH:
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						(65535 / 64 * 63) + 65535,				// Focus x(Mouse)
						65535 / 64,								// Focus y(Mouse) : 2008.08.05
						NULL, 
						NULL);
			break;
		case OP_TSPL:
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						65535 / 64 * 63,						// Focus x(Mouse)
						65535 / 64,								// Focus y(Mouse)
						NULL, 
						NULL);
			break;
		case OP_TSPLCHN:
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						65535 / 64 * 63,						// Focus x(Mouse)
						65535 / 64,								// Focus y(Mouse)
						NULL, 
						NULL);
			break;
		case OP_HSPL:
			mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 
						65535 / 64 * 63,						// Focus x(Mouse)
						65535 / 64,								// Focus y(Mouse)
						NULL, 
						NULL);
			break;
	}

	if ((strDataValue >= "0")	&&								// 0-9
		(strDataValue <= "9")	&&
		(strDataValue.GetLength() == 1))
		memcpy(&VirtualKeyCode, strDataValue.GetBuffer(0), 1);
	else
	if (strDataValue == ".")									// .
		VirtualKeyCode = 'X';
	else
	if (strDataValue == "00")									// 00
		VirtualKeyCode = 'Z';
	else
	if (strDataValue == "CANCEL")								// Cancel
		VirtualKeyCode = VK_ESCAPE;
	else
	if (strDataValue == "CLEAR")								// Clear
		VirtualKeyCode = VK_DELETE;
	else
	if (strDataValue == "ENTER")								// Enter
		VirtualKeyCode = VK_RETURN;
	else
	if (strDataValue == "000")									// BackSpace : 2004.06.30
		VirtualKeyCode = VK_BACK;
	else
	if (strDataValue == "F1")									// F1
		VirtualKeyCode = 'Q';
	else
	if (strDataValue == "F2")									// F2
		VirtualKeyCode = 'W';
	else
	if (strDataValue == "F3")									// F3
		VirtualKeyCode = 'E';
	else
	if (strDataValue == "F4")									// F4
		VirtualKeyCode = 'R';
	else
	if (strDataValue == "F5")									// F5
		VirtualKeyCode = 'U';
	else
	if (strDataValue == "F6")									// F6
		VirtualKeyCode = 'I';
	else
	if (strDataValue == "F7")									// F7
		VirtualKeyCode = 'O';
	else
	if (strDataValue == "F8")									// F8
		VirtualKeyCode = 'P';
	else return;

	keybd_event(VirtualKeyCode, 0, KEYEVENTF_KEYUP, 0);			// 2004.08.23
	return;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long AtmStart()
// DESCRIPT  : Start ATM
// PARAMETER : Void
// RETURN    : NORMAL(0)
// REMARK    : 
/////////////////////////////////////////////////////////////////////////////
long CWinAtmCtrl::AtmStart() 
{
	// TODO: Add your dispatch handler code here

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart()");
/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn = new CDevCmn();									// Create Device
	m_pTranCmn = new CTranCmn();								// Create Transaction
	m_pDevCmn->SetOwner(&Mwi);									// Set Owner(Device)
	m_pTranCmn->SetOwner(this);									// Set Owner(Transaction)
	SetOwner(this);												// Set Owner(Winatm)

	m_pDevCmn->fnAPL_SetCashInfo(CSH_M10050_MAXOUT,				// 2007.06.06
				IniGetInt(_TIMEDEFINE_INI, _INIKEY_TIMEDEFINE, _INIKEY_TIMEDEFINE_WITHMAXAMOUNT,     CSH_M10050_MAXOUT));

	m_pDevCmn->ATMFirstStart = 1;											// First Start
	m_pDevCmn->bFirstStartRQK = TRUE;							// µÚÒ»´ÎÆô¶¯RQK   by zjd added by liuxl 20110622

/////////////////////////////////////////////////////////////////////////////
	if (m_pDevCmn->fnAPL_CheckRebootDevice())					// Check Reboot Device : 2006.04.19
		m_pDevCmn->fnAPL_LoadDevice();							// Load Device
	else
	{
		WindowsShutdownProcess(TRUE);							// Windows Reboot
		return 0;
	}

////////////////  for stream camera capture  by zjd ////////////////////////
	m_pDevCmn->iFaceCamNo = IniGetInt(_MONITOR_INI_DAT, "CAMERA", "FACE_NO", 0);
	m_pDevCmn->iHandCamNo = IniGetInt(_MONITOR_INI_DAT, "CAMERA", "HAND_NO", 1);
	m_pDevCmn->iAddCashCamNo = IniGetInt(_MONITOR_INI_DAT, "CAMERA", "ADDCASH_NO", 2);
	m_pDevCmn->iMonitorCS_Custom = -1;
	m_pDevCmn->iMonitorCS_AddCash = -1;
	hy_HCMonitor2_Register();
//////////////////////////////////////////////////////////////////////////////////////////////
//  ÎªÁËÔ¤·ÀÉãÏñÍ·¼ÓÔØ¿ÉÄÜÒýÆðµÄ³åÍ»£¬ÏÈ½øÐÐÔ¤ÅÄ²âÊÔ   by zjd
	m_pDevCmn->iMonitorCS_Custom = hy_HCMonitor2_CustomerServiceBegin();
	hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iFaceCamNo, 2,
		m_pDevCmn->iMonitorCS_Custom, "SYSTEM INIT ", " TEST FACE", "NO");
	hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iHandCamNo, 2,
		m_pDevCmn->iMonitorCS_Custom, "SYSTEM INIT ", " TEST HAND", "NO");
	m_pDevCmn->iMonitorCS_AddCash = hy_HCMonitor2_CustomerServiceBegin();
	hy_HCMonitor2_StartCapWithCapSeconds(m_pDevCmn->iAddCashCamNo, 2,
		m_pDevCmn->iMonitorCS_AddCash, "SYSTEM INIT ", " TEST ADD CASH", "NO");
//////////////////////////////////////////////////////////////////////////////////////////////

	if(IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA,_INIKEY_TRAN_DVRMODE,0) == 1)
	{
MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "fnAPP_DvrDataComm()INITEST");
		m_pTranCmn->fnAPP_DvrDataComm(CMDCODE_SETTIME,DVRCHAN_IDX0,0,"INITTEST");
		m_pTranCmn->fnAPP_DvrDataComm(CMDCODE_GETSTATUS,DVRCHAN_IDX0,0,"INITTEST");   
	}

	 
	SetTimer(TIMER_THREAD, TIMER_mSEC1000, NULL);				// Timer Start
	SetTimer(TIMER_VFD_THREAD, TIMER_mSEC1000, NULL);			// Vfd Start

	
	if(IniGetInt(_TRANSACTION_INI, _INIKEY_DVRCOMMDATA,_INIKEY_TRAN_DVRMODE,0) == 1) 
	{
		MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "SetTimer()DvrIdel");
//		SetTimer(TIMER_DVR_THREAD,TIMER_mMIN1,(TIMERPROC)m_pTranCmn->fnAPP_DvrIdel());   //Dvr Start
        SetTimer(TIMER_DVR_THREAD,TIMER_mMIN1,NULL);
	} 
	
	//agent
 	SetTimer(TIMER_AGENT_THREAD,TIMER_mMIN5,NULL);
    

/////////////////////////////////////////////////////////////////////////////
	m_pDevCmn->AtmDoing = TRUE;									// Atm Doing
	while (m_pDevCmn->AtmDoing)
	{
		Delay_Msg(50);
		AtmHostCtrl();											// ATM Host Control

		Delay_Msg(50);
		AtmModeCtrl();											// ATM Mode Control
	}

MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "AtmStart():return");
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long TraceDump(LPCTSTR szData)
// DESCRIPT  : External Trace Method
// PARAMETER : Trace Data
// RETURN    : NORMAL(0)
// REMARK    : 
/////////////////////////////////////////////////////////////////////////////
long CWinAtmCtrl::TraceDump(LPCTSTR szTraceData) 
{
	// TODO: Add your dispatch handler code here

	MsgDump(TRACE_CODE_MODE, "Log", __FILE__, __LINE__, "[HTML] %s", szTraceData);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue)
// DESCRIPT  : Transmit data from screen.(To Mwi)
// PARAMETER : Data Name, Data
// RETURN    : NORMAL(0)
// REMARK    : 
/////////////////////////////////////////////////////////////////////////////
long CWinAtmCtrl::SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue) 
{
	// TODO: Add your dispatch handler code here

	m_pDevCmn->fnSCR_SetScreenData(SetScreenDataName, SetScreenDataValue);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Active X Control ¹®Á¦ ÇØ°á
// ¹®Á¦Á¡ : Invisible on Runtime¼³Á¤½Ã ActiveXÀÇ Window°¡ ¸¸µé¾îÁöÁö ¾Ê¾Æ ÇÚµéÀÌ ¾øÀ½
// Áõ»ó   : ActiveX¸¦ ÀÌ¿ëÇÑ ¾î¶°ÇÑ ¼öÇàµµ Ã³¸®µÇÁö ¾ÊÀ½
// ÇØ°á   : À©µµ¸¦ Àç»ý¼ºÇÏ´Â °úÁ¤À» ¼öÇà(?)
// Âü°í   : MSDN Article No : Q195188
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnSetClientSite() 
{
    if (m_pClientSite)
        // It doesn't matter who the parent window is or what the size of
        // the window is because the control's window will be reparented
        // and resized correctly later when it's in-place activated.
        VERIFY (CreateControlWindow (::GetDesktopWindow(), CRect(0,0,0,0),
                                      CRect(0,0,0,0)));
    COleControl::OnSetClientSite();
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnTimer(UINT nIDEvent) 
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
/////////////////////////////////////////////////////////////////////////////
void CWinAtmCtrl::OnTimer(UINT nIDEvent) 
{
	int			*pTimer;
	int			i = 0;
// 	int         DvrErrorTime=0;
	
	KillTimer(nIDEvent);										// Stop Timer 
	switch(nIDEvent)
	{
		case TIMER_THREAD:
			pTimer = (int *)&m_pDevCmn->Timer;
			for (i = 0; i < (sizeof(m_pDevCmn->Timer) / sizeof(int)); i++)
			{
				if (pTimer[i])									// Until Zero
					pTimer[i]--;
			}
			
			SetTimer(TIMER_THREAD, TIMER_mSEC1000, NULL);		// Timer Start
			break;
			
		case TIMER_VFD_THREAD:
			m_pDevCmn->fnAPT_VfdControlThread();				// Vfd Control Thread
			
			SetTimer(TIMER_VFD_THREAD, TIMER_mSEC1000, NULL);	// Vfd Start
			break;
		case TIMER_DVR_THREAD:
            m_pTranCmn->fnAPP_DvrIdel();
			SetTimer(TIMER_DVR_THREAD,TIMER_mMIN1,NULL);
            break;

		case TIMER_AGENT_THREAD:
 			if(IniGetInt("D:\\INI\\TRANSACTION.INI", "TRANSACTION","AGENTOPEN", 0) == 1)
			{
				ShellExecute(NULL, "open", "C:\\ATMS\\Agent.exe", "1", NULL, SW_HIDE);
				SetTimer(TIMER_AGENT_THREAD,TIMER_mMIN5,NULL);
			}
            break;

		default:
			break;
	}
	
	COleControl::OnTimer(nIDEvent);
}
