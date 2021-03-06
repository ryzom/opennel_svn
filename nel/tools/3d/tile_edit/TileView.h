#if !defined(AFX_TILEVIEW_H__DEA1C044_F483_408A_84CE_5864950834D5__INCLUDED_)
#define AFX_TILEVIEW_H__DEA1C044_F483_408A_84CE_5864950834D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TileView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TileView window

class TileView : public CListCtrl
{
// Construction
public:
	TileView();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TileView)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~TileView();

	// Generated message map functions
protected:
	//{{AFX_MSG(TileView)
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TILEVIEW_H__DEA1C044_F483_408A_84CE_5864950834D5__INCLUDED_)
