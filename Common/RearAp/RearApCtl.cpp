// RearApCtl.cpp : Implementation of the CRearApCtrl ActiveX Control class.

#include "stdafx.h"

#include "CmnLibIn.h"

#include "RearAp.h"
#include "RearApCtl.h"
#include "RearApPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRearApCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// AXEvent map : AXINAX

BEGIN_EVENTSINK_MAP(CRearApCtrl, COleControl)
    //{{AFX_EVENTSINK_MAP(CRearApCtrl)
	ON_EVENT(CRearApCtrl, IDC_NHMWIOP, 3 /* OnDisplayData */, OnDisplayData, VTS_BSTR VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRearApCtrl, COleControl)
	//{{AFX_MSG_MAP(CRearApCtrl)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CRearApCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CRearApCtrl)
	DISP_FUNCTION(CRearApCtrl, "RearStart", RearStart, VT_I4, VTS_NONE)
	DISP_FUNCTION(CRearApCtrl, "SetScreenData", SetScreenData, VT_I4, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CRearApCtrl, "TraceDump", TraceDump, VT_I4, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CRearApCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CRearApCtrl, COleControl)
	//{{AFX_EVENT_MAP(CRearApCtrl)
	EVENT_CUSTOM("OnDisplayData", FireOnDisplayData, VTS_BSTR  VTS_BSTR)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CRearApCtrl, 1)
	PROPPAGEID(CRearApPropPage::guid)
END_PROPPAGEIDS(CRearApCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRearApCtrl, "RearAP.RearApCtrl.1",
	0x231ac0d1, 0x5b4e, 0x4359, 0x9d, 0x8c, 0x3e, 0xd, 0xa0, 0x6b, 0xb6, 0x9)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CRearApCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DRearAp =
		{ 0x4205d536, 0xab67, 0x45d3, { 0xb6, 0xe7, 0x56, 0x77, 0x81, 0x5, 0x85, 0xb5 } };
const IID BASED_CODE IID_DRearApEvents =
		{ 0x40ac1ab7, 0x3a32, 0x42b0, { 0xb4, 0xe6, 0xcf, 0x33, 0x45, 0xc7, 0x34, 0xaf } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwRearApOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CRearApCtrl, IDS_REARAP, _dwRearApOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl::CRearApCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CRearApCtrl
// 웹페이지에서 사용시 보안문제 해결용
#include "Helpers.h"
#include "ObjSafe.h"

BOOL CRearApCtrl::CRearApCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_REARAP,
			IDB_REARAP,
			afxRegApartmentThreading,
			_dwRearApOleMisc,
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
// CRearApCtrl::CRearApCtrl - Constructor

CRearApCtrl::CRearApCtrl()
{
	InitializeIIDs(&IID_DRearAp, &IID_DRearApEvents);

	// TODO: Initialize your control's instance data here.
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "CRearApCtrl()");
}

/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl::~CRearApCtrl - Destructor

CRearApCtrl::~CRearApCtrl()
{
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "~CRearApCtrl()");
}


/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl::OnDraw - Drawing function

void CRearApCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl::DoPropExchange - Persistence support

void CRearApCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: Call PX_ functions for each persistent custom property.

}

/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl::OnResetState - Reset control to default state

void CRearApCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl::AboutBox - Display an "About" box to the user

void CRearApCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_REARAP);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CRearApCtrl message handlers

int CRearApCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "OnCreate()");

	// Create Middleware Interface Object for control device
	MwiOp.Create(NULL, 0, CRect(0,0,0,0), this, IDC_NHMWIOP);

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "OnCreate()");
	
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
void CRearApCtrl::OnDisplayData(LPCTSTR szDataName, LPCTSTR szDataValue)
{
	FireOnDisplayData(szDataName, szDataValue);
}

/////////////////////////////////////////////////////////////////////////////
// PROTOTYPE : void OnTimer(UINT nIDEvent) 
// DESCRIPT  : 
// PARAMETER : 
// RETURN    : 
/////////////////////////////////////////////////////////////////////////////
void CRearApCtrl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	COleControl::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long RearStart()
// DESCRIPT  : Start Rear OP Program
//////////////////////////////////////////////////////////////////////////
long CRearApCtrl::RearStart()
{
	// TODO: Add your dispatch handler code here
MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "RearStart()");

	m_pMwiOp	= &MwiOp;

	ClerkModeCtrl();											// Clerk Mode Control

MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "RearStart():return");
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue) 
// DESCRIPT  : Processing Screen Data From Flash (Translate to MwiScr)
//////////////////////////////////////////////////////////////////////////
long CRearApCtrl::SetScreenData(LPCTSTR SetScreenDataName, LPCTSTR SetScreenDataValue) 
{
	// TODO: Add your dispatch handler code here
	fnSCR_SetScreenData(SetScreenDataName, SetScreenDataValue);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// PROTOTYPE : long TraceDump(LPCTSTR szTraceData) 
// DESCRIPT  : Trace When Display Screen
//////////////////////////////////////////////////////////////////////////
long CRearApCtrl::TraceDump(LPCTSTR szTraceData) 
{
	// TODO: Add your dispatch handler code here

//	MsgDump(TRACE_CODE_MODE, "Rear", __FILE__, __LINE__, "[HTML] %s", szTraceData);
																// 2005.03.30
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// Active X Control 문제 해결
// 문제점 : Invisible on Runtime설정시 ActiveX의 Window가 만들어지지 않아 핸들이 없음
// 증상   : ActiveX를 이용한 어떠한 수행도 처리되지 않음
// 해결   : 윈도를 재생성하는 과정을 수행(?)
// 참고   : MSDN Article No : Q195188
/////////////////////////////////////////////////////////////////////////////
void CRearApCtrl::OnSetClientSite() 
{
	// TODO: Add your specialized code here and/or call the base class
    if (m_pClientSite)
        // It doesn't matter who the parent window is or what the size of
        // the window is because the control's window will be reparented
        // and resized correctly later when it's in-place activated.
        VERIFY (CreateControlWindow (::GetDesktopWindow(), CRect(0,0,0,0),
                                      CRect(0,0,0,0)));
    COleControl::OnSetClientSite();
	
}
