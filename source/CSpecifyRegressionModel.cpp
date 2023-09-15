// CSpecifyRegressionModel.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CSpecifyRegressionModel.h"

// CSpecifyRegressionModel dialog

IMPLEMENT_DYNAMIC(CSpecifyRegressionModel, CDialogEx)

CSpecifyRegressionModel::CSpecifyRegressionModel(CWnd* pParent, CStringArray* Names)
	: CDialogEx(IDD_DIALOG_MULTIPLE_REGRESSION, pParent)
	, ValidationMethod(0)
	, alpha(0.05)
	, FactorAnalysis(0)
	, NumberRuns(100)
	, PercentageTestingData(5)
{
	n_Var = (int)Names->GetSize();
	CString str, Text;
	VariableList.RemoveAll();
	for (int i = 0; i < n_Var - 1; i++) {
		str.Empty();
		Text.Empty();
		if (i < 9) str.Format(L" %d", i + 1);
		else str.Format(L"%d", i + 1);
		Text.Append(str + _T(". Variable -> "));
		str.Empty();
		str.Append(Names->GetAt(i));
		Text.Append(str);
		VariableList.Add(Text);
	}
}

CSpecifyRegressionModel::~CSpecifyRegressionModel(){
}

void CSpecifyRegressionModel::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_REGRESSION_LOO_CV_ASSESSMENT, ValidationMethod);
	DDX_Control(pDX, IDC_LIST_REGRESSION_AVAILABLE_REGRESSORS, AvailableRegressorList);
	DDX_Control(pDX, IDC_LIST_REGRESSION_SELECTED_REGRESSORS, SelectedRegressorList);
	DDX_Control(pDX, IDC_BUTTON_SELECT_PREDICTORS, AddVariables);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_PREDICTORS, RemoveVariables);
	DDX_Control(pDX, IDC_BUTTON_SELECT_ALL_PREDICTORS, AddAllVariables);
	DDX_Control(pDX, IDC_BUTTON_REMOVE_ALL_PREDICTORS, RemoveAllVariables);
	DDX_Text(pDX, IDC_EDIT_SELECT_SIGNIFICANCE, alpha);
	DDX_Radio(pDX, IDC_RADIO_FACTOR_ANALYSIS_YES, FactorAnalysis);
	DDX_Control(pDX, IDC_STATIC_TEXT_SIGNFICANCE, cntr_ShowSignificanceText);
	DDX_Control(pDX, IDC_EDIT_SELECT_SIGNIFICANCE, cntr_ShowSignificanceSelection);
	DDX_Text(pDX, IDC_EDIT_NUMBER_RUNS, NumberRuns);
	DDV_MinMaxInt(pDX, NumberRuns, 1, INT_MAX);
	DDX_Control(pDX, IDC_EDIT_NUMBER_RUNS, cntr_ShowNumberRuns);
	DDX_Control(pDX, IDC_STATIC_NUMBER_RUNS, cntr_ShowNumberRunsText);
	DDX_Text(pDX, IDC_EDIT_PERCENTAGE_TESTING_DATA, PercentageTestingData);
	DDV_MinMaxDouble(pDX, PercentageTestingData, 1, DBL_MAX);
	DDX_Control(pDX, IDC_EDIT_PERCENTAGE_TESTING_DATA, cntr_ShowPercentageTestData);
	DDX_Control(pDX, IDC_STATIC_PERCENTAGE_TESTING_DATA, cntr_ShowPercentageTestDataText);
}

BEGIN_MESSAGE_MAP(CSpecifyRegressionModel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PREDICTORS, &CSpecifyRegressionModel::OnBnClickedButtonSelectPredictors)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_PREDICTORS, &CSpecifyRegressionModel::OnBnClickedButtonRemovePredictors)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL_PREDICTORS, &CSpecifyRegressionModel::OnBnClickedButtonSelectAllPredictors)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_ALL_PREDICTORS, &CSpecifyRegressionModel::OnBnClickedButtonRemoveAllPredictors)
	ON_BN_CLICKED(IDC_RADIO_REGRESSION_MONTE_CARLO, &CSpecifyRegressionModel::OnBnClickedRadioRegressionMonteCarlo)
	ON_BN_CLICKED(IDC_RADIO_REGRESSION_LOO_CV_ASSESSMENT, &CSpecifyRegressionModel::OnBnClickedRadioRegressionLooCvAssessment)
	ON_BN_CLICKED(IDC_RADIO_REGRESSION_10_FOLD_CV, &CSpecifyRegressionModel::OnBnClickedRadioRegression10FoldCv)
	ON_BN_CLICKED(IDC_RADIO_REGRESSION_5_FOLD_CV, &CSpecifyRegressionModel::OnBnClickedRadioRegression5FoldCv)
	ON_BN_CLICKED(IDOK, &CSpecifyRegressionModel::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_FACTOR_ANALYSIS_YES, &CSpecifyRegressionModel::OnBnClickedRadioFactorAnalysisYes)
	ON_BN_CLICKED(IDC_RADIO_FACTOR_ANALYSIS_NO, &CSpecifyRegressionModel::OnBnClickedRadioFactorAnalysisNo)
