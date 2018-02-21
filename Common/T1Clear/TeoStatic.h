#if !defined(AFX_TEOSTATIC_H__45BEDD15_BBA8_4C14_8ED4_28A74E421EC4__INCLUDED_)
#define AFX_TEOSTATIC_H__45BEDD15_BBA8_4C14_8ED4_28A74E421EC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeoStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTeoStatic window

class CTeoStatic : public CStatic
{
// Construction
public:
	CTeoStatic();

// Attributes
public:

// local Attributes
private:
	COLORREF	m_clBgColor;						// Static의 Background(배경)색상
	COLORREF	m_clFgColor;						// Static의 Foreground(전경)색상
	COLORREF	m_clBlColor;						// Static의 깜빡임 색상
	CBrush		m_brBrush;							// 사용할 Brush
	CFont		m_ftFont;							// 사용할 Font
	LOGFONT		m_lf;

	CSize		m_ftSize;							// 사용할 Font의 Size(Width/Height)
	CString		m_ftFontName;						// 사용할 Font의 이름
	UINT		m_nBold;							// 사용할 Font의 Weight
	BOOL		m_bItalic;							// 사용할 Font의 Italic여부
	BOOL		m_bUnderLine;						// 사용할 Font의 UnderLine여부

	COLORREF	m_clTextColor;						// 현재 설정된 글자의 색상
	BOOL		m_bBlink;							// 현재 깜박임 설정여부
	UINT		m_nInterval;						// 깜박임시 시간간격
	UINT		m_idTimer;							// 깜박임 설정시 타이머이름

	BOOL		m_bFitText;							// 출력할 문자열을 컨트롤에 꽉차게 크기를 설정할지 여부

// Operations
public:
	UINT		GetForeColor(){ return m_clFgColor; }
	VOID		SetForeColor(const COLORREF nColor);
	UINT		GetBackColor(){ return m_clBgColor; }
	VOID		SetBackColor(const COLORREF nColor);
	UINT		GetBlinkColor(){ return m_clBlColor; }
	VOID		SetBlinkColor(const COLORREF nColor);
	
	CString		GetFontName() { return m_ftFontName; }
	VOID		SetFontName(CString m_FontName);

	UINT		GetBold() { return m_nBold; }
	VOID		SetBold( UINT nBold );
	BOOL		GetItalic() { return m_bItalic; }
	VOID		SetItalic( BOOL bItalic );
	BOOL		GetUnderLine() { return m_bUnderLine; }
	VOID		SetUnderLine( BOOL bUnderLine );
	BOOL		GetBlink() { return m_bBlink; }

	UINT		GetInterval(){ return m_nInterval; }
	VOID		SetInterval(const UINT nInterval);
	VOID		ClearTimer();	

	CSize		GetFontSize() { return m_ftSize; }
	VOID		SetFontSize( CSize m_Size );

	BOOL		GetFitText() { return m_bFitText; }
	VOID		SetFitText( BOOL bFit );

private:
	VOID		ResetFont();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeoStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTeoStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTeoStatic)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEOSTATIC_H__45BEDD15_BBA8_4C14_8ED4_28A74E421EC4__INCLUDED_)
