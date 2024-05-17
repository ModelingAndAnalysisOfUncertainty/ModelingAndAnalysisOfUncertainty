// CLinearClassificationMulticlassFDA.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CLinearClassificationMulticlassFDA.h"


// CLinearClassificationMulticlassFDA dialog

IMPLEMENT_DYNAMIC(CLinearClassificationMulticlassFDA, CDialogEx)

CLinearClassificationMulticlassFDA::CLinearClassificationMulticlassFDA(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FDA_MULTICLASS, pParent)
	, FractionTraining(0.8)
	, NumberRuns(100)
	, ValidationMethod(0)
	, ClassificationMethod(0){
}

CLinearClassificationMulticlassFDA::~CLinearClassificationMulticlassFDA(){
}

void CLinearClassificationMulticlassFDA::DoDataExchange(CDataExchange* pDX){
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRACTION_TRAINING_FDA_MULTICLASS, FractionTraining);
	DDV_MinMaxDouble(pDX, FractionTraining, 0.5, 1);
	DDX_Text(pDX, IDC_EDIT_NUMBER_RUNS_FDA_MULTICLASS, NumberRuns);
	DDX_Radio(pDX, IDC_RADIO_TRAIN_VALIDATION_FDA_MULTICLASS, ValidationMethod);
	DDX_Radio(pDX, IDC_RADIO_ONE_HOT_ENCODING, ClassificationMethod);
	DDX_Control(pDX, IDC_STATIC_FRACTION, FractionText);
	DDX_Control(pDX, IDC_STATIC_TEXT_NUMBER_RUNS, NumberRunsText);
	DDX_Control(pDX, IDC_EDIT_FRACTION_TRAINING_FDA_MULTICLASS, FractionInput);
	DDX_Control(pDX, IDC_EDIT_NUMBER_RUNS_FDA_MULTICLASS, NumberRunsInput);
}


BEGIN_MESSAGE_MAP(CLinearClassificationMulticlassFDA, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_TRAIN_VALIDATION_FDA_MULTICLASS, &CLinearClassificationMulticlassFDA::OnBnClickedRadioTrainValidationFdaMulticlass)
	ON_BN_CLICKED(IDC_RADIO_5_FOLD_CROSSVALIDATION_FDA_MULTICLASS, &CLinearClassificationMulticlassFDA::OnBnClickedRadio5FoldCrossvalidationFdaMulticlass)
	ON_BN_CLICKED(IDC_RADIO_10_FOLD_CROSSVALIDATION_FDA_MULTICLASS, &CLinearClassificationMulticlassFDA::OnBnClickedRadio10FoldCrossvalidationFdaMulticlass)
	ON_BN_CLICKED(IDC_RADIO_MONTE_CARLO_FDA_MULTICLASS, &CLinearClassificationMulticlassFDA::OnBnClickedRadioMonteCarloFdaMulticlass)
END_MESSAGE_MAP()


// CLinearClassificationMulticlassFDA message handlers


void CLinearClassificationMulticlassFDA::OnBnClickedRadioTrainValidationFdaMulticlass() {
	FractionText.ShowWindow(SW_SHOW);
	FractionInput.ShowWindow(SW_SHOW);
	NumberRunsText.ShowWindow(SW_HIDE);
	NumberRunsInput.ShowWindow(SW_HIDE);
}


void CLinearClassificationMulticlassFDA::OnBnClickedRadio5FoldCrossvalidationFdaMulticlass(){
	FractionText.ShowWindow(SW_HIDE);
	FractionInput.ShowWindow(SW_HIDE);
	NumberRunsText.ShowWindow(SW_HIDE);
	NumberRunsInput.ShowWindow(SW_HIDE);
}


void CLinearClassificationMulticlassFDA::OnBnClickedRadio10FoldCrossvalidationFdaMulticlass(){
	FractionText.ShowWindow(SW_HIDE);
	FractionInput.ShowWindow(SW_HIDE);
	NumberRunsText.ShowWindow(SW_HIDE);
	NumberRunsInput.ShowWindow(SW_HIDE);
}


void CLinearClassificationMulticlassFDA::OnBnClickedRadioMonteCarloFdaMulticlass(){
	FractionText.ShowWindow(SW_SHOW);
	FractionInput.ShowWindow(SW_SHOW);
	NumberRunsText.ShowWindow(SW_SHOW);
	NumberRunsInput.ShowWindow(SW_SHOW);
}
