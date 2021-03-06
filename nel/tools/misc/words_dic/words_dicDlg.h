// words_dicDlg.h : header file
//

#if !defined(AFX_WORDS_DICDLG_H__B0C9AB06_2310_4A82_9C46_A7667C7D3407__INCLUDED_)
#define AFX_WORDS_DICDLG_H__B0C9AB06_2310_4A82_9C46_A7667C7D3407__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CWords_dicDlg dialog

class CWords_dicDlg : public CDialog
{
// Construction
public:
	CWords_dicDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CWords_dicDlg)
	enum { IDD = IDD_WORDS_DIC_DIALOG };
	CEdit	m_LookUp;
	CListBox	m_Results;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWords_dicDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	void	lookUp( const CString& inputStr );
	void	clear();

	// Generated message map functions
	//{{AFX_MSG(CWords_dicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnChangeLookUp();
	afx_msg void OnBtnFind();
	afx_msg void OnBtnClear();
	afx_msg void OnDblclkResultList();
	afx_msg void OnSelchangeResultList();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnFileList();
	afx_msg void OnShowAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORDS_DICDLG_H__B0C9AB06_2310_4A82_9C46_A7667C7D3407__INCLUDED_)
