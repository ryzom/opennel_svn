// choose_sun_color_dlg.cpp : implementation file
//

#include "std_afx.h"
#include "object_viewer.h"
#include "choose_sun_color_dlg.h"
#include "color_edit.h"


/////////////////////////////////////////////////////////////////////////////
// CChooseSunColorDlg dialog

//*************************************************************************************************
CChooseSunColorDlg::CChooseSunColorDlg(NL3D::CScene *scene, CWnd* pParent /*=NULL*/)
	: CDialog(CChooseSunColorDlg::IDD, pParent),
	  _DiffuseColorEdit(NULL),
	  _AmbientColorEdit(NULL),
	  _SpecularColorEdit(NULL)
{
	//{{AFX_DATA_INIT(CChooseSunColorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	nlassert(scene);
	_DiffuseColorWrapper.S = scene;
	_DiffuseColorEdit = new CColorEdit(this);
	_DiffuseColorEdit->setWrapper(&_DiffuseColorWrapper);
	//
	_AmbientColorWrapper.S = scene;
	_AmbientColorEdit = new CColorEdit(this);
	_AmbientColorEdit->setWrapper(&_AmbientColorWrapper);
	//
	_SpecularColorWrapper.S = scene;
	_SpecularColorEdit = new CColorEdit(this);
	_SpecularColorEdit->setWrapper(&_SpecularColorWrapper);
}

//*************************************************************************************************
CChooseSunColorDlg::~CChooseSunColorDlg()
{	
	if (_DiffuseColorEdit)
	{
		_DiffuseColorEdit->DestroyWindow();
		delete _DiffuseColorEdit;
	}
	if (_SpecularColorEdit)
	{
		_SpecularColorEdit->DestroyWindow();
		delete _SpecularColorEdit;
	}
	if (_AmbientColorEdit)
	{
		_AmbientColorEdit->DestroyWindow();
		delete _AmbientColorEdit;
	}
}

//*************************************************************************************************
void CChooseSunColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChooseSunColorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}
BEGIN_MESSAGE_MAP(CChooseSunColorDlg, CDialog)
	//{{AFX_MSG_MAP(CChooseSunColorDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChooseSunColorDlg message handlers

//*************************************************************************************************
BOOL CChooseSunColorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	RECT r;
	//
	GetDlgItem(IDC_DIFFUSE)->GetWindowRect(&r);
	ScreenToClient(&r);
	_DiffuseColorEdit->init(r.left, r.top, this);		
	//
	GetDlgItem(IDC_AMBIENT)->GetWindowRect(&r);
	ScreenToClient(&r);
	_AmbientColorEdit->init(r.left, r.top, this);
	//
	GetDlgItem(IDC_SPECULAR)->GetWindowRect(&r);
	ScreenToClient(&r);
	_SpecularColorEdit->init(r.left, r.top, this);
	//
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//*************************************************************************************************
void CChooseSunColorDlg::OnDestroy() 
{		
	setRegisterWindowState (this, REGKEY_CHOOSE_SUN_COLOR_DLG);		
	CDialog::OnDestroy();		// TODO: Add your message handler code here	
}
