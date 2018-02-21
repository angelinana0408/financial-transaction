// ShareMemPpg.cpp : Implementation of the CShareMemPropPage property page class.

#include "stdafx.h"
#include "ShareMem.h"
#include "ShareMemPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CShareMemPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CShareMemPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CShareMemPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CShareMemPropPage, "SHAREMEM.ShareMemPropPage.1",
	0xbd0536fa, 0xb1d5, 0x4d76, 0xae, 0x11, 0x3f, 0x96, 0xd, 0xcf, 0x94, 0x9f)


/////////////////////////////////////////////////////////////////////////////
// CShareMemPropPage::CShareMemPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CShareMemPropPage

BOOL CShareMemPropPage::CShareMemPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SHAREMEM_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemPropPage::CShareMemPropPage - Constructor

CShareMemPropPage::CShareMemPropPage() :
	COlePropertyPage(IDD, IDS_SHAREMEM_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CShareMemPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemPropPage::DoDataExchange - Moves data between page and properties

void CShareMemPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CShareMemPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CShareMemPropPage message handlers
