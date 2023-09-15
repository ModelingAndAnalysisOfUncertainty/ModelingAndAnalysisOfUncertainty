// CDisplayPCs.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CDisplayPCs.h"


// CDisplayPCs dialog

IMPLEMENT_DYNAMIC(CDisplayPCs, CDialog)

CDisplayPCs::CDisplayPCs(CWnd* pParent /*=nullptr*/) : CDialog(IDD_DISPLAY_PCS, pParent)
	, PC_1(1)
	, PC_2(2)
	, PC_3(3)
	, PC_4(4)
	, SelectOption(0)
{

}

CDisplayPCs::~CDisplayPCs(){
}

void CDisplayPCs::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PC_1, PC_1);
	DDV_MinMaxInt(pDX, PC_1, 1, 999);
	DDX_Text(pDX, IDC_EDIT_PC_2, PC_2);
	DDV_MinMaxInt(pDX, PC_2, 1, 999);
	DDX_Text(pDX, IDC_EDIT_PC_3, PC_3);
	DDV_MinMaxInt(pDX, PC_3, 1, 999);
	DDX_Text(pDX, IDC_EDIT_PC_4, PC_4);
	DDV_MinMaxInt(pDX, PC_4, 1, 999);
	DDX_Radio(pDX, IDC_RADIO_PCA_DISPLAY_SELECTION, SelectOption);
}


BEGIN_MESSAGE_MAP(CDisplayPCs, CDialog)
END_MESSAGE_MAP()


// CDisplayPCs message handlers
