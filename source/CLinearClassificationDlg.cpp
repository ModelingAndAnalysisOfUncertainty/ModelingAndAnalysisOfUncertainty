// CLinearClassificationDlg.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CLinearClassificationDlg.h"
#include "Modeling and Analysis of UncertaintyDoc.h"

// CLinearClassificationDlg dialog

IMPLEMENT_DYNAMIC(CLinearClassificationDlg, CDialog)

CLinearClassificationDlg::CLinearClassificationDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_LINEAR_CLASSIFICATION, pParent)
	, five_fold_value(0)
	, ten_fold_value(1)
	, LOO_value(1)
	
	,training_validation(1)
	,cross_validation(0)
{
	
}

CLinearClassificationDlg::~CLinearClassificationDlg()
{
}

void CLinearClassificationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control (pDX, IDC_RADIO3, five_fold);
	DDX_Radio(pDX, IDC_RADIO3, five_fold_value);
	DDX_Control(pDX, IDC_RADIO4, ten_fold);
	DDX_Control(pDX, IDC_RADIO5, LOO);
	DDX_Radio(pDX, IDC_CROSS, cross_validation);
	DDX_Control(pDX, IDC_Cross_Validation, group_cross_validation);
}


BEGIN_MESSAGE_MAP(CLinearClassificationDlg, CDialog)
	
	ON_BN_CLICKED(IDC_CROSS, &CLinearClassificationDlg::OnBnClickedCross)
	ON_BN_CLICKED(IDC_TRAINING, &CLinearClassificationDlg::OnBnClickedTraining)
	ON_BN_CLICKED(IDOK, &CLinearClassificationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO4, &CLinearClassificationDlg::OnBnClickedTenFold)
END_MESSAGE_MAP()


// CLinearClassificationDlg message handlers



void CLinearClassificationDlg::OnBnClickedCross()
{
	CWnd* pGroupBox = GetDlgItem(IDC_Cross_Validation);
	pGroupBox->ShowWindow(SW_SHOW);
	five_fold.ShowWindow(SW_SHOW);
	ten_fold.ShowWindow(SW_SHOW);
	LOO.ShowWindow(SW_SHOW);
}


void CLinearClassificationDlg::OnBnClickedTraining()
{
	five_fold.ShowWindow(SW_HIDE);
	ten_fold.ShowWindow(SW_HIDE);
	LOO.ShowWindow(SW_HIDE);
	CWnd* pGroupBox = GetDlgItem(IDC_Cross_Validation);
	pGroupBox->ShowWindow(SW_HIDE);
}


void CLinearClassificationDlg::OnBnClickedOk()
{


	CDialog::OnOK();
}


void CLinearClassificationDlg::OnBnClickedTenFold()
{
	ten_fold_value = 0;
}
