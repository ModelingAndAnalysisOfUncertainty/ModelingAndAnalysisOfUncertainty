// CLinearClassificationSetUp.cpp : implementation file
//

#include "pch.h"
#include "Modeling and Analysis of Uncertainty.h"
#include "afxdialogex.h"
#include "CLinearClassificationBinaryFDA.h"


// CLinearClassificationSetUp dialog

IMPLEMENT_DYNAMIC(CLinearClassificationBinaryFDA, CDialogEx)

CLinearClassificationBinaryFDA::CLinearClassificationBinaryFDA(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_FDA_BINARY, pParent)
	, FractionTraining(0.8)
	, NumberRuns(100)
	, ValidationMethod(0){
}

CLinearClassificationBinaryFDA::~CLinearClassificationBinaryFDA(){
}

void CLinearClassificationBinaryFDA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FRACTION_TRAINING_FDA, FractionTraining);
	DDV_MinMaxDouble(pDX, FractionTraining, 0.5, 1);
	DDX_Text(pDX, IDC_EDIT_NUMBER_RUNS_FDA, NumberRuns);
	DDV_MinMaxInt(pDX, NumberRuns, 1, 1000);
	DDX_Radio(pDX, IDC_RADIO_TRAIN_VALIDATION_FDA, ValidationMethod);
	DDX_Control(pDX, IDC_STATIC_FRACTION, FractionText);
	DDX_Control(pDX, IDC_EDIT_FRACTION_TRAINING_FDA, FractionInput);
	DDX_Control(pDX, IDC_STATIC_TEXT_NUMBER_RUNS, NumberRunsText);
	DDX_Control(pDX, IDC_EDIT_NUMBER_RUNS_FDA, NumberRunsInput);
}


BEGIN_MESSAGE_MAP(CLinearClassificationBinaryFDA, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_TRAIN_VALIDATION_FDA, &CLinearClassificationBinaryFDA::OnBnClickedRadioTrainValidationClassificationLinear)
	ON_BN_CLICKED(IDC_RADIO_5_FOLD_CROSSVALIDATION_FDA, &CLinearClassificationBinaryFDA::OnBnClickedRadio5FoldCrossvalidationClassificationLinear)
	ON_BN_CLICKED(IDC_RADIO_10_FOLD_CROSSVALIDATION_FDA, &CLinearClassificationBinaryFDA::OnBnClickedRadio10FoldCrossvalidationClassificationLinear)
	ON_BN_CLICKED(IDC_RADIO_MONTE_CARLO_FDA, &CLinearClassificationBinaryFDA::OnBnClickedRadioMonteCarloClassificationLinear)
END_MESSAGE_MAP()


// CLinearClassificationSetUp message handlers


void CLinearClassificationBinaryFDA::OnBnClickedRadioTrainValidationClassificationLinear(){
	FractionText.ShowWindow(SW_SHOW);
	FractionInput.ShowWindow(SW_SHOW);
	NumberRunsText.ShowWindow(SW_HIDE);
	NumberRunsInput.ShowWindow(SW_HIDE);
}


void CLinearClassificationBinaryFDA::OnBnClickedRadio5FoldCrossvalidationClassificationLinear(){
	FractionText.ShowWindow(SW_HIDE);
	FractionInput.ShowWindow(SW_HIDE);
	NumberRunsText.ShowWindow(SW_HIDE);
	NumberRunsInput.ShowWindow(SW_HIDE);
}


void CLinearClassificationBinaryFDA::OnBnClickedRadio10FoldCrossvalidationClassificationLinear(){
	FractionText.ShowWindow(SW_HIDE);
	FractionInput.ShowWindow(SW_HIDE);
	NumberRunsText.ShowWindow(SW_HIDE);
	NumberRunsInput.ShowWindow(SW_HIDE);
}


void CLinearClassificationBinaryFDA::OnBnClickedRadioMonteCarloClassificationLinear(){
	FractionText.ShowWindow(SW_SHOW);
	FractionInput.ShowWindow(SW_SHOW);
	NumberRunsText.ShowWindow(SW_SHOW);
	NumberRunsInput.ShowWindow(SW_SHOW);
}
