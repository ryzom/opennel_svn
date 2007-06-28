// GetVal.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GetVal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GetVal dialog

//#define CARACTERES_INVALIDE "/\:*?"<>|"
char CaracteresInvalides[]={47,92,58,'*','?',34,60,62,124,0};

GetVal::GetVal(CWnd* pParent /*=NULL*/)
	: CDialog(GetVal::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetVal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void GetVal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetVal)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(GetVal, CDialog)
	//{{AFX_MSG_MAP(GetVal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetVal message handlers


void GetVal::OnOK() 
{
	// TODO: Add extra validation here
	NameOk=1;
	CEdit *EditStr=(CEdit*)GetDlgItem(IDC_EDIT_ADD_TERRITOIRE);
	CString rString;
	EditStr->GetWindowText( rString );
	int size = rString.GetLength();
	name = new char[size+1];
	strcpy (name, rString);
	/**((short*)name)=size;
	EditStr->GetLine(0,name,size);
	for (int i=0;i<size;i++)
		for (int c=0;c<strlen(CaracteresInvalides);c++)
		{
			if (name[i]==CaracteresInvalides[c])
			{
				char Message[100];
				strcpy(Message,"Le message ne peut pas contenir les caracteres ");
				strcat(Message,CaracteresInvalides);
				MessageBox(Message,"Erreur de saisie");
				return;
			}
		}*/
	CDialog::OnOK();
}

void GetVal::OnCancel() 
{
	// TODO: Add extra cleanup here
	NameOk=0;	
	CDialog::OnCancel();
}

LRESULT GetVal::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message==WM_INITDIALOG)
	{
		CEdit *ed = (CEdit*)GetDlgItem(IDC_EDIT_ADD_TERRITOIRE);
		ed->SetFocus();
		ed->SetLimitText(100);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
