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
	this->m_ftFontName= "굴림체";

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

	// 내가 만든 브러쉬를 이용하여 화면을 출력하고 갱신하고자 한다면
	// 만든 브러쉬의 핸들을 리턴하여야 한다.
	// 이곳에서 리턴하는 값이 바로 해당 스태틱을 그리기 위해서 사욛되는 브러쉬이다.
	return this->m_brBrush;
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// 전경색을 설정한다. 만일 깜박임모드가 아니라면 문자색을 전경색으로 지정한다
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
// 배경색을 설정한다. 현재의 브러쉬를 삭제하고 새로운 배경색으로 브러쉬를 만든다
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
// 문자색을 설정한다. 
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
// 폰트의 문자폰트를 설정한다.
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetFontName(CString m_FontName)
{
	this->m_ftFontName.Format("%s", m_FontName.GetBuffer(m_FontName.GetLength()));
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// Bold를 설정한다
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetBold( UINT nBold )
{
	this->m_nBold = nBold;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// 이탤릭체여부를 설정한다
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetItalic( BOOL bItalic )
{
	this->m_bItalic = bItalic;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// 문자의 밑줄여부를 설정한다
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::SetUnderLine( BOOL bUnderLine )
{
	this->m_bUnderLine = bUnderLine;
	ResetFont();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// 폰트의 사이즈를 설정한다.
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
// 컨트럴의 사이즈에 맞게 문자출력
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
// 문자출력을 위한 폰트의 값을 다시 설정한다.
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::ResetFont()
{
	GetFont()->GetLogFont(&m_lf);				

	// 폰트의 사이즈를 설정한다.
	m_lf.lfHeight		= this->m_ftSize.cx;
	m_lf.lfWidth		= this->m_ftSize.cy;

	// 기울림체인가?
	m_lf.lfItalic		= this->m_bItalic;

	// 밑줄이 있던가?
	m_lf.lfUnderline	= this->m_bUnderLine;

	// 어떤 스타일이었지?
	m_lf.lfWeight		= this->m_nBold;

	// 어떤 글씨체였지?
	strcpy(m_lf.lfFaceName, m_ftFontName.GetBuffer(m_ftFontName.GetLength()));

	// 다시 출력할 폰트를 만들자구...
	this->m_ftFont.DeleteObject();
	this->m_ftFont.CreateFontIndirect(&m_lf);

	Invalidate();
}

///////////////////////////////////////////////////////////////////////////////
// ----------------------------------------------------------------------------
// 깜박임의 정도를 설정한다.(MiliSecond단위)
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
// 깜빡임을 위한 타이머를 죽인다
// ----------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////
VOID CTeoStatic::ClearTimer( )
{
	if (m_idTimer)	KillTimer(m_idTimer);
}

