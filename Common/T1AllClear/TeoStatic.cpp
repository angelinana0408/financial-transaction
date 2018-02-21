// TeoStatic.cpp : implementation file
//

#include "stdafx.h"
#include "TeoStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTeoStatic

CTeoStatic::CTeoStatic()
{
	this->m_clFgColor = RGB(0,0,0);
	this->m_clBlColor = RGB(0,0,255);
	this->m_clTextColor = RGB(0,0,0);
	
	this->m_bBlink    = FALSE;

	this->m_nInterval = 0;
	this->m_idTimer   = 0;

	this->m_clBgColor = GetSysColor(COLOR_BTNFACE);
	
	this->m_nBold	  = FW_NORMAL;
	this->m_bItalic   = FALSE;
	this->m_bUnderLine= FALSE;

	this->m_ftSize    = CSize(12, 6);
	this->m_ftFontName= "����ü";

	this->m_brBrush.CreateSolidBrush(m_clBgColor);
	this->m_ftFont.CreateFont(
							   m_ftSize.cx,					// nHeight
							   m_ftSize.cy,					// nWidth
							   0,							// nEscapement
							   0,							// nOrientation
							   m_nBold,						// nWeight
							   m_bItalic,					// bItalic
							   m_bUnderLine,				// bUnderline
							   0,							// cStrikeOut
							   ANSI_CHARSET,				// nCharSet
							   OUT_DEFAULT_PRECIS,			// nOutPrecision
							   CLIP_DEFAULT_PRECIS,			// nClipPrecision
							   DEFAULT_QUALITY,				// nQuality
							   DEFAULT_PITCH | FF_SWISS,	// nPitchAndFamily
							   m_ftFontName.GetBuffer(m_ftFontName.GetLength()));// lpszFacename

	this->m_bFitText	= FALSE;

}

CTeoStatic::~CTeoStatic()
{
}


BEGIN_MESSAGE_MAP(CTeoStatic, CStatic)
	//{{AFX_MSG_MAP(CTeoStatic)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTeoStatic message handlers

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Timer
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void CTeoStatic::OnTimer(UINT nIDEvent) 
{
	CStatic::OnTimer(nIDEvent);

	if ( nIDEvent == m_idTimer )
	{
		m_bBlink = ~m_bBlink;

		if ( m_bBlink )	m_clTextColor = m_clBlColor;
		else			m_clTextColor = m_clFgColor;

    Invalidate(FALSE);
  }

}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Control Create
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
int CTeoStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if ( m_nInterval > 0 )
	{
		m_idTimer = SetTimer(GetWindowLong(GetSafeHwnd(),
							GWL_ID),
							m_nInterval,
							NULL);
	}
	
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// WM_DESTROY
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
void CTeoStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
	if ( m_idTimer )	KillTimer(m_idTimer);	
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// REFLECT
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
HBRUSH CTeoStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	pDC->SelectObject(&m_ftFont);
	pDC->SetTextColor(m_clTextColor);
	pDC->SetBkColor(m_clBgColor);
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	//return NULL;

	// ���� ���� �귯���� �̿��Ͽ� ȭ���� ����ϰ� �����ϰ��� �Ѵٸ�
	// ���� �귯���� �ڵ��� �����Ͽ��� �Ѵ�.
	// �̰����� �����ϴ� ���� �ٷ� �ش� ����ƽ�� �׸��� ���ؼ� ����Ǵ� �귯���̴�.
	return this->m_brBrush;
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ������� �����Ѵ�. ���� �����Ӹ�尡 �ƴ϶�� ���ڻ��� ��������� �����Ѵ�
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetForeColor(const COLORREF nColor)
{
	this->m_clFgColor = nColor;

	if (!this->m_bBlink)
	{
		this->m_clTextColor = nColor;
		Invalidate(FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ������ �����Ѵ�. ������ �귯���� �����ϰ� ���ο� �������� �귯���� �����
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetBackColor(const COLORREF nColor)
{
	this->m_clBgColor = nColor;

	this->m_brBrush.DeleteObject();
	this->m_brBrush.CreateSolidBrush(nColor);
	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ���ڻ��� �����Ѵ�. 
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetBlinkColor(const COLORREF nColor)
{
	this->m_clBlColor = nColor;

	if (!this->m_bBlink)
	{
		this->m_clTextColor = nColor;
		Invalidate(FALSE);
	}
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ��Ʈ�� ������Ʈ�� �����Ѵ�.
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetFontName(CString m_FontName)
{
	this->m_ftFontName.Format("%s", m_FontName.GetBuffer(m_FontName.GetLength()));
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Bold�� �����Ѵ�
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetBold( UINT nBold )
{
	this->m_nBold = nBold;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ���Ÿ�ü���θ� �����Ѵ�
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetItalic( BOOL bItalic )
{
	this->m_bItalic = bItalic;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ������ ���ٿ��θ� �����Ѵ�
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetUnderLine( BOOL bUnderLine )
{
	this->m_bUnderLine = bUnderLine;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ��Ʈ�� ����� �����Ѵ�.
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetFontSize( CSize m_Size )
{
	this->m_ftSize.cx = m_Size.cx;
	this->m_ftSize.cy = m_Size.cy;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ��Ʈ���� ����� �°� �������
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetFitText( BOOL bFit )
{
	CRect rcRect;
	CString strTmp;
	if (bFit)
	{
		this->GetWindowRect(&rcRect);
		this->GetWindowText(strTmp);
		SetFontSize(CSize(rcRect.Height(), rcRect.Width()/strTmp.GetLength()));
	}
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// ��������� ���� ��Ʈ�� ���� �ٽ� �����Ѵ�.
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::ResetFont()
{
	GetFont()->GetLogFont(&m_lf);				

	// ��Ʈ�� ����� �����Ѵ�.
	m_lf.lfHeight		= this->m_ftSize.cx;
	m_lf.lfWidth		= this->m_ftSize.cy;

	// ��︲ü�ΰ�?
	m_lf.lfItalic		= this->m_bItalic;

	// ������ �ִ���?
	m_lf.lfUnderline	= this->m_bUnderLine;

	// � ��Ÿ���̾���?
	m_lf.lfWeight		= this->m_nBold;

	// � �۾�ü����?
	strcpy(m_lf.lfFaceName, m_ftFontName.GetBuffer(m_ftFontName.GetLength()));

	// �ٽ� ����� ��Ʈ�� �����ڱ�...
	this->m_ftFont.DeleteObject();
	this->m_ftFont.CreateFontIndirect(&m_lf);

	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// �������� ������ �����Ѵ�.(MiliSecond����)
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetInterval(const UINT nInterval)
{
	if ( nInterval > 0 )
	{
		m_nInterval = nInterval;
		m_idTimer = SetTimer(GetWindowLong(GetSafeHwnd(),GWL_ID),
                          m_nInterval,
                          NULL);
	}
	else
	{
		ClearTimer();
	}
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// �������� ���� Ÿ�̸Ӹ� ���δ�
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::ClearTimer( )
{
	if (m_idTimer)	KillTimer(m_idTimer);
}

