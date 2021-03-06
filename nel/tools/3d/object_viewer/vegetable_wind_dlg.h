#if !defined(AFX_VEGETABLE_WIND_DLG_H__2A8C3E24_6763_4B2C_AFD4_DD6F74D3871E__INCLUDED_)
#define AFX_VEGETABLE_WIND_DLG_H__2A8C3E24_6763_4B2C_AFD4_DD6F74D3871E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// vegetable_wind_dlg.h : header file
//


class	CObjectViewer;


/////////////////////////////////////////////////////////////////////////////
// CVegetableWindDlg dialog

class CVegetableWindDlg : public CDialog
{
// Construction
public:
	CVegetableWindDlg(CObjectViewer *objViewer, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CVegetableWindDlg)
	enum { IDD = IDD_VEGETABLE_WIND };
	CStatic	StaticPower;
	CStatic	StaticFrequency;
	CStatic	StaticBendStart;
	CSliderCtrl	SliderPower;
	CSliderCtrl	SliderFrequency;
	CSliderCtrl	SliderBendStart;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVegetableWindDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CVegetableWindDlg)
	afx_msg void OnReleasedcaptureSliderVegetableWindBendstart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderVegetableWindFrequency(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnReleasedcaptureSliderVegetableWindPower(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CObjectViewer	*_ObjViewer;

	// update sliders and static according to objViewer wind setup
	void	updateView();


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VEGETABLE_WIND_DLG_H__2A8C3E24_6763_4B2C_AFD4_DD6F74D3871E__INCLUDED_)
