// CTwoSampleHypothesisSelection.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CTwoSampleHypothesisSelection.h"


// CTwoSampleHypothesisSelection dialog

IMPLEMENT_DYNAMIC(CTwoSampleHypothesisSelection, CDialogEx)



CTwoSampleHypothesisSelection::CTwoSampleHypothesisSelection(CWnd* pParent, CStringArray* Names)
	: CDialogEx(IDD_DIALOG_TWOSAMPLE_HYPOTHESIS, pParent)
	, SelectMeanVariance(0)
	, SelectUnknownKnown(0)
	, EnterVariance(1)
	, Significance(0.05)
	, Value_0(0)
	, TwoSided(0)
	, SelectedClass_1(1)
	, VarianceEnter(1)
	, SelectedClass_2(2)
	, SelectedVariable(0)
	
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

CTwoSampleHypothesisSelection::~CTwoSampleHypothesisSelection()
{
}

void CTwoSampleHypothesisSelection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_SELECT_MEAN, SelectMeanVariance);
	DDX_Control(pDX, IDC_RADIO_SELECT_MEAN, ctrl_SelectMean);
	DDX_Control(pDX, IDC_RADIO_SELECT_VARIANCE, ctrl_SelectVariance);
	DDX_Control(pDX, IDC_STATIC_UNKNOWNKNOWN, ctrl_ShowKnownUnknown);
	DDX_Radio(pDX, IDC_RADIO_SELECT_KNOWN, SelectUnknownKnown);
	DDX_Control(pDX, IDC_RADIO_SELECT_KNOWN, ctrl_SelectKnown);
	DDX_Control(pDX, IDC_RADIO_SELECT_UNKNOWN, ctrl_SelectUnknown);
	DDX_Control(pDX, IDC_COMBO_VARIABLE_SET, VariableSet);
	DDX_Text(pDX, IDC_EDIT4_HYPOTHESIZED, Value_0);
	DDX_Radio(pDX, IDC_RADIO_TWO_SIDED, TwoSided);
	DDX_Control(pDX, IDC_STATIC_SHOW_SPECIFY_VARIANCE, ctrl_ShowSpecifyVariance);
	DDX_Control(pDX, IDC_EDIT_KNOWNVARIANCEVALUE, ctrl_EnterVariance);
	DDX_Control(pDX, IDC_STATIC_SHOW_VARIANCE_SPECIFY, ctrl_ShowVarianceSpecify);
	DDX_Text(pDX, IDC_EDIT_VARIANCE_KNOWN, VarianceEnter);
	DDX_Control(pDX, IDC_EDIT_VARIANCE_KNOWN, ctrl_VarianceEnter);
	DDX_Text(pDX, IDC_EDIT_SIGNIFICANCEVALUE, Significance);
	DDX_Text(pDX, IDC_EDIT_CLASSVALUE, SelectedClass_1);
	DDX_Text(pDX, IDC_EDIT_VALUECLASS, SelectedClass_2);
	DDX_Text(pDX, IDC_EDIT_KNOWNVARIANCEVALUE, EnterVariance);
}


BEGIN_MESSAGE_MAP(CTwoSampleHypothesisSelection, CDialogEx)
	
	ON_BN_CLICKED(IDC_RADIO_SELECT_MEAN, &CTwoSampleHypothesisSelection::OnBnClickedRadioSelectMean)
	ON_BN_CLICKED(IDC_RADIO_SELECT_VARIANCE, &CTwoSampleHypothesisSelection::OnBnClickedRadioSelectVariance)
	ON_BN_CLICKED(IDC_RADIO_SELECT_KNOWN, &CTwoSampleHypothesisSelection::OnBnClickedRadioSelectKnown)
	ON_BN_CLICKED(IDC_RADIO_SELECT_UNKNOWN, &CTwoSampleHypothesisSelection::OnBnClickedRadioSelectUnknown)
	ON_CBN_SELCHANGE(IDC_COMBO_VARIABLE_SET, &CTwoSampleHypothesisSelection::OnCbnSelchangeComboVariableSet)
END_MESSAGE_MAP()


// CTwoSampleHypothesisSelection message handlers




void CTwoSampleHypothesisSelection::OnBnClickedRadioSelectMean()
{
	ctrl_ShowKnownUnknown.ShowWindow(SW_SHOW);
	ctrl_SelectKnown.ShowWindow(SW_SHOW);
	ctrl_SelectUnknown.ShowWindow(SW_SHOW);
	ctrl_EnterVariance.ShowWindow(SW_SHOW);
	ctrl_ShowSpecifyVariance.ShowWindow(SW_SHOW);
	ctrl_VarianceEnter.ShowWindow(SW_SHOW);
	ctrl_ShowVarianceSpecify.ShowWindow(SW_SHOW);
}


void CTwoSampleHypothesisSelection::OnBnClickedRadioSelectVariance()
{
	// TODO: Add your control notification handler code here
	ctrl_ShowKnownUnknown.ShowWindow(SW_HIDE);
	ctrl_SelectKnown.ShowWindow(SW_HIDE);
	ctrl_SelectUnknown.ShowWindow(SW_HIDE);

	ctrl_EnterVariance.ShowWindow(SW_HIDE);
	ctrl_ShowSpecifyVariance.ShowWindow(SW_HIDE);

	ctrl_VarianceEnter.ShowWindow(SW_HIDE);
	ctrl_ShowVarianceSpecify.ShowWindow(SW_HIDE);
}


void CTwoSampleHypothesisSelection::OnBnClickedRadioSelectKnown()
{
	ctrl_EnterVariance.ShowWindow(SW_SHOW);
	ctrl_ShowSpecifyVariance.ShowWindow(SW_SHOW);
	ctrl_VarianceEnter.ShowWindow(SW_SHOW);
	ctrl_ShowVarianceSpecify.ShowWindow(SW_SHOW);

}


void CTwoSampleHypothesisSelection::OnBnClickedRadioSelectUnknown()
{
	ctrl_EnterVariance.ShowWindow(SW_HIDE);
	ctrl_ShowSpecifyVariance.ShowWindow(SW_HIDE);
	ctrl_VarianceEnter.ShowWindow(SW_HIDE);
	ctrl_ShowVarianceSpecify.ShowWindow(SW_HIDE);

}



void CTwoSampleHypothesisSelection::OnCbnSelchangeComboVariableSet()
{
	SelectedVariable = VariableSet.GetCurSel();
}

void CTwoSampleHypothesisSelection::LoadComboBoxEntries() {
	for (int i = 0; i < n_Var; i++) {
		VariableSet.AddString(VarNames.GetAt(i));
	}
}

BOOL CTwoSampleHypothesisSelection::OnInitDialog() {
	CDialogEx::OnInitDialog();
	LoadComboBoxEntries();
	return true;
}

