// CSelectNumberPCs.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CSelectNumberPCs.h"


// CSelectNumberPCs dialog

IMPLEMENT_DYNAMIC(CSelectNumberPCs, CDialog)

CSelectNumberPCs::CSelectNumberPCs(CWnd* pParent /*=nullptr*/)
	: CDialog(ID_SELECT_PCS, pParent), n_PCs(1)
{

}

CSelectNumberPCs::~CSelectNumberPCs(){
}

void CSelectNumberPCs::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GET_NUMBER_PCS, n_PCs);
}


BEGIN_MESSAGE_MAP(CSelectNumberPCs, CDialog)
END_MESSAGE_MAP()


// CSelectNumberPCs message handlers