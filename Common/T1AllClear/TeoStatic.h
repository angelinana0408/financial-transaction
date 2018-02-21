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
	COLORREF	m_clBgColor;						// Static�� Background(���)����
	COLORREF	m_clFgColor;						// Static�� Foreground(����)����
	COLORREF	m_clBlColor;						// Static�� ������ ����
	CBrush		m_brBrush;							// ����� Brush
	CFont		m_ftFont;							// ����� Font
	LOGFONT		m_lf;

	CSize		m_ftSize;							// ����� Font�� Size(Width/Height)
	CString		m_ftFontName;						// ����� Font�� �̸�
	UINT		m_nBold;							// ����� Font�� Weight
	BOOL		m_bItalic;							// ����� Font�� Italic����
	BOOL		m_bUnderLine;						// ����� Font�� UnderLine����

	COLORREF	m_clTextColor;						// ���� ������ ������ ����
	BOOL		m_bBlink;							// ���� ������ ��������
	UINT		m_nInterval;						// �����ӽ� �ð�����
	UINT		m_idTimer;							// ������ ������ Ÿ�̸��̸�

	BOOL		m_bFitText;							// ����� ���ڿ��� ��Ʈ�ѿ� ������ ũ�⸦ �������� ����

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
