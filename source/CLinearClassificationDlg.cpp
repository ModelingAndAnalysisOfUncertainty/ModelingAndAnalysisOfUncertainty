// CLinearClassificationDlg.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CLinearClassificationDlg.h"


// CLinearClassificationDlg dialog

IMPLEMENT_DYNAMIC(CLinearClassificationDlg, CDialog)

CLinearClassificationDlg::CLinearClassificationDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_LINEAR_CLASSIFICATION, pParent)
	, cross_validation()
{

}

CLinearClassificationDlg::~CLinearClassificationDlg()
{
}

void CLinearClassificationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_RADIO1, cross_validation);
	DDX_Control(pDX, IDC_RADIO2, training_validation);
	DDX_Control(pDX, IDC_RADIO3, five_fold);
	DDX_Control(pDX, IDC_RADIO4, ten_fold);
	DDX_Control(pDX, IDC_RADIO5, LOO);
	DDX_Control(pDX, IDCANCEL, box_cross_validation);
}


BEGIN_MESSAGE_MAP(CLinearClassificationDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &CLinearClassificationDlg::OnBnClickedRadioCrossValidation)
	ON_BN_CLICKED(IDC_RADIO2, &CLinearClassificationDlg::OnBnClickedRadioTrainingValidation)
END_MESSAGE_MAP()


// CLinearClassificationDlg message handlers


void CLinearClassificationDlg::OnBnClickedRadioCrossValidation()
{
	five_fold.ShowWindow(SW_SHOW);
	ten_fold.ShowWindow(SW_SHOW);
	LOO.ShowWindow(SW_SHOW);
	box_cross_validation.ShowWindow(SW_SHOW);
}


void CLinearClassificationDlg::OnBnClickedRadioTrainingValidation()
{
	five_fold.ShowWindow(SW_HIDE);
	ten_fold.ShowWindow(SW_HIDE);
	LOO.ShowWindow(SW_HIDE);
	box_cross_validation.ShowWindow(SW_HIDE);
}
