// CSettingsDescriptiveStatistics.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CSettingsDescriptiveStatistics.h"


// CSettingsDescriptiveStatistics dialog

IMPLEMENT_DYNAMIC(CSettingsDescriptiveStatistics, CDialogEx)

CSettingsDescriptiveStatistics::CSettingsDescriptiveStatistics(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTINGS_DESCRIPTIVE_STATISTICS, pParent)
	, Selected_Distribution(0)
	, n_Bins(2)
{

}

CSettingsDescriptiveStatistics::~CSettingsDescriptiveStatistics(){
}

void CSettingsDescriptiveStatistics::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_SELECT_DISTRIBUTION, Selected_Distribution);
	DDX_Text(pDX, IDC_EDIT_SELECT_NUMBER_BINS, n_Bins);
	DDV_MinMaxInt(pDX, n_Bins, 2, INT_MAX);
	DDX_Control(pDX, IDC_COMBO_SELECT_DISTRIBUTION, List_Distributions);
}

BOOL CSettingsDescriptiveStatistics::OnInitDialog() {
	CDialogEx::OnInitDialog();
	List_Distributions.AddString(L"1. -> Normal");
	List_Distributions.AddString(L"2. -> Log-normal");
	List_Distributions.AddString(L"3. -> Weibull");
	return true;
}


BEGIN_MESSAGE_MAP(CSettingsDescriptiveStatistics, CDialogEx)
END_MESSAGE_MAP()


// CSettingsDescriptiveStatistics message handlers
