// ShareMemCtl.cpp : Implementation of the CShareMemCtrl ActiveX Control class.

#include "stdafx.h"
#include "ShareMem.h"
#include "ShareMemCtl.h"
#include "ShareMemPpg.h"

#include "..\CmnLibIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CShareMemCtrl, COleControl)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CShareMemCtrl, COleControl)
	//{{AFX_MSG_MAP(CShareMemCtrl)
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CShareMemCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CShareMemCtrl)
	DISP_FUNCTION(CShareMemCtrl, "OpenShareData", OpenShareData, VT_I4, VTS_NONE)
	DISP_FUNCTION(CShareMemCtrl, "CloseShareData", CloseShareData, VT_I4, VTS_NONE)
	DISP_FUNCTION(CShareMemCtrl, "ClearShareData", ClearShareData, VT_I4, VTS_NONE)
	DISP_FUNCTION(CShareMemCtrl, "GetShareData", GetShareData, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CShareMemCtrl, "SetShareData", SetShareData, VT_I4, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CShareMemCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CShareMemCtrl, COleControl)
	//{{AFX_EVENT_MAP(CShareMemCtrl)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

BEGIN_PROPPAGEIDS(CShareMemCtrl, 1)
	PROPPAGEID(CShareMemPropPage::guid)
END_PROPPAGEIDS(CShareMemCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CShareMemCtrl, "SHAREMEM.ShareMemCtrl.1",
	0x52afe3a, 0xbd56, 0x473a, 0x87, 0x2e, 0x69, 0x56, 0xf, 0xd9, 0x6c, 0xfe)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CShareMemCtrl, _tlid, _wVerMajor, _wVerMinor)


/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DShareMem =
		{ 0x87eab939, 0xa5ac, 0x454d, { 0x88, 0xc9, 0xef, 0xab, 0x91, 0xac, 0x3f, 0x52 } };
const IID BASED_CODE IID_DShareMemEvents =
		{ 0x8ad0e5f5, 0x5c6b, 0x4e58, { 0xac, 0x1f, 0xd4, 0xea, 0xc6, 0xaa, 0x5a, 0x9b } };


/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwShareMemOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CShareMemCtrl, IDS_SHAREMEM, _dwShareMemOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl::CShareMemCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CShareMemCtrl
// 웹페이지에서 사용시 보안문제 해결용
#include "Helpers.h"
#include "ObjSafe.h"

BOOL CShareMemCtrl::CShareMemCtrlFactory::UpdateRegistry(BOOL bRegister)
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
			IDS_SHAREMEM,
			IDB_SHAREMEM,
			afxRegApartmentThreading,
			_dwShareMemOleMisc,
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
// CShareMemCtrl::CShareMemCtrl - Constructor

CShareMemCtrl::CShareMemCtrl()
{
	InitializeIIDs(&IID_DShareMem, &IID_DShareMemEvents);
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl::~CShareMemCtrl - Destructor

CShareMemCtrl::~CShareMemCtrl()
{
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl::OnDraw - Drawing function

void CShareMemCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl::DoPropExchange - Persistence support

void CShareMemCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

}


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl::OnResetState - Reset control to default state

void CShareMemCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl::AboutBox - Display an "About" box to the user

void CShareMemCtrl::AboutBox()
{
	CDialog dlgAbout(IDD_ABOUTBOX_SHAREMEM);
	dlgAbout.DoModal();
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemCtrl message handlers

long CShareMemCtrl::OpenShareData() 
{
	// TODO: Add your dispatch handler code here

	return ::OpenShareData();
}

long CShareMemCtrl::CloseShareData() 
{
	// TODO: Add your dispatch handler code here

	return ::CloseShareData();
}

long CShareMemCtrl::ClearShareData() 
{
	// TODO: Add your dispatch handler code here

	return ::ClearShareData();
}

BSTR CShareMemCtrl::GetShareData(LPCTSTR szGetDataName) 
{
	CString strResult("");
	// TODO: Add your dispatch handler code here

	strResult = ::GetShareData(szGetDataName);
	return strResult.AllocSysString();
}

long CShareMemCtrl::SetShareData(LPCTSTR szSetDataName, LPCTSTR szSetDataValue) 
{
	// TODO: Add your dispatch handler code here

	return ::SetShareData(szSetDataName, szSetDataValue);
}