END_MESSAGE_MAP()

void CSpecifyRegressionModel::LoadListBoxEntries() {
	for (int i = 0; i < n_Var -1; i++) {
		AvailableRegressorList.AddString(VariableList.GetAt(i));
	}
}

BOOL CSpecifyRegressionModel::OnInitDialog() {
	CDialogEx::OnInitDialog();
	LoadListBoxEntries();
	return true;
}

// CSpecifyRegressionModel message handlers

void CSpecifyRegressionModel::OnBnClickedButtonSelectPredictors(){
	int SelectedRegressor = AvailableRegressorList.GetCurSel();
	if (SelectedRegressor != LB_ERR) {
		CString Text;
		AvailableRegressorList.GetText(SelectedRegressor, Text);
		SelectedRegressorList.AddString(Text);
		AvailableRegressorList.DeleteString(SelectedRegressor);
	}
}

void CSpecifyRegressionModel::OnBnClickedButtonRemovePredictors(){
	int SelectedRegressor = SelectedRegressorList.GetCurSel();
	if (SelectedRegressor != LB_ERR) {
		CString Text;
		SelectedRegressorList.GetText(SelectedRegressor, Text);
		AvailableRegressorList.AddString(Text);
		SelectedRegressorList.DeleteString(SelectedRegressor);
	}
}

void CSpecifyRegressionModel::OnBnClickedButtonSelectAllPredictors(){
	CString Text;
	for (int i = 0; i < AvailableRegressorList.GetCount(); i++) {
		AvailableRegressorList.GetText(i, Text);
		SelectedRegressorList.AddString(Text);
	}
	for (int i = AvailableRegressorList.GetCount() - 1; i >= 0; i--) AvailableRegressorList.DeleteString(i);
}

void CSpecifyRegressionModel::OnBnClickedButtonRemoveAllPredictors(){
	CString Text;
	for (int i = 0; i < SelectedRegressorList.GetCount(); i++) {
		SelectedRegressorList.GetText(i, Text);
		AvailableRegressorList.AddString(Text);
	}
	for (int i = SelectedRegressorList.GetCount() - 1; i >= 0; i--) SelectedRegressorList.DeleteString(i);
}

void CSpecifyRegressionModel::OnBnClickedRadioRegressionMonteCarlo(){
	cntr_ShowNumberRunsText.ShowWindow(SW_SHOW);
	cntr_ShowNumberRuns.ShowWindow(SW_SHOW);
	cntr_ShowPercentageTestDataText.ShowWindow(SW_SHOW);
	cntr_ShowPercentageTestData.ShowWindow(SW_SHOW);
}

void CSpecifyRegressionModel::OnBnClickedRadioRegressionLooCvAssessment(){
	cntr_ShowNumberRunsText.ShowWindow(SW_HIDE);
	cntr_ShowNumberRuns.ShowWindow(SW_HIDE);
	cntr_ShowPercentageTestDataText.ShowWindow(SW_HIDE);
	cntr_ShowPercentageTestData.ShowWindow(SW_HIDE);
}

void CSpecifyRegressionModel::OnBnClickedRadioRegression10FoldCv(){
	cntr_ShowNumberRunsText.ShowWindow(SW_HIDE);
	cntr_ShowNumberRuns.ShowWindow(SW_HIDE);
	cntr_ShowPercentageTestDataText.ShowWindow(SW_HIDE);
	cntr_ShowPercentageTestData.ShowWindow(SW_HIDE);
}


void CSpecifyRegressionModel::OnBnClickedRadioRegression5FoldCv(){
	cntr_ShowNumberRunsText.ShowWindow(SW_HIDE);
	cntr_ShowNumberRuns.ShowWindow(SW_HIDE);
	cntr_ShowPercentageTestDataText.ShowWindow(SW_HIDE);
	cntr_ShowPercentageTestData.ShowWindow(SW_HIDE);
}

void CSpecifyRegressionModel::OnBnClickedOk(){
	CDialogEx::OnOK();
	SelectedPredictors.RemoveAll();
	CString Predictor, Item;
	int j = 0;
	Predictor.Empty();
	SelectedRegressorList.GetText(j, Predictor);
	for (int i = 0; i < VariableList.GetSize(); i++) {
		Item.Empty();
		Item.Append(VariableList.GetAt(i));
		if (Predictor == Item) {
			j++;
			SelectedPredictors.Add(i);
			if (j < SelectedRegressorList.GetCount()) {
				Predictor.Empty();
				SelectedRegressorList.GetText(j, Predictor);
			}
		}
	}
}

void CSpecifyRegressionModel::OnBnClickedRadioFactorAnalysisYes(){
	cntr_ShowSignificanceText.ShowWindow(SW_SHOW);
	cntr_ShowSignificanceSelection.ShowWindow(SW_SHOW);
}

void CSpecifyRegressionModel::OnBnClickedRadioFactorAnalysisNo(){
	cntr_ShowSignificanceText.ShowWindow(SW_HIDE);
	cntr_ShowSignificanceSelection.ShowWindow(SW_HIDE);
}