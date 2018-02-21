// RearApPpg.cpp : Implementation of the CRearApPropPage property page class.

#include "stdafx.h"
#include "RearAp.h"
#include "RearApPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CRearApPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CRearApPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CRearApPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CRearApPropPage, "REARAP.RearApPropPage.1",
	0xb3f01880, 0x178, 0x46fe, 0xa9, 0x92, 0xc7, 0x2d, 0xb, 0x95, 0xd8, 0x56)


/////////////////////////////////////////////////////////////////////////////
// CRearApPropPage::CRearApPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CRearApPropPage

BOOL CRearApPropPage::CRearApPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_REARAP_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CRearApPropPage::CRearApPropPage - Constructor

CRearApPropPage::CRearApPropPage() :
	COlePropertyPage(IDD, IDS_REARAP_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CRearApPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CRearApPropPage::DoDataExchange - Moves data between page and properties

void CRearApPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CRearApPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CRearApPropPage message handlers
