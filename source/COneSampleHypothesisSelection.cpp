// COneSampleHypothesisSelectoin.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "COneSampleHypothesisSelection.h"

// COneSampleHypothesisSelection dialog

IMPLEMENT_DYNAMIC(COneSampleHypothesisSelection, CDialogEx)

COneSampleHypothesisSelection::COneSampleHypothesisSelection(CWnd* pParent, CStringArray* Names)
	: CDialogEx(IDD_DIALOG_ONESAMPLE_HYPOTHESIS, pParent)
	, Mean(0)
	, Selection_knownVariance(0)
	, SelectedVariable(0)
	, Variance(1.0)
	, alpha(0.05)
	, OneTwoSided(0)
	, SelectedClass(1)
	, Value_0(1.0)
{
	n_Var = (int)Names->GetSize();
	CString str, Text;
	VarNames.RemoveAll();
	for (int i = 0; i < n_Var; i++) {
		str.Empty();
		Text.Empty();
		if (i < 9) str.Format(L" %d", i + 1);
		else str.Format(L"%d", i + 1);
		Text.Append(str + _T(". Variable -> "));
		str.Empty();
		str.Append(Names->GetAt(i));
		Text.Append(str);
		VarNames.Add(Text);
	}
}

COneSampleHypothesisSelection::~COneSampleHypothesisSelection(){
}

void COneSampleHypothesisSelection::DoDataExchange(CDataExchange* pDX){
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_ONESAMPLEHYPOTHESIS_MEAN, Mean);
	DDV_MinMaxInt(pDX, Mean, 0, INT_MAX);
	DDX_Control(pDX, IDC_COMBO_VARIABLE_LIST, VariableSet);
	DDX_Control(pDX, IDC_STATIC_SPECIFY_VARIANCE, cntr_Show_KnownUnknownVariance);
	DDX_Control(pDX, IDC_STATIC_VARIANCE, cntr_Show_SpecifyVariance);
	DDX_Control(pDX, IDC_RADIO_ONESAMPLE_KNOWN_MEAN, cntr_Show_KnownVariance);
	DDX_Control(pDX, IDC_RADIO_ONESAMPLE_UNKNOWN_MEAN, cntr_Show_UnknownVariance);
	DDX_Control(pDX, IDC_EDIT_ONESAMPLE_VARIANCE, cntr_Show_ShowVariance);
	DDX_Radio(pDX, IDC_RADIO_ONESAMPLE_KNOWN_MEAN, Selection_knownVariance);
	DDX_Radio(pDX, IDC_RADIO_ONESAMPLE_TWO_SIDED, OneTwoSided);
	DDX_Text(pDX, IDC_EDIT_ONESAMPLE_VARIANCE, Variance);
	DDX_Text(pDX, IDC_EDIT_ONESAMPLE_SIGNIFICANCE, alpha);
	DDX_Text(pDX, IDC_EDIT_ONESAMPLE_CLASS, SelectedClass);
	DDX_Text(pDX, IDC_EDIT_ONESAMPLE_HYPOTHESIZED_VALUE, Value_0);
}

BEGIN_MESSAGE_MAP(COneSampleHypothesisSelection, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_ONESAMPLEHYPOTHESIS_MEAN, &COneSampleHypothesisSelection::OnBnClickedRadioOnesamplehypothesisMean)
	ON_BN_CLICKED(IDC_RADIO_ONESAMPLEHYPOTHESIS_VARIANCE, &COneSampleHypothesisSelection::OnBnClickedRadioOnesamplehypothesisVariance)
	ON_BN_CLICKED(IDC_RADIO_ONESAMPLE_KNOWN_MEAN, &COneSampleHypothesisSelection::OnBnClickedRadioOnesampleKnownMean)
	ON_BN_CLICKED(IDC_RADIO_ONESAMPLE_UNKNOWN_MEAN, &COneSampleHypothesisSelection::OnBnClickedRadioOnesampleUnknownMean)
	ON_CBN_SELCHANGE(IDC_COMBO_VARIABLE_LIST, &COneSampleHypothesisSelection::OnCBnSelection)
END_MESSAGE_MAP()

void COneSampleHypothesisSelection::LoadComboBoxEntries() {
	for (int i = 0; i < n_Var; i++) {
		VariableSet.AddString(VarNames.GetAt(i));
	}
}

BOOL COneSampleHypothesisSelection::OnInitDialog() {
	CDialogEx::OnInitDialog();
	LoadComboBoxEntries();
	return true;
}

void COneSampleHypothesisSelection::OnBnClickedRadioOnesamplehypothesisMean(){
	cntr_Show_KnownUnknownVariance.ShowWindow(SW_SHOW);
	cntr_Show_KnownVariance.ShowWindow(SW_SHOW);
	cntr_Show_UnknownVariance.ShowWindow(SW_SHOW);
}

void COneSampleHypothesisSelection::OnBnClickedRadioOnesamplehypothesisVariance(){
	cntr_Show_KnownUnknownVariance.ShowWindow(SW_HIDE);
	cntr_Show_KnownVariance.ShowWindow(SW_HIDE);
	cntr_Show_UnknownVariance.ShowWindow(SW_HIDE);
	cntr_Show_SpecifyVariance.ShowWindow(SW_HIDE);
	cntr_Show_ShowVariance.ShowWindow(SW_HIDE);
}

void COneSampleHypothesisSelection::OnBnClickedRadioOnesampleKnownMean(){
	cntr_Show_SpecifyVariance.ShowWindow(SW_SHOW);
	cntr_Show_ShowVariance.ShowWindow(SW_SHOW);
}

void COneSampleHypothesisSelection::OnBnClickedRadioOnesampleUnknownMean(){
	cntr_Show_SpecifyVariance.ShowWindow(SW_HIDE);
	cntr_Show_ShowVariance.ShowWindow(SW_HIDE);
}

void COneSampleHypothesisSelection::OnCBnSelection() {
	SelectedVariable = VariableSet.GetCurSel();
}